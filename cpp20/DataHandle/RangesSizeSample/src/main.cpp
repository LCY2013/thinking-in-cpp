//
// Created by fufeng on 2024/3/12.
//
#include <vector>
#include <ranges>
#include <iostream>

int main() {
    namespace ranges = std::ranges;

    std::vector<int32_t> v = {3, 1, 4, 1, 5, 9, 2, 6};
    std::cout << ranges::size(v) << std::endl;
    std::cout << ranges::ssize(v) << std::endl;
    return 0;
}
