//
// Created by fufeng on 2024/1/29.
//

#ifndef THINKING_IN_CPP_SINGLETON_H
#define THINKING_IN_CPP_SINGLETON_H

#include <iostream>
using namespace std;

class Singleton {
public:
    static const Singleton* getInstance();
    static void doSomething(){
        cout << "Do something" << endl;
    }
private:
    Singleton();
    ~Singleton() ;
    static Singleton* This;  // 单例
};


#endif //THINKING_IN_CPP_SINGLETON_H
