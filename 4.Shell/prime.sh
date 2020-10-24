#!/bin/bash
declare -a prime
sum=0

function usage() {
    printf "Usage : %s start_num end_num\n" $0
}

function init() {
    S=$1
    E=$2
    for (( i=$S; i<=$E; i++ ));do
        prime[$i]=0
    done

}

if [[ $# -ne 2 ]];then
    usage
    exit
fi

start_num=$1
end_num=$2

if [[ $start_num -lt 0 ]];then
    start_num=0
fi

if [[ ${start_num} -gt ${end_num} ]];then
    usage
    exit
fi

init 0 ${end_num}

for (( i=2; i<=${end_num}; i++ ));do
    if [[ ${prime[$i]} -eq 0 ]];then
        prime[0]=$[ ${prime[0]} + 1 ]
        prime[${prime[0]}]=$i
        if [[ $i -ge ${start_num} ]];then
            sum=$[ ${sum} + $i ]
        fi
    fi
    for (( j=1; j<=${prime[0]}; j++ ));do
        if [[ $[ ${i} * ${prime[$j]} ] -gt ${end_num} ]];then
            break
        fi
        prime[$[ ${i} * ${prime[$j]} ]]=1
        if [[ $[ ${i} % ${prime[$j]} ] -eq 0  ]];then
            break
        fi
    done
done

echo $sum
