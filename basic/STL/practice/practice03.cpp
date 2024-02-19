//
// Created by fufeng on 2024/2/18.
//
#include <algorithm>
#include <iostream>

using namespace std;

bool MySort(int a, int b) {
    return a < b;
}

void Display(int a) {
    cout << a << " ";
}

template<class T>
inline bool MySortT(T const& a, T const& b) {
    return a < b;
}

template<class T>
inline void DisplayT(T const& a) {
    cout << a << " ";
}

struct SortF {
    bool operator()(int a, int b) {
        return a < b;
    }
};

struct DisplayF {
    void operator()(int a) {
        cout << a << " ";
    }
};

template<class T>
struct SortFT {
    inline bool operator() (const T &a, const T &b) const {
        return a < b;
    }
};

template<class T>
struct DisplayFT {
    void operator()(const T &a) const{
        cout << a << " ";
    }
};

int main() {
    // C++函数
    int arr[] = { 4, 3, 2, 1, 7 };
    sort(arr, arr + 5, MySort);
    for_each(arr, arr + 5, Display);
    cout << endl;

    // C++泛型函数
    int arr2[] = { 4, 3, 2, 1, 7 };
    sort(arr2, arr2 + 5, MySortT<int>);
    for_each(arr2, arr2 + 5, DisplayT<int>);
    cout << endl;

    // C++泛型结构体
    int arr3[] = { 4, 3, 2, 1, 7 };
    sort(arr3, arr3 + 5, SortFT<int>() );
    for_each(arr3, arr3 + 5, DisplayFT<int>());
    cout << endl;

    // C++结构体
    int arr4[] = { 4, 3, 2, 1, 7 };
    sort(arr4, arr4 + 5, SortF());
    for_each(arr4, arr4 + 5, DisplayF());
    cout << endl;
    return 0;
}
