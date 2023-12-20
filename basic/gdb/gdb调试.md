# gdb常用命令详解

## gdb常用调试命令概览和说明

先给出一个常用命令的列表，后面会结合具体的例子详细介绍每个命令的用法。

| 命令名称        | 命令缩写    | 命令说明                             |
|-------------|---------|----------------------------------|
| run         | r       | 运行一个程序                           |
| continue    | c       | 让暂停的程序继续运行                       |
| break       | b       | 添加断点                             |
| tbreak      | tb      | 添加临时断点                           |
| backtrace   | bt      | 查看当前线程的调用堆栈                      |
| frame       | f       | 切换到当前调用线程的指定堆栈                   |
| info        | info    | 查看断点/线程等信息                       |
| enable      | enable  | 启用某个断点                           |
| disable     | disable | 禁用某个断点                           |
| delete      | del     | 删除断点                             |
| list        | l       | 显示源码                             |
| print       | p       | 打印或修改变量或寄存器值                     |
| ptype       | ptype   | 查看变量类型                           |
| thread      | thread  | 切换到指定线程                          |
| next        | n       | 运行到下一行                           |
| step        | s       | 如果有调用函数，进入调用的函数内部，相当于step into   |
| until       | u       | 运行到指定行停下来                        |
| finish      | fi      | 结束当前调用函数，到上一层函数调用处               |
| return      | return  | 结束当前调用函数并返回指定值，到上一层函数调用处         |
| jump        | j       | 将当前程序执行流跳转到指定行或地址                |
| disassemble | dis     | 查看汇编代码                           |
| set args    |         | 设置程序启动命令行参数                      |
| show args   |         | 查看设置的命令行参数                       |
| watch       | watch   | 监视某一个变量或内存地址的值是否发生变化             |
| display     | display | 监视的变量或者内存地址，当程序中断后自动输出监控的变量或内存地址 |
| dir         | dir     | 重定向源码文件的位置                       |

上表只列举了一些常用命令，未列举一些不常用命令（如file命令）。不建议读者刻意记忆这些命令，因为命令数目有点多，建议读者找几个程序实际练习一下，这样就容易记住了。表中“命令缩写”那一栏，是笔者平时对命令的简写输入，读者可以选择采用或者不采用。对于一个命令可以简写成什么样子，gdb没有强行规定，但读者在简写gdb命令时需要遵循如下两个规则：

1. 一个命令简写时，不能让gdb出现多个选择，若出现多个选择， gdb就不知道对应哪个命令了。举个例子，例如输入“th”， th对应的命令有thread和thbreak（上表没有列出），这样gdb就不知道要使用哪个了，因此需要更具体的输入，gdb才能识别。
```
(gdb) th
Ambiguous command "th": thbreak, thread.
```
2. gdb有些命令虽然也对应多个选择，但是有些命令的简写就是确定的，比如“r” 就是命令"run"的简写，虽然读者输入"r"时可能是想使用"return"命令。

## gdb调试redis准备工作
redis的最新源码下载地址可以在redis官网获得。使用wget命令将redis源码文件下载下来：
```bash
[root@localhost ~]# wget http://download.redis.io/releases/redis-7.2.3.tar.gz
--2020-05-27 11:55:15--  http://download.redis.io/releases/redis-7.2.3.tar.gz
Resolving download.redis.io (download.redis.io)... 109.74.203.151
Connecting to download.redis.io (download.redis.io)|109.74.203.151|:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 2210882 (2.1M) [application/x-gzip]
Saving to: ‘redis-7.2.3.tar.gz’

39% [=============================================>                                                                        ] 869,940     29.0KB/s  eta 39s 

```

下载完成后解压：
```bash
[root@localhost ~]# tar zxvf redis-7.2.3.tar.gz
```

进入生成的redis-7.2.3目录使用makefile进行编译，为了方便调试，我们需要生成调试符号并且关闭编译器优化选项，操作如下：
```base
[root@localhost ~]# cd redis-7.2.3
[root@localhost redis-7.2.3]# make CFLAGS="-g -O0" -j 4
```

-g选项表示生成调试符号，-o0选项表示关闭优化，-j 4选项表示同时开启4个进程同时编译，加快编译速度。 redis是纯C项目，使用的编译器是gcc，所以这里设置编译器的选项时使用的是CFLAGS选项；对于C++项目，使用的编译器一般是g++，相对应的编译器选项是CXXFLAGS。

如果在编译过程中出现如下错误：
```base
[root@localhost redis-7.2.3]# make CFLAGS="-g -O0"
cd src && make all
make[1]: Entering directory `/root/redis-7.2.3/src'
    CC adlist.o
In file included from adlist.c:34:0:
zmalloc.h:50:31: fatal error: jemalloc/jemalloc.h: No such file or directory
 #include <jemalloc/jemalloc.h>
                               ^
compilation terminated.
make[1]: *** [adlist.o] Error 1
make[1]: Leaving directory `/root/redis-7.2.3/src'
make: *** [all] Error 2
```

可以改用以下命令来编译，这是由于系统没有安装jemalloc库，可以修改编译参数让redis使用系统默认的malloc而不是jemalloc：

```bash
[root@localhost redis-7.2.3]# make MALLOC=libc CFLAGS="-g -O0" -j 4
```

编译的过程中如果出现类似如下错误：
```
server.c:4706:25: error: ‘struct redisServer’ has no member named ‘port’
             mode, server.port ? server.port : server.tls_port,
                         ^
server.c:4706:39: error: ‘struct redisServer’ has no member named ‘port’
             mode, server.port ? server.port : server.tls_port,
                                       ^
server.c:4706:53: error: ‘struct redisServer’ has no member named ‘tls_port’
             mode, server.port ? server.port : server.tls_port,
```

可以通过升级gcc编译器来解决，对于CentOS系统：
```bash
# 升级到gcc 9.3：
yum -y install centos-release-scl
yum -y install devtoolset-9-gcc devtoolset-9-gcc-c++ devtoolset-9-binutils
scl enable devtoolset-9 bash

# 需要注意的是scl命令启用只是临时的，退出shell或重启就会恢复原系统gcc版本。
# 如果要长期使用gcc 9.3的话：
# 这样退出shell重新打开就是新版的gcc了
# 以下其他版本同理，修改devtoolset版本号即可。
echo "source /opt/rh/devtoolset-9/enable" >>/etc/profile
```

升级gcc编译器后再次执行上述编译指令，编译成功后，会在src目录下生成多个可执行程序，其中redis-server和redis-cli是我们需要即将调试的程序。

我们进入src目录，使用gdb启动redis-server这个程序：
```bash
[root@localhost src]# gdb redis-server
Reading symbols from redis-server...
```

## run命令
默认情况下，gdb+filename只是附加一个调试文件，并没有启动这个程序，我们需要输入run命令启动这个程序（run命令简写成r）：
```
(gdb) r
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Process 31134 launched: 'redis-7.2.3/src/redis-server' (arm64)
31134:C 19 Dec 2023 16:53:12.321 * oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
31134:C 19 Dec 2023 16:53:12.321 * Redis version=7.2.3, bits=64, commit=00000000, modified=0, pid=31134, just started
31134:C 19 Dec 2023 16:53:12.321 # Warning: no config file specified, using the default config. In order to specify a config file use redis-7.2.3/src/redis-server /path/to/redis.conf
31134:M 19 Dec 2023 16:53:12.321 * Increased maximum number of open files to 10032 (it was originally set to 2560).
31134:M 19 Dec 2023 16:53:12.321 * monotonic clock: POSIX clock_gettime
                _._                                                  
           _.-``__ ''-._                                             
      _.-``    `.  `_.  ''-._           Redis 7.2.3 (00000000/0) 64 bit
  .-`` .-```.  ```\/    _.,_ ''-._                                  
 (    '      ,       .-`  | `,    )     Running in standalone mode
 |`-._`-...-` __...-.``-._|'` _.-'|     Port: 6379
 |    `-._   `._    /     _.-'    |     PID: 31134
  `-._    `-._  `-./  _.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |           https://redis.io       
  `-._    `-._`-.__.-'_.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |                                  
  `-._    `-._`-.__.-'_.-'    _.-'                                   
      `-._    `-.__.-'    _.-'                                       
          `-._        _.-'                                           
              `-.__.-'                                               

31134:M 19 Dec 2023 16:53:12.322 # WARNING: The TCP backlog setting of 511 cannot be enforced because kern.ipc.somaxconn is set to the lower value of 128.
31134:M 19 Dec 2023 16:53:12.323 * Server initialized
31134:M 19 Dec 2023 16:53:12.323 * Ready to accept connections tcp
```

