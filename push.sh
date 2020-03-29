#########################################################################
# File Name: push.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Sat 28 Mar 2020 02:56:40 PM CST
#########################################################################
#!/bin/bash
if [[ $# -ne 1 ]]; then
	echo "Usage : $0 msg"
	exit
fi

git add .
git commit -m $1
git push
