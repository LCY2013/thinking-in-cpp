# thread杂项

## 如何确保创建的线程一定运行起来？
很少有人会注意到创建的线程如何确保一定运行起来了？很多人会说，对于使用系统 API 创建的线程，只需要判断一下创建的线程函数是否是调用成功的，这只做了一步，线程函数调用成功，也没法百分百保证线程函数一定运行起来了。

在一些“古老”或者“严谨”的项目中，你会发现这些代码创建线程时不仅判断线程创建函数是否调用成功，还会在线程函数中利用上文介绍的一些线程同步对象来通知线程的创建者线程是否创建成功。我们来看一段这样的代码：
```c++
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

std::mutex 				mymutex;
std::condition_variable mycv;
bool success = false;

void thread_func()
{
    {
        std::unique_lock<std::mutex> lock(mymutex);
        success = true;
        mycv.notify_all();
    }

    //实际的线程执行的工作代码放在下面
    //这里为了模拟方便，简单地写个死循环
    while (true)
    {

    }
}

int main()
{
    std::thread t(thread_func);

    //使用花括号减小锁的粒度
    {
        std::unique_lock<std::mutex> lock(mymutex);
        while (!success)
        {
            mycv.wait(lock);
        }
    }

    std::cout << "start thread successfully." << std::endl;

    t.join();

    return 0;
}
```

上述代码，发出一个创建新线程的请求后，立刻阻塞在一个条件变量上，工作线程如果成功运行起来，会发送条件变量信号告知主线程，这样主线程就知道新线程一定成功运行起来了。

基于以上思路，我们创建一组线程时，可以一个一个地创建，每成功运行一个新线程再创建下一个，确保线程组中的每一个线程都可以运行起来。示例代码如下：
```c++
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <memory>

std::mutex 				mymutex;
std::condition_variable mycv;
bool success = false;

void thread_func(int no)
{
    {
        std::unique_lock<std::mutex> lock(mymutex);
        success = true;
        mycv.notify_all();
    }

    std::cout << "worker thread started, threadNO: " << no << std::endl;
    //实际的线程执行的工作代码放在下面
    //这里为了模拟方便，简单地写个死循环
    while (true)
    {

    }
}

int main()
{
    std::vector<std::shared_ptr<std::thread>> threads;
    
    for (int i = 0; i < 5; ++i)
    {
        success = false;
        std::shared_ptr<std::thread> spthread;
        spthread.reset(new std::thread(thread_func, i));

        //使用花括号减小锁的粒度
        {
            std::unique_lock<std::mutex> lock(mymutex);
            while (!success)
            {
                mycv.wait(lock);
            }
        }

        std::cout << "start thread successfully， index: " << i << std::endl;

        threads.push_back(spthread);
    }

    for (auto& iter : threads)
    {
        iter->join();
    }

    return 0;
}
```

编译上述程序并运行，运行结果如下所示：
```shell
[root@localhost codes]# g++ -g -o makesurethreadgroup makesurethreadgroup.cpp -std=c++0x -lpthread
[root@localhost codes]# ./makesurethreadgroup 
worker thread started, threadNO: 0
start thread successfully， index: 0
worker thread started, threadNO: 1
start thread successfully， index: 1
worker thread started, threadNO: 2
start thread successfully， index: 2
worker thread started, threadNO: 3
start thread successfully， index: 3
worker thread started, threadNO: 4
start thread successfully， index: 4
```

可以看到，新线程挨个运行起来。当然，你不一定要使用条件变量，也可以使用其他类型的线程同步对象，如 Windows 平台的 Event 对象等等。