这就是redis-server启动界面。如果程序已经启动，我们再次输入run命令则是重启程序。我们在gdb界面按Ctrl + c（界面中的**^C**）让程序中断下来，再次输入r命令，gdb会提示我们是否重启程序，输入y确认重启。

```text
(gdb) r
There is a running process, kill it and restart?: [Y/n] y
Process 31134 exited with status = 9 (0x00000009) killed
```

## continue 命令
当程序触发断点或者使用Ctrl + c命令中断下来后，如果我们想让程序继续运行，只要输入continue命令即可（简写成c），当然，如果continue命令让程序继续运行的过程中触发了设置的程序断点，程序在断点处中断下来。

```text
Thread 1 "redis-server" received signal SIGINT, Interrupt.
0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
(gdb) c
Continuing.
```

## break命令
break命令即我们添加断点的命令，可以简写成b，我们可以使用以下方式添加断点：
```text
## 在函数名为functionname的入口处添加一个断点 
break functionname                 
## 在当前文件行号为LineNo处添加一个断点
break LineNo                            
## 在filename文件行号为LineNo处添加一个断点
break filename:LineNo            
```

这三种方式是添加断点的常用方式。举个例子，对于一般的Linux程序来说，main函数是程序入口函数，redis-server也不例外，如果我们知道了函数名字，可以直接利用函数名添加一个断点，这里以在main函数处设置断点为例，执行如下命令：
```text
(gdb) b main
Breakpoint 1 at 0x436abd: file server.c, line 5001.
```

添加好了以后，我们使用run命令重启程序，就可以触发这个断点了，gdb会停在断点处。
```text
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) 
```

redis-server默认端口号是6379，我们知道无论上层如何封装，最终这个端口号肯定是通过操作系统的socket API bind函数绑定上去的，我们通过文件搜索，找到调用这个函数的文件，其位于anet.c 455行。

我们使用break命令在这个地方加一个断点：
```text
(gdb) b anet.c:455
Breakpoint 2 at 0x42aab2: file anet.c, line 455.
```

由于程序绑定端口号是redis-server启动时初始化的，为了能触发这个断点，我们再次使用run命令重启下这个程序，程序第一次会触发main函数处的断点，我们输入continue命令继续运行，接着触发anet.c:455行的断点：
```text
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".

Breakpoint 1, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) c
Continuing.
32219:C 27 May 2020 13:06:17.326 # oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
32219:C 27 May 2020 13:06:17.326 # Redis version=6.0.3, bits=64, commit=00000000, modified=0, pid=32219, just started
32219:C 27 May 2020 13:06:17.326 # Warning: no config file specified, using the default config. In order to specify a config file use /root/redis-7.2.3/src/redis-server /path/to/redis.conf

Breakpoint 2, anetListen (err=0x568c48 <server+680> "", s=6, sa=0x6173f0, len=28, backlog=511) at anet.c:455
455         if (bind(s,sa,len) == -1) {
(gdb)  
```

现在断点停在455行，所以当前文件就是anet.c，我们可以直接使用break + 行号添加断点，例如我们可以在458行、464行、466行分别加一个断点，看看这个函数执行完毕后走哪个return语句退出，通过“b 行号” 来添加三个断点，操作如下所示：
```text
454     static int anetListen(char *err, int s, struct sockaddr *sa, socklen_t len, int backlog) {
455         if (bind(s,sa,len) == -1) {
456             anetSetError(err, "bind: %s", strerror(errno));
457             close(s);
458             return ANET_ERR;
459         }
(gdb) l
460
461         if (listen(s, backlog) == -1) {
462             anetSetError(err, "listen: %s", strerror(errno));
463             close(s);
464             return ANET_ERR;
465         }
466         return ANET_OK;
467     }
468
469     static int anetV6Only(char *err, int s) {
(gdb) b 458
Breakpoint 3 at 0x42aafc: file anet.c, line 458.
(gdb) b 464
Breakpoint 4 at 0x42ab48: file anet.c, line 464.
(gdb) b 466
Breakpoint 5 at 0x42ab4f: file anet.c, line 466.
(gdb) 
```

添加好三个断点后，我们使用continue命令继续运行程序，程序运行到466行中断下来，说明该函数执行了466 行的return语句：
```text
(gdb) c
Continuing.

Breakpoint 5, anetListen (err=0x568c48 <server+680> "", s=6, sa=0x6173f0, len=28, backlog=511) at anet.c:466
466         return ANET_OK;
(gdb) 
```

至此，先调用bind函数再调用listen函数， redis-server绑定了端口并成功开启侦听。我们可以再打开一个shell窗口验证一下，结果证实6379端口确实已经处于侦听状态了：

```text
[root@localhost redis-7.2.3]# lsof -i -Pn | grep redis
redis-ser 32219   root    6u  IPv6 36564164      0t0  TCP *:6379 (LISTEN)
```

## tbreak命令
break命令添加是一个永久断点，tbreak命令也是添加一个断点，第一个字母“t”的意思是“temporarily ”（“临时的”），也就是说这个命令加的断点是临时的，所谓临时断点，就是一旦该断点触发一次后，就会自动被删除。添加断点的方法与上文中介绍的break命令一模一样，这里不再赘述。

```text
(gdb) tbreak main
Temporary breakpoint 1 at 0x436abd: file server.c, line 5001.
(gdb) r
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".

Temporary breakpoint 1, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
//...省略redis-server启动成功的部分输出
24068:M 27 May 2020 21:19:31.141 # Server initialized                           
```

上述代码，我们使用tbreak命令在**main()**函数处添加了一个临时断点，第一次启动程序，触发断点后，再次重新运行程序，不再触发断点，因为这个临时断点已经被删除，此时redis-server直接启动成功。

## backtrace与frame命令
backtrace，可简写成bt，用来查看当前所在线程的调用堆栈。现在我们的redis-server中断在anet.c:466行，我们可以通过backtrace命令来查看当前的调用堆栈：
```text
(gdb) bt
#0 anetListen (err=0x568c48 <server+680> "", s=6, sa=0x6173f0, len=28, backlog=511) at anet.c:466
#1 0x000000000042ad26 in_anetTcpServer (err=0x568c48 ‹server+680> "", port=6379, bindaddr=0x0, af=10, backlog=511) at anet.c:501
#2 0x000000000042adf8 in anetTcp6Server (err=0x568c48 «server+680> "', port=6379, bindaddr=0x0, backlog=511) at anet.c:524
#3 0x0000000000430bba in listenToPort (port=6379, fds=0x568b14 «server+372>, count=0x568b54 <server+436>) at server.c: 2648
#4 000000000004312ef in initServer () at server.c: 2792
#5 0x000000000043712a in main (argc=1, argv=0x7fffffffe308) at server.c:5128
(gdb)
```

这里一共有6层堆栈，堆栈编号分别是**#0 ~ #5，最顶层是main函数，最底层是我们断点所在的anetListen函数，如果我们想切换到其他堆栈处，可以使用frame**命令，frame命令的使用方法如下：
```text
frame 堆栈编号（编号不用加#）
```

frame命令可以简写成f。我们这里依次切换至堆栈**#1**、#2、#3、#4、#5，然后再切换回**#0** ，操作如下所示：
```text
(gdb) bt
anetListen (err=0x568c48 <server+680> "', s=6, sa=0x6173f0, len=28, backlog=511) at anet.c:466
#2 x000004276 in anetteserver (er-56848 server+680 , port-6379, bindad =0, Back0, -32) at and at snet. c: 502
#3 0x0000000000430bba in listenToPort (port=6379, fds=0x568b14 «server+372>, count=0x568b54 ‹server+436>) at server.c: 2648
#4 0x00000000004312ef in initServer () at server.c:2792
#5 0x000000000043712a in main (argc=1, argv=0x7fffffffe308) at server.c:5128
(gdb) f 1
#1 0x000000000042ad26 in_anetTcpServer (err=0x568c48 ‹server+680> "", port=6379, bindaddr=0x0, af=10, backlog=511) at anet.c:501
501 if (anetListen(err,s,p->ai_addr,p->ai_addrlen,backlog) = ANET_ERR) s = ANET_ERR;
(gdb) f 2
#2 0x000000000042adf8 in anetTcp6Server (err=0x568c48 <server+680> ", port=6379, bindaddr=0x0, backlog=511) at anet.c:524
524 return _anetTcpServer(err, port, bindaddr, AF_INET6, backlog);
(gdb) f 3
#3 0x0000000000430bba in listenToPort (port=6379, fds=0x568b14 ‹server+372>, count=0x568b54 <server+436>) at server.c: 2648
2648 fds [*count] = anetTcp6Server(server.neterr, port,NULL,
(gdb) f 4
#4 0x00000000004312ef in initServer () at server.c:2792
2792 listenToPort (server port,server.ipfd,&server.ipfd_count) == C_ERR)
(gdb) f 5
#5 0x000000000043712a in main (argc=1, argv=0x7fffffffe308) at server.c:5128
5128 initServer();
(gdb) f 0
#0 anetListen (err=0x568c48 <server+680> '", s=6, sa=0x6173f0, len=28, backlog=511) at anet.c:466
466 return ANET_OK;
(gdb)
```

