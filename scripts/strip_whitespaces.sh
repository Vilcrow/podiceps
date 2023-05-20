#!/bin/bash
# Removes whitespace characters at the end of lines

if [ $# -eq 0 ] ; then
    echo "Too few arguments. Usage: $(basename $0) [FILE] ..."
    exit 1
fi

tmp_file="tmp.txt"

for arg in $@ ; do
    if [ ! -f "$arg" ] ; then
        echo "The file '$arg' doesn't exist"
        continue
    fi

    IFS_OLD=$IFS
    IFS=

    while read line ; do
        echo "$line" | sed 's/[\t ]\+$//'
    done < "$arg" > "$tmp_file"

    IFS=$IFS_OLD
    mv "$tmp_file" "$arg"
done