确保线程一定运行起来的做法时使用了两个词——“古老”和“严谨”，“严谨”不用多介绍，之所以说“古老”是因为现在在各种新型的项目中基本上再也看不到这种确保线程运行起来的做法了，许多年以前多线程编程开始流行起来的时候，那时由于软硬件的水平限制，加之很多开发人员对多线程编程技术的不熟悉，创建新线程时确保一个线程跑起来非常必要；而如今多线程编程已经如此的司空见惯，加上操作系统和 CPU 普遍对多线程技术的支持，我们再也不用写这样的“防御“代码了；甚至只要你正确使用线程创建函数，我们实际编码时连线程函数的返回值都不必判断，基本上可以认为新线程一定会创建成功，且线程可以正常跑起来。

## 多线程使用锁实践经验总结

### 减少锁的使用
实际开发中能不使用锁尽量不使用锁，当然这不是绝对的，如果使用锁也能满足性能要求，使用锁也无妨，一般使用了锁的代码会带来如下性能损失：

- 加锁和解锁操作，本身有一定的开销；

- 临界区的代码不能并发执行；

- 进入临界区的次数过于频繁，线程之间对临界区的争夺太过激烈，若线程竞争互斥体失败，就会陷入阻塞，让出 CPU，所以执行上下文切换的次数要远远多于不使用互斥体的版本。

替代锁的方式有很多，如无锁队列。


### 明确锁的范围
看下面这段代码：
```c++
if(hashtable.is_empty())
{
    pthread_mutex_lock(&mutex);
    htable_insert(hashtable, &elem);
    pthread_mutex_unlock(&mutex);
}
```

能看出这段代码的问题吗？虽然对 hashtable 的插入使用了锁做保护，但是判断 hash_table 是否为空也需要使用锁保护，所以正确的写法应该是：
```c++
pthread_mutex_lock(&mutex);
if(hashtable.is_empty())
{
    htable_insert(hashtable, &elem);
}
pthread_mutex_unlock(&mutex);
```

### 减少锁的粒度
所谓减小锁使用粒度指的是尽量减小锁作用的临界区代码范围，临界区的代码范围越小，多个线程排队进入临界区的时间就会越短。这就类似高速公路上堵车，如果堵车的路段越长，那么后续过来的车辆通行等待时间就会越长。

来看两个具体的例子：

**示例一**

```c++
void TaskPool::addTask(Task* task)
{
    std::lock_guard<std::mutex> guard(m_mutexList); 
    std::shared_ptr<Task> spTask;
    spTask.reset(task);            
    m_taskList.push_back(spTask);
          
    m_cv.notify_one();
}
```

上述代码中 guard 锁保护 m_taskList，仔细分析下这段代码发现，除了m_taskList其他代码其实没必要作为临界区内的代码的，所以建议挪到临界区外面去，修改如下：
```c++
std::shared_ptr<Task> spTask;
spTask.reset(task);
{
    std::lock_guard<std::mutex> guard(m_mutexList);
    m_taskList.push_back(spTask);  
}
m_cv.notify_one();
```
修改之后， guard 锁的作用范围就只有两行了，仅对 **m_taskList.push_back() ** 操作做保护，这样锁的粒度就变小了。

**示例二**
```c++
void EventLoop::doPendingFunctors()
{
    std::unique_lock<std::mutex> lock(m_mutex);
	for (size_t i = 0; i < m_pendingFunctors.size(); ++i)
	{
		m_pendingFunctors[i]();
	}
}
```

上述代码中 m_pendingFunctors 是被锁保护的对象，它的类型是 std::vector<Functor>，这样的代码效率比较低，必须等当前线程挨个处理完 m_pendingFunctors 中的元素后其他线程才能操作 m_pendingFunctors 。修改代码如下：

```c++
void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		functors.swap(m_pendingFunctors);
	}

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}	
}
```

修改之后的代码使用了一个局部变量 functors，然后把 m_pendingFunctors 中的内容倒换到 functors 中，这样就可以释放锁了，允许其他线程操作 m_pendingFunctors ，现在只要继续操作本地对象 functors 就可以了，提高了效率。

