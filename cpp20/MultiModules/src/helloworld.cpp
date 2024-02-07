//
// Created by fufeng on 2024/2/7.
//
export module helloworld;
export import bye;

import <iostream>;

void private_hello() {
    std::cout << "Hello World!" << std::endl;
}

export void hello() {
    std::cout << "Hello World!" << std::endl;
}
