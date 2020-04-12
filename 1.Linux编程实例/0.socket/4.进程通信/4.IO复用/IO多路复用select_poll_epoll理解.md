### IO多路复用

------

- **并发度**

>  首先理解什么是并发度：并发度就是在一个时间间隔上，与服务端进行交互的用户的数量

> 当一个IO进来的时候，如果我们开启一个进程，但是如果我们有一百万个IO进来，需要开启一百万个进程来处理IO，这样就会使CPU占用率大大增加，是及其不合理的，引入IO多路复用的目的就是用一个线程来处理IO流，根据每一个IO的状态来管理，提高服务器的并发度。IO多路复用简单理解最主要的就是**感知IO是否可读可写**，然后对于IO进行处理
>



- **select函数**

``` c
//函数原型
int select(int n,fd_set * readfds,fd_set * writefds,fd_set * exceptfds,struct timeval * timeout);
```

> 函数说明：（同时监控`readfds`,`writefds`,以及`exceptfds`三种文件状态）
>
> 1. `nfds` 指的是最大文件描述符的集合数目
> 2. `readfds`指的是可读文件描述符的集合，表示文件字符是否可读，如果可读就是`readready`
> 3. write表示可写文件描述符的集合，判断空间大小是否充足，充足就变成`writeready`
> 4. `exceptfds`表示一异常文件描述符
>
> 5. `timeout`指的是本次`select()`的超时结束时间，使用的是`timeval`结构体

注意：当以下事件发生时，select调用会被解除阻塞

1. 一个文件描述符变成就绪
2. 调用被一个信号中断
3. 调用select时间超时

`struct fd_set` 集合的宏定义

`FD_ZERO(fd_set *)`:清空集合

`FD_SET(int , fd_set *)`:将一个给定的文件描述符加入集合

`FD_CLR(int , fd_set *)`:将一个给定的文件描述符删除

`FD_ISSET(int  , fd_set *)`:检查集合中指定的文件描述符是否可读可写

**返回值**：select调用成功的时候返回三中描述符集合中”准备好了“的文件描述符数量。超时：返回0错误：返回-1，并设置 errno 

**缺点**： 

		1. 每次调用select，都需要把fd集合从用户态拷贝到内核态
　　2.同时每次调用select都需要在内核遍历传递进来的所有fd
　　3.select支持的文件描述符数量太小，默认是1024 

------

- **poll函数**

``` c
//函数原型
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
//nfds用来标记数组fds中的结构体元素数量

//pollfd结构体结构如下
struct pollfd {
    int fd;
    short events;//请求事件
    short revents;//返回事件
};
```

> `pollfd`结构体指定一个文件描述符，poll函数通过这个结构体可以监控多个文件描述符

|     常量     |        说明        |
| :----------: | :----------------: |
|    POLLIN    |      数据可读      |
|  POLLRANDOM  |    普通数据可读    |
|  POLLRDBAND  |   优先级数据可读   |
|   POLLPRI    |  高优先级数据可读  |
|   POLLOUT    |      数据可写      |
| POLLWRNORMRM |    普通数据可写    |
|  POLLWRBAND  |   优先级数据可写   |
|   POLLERR    |        错误        |
|   POLLHUP    |        挂起        |
|   POLLNVAL   | 描述符不是打开文件 |

**返回值**：成功返回值大于0，表示数组fds中准备好读、写或出错状态的那些socket描述符的总数量 ,如果返回0，则表示数组没有任何文件描述符处于就绪状态，poll函数会阻塞timeout时间长度，如果返回值为-1表示poll函数调用失败，并且设置errno

**优点**：

1. poll() 不要求开发者计算最大文件描述符加一的大小。
2.  poll() 在应付大数目的文件描述符的时候速度更快，相比于select。
3. 它没有最大连接数的限制

**缺点**：

1. poll返回后，需要循环遍历pollfd来获取就绪的描述符 

