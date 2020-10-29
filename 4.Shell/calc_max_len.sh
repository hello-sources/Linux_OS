#!/bin/bash
max_len=0
max_string=''
if [[ $# -lt 1 ]];then
    printf "Usage: %s file[...]\n" $0
fi

#for (( i=1; i<=$#; i++ ));do
#    echo ${$i}
#done

for i in $*;do
    echo $i
done

for i in `cat $1 | tr -s -c "a-zA-Z" " "`;do
    len=${#i}
    if [[ $len -gt ${max_len} ]];then
        max_len=$len
        max_string=$i
    fi
done

echo $max_string $max_len
