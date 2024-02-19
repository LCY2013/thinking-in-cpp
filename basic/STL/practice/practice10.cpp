//
// Created by fufeng on 2024/2/19.
//
#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

mutex g_mutex;
void T1()
{
    g_mutex.lock();
    cout << "T1 Hello" << endl;
    g_mutex.unlock();
}
void T2(const char* str)
{
    g_mutex.lock();
    cout << "T2 " << str << endl;
    g_mutex.unlock();
}
int main()
{
    thread t1(T1);
    thread t2(T2, "Hello World");
    t1.join();
    //t2.join();
    t2.detach();
    cout << "Main Hi" << endl;


    return 0;
}

