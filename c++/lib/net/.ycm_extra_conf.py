import os
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath(os.path.dirname(__file__))

flags = [
    '-Wall',
    '-Wextra',
    '-Iinclude',
    '-Isrc',
    '-std=c++17',
    '-x',
    'c++'
]

def Settings( **kwargs):
    cspec_include = os.getenv('CSPEC_INCLUDE')
    flags.append("-I{include}".format(include = cspec_include))

    utility_include = os.getenv('UTILITY_INCLUDE')
    flags.append("-I{include}".format(include = utility_include))

    return {
        'flags': flags,
        'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT
    }

