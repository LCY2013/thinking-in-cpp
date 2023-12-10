//
// Created by fufeng on 2023/12/10.
//
#include <iostream>

using namespace std;

// c语言强制类型转换问题
// 问题表现：
// 1、隐藏的bug和陷阱
// 2、滥用类型转换可能导致灾难性后果，且很难排查
// 问题原因：
// 类型转换在底层语言中运用很广泛，灵活方便
// 问题解决：
// c++语言中，类型转换的使用要遵循一定的规则，避免类型转换的滥用，分类便于排查隐藏bug，复杂性鼓励减少使用
// static_cast、dynamic_cast、const_cast、reinterpret_cast

// 1 + 1/2 + 1/3 + 1/4 +... + 1/n
double sum_of_reciprocals_error(int n) {
    double sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += 1 / i;
    }
    return sum;
}

// 1 + 1/2 + 1/3 + 1/4 +... + 1/n
double sum_of_reciprocals_current(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / i;
    }
    return sum;
}

// 1 + 1/2 + 1/3 + 1/4 +... + 1/n
double sum_of_reciprocals_current_cpp(int n) {
    double sum = 0.0;
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / static_cast<double>(i);
    }
    return sum;
}

int main() {
    int array[] = {1, 2, 3};
    cout << sizeof(array) / sizeof(array[0]) << endl;

    int threshold = -1;

    // sizeof 返回的是一个无符号数，这里两个数都会变成无符号数(隐式类型转换), 所以会有问题
    //if (sizeof(array) / sizeof(array[0]) > threshold) {
    // 解决办法就是如下
    int len = static_cast<int>(sizeof(array) / sizeof(array[0]));
    //if (sizeof(array) / sizeof(array[0]) > static_cast<unsigned int>(threshold)) {
    if (len > threshold) {
        cout << "positive number array" << endl;
    } else {
        cout << "negative number array" << endl;
    }

    cout << unsigned(threshold) << endl;
    cout << static_cast<unsigned int>(threshold) << endl;

    // 1 + 1/2 + 1/3 + 1/4 +... + 1/n
    int n = 0;
    cin >> n;
    cout << sum_of_reciprocals_error(n) << endl;

    cin >> n;
    cout << sum_of_reciprocals_current(n) << endl;

    cin >> n;
    cout << sum_of_reciprocals_current_cpp(n) << endl;

    return 0;
}

