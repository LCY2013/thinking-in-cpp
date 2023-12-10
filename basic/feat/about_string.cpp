//
// Created by fufeng on 2023/12/10.
//
#include <iostream>
#include <string>

using namespace std;

// C语言字符串问题
// 问题表现
// ‘\0’ 结束符，表达能力的天生缺陷，运行效率低
// 问题原因
// 特殊场景的，有针对性的设计问题
// C++中的解决方案
// C++的string类以及一些开源库解决方案如redis库的实现
// https://redis.com https://github.com/redis sds.h sds.c

#define MAX_LEN 100

int main() {
    char str1[] = "string";
    cout << strlen(str1) << endl;  // 6
    cout << sizeof(str1) / sizeof(str1[0]) << endl;  // 7

    char str2[] = "str\0ing";
    cout << strlen(str2) << endl; // 3
    cout << sizeof(str2) / sizeof(str2[0]) << endl; // 8

    char str1A[MAX_LEN] = "stringA";
    strcat(str1A, str2);
    cout << str1A << endl; // stringAstr
    cout << strlen(str1A) << endl; // 10
    cout << sizeof(str1A) / sizeof(str1A[0]) << endl; // 100

    cout << endl;
    cout << "Testing C++ string: " << endl;
    string sstr1 = "string";
    cout << sstr1.length() << endl;  // 6
    cout << sstr1.capacity() << endl; // 22
    cout << sstr1.size() << endl;  // 6
    cout << sizeof(sstr1) << endl; // 24

    string sstr2 = "str\0ing";
    cout << sstr2.length() << endl;  // 3
    cout << sstr2.capacity() << endl; // 22
    cout << sstr2.size() << endl;  // 3
    cout << sizeof(sstr2) << endl; // 24

    sstr1 += sstr2;
    cout << sstr1.length() << endl;  // 9
    cout << sstr1.capacity() << endl; // 22
    cout << sstr1.size() << endl;  // 9
    cout << sizeof(sstr1) << endl; // 24

    string s1 = "hello";
    string s2 = "world";
    string s3 = s1 + s2;
    cout << s3 << endl;

    return 0;
}
