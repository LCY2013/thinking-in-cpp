//
// Created by fufeng on 2023/12/21.
//

#include <stdio.h>
#include <thread>

void threadproc()
{
    while (true)
    {
        printf("I am New Thread!\n");
    }
}

// err
void func()
{
    std::thread t(threadproc);
}

// 正确
void func_current()
{
    std::thread t(threadproc);
    t.detach();
}

int main()
{
    func_current();

    //权宜之计，让主线程不要提前退出
    while (true)
    {
    }

    return 0;
}
