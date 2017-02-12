import math
import os
import operator
import itertools
import unicodedata
import string
import re
import cgen
from PIL import Image

# A list of species already in the game
existing_species = {
    'abra': 63, 'absol': 376, 'aerodactyl': 142, 'aggron': 384, 'aipom': 190,
    'alakazam': 65, 'altaria': 359, 'ampharos': 181, 'anorith': 390, 'arbok': 24,
    'arcanine': 59, 'ariados': 168, 'armaldo': 391, 'aron': 382, 'articuno': 144,
    'azumarill': 184, 'azurill': 350, 'bagon': 395, 'baltoy': 318, 'banette': 378,
    'barboach': 323, 'bayleef': 153, 'beautifly': 292, 'beedrill': 15, 'beldum': 398,
    'bellossom': 182, 'bellsprout': 69, 'blastoise': 9, 'blaziken': 282, 'blissey': 242,
    'breloom': 307, 'bulbasaur': 1, 'butterfree': 12, 'cacnea': 344, 'cacturne': 345,
    'camerupt': 340, 'carvanha': 330, 'cascoon': 293, 'castform': 385, 'caterpie': 10,
    'celebi': 251, 'chansey': 113, 'charizard': 6, 'charmander': 4, 'charmeleon': 5,
    'chikorita': 152, 'chimecho': 411, 'chinchou': 170, 'clamperl': 373, 'claydol': 319,
    'clefable': 36, 'clefairy': 35, 'cleffa': 173, 'cloyster': 91, 'combusken': 281,
    'corphish': 326, 'corsola': 222, 'cradily': 389, 'crawdaunt': 327, 'crobat': 169,
    'croconaw': 159, 'cubone': 104, 'cyndaquil': 155, 'delcatty': 316, 'delibird': 225,
    'deoxys': 410, 'dewgong': 87, 'diglett': 50, 'ditto': 132, 'dodrio': 85,
    'doduo': 84, 'donphan': 232, 'dragonair': 148, 'dragonite': 149,
    'dratini': 147, 'drowzee': 96, 'dugtrio': 51, 'dunsparce': 206,
    'dusclops': 362, 'duskull': 361, 'dustox': 294, 'eevee': 133,
    'ekans': 23, 'electabuzz': 125, 'electrike': 337, 'electrode': 101, 'elekid': 239,
    'entei': 244, 'espeon': 196, 'exeggcute': 102, 'exeggutor': 103, 'exploud': 372,
    'farfetchd': 83, 'fearow': 22, 'feebas': 328, 'feraligatr': 160, 'flaaffy': 180,
    'flareon': 136, 'flygon': 334, 'forretress': 205, 'furret': 162, 'gardevoir': 394,
    'gastly': 92, 'gengar': 94, 'geodude': 74, 'girafarig': 203, 'glalie': 347,
    'gligar': 207, 'gloom': 44, 'golbat': 42, 'goldeen': 118, 'golduck': 55,
    'golem': 76, 'gorebyss': 375, 'granbull': 210, 'graveler': 75,
    'grimer': 88, 'groudon': 405, 'grovyle': 278, 'growlithe': 58, 'grumpig': 352,
    'gulpin': 367, 'gyarados': 130, 'hariyama': 336, 'haunter': 93, 'heracross': 214,
    'hitmonchan': 107, 'hitmonlee': 106, 'hitmontop': 237, 'hooh': 250, 'hoothoot': 163,
    'hoppip': 187, 'horsea': 116, 'houndoom': 229, 'houndour': 228, 'huntail': 374,
    'hypno': 97, 'igglybuff': 174, 'illumise': 387, 'ivysaur': 2, 'jigglypuff': 39,
    'jirachi': 409, 'jolteon': 135, 'jumpluff': 189, 'jynx': 124, 'kabuto': 140,
    'kabutops': 141, 'kadabra': 64, 'kakuna': 14, 'kangaskhan': 115, 'kecleon': 317,
    'kingdra': 230, 'kingler': 99, 'kirlia': 393, 'koffing': 109, 'krabby': 98,
    'kyogre': 404, 'lairon': 383, 'lanturn': 171, 'lapras': 131, 'larvitar': 246,
    'latias': 407, 'latios': 408, 'ledian': 166, 'ledyba': 165, 'lickitung': 108,
    'lileep': 388, 'linoone': 289, 'lombre': 296, 'lotad': 295, 'loudred': 371,
    'ludicolo': 297, 'lugia': 249, 'lunatone': 348, 'luvdisc': 325,
    'machamp': 68, 'machoke': 67, 'machop': 66, 'magby': 240, 'magcargo': 219,
    'magikarp': 129, 'magmar': 126, 'magnemite': 81, 'magneton': 82, 'makuhita': 335,
    'manectric': 338, 'mankey': 56, 'mantine': 226, 'mareep': 179, 'marill': 183,
    'marowak': 105, 'marshtomp': 284, 'masquerain': 312, 'mawile': 355, 'medicham': 357,
    'meditite': 356, 'meganium': 154, 'meowth': 52, 'metagross': 400,
    'metang': 399, 'metapod': 11, 'mew': 151, 'mewtwo': 150, 'mightyena': 287,
    'milotic': 329, 'miltank': 241, 'minun': 354, 'misdreavus': 200,
    'moltres': 146, 'mrmime': 122, 'mudkip': 283, 'muk': 89,
    'murkrow': 198, 'natu': 177, 'nidoking': 34, 'nidoqueen': 31, 'nidoranf': 29,
    'nidoranm': 32, 'nidorina': 30, 'nidorino': 33, 'nincada': 301, 'ninetales': 38,
    'ninjask': 302, 'noctowl': 164, 'nosepass': 320, 'numel': 339, 'nuzleaf': 299,
    'octillery': 224, 'oddish': 43, 'omanyte': 138, 'omastar': 139, 'onix': 95,
    'paras': 46, 'parasect': 47, 'pelipper': 310, 'persian': 53, 'phanpy': 231,
    'pichu': 172, 'pidgeot': 18, 'pidgeotto': 17, 'pidgey': 16, 'pikachu': 25,
    'piloswine': 221, 'pineco': 204, 'pinsir': 127, 'plusle': 353,
    'politoed': 186, 'poliwag': 60, 'poliwhirl': 61, 'poliwrath': 62,
    'ponyta': 77, 'poochyena': 286, 'porygon': 137, 'porygon2': 233,
    'primeape': 57, 'psyduck': 54, 'pupitar': 247, 'quagsire': 195, 'quilava': 156,
    'qwilfish': 211, 'raichu': 26, 'raikou': 243, 'ralts': 392, 'rapidash': 78,
    'raticate': 20, 'rattata': 19, 'rayquaza': 406, 'regice': 402, 'regirock': 401,
    'registeel': 403, 'relicanth': 381, 'remoraid': 223, 'rhydon': 112, 'rhyhorn': 111,
    'roselia': 363, 'sableye': 322, 'salamence': 397, 'sandshrew': 27, 'sandslash': 28,
    'sceptile': 279, 'scizor': 212, 'scyther': 123, 'seadra': 117, 'seaking': 119,
    'sealeo': 342, 'seedot': 298, 'seel': 86, 'sentret': 161,
    'seviper': 379, 'sharpedo': 331, 'shedinja': 303, 'shelgon': 396, 'shellder': 90,
    'shiftry': 300, 'shroomish': 306, 'shuckle': 213, 'shuppet': 377, 'silcoon': 291,
    'skarmory': 227, 'skiploom': 188, 'skitty': 315, 'slaking': 366, 'slakoth': 364,
    'slowbro': 80, 'slowking': 199, 'slowpoke': 79, 'slugma': 218, 'smeargle': 235,
    'smoochum': 238, 'sneasel': 215, 'snorlax': 143, 'snorunt': 346, 'snubbull': 209,
    'solrock': 349, 'spearow': 21, 'spheal': 341, 'spinarak': 167,
    'spinda': 308, 'spoink': 351, 'squirtle': 7, 'stantler': 234,
    'starmie': 121, 'staryu': 120, 'steelix': 208, 'sudowoodo': 185,
    'suicune': 245, 'sunflora': 192, 'sunkern': 191, 'surskit': 311, 'swablu': 358,
    'swalot': 368, 'swampert': 285, 'swellow': 305, 'swinub': 220, 'taillow': 304,
    'tangela': 114, 'tauros': 128, 'teddiursa': 216, 'tentacool': 72, 'tentacruel': 73,
    'togepi': 175, 'togetic': 176, 'torchic': 280, 'torkoal': 321, 'totodile': 158,
    'trapinch': 332, 'treecko': 277, 'tropius': 369, 'typhlosion': 157, 'tyranitar': 248,
    'tyrogue': 236, 'umbreon': 197, 'unown': 201, 'ursaring': 217, 'vaporeon': 134,
    'venomoth': 49, 'venonat': 48, 'venusaur': 3, 'vibrava': 333, 'victreebel': 71,
    'vigoroth': 365, 'vileplume': 45, 'volbeat': 386, 'voltorb': 100,
    'vulpix': 37, 'wailmer': 313, 'wailord': 314, 'walrein': 343, 'wartortle': 8,
    'weedle': 13, 'weepinbell': 70, 'weezing': 110, 'whiscash': 324, 'whismur': 370,
    'wigglytuff': 40, 'wingull': 309, 'wobbuffet': 202, 'wooper': 194, 'wurmple': 290,
    'wynaut': 360, 'xatu': 178, 'yanma': 193, 'zangoose': 380, 'zapdos': 145,
    'zigzagoon': 288, 'zubat': 41,
}

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
    constant = constant.translate({ ord(c): None for c in string.punctuation + '’' })
    constant = re.sub(r'[\s]+', '_', constant)

    return prefix + constant

