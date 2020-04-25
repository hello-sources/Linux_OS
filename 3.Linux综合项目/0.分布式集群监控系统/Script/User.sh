#########################################################################
# File Name: 4.User.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Sat 25 Apr 2020 07:05:10 PM CST
#########################################################################
#!/bin/bash
time=`date +"%Y-%m-%d__%H:%M:%S"`
user_num=$(awk -F : '{if ($3 > 500) printf("%s\n", $1)}' /etc/passwd | wc -l)
active_user=$(last | head -3 | awk '{print($1)}' | sort -u | tr -c "a-z" ",")
root_user=$(awk -F : '{if ($3==0) printf("%s\n",$1)}' /etc/passwd)
online_user=$(w | awk '{print($1)}' | tail -n +3)
online_info=$(w | awk '{printf("%s_%s_%s\n",$1,$3,$2)}' | tail -n +3 | tr -c "a-zA-Z0-9" ",")

echo -n "${time} "
echo -n "${user_num} "
echo -n "[${active_user}] "
echo -n "[${root_user}] "
echo -n -e "[${online_info}]\n"

