//
// Created by fufeng on 2023/12/6.
//

#include <iostream>

//假设类A有一个成员变量是一个int数组，在C++98/03标准中，如果要在构造函数中对其进行初始化，需要这样写：
//C++98/03类成员变量是数组时的初始化语法
class A {
public:
    A() {
        arr[0] = 2;
        arr[1] = 0;
        arr[2] = 1;
        arr[3] = 9;
    }

public:
    int arr[4];
};

// 对于字符数组，可能就要在构造函数中使用strcpy、memcpy这一类函数了；
// 再者如果数组元素足够多，初始值又没什么规律，这种赋值代码会有很多行。
// 但是，如果arr是一个局部变量，在定义arr时其实是可以使用如下的语法初始化的：
// int arr[4] = {2, 0, 1, 9};
//
// 既然C++98/03标准中，局部变量数组支持这种语法，为什么在类成员变量语法中就不支持呢？
// 这是旧语法不合理的一个地方，因此在C++11语法中类成员变量也可以使用这种语法进行初始化了：
//C++11类成员变量是数组时的初始化语法
class A11 {
public:
    A11() : arr{2, 0, 1, 9} {
    }

public:
    int arr[4];
};

// 但在C++89/03标准中要使用这种语法，必须是针对类的static const成员，且必须是整型（包括bool、char、int、long等）。
//C++89/03在类定义处初始化成员变量
class A_const {
public:
    //T的类型必须整型，且必须是static const成员
    //static const T t = 某个整型值;
    static const int t = 0;
};

//在C++11标准中，就没有这种限制了，你可以使用花括号（即{}）对任意类型的变量进行初始化，且不用是static类型。
//C++11在类定义处初始化成员变量
class A11_const {
public:
    bool ma{true};
    int mb{2019};
    std::string mc{"helloworld"};
};


int main(int argc, char *argv[]) {
    return 0;
}
