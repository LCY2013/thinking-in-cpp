//
// Created by fufeng on 2024/2/7.
//
export module helloworld;

import <iostream>;
import "h1.h";
//#define H1 (1)  //h2.h 获取不到宏定义H1
import "h2.h";

export void hello() {
    std::cout << "Hello World!" << std::endl;
    std::cout << "Hello " << H1 << std::endl;
    std::cout << "Hello " << H2 << std::endl;
}
