//
// Created by fufeng on 2024/2/19.
//
#include <iostream>
#include <vector>
#include <list>
#include <cstdint>
#include <type_traits>
#include <algorithm>

template<class T>
struct Display {
    void operator() (const T &a) {
        std::cout << a << " ";
    }
};

template<typename T,
        std::enable_if_t<
                std::is_same<
                        std::list<typename T::value_type>,
                        T
                >::value,
                bool
        > = true>
void printCollection(const T &a) {
    std::cout << "List version" << std::endl;

    /*for (auto element : a) {
        std::cout << element << " ";
    }*/
    for_each(a.begin(), a.end(), Display<typename T::value_type>());

    std::cout << std::endl;
}

template<typename T,
        std::enable_if_t<
                std::is_same<
                        std::vector<typename T::value_type>,
                        T
                >::value,
                bool
        > = true>
void printCollection(const T &a) {
    std::cout << "Vector version" << std::endl;

    /*for (auto element : a) {
        std::cout << element << " ";
    }*/
    for_each(a.begin(), a.end(), Display<typename T::value_type>());

    std::cout << std::endl;
}

void c14() {
    std::vector<int32_t> arr1{
            1, 2, 3, 4, 5
    };
    printCollection(arr1);

    std::list<int32_t> arr2{
            1, 2, 3, 4, 5
    };
    printCollection(arr2);
}

int main() {
    c14();
    return 0;
}