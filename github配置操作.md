

### GitHub相关知识

1. 首先要有一个GitHub账号

2. 然后在账号里面创建一个仓库（仓库分为公有跟私有，公有任何人可见，私有仅自己可见）

3. 配置ssh密钥，用来Ubuntu免密操作使用

> 首先我们要在Ubuntu中的.ssh根目录下找到id_rsa.pub文件，如果没有文件就可以使用以下命令来生成，后面的可以不管直接敲回车即可

``` shell
$ ssh-keygen
```

> 这样我们就会有以下文件

<img src="C:\Users\32458\AppData\Roaming\Typora\typora-user-images\image-20200225222205682.png" alt="image-20200225222205682" style="zoom:80%;" />

> 我们使用以下命令把对应的密钥复制一下，放到GitHub网站对应的ssh密钥设置中去添加密钥即可，在setting->SSH and GPG keys中

``` shell
$ cat id_rsa.pub
```

4. 如果对应的Ubuntu没有安装git，需要安装git,使用以下命令

``` shell
$  sudo apt-get install git
```

5. 安装完之后就是对应配置文件的操作，分别配置文件名跟邮箱

``` shell
$ git config --global user.name "用户名"
$ git config --global user.email "邮箱"
```

6. 初始化一个仓库，这样我们就可以在不复制文件的情况下，把当前目录初始化成为一个仓库，可以进行仓库克隆，上传到git等操作

``` shell 
$ git init
```

7. 远程仓库克隆到本地文件夹，我们先到GitHub界面上，进入到我们所需要进行克隆的仓库下面，右边有一个clone or dowload按钮，我们使用ssh进行复制，复制完之后在Ubuntu终端中输入，也可以使用http传输协议，而且在复制库后面可以加上一个我们自定义的名字

``` shell
$ git clone "从GitHub中得到的链接，是以.git为结尾"  "我们自定义的名字"
```

8. 我们可以检查文件处于什么状态使用，下面命令，根据输出的提示可以判断对应文件状态

``` shell
$ git status
```

9. 我们可以添加一个文件到缓存中，然后我们再使用步骤8的操作的时候就会发现原来文件状态就已经发生改变了

``` shell
$ git add "文件名称"
```

10. 在我们把文件添加到缓存中之后，我们需要把他提交到仓库中去，也就是说我们要把这个文件添加到本地仓库中

``` shell 
$ git commit -m "文件说明"
```

11. 接下来我们需要把本地仓库上传到远程仓库中,在远程登录GitHub的时候就出现了，我们本地提交的仓库

``` shell
$ git push origin master
```

12. 查看远程仓库使用，可以看到我们指定的每一个服务器简写

``` shell
$ git remote
```

13. 创建远程仓库

```  shell
$ git remote add "库名称" "对应的链接"
```

14. 远程仓库重命名

``` shell
$ git remote rename "原名称" "新名称"
```



下图就是对应git工作原理

<img src="C:\Users\32458\AppData\Roaming\Typora\typora-user-images\bg2015120901.png" alt="bg2015120901" style="zoom:80%;" />

>- Workspace：工作区
>- Index / Stage：暂存区
>- Repository：仓库区（或本地仓库）
>- Remote：远程仓库