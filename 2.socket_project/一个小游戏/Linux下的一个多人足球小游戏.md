### Linux一个多人足球小游戏

> 这是一个基于CS架构的多人在线小游戏
>
> 游戏中人员运动的位置信息实时发送到服务器，发数据用UDP，根据踢球的速度方向等数据传输到本地，然后服务器判定是否球以及人员的状态，下方是对应的实时信息，右边对应的是帮助手册以及比赛比分



![image](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/Linux%E8%B6%B3%E7%90%83%E6%B8%B8%E6%88%8F.png?raw=true)



1. 在client端里面，当按住回车键的时候才能在Write窗口输入数据，当再次按下回车之后，清除输入框里面的内容
2. 实现接口：服务器把一条消息广播给所有用户，采用FootBallMsg封装
3. 实现客户端发送消息到服务端的功能

> 当有客户登录在服务端初始化一个“精灵”，通过控制信息来移动球员
>
> 实现多人控制多个“精灵”同时在窗口中移动

4. 实现C语言对于json格式文件的解析，可以写一个小栗子

**对于以下数据进行重新定义**

> 以下json字符串都放在FootBallMsg中的msg成员中

```c++
#define FT_TEST 0x01   //服务端心跳信息
#define FT_WALL 0x02   //服务端以字符串方式广播游戏情况
#define FT_MSG 0x04    //客户端发送的消息，服务端转发的消息 
#define FT_ACK 0x08    //客户端对于心跳的ACK确认
#define FT_FIN 0x10	   //客户端服务端下线前，对于彼此的告知
#define FT_CTL 0x20    //客户端发送的控制信息
#define FT_GAME 0x40   //服务端对客户端广播的游戏实时画图，json发送
#define FT_SCORE 0x80  //服务端向客户端广播的游戏比分变化，json发送（包括进球人）
#define FT_GAMEOVER 0x100 //游戏结束
```



