# gdb调试多进程程序——nginx
这里说的多进程程序指的是一个进程使用Linux系统调用fork函数产生的子进程，没有相互关联的进程调试就是gdb调试单个进程，前面已经详细讨论过了。

在实际的应用中，有一类应用会通过Linux函数fork出新的子进程。以nginx为例，nginx对客户端的连接是采用多进程模型，当nginx接受客户端连接后，创建一个新的进程来处理该连接上的信息来往。新产生的进程与原进程互为父子关系。那么如何用gdb调试这样父子进程呢？一般有两种方法：

## 方法一
用gdb先调试父进程，等子进程被fork出来后，使用gdb attach到子进程上去。当然，这里需要再重新打开一个 shell 窗口用于调试，gdb attach的用法在前面已经介绍过了。

我们这里以调试 nginx 服务为例。

从nginx官网（链接4）下载最新的nginx源码，然后编译安装（笔者写作此书时，nginx最新稳定版本是 1.18.0）。
```text
## 下载nginx源码
[root@localhost root]# wget http://nginx.org/download/nginx-1.18.0.tar.gz
--2020-07-05 17:22:10--  http://nginx.org/download/nginx-1.18.0.tar.gz
Resolving nginx.org (nginx.org)... 95.211.80.227, 62.210.92.35, 2001:1af8:4060:a004:21::e3
Connecting to nginx.org (nginx.org)|95.211.80.227|:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 1039530 (1015K) [application/octet-stream]
Saving to: ‘nginx-1.18.0.tar.gz’

nginx-1.18.0.tar.gz                            100%[===================================================================================================>]   1015K   666KB/s    in 1.5s    

2020-07-05 17:22:13 (666 KB/s) - ‘nginx-1.18.0.tar.gz’ saved [1039530/1039530]

## 解压nginx
[root@localhost root]# tar zxvf nginx-1.18.0.tar.gz

## 编译nginx
[root@localhost root]# cd nginx-1.18.0
[root@localhost nginx-1.18.0]# ./configure --prefix=/usr/local/nginx
[root@localhost nginx-1.18.0]make CFLAGS="-g -O0"

## 安装，这样nginx就被安装到/usr/local/nginx/目录下
[root@localhost nginx-1.18.0]make install
```

> 注意：使用make命令编译时我们为了让生成的nginx带有调试符号信息同时关闭编译器优化，我们设置了"-g -O0"选项。

启动nginx：

```text
[root@localhost sbin]# cd /usr/local/nginx/sbin
[root@localhost sbin]# ./nginx -c /usr/local/nginx/conf/nginx.conf
[root@localhost sbin]# lsof -i -Pn | grep nginx
nginx      5246            root    9u  IPv4 22252908      0t0  TCP *:80 (LISTEN)
nginx      5247          nobody    9u  IPv4 22252908      0t0  TCP *:80 (LISTEN)
```

如上所示，nginx默认会开启两个进程，在我的机器上以root用户运行的nginx进程是父进程，进程号5246，以nobody用户运行的进程是子进程，进程号5247。我们在当前窗口使用gdb attach 5246命令将gdb附加到nginx主进程上去。

```text
[root@localhost sbin]# gdb attach 5246
...省略部分输出信息...
0x00007fd42a103c5d in sigsuspend () from /lib64/libc.so.6
Missing separate debuginfos, use: yum debuginfo-install glibc-2.28-72.el8_1.1.x86_64 libxcrypt-4.1.1-4.el8.x86_64 pcre-8.42-4.el8.x86_64 sssd-client-2.2.0-19.el8.x86_64 zlib-1.2.11-10.el8.x86_64
(gdb)
```

此时我们就可以调试nginx父进程了，例如使用bt命令查看当前调用堆栈：
```text
(gdb) bt
#0  0x00007fd42a103c5d in sigsuspend () from /lib64/libc.so.6
#1  0x000000000044ae32 in ngx_master_process_cycle (cycle=0x1703720) at src/os/unix/ngx_process_cycle.c:164
#2  0x000000000040bc05 in main (argc=3, argv=0x7ffe49109d68) at src/core/nginx.c:382
(gdb) f 1
#1  0x000000000044ae32 in ngx_master_process_cycle (cycle=0x1703720) at src/os/unix/ngx_process_cycle.c:164
164             sigsuspend(&set);
(gdb) l
159                 }
160             }
161
162             ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");
163
164             sigsuspend(&set);
165
166             ngx_time_update();
167
168             ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
(gdb)
```

