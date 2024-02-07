//
// Created by fufeng on 2024/2/7.
//
export module helloworld;

import <iostream>;

export import :A;
import :B;

export void hello() {
    std::cout << "Hello world!" << std::endl;
    helloworldA();
    helloworldB();
}