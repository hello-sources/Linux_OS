#########################################################################
# File Name: SysInfo.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Thu 30 Apr 2020 10:41:24 PM CST
#########################################################################
#!/bin/bash
Time=`date +"%Y-%m-%d__%H:%M:%S"`

HostName=`hostname`

OSVersion=`cat /etc/issue | cut -c 1-18 `

KernalVersion=`cat /proc/version | cut -d ' ' -f 1-3`

Uptime=`uptime |cut -d ' ' -f 5 | cut -d ',' -f 1`

LoadAvg=`cat /proc/loadavg  | cut -d ' ' -f 1-3`

eval `df -T -m -x tmpfs -x devtmpfs | tail -n +2 |\
	    awk -v DiskSum=0 -v DiskLeft=0 \
			'{printf("Pname["NR"]=%s;Psum["NR"]=%d;Pleft["NR"]=%d;Puseperc["NR"]=%s;", $7, $3, $4, $6); DiskSum+=$3; DiskLeft+=$5} \
	        END {printf("Pnum=%d;DiskSum=%d;DiskLeft=%d;", NR, DiskSum, DiskLeft)}'`

DiskUsedPerc=$[ ($DiskSum - $DiskLeft)*100/$DiskSum ]

eval $(free -m | head -n +2 | tail -n +2 | awk '{printf("Mem=%d MemUsed=%d", $2, $3)}')

MemUsedPerc=$[ $MemUsed*100/ Mem ]

CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`

if [[ $DiskUsedPerc -lt 80 ]]; then
	DiskWarningLevel="normal"
elif [[ $DiskUsedPerc -lt 90 ]]; then
	DiskWarningLevel="note"
elif [[ $DiskUsedPerc -lt 100 ]]; then
	DiskWarningLevel="warning"
fi

if [[ $MemUsedPerc -lt 70 ]]; then
	MemWarningLevel="normal"
elif [[ $MemUsedPerc -lt 80 ]]; then
	MemWarningLevel="note"
elif [[ $MemkUsedPerc -lt 100 ]]; then
	MemWarningLevel="warning"
fi


if [[ $CpuTemp/1000 -lt 50 ]]; then
	CpuTempWarningLevel="normal"
elif [[ $CpuTemp/1000 -lt 70 ]]; then
	CpuTempWarningLevel="note"
elif [[ $CpuTemp/1000 -lt 100 ]]; then
	CpuTempWarningLevel="warning"
fi
CpuTemp=`echo "scale=2;$CpuTemp/1000" | bc`

echo ${Time} ${HostName} ${OSVersion} ${KernelVersion} ${KernalVersion} \
	${Uptime} ${LoadAvg} ${Mem} ${MemUsedPerc}'%' ${DiskSum} ${DiskUsedPerc}'%' ${CpuTemp} ${DiskWarningLevel} ${MemWarningLevel} ${CpuTempWarningLevel}
