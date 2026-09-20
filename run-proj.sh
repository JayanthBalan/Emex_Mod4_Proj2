#!/bin/bash

target_executable="inverted-search"
file_path="target-files/"

make clean
make

if [ $# -eq 1 ]
then
    if [ "$1" == "all" ]; then
        ./${target_executable} ${file_path}*.txt
    else
        ./${target_executable} "${file_path}$1"
    fi
else
    echo "1
     2
     darrow war KEEP chained crimes fear him her ROARED reigns rain
      3" | ./${target_executable}
fi
