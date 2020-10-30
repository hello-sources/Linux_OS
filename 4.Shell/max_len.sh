#!/bin/bash
len_max=
max_string=""
max_file=""

source function.sh

if [[ $# -eq 0 ]];then
    find_max_in_dir "."
else
    for i in $@;do
        if [[ -d $i ]];then
            find_max_in_dir $i
        else
            find_max_in_file $i
        fi
    done
fi

printf "The max string is \033[31;34m%s \033[0m , with length \033[31;34m %d \033[0m in file \033[31;34m %s \033[0m\n" $max_string $len_max $max_file