通过对上面的各个堆栈查看，我们可以得出这里的调用层级关系，即main函数在5128行调用了initServer函数，initServer又在2792行调用了listenToPort函数，listenToPort又在2648行调用了anetTcp6Server函数，anetTcp6Server又在524行调用了**_anetTcpServer函数，_anetTcpServer函数在501行调用了anetListen**，当前断点正好位于anetListen函数中。

## info break、enable、disable、delete命令
当我们在程序中加了很多断点以后，我们想查看我们加了哪些断点时，我们可以使用info break 命令（简写成info b）：
```text
(gdb) info b
Num              Type                Disp Enb Address           what
1               breakpoint              keep y              0x0000000000436abd in main at server.c:5001 
                breakpoint already hit 1 time
2               breakpoint              keep y              0x000000000042aab2 in anetListen at anet.c:455
                breakpoint already hit 1 time               
3               breakpoint              keep y              0000000000042aafc in anetListen at anet.c: 458 
4               breakpoint              keep y              0x000000000042ab48 in anetListen at anet.c: 464 
5               breakpoint              keep y              0x000000000042ab4f in anetListen at anet.c: 466
                breakpoint already hit 1 time
(gdb)
```

通过上图所示，我们可以得到如下信息：目前一共增加了6个断点，断点1、2、5已经触发一次，其他断点未触发；每个断点的位置（所在的文件和行号）、内存地址、断点启用和禁用状态信息也一目了然地展示出来。

如果我们想禁用某个断点，使用disable 断点编号就可以禁用这个断点了，被禁用的断点，不会再被触发；被禁用的断点可以使用enable 断点编号重新开启。

```text
(gdb) disable 1
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep n   0x0000000000436abd in main at server.c:5001
        breakpoint already hit 1 time
2       breakpoint     keep y   0x000000000042aab2 in anetListen at anet.c:455
        breakpoint already hit 1 time
3       breakpoint     keep y   0x000000000042aafc in anetListen at anet.c:458
4       breakpoint     keep y   0x000000000042ab48 in anetListen at anet.c:464
5       breakpoint     keep y   0x000000000042ab4f in anetListen at anet.c:466
        breakpoint already hit 1 time
```

使用disable 1以后，第一个断点的Enb一栏的值由y变成n，断点1不会再触发，即程序不会在main函数处中断，程序一直到断点2处才会停下来：
```text
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".
1599:C 27 May 2020 13:43:45.923 # oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
1599:C 27 May 2020 13:43:45.923 # Redis version=6.0.3, bits=64, commit=00000000, modified=0, pid=1599, just started
1599:C 27 May 2020 13:43:45.923 # Warning: no config file specified, using the default config. In order to specify a config file use /root/redis-7.2.3/src/redis-server /path/to/redis.conf

Breakpoint 2, anetListen (err=0x568c48 <server+680> "", s=6, sa=0x6173e0, len=28, backlog=511) at anet.c:455
455         if (bind(s,sa,len) == -1) {
(gdb) 
```

如果disable和enable命令不加断点编号，则分别表示禁用和启用所有断点：
```text
(gdb) disable
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep n   0x0000000000436abd in main at server.c:5001
2       breakpoint     keep n   0x000000000042aab2 in anetListen at anet.c:455
        breakpoint already hit 1 time
3       breakpoint     keep n   0x000000000042aafc in anetListen at anet.c:458
4       breakpoint     keep n   0x000000000042ab48 in anetListen at anet.c:464
5       breakpoint     keep n   0x000000000042ab4f in anetListen at anet.c:466
(gdb) enable
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000436abd in main at server.c:5001
2       breakpoint     keep y   0x000000000042aab2 in anetListen at anet.c:455
        breakpoint already hit 1 time
3       breakpoint     keep y   0x000000000042aafc in anetListen at anet.c:458
4       breakpoint     keep y   0x000000000042ab48 in anetListen at anet.c:464
5       breakpoint     keep y   0x000000000042ab4f in anetListen at anet.c:466
(gdb) 
```

使用delete 编号可以删除某个断点，如delete 2 3则表示要删除断点2和断点3：
```text
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000436abd in main at server.c:5001
2       breakpoint     keep y   0x000000000042aab2 in anetListen at anet.c:455
        breakpoint already hit 1 time
3       breakpoint     keep y   0x000000000042aafc in anetListen at anet.c:458
4       breakpoint     keep y   0x000000000042ab48 in anetListen at anet.c:464
5       breakpoint     keep y   0x000000000042ab4f in anetListen at anet.c:466
(gdb) delete 2 3
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000436abd in main at server.c:5001
4       breakpoint     keep y   0x000000000042ab48 in anetListen at anet.c:464
5       breakpoint     keep y   0x000000000042ab4f in anetListen at anet.c:466
(gdb) 
```

同样的道理，如果输入delete不加命令号则表示删除所有断点。

## list 命令
list命令和后面介绍的print命令是gdb调试中使用频率最高的命令。list命令用于查看当前断点附近的代码，可以简写成l。我们使用frame命令切换到上文的堆栈**#4处，然后输入list**命令查看下效果：

```text
(gdb) bt
#0 anetListen (err=0x568c48 «server+680> ', 5=6, sa=0x6173e0, len=28, backlog=511) at anet.c:455
#1 0x000000000042ad26 in _anetTcpServer （err=0x568c48 <server+680> ""，port=6379,bindaddr=0x0,af=10,backlog=511） at anet.C:501
#2 0x000000000042adf8 in anetTcp6Server （err=0x568c48 <server+680> ""，port=6379,bindaddr=0x@，backlog=511） at anet.C:524
#3 0x0000000000430bba in listenToPort （port=6379,fds=0x568b14 <server+372>，count=0x568b54 <server+436>） at server.C:2648
#4 0x00000000004312ef in initServer （） at server.c:2792
#5 0x000000000043712a in main（argc=1,argv=0x7fffffffe308） at server.C:5128
(gdb) f 4
#4 0x00000000004312ef in initServer () at server.c:2792
2792 listenToPort (server-port, server.ipfd,&server.ipfd_count) == C_ERR)
(gdb) l
2787            ｝
2788            server.db = malloc(sizeof (redisDb)*server.dbnum) ;
2789
2790            /* Open the TCP listening socket for the user commands. */
2791            if (server-port != 0 &&
2792                listenToPort (server-port,server.ipfd,&server.ipfd_count) = C_ERR)
2793                exit (1) ;
2794            if (server.tls_port != 0 &&
2795                listenToPort(server.tls_port,server.tlsfd,&server.tlsfd_count) == C_ERR)
2796                exit (1); 
(gdb)
```

我们的断点“停在”2792行，输入list命令以后，会显示2792行 前后10行代码（2787 ~ 2796行）。

再次输入list命令试一下：
```text
(gdb) l
2797
2798        /* Open the listening Unix domain socket. */
2799        if (server.unixsocket != NULL) {
2800            unlink(server.unixsocket); /* don't care if this fails */
2801            server.sofd = anetUnixServer(server.neterr,server.unixsocket,
2802                server.unixsocketperm, server.tcp_backlog);
2803            if (server.sofd == ANET_ERR) {
2804                serverLog(LL_WARNING, "Opening Unix socket: %s", server.neterr);
2805                exit(1);
2806            }
(gdb) l
2807            anetNonBlock(NULL,server.sofd);
2808        }
2809
2810        /* Abort if there are no listening sockets at all. */
2811        if (server.ipfd_count == 0 && server.tlsfd_count == 0 && server.sofd < 0) {
2812            serverLog(LL_WARNING, "Configured to not listen anywhere, exiting.");
2813            exit(1);
2814        }
2815
2816        /* Create the Redis databases, and initialize other internal state. */
(gdb) 
```

