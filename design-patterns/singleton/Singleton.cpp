//
// Created by fufeng on 2024/1/29.
//

#include "Singleton.h"

// nullptr c++定义的空
Singleton* Singleton::This = nullptr;

const Singleton* Singleton::getInstance() {
    if (!This) {
        This = new Singleton();
    }
    return This;
}

Singleton::Singleton() {

}

Singleton::~Singleton() {

}
