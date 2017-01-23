def structure(dict, indent=0):
    """
    Generates a static C structure declaration from a Python dictionary.
    """

    entries = ['.{} = {}'.format(key, generate(value, indent=indent + 1))
               for key, value in dict.items()]

    indentation = indent * '\t'
    return '{\n\t' + indentation + (',\n\t' + indentation).join(entries) + ',\n' + indentation + '}'


def array(list, indent=0):
    """
    Generates a static C array declaration from a Python list.
    """

    entries = [generate(item, indent=indent + 1) for item in list]

    indentation = indent * '\t'
    return '{\n\t'  + indentation + (',\n\t' + indentation).join(entries) + ',\n' + indentation + '}'


def generate(data, indent=0):
    """
    Generates a static C declaration from a Python object.
    """

    if type(data) == list:
        return array(data, indent)
    elif type(data) == dict:
        return structure(data, indent)
    elif type(data) == int:
        return str(data)
    elif type(data) == str:
        return '_"' + repr(data)[1:-1] + '"'
    else:
        raise RuntimeError('Unrecognised type: ' + type(data).__name__)


def define(variable, data, indent=4):
    """
    Define a C variable given Python data.
    """

    if type(indent) == int:
        indentation = ' ' * indent
    elif type(indent) == str:
        indentation = indent
    else:
        raise TypeError('indent must be str or int')

    return '{} = {};'.format(variable, generate(data)).replace('\t', indentation)
