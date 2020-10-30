#!/bin/bash

function find_max_in_file(){
	if [[ ! -f ${1} ]];then
        echo "$1 is not a r_file!"
        return
    fi
    echo "Finding Max_String in file $1"
    words=`cat $1 | tr -s -c "a-zA-Z" "\n"`
    for i in ${words};do
        len_t=`echo -n ${i} | wc -c`
        if [[ ${len_t} -gt ${len_max} ]];then
            len_max=${len_t}
            max_string=${i}
            max_file=$1
        fi
    done
}


function find_max_in_dir(){
    for i in `ls -A $1`;do
        if [[ -d ${1}/${i} ]];then
            echo "${1}/$i is a dir"
            find_max_in_dir ${1}/${i}
        else
            echo "${1}/${i} is a file"
            find_max_in_file ${1}/${i}
        fi
    done
}
