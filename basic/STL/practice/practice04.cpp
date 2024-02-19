//
// Created by fufeng on 2024/2/18.
//
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include<iostream>
using namespace std;

int main() {
    // transform lambda
    int ones[] = {1, 2, 3, 4, 5};
    int twos[] = {10, 20, 30, 40, 50};
    int results[5];
    transform(ones, ones + 5, twos, results, std::plus<int>());
    for_each(results, results + 5, [](int i) { cout << i << endl; } /*lambda*/ );
    cout << endl;

    //find
    int arr[] = {0, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 7, 7, 8 };
    int len = sizeof(arr) / sizeof(arr[0]);
    vector<int> iA(arr + 2, arr + 6); // {2,3,3,4}
    //vector<int> iA;
    //iA.push_back(1);
    //iA.push_back(9); // {1, 9}
    cout << count(arr, arr+len, 6) << endl;
    cout << count_if(arr, arr + len, [](int i) { return i > 5; }) << endl;
    //mac os标准库中可能没有实现bind2nd，在 C++11 标准中，std::bind2nd 已经被弃用，并且在 C++17 标准中被移除了。
    //cout << count_if(arr, arr + len, bind2nd(less<int>(),  7) ) << endl;
    //使用bind来代替bind2nd
    cout << count_if(arr, arr + len, bind(less<int>(), std::placeholders::_1,  7) ) << endl;
    cout << binary_search(arr, arr + len, 9) << endl;
    cout << *search(arr, arr + len, iA.begin(), iA.end()) << endl;

    cout << endl;

    return 0;
}
