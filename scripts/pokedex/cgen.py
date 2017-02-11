import itertools

def flatten(l):
    return list(itertools.chain.from_iterable(l))


def _generateall(values, indent):
    return map(lambda x: x.generate(indent), values)


def _wrap(lines, indent):
    indentation = indent * '\t'
    sep = ',\n\t' + indentation
    return '{\n\t' + indentation + sep.join(lines) + '\n' + indentation + '}'

class CValue:
    def generate(self, indent=0):
        raise NotImplementedError

    def __repr__(self):
        return '{}({})'.format(self.__class__.__name__, repr(self.data))

    def values(self):
        return []


class CStructure(CValue):
    def __init__(self, data):
        self.data = data
        # TODO: Validate

    def generate(self, indent=0):
        entries = []

        for item in self.data:
            if isinstance(item, tuple):
                entries.append('.{} = {}'.format(item[0], item[1].generate(indent=indent + 1)))
            elif isinstance(item, CValue):
                entries.append(item.generate(indent=indent + 1))

        return _wrap(entries, indent)

    def values(self):
        return list(map(lambda item: item[1] if isinstance(item, tuple) else item, self.data))


class CArray(CValue):
    def __init__(self, data):
        self.data = data
        # TODO: Validate

    def generate(self, indent=0):
        entries = []
        for item in self.data:
            if isinstance(item, tuple):
                entries.append('[{}] = {}'.format(*_generateall(item, indent=indent + 1)))
            elif isinstance(item, CValue):
                entries.append(item.generate(indent=indent + 1))

        return _wrap(entries, indent)

    def values(self):
        return list(map(lambda item: item[1] if isinstance(item, tuple) else item, self.data))


class CDefinition:
    def values(self):
        return []


class CVariableDefinition(CDefinition):
    def __init__(self, variable, value):
        self.variable = variable
        self.value = value

    def generate(self, indent=0):
        return '{} = {};'.format(self.variable, self.value.generate(indent=indent))

    def values(self):
        return self.value.values()


class CExternalVariableDefinition(CDefinition):
    def __init__(self, variable):
        self.variable = variable

    def generate(self, indent=0):
        return 'extern {};'.format(self.variable)


class CDefinedVariable(CValue):
    """
    Abstract wrapper for a value that causes a top-level definition to be generated.
    """

    def __init__(self, definition, value):
        self.definition = definition
        self.value = CConstant(value)
        self.data = self.value

    def generate(self, indent=0):
        return self.value.generate(indent=indent)

    def generate_definition(self, indent=0):
        return self.definition.generate(indent=indent)

    def values(self):
        return self.value.values()


class CExternalVariable(CDefinedVariable):
    def __init__(self, variable, name):
        super().__init__(
            CExternalVariableDefinition(variable),
            name,
        ),


class CFile:
    def __init__(self, *definitions):
        for definition in definitions:
            if not isinstance(definition, CDefinition):
                raise TypeError('File must consist of a list of definitions')

        self.definitions = definitions

    def generate_file(self, indent=4):
        def gather_values(value):
            return [value] + flatten(list(map(gather_values, value.values())))

        values = flatten(map(gather_values, self.definitions))

        defs = [value.generate_definition(indent=0)
                for value in values if isinstance(value, CDefinedVariable)]

        defs += [definition.generate(indent=0) for definition in self.definitions]

        contents = '\n\n'.join(defs)

        if type(indent) == int:
            indentation = ' ' * indent
        elif type(indent) == str:
            indentation = indent
        else:
            raise TypeError('indent must be str or int')

        return contents.replace('\t', indentation)


class CConstant(CValue):
    def __init__(self, data):
        self.data = str(data)

    def generate(self, indent=0):
        return self.data


class CString(CValue):
    def __init__(self, data):
        self.data = str(data)

    def generate(self, indent=0):
        return '_"' + repr(self.data)[1:-1] + '"'


def coerce(data):
    if isinstance(data, CValue):
        return data
    elif isinstance(data, dict):
        data = { key: coerce(value) for key, value in data.items() }
        return CStructure(data.items())
    elif isinstance(data, list):
        data = [coerce(value) for value in data]
        return CArray(data)
    elif isinstance(data, str):
        return CString(data)
    elif isinstance(data, int):
        return CConstant(data)
    else:
        raise ValueError('Failed to coerce Python type `{}` to C type'.format(type(data).__name__))


# def define(variable, data, indent=4):
#     """
#     Define a C variable given Python data.
#     """

#     if type(indent) == int:
#         indentation = ' ' * indent
#     elif type(indent) == str:
#         indentation = indent
#     else:
#         raise TypeError('indent must be str or int')

#     return '{} = {};'.format(variable, generate(data)).replace('\t', indentation)