使用f 1命令切换到当前调用堆栈#1，我们可以发现nginx父进程的主线程挂起在src/core/nginx.c:382处。

此时你可以使用c命令让程序继续运行起来，也可以添加断点或者做一些其他的调试操作。

再开一个shell窗口，使用gdb attach 5247将gdb附加到nginx子进程：

```text
[root@localhost sbin]# gdb attach 5247
...部署输出省略...
0x00007fd42a1c842b in epoll_wait () from /lib64/libc.so.6
Missing separate debuginfos, use: yum debuginfo-install glibc-2.28-72.el8_1.1.x86_64 libblkid-2.32.1-17.el8.x86_64 libcap-2.26-1.el8.x86_64 libgcc-8.3.1-4.5.el8.x86_64 libmount-2.32.1-17.el8.x86_64 libselinux-2.9-2.1.el8.x86_64 libuuid-2.32.1-17.el8.x86_64 libxcrypt-4.1.1-4.el8.x86_64 pcre-8.42-4.el8.x86_64 pcre2-10.32-1.el8.x86_64 sssd-client-2.2.0-19.el8.x86_64 systemd-libs-239-18.el8_1.2.x86_64 zlib-1.2.11-10.el8.x86_64
(gdb)
```

我们使用bt命令查看一下子进程的主线程的当前调用堆栈：
```text
(gdb) bt
#0  0x00007fd42a1c842b in epoll_wait () from /lib64/libc.so.6
#1  0x000000000044e546 in ngx_epoll_process_events (cycle=0x1703720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:800
#2  0x000000000043f317 in ngx_process_events_and_timers (cycle=0x1703720) at src/event/ngx_event.c:247
#3  0x000000000044c38f in ngx_worker_process_cycle (cycle=0x1703720, data=0x0) at src/os/unix/ngx_process_cycle.c:750
#4  0x000000000044926f in ngx_spawn_process (cycle=0x1703720, proc=0x44c2e1 <ngx_worker_process_cycle>, data=0x0, name=0x4cfd70 "worker process", respawn=-3)
    at src/os/unix/ngx_process.c:199
#5  0x000000000044b5a4 in ngx_start_worker_processes (cycle=0x1703720, n=1, type=-3) at src/os/unix/ngx_process_cycle.c:359
#6  0x000000000044acf4 in ngx_master_process_cycle (cycle=0x1703720) at src/os/unix/ngx_process_cycle.c:131
#7  0x000000000040bc05 in main (argc=3, argv=0x7ffe49109d68) at src/core/nginx.c:382
(gdb) f 1
#1  0x000000000044e546 in ngx_epoll_process_events (cycle=0x1703720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:800
800         events = epoll_wait(ep, event_list, (int) nevents, timer);
(gdb)
```

可以发现子进程挂起在src/event/modules/ngx_epoll_module.c:800的epoll_wait函数处。我们在epoll_wait函数返回后（src/event/modules/ngx_epoll_module.c:804）加一个断点，然后使用c命令让nginx子进程继续运行。

```text
800         events = epoll_wait(ep, event_list, (int) nevents, timer);
(gdb) list
795         /* NGX_TIMER_INFINITE == INFTIM */
796
797         ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
798                        "epoll timer: %M", timer);
799
800         events = epoll_wait(ep, event_list, (int) nevents, timer);
801
802         err = (events == -1) ? ngx_errno : 0;
803
804         if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
(gdb) b 804
Breakpoint 1 at 0x44e560: file src/event/modules/ngx_epoll_module.c, line 804.
(gdb) c
Continuing.
```

接着我们在浏览器里面访问nginx的站点，如果是本机就是127.0.0.1，由于默认端口是80，所以不用指定端口号。
```text
http://你的ip地址:80
等价于
http://你的ip地址
```

