//
// Created by fufeng on 2024/2/20.
//
#include <concepts>
#include <iostream>

template<class T>
concept Integral = std::is_integral_v<T>;

template<class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

// 逻辑与&&
template<class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;


class Class {
public:
    static int const value = 1;
    [[nodiscard]] static int getValue() {
        return 1;
    }
private:
    int v;
};

//constexpr 是 C++11 中引入的关键字，用于指示编译器在编译时计算表达式的值或者执行函数，以及将值保存为编译时常量。
template <typename T>
constexpr bool getValue() {
    return T::value;
    //下面会提示编译错误，常量表达式才能用在constexpr函数中
    //return T::getValue();
}

template <typename T> requires(sizeof(T) > 1 && getValue<T>())
void f(T) {
    std::cout << "template version" << std::endl;
}

void f(int32_t) {
    std::cout << "int version" << std::endl;
}

void c15() {
    f('A');
    Class c{};
    f(c);
}

int main() {
    c15();

    return 0;
}