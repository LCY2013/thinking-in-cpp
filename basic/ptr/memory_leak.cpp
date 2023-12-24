//
// Created by fufeng on 2023/12/24.
//
#include <stdio.h>
#include <string>
using namespace std;

int a = 0; // (GVAR)全局初始化区
int *p1; // (bss)全局未初始化区

int main() {
    int b = 1; //(stack) 栈区变量
    char s[] = "abc"; //(stack) 栈区变量
    int *p2 = NULL; //(stack) 栈区变量
    char *p3 = "1234567"; //1234567\0在常量区，p3在栈区
    static int c = 0; // (GVAR)全局(静态)初始化区
    p1 = new int(10); //(heap)堆区变量
    p2 = new int(20); //(heap)堆区变量
    char *p4 = new char[7]; //(heap)堆区变量
    strcpy(p4, "1234567"); //(text)代码区

    //(text)代码区
    if (p1 != NULL) {
        delete p1;
        p1 = NULL;
    }
    if (p2!= NULL) {
        delete p2;
        p2 = NULL;
    }
    if (p4!= NULL) {
        delete[] p4;
        p4 = NULL;
    }
    //(text)代码区

    //内存泄漏模拟
    while (true) {
        int *wp = new int(10);
    }
    return 0;   //(text)代码区
}

