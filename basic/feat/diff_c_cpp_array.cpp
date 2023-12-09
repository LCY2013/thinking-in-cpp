//
// Created by fufeng on 2023/12/9.
//

#include <iostream>
#include <vector>

using namespace std;

// c语言数组作为参数时的退化问题，比如下面的int arr[10]，实则退化为int *arr，丢失了相关的长度信息
// 字符数组可以通过\0来判断是否结束，但是普通的数组没有这个信息，所以需要额外的len参数
// c语言这么设计的原因？
//  空间上进行节省，精巧的设计使语言更加精巧简单
//double average1(int arr[10]) {  //与下面的等价
double average1(int *arr) {
    double result = 0.0;
    int len = sizeof(arr) / sizeof(arr[0]);
    cout << "average1 arr length: " << len << endl;

    for (int i = 0; i < len; i++) {
        result += arr[i];
    }

    return result / len;
}

double average2(int arr[10], int len) {
    double result = 0.0;

    for (int i = 0; i < len; i++) {
        result += arr[i];
    }

    return result / len;
}

// c++如何优化c语言数组问题？ STL容器
double average3(vector<int> &arr) {
    double result = 0.0;
    int len = arr.size();
    cout << "average3 arr length: " << len << endl;

    for (int i = 0; i < len; i++) {
        result += arr[i];
    }

    return result / len;
}

double average4(vector<int> &arr) {
    double result = 0.0;
    int len = arr.size();
    cout << "average4 arr length: " << len << endl;

    //vector<int>::iterator iter = arr.begin();
    auto iter = arr.begin();

    for (; iter != arr.end(); ++iter) {
        result += *iter;
    }

    return result / len;
}

double average2DV(vector <vector<int>> &vv) {
    double result = 0.0;
    unsigned int size = 0;

    //vector<vector<int>> vvIter = vv.bengin;
    auto vvIter = vv.begin();

    for (; vvIter != vv.end(); ++vvIter) {
        //vector<int>::iterator vIter = vvIter->begin();
        auto vIter = vvIter->begin();
        for (; vIter != vvIter->end(); ++vIter) {
            result += *vIter;
            size++;
            cout << *vIter << " ";
        }
        cout << endl;
    }

    return result / size;
}

//g++ -Wall -g diff_c_cpp_array.cpp -std=c++20
int main() {
    int array1[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
    int len = sizeof(array1) / sizeof(array1[0]);
    cout << "array1 length: " << len << endl;
    cout << average1(array1) << endl;
    cout << average2(array1, len) << endl;
    vector<int> vt{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110};
    cout << average3(vt) << endl;
    cout << average4(vt) << endl;

    cout << "-----------------------------" << endl;
    vector <vector<int>> vv{{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110},
                            {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110}};
    cout << average2DV(vv) << endl;
    cout << "-----------------------------" << endl;
    vector <vector<int>> vv2{8, vector < int > {1, 2, 3, 4, 5}};
    cout << average2DV(vv2) << endl;
    cout << "-----------------------------" << endl;
    vector <vector<int>> vv3{8, vector<int>(8, 7)};
    cout << average2DV(vv3) << endl;
    return 0;
}
