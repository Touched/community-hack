import os
import operator
import itertools
import unicodedata
import string
import re
import cgen
from PIL import Image

def strip_accents(s):
   return ''.join(c for c in unicodedata.normalize('NFD', s)
                  if unicodedata.category(c) != 'Mn')


def getter(value, *path, default=None):
    for key in path:
        value = value.get(key)

        if value == None:
            break

    return default if value == None else value


def to_constant(prefix, value):
    constant = strip_accents(value).upper()
    constant = constant.translate({ ord(c): None for c in string.punctuation })
    constant = re.sub(r'[\s]+', '_', constant)

    return prefix + constant


def base_form_name(pokedex, id):
    # TODO
    return '-'


def padlist(l, length, value=None):
    if value == None:
        value = l[0]

    return l + [value] * (length - len(l))


def constant_list(prefix, l):
    return list(map(lambda x: cgen.CConstant(to_constant(prefix, x)), l))


def enum_list(enum, l):
    return list(map(lambda x: cgen.CConstant(enum[x]), l))


def get_base_stats(enums, base, pokedex):
    exp_curves = {
        0: cgen.CConstant('EXP_MEDIUM_FAST'),
        1: cgen.CConstant('EXP_ERRATIC'),
        2: cgen.CConstant('EXP_FLUCTUATING'),
        3: cgen.CConstant('EXP_MEDIUM_SLOW'),
        4: cgen.CConstant('EXP_FAST'),
        5: cgen.CConstant('EXP_SLOW'),
    }

    abilities = [base['abilities'].get('primary'), base['abilities'].get('secondary')]

    return cgen.coerce({
        **base['stats'],
        'types': padlist(constant_list('TYPE_', base['types']), 2),
        'egg_groups': padlist(constant_list('EGG_GROUP_', base['egg_groups']), 2),
        'abilities': padlist(enum_list(enums['abilities'], abilities), 2),
        'items': padlist(enum_list(enums['items'], base['items']), 2),

        'exp_curve': exp_curves[base['exp_curve']],
        'gender_ratio': 255 - int(base['gender_ratio'] * 256),
        'color': cgen.CConstant(to_constant('POKEMON_COLOR_', pokedex['color'])),

        'catch_rate': base['catch_rate'],
        'base_friendship': base['base_friendship'],
        'escape_rate': base['escape_rate'],
        'ev_yield': base['ev_yield'],
        'hatch_cycles': base['hatch_cycles'],

        # TODO: Adjust for Gen III
        'base_exp': 0, # base['base_exp'],

        # TODO: Hidden abilities
    })


def grit_entry(config, asset, label, id):
   name = '{}_sprite_{}{}'.format(id, label, asset)
   variable = 'void* {}'.format(name)
   pointer = cgen.CExternalVariable(variable, '&' + name)

   if label.startswith('female_'):
      asset = config.get_asset(id, 'sprite.png')
      w, h = Image.open(asset).size

      if h <= 64:
         pointer = cgen.CConstant('NULL')

   if asset == 'Tiles':
      return cgen.CStructure([
         pointer,
         cgen.CConstant(0x800),
         cgen.CConstant(1),
      ])
   else:
      return cgen.CStructure([
         pointer,
         cgen.CConstant(1),
      ])


def generate_tables(config, output):
    pokedex = config.get_table('pokedex')
    abilities = config.get_table('abilities')
    base_stats = config.get_table('base')
    items = config.get_table('items')

    enums = {
        'pokemon': {},
        'abilities': {},
        'items': {},
    }

    def build_enum(it, key, prefix, builder, null=None):
        for id, value in it.items():
            name = to_constant(prefix, builder(value))

            if name in enums[key].values():
                raise RuntimeError('Duplicate enum value')

            enums[key][id] = name

        if null:
            nullvalue = to_constant(prefix, null)

            if nullvalue in enums[key].values():
                raise RuntimeError('Could define null value')

            enums[key][None] = nullvalue

    build_enum(pokedex, 'pokemon', 'SPECIES_', lambda x: x.get('name', x.get('forme'))['english'])
    build_enum(abilities, 'abilities', 'ABILITY_', lambda x: x.get('name')['english'], null='none')
    build_enum(items, 'items', 'ITEM_', lambda x: x.get('name')['english'], null='none')

    def mapenum(fn, enum, d):
        f = lambda key, value: (cgen.CConstant(enum[key]), fn(key, value))

        return list(itertools.starmap(f, d.items()))


    def grit_table(variable, label, asset):
       return cgen.CVariableDefinition(
          variable,
          cgen.CArray(mapenum(
             lambda key, value: grit_entry(config, asset, label, key),
             enums['pokemon'],
             enums['pokemon'],
          )),
       )


    tables = {
        # 'names': cgen.CArray(mapenum(
        #     lambda k, v: cgen.CString(
        #        getter(v, 'name', 'english', default=base_form_name(pokedex, k))
        #     ),
        #     enums['pokemon'],
        #     pokedex,
        # )),

        # 'base_stats': cgen.CArray(mapenum(
        #     lambda key, value: get_base_stats(enums, value, pokedex[key]),
        #     enums['pokemon'],
        #     base_stats,
        # )),

       'sprites': cgen.CFile(
          grit_table(
             'const struct SpriteTiles pkmn_front_spritez[SPECIES_MAX]',
             'front',
             'Tiles',
          ),
          grit_table(
             'const struct SpriteTiles pkmn_back_sprites[SPECIES_MAX]',
             'back',
             'Tiles',
          ),
          grit_table(
             'const struct SpriteTiles pkmn_front_female_sprites[SPECIES_MAX]',
             'female_front',
             'Tiles',
          ),
          grit_table(
             'const struct SpriteTiles pkmn_back_female_sprites[SPECIES_MAX]',
             'female_back',
             'Tiles',
          ),
          grit_table(
             'const struct SpritePalette pkmn_normal_palettes[SPECIES_MAX]',
             'normal',
             'Pal',
          ),
          grit_table(
             'const struct SpritePalette pkmn_shiny_palettes[SPECIES_MAX]',
             'shiny',
             'Pal',
          ),
       ),
    }

    os.makedirs(os.path.join(output, 'tables'), exist_ok=True)
    for name, table in tables.items():
       with open(os.path.join(output, 'tables', '{}.c'.format(name)), 'w') as outfile:
          print('#include <pokeagb/pokeagb.h>', file=outfile)
          outfile.write(table.generate_file())

    # notes:
    # - Define the extra enum values as preprocessor macros to avoid redefining the type
    # - Use weak symbols to override the tables
    # - Use _Pragma in a macro to undefine an enum value
