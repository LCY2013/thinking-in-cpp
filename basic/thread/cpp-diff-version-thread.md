# C++ 11/14/17 线程同步对象

在 C/C++ 语言中直接使用操作系统提供的多线程资源同步 API 虽然功能强大，但毕竟存在诸多限制，且同样的代码却不能同时兼容
Windows 和 Linux 两个平台；再者 C/C++ 这种传统语言的使用份额正在被 Java、python、go 等语言慢慢蚕食，很大一部分原因是 C/C++
这门编程语言在一些功能上缺少“完备性”，如对线程同步技术的支持，而这些功能在像 Java、python、go 中是标配。因此 C++ 11
标准新加入了很多现代语言标配的东西，其中线程资源同步对象就是其中很重要的一部分。本小节将讨论 C++ 11 标准中新增的用于线程同步的
std::mutex 和 std::condition_variable 对象的用法，有了它们我们就可以写出跨平台的多线程程序了。

## std::mutex 系列

关于 mutex 的基本概念上文已经介绍过了，这里不再赘述。

C++ 11/14/17 中提供了如下 mutex 系列类型：

| 互斥量                   | 版本    | 作用                                      |
|-----------------------|-------|-----------------------------------------|
| mutex                 | C++11 | 最基本的互斥量                                 |
| timed_mutex           | C++11 | 有超时机制的互斥量                               |
| recursive_mutex       | C++11 | 可重入的互斥量                                 |
| recursive_timed_mutex | C++11 | 结合 timed_mutex 和 recursive_mutex 特点的互斥量 |
| shared_timed_mutex    | C++14 | 具有超时机制的可共享互斥量                           |
| shared_mutex          | C++17 | 共享的互斥量                                  |

这个系列的对象均提供了加锁（lock）、尝试加锁（trylock）和解锁（unlock）的方法，我们以 std::mutex 为例来看一段示例代码：

```c++
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

// protected by g_num_mutex
int g_num = 0;  
std::mutex g_num_mutex;
 
void slow_increment(int id) 
{
    for (int i = 0; i < 3; ++i) {
        g_num_mutex.lock();
        ++g_num;
        std::cout << id << " => " << g_num << std::endl;
        g_num_mutex.unlock();
		
		//sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
 
int main()
{
    std::thread t1(slow_increment, 0);
    std::thread t2(slow_increment, 1);
    t1.join();
    t2.join();
	
	return 0;
}
```

上述代码中，创建了两个线程 t1 和 t2，在线程函数的 for 循环中调用 std::mutex.lock() 和 std::mutex.unlock() 对全局变量 g_num
进行保护。编译程序并输出结果如下：

```shell
[root@localhost testmultithread]# g++ -g -o mutex c11mutex.cpp -std=c++0x -lpthread
[root@localhost testmultithread]# ./mutex 
0 => 1
1 => 2
0 => 3
1 => 4
1 => 5
0 => 6
```

> 注意：如果你在 Linux 下编译和运行程序，在编译时你需要链接 pthread 库，否则能够正常编译但是运行时程序会崩溃，崩溃原因：
> terminate called after throwing an instance of 'std::system_error' what(): Enable multithreading to use std::thread:
> Operation not permitted

为了避免死锁， std::mutex.lock() 和 std::mutex::unlock()
方法需要成对使用，但是如上文介绍的如果一个函数中有很多出口，而互斥体对象又是需要在整个函数作用域保护的资源，那么在编码时因为忘记在某个出口处调用
std::mutex.unlock 而造成死锁，上文中推荐使用利用 RAII 技术封装这两个接口，其实 C++ 11 标准也想到了整个问题，因为已经为我们提供了如下封装：

| 互斥量管理       | 版本    | 作用          |
|-------------|-------|-------------|
| lock_guard  | C++11 | 基于作用域的互斥量管理 |
| unique_lock | C++11 | 更加灵活的互斥量管理  |
| shared_lock | C++14 | 共享互斥量的管理    |
| scope_lock  | C++17 | 多互斥量避免死锁的管理 |

