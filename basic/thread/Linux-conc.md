# Linux线程同步对象

## Linux互斥体
Linux互斥体的用法和Windows的临界区对象用法很相似，一般也是通过限制多个线程同时执行某段代码来达到保护资源的目的。和接下来要介绍的信号量、条件变量一样，Linux互斥体都实现在 NPTL （Native POSIX Thread Library）。在NPTL中我们使用数据结构pthread_mutex_t来表示一个互斥体对象（定义于pthread.h头文件中）。互斥体对象我们可以使用两种方式来初始化：

- 使用PTHREAD_MUTEX_INITIALIZER直接给互斥体变量赋值

示例代码如下：

```c++
#include <pthread.h>
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
```

- 使用pthread_mutex_init函数初始化

如果互斥体是动态分配的或者需要给互斥体设置属性，则需要使用pthread_mutex_init函数来初始化互斥体，这个函数的签名如下：

```c++
int pthread_mutex_init(pthread_mutex_t* restrict mutex, 
					   const pthread_mutexattr_t* restrict attr);
``` 

参数mutex即我们需要初始化的 mutex 对象的指针，参数attr是需要设置的互斥体属性，通常情况下，我们使用默认属性可以将这个参数设置为NULL，后面我们会详细介绍每一种属性的用法。如果函数执行成功会返回0，如果执行失败会返回一个错误码。 pthread_mutex_init代码示例如下：

```c++
#include <pthread.h>

pthread_mutex_t mymutex;
pthread_mutex_init(&mymutex, NULL);
```

当我们不再需要一个互斥体对象时，可以使用pthread_mutex_destroy函数来销毁它， pthread_mutex_destroy函数的签名如下：

```c++
int pthread_mutex_destroy(pthread_mutex_t* mutex);
```

参数mutex即我们需要销毁的互斥体对象，如果函数执行成功会返回0，如果执行失败会返回一个错误码表明出错原因。这里我们需要注意两点：

1. 使用PTHREAD_MUTEX_INITIALIZER初始化的互斥体无须销毁；

2. 不要去销毁一个已经加锁或正在被条件变量使用的互斥体对象，当互斥体处于已加锁的状态或者正在和条件变量配合使用时，调用pthread_mutex_destroy函数会返回 EBUSY 错误。

以下代码段演示了尝试销毁一个被锁定的互斥体对象：
```c++
//destroy_locked_mutex.cpp
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

int main()
{
	pthread_mutex_t mymutex;
	pthread_mutex_init(&mymutex, NULL);
	int ret = pthread_mutex_lock(&mymutex);
	
	//尝试对被锁定的mutex对象进行销毁
	ret = pthread_mutex_destroy(&mymutex);
	if (ret != 0)
	{
		if (ret == EBUSY)
			printf("EBUSY\n");
		printf("Failed to destroy mutex.\n");
	}

	ret = pthread_mutex_unlock(&mymutex);
	//尝试对已经解锁的mutex对象进行销毁
	ret = pthread_mutex_destroy(&mymutex);
	if (ret == 0)
		printf("Succeeded to destroy mutex.\n");

	return 0;
}
```

编译上述代码并执行得到我们期望的结果：
```shell
[root@localhost codes]# g++ -g -o destroy_locked_mutex destroy_locked_mutex.cpp -lpthread
[root@localhost codes]# ./destroy_locked_mutex 
EBUSY
Failed to destroy mutex.
Succeed to destroy mutex.
```

在实际开发中，如果我们遵循正确的使用互斥体的规范，如创建互斥体对象后再对其加锁，加锁后才对其进行解锁操作，解锁后才做销毁操作，那么编码时一般不用考虑pthread_mutex_init/pthread_mutex_destroy/pthread_mutex_lock/pthread_mutex_unlock等函数的返回值。

对于互斥体的加锁和解锁操作我们一般使用以下三个函数：

```c++
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
```

参数mutex设置为我们需要加锁和解锁的互斥体对象，上述函数执行成功返回0，如果执行失败则返回一个错误码表示具体的出错原因。具体错误码，随互斥体对象的属性类型的不同而不同。

设置互斥体对象的属性需要创建一个pthread_mutexattr_t类型的对象，和互斥体对象一样，需要使用pthread_mutexattr_init函数初始化之，当不需要这个属性对象时，记得使用pthread_mutexattr_destroy去销毁它，这两个函数的签名如下：

```c++
int pthread_mutexattr_init(pthread_mutexattr_t* attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t* attr);
```

使用pthread_mutexattr_settype/pthread_mutexattr_gettype设置或获取想要的属性类型：

```c++
int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type);
int pthread_mutexattr_gettype(const pthread_mutexattr_t* restrict attr, int* restrict type);
```

属性类型一般有如下取值：

- PTHREAD_MUTEX_NORMAL（普通锁）

这是互斥体对象的默认属性（即上文中介绍的pthread_mutex_init第二个函数设置为NULL）。当一个线程对一个普通锁加锁以后，其他线程会阻塞在pthread_mutex_lock调用处， 直到对互斥体加锁的线程释放了锁，我们来用一段实例代码来验证一下：

```c++
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t mymutex;
int             resourceNo = 0;

void* worker_thread(void* param)
{
	pthread_t threadID = pthread_self();

	printf("thread start, ThreadID: %d\n", threadID);
	
	while (true)
	{
		pthread_mutex_lock(&mymutex);

		printf("Mutex lock, resourceNo: %d, ThreadID: %d\n", resourceNo, threadID);
		resourceNo++;

		printf("Mutex unlock, resourceNo: %d, ThreadID: %d\n", resourceNo, threadID);

		pthread_mutex_unlock(&mymutex);

		//休眠1秒
		sleep(1);
	}
	
	return NULL;
}

int main()
{
	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&mymutex, &mutex_attr);

	//创建5个工作线程
	pthread_t threadID[5];
	
	for (int i = 0; i < 5; ++i)
	{
		pthread_create(&threadID[i], NULL, worker_thread, NULL);
	}

	for (int i = 0; i < 5; ++i)
	{
		pthread_join(threadID[i], NULL);
	}

	pthread_mutex_destroy(&mymutex);
	pthread_mutexattr_destroy(&mutex_attr);

	return 0;
}
```














