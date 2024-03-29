//
// Created by fufeng on 2024/3/12.
//
#include <vector>
#include <ranges>
#include <iostream>

int main() {
    namespace ranges = std::ranges;

    std::vector<int32_t> v = {3, 1, 4, 1, 5, 9, 2, 6};

    auto data = ranges::data(v);
    std::cout << "[1]" << data[1] << std::endl;
    data[2] = 10;

    auto cdata = ranges::cdata(v);
    std::cout << "[2]" << cdata[2] << std::endl;
    std::cout << "[2]" << v[2] << std::endl;

    return 0;
}
