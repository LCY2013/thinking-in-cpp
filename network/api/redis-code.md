# Redis 源码

## 调试 redis 环境与准备工作
分析 redis 源码与 gdb 调试相结合的方式，来探究 redis 并逐步搞清楚 redis 的网络通信模块结构。

### 编译和调试环境
在 Linux 机器上调试，代码分析和阅读使用的是 Mac 机器上的 CLion。

- Linux 调试机器：CentOS 7.0，gdb 8.3

- 代码阅读机器：macOS Catalina，CLion

- redis 版本：6.0.3

### redis 源码下载与编译
下载 redis 源码后解压并在 CentOS 系统中编译出带有调试符号可执行文件。

由于不同版本的代码可能存在与本章节内容中的行号不完全匹配的情况，为了方便读者阅读本文时与源码相对应，这里提供两个 redis-6.0.3 版本下载地址：

> github：https://github.com/balloonwj/redis-6.0.3

> 码云：https://gitee.com/balloonwj/redis-6.0.3

利用 gdb 启动 redis-server，进入 redis-6.0.3/src，其中 redis-server 和 redis-cli 是我们需要调试的程序。用 gdb 关联 redis-server，并设置通过命令行设置 redis.conf 文件的路径，然后启动，操作如下：

```shell
[root@myaliyun ~]# cd redis-6.0.3/src       
[root@myaliyun src]# gdb redis-server 
GNU gdb (GDB) 8.3
Reading symbols from redis-server...
(gdb) set args "../redis.conf"
(gdb) run
Starting program: /root/redis-6.0.3/src/redis-server "../redis.conf"
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".
7470:C 10 Jun 2020 20:32:19.625 # oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
7470:C 10 Jun 2020 20:32:19.625 # Redis version=6.0.3, bits=64, commit=00000000, modified=0, pid=7470, just started
7470:C 10 Jun 2020 20:32:19.625 # Configuration loaded
                _._                                                  
           _.-``__ ''-._                                             
      _.-``    `.  `_.  ''-._           Redis 6.0.3 (00000000/0) 64 bit
  .-`` .-```.  ```\/    _.,_ ''-._                                   
 (    '      ,       .-`  | `,    )     Running in standalone mode
 |`-._`-...-` __...-.``-._|'` _.-'|     Port: 6379
 |    `-._   `._    /     _.-'    |     PID: 7470
  `-._    `-._  `-./  _.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |           http://redis.io        
  `-._    `-._`-.__.-'_.-'    _.-'                                   
 |`-._`-._    `-.__.-'    _.-'_.-'|                                  
 |    `-._`-._        _.-'_.-'    |                                  
  `-._    `-._`-.__.-'_.-'    _.-'                                   
      `-._    `-.__.-'    _.-'                                       
          `-._        _.-'                                           
              `-.__.-'                                               

7470:M 10 Jun 2020 20:32:19.627 # WARNING: The TCP backlog setting of 511 cannot be enforced because /proc/sys/net/core/somaxconn is set to the lower value of 128.
7470:M 10 Jun 2020 20:32:19.627 # Server initialized
7470:M 10 Jun 2020 20:32:19.627 # WARNING overcommit_memory is set to 0! Background save may fail under low memory condition. To fix this issue add 'vm.overcommit_memory = 1' to /etc/sysctl.conf and then reboot or run the command 'sysctl vm.overcommit_memory=1' for this to take effect.
7470:M 10 Jun 2020 20:32:19.627 # WARNING you have Transparent Huge Pages (THP) support enabled in your kernel. This will create latency and memory usage issues with Redis. To fix this issue run the command 'echo never > /sys/kernel/mm/transparent_hugepage/enabled' as root, and add it to your /etc/rc.local in order to retain the setting after a reboot. Redis must be restarted after THP is disabled.
[New Thread 0x7ffff0bb9700 (LWP 7475)]
[New Thread 0x7ffff03b8700 (LWP 7476)]
[New Thread 0x7fffefbb7700 (LWP 7477)]
[New Thread 0x7fffef3b6700 (LWP 7478)]
[New Thread 0x7fffeebb5700 (LWP 7479)]
[New Thread 0x7fffee3b4700 (LWP 7480)]
7470:M 10 Jun 2020 20:32:19.634 * Ready to accept connections


```

以上是 redis-server 的启动成功后的画面。

再开一个 session，再次进入 redis 源码所在的 src 目录，然后使用 gdb 启动 redis 客户端 redis-cli：
```shell
[root@myaliyun src]# gdb redis-cli
GNU gdb (GDB) 8.3
Reading symbols from redis-cli...
(gdb) r
Starting program: /root/redis-6.0.3/src/redis-cli 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib64/libthread_db.so.1".
127.0.0.1:6379> 

```

### 通信示例与术语约定
由于主要目的是为了学习和研究 redis 的网络通信模块，并不关心 redis 其他一些内容，因此为了方便说明问题，使用的一个简单的通信实例，即通过 redis-cli 产生一个 key 为 "hello"，value 为 "world" 的 key-value 数据，然后得到 redis-server 的响应。我们通过这样一个实例来研究 redis 的网络通信模块。

```shell
127.0.0.1:6379> set hello world
OK
127.0.0.1:6379>
```

为了方便行文，我们先约定几个技术术语：

- listenfd，即侦听 fd，指的是网络通信中作为服务器端的一方，用于绑定 ip 和端口号，并调用 listen 函数启动侦听操作的 socket 对象；

- clientfd，即客户端 fd，指的是网络通信中作为服务器端的一方，调用 accept 函数接受连接返回的、与某路客户端连接对应的 socket；

- connfd，指的是网络通信中作为客户端一方，创建后可以调用 connect 函数去连接服务器的 socket。

Linux 系统上 socket 也是一种文件描述符（File Descriptor），因此也称之为 fd。






















