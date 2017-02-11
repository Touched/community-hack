import os.path
from collections import OrderedDict, namedtuple
import sys
import shutil

Rule = namedtuple('Rule', ('action', 'dependents'))

def asset_process(id, path):
    return 'pokedex/assets/{}_{}'.format(id, os.path.basename(path))


def link(src, dst):
    src = os.path.abspath(src)

    try:
        if os.path.lexists(dst):
            if os.readlink(dst) == src:
                return
            else:
                os.unlink(dst)

        os.symlink(src, dst)
    except NotImplementedError:
        shutil.copyfile(src, dst)


def link_assets(config, output):
    pokedex = config.get_table('pokedex')

    os.makedirs('pokedex/assets', exist_ok=True)

    for id in config.pokedex:
        assets = ['cry.aif', 'sprite.png', 'icon.png']

        for asset in assets:
            src = config.get_asset(id, asset)

            if src == None:
                continue

            dst = asset_process(id, src)

            link(src, dst)
