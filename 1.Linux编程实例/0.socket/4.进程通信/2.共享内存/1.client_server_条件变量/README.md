# client_server
1. 非亲缘进程通信
2. 条件变量
3. 内存共享
> 主要通过对应修改第一版本client_server加上了`pthread_mutex`线程锁，以及内存共享中的`shm`一族函数，还有就是pthread中的条件变量`pthread_cond`一族的函数