此时我们回到nginx子进程的调试界面发现断点被触发：
```text
Breakpoint 1, ngx_epoll_process_events (cycle=0x1703720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:804
804         if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
(gdb) 
```

使用bt命令可以获得此时的调用堆栈：
```text
(gdb) bt
#0  ngx_epoll_process_events (cycle=0x1703720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:804
#1  0x000000000043f317 in ngx_process_events_and_timers (cycle=0x1703720) at src/event/ngx_event.c:247
#2  0x000000000044c38f in ngx_worker_process_cycle (cycle=0x1703720, data=0x0) at src/os/unix/ngx_process_cycle.c:750
#3  0x000000000044926f in ngx_spawn_process (cycle=0x1703720, proc=0x44c2e1 <ngx_worker_process_cycle>, data=0x0, name=0x4cfd70 "worker process", respawn=-3)
    at src/os/unix/ngx_process.c:199
#4  0x000000000044b5a4 in ngx_start_worker_processes (cycle=0x1703720, n=1, type=-3) at src/os/unix/ngx_process_cycle.c:359
#5  0x000000000044acf4 in ngx_master_process_cycle (cycle=0x1703720) at src/os/unix/ngx_process_cycle.c:131
#6  0x000000000040bc05 in main (argc=3, argv=0x7ffe49109d68) at src/core/nginx.c:382
(gdb) 
```

使用info threads命令可以查看子进程所有线程信息，我们发现nginx子进程只有一个主线程：

```text
(gdb) info threads
  Id   Target Id                                Frame 
* 1    Thread 0x7fd42b17c740 (LWP 5247) "nginx" ngx_epoll_process_events (cycle=0x1703720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:804
(gdb) 
```

nginx父进程不处理客户端请求，处理客户端请求的逻辑在子进程中，当单个子进程客户端请求数达到一定数量时，父进程会重新fork一个新的子进程来处理新的客户端请求，也就是说子进程数量可以有多个，你可以开多个shell窗口，使用gdb attach到各个子进程上去调试。

总结起来，我们可以使用这种方法添加各种断点调试nginx的功能，慢慢就能熟悉nginx的各个内部逻辑了。

然而，方法一存在一个缺点，即程序已经启动了，我们只能使用gdb观察程序在这之后的行为，如果我们想调试程序从启动到运行起来之间的执行流程，方法一可能不太适用。有些读者可能会说：用gdb附加到进程后，加好断点，然后使用run命令重启进程，这样就可以调试程序从启动到运行起来之间的执行流程了。问题是这种方法不是通用的，因为对于多进程服务模型，有些父子进程有一定的依赖关系，是不方便在运行过程中重启的。这个时候方法二就比较合适了。

## 方法二
gdb调试器提供一个选项叫follow-fork，通过set follow-fork mode来设置：当一个进程fork出新的子进程时，gdb是继续调试父进程（取值是parent）还是子进程（取值是child），默认是父进程（取值是parent）。

```text
# fork之后gdb attach到子进程
set follow-fork child
# fork之后gdb attach到父进程，这是默认值
set follow-fork parent
```

我们可以使用show follow-fork mode查看当前值：

```text
(gdb) show follow-fork mode
Debugger response to a program call of fork or vfork is "child".
```

我们还是以调试nginx为例，先进入nginx可执行文件所在的目录，将方法一中的nginx服务停下来：
```text
[root@localhost sbin]# cd /usr/local/nginx/sbin/
[root@localhost sbin]# ./nginx -s stop
```

nginx源码中存在这样的逻辑，这个逻辑会在程序main函数处被调用：
```c
//src/os/unix/ngx_daemon.c:13行
ngx_int_t
ngx_daemon(ngx_log_t *log)
{
    int  fd;

    switch (fork()) {
    case -1:
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "fork() failed");
        return NGX_ERROR;
	
	//fork出来的子进程走这个case
    case 0:
        break;
	
	//父进程中fork返回值是子进程的PID，大于0，因此走这个case
	//因此主进程会退出
    default:
        exit(0);
    }

    //...省略部分代码...
}
```

