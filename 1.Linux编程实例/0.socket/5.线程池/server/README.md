# server端文件说明
> 我们分别用三种方式实现了server端，一个是正常的server端，即server.c
> 一个是使用select函数的server端，即server_select.c
> 另一个就是输入两次回车才回响的server_select修改版，即server_select_modify.c文件
> 在该文件夹下面的server_select_modify1.c文件是我们修改的第一个版本，主要是出现两次回车不能回响的解决办法
> 第二个server_select_modify2.c是增加了一些方便我们理解的输出状态，增加了颜色输出
