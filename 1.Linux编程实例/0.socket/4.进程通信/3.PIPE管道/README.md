# PIPE管道引发的问题
> 缓冲区分为非缓冲区，块缓冲，行缓冲, 在pipe_create.c中我们实现的是pipe的使用，在pipe_create1.c中我们使用的是fscanf与fprintf函数来实现文件的读写，其中里面要理解的代码就是对应文件缓冲，如果没加`fflush(func)`这样一行代码，可能会需要读入一个块的大小数据才显示出来
------
> 我们学到的缓冲：缓冲IO 直接IO 同步IO 异步IO，文件缓冲对应为块缓冲，缓冲区大小一般为512, 1024, 2048, 4096等
------
> 注意fscanf以及fprintf对应是否存在换行符号的问题，fscanf对应的是读入空格结尾的数据，在spritnf中需要对应的加上换行符号
------
> 在popentask.c文件中是真正意义上的自己创建的popen函数与pclose函数，而在popen_pclose.c文件中是借鉴网上的代码
