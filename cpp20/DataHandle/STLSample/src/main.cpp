//
// Created by fufeng on 2024/3/12.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

int main() {
    std::vector<int32_t> numbers{
            1, 2, 3, 4, 5
    };

    std::vector<int32_t> doubleNumbers;
    std::transform(
            numbers.begin(), numbers.end(),
            std::back_inserter(doubleNumbers),
            [](int32_t number) {
                return number * 2;
            }
    );
    std::for_each(
            doubleNumbers.begin(), doubleNumbers.end(),
            [](int32_t number) {
                std::cout << number << std::endl;
            });

    std::vector<int32_t> filteredNumbers;
    std::copy_if(
            numbers.begin(), numbers.end(),
            std::back_inserter(filteredNumbers),
            [](int32_t number) {
                return number < 5;
            });

    std::for_each(
            filteredNumbers.begin(), filteredNumbers.end(),
            [](int32_t number) {
                std::cout << number << std::endl;
            });
    return 0;
}
