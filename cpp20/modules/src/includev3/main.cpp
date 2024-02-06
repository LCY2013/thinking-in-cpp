//
// Created by fufeng on 2024/2/6.
//
#include <cstdint>
#include <iostream>
#include "add.h"

int32_t add(int32_t a, int32_t b) {
    return a + b;
}

// g++ -o add main.cpp add.cpp -std=c++11
int main() {
    int32_t sum = add(1,2);
    std::cout << "sum: " << sum << std::endl;
    return 0;
}
