//
// Created by fufeng on 2024/1/29.
//
#include "Singleton.h"

int main() {
    Singleton::getInstance()->doSomething();
    Singleton::getInstance()->doSomething();
    return 0;
}
