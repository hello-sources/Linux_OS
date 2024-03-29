### Linux记录登录时间

> 首先我们需要知道全局设置所在的目录位置,由于我们需要在打开bash以及退出bash的时候记录相应的时间，我们要知道相应的系统文件的位置
>
> 在/etc/bash.bashrc文件中存贮的是登录时的变量，对应为bash系统资源文件
>
> 在/etc/bash.profile是一些相关的配置文件，用户登录时候一定会执行
>
> 在/etc/bash.bash_logout文件中记录的是用户退出登录时执行的系统文件

**注意**
> 如果对于某一个用户记录他的启动以及结束时间，需要在用户家目录下的.bashrc文件中，也就是~/.bashrc  以及~/.bash_logout文件中记录的是结束时间，以及相关计算

1. 首先我们需要打开.bashrc文件，里面设置一个变量，记录登录时间

``` shell
LogInTime=${date +%s}
#%s记录的是秒数，从计算机元年开始计时，以秒为单位
echo ${LogInTime}
```

2. 然后我们在/etc/bash.bash.logout文件中,设置一个变量，记录登出时间

``` shell
LogOutTime=${date +%s}
echo $[ ${LogOutTime} - ${LogInTime} ]
# 输出登出时间与登录时间时间差，就是我们登陆总时间
```



- **附date详细参数**

>  %n : 下一行
> %t : 跳格
> %H : 小时(00-23)
> %I : 小时(01-12)
> %k : 小时(0-23)
> %l : 小时(1-12)
> %M : 分钟(00-59)
> %p : 显示本地 AM 或 PM
> %r : 直接显示时间 (12 小时制，格式为 hh:mm:ss [AP]M)
> %s : 从 1970 年 1 月 1 日 00:00:00 UTC 到目前为止的秒数
> %S : 秒(00-60)
> %T : 直接显示时间 (24 小时制)
> %X : 相当于 %H:%M:%S
> %Z : 显示时区
> %a : 星期几 (Sun-Sat)
> %A : 星期几 (Sunday-Saturday)
> %b : 月份 (Jan-Dec)
> %B : 月份 (January-December)
> %c : 直接显示日期与时间
> %d : 日 (01-31)
> %D : 直接显示日期 (mm/dd/yy)
> %h : 同 %b
> %j : 一年中的第几天 (001-366)
> %m : 月份 (01-12)
> %U : 一年中的第几周 (00-53) (以 Sunday 为一周的第一天的情形)
> %w : 一周中的第几天 (0-6)
> %W : 一年中的第几周 (00-53) (以 Monday 为一周的第一天的情形)
> %x : 直接显示日期 (mm/dd/yy)
> %y : 年份的最后两位数字 (00.99)
> %Y : 完整年份 (0000-9999) 
