//
// Created by fufeng on 2024/2/19.
//
#pragma once

#include <vector>
#include <iostream>

template <size_t Size, class T, typename U>
void fillContainer(T& collection, U value) {
    std::cout << "Universal" << std::endl;

    for (size_t i = 0; i != Size; ++i) {
        collection.push_back(value);
    }
}

template <>
void fillContainer<10, std::vector<double>, double>(
        std::vector<double>& collection, double value
) {
    std::cout << "Explicit (full) template specialization" << std::endl;

    for (size_t i = 0; i != 10; ++i) {
        collection.push_back(value + 2.0);
    }
}

void c13() {
    std::vector<int> vec;
    fillContainer<5>(vec, 10);

    std::vector<double> doubleVec;
    fillContainer<10>(doubleVec, 10);
    fillContainer<10, std::vector<double>, double>(doubleVec, 10);
}

