//
// Created by fufeng on 2024/3/12.
//
#include <vector>
#include <algorithm>
#include <ranges>
#include <iostream>

int main() {
    namespace ranges = std::ranges;

    //首先，调用ranges::begin和ranges::end函数，返回一个指向容器的开始和结束的迭代器。
    //接着，通过迭代器访问数据中的元素
    std::vector<int> v = {3, 1, 2, 1, 4, 5, 9, 2, 6};
    auto start = ranges::begin(v);
    std::cout << "[0]: " << *start << std::endl;

    auto curr = start;
    curr++;
    std::cout << "[1]: " << *curr << std::endl;

    std::cout << "[4]: " << *(curr + 3) << std::endl;

    auto stop = ranges::end(v);
    std::sort(start, stop);

    //最后，调用ranges::cbegin和ranges::cend函数，返回一个指向容器的开始和结束的常量迭代器。
    for (auto it = ranges::cbegin(v); it != ranges::cend(v); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
