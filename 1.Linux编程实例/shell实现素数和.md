### shell实现随机点名

- **我自己写的版本**

> 单纯的按照C语言来改成shell脚本的实现从1到1000的素数和计算,值得注意的是我是直接给数组进行初始化，赋值，这也是我对于shell理解不够深刻的原因之一

``` shell

#!/bin/bash

max_n=1000
declare -a prime

function _init_() {
        for (( i=0; i<${max_n}; i++ )); do
                prime[i]=0
        done
        for (( i=2; i<=${max_n}; i++ )); do
                if [[ ${prime[${i}]} -eq 0 ]]; then
                        prime[0]=$[ ${prime[0]} + 1 ]
                        prime[${prime[0]}]=${i}
                fi
                for (( j=1; j<=${prime[0]}; j++ )); do
                        if [[ $[ ${i} * ${prime[${j}]} ] -gt ${max_n} ]]; then
                                break
                        fi
                        prime[$[ ${i} * ${prime[${j}]} ]]=1
                        if [[ $[ ${i} % ${prime[${j}]} ] -eq 0 ]]; then
                                break
                        fi
                done
        done
}

_init_
ans=0
for i in `seq 1 ${prime[0]}`; do
        echo ${prime[$i]}
done

for (( i = 1; i <= ${prime[0]}; i++ )); do
        #if [[ ${prime[$i]} -eq 0 ]]; then
                ans=$[ ${prime[${i}]} + ${ans}]
        #fi
done

echo ${ans}

```



- **正确理解shell之后的版本**

> 首先要明确的是，shell脚本语言是一门弱类型语言，对于数组声明不需要初始化，因为一开始声明一个数组的时候对应数组里面元素都为空，所以我们在线性筛的基础上只是需要把判断是否为0的操作改为是否为空,从而不需要进行对于数组初始化操作，因此我们不是很需要把对应计算素数和封装在函数里面所以直接进行写功能就行

``` shell
#!/bin/bash

declare -a prime
End=1000
Sum=0

for (( i=2; i<${End}; i++ )); do
        if [[ ${prime[$i]}x == x ]]; then
                prime[0]=$[ ${prime[0]} + 1 ]
                prime[${prime[0]}]=$i
                Sum=$[ ${Sum} + $i ]
        fi
        for (( j=1; j<=${prime[0]}; j++ )); do
                if [[  $[ $i * ${prime[$j]} ] -gt ${End} ]]; then
                        break;
                fi
                prime[$[ $i * ${prime[$j]} ]]=1
                if [[ $[ $i % ${prime[$j]} ] -eq 0 ]]; then
                        break
                fi
        done
done

echo $Sum

```