代码继续往后显示10行（2797 ~ 2806行），也就是说，第一次输入list命令会显示断点处前后的10行代码，继续输入list命令每次会接着向后显示10行代码，一直到文件结束处。

list +命令(即list 加号 )可以从当前代码位置向下显示10行代码（往文件末尾方向），这和连续输入多条list命令效果是一样的；list -命令(即list 减号)可以从当前代码位置向上显示10行代码（往文件开始方向）。操作效果如下所示：

```text
(gdb) l
2797
2798        /* Open the listening Unix domain socket. */
2799        if (server.unixsocket != NULL) {
2800            unlink(server.unixsocket); /* don't care if this fails */
2801            server.sofd = anetUnixServer(server.neterr,server.unixsocket,
2802                server.unixsocketperm, server.tcp_backlog);
2803            if (server.sofd == ANET_ERR) {
2804                serverLog(LL_WARNING, "Opening Unix socket: %s", server.neterr);
2805                exit(1);
2806            }
(gdb) list +
2807            anetNonBlock(NULL,server.sofd);
2808        }
2809
2810        /* Abort if there are no listening sockets at all. */
2811        if (server.ipfd_count == 0 && server.tlsfd_count == 0 && server.sofd < 0) {
2812            serverLog(LL_WARNING, "Configured to not listen anywhere, exiting.");
2813            exit(1);
2814        }
2815
2816        /* Create the Redis databases, and initialize other internal state. */
(gdb) list -
2797
2798        /* Open the listening Unix domain socket. */
2799        if (server.unixsocket != NULL) {
2800            unlink(server.unixsocket); /* don't care if this fails */
2801            server.sofd = anetUnixServer(server.neterr,server.unixsocket,
2802                server.unixsocketperm, server.tcp_backlog);
2803            if (server.sofd == ANET_ERR) {
2804                serverLog(LL_WARNING, "Opening Unix socket: %s", server.neterr);
2805                exit(1);
2806            }
(gdb) list -
2787        }
2788        server.db = zmalloc(sizeof(redisDb)*server.dbnum);
2789
2790        /* Open the TCP listening socket for the user commands. */
2791        if (server.port != 0 &&
2792            listenToPort(server.port,server.ipfd,&server.ipfd_count) == C_ERR)
2793            exit(1);
2794        if (server.tls_port != 0 &&
2795            listenToPort(server.tls_port,server.tlsfd,&server.tlsfd_count) == C_ERR)
2796            exit(1);
(gdb) 
```

list默认显示的行数目可以通过修改gdb的相关配置来实现，由于我们一般不会修改这个配置值，因此这里就不介绍了。

list不仅可以显示当前断点处的代码，也可以显示其他文件某一行的代码，更多的用法读者可以在gdb中输入help list查看：

```text
(gdb) help list
List specified function or line.
With no argument, lists ten more lines after or around previous listing.
"list -" lists the ten lines before a previous ten-line listing.
One argument specifies a line, and ten lines are listed around that line.
Two arguments with comma between specify starting and ending lines to list.
Lines can be specified in these ways:
  LINENUM, to list around that line in current file,
  FILE:LINENUM, to list around that line in that file,
  FUNCTION, to list around beginning of that function,
  FILE:FUNCTION, to distinguish among like-named static functions.
  *ADDRESS, to list around the line containing that address.
With two args if one is empty it stands for ten lines away from the other arg.
```

上面的帮助信息中，介绍了可以使用list FILE:LINENUM来显示某个文件的某一行处的代码实用性不大。我们使用gdb的目的是为了调试，所以我们更关心的是断点附近的代码，而不是通过gdb阅读代码，对于阅读代码，gdb不是一个好的工具。

## print和ptype 命令
print命令可以在我们调试过程中方便地查看变量的值，也可以修改当前内存中的变量值，print命令可以简写成p，我们切换到当前断点的到堆栈**#4**，然后打印一下三个变量。

```text
(gdb) bt
#0  anetListen (err=0x568c48 <server+680> "", s=6, sa=0x6173e0, len=28, backlog=511) at anet.c:455
#1  0x000000000042ad26 in _anetTcpServer (err=0x568c48 <server+680> "", port=6379, bindaddr=0x0, af=10, backlog=511) at anet.c:501
#2  0x000000000042adf8 in anetTcp6Server (err=0x568c48 <server+680> "", port=6379, bindaddr=0x0, backlog=511) at anet.c:524
#3  0x0000000000430bba in listenToPort (port=6379, fds=0x568b14 <server+372>, count=0x568b54 <server+436>) at server.c:2648
#4  0x00000000004312ef in initServer () at server.c:2792
#5  0x000000000043712a in main (argc=1, argv=0x7fffffffe308) at server.c:5128
(gdb) f 4
#4  0x00000000004312ef in initServer () at server.c:2792
2792            listenToPort(server.port,server.ipfd,&server.ipfd_count) == C_ERR)
(gdb) p server.port
$1 = 6379
(gdb) p server.ipfd 
$2 = {0 <repeats 16 times>}
(gdb) p server.ipfd_count
$3 = 0
(gdb) 
```

这里使用print命令分别打印出server.port、server.ipfd、server.ipfd_count的值。其中server.ipfd显示“{0 <repeats 16 times>}”，这是 gdb 显示字符串、字符数组特有的方式，当一个字符串变量、字符数组或者连续的内存值重复若干次，gdb就会以这种模式来显示，以节约显示空间。

print命令不仅可以输出变量值，也可以输出特定表达式计算结果值，甚至可以输出一些函数的执行结果值。

举个例子，我们可以输入p &server.port来输出server.port的地址，对于C++对象，我们可以通过p this来显示当前对象的地址，也可以通过p *this来列出当前对象的各个成员变量值，如果有三个变量可以相加（假设变量名分别叫a、b、c），我们可以使用p a+b+c来打印这三个变量的结果值。

假设func是一个可以执行的函数，p func()命令可以输出该变量的执行结果。以一个常用的情形为例，某个时刻，某个系统函数执行失败了，通过系统变量errno得到一个错误码，我们可以使用**p strerror(errno)**将这个错误码对应的文字信息打印出来，这样我们就不用费劲地去man手册上查找这个错误码对应的错误含义了。

print命令不仅可以输出表达式结果，同时也可以修改变量的值，我们尝试将上文中的端口号从6379改成6400试试：
```text
(gdb) p server.port=6400
$4 = 6400
(gdb) p server.port
$5 = 6400
(gdb) 

```

当然，一个变量值修改后能否起作用要看这个变量的具体位置和作用了，举个例子，对于表达式int a = b / c ; 如果将c修改成0，那么程序就会产生除零异常。再例如，对于如下代码：
```text
int j = 100;
for (int i = 0; i < j; ++i) {
    printf("i = %d\n", i);
}
```

如果在循环的过程中，利用print命令将j的大小由100改成1000，那么这个循环将输出i的值1000次。

print输出变量值时可以指定输出格式，命令使用格式如下：

```text
print /format variable
```

format 常见的取值有：
```text
o octal 八进制显示
x hex 十六进制显示
d decimal 十进制显示
u unsigned decimal 无符号十进制显示
t binary 二进制显示
f float 浮点值显示
a address 内存地址格式显示(与十六进制相似)
i instruction 指令格式显示
s string 字符串形式显示
z hex, zero padded on the left 十六进制左侧补0显示
```
完整的格式和用法读者可以在gdb中输入help x来查看。

演示如下：
```text
(gdb) p /x server.port
$6 = 0x1900
(gdb) p /s server.port
$7 = 6400
(gdb) p /o server.port
$8 = 014400
(gdb) p /i server.port
Format letter "i" is meaningless in "print" command.
(gdb) p /t server.port
$9 = 1100100000000
(gdb) p /f server.port
$10 = 8.96831017e-42
(gdb) p /t server.port
$11 = 1100100000000
(gdb) p /a server.port
$12 = 0x1900
(gdb) 
```

总结起来，利用print命令，我们不仅可以查看程序运行过程中的各个变量的状态值，也可以通过临时修改变量的值来控制程序的行为。

gdb还有另外一个命令叫ptype，顾名思义，其含义是“print type”，就是输出一个变量的类型。例如我们试着输出redis堆栈**#4处的变量server和变量server.port**的类型：

```text
(gdb) ptype server
type = struct redisServer {
    pid_t pid;
    char *configfile;
    char *executable;
    char **exec_argv;
    int dynamic_hz;
    int config_hz;
    ...省略部分字段...
(gdb) ptype server.port
type = int  
```

