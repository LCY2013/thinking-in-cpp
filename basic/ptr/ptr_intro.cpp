//
// Created by fufeng on 2023/12/17.
//

#include <iostream>

using namespace std;

void demo01() {
    cout << __FUNCTION__ << endl;
    int a = 7, b = -1;
    float c = 3.14f;
    int *d = &a;
    float *e = &c;
    cout << d << endl;
    cout << e << endl;
    cout << *d << endl;
    cout << *e << endl;
}

void demo02() {
    cout << __FUNCTION__ << endl;
    //T*: 注意*在定义和间接访问上的作用
    int i = 3;
    int *ip = &i;
    cout << *ip << endl;
    double d = 3.14;
    double *dp = &d;
    cout << *dp << endl;
    char c = 'a';
    char *cp = &c;
    cout << *cp << endl;
}

void demo03() {
    cout << __FUNCTION__ << endl;
    // array of pointers 和 a pointer to an array
    int c[4] = {0x8000000, 0xFFFFFFF, 0x000000000, 0x7FFFFFFF};
    int *a[4]; // array of pointers 指针的数组
    int(*b)[4]; // pointer to an array 指向数组的指针
    b = &c; // 注意这里数组的个数得要匹配
    // 将数组中元素赋给数组a
    for (int i = 0; i < 4; ++i) {
        a[i] = &c[i];
    }
    // 输出结果
    cout << *(a[0]) << endl;
    cout << (*b)[3] << endl;
}

void demo04() {
    cout << __FUNCTION__ << endl;
    // 指针的运算
    int a = 7, b = -1;
    int *p = &a;
    cout << p << endl;
    cout << *p << endl;
    cout << p + 1 << endl;
    cout << p - 1 << endl;
    cout << p + 2 << endl;
    cout << p - 2 << endl;
    cout << p + 3 << endl;
    *p++ = 1;
    *++p = 2;
    cout << a++ +b << endl;
    int * pa = &a;
    int * pb = &b;
    cout << *pa++ + *pb << endl;
}

int main() {
    demo01();

    demo02();

    demo03();

    demo04();
    return 0;
}
