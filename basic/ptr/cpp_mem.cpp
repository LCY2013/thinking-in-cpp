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

    return 0;   //(text)代码区
}