可以看到，对于一个复合数据类型的变量，ptype不仅列出了这个变量的类型（这里是一个名叫redisServer的结构体），而且详细地列出了每个成员变量的字段名，有了这个功能，在调试时我们不必去代码文件中翻看某个变量的类型定义。

## info和thread命令
info命令，我们在前面在介绍**"使用 info break 查看当前的断点"时介绍过，这是一个复合指令，还可以用来查看当前进程的所有线程的运行情况。我们还是以redis-server这个进程为例演示一下，我们使用delete命令删掉所有断点，然后使用run命令重启一下redis-server**，等程序正常启动后，我们按Ctrl+C （代码中的**^C**）将程序中断下来，然后使用info threads来查看进程当前所有线程信息和这些线程分别中断在何处。

```text
(gdb) delete
(gdb) run
[1]    10880 segmentation fault (core dumped)  gdb redis-server
(gdb) info threads
Thread 1 (process 10880):
Id Target Id
Frame
1 Thread 0x7ffff7feb740 (LWP 5029) "redis-server" 0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
2 Thread X7f038708 (L 5831) "00-f@×08774b965 in phread.cond waiteL 2.3.2 () from /us/11064/10pthread.50. 0
3 Thread 0x7fffefbb7700 (LWP 5032) "bio_lazy_free" 0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
4 Thread 0x7fffefbb7700 (LWP 5032) "bio_lazy_free" 0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
(gdb) 
```

通过info threads的输出，我们可以知道redis-server正常启动后，一共产生4个线程，其中一个主线程、三个工作线程，线程编号（Id那一列）分别是1、2、3、4。三个工作线程（2、3、4）分别阻塞在Linux API pthread_cond_wait处，而主线程（1）阻塞在epoll_wait处。注意，第一栏的名称虽然叫Id，但第一栏的数值并不是线程的Id，第三栏有个括号，内容如**( LWP 5029)，这个5029** 才是当前线程真正的线程Id。那LWP是什么意思呢？在早期的Linux系统的内核里面，其实不存在真正的线程实现，当时所有的线程都是用进程来实现的，这些模拟线程的进程被称为Light Weight Process（轻量级进程），后来版本Linux系统内核有了真正的线程实现，但这个名字却被保留下来。

读者可能有个疑问：怎么知道线程1就是主线程，线程2、3、4就是工作线程呢？是不是因为线程1前面有个星号（*）？错了，线程编号前面的星号表示的是gdb当前作用于哪个线程，而不是说标了星号就是主线程。当前有4个线程，也就有4个调用堆栈，如果我们此时输入backtrace命令查看调用堆栈，由于gdb当前作用于线程1，因此backtrace命令显示的是线程1的调用堆栈：

```text
(gdb) bt
#0  0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
#1  0x0000000000428a9e in aeApiPoll (eventLoop=0x5e5770, tvp=0x7fffffffe140) at ae_epoll.c:112
#2  0x00000000004297e2 in aeProcessEvents (eventLoop=0x5e5770, flags=27) at ae.c:447
#3  0x0000000000429ab6 in aeMain (eventLoop=0x5e5770) at ae.c:539
#4  0x00000000004372bb in main (argc=1, argv=0x7fffffffe308) at server.c:5175
(gdb) 
```

看到了吧？堆栈**#4的main函数也证明了线程编号为1**的线程是主线程。

那么如何切换到其他线程呢？我们可以通过thread 线程编号 命令切换到指定的线程。例如，我们想切换到线程2 ，只需要输入thread 2命令即可，接着输入bt命令就能查看这个线程的调用堆栈：

```text
(gdb) thread 2
[Switching to thread 2 (Thread 0x7ffff0bb9700 (LWP 5030))]
#0  0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
(gdb) bt
#0  0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
#1  0x00000000004991c0 in bioProcessBackgroundJobs (arg=0x0) at bio.c:190
#2  0x00007ffff74b8dd5 in start_thread () from /usr/lib64/libpthread.so.0
#3  0x00007ffff71e202d in clone () from /usr/lib64/libc.so.6
(gdb) 
```

所以利用info threads命令我们就可以调试多线程程序呢。当然用gdb调试多线程程序还有一个很麻烦的问题。当我们把gdb当前作用的线程切换到线程2上之后，线程2前面就被加上了星号：

```text
(gdb) info threads
Id              Target Id                                   Frame
1       Thread 0x7ffff7feb740 (LWP 5029) "redis-server"     0x00007ffff712603
*2      Thread 0x7ffff0bb9700 (LWP 5030) "bio_close_file"   0x00007ffff74bc965
3       Thread 0x7ffff03b8700 (LWP 5031) "bio_aof_fsync"    0x00007ffff74bc965
4       Thread 0x7fffefbb7700 (LWP 5032) "bio_lazy_free"    0x00007ffff74bc965
(gdb) |
```

info命令还可以用来查看当前函数的参数值，组合命令是： info args。我们找个函数来试一下这个命令：
```text
(gdb) info threads
  Id   Target Id                                         Frame 
  1    Thread 0x7ffff7feb740 (LWP 5029) "redis-server"   0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
* 2    Thread 0x7ffff0bb9700 (LWP 5030) "bio_close_file" 0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
  3    Thread 0x7ffff03b8700 (LWP 5031) "bio_aof_fsync"  0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
  4    Thread 0x7fffefbb7700 (LWP 5032) "bio_lazy_free"  0x00007ffff74bc965 in pthread_cond_wait@@GLIBC_2.3.2 () from /usr/lib64/libpthread.so.0
(gdb) thread 1
[Switching to thread 1 (Thread 0x7ffff7feb740 (LWP 5029))]
#0  0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
(gdb) bt
#0  0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
#1  0x0000000000428a9e in aeApiPoll (eventLoop=0x5e5770, tvp=0x7fffffffe140) at ae_epoll.c:112
#2  0x00000000004297e2 in aeProcessEvents (eventLoop=0x5e5770, flags=27) at ae.c:447
#3  0x0000000000429ab6 in aeMain (eventLoop=0x5e5770) at ae.c:539
#4  0x00000000004372bb in main (argc=1, argv=0x7fffffffe308) at server.c:5175
(gdb) f 2
#2  0x00000000004297e2 in aeProcessEvents (eventLoop=0x5e5770, flags=27) at ae.c:447
447             numevents = aeApiPoll(eventLoop, tvp);
(gdb) info args
eventLoop = 0x5e5770
flags = 27
(gdb)
```

上述代码片段先切回至主线程1，然后切换到堆栈 **#2**，堆栈 **#2调用处的函数是aeProcessEvents** ，这个函数一共有两个参数，分别是eventLoop和tvp，使用info args命令可以输出这两个参数值，参数eventLoop是一个指针类型的参数，对于指针类型的参数，gdb默认会输出该变量的指针值，如果想输出该指针指向的对象的值，可以在变量名前面加上*解引用操作符，这里使用p *eventLoop即可：

```text
(gdb) p *eventLoop
$16 = {maxfd = 7, setsize = 10128, timeEventNextId = 1, lastTime = 1590562419, events = 0x7ffff7f9b010, fired = 0x5e57d0, timeEventHead = 0x617440, stop = 0, apidata = 0x5f9460, beforesleep = 0x42f866 <beforeSleep>, 
  aftersleep = 0x42fa28 <afterSleep>, flags = 0}
(gdb)
```

如果还要查看其成员值，继续使用变量名->字段名 即可（如p eventLoop->maxfd）。

info命令的功能远非上面介绍的三种，可以在gdb中输入help info查看更多info组合命令的用法。

## next、step、until、finish、return、jump命令
之所以把这几个命令放在一起是因为它们是我们用gdb调试程序时最常用的几个控制流命令。next命令简写成n，作用是让gdb跳到下一行代码，这里跳到下一行代不是说一定跳到代码最近的下一行，而是根据程序逻辑跳转到相应的位置。举个例子：
```text
int a = 0;
if (a == 9)
{
    print("a is equal to 9.\n");
}

int b = 10;
print("b = %d.\n", b);
```

如果当gdb中断在上述代码第2行，此时输入next命令时，gdb将跳到第7行，因为这里的if条件不满足。

> 这里和大家说一个小技巧，在刚接触gdb调试时，使用next单步调试时，总是喜欢不断地输入n以跳到下一行去。其实在gdb命令行界面如果直接按下回车键，默认是将最近一条命令重新执行一遍，所以，当我们使用next命令单步调试时，不必反复输入n命令，输入一次n之后再次想输入next明亮直接按回车就可以了。