------

- **epoll函数**

``` c
//epoll使用函数接口
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

struct epoll_event {
    uint32_t events;
    epoll_data_t data;
};

typedef union epoll_data {
	void        *ptr;
	int          fd;
	uint32_t     u32;
	uint64_t     u64;
} epoll_data_t;
```

`epoll_create()`://创建一个epoll实例,传入大小表示监听的个数，返回一个文件描述符，所以在调用完之后需要close

`epoll_ctl()`://向epoll实例中注册监听的事件，也叫epoll集合，第一个参数epfd表示epoll_create创建并且返回的fd，第二个参数表示操作值含有三个类型

1.  EPOLL_CTL_ADD  注册目标fd到epfd中，同时关联内部event到fd上 
2.  EPOLL_CTL_MOD 修改已经注册到fd的监听事件 
3.  EPOLL_CTL_DEL  从epfd中删除已注册的fd

第三个参数表示要监听的文件描述符，第四个参数表示要监听的事件

`epoll_wait()`://表示事件等待，传入的第一个参数表示要等待的事件，第二个是 epoll_data_t 类型的数据，第三个参数表示最大的事件，还有一个是延时

`events`：对应的宏集合

> `EPOLLIN` ：表示对应的文件描述符可以读
> `EPOLLOUT`：表示对应的文件描述符可以写；
> `EPOLLPRI`：表示对应的文件描述符有紧急的数据可读
> `EPOLLERR`：表示对应的文件描述符发生错误；
> `EPOLLHUP`：表示对应的文件描述符被挂起；
> `EPOLLET`： 将EPOLL设为边缘模式
> `EPOLLONESHOT`：一次只监听一次事件，当监听完这次事件之后，如果还需要继续监听，需要再次把这个socket加入到EPOLL队列里

**边缘模式**:指的是 `epoll_ wait`检测到描述符事件发生并将此事件通知应用程序，应用程序立即处理该事件。下次调用`epoll_wait`时，不会再次响应应用程序并通知此事件。 

**水平模式**:指的是 `epoll_ wait`检测到描述符事件发生并将此事件通知应用程序，应用程序可以不立即处理该事件。下次调用`epoll_wait`时，会再次响应应用程序并通知此事件。 

**返回值：** 如果成功就返回文件描述符，返回值为0表示超时等待，返回值-1表示调用失败并且设置errno

**优点**：

1. 支持一个进程打开大数目文件描述符
2.  随文件描述符数目增加IO效率不是线性减少的

------

- **综合分析比较**

1、select，poll实现需要自己不断循环遍历所有fd集合，直到设备就绪。而epoll其实也需要调用epoll_wait不断遍历就绪链表，但是它是设备就绪时，调用回调函数，把就绪fd放入就绪链表中，并唤醒在epoll_wait中进入睡眠的进程。select和poll在就绪态时候要遍历整个fd集合，而epoll在就绪态时候只要判断一下链表是否为空就行了，节省了大量的CPU时间。

2、select，poll每次调用都要把fd集合从用户态往内核态拷贝一次，并且要把current往设备等待队列中挂一次，而epoll只要一次拷贝，而且把current往等待队列上挂也只挂一次（在epoll_wait的开始，注意这里的等待队列并不是设备等待队列，只是一个epoll内部定义的等待队列）。这也能节省不少的开销。

|                | select函数                     | poll函数                       | epoll函数                             |
| -------------- | ------------------------------ | ------------------------------ | ------------------------------------- |
| 最大支持连接数 | 1024                           | 无上限                         | 无上限                                |
| IO效率         | 每次进行线性遍历时间复杂度O(n) | 每次进行线性遍历时间复杂度O(n) | 用事件通知方式时间复杂度O(1)          |
| fd拷贝         | 每次调用都拷贝                 | 每次调用都拷贝                 | 调用epoll_ctl时拷贝，epoll_wait不拷贝 |