如上述代码中注释所示，为了不让主进程退出，我们在nginx的配置文件中增加一行：

> daemon off;

这样nginx就不会调用ngx_daemon函数了。

接下来，我们执行gdb nginx，然后通过设置参数将配置文件nginx.conf传给待调试的nginx进程：

```text
Quit anyway? (y or n) y
[root@localhost sbin]# gdb nginx 
...省略部分输出...
Reading symbols from nginx...done.
(gdb) set args -c /usr/local/nginx/conf/nginx.conf
(gdb)
```

接着输入run命令尝试运行nginx：
```text
(gdb) run
Starting program: /usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf
[Thread debugging using libthread_db enabled]
...省略部分输出信息...
[Detaching after fork from child process 7509]
```

如前文所述，gdb遇到fork指令时默认会attach到父进程去，因此上述输出中有一行提示”Detaching after fork from child process 7509“，我们按Ctrl + c将程序中断下来，然后输入 bt 命令查看当前调用堆栈，输出的堆栈信息和我们在方法一中看到的父进程的调用堆栈一样，说明gdb在程序fork之后确实attach了父进程：

```text
^C
Program received signal SIGINT, Interrupt.
0x00007ffff6f73c5d in sigsuspend () from /lib64/libc.so.6
(gdb) bt
#0  0x00007ffff6f73c5d in sigsuspend () from /lib64/libc.so.6
#1  0x000000000044ae32 in ngx_master_process_cycle (cycle=0x71f720) at src/os/unix/ngx_process_cycle.c:164
#2  0x000000000040bc05 in main (argc=3, argv=0x7fffffffe4e8) at src/core/nginx.c:382
(gdb) 
```

如果想让gdb在fork之后去attach子进程，我们可以在程序运行之前设置 set follow-fork child，然后使用run命令重新运行程序。

```text
(gdb) set follow-fork child 
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
[Attaching after Thread 0x7ffff7fe7740 (LWP 7664) fork to child process 7667]
[New inferior 2 (process 7667)]
[Detaching after fork from parent process 7664]
[Inferior 1 (process 7664) detached]
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
^C
Thread 2.1 "nginx" received signal SIGINT, Interrupt.
[Switching to Thread 0x7ffff7fe7740 (LWP 7667)]
0x00007ffff703842b in epoll_wait () from /lib64/libc.so.6
(gdb) bt
#0  0x00007ffff703842b in epoll_wait () from /lib64/libc.so.6
#1  0x000000000044e546 in ngx_epoll_process_events (cycle=0x71f720, timer=18446744073709551615, flags=1) at src/event/modules/ngx_epoll_module.c:800
#2  0x000000000043f317 in ngx_process_events_and_timers (cycle=0x71f720) at src/event/ngx_event.c:247
#3  0x000000000044c38f in ngx_worker_process_cycle (cycle=0x71f720, data=0x0) at src/os/unix/ngx_process_cycle.c:750
#4  0x000000000044926f in ngx_spawn_process (cycle=0x71f720, proc=0x44c2e1 <ngx_worker_process_cycle>, data=0x0, name=0x4cfd70 "worker process", respawn=-3)
    at src/os/unix/ngx_process.c:199
#5  0x000000000044b5a4 in ngx_start_worker_processes (cycle=0x71f720, n=1, type=-3) at src/os/unix/ngx_process_cycle.c:359
#6  0x000000000044acf4 in ngx_master_process_cycle (cycle=0x71f720) at src/os/unix/ngx_process_cycle.c:131
#7  0x000000000040bc05 in main (argc=3, argv=0x7fffffffe4e8) at src/core/nginx.c:382
(gdb) 
```

我们接着按Ctrl + c将程序中断下来，然后使用bt命令查看当前线程调用堆栈，结果显示确实是我们在方法一中子进程的主线程所在的调用堆栈，这说明gdb确实attach到子进程了。

我们可以利用方法二调试程序fork之前和之后的任何逻辑，是一种较为通用的多进程调试方法。