```text
Breakpoint 7, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) n
5003        setlocale(LC_COLLATE,"");
(gdb) n
5004        tzset(); /* Populates 'timezone' global. */
(gdb) n
5005        zmalloc_set_oom_handler(redisOutOfMemoryHandler);
(gdb) n
5006        srand(time(NULL)^getpid());
(gdb) n
5007        gettimeofday(&tv,NULL);
(gdb) n
5008        crc64_init();
(gdb) n
5011        getRandomBytes(hashseed,sizeof(hashseed));
(gdb) n
5012        dictSetHashFunctionSeed(hashseed);
```

上面的执行过程等价于输入第一个n后直接回车：
```text
Breakpoint 7, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) n
5003        setlocale(LC_COLLATE,"");
(gdb) 
5004        tzset(); /* Populates 'timezone' global. */
(gdb) 
5005        zmalloc_set_oom_handler(redisOutOfMemoryHandler);
(gdb) 
5006        srand(time(NULL)^getpid());
(gdb) 
5007        gettimeofday(&tv,NULL);
(gdb) 
5008        crc64_init();
(gdb) 
5011        getRandomBytes(hashseed,sizeof(hashseed));
(gdb) 
5012        dictSetHashFunctionSeed(hashseed); 
```

next命令用调试的术语叫”单步步过“（step over），即遇到函数调用不进入函数体内部而直接跳过；而下面说的step命令就是”单步步入“（step into），顾名思义，就是遇到函数调用，进入函数内部。step可简写成s。举个例子，在redis-server的main函数中有个叫spt_init(argc, argv) 的函数调用，当我们停在这一行时，输入s时将进入这个函数内部。

```text
//为了说明问题本身，除去不相关的干扰，代码有删减
int main(int argc, char **argv) {
    struct timeval tv;
    int j;

    spt_init(argc, argv);

    setlocale(LC_COLLATE,"");
    tzset(); /* Populates 'timezone' global. */
    zmalloc_set_oom_handler(redisOutOfMemoryHandler);
    srand(time(NULL)^getpid());
    gettimeofday(&tv,NULL);
    //省略部分无关代码...
}
```

我们演示一下，先使用b main在main函数处加一个断点，然后使用r命令重新跑一下程序，会触发刚才加在main函数处的断点，然后使用n命令让程序走到spt_init(argc, argv)函数调用处，再输入s命令就可以进入该函数了：
```text
(gdb) b main
Breakpoint 8 at 0x436abd: file server.c, line 5001.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".

Breakpoint 8, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
(gdb) s
spt_init (argc=1, argv=0x7fffffffe308) at setproctitle.c:153
153             char **envp = environ;
(gdb) l
148             return 0;
149     } /* spt_copyargs() */
150
151
152     void spt_init(int argc, char *argv[]) {
153             char **envp = environ;
154             char *base, *end, *nul, *tmp;
155             int i, error;
156
157             if (!(base = argv[0]))
(gdb) 
```

说到step命令，我们还有一个需要注意的地方，就是当函数的参数也是函数调用时，这个时候，我们使用step命令会依次进入各个函数，那么顺序是什么呢？举个例子，看下面这段代码：

```text
int func1(int a, int b)
{
    int c = a + b;
    c += 2;
    return c;
}

int func2(int p, int q)
{
    int t = q * p;
	return t * t;
}

int func3(int m, int n) 
{
    return m + n;    
}

int main()
{
    int c;
    c = func3(func1(1, 2),  func2(8, 9));
    printf("c=%d.\n", c);
    return 0; 
}
```

上述代码，程序入口函数是main函数，在func3使用func1和func2的返回值作为自己的参数，在func3函数处step命令，会先进入哪个函数呢？这个就需要补充一个知识点了——函数调用方式，我们常用的函数调用方式有 **__cdecl**、__stdcall，C++的非静态成员函数的调用方式是 **__thiscall**，这些调用方式，函数参数的传递本质上是函数参数的入栈的过程，而这三种调用方式参数的入栈顺序都是从右往左的，所以，这段代码中并没有显式标明函数的调用方式，所以采用默认 **__cdecl方式。当在func3输入step先进入的是func2**，当从func2返回时再次输入step命令会接着进入func1，当从func1返回时，此时两个参数已经计算出来了，这时候会最终进入func3。希望读者理解这一点，这样我们在遇到这样的代码时，才能根据需要进入我们想要的函数中去调试。

在实际调试的时候，我们在某个函数中调试一会儿后，我们不需要再一步步执行到函数返回处，我们希望直接执行完当前函数并回到上一层调用处，我们可以使用finish命令。与finish命令类似的还有return命令，return命令的作用是结束执行当前函数，同时可以指定该函数的返回值。这里需要注意一下二者的区别：finish命令会执行完整个函数体，然后正常返回到上层调用；而return命令是立即从函数当前位置结束并返回到上层调用，也就是说，如果使用了return命令，当前函数还有剩余的代码未执行完毕，也不会再执行了。我们用一个例子来验证一下：

```text
  #include <stdio.h>

  int func()
  {
     int a = 9;
     printf("a=%d.\n", a);
 
    int b = 8;
    printf("b=%d.\n", b);
    return a + b;
 }

 int main()
 {
    int c = func();
    printf("c=%d.\n", c);

    return 0;
 }
```

在main函数处加一个断点，然后运行程序，在main函数里面使用step命令进入func函数，接着单步到代码变量b赋值前，直接输入return命令，这样func函数剩余的代码就不会接着执行了，所以printf("b=%d.\n", b); 这一行就没有输出出来。同时由于我们没有在return命令中指定这个函数的返回值，所以最终在main函数中得到的变量c的值是一个脏数据。这就验证了上面的结论：return命令立即从函数当前位置结束并返回到上一层调用。验证过程如下：
```text
(gdb) b main
Breakpoint 1 at 0x40057d: file test.c, line 15.
(gdb) r
Starting program: /root/testreturn/test 

Breakpoint 1, main () at test.c:15
15          int c = func();
Missing separate debuginfos, use: debuginfo-install glibc-2.17-196.el7_4.2.x86_64
(gdb) s
func () at test.c:5
5           int a = 9;
(gdb) n
6           printf("a=%d.\n", a);
(gdb) n
a=9.
8           int b = 8;
(gdb) return
Make func return now? (y or n) y
#0  0x0000000000400587 in main () at test.c:15
15          int c = func();
(gdb) n
16          printf("c=%d.\n", c);
(gdb) n
c=-134250496.
18          return 0;
(gdb) 
```

我们再次用return命令指定一个值试一下，这样我们得到变量c的值应该就是我们指定的值。验证过程如下：

```text
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/testreturn/test 

Breakpoint 1, main () at test.c:15
15          int c = func();
(gdb) s
func () at test.c:5
5           int a = 9;
(gdb) n
6           printf("a=%d.\n", a);
(gdb) n
a=9.
8           int b = 8;
(gdb) return 9999
Make func return now? (y or n) y
#0  0x0000000000400587 in main () at test.c:15
15          int c = func();
(gdb) n
16          printf("c=%d.\n", c);
(gdb) n
c=9999.
18          return 0;
(gdb) p c
$1 = 9999
(gdb) 
```

仔细观察上述代码，我们用return命令修改了函数的返回值，当我们使用print命令打印c的值的时候，c的值也确实被我们修改成9999。

我们再对比一下使用finish命令来结束函数执行的结果：

```text
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/testreturn/test 

Breakpoint 1, main () at test.c:15
15          int c = func();
(gdb) s
func () at test.c:5
5           int a = 9;
(gdb) n
6           printf("a=%d.\n", a);
(gdb) n
a=9.
8           int b = 8;
(gdb) finish
Run till exit from #0  func () at test.c:8
b=8.
0x0000000000400587 in main () at test.c:15
15          int c = func();
Value returned is $3 = 17
(gdb) n
16          printf("c=%d.\n", c);
(gdb) n
c=17.
18          return 0;
(gdb) 
```

结果和我们预期的一样，finish正常结束我们的函数，剩余的代码也会被正常执行。因此c的值是 17。

