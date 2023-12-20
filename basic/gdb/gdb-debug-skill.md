# gdb实用调试技巧

## 将print输出的字符串或字符数组显示完整
当我们使用print命令打印一个字符串或者字符数组时，如果该字符串太长，print命令默认显示不全的，我们可以通过在gdb中输入set print element 0设置一下，这样再次使用print命令就能完整地显示该变量所有字符串了。

```text
void ChatSession::OnGetFriendListResponse(const std::shared_ptr<TcpConnection>& conn)
{
    std::string friendlist;
    MakeUpFriendListInfo(friendlist, conn);
    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendlist << "}";
    Send(msg_type_getofriendlist, m_seq, os.str());    
}
```

以上代码，当我们第一次用print命令输出friendlist变量值时，只能显示部分字符串。当使用set print element 0设置以后就能完整地显示出来了。

```text
(gdb) n
563         os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendlist << "}";
(gdb) p friendlist
$1 = "[{\"members\":[{\"address\":\"\",\"birthday\":19900101,\"clienttype\":0,\"customface\":\"\",\"facetype\":2,\"gender\":0,\"mail\":\"\",\"markname\":\"\",\"nickname\":\"bj_man\",\"phonenumber\":\"\",\"signature\":\"\",\"status\":0,\"userid\":4,"...
(gdb) set print element 0
(gdb) p friendlist       
$2 = "[{\"members\":[{\"address\":\"\",\"birthday\":19900101,\"clienttype\":0,\"customface\":\"\",\"facetype\":2,\"gender\":0,\"mail\":\"\",\"markname\":\"\",\"nickname\":\"bj_man\",\"phonenumber\":\"\",\"signature\":\"\",\"status\":0,\"userid\":4,\"username\":\"13811411052\"},{\"address\":\"\",\"birthday\":19900101,\"clienttype\":0,\"customface\":\"\",\"facetype\":0,\"gender\":0,\"mail\":\"\",\"markname\":\"\",\"nickname\":\"Half\",\"phonenumber\":\"\",\"signature\":\"\",\"status\":0,\"userid\":5,\"username\":\"15618326596\"},{\"address\":\"\",\"birthday\":19900101,\"clienttype\":0,\"customface\":\"\",\"facetype\":34,\"gender\":0,\"mail\":\"\",\"markname\":\"\",\"nickname\":\"云淡风轻\",\"phonenumber\":\"\",\"signature\":\"\",\"status\":0,\"userid\":7,\"username\":\"china001\"},...太长了，这里省略...
```

## 让被gdb调试的程序接收信号
看下面的代码：
```
void prog_exit(int signo)
{
    std::cout << "program recv signal [" << signo << "] to exit." << std::endl;
}

int main(int argc, char* argv[])
{
    //设置信号处理
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, prog_exit);
    signal(SIGTERM, prog_exit);

    int ch;
    bool bdaemon = false;
    while ((ch = getopt(argc, argv, "d")) != -1)
    {
        switch (ch)
        {
        case 'd':
            bdaemon = true;
            break;
        }
    }

    if (bdaemon)
        daemon_run();
        
    //省略无关代码...
 }
```

这个程序中，我们让程序在接收到Ctrl + c信号（对应信号值是SIGINT）时简单打印一行信息，当我们用gdb调试这个程序时，由于Ctrl + c默认会被gdb接收到（让调试器中断下来），导致我们无法模拟程序接收这一信号。解决这个问题有两种方式：

1. 在gdb中使用signal函数手动给我们的程序发送信号，这里就是signal SIGINT 。
2. 改变gdb信号处理的设置，通过handle SIGINT nostop print告诉gdb在接收到SIGINT时不要停止、并把该信号传递给调试目标程序 。

```text
(gdb)handle SIGINT nostop print pass
SIGINT is used by the debugger.
Are you sure you want to change it? (y or n) y 


Signal Stop Print Pass to program Description 
SIGINT No Yes Yes Interrupt
(gdb)  
```

## 明明函数存在，添加断点时却无效的解决方案
有时候，一个函数明明存在，并且我们的程序也存在调试符号，我们使用break functionName添加断点时，gdb却提示：

> Make breakpoint pending on future shared library load? y/n

即使我们输入y，添加的断点可能也不会被正确地触发。此时我们就需要改变添加断点的策略，使用该函数所在的代码文件和行号这种方式添加断点就能添加同样效果的断点。

## 调试中的断点
实际调试中，我们一般会用到三种断点：普通断点、条件断点和数据断点。

