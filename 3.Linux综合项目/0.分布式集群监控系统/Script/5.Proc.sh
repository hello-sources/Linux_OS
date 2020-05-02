#########################################################################
# File Name: 5.Proc.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Sat 02 May 2020 02:09:32 PM CST
#########################################################################
#!/bin/bash

eval `ps -aux --sort=-%cpu -h | awk -v num=0 \
	'{ if ($3 < 50) {exit} else {num++; printf("cpupid["num"]=%d\n", $2)}} END {printf("cpunum=%d\n", num)}'`

eval `ps -aux --sort=-%mem -h | awk -v num=0 \
	'{ if ($4 < 50) {exit} else {num++; printf("mempid["num"]=%d\n", $2)}} END {printf("memnum=%d\n", num)}'`

if [[ ${cpunum} -gt 0 || ${memnum} -gt 0 ]]; then
	sleep 5
else 
	exit 0
fi

Time=`date +"%Y-%m-%d__%H:%M:%S"`

if [[ ${cpunum} -gt 0 ]]; then
	cnt=0
	for i in ${cpupid[*]}; do
		eval `ps -aux -h -q $i | awk -v num=${cnt}\
			'{ if ($3 < 50) {exit} else{printf("Pname["num"]=%s;Pid["num"]=%d;User["num"]=%s;CpuP["num"]=%.2f;MemP["num"]=%.2f", $11, $2, $1, $3, $4)}}'`
		cnt=$[cnt+1]
	done
fi

if [[ ${memnum} -gt 0 ]]; then
	cnt=0
	for i in ${mempid[*]}; do
		eval `ps -aux -h -q $i | awk -v num=${cnt}\
			'{ if ($4 < 50) {exit} else{printf("Pname["num"]=%s;Pid["num"]=%d;User["num"]=%s;CpuP["num"]=%.2f;MemP["num"]=%.2f", $11, $2, $1, $3, $4)}}'`
		cnt=$[cnt+1]
	done
fi

for (( i = 0; i < ${cnt}; i++)); do
	echo "${Time} ${Pname[$i]} ${Pid[$i]} ${User[$i]} ${CpuP[$i]} ${MemP[$i]}%"
done