### until命令
实际调试时，还有一个叫until的命令，简写成u，我们使用这个命令让程序运行到指定行停下来。还是以redis-server的代码为例。
```text
Breakpoint 1, initServer () at server.c:2742
2742        signal(SIGHUP, SIG_IGN);
(gdb) l
2737    }
2738
2739    void initServer(void) {
2740        int j;
2741
2742        signal(SIGHUP, SIG_IGN);
2743        signal(SIGPIPE, SIG_IGN);
2744        setupSignalHandlers();
2745
2746        if (server.syslog_enabled) {
(gdb) l
2747            openlog(server.syslog_ident, LOG_PID | LOG_NDELAY | LOG_NOWAIT,
2748                server.syslog_facility);
2749        }
2750
2751        /* Initialization after setting defaults from the config system. */
2752        server.aof_state = server.aof_enabled ? AOF_ON : AOF_OFF;
2753        server.hz = server.config_hz;
2754        server.pid = getpid();
2755        server.current_client = NULL;
2756        server.fixed_time_expire = 0;
(gdb) l
2757        server.clients = listCreate();
2758        server.clients_index = raxNew();
2759        server.clients_to_close = listCreate();
2760        server.slaves = listCreate();
2761        server.monitors = listCreate();
2762        server.clients_pending_write = listCreate();
2763        server.clients_pending_read = listCreate();
2764        server.clients_timeout_table = raxNew();
2765        server.slaveseldb = -1; /* Force to emit the first SELECT command. */
2766        server.unblocked_clients = listCreate();
(gdb) l
2767        server.ready_keys = listCreate();
2768        server.clients_waiting_acks = listCreate();
2769        server.get_ack_from_slaves = 0;
2770        server.clients_paused = 0;
2771        server.events_processed_while_blocked = 0;
2772        server.system_memory_size = zmalloc_get_memory_size();
2773
2774        if (server.tls_port && tlsConfigure(&server.tls_ctx_config) == C_ERR) {
2775            serverLog(LL_WARNING, "Failed to configure TLS. Check logs for more info.");
2776            exit(1);
(gdb) 
```

上述代码是redis-server中initServer函数的部分代码，位于文件server.c中，当gdb停在2740行（注意：这里的行号以gdb调试器中显示的行号为准，不是源码文件中的行号，由于存在条件编译，部分代码可能不会被编译进可执行文件中，所以实际的调试符号文件中的行号与源码文件中的行号可能不会完全一致），此时我们可以通过输入u 2774命令让gdb直接跳到2774行，这样就能快速执行完2740 ~ 2774行中间的代码（不包括2774行）。当然，我们也可以先在2774行加一个断点，然后使用continue命令运行到这一行来达到同样的效果，但是使用until命令显然更方便。

```text
(gdb) b 2740
Note: breakpoint 1 also set at pc 0x4310d4.
Breakpoint 2 at 0x4310d4: file server.c, line 2742.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
...省略部分输出...
Breakpoint 1, initServer () at server.c:2742
2742        signal(SIGHUP, SIG_IGN);
(gdb) u 2774
initServer () at server.c:2774
2774        if (server.tls_port && tlsConfigure(&server.tls_ctx_config) == C_ERR) {
(gdb) 
```

### jump命令基本用法是：

> jump <location>

location可以是程序的行号或者函数的地址，jump会让程序执行流跳转到指定位置执行，当然其行为也是不可控制的，例如跳过了某个对象的初始化代码，直接执行操作该对象的代码，那么可能会导致程序崩溃或其他意外行为。jump命令可以简写成j，但是不可以简写成jmp，使用该命令时有一个注意事项，即如果jump跳转到的位置没有设置断点，那么gdb执行完跳转操作后，会继续往下执行。举个例子：
```text
1 int somefunc()
2 {
3   //代码A
4   //代码B
5   //代码C
6   //代码D
7   //代码E
8   //代码F    
9 }
```

假设我们的断点初始位置在行号3处（代码A），这个时候我们使用jump 6，那么程序会跳过代码B和C的执行，执行完代码D（ 跳转点），程序并不会停在代码6处，而是继续执行后续代码，因此如果我们想查看执行跳转处的代码后的结果，需要在行号6、7或8处设置断点。

jump命令除了跳过一些代码的执行外，还有一个妙用就是可以执行一些我们想要执行的代码，而这些代码在正常的逻辑下可能并不会执行，当然根据实际的程序逻辑可能会产生一些非预期结果。举个例子，假设现在有如下代码：

```text
1  #include <stdio.h>
2  int main()
3  {
4    int a = 0;
5    if (a != 0)
6    {
7    	printf("if condition\n");
8    }
9    else
10   {
11   	printf("else condition\n");
12   }
13
14   return 0;
15 }
```

我们在行号4 、14处设置一个断点，当触发行号4处的断点后，正常情况下程序执行流会走else分支，我们可以使用jump 7强行让程序执行if分支，接着gdb会因触发行号14处的断点而停下来，此时我们接着执行jump 11，程序会将else分支中的代码重新执行一遍。整个操作过程如下：

```text
[root@localhost testcore]# gdb test
Reading symbols from /root/testcore/test...done.
(gdb) b main
Breakpoint 1 at 0x400545: file main.cpp, line 4.
(gdb) b 14
Breakpoint 2 at 0x400568: file main.cpp, line 14.
(gdb) r
Starting program: /root/testcore/test 

Breakpoint 1, main () at main.cpp:4
4	   int a = 0;
Missing separate debuginfos, use: debuginfo-install glibc-2.17-260.el7.x86_64 libgcc-4.8.5-36.el7.x86_64 libstdc++-4.8.5-36.el7.x86_64
(gdb) jump 7
Continuing at 0x400552.
if condition

Breakpoint 2, main () at main.cpp:14
14	   return 0;
(gdb) jump 11
Continuing at 0x40055e.
else condition

Breakpoint 2, main () at main.cpp:14
14	   return 0;
(gdb) c
Continuing.
[Inferior 1 (process 13349) exited normally]
(gdb)
```

redis-server在入口函数main处调用initServer ，我们使用“b initServer” 、“b 2753”、“b 2755”分别在这个函数入口处、2753行、2755行增加三个断点，然后使用run命令重新运行一下程序，触发第一个断点后，输入c命令继续运行，然后触发2753行处的断点，接着输入jump 2755，以下是操作过程：
```text
Breakpoint 3, initServer () at server.c:2742
2742        signal(SIGHUP, SIG_IGN);
(gdb) l
2737    }
2738
2739    void initServer(void) {
2740        int j;
2741
2742        signal(SIGHUP, SIG_IGN);
2743        signal(SIGPIPE, SIG_IGN);
2744        setupSignalHandlers();
2745
2746        if (server.syslog_enabled) {
(gdb) l
2747            openlog(server.syslog_ident, LOG_PID | LOG_NDELAY | LOG_NOWAIT,
2748                server.syslog_facility);
2749        }
2750
2751        /* Initialization after setting defaults from the config system. */
2752        server.aof_state = server.aof_enabled ? AOF_ON : AOF_OFF;
2753        server.hz = server.config_hz;
2754        server.pid = getpid();
2755        server.current_client = NULL;
2756        server.fixed_time_expire = 0;
(gdb) b 2753
Breakpoint 4 at 0x43112f: file server.c, line 2753.
(gdb) b 2755
Breakpoint 5 at 0x431146: file server.c, line 2755.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
...省略部分输出...
Breakpoint 3, initServer () at server.c:2742
2742        signal(SIGHUP, SIG_IGN);
(gdb) c
Continuing.

Breakpoint 4, initServer () at server.c:2753
2753        server.hz = server.config_hz;
(gdb) jump 2755
Continuing at 0x431146.

Breakpoint 5, initServer () at server.c:2755
2755        server.current_client = NULL;
(gdb) 
```

程序将2754行的代码跳过了，2754行的代码是获取当前进程id：

```text
2754 server.pid = getpid();
```

由于这一行被跳过了，所以server.pid的值应该是一个无效的值，我们可以使用print命令将这个值打印出来看一下：

```text
(gdb) p server.pid
$1 = 0
```

结果是0 ，这个0值是redis初始化时设置的。

> gdb的jump命令的作用与使用Visual Studio调试时，通过用鼠标将程序当前执行点从一个位置拖到另外一个位置效果一样。

## disassemble命令
在某些场景下，我们可能要通过查看某段代码的汇编指令去排查问题，或者在调试一些不含调试信息的 release 版程序时，只能通过反汇编代码去定位问题。此类场景下disassemble命令就派上用场了。disassemble会输出当前函数的汇编指令，例如在redis的initServer函数中执行该命令会输出initServer函数的汇编指令，操作如下：

```text
Breakpoint 1, initServer () at server.c:2742
2742        signal(SIGHUP, SIG_IGN);
(gdb) disassemble
Dump of assembler code for function initServer:
   0x00000000004310cb <+0>:     push   %rbp
   0x00000000004310cc <+1>:     mov    %rsp,%rbp
   0x00000000004310cf <+4>:     push   %rbx
  ...太多了，省略...
   0x0000000000431a93 <+2504>:  add    $0x18,%rsp
--Type <RET> for more, q to quit, c to continue without paging--
   0x0000000000431a97 <+2508>:  pop    %rbx
   0x0000000000431a98 <+2509>:  pop    %rbp
   0x0000000000431a99 <+2510>:  retq   
End of assembler dump.
(gdb)
```

