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

void cVsCpp() {
    cout <<  __FUNCTION__ << endl;
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
}

void cStr1() {
    cout <<  __FUNCTION__ << endl;
    //原生C字符串需要预留一个'\0'结束符
    //initializer-string for char array is too long, array size is 10 but initializer has size 11 (including the null terminating character
    //char strHelloWorld1[10] = {"helloworld"};
    char strHelloWorld2[11] = {"helloworld"};
    char strHelloWorld3[] = {"helloworld"};
}

void cStr2() {
    cout <<  __FUNCTION__ << endl;
    char c1 = 0;
    char c2 = '\0';
    char c3 = '0';

    // c1 == c2 != c3

    cout << endl;
}

void strArrayVsPtrStr() {
    cout <<  __FUNCTION__ << endl;

    char str1[] = "helloworld";
    char *str2 = "helloworld";
    str2 = str1;  //正确，可以修改
    //str1 = str2;  //错误，不可以修改 数组变量名称不允许修改

    //for (int index = 0; index < 10; index++) {
    for (int index = 0; index < strlen(str1); index++) {
        str1[index] += 1;
        cout << str1[index] << endl;
    }

    cout << "---------------" << endl;

//    for (int index = 0; index < 10; index++) {
//        str2[index] += 1;   //Exception: EXC_BAD_ACCESS (code=2, address=0x1007afd60)
//        cout << str2[index] << endl;
//    }

    // 结论：
    // string[] 时，string是不可变，string[i]是可变的
    // string* 时，string是可变的，string[i]是不可变的

    char *str3 = str1;
    for (int index = 0; index < strlen(str3); index++) {
        str3[index] += 1;  // 没问题
        cout << str3[index] << endl;
    }

    // 结论：
    // 指针指向的区域是不可变的那么就不允许修改
    // 指针指向的区域是可变的那么就允许修改

    //计算字符串长度以及内存占用
    cout << "str1: " << strlen(str1) << " bytes, " << sizeof(str1) << " bytes" << endl;
    cout << "str2: " << strlen(str2) << " bytes, " << sizeof(str2) << " bytes" << endl;
}

void strFunc() {
    cout <<  __FUNCTION__ << endl;
    char str1[] = "helloworld";
    cout << strlen(str1) << endl;
    cout << sizeof(str1) << endl;

    char str2[] = "helloworld";
    char str3[] = "hflloworld";
    char str4[] = "halloworld";
    cout << strcmp(str1, str2) << endl;
    cout << strcmp(str1, str3) << endl;
    cout << strcmp(str1, str4) << endl;
}

void strFunc1() {
    cout <<  __FUNCTION__ << endl;
    char str1[] = "hello";
    char str2[] = "world";
    char str3[MAX_LEN] = {0};
    strcpy(str3, str1); // hello
    strncpy(str3, str2, 2); // wollo
    strcat(str3, str2); // wolloworld
    unsigned int len = strlen(str3);
    //unsigned int len = sizeof(str3) / sizeof(str3[0]);
    for (int index = 0; index < len; index++) {
        cout << str3[index] << " ";
    }
    cout << endl;
}

int main() {
    cVsCpp();

    cStr1();

    cStr2();

    strArrayVsPtrStr();

    strFunc();

    strFunc1();

    return 0;
}
