//
// Created by fufeng on 2024/2/7.
//
module;

#define H1 (1)
#include "h2.h"

export module helloworld;

import <iostream>;

export void hello() {
    std::cout << "Hello World!" << std::endl;
    std::cout << "Hello " << H1 << std::endl;
    std::cout << "Hello " << H2 << std::endl;
}