gdb反汇编格式默认为AT&T格式，可以通过show disassembly-flavor查看当前反汇编格式。如果读者习惯阅读intel汇编格式，可以使用set disassembly-flavor intel命令来设置。操作如下：
```text
(gdb) set disassembly-flavor intel  
(gdb) disassemble                 
Dump of assembler code for function initServer:
   0x00000000004310cb <+0>:     push   rbp
   0x00000000004310cc <+1>:     mov    rbp,rsp
   0x00000000004310cf <+4>:     push   rbx
   0x00000000004310d0 <+5>:     sub    rsp,0x18
	...太多了，省略...
   0x0000000000431a93 <+2504>:  add    rsp,0x18
--Type <RET> for more, q to quit, c to continue without paging--
   0x0000000000431a97 <+2508>:  pop    rbx
   0x0000000000431a98 <+2509>:  pop    rbp
   0x0000000000431a99 <+2510>:  ret    
End of assembler dump.
(gdb) 
```

disassemble命令在程序崩溃后产生core文件，且无对应的调试符号时非常有用，此时我们可以通过分析汇编代码来排查一些问题。

## set args与show args命令
很多程序，需要我们传递命令行参数。在gdb调试中，很多人会觉得可以使用gdb filename args这种形式来给gdb调试的程序传递命令行参数，这样是不行的。正确的做法是在用gdb附加程序后，在使用run命令之前，使用set args 命令行参数来指定，还是以redis-server为例，redis启动时可以指定一个命令行参数，就是它的配置文件，它的默认配置文件位于redis-server这个文件的上一层目录，所以我们可以在gdb中这样传递这个参数：set args ../redis.conf，可以通过show args查看命令行参数是否设置成功。

```text
(gdb) set args ../redis.conf 
(gdb) show args
Argument list to give program being debugged when it is started is "../redis.conf ".
(gdb) 
```

如果单个命令行参数之间含有空格，可以使用引号将参数包裹起来。

```text
(gdb) set args "999 xx" "hu jj"
(gdb) show args
Argument list to give program being debugged when it is started is ""999 xx" "hu jj"".
(gdb) 
```

如果想清除掉已经设置好的命令行参数，使用set args不加任何参数即可。

```text
(gdb) set args
(gdb) show args
Argument list to give program being debugged when it is started is "".
(gdb) 
```

## watch命令
watch是一个强大的命令，它可以用来监视一个变量或者一段内存，当这个变量或者该内存处的值发生变化时，gdb就会中断下来。监视某个变量或者某个内存地址会产生一个“watch point”（观察点）。

有这样一个问题：有一个变量其值被意外的改掉了，通过单步调试或者挨个检查使用该变量的代码，工作量非常大，如何快速地定位到该变量在哪里被修改了？其实，面试官想要的答案是“数据断点”。那什么是数据断点呢，而watch命令就实际上可能会通过添加硬件断点来达到监视数据变化的目的。watch命令的使用方式是watch 变量名或内存地址，一个watch point一般有以下几种格式：

- 整形变量

```text
int i;  
watch i
```

- 指针类型

```text
char *p; 
watch p 与 watch *p
```

> 注意： watch p与watch *p是有区别的，前者是查看*(&p)， 是p变量本身；后者是p所指的内存的内容，一般是我们所需要的，我们大多数情况就是要看某内存地址上的数据是怎样变化的。

- watch一个数组或内存区间

```text
char buf[128];
watch buf
```

这里是对buf的128个数据进行了监视。

需要注意的是：当设置的观察点是一个局部变量时。局部变量无效后，观察点也会失效。例如在观察点失效时， gdb可能会提示如下信息：

> Watchpoint 2 deleted because the program has left the block in which its expression is valid. 

## display命令
display命令用于监视变量或者内存值，每次 gdb 中断下来都会自动输出这些被监视变量或内存的值。例如，某个程序有一些全局变量，每次触发断点gdb中断下来，希望可以自动输出这些全局变量的最新值，那么就可以使用display命令。display命令使用格式是display 变量名/内存地址/寄存器名。

```text
0x00007ffff71e2603 in epoll_wait () from /usr/lib64/libc.so.6
(gdb) display $ebx
1: $ebx = 24068
(gdb) display /x $ebx
2: /x $ebx = 0x5e04
(gdb) display $eax
3: $eax = -4
(gdb) b main
Breakpoint 2 at 0x436abd: file server.c, line 5001.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/redis-7.2.3/src/redis-server 
...省略部分输出...
Breakpoint 2, main (argc=1, argv=0x7fffffffe308) at server.c:5001
5001        spt_init(argc, argv);
1: $ebx = 0
2: /x $ebx = 0x0
3: $eax = 4418219
(gdb)
```

上述代码中，我们使用display命令分别监视寄存器ebp和寄存器eax，其中要求ebp寄存器分别使用十进制和十六进制两种形式输出其值，这样每次gdb中断下来都会自动把这些寄存器值输出出来，可以使用info display查看当前已经监视了哪些值，使用delete display清除全部被监视的变量，使用delete display 编号移除对指定变量的监视。操作演示如下：

```text
(gdb) delete display
Delete all auto-display expressions? (y or n) n
(gdb) delete display 3
(gdb) info display
Auto-display expressions now in effect:
Num Enb Expression
2:   y  $ebp
1:   y  $eax
```

## dir命令——让被调试的可执行程序匹配源代码
读者可能会遇到这样的场景：在使用gdb调试时，由于生成可执行文件的机器和实际执行该可执行程序的机器不是同一台机器，例如大多数企业产生目标服务程序的机器是编译机器，即发版机，然后把发版机产生的可执行程序拿到生产机器上去执行。这个时候，如果可执行程序产生了崩溃，我们用gdb调试core文件时，gdb会提示"No such file or directory"，如下所示：

```text
Program received signal SIGSEGV, Segmentation fault.
0x00000000004d5662 in CAsyncLog::crash () at /home/flamingoserver/base/AsyncLog.cpp:475
475     /home/flamingoserver/base/AsyncLog.cpp: No such file or directory.
```

或者由于一些原因，编译时的源码文件被挪动了位置，使用gdb调试时也会出现上述情况。

gcc/g++编译出来的可执行程序并不包含完整源码，-g只是加了一个可执行程序与源码之间的位置映射关系，我们可以通过dir命令重新定位这种关系。

dir命令使用格式：

```text
# 加一个源文件路径到当前路径的前面,指定多个路径，可以使用”:”
dir SourcePath1:SourcePath2:SourcePath3
```

SourcePath1、SourcePath2、SourcePath3指的就是需要设置的源码目录，gdb会依次去这些目录搜索相应的源文件。

以上面的错误提示为例，原来的AsyncLog.cpp文件位于/home/flamingoserver/base/目录，由于这个目录被挪动了，所以gdb提示找不到该文件。现在假设这个文件被移动到/home/flamingoserver1/base/目录。那么我们只需要在gdb调试中执行dir /home/flamingoserver1/base/即可重定向可执行程序与源码的位置关系：

```text
(gdb) dir /home/flamingoserver1/base/    
Source directories searched: /home/flamingoserver1/base:$cdir:$cwd
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /chatserver 
...省略部分输出...
Program received signal SIGSEGV, Segmentation fault.
0x00000000004d5662 in CAsyncLog::crash () at /home/flamingoserver/base/AsyncLog.cpp:475
475         *p = 0;
(gdb) list
470     }
471
472     void CAsyncLog::crash()
473     {
474         char* p = nullptr;
475         *p = 0;
476     }
477
478     void CAsyncLog::writeThreadProc()
479     {
(gdb) 
```

使用dir命令重新定位源文件的位置之后， gdb就不会再提示这样的错误了，此时我们也可以使用gdb其他命令（如list命令）查看源码了。

如果要查看当前设置了哪些源码搜索路径，可以使用show dir命令：

```text
(gdb) show dir
Source directories searched: /home/flamingoserver1/base:$cdir:$cwd
```

dir命令不加参数表示清空当前已设置的源码搜索路径：
```text
(gdb) dir
Reinitialize source path to empty? (y or n) y
Source directories searched: $cdir:$cwd
(gdb) show dir
Source directories searched: $cdir:$cwd
(gdb) 
```
