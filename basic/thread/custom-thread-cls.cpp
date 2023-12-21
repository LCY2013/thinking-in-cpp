//
// Created by fufeng on 2023/12/21.
//
#include <thread>
#include <memory>
#include <stdio.h>

class Thread
{
public:
    Thread()
    {
    }

    ~Thread()
    {
    }

    void Start()
    {
        m_stopped = false;
        //threadFunc是类的非静态方法，所以作为线程函数，第一个参数必须传递类实例地址，即this指针
        m_spThread.reset(new std::thread(&Thread::threadFunc, this, 8888, 9999));
    }

    void Stop()
    {
        m_stopped = true;
        if (m_spThread)
        {
            if (m_spThread->joinable())
                m_spThread->join();
        }
    }

private:
    void threadFunc(int arg1, int arg2)
    {
        while (!m_stopped)
        {
            printf("Thread function use instance method.\n");
        }
    }

private:
    std::shared_ptr<std::thread>  m_spThread;
    bool                          m_stopped;
};

int main()
{
    Thread mythread;
    mythread.Start();

    //权宜之计，让主线程不要提前退出
    while (true)
    {
    }

    return 0;
}