数据断点是被监视的内存值或者变量值发送变化时触发的断点，前面章节中介绍watch命令时添加的部分断点就是数据断点。

普通断点就是我们添加的断点除去条件断点和硬件断点以外的断点。

下面重点来介绍一下条件断点。所谓条件断点，就是满足某个条件才会触发的断点。这里先举一个直观的例子：

```text
1 void do_something_func(int i)
2 {
3    i ++;
4    i = 100 * i;
5 }
6
7 int main()
8 {
9    for(int i = 0; i < 10000; ++i)
10    {
11       do_something_func(i);
12    }
13 
14    return 0;
15 }
```

上述代码中，假如我们希望当变量i=5000时，进入do_something_func函数追踪一下这个函数的执行细节。此时我们可以修改代码增加一个i=5000的if条件，然后重新编译链接调试，这样显然比较麻烦，尤其是对于一些大型项目，每次重新编译链接都需要花一定的时间，而且调试完了还得把程序修改回来。有了条件断点，我们就不需要这么麻烦了。直接添加一个条件断点即可，添加条件断点的命令是：break [lineNo] if [condition]，其中lineNo是程序触发断点后需要停的位置，condition是断点触发的条件。我们这里可以写成break 11 if i==5000，这里的11就是调用do_something_fun 函数所在的行号。当然这里的行号必须是合理的行号，如果行号非法或者行号位置不合理也不会触发这个断点。

```text
(gdb) break 11 if i==5000       
Breakpoint 2 at 0x400514: file test1.c, line 10.
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /root/testgdb/test1 

Breakpoint 1, main () at test1.c:9
9          for(int i = 0; i < 10000; ++i)
(gdb) c
Continuing.

Breakpoint 2, main () at test1.c:11
11            do_something_func(i);
(gdb) p i
$1 = 5000
```

把i打印出来，gdb确实是在i=5000时停下来了。

添加条件断点，还有一个方法就是先添加一个普通断点，然后使用condition 断点编号 断点触发条件这样的格式来添加。我们使用这种方式添加一下上述断点：

```text
(gdb) b 11
Breakpoint 1 at 0x400514: file test1.c, line 11.
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000400514 in main at test1.c:11
(gdb) condition 1 i==5000
(gdb) r
Starting program: /root/testgdb/test1 
y

Breakpoint 1, main () at test1.c:11
11            do_something_func(i);
Missing separate debuginfos, use: debuginfo-install glibc-2.17-196.el7_4.2.x86_64
(gdb) p i
$1 = 5000
(gdb) 
```

同样的规则，如果断点编号不存在，也无法添加成功，gdb会提示断点不存在：

```text
(gdb) condition 2 i==5000
No breakpoint number 2.
```

## 自定义gdb调试命令
有些场景下，我们需要根据自己的程序情况，自定义一些可以在调试时输出我们程序特定信息的命令。这个在gdb中很容易做到，只要在Linux用户根目录下，root用户就是 **/root目录，非root用户，是/home/用户名这个目录。在上述目录中自定义一个.gdbinit文件即可，注意在Linux系统中这是一个隐藏文件，可以使用ls -a**命令查看；如果不存在，创建一个就可以。然后在这个文件中写上你自定义命令的shell脚本就可以。

这里以apache web服务器的源码为例（apache server的源码下载地址可参见链接5），在源码根目录下有个文件叫 .gdbinit，这个就是apache server自定义的gdb命令：
```text
# gdb macros which may be useful for folks using gdb to debug
# apache.  Delete it if it bothers you.

define dump_table
    set $t = (apr_table_entry_t *)((apr_array_header_t *)$arg0)->elts
    set $n = ((apr_array_header_t *)$arg0)->nelts
    set $i = 0
    while $i < $n
	if $t[$i].val == (void *)0L
	   printf "[%u] '%s'=>NULL\n", $i, $t[$i].key
	else
	   printf "[%u] '%s'='%s' [%p]\n", $i, $t[$i].key, $t[$i].val, $t[$i].val
	end
	set $i = $i + 1
    end
end

# 省略部分代码

# Set sane defaults for common signals:
handle SIGPIPE noprint pass nostop
handle SIGUSR1 print pass nostop
```

当然在这个文件的最底部，apache的配置了让gdb调试器不要处理SIGPIPE和SIGUSR1这两个信号，而是将这两个信号直接传递给被调试的程序本身（这里就是apache server）。

