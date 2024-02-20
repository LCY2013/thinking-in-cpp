//
// Created by fufeng on 2024/2/20.
//
#include <concepts>
#include <iostream>
#include <vector>
#include <type_traits>

using std::same_as;
using std::convertible_to;

template <typename T>
concept Histogram = requires(T t1, T t2) {
    t1.getMoments();         // 要求有getMoments接口
    T::count;                // 要求有静态变量count
    t1.moments;              // 要求有成员变量moments
    t1 + t2;                 // 要求对象能够进行+操作

    typename T::type;        // 要求存在类型成员type
    typename std::vector<T>; // 要求能够模板实例化并与std::vector组合使用

    { t1.getSubHistogram() } -> same_as<T>;    // 要求接口返回类型与T一致
    { t1.getUnit() } -> convertible_to<float>; // 要求接口返回类型能转换成float，本质上接口返回类型可能是double
    { t1 = std::move(t2) } noexcept;          // 要求表达式不能抛出异常

    requires sizeof(T) > 4;
};

int main() {
    return 0;
}
