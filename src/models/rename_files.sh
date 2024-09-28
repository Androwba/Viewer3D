#!/bin/bash

directory="."

cd "$directory" || exit

for file in *.bj; do
    if [ -e "$file" ]; then
        mv "$file" "${file%.bj}.obj"
        echo "Renamed $file to ${file%.bj}.obj"
    else
        echo "$file does not exist, skipping."
    fi
done

