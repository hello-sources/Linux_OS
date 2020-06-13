### 五种IO模型

五种I/O模式

> 1、阻塞 I/O
> 2、非阻塞 I/O
> 3、I/O 多路复用
> 4、信号驱动 I/O（ SIGIO）
> 5、异步 I/O

一般来说，程序进行输入操作有两步：

> 1．等待有数据可以读
> 2．将数据从系统内核中拷贝到程序的数据区。



对于一个对套接字的输入操作，第一步一般来说是等待数据从网络上传到本地。当数据包到达的时候，数据将会从网络层拷贝到内核的缓存中；第二步是从内核中把数据拷贝到程序的数据区中

1. 阻塞 I/O 模式

 进程在调用 recvfrom 一直到从 recvfrom 返回或发生错误，这段时间是阻塞的。当 recvfrom正常返回时，我们的进程继续它的操作。

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO1.png?raw=true)



2. 非阻塞模式 I/O

 假设前三次recvfrom从应用层到内核的时候，如果该缓冲区没有数据的话，就直接返回一个EWOULDBLOCK 的错误。第四次我们调用 recvfrom 函数，一个数据报已经到达了，内核将它拷贝到我们的应用程序的缓冲区中，然后 recvfrom 正常返回，我们就可以对接收到的数据进行处理了。
 当一个应用程序使用了非阻塞模式的套接字，它需要使用一个循环来不停的测试是否一个文件描述符有数据可读（称做 polling）。应用程序不停的 polling 内核来检查是否 I/O操作已经就绪。这将是一个极浪费 CPU 资源的操作。这种模式使用中不是很普遍。

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO2.png?raw=true)



3. I/O 多路复用

 Linux提供select/poll，进程通过将一个或多个fd传递给select或者poll系统调用，并阻塞在select操作上，而不是我们来调用 recvfrom（或 recv）的时候阻塞。这样select/poll可以帮助我们侦测多个fd是否处于就绪状态。当select函数返回的时候，也就是套接字可以读取数据的时候。这时候我们就可以调用 recvfrom函数来将数据拷贝到我们的程序缓冲区中。

 和阻塞模式相比较，多路复用的高级之处在于，它能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个进入读就绪状态， select()函数就可以返回。

 select/poll是顺序扫描fd是否就绪，而且支持的fd数量有限。Linux还提供了一个epoll系统调用，epoll使用基于事件驱动方式代替顺序扫描，因此性能更高。当fd就绪时，立即回调函数rollback。

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO3.png?raw=true)





假设一个程序要同时处理套接字传来的网络数据又要处理本地本地的标准输入输出。程序阻塞于本地时，此时服务器关闭，在读取套接字之前将会看不见结束标志。此时就不能使用阻塞的套接字。

IO 多路技术一般在下面这些情况中被使用：

> 1、当一个客户端需要同时处理多个文件描述符的输入输出操作的时候（一般来说是标准的输入输出和网络套接字）， I/O 多路复用技术将会有机会得到使用。
>  2、当程序需要同时进行多个套接字的操作的时候。
>  3、如果一个 TCP 服务器程序同时处理正在侦听网络连接的套接字和已经连接好的套接字。
>  4、如果一个服务器程序同时使用 TCP 和 UDP 协议。
>  5、如果一个服务器同时使用多种服务并且每种服务可能使用不同的协议（比如 inetd就是这样的）。
>  6、I/O 多路服用技术并不只局限与网络程序应用上。几乎所有的程序都可以找到应用 I/O多路复用的地方。



4. 信号驱动 I/O 模式

 使用信号，让内核在文件描述符就绪的时候使用 SIGIO 信号来通知我们。我们首先需要允许套接字使用信号驱动 I/O。在这种模式下，系统调用将会立即返回，然后我们的程序可以继续做其他的事情。当数据就绪的时候，系统会向我们的进程发送一个 SIGIO 信号。这样我们就可以在 SIGIO信号的处理函数中进行 I/O 操作。不必为等待数据进行额外的编码，灵活性大。

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO4.png?raw=true)





5. 异步 I/O 模式

 该模式下，进行一个 IO 操作的时候，我们传递给内核我们的文件描述符，我们的缓存区指针和缓存区的大小，一个偏移量 offset，以及在内核结束所有操作后和我们联系的方法。这种调用是立即返回的，我们的程序不需要阻塞住来等待数据的就绪。我们可以要求系统内核在所有的操作结束后（包括从网络上读取信息，然后拷贝到我们提供给内核的缓存区中）给我们发一个消息。

异步 I/O 和 信号驱动 I/O 的区别是：
 信号驱动 I/O 模式下，内核在操作可以被操作的时候通知给我们的应用程序发送SIGIO 消息。
 异步 I/O 模式下，内核在所有的操作都已经被内核操作结束之后才会通知我们的应用程序。

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO5.png?raw=true)





**几种IO模式比较**

![img](https://github.com/hello-sources/Relative_Things/blob/master/img/Linux_OS_img/IO6.png?raw=true)