我们这里以 std::lock_guard 为例：
```c++
void func()
{
	std::lock_guard<std::mutex> guard(mymutex);
	//在这里放被保护的资源操作
}
```

mymutex 的类型是 std::mutex，在 guard 对象的构造函数中，会自动调用 mymutex.lock() 方法加锁，当该函数出了作用域后，调用 guard 对象时析构函数时会自动调用 mymutex.unlock() 方法解锁。

注意： mymutex 生命周期必须长于函数 func 的作用域，很多人在初学这个利用 RAII 技术封装的 std::lock_guard 对象时，可能会写出这样的代码：

```c++
//错误的写法，这样是没法在多线程调用该函数时保护指定的数据的。
void func()
{
	std::mutex m;
	std::lock_guard<std::mutex> guard(m);
	//在这里放被保护的资源操作
}
```

另外，如果某个线程已经对一个std::mutex对象调用了lock方法，该线程再次调用lock方法对这个std::mutex进行加锁时，其行为是未定义的，这是一个错误的做法。所谓“行为未定义”即在不同平台上可能会有不同的行为。

```c++
#include <mutex>

int main()
{
    std::mutex m;
    m.lock();
    m.lock();
    m.unlock();

    return 0;
}
```

实际测试时，上述代码重复调用 std::mutex.lock() 方法在 Windows 平台上会引起程序崩溃。

上述代码在 Linux 系统上运行时会阻塞在第二次调用 std::mutex.lock() 处，验证结果如下：
```shell
[root@localhost testmultithread]# g++ -g -o mutexlock mutexlock.cpp -std=c++0x -lpthread
[root@localhost testmultithread]# gdb mutexlock
Reading symbols from /root/testmultithread/mutexlock...done.
(gdb) r
Starting program: /root/testmultithread/mutexlock 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
^C
Program received signal SIGINT, Interrupt.
0x00007ffff7bcd4ed in __lll_lock_wait () from /lib64/libpthread.so.0
Missing separate debuginfos, use: debuginfo-install glibc-2.17-260.el7.x86_64 libgcc-4.8.5-36.el7.x86_64 libstdc++-4.8.5-36.el7.x86_64
(gdb) bt
#0  0x00007ffff7bcd4ed in __lll_lock_wait () from /lib64/libpthread.so.0
#1  0x00007ffff7bc8dcb in _L_lock_883 () from /lib64/libpthread.so.0
#2  0x00007ffff7bc8c98 in pthread_mutex_lock () from /lib64/libpthread.so.0
#3  0x00000000004006f7 in __gthread_mutex_lock (__mutex=0x7fffffffe3e0)
    at /usr/include/c++/4.8.2/x86_64-redhat-linux/bits/gthr-default.h:748
#4  0x00000000004007a2 in std::mutex::lock (this=0x7fffffffe3e0) at /usr/include/c++/4.8.2/mutex:134
#5  0x0000000000400777 in main () at mutexlock.cpp:7
(gdb) f 5
#5  0x0000000000400777 in main () at mutexlock.cpp:7
7	    m.lock();
(gdb) l
2	
3	int main()
4	{
5	    std::mutex m;
6	    m.lock();
7	    m.lock();
8	    m.unlock();
9	
10	    return 0;
11	}
(gdb)
```

我们使用 gdb 运行程序，然后使用 bt 命令看到程序确实阻塞在第二个m.lock() 的地方（代码第 7 行）。

总结起来，实际开发中，我们应该尽量避免同一个线程对一个已经调用lock方法的std::mutex对象再次调用lock方法。

```shell
//错误的做法
线程A对mutexM加锁 => 线程A对mutexM加锁

//正确的做法
线程A对mutexM加锁 => 线程A对mutexM解锁 => 线程A对mutexM加锁

//正确的做法
线程A对mutexM加锁 => 线程B对mutexM加锁（会阻塞）
```

		
		
		


