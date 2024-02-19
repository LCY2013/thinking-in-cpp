//
// Created by fufeng on 2024/2/7.
//
export module helloworld;

export import :A;
import :B;

export void hello() {
    std::cout << "Hello World!" << std::endl;
    helloworldA();
    helloworldB();
}

