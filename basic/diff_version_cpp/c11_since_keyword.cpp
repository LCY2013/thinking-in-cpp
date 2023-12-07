//
// Created by fufeng on 2023/12/7.
//
#include <iostream>

//final/override/=default/=delete语法
//final、override、=default、=delete是C++11添加的一组非常具有标记意义的新语法

//final 关键字
//final关键字修饰一个类，这个类将不允许被继承，这在其他语言（如Java）中早就实现了。
// 在C++ 11中final关键字要写在类名的后面，其他语言是写在class关键字的前面。示例如下：

//#define __final_key__
#ifdef __final_key__
class A final {

};

class B : A {

};
#endif
// c11_since_keyword.cpp:16:11: error: base 'A' is marked 'final'
// error C3246: 'B' : cannot inherit from 'A' as it has been declared as 'final'

// override关键字
// C++语法规定，父类中加了virtual关键字的方法可以被子类重写，子类重写该方法时可以加或不加virtual关键字，例如像下面这样：
//#define __override_key__
#ifdef __override_key__

class A {
protected:
    virtual void func(int x, int y) {
    }
};

class B : public A {
protected:
    virtual void func(int x, int y) override {
    }
};

class C : public B {
protected:
    void func(int x, int y) {
    }
};

//这种宽松的规定可能会带来两个问题：
// 1. 当我们阅读代码时，无论子类重写的方法是否添加了virtual关键字，我们都没法直观地确定该方法是否是重写的父类方法；
// 2. 如果我们在子类中不小心写错了需要重写的方法的函数签名（可能是参数类型、个数或返回值类型），这个方法就会变成一个独立的方法，这可能会违背我们最初想重写父类某个方法的初衷，而编译器在编译时并不会检查到这个错误。

// 为了解决以上两个问题， C++ 11引进了override关键字，其实override关键字并不是什么新语法，在Java等其他语言中早就支持。
// 被override修饰的类方法是改写父类的同名方法，加了该关键字后在编译阶段，编译器会作相应的检查，
// 如果其父类不存在相同签名格式的类方法，编译器会给出相应的错误提示。

// 正确示例
class D : public B {
protected:
    void func(int x, int y) override {
    }
};

//错误示例一: 父类没有子类标记了override的方法
// Only virtual member functions can be marked 'override'
class E : public B {
    void func_not_exist(int x, int y) override {

    }
};

// 错误示例二：父类有子类标记了override的方法，但函数签名不一致
// hidden overloaded virtual function 'B::func' declared here: different number of parameters (2 vs 0)
class F : public B {
    void func() override {

    }
};

#endif

//=default标记
//如果一个C++类没有显式地给出构造函数、析构函数、拷贝构造函数、operator =这几类函数的实现，在需要它们时，编译器会自动生成；
// 或者，在给出这些函数的声明时，如果没有给出其实现，编译器在链接时就会报错。
// 如果使用=default标记这类函数，编译器会给出默认实现。来看一个例子：
//#define __default_key__
#ifdef __default_key__

// 编译能够通过
class A {

};

class B {
public:

    B(int i) {

    }
};

class C {
public:
    C() = default;

    C(int i) {

    }
};


int main() {
    //这样的代码是可以编译通过的，因为编译器会默认生成一个A的无参构造函数，假设我们现在给A提供一个有参数形式的构造函数：
    A a;  // 能编译通过
    //这个时候编译器就不会自动生成默认无参数的构造函数了，这段代码会编译出错，错误提示B没有合适的无参构造函数：
    //B b;  // 编译不能通过
    // 这个时候可以手动给加上无参构造函数(用C演示)，也可以使用=default语法强行让编译器自己生成
    // C() = default;
    C c;
    return 0;
}

//=default最大的作用就是，在开发中简化了那些构造函数中没有实际的初始化代码的写法，
// 尤其是声明和实现分别属于一个*.h和*.cpp文件。
// 例如，对于类AD，其头文件为c11_since_keyword.h ，其实现文件为c11_since_keyword.cpp，
// 正常情况下需要在c11_since_keyword.cpp文件中写其构造函数和析构函数的实现（可能没有实际构造和析构代码）：
#include "c11_since_keyword.h"
AD::AD() {

}

AD::~AD() {

}

//c11_since_keyword.cpp中构造函数和析构函数我们不得不写上，有了=default关键字，我们可以在c11_since_keyword.h中直接写成：
//如类ADV
//这里不用在写ADV的构造函数和析构函数的实现了

#endif

// =delete标记
//既然有强制让编译器生成构造函数、析构函数、拷贝构造函数、operator =的语法，那么也应该有禁止编译器生成这些函数的语法，没错，就是=delete。
//
//在C++98/03规范中，如果我们想让一个类不能被拷贝（即不能调用其拷贝构造函数），可以将其拷贝构造和operator =函数定义成private的：
#define __delete_key__
#ifdef __delete_key__

class A {
public:
    A() = default;

    ~A() = default;

private:
    A(const A &a) {

    }

    A &operator=(const A &a) {

    }
};

// 利用了这种方式间接实现了一个类不能被拷贝的功能，这也是继承自 boost::noncopyable 的类不能被拷贝的实现原理。
// 现在有了 =delete 语法，直接使用该语法，直接禁止编译器生成这两个函数即可：
class B {
public:
    B() = default;

    ~B() = default;

private:
    B(const B &b) = delete;

    B &operator=(const B &b) = delete;
};

int main() {
    A a1;
//    A a2(a1);  //calling a private constructor of class 'A'
    A a3;
//    a3 = a1;  //'operator=' is a private member of 'A'

    B b1;
//    B b2(b1);  //error: call to deleted constructor of 'B'
    B b3;
//    b3 = b1;  // error: overload resolution selected deleted operator '='
    return 0;
}

//一般在一些工具类中，我们不需要用到构造函数、析构函数、拷贝构造函数、operator=这四个函数，为了防止编译器自己生成，
// 同时也是为了减小生成的可执行文件的体积，建议使用 =delete 语法将这四个函数“删除”，例如：
class EncodeUtil {
public:
    static std::wstring AnsiToUnicode(const std::string &strAnsi);

    static std::string UnicodeToAnsi(const std::wstring &strUnicode);

    static std::string AnsiToUtf8(const std::string &strAnsi);

    static std::string Utf8ToAnsi(const std::string &strUtf8);

    static std::string UnicodeToUtf8(const std::wstring &strUnicode);

    static std::wstring Utf8ToUnicode(const std::string &strUtf8);

private:
    EncodeUtil() = delete;

    ~EncodeUtil() = delete;

    EncodeUtil(const EncodeUtil &rhs) = delete;

    EncodeUtil &operator=(const EncodeUtil &rhs) = delete;
};


#endif