def base_form_id(pokedex, id):
    for base_id, entry in pokedex.items():
        if id in entry.get('formes', []):
            return base_id

    # This ID is not a forme, so it must be the base ID
    return id


def base_form_name(pokedex, id):
    return pokedex[base_form_id(pokedex, id)]['name']['english']


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

    # TODO: Fix the stats naming convention in pokedex-data
    def translate_stats(stats):
        return {
            'atk': stats['atk'],
            'def': stats['def'],
            'spd': stats['spe'],
            'spdef': stats['spd'],
            'spatk': stats['spa'],
        }

    abilities = [base['abilities'].get('primary'), base['abilities'].get('secondary')]

    return cgen.coerce({
        **translate_stats(base['stats']),
        'types': padlist(constant_list('TYPE_', base['types']), 2),
        'egg_groups': padlist(constant_list('EGG_GROUP_', base['egg_groups']), 2),
        'abilities': padlist(enum_list(enums['abilities'], abilities), 2),
        'items': padlist(enum_list(enums['items'], base['items']), 2),

        'exp_curve': exp_curves[base['exp_curve']],
        'gender_ratio': 255 - int((base['gender_ratio'] or 0) * 256),
        'color': cgen.CConstant(to_constant('POKEMON_COLOR_', pokedex['color'])),

        'catch_rate': base['catch_rate'],
        'base_friendship': base['base_friendship'],
        'escape_rate': base['escape_rate'],
        'ev_yield': base['ev_yield'],
        'hatch_cycles': base['hatch_cycles'],

        # TODO: Adjust for Gen III
        'exp_yield': 0,

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


def build_species_enum(config, pokedex, file):
    print('#ifndef GENERATED_POKEDEX_SPECIES_H', file=file)
    print('#define GENERATED_POKEDEX_SPECIES_H', file=file)

    for name, index in existing_species.items():
        enum = to_constant('SPECIES_', name)

        if name in pokedex:
            value = str(index)
        else:
            value = '_Pragma("GCC warning \\"Pokémon \'{}` has been ' \
                    'removed from the Pokédex\\"") SPECIES_MISSINGNO'.format(name)

        print('#define', to_constant('SPECIES_', name), value, file=file)

    # Add the new species/formes
    last_index = max(data['index'] for data in pokedex.values())

    def species_and_formes():
        for species, data in pokedex.items():
            yield (species, data['index'])

    # Sort new species by index number
    new_species = sorted(
        filter(lambda entry: entry[0] not in existing_species, species_and_formes()),
        key=lambda entry: entry[1],
    )

    last_existing_index = max(existing_species.values())

    for index, entry in enumerate(new_species, start=last_existing_index + 1):
        name, _ = entry
        print('#define', to_constant('SPECIES_', name), index, file=file)

    print('#define SPECIES_MAX', index + 1, file=file)
    print('#endif /* GENERATED_POKEDEX_SPECIES_H */', file=file)

def generate_tables(config, output):
    pokedex = config.get_table('pokedex')
    abilities = config.get_table('abilities')
    base_stats = config.get_table('base')
    items = config.get_table('items')
    learnsets = config.get_table('learnsets')
    moves = config.get_table('moves')

    enums = {
        'pokemon': {},
        'abilities': {},
        'items': {},
        'moves': {},
    }

    def get_learnset(id, type, generation=7):
        return [move for move in learnsets.get(id) or []
                if move['type'] == type and move['generation'] == generation]


    def build_enum(it, key, prefix, builder, null=None):
        for id, value in it.items():
            name = to_constant(prefix, builder(id, value))

            if name in enums[key].values():
                raise RuntimeError('Duplicate enum value')

            enums[key][id] = name

        if null:
            nullvalue = to_constant(prefix, null)

            if nullvalue in enums[key].values():
                raise RuntimeError('Could define null value')

            enums[key][None] = nullvalue

    build_enum(pokedex, 'pokemon', 'SPECIES_', lambda id, x: id)
    build_enum(
        abilities,
        'abilities',
        'ABILITY_',
        lambda id, x: x.get('name')['english'], null='none'
    )
    build_enum(items, 'items', 'ITEM_', lambda id, x: x.get('name')['english'], null='none')
    build_enum(moves, 'moves', 'MOVE_', lambda id, x: x.get('name')['english'], null='none')

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
        'names': cgen.CFile(
            cgen.CVariableDefinition(
                'const pchar expanded_pokemon_names[SPECIES_MAX][POKEAGB_POKEMON_NAME_LENGTH]',
                cgen.CArray(mapenum(
                    lambda k, v: cgen.CString(
                        getter(v, 'name', 'english', default=base_form_name(pokedex, k))
                    ),
                    enums['pokemon'],
                    pokedex,
                )),
            ),
        ),
        'base_stats': cgen.CFile(
            cgen.CVariableDefinition(
                'const struct PokemonBaseStat expanded_pokemon_base_stats[SPECIES_MAX]',
                cgen.CArray(mapenum(
                    lambda key, value: get_base_stats(enums, value, pokedex[key]),
                    enums['pokemon'],
                    base_stats,
                )),
            )
        ),
        'sprites': cgen.CFile(
            grit_table(
                'const struct SpriteTiles expanded_pokemon_front_sprites[SPECIES_MAX]',
                'front',
                'Tiles',
            ),
            grit_table(
                'const struct SpriteTiles expanded_pokemon_back_sprites[SPECIES_MAX]',
                'back',
                'Tiles',
            ),
            grit_table(
                'const struct SpriteTiles expanded_pokemon_front_female_sprites[SPECIES_MAX]',
                'female_front',
                'Tiles',
            ),
            grit_table(
                'const struct SpriteTiles expanded_pokemon_back_female_sprites[SPECIES_MAX]',
                'female_back',
                'Tiles',
            ),
            grit_table(
                'const struct SpritePalette expanded_pokemon_normal_palettes[SPECIES_MAX]',
                'normal',
                'Pal',
            ),
            grit_table(
                'const struct SpritePalette expanded_pokemon_shiny_palettes[SPECIES_MAX]',
                'shiny',
                'Pal',
            ),
        ),
        'icons': cgen.CFile(
            cgen.CVariableDefinition(
                'const void* expanded_pokemon_icons[SPECIES_MAX]',
                cgen.CArray(mapenum(
                    lambda key, value: cgen.CExternalVariable(
                        'const void* pokedex_assets_{}_iconTiles'.format(key),
                        '&pokedex_assets_{}_iconTiles'.format(key)
                    ),
                    enums['pokemon'],
                    base_stats,
                )),
            ),

            # TODO: Determine correct icon palette
            cgen.CVariableDefinition(
                'const u8 expanded_pokemon_icon_palettes[SPECIES_MAX]',
                cgen.CArray(mapenum(
                    lambda key, value: cgen.CConstant(0),
                    enums['pokemon'],
                    base_stats,
                )),
            ),
        ),
        'learnset': cgen.CFile(
            # Build level up moves
            cgen.CVariableDefinition(
                'const struct ExtendedLevelUpMove* expanded_pokemon_level_up_moves[SPECIES_MAX]',
                cgen.CArray(mapenum(
                    lambda key, value: cgen.CDefinedVariable(
                        cgen.CVariableDefinition(
                            'static const struct ExtendedLevelUpMove {}_level_up[]'.format(key),
                            cgen.CArray(map(
                                lambda move: cgen.CStructure([
                                    cgen.CConstant(enums['moves'][move['id']]),
                                    cgen.CConstant(move['level'])
                                ]),

                                # Level up moves in order of learning with sentinel
                                sorted(
                                    [*get_learnset(key, 'level', 7), {
                                        'level': 255,
                                        'id': None,
                                    }],
                                    key=lambda move: move['level']
                                ),
                            )),
                        ),
                        '{}_level_up'.format(key)
                    ),
                    enums['pokemon'],
                    base_stats,
                )),
            ),
        ),
    }

    os.makedirs(os.path.join(output, 'tables'), exist_ok=True)
    for name, table in tables.items():
       with open(os.path.join(output, 'tables', '{}.c'.format(name)), 'w') as outfile:
          print('#include <pokeagb/pokeagb.h>', file=outfile)
          print('#include "species.h"', file=outfile)
          print('#include "moves.h"', file=outfile)
          print('#include "pokedex_expansion/structures.h"', file=outfile)
          outfile.write(table.generate_file())

    # Build species enum
    with open(os.path.join(output, 'tables', 'species.h'), 'w') as outfile:
        build_species_enum(config, pokedex, outfile)


    with open(os.path.join(output, 'tables', 'moves.h'), 'w') as outfile:
        print('#ifndef GENERATED_POKEDEX_MOVES_H', file=outfile)
        print('#define GENERATED_POKEDEX_MOVES_H', file=outfile)
        for id, name in enums['moves'].items():
            if id == None:
                continue

            print('#define', name, moves[id]['index'], file=outfile)

        print('#endif /* GENERATED_POKEDEX_MOVES_H */', file=outfile)


    # notes:
    # - Define the extra enum values as preprocessor macros to avoid redefining the type
    # - Use weak symbols to override the tables
    # - Use _Pragma in a macro to undefine an enum value
