#########################################################################
# File Name: 6.Sys.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Sat 02 May 2020 03:24:17 PM CST
#########################################################################
#!/bin/bash
Time=`date +"%Y-%m-%d__%H:%M:%S"`
HostName=`hostname`
OsType=`cat /etc/issue.net | tr " " "_"`
KernelVersion=`uname -r`
LoadAvg=`cut -d " " -f 1-3 /proc/loadavg`
UpTime=`uptime -p | tr -s " " "_"`
eval `df -T -x devtmpfs -x tmpfs -m --total | tail -n 1 | awk\
	'{printf("DiskTotal=%s;DiskUsedP=%d", $3, $6)}'`

DiskWarningLevel="normal"
if [[ ${DiskUsedP} -gt 90 ]]; then
	DiskWarningLevel="warning"
elif [[ ${DiskUsedP} -gt 80 ]]; then
	DiskWarningLevel="note"
fi

MemWarningLevel="normal"
if [[ ${MemUsedP} -gt 80 ]]; then
	MemWarningLevel="warning"
elif [[ ${MemUsedP} -gt 70 ]]; then
	MemWarningLevel="note"
fi

eval `free -m | head -n 2 | tail -n 1 | awk \
	'{printf("MemTotal=%s;MemUsed=%s", $2, $3)}'`
MemUsedP=$[ ${MemUsed}*100 / ${MemTotal} ]

CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`
CpuTemp=`echo "scale=2;${CpuTemp}/1000" | bc`

CpuWarningLevel="normal"
if [[ `echo "${CpuTemp} >= 70" | bc -l` -eq 1 ]]; then
	CpuWarningLevel="warning"
elif [[ `echo ${CpuTemp} '>=' 50 | bc -l` == 1 ]]; then
	CpuWarningLevel="note"
fi

echo "${Time} ${HostName} ${OsType} ${KernelVersion} ${UpTime} ${LoadAvg} ${DiskTotal} ${DiskUsedP}% ${MemTotal} ${MemUsedP}% ${CpuTemp} ${DiskWarningLevel} ${MemWarningLevel} ${CpuWarningLevel}"

