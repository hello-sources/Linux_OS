### Shell实现随机点名

> 首先我们要明确输入的是那些参数，我们需要输入要随机出的人数，这里我们采用cnt来记录当前要随机输出的人数，然后我们把学生姓名放在一个names文件中，我们可以直接把cat names的值赋值给一个数组，使用函数封装以下功能，用数组来存储相应的名字，对应数组下标开始是1，结束当然是对应数组元素个数，，我们通过/dev/urandom来实现随机数，但是我们只是需要前面十行，然后计算得到值，用cut命令截取，最后输出对应的随机数对于数组元素个数求余，然后加上开始下标
>
> 函数实现完成之后需要用循环遍历的方式，从1到我们需要的人数，先调用随机数函数，然后输出对应的值即可

``` shell
#!/bin/bash

if [[  $# -ne 1 ]]; then
        echo "Usage: bash List.sh num"
        exit
fi
Cnt=$1
Names=(`cat names`)
function rand() {
        Min=1
        Max=${#Names[*]}
        Num=`cat /dev/urandom | head -n 10 | cksum | cut -d ' ' -f 1`
        echo $[ $Num % $Max + $Min ]
}

for i in `seq 1 $Cnt`; do
        RandNum=`rand`
        echo "${Names[${RandNum}]}"
done

```