### 避免死锁的一些建议
**一个函数中，如果有一个加锁操作，那么一定要记得在函数退出时记得解锁，且每个退出路径上都不要忘记解锁路径。** 例如：

```c++
void some_func()
{
	//加锁代码
	
	if (条件1)
	{
		//其他代码
		//解锁代码
		return;
	} 
	else
	{
		//其他代码
		//解锁代码
		return;
	}
	
	
	if (条件2)
	{
		if (条件3)
		{
			//其他代码
			//解锁代码
			return;
		}
		
		if (条件4)
		{
			//其他代码
			//解锁代码
			return;
		}	
	} 
	
	while (条件5)
	{
		if (条件6)
		{
			//其他代码
			//解锁代码
			return;
		}
	}
}
```

上述函数中每个逻辑出口处都需要写上解锁代码。前面也说过，这种逻辑非常容易因为疏忽忘记在某个地方加上解锁代码而造成死锁，所以一般建议使用 RAII 技术将加锁和解锁代码封装起来，也就是说，我们可以定义一个锁对象包装类，其成员变量含有一个锁对象，在该包装类的构造函数中对锁对象进行加锁，在析构函数中对锁对象进行解锁，这样在函数出口处，锁对象包装类由于出了作用域被析构进而对其锁对象进行解锁。按照这个方法则上述代码就不需要在每个出口处加上解锁代码了：

```c++
//锁
class LockWrapper final
{
public:
	LockWrapper(SomeLock& lock) : someLock(lock)
	{
		//对someLock进行加锁
	}
	
	~LockWrapper()
	{
		//对someLock进行解锁
	}

private:
	//某个锁对象，如std::mutex、pthread_mutex_t等类型
	SomeLock&   someLock;
};

void some_func()
{
	SomeLock someLock;
	//在LockWrapper中对someLock进行加锁
	LockWrapper wrapper(someLock);
	
	if (条件1)
	{
		//其他代码
		return;
	} 
	else
	{
		//其他代码
		return;
	}
	
	
	if (条件2)
	{
		if (条件3)
		{
			//其他代码
			return;
		}
		
		if (条件4)
		{
			//其他代码
			return;
		}	
	} 
	
	while (条件5)
	{
		if (条件6)
		{
			//其他代码
			return;
		}
	}
}
```

这种利用 RAII 技术实现自动加解锁的方法，如boost::scoped_lock 和 std::lock_guard 的实现。

- **线程退出时一定要及时释放其持有的锁**

实际开发中会因一些特殊需求创建一些临时线程，这些线程执行完相应的任务后就会退出。对于这类线程，如果其持有了锁，在线程退出时，一定要记得释放其持有的锁对象。

- **多线程请求锁的方向要一致，以避免死锁**

假设现在有两个锁 A 和 B，线程 1 在请求了锁 A 之后再请求 B，线程 2 在请求了锁 B 后再请求锁 A，这种线程请求锁的方向就不一致了，线程 1 的方向是从 A 到 B，线程 2 的方向是从 B 到 A，多个线程请求锁的方向不一致容易造成死锁。所以建议的方式是 线程 1 和 线程 2 请求锁的方向保持一致，要么都从 A 到 B，要么都从 B 到 A。

- **当需要同一个线程重复请求一个锁时，搞清楚你所使用的锁的行为，是递增锁引用计数，还是会阻塞抑或是直接获得锁？**

### 避免活锁的一些建议
说了避免“死锁”，应该比较容易理解，但是这里出现的避免“活锁”。所谓活锁就是，当多个线程使用 trylock 系列的函数时，由于多个线程相互谦让，导致即使在某段时间内锁资源是可用的，也可能导致需要锁的线程拿不到锁。举个生活中的例子，马路上两个人迎面走来，两个人同时往一个方向避让，原来本意是给对方让路，结果还是发生了碰撞。

我们在实际编码时，尽量避免让过多的线程使用 trylock 请求锁，以免出现“活锁”现象，这是对资源的一种浪费。






