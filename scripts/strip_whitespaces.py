#!/usr/bin/env python3

import os
import sys

def get_lines(file_name):
    lines = []
    try:
        with open(file_name, 'r') as file:
            lines = file.readlines()
        file.close()
    except FileNotFoundError:
        print(f"Error: '{file_name}': No such file")
    except PermissionError:
        print(f"Error: '{file_name}': Can't read. Permission denied")
    except IsADirectoryError:
        print(f"Error: '{file_name}': Is a directory")
    return lines


def rm_spaces_at_end(lines):
    if not lines:
        return
    for i, line in enumerate(lines):
        lines[i] = line.rstrip()


def write_to_file(file_name, lines):
    if not lines:
        return
    try:
        with open(file_name, 'w') as file:
            for line in lines:
                file.write(line + "\n")
        file.close()
    except PermissionError:
        print(f"Error: '{file_name}': Can't rewrite. Permission denied")


if __name__ == '__main__':
    file_names = sys.argv.copy()
    # Remove the script name from the list.
    file_names.pop(0)

    if not file_names:
        sys.exit("Error: Files are not specified." +
                 f" Use \"{sys.argv[0]} <file>...\"")

    for file_name in file_names:
        lines = get_lines(file_name)
        rm_spaces_at_end(lines)
        write_to_file(file_name, lines)
