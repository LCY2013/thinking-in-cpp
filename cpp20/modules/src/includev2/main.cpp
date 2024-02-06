//
// Created by fufeng on 2024/2/6.
//
#include <cstdint>
#include <iostream>
#include "add.h"

// g++ -o add main.cpp add.cpp -std=c++11
int main() {
    int32_t sum = add(1,2);
    std::cout << "sum: " << sum << std::endl;
    return 0;
}
