//
// Created by fufeng on 2024/2/20.
//
#include <concepts>
#include <iostream>

template <class T>
concept Integral = std::is_integral_v<T>;

template <class T>
concept FloatingPoint = std::is_floating_point_v<T>;

template <class T>
concept Number = Integral<T> || FloatingPoint<T>;

template <Number T>
void f13(T t) {
    std::cout << t << std::endl;
}

void c13() {
    f13(5);
    f13(20.0f);
    f13(30.0);
    //解除注释会引发错误
    //f13("aaa");
}

int main() {
    c13();

    return 0;
}
