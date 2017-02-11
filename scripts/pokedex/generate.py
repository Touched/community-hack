#!/usr/bin/env python3

import json
import sys
import os.path
from tables import generate_tables
from assets import link_assets

def open_json(path):
    with open(path) as fd:
        return json.load(fd)


def dict_merge(*dicts):
    result = {}

    for d in dicts:
        result.update(d)

    return result


class Config:
    """
    A configuration file.
    """

    def __init__(self, fd):
        self.data = json.load(fd)
        self.configfile = fd.name
        self.directory = os.path.dirname(self.configfile)
        self.lockfile = os.path.join(self.directory, 'pokedex.lock')
        self.cached = {}

        self._validate()
        self.pokedex = self._load_pokedex()
        self._filter_ids()

    def get_table(self, name):
        if name in self.cached:
            return self.cached[name]

        files = (os.path.join(data_path, name + '.json') for data_path in self.data['data'])
        tables = list(map(open_json, filter(os.path.exists, files)))

        if len(tables) == 0:
            raise RuntimeError('No tables found for name: {}'.format(name))

        self.cached[name] = dict_merge(*tables)
        return self.cached[name]

    def get_asset(self, id, filename):
        paths = (os.path.join(data_path, id, filename) for data_path in self.data['assets'])

        for path in paths:
            if os.path.exists(path):
                return path
        else:
            return None

    def _validate(self):
        if not isinstance(self.data, dict):
            raise TypeError('Configuration data must be a dictionary')

        self._assert_dir_list('assets')
        self._assert_dir_list('data')
        self._assert_type('pokedex', dict)

    def _assert_type(self, key, type):
        if key not in self.data:
            raise ValueError('Configuration key missing: {}'.format(key))

        if not isinstance(self.data[key], type):
            raise TypeError('Config `{}` must be a {}'.format(key, type.__name__))

    def _assert_dir_list(self, key):
        self._assert_type(key, list)
        l = self.data[key]

        for path in l:
            if not os.path.exists(path):
                raise RuntimeError('Could not find config path `{}`'.format(path))

            if not os.path.isdir(path):
                raise RuntimeError('Config path `{}` is not a directory'.format(path))

    def _load_pokedex(self):
        # Load the data from the lockfile
        if os.path.exists(self.lockfile):
            if os.path.getmtime(self.configfile) < os.path.getmtime(self.lockfile):
                return open_json(self.lockfile)

        pokedex = self.data['pokedex']

        evolutions = self.get_table('evolutions')
        formes = self.get_table('pokedex')

        # Work out the dependent formes and evolutions
        def get_evolutions_and_formes(id, acc=[]):
            # Get evolutions
            evos = [(e.get('forme') or e['species']) for e in evolutions.get(id, [])]

            # Add pre-evelutions
            evos += [
                evo_id
                for evo_id, evo_data in evolutions.items()
                for evolution in evo_data
                if evolution['species'] == id
            ]

            # Recursively collect
            for evo in evos:
                if evo not in acc:
                    acc = [*acc, evo]
                    acc = [*acc, *get_evolutions_and_formes(evo, acc)]

            # Add formes
            f = formes.get(id, {}).get('formes') or []
            acc = [*evos, *f, *acc]

            return acc

        evolutions_and_formes = ((id, m) for m in pokedex for id in get_evolutions_and_formes(m))

        # Warn about missing formes/evolutions
        for id, owner in dict(evolutions_and_formes).items():
            if id not in pokedex:
                print('Warning: missing id `{}` (required by `{}`)'.format(id, owner))

        # Raise an error if any formes are invalid
        base_stats = formes = self.get_table('base')

        for id in pokedex:
            if id not in base_stats:
                raise KeyError('Invalid id: {}'.format(id))

        # Generate lockfile
        ids = list(pokedex.keys())

        with open(self.lockfile, 'w') as lockfile:
            json.dump(ids, lockfile)

        return ids

    def _filter_ids(self):
        pokemon_tables = ['base', 'evolutions', 'incense', 'learnsets', 'mega_evolutions', 'pokedex']
        for table in pokemon_tables:
            # Cache the table
            table_data = self.get_table(table)

            # Filter cached table
            popped = [key for key in table_data if key not in self.pokedex]
            for key in popped:
                table_data.pop(key)


def load_config(path, output):
    with open(path) as configfile:
        config = Config(configfile)
        generate_tables(config, output)
        link_assets(config, output)


def main():
    # TODO: Parse arguments properly
    load_config(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
