//
// Created by fufeng on 2023/12/7.
//
#include <iostream>
#include <map>
#include <string>

//Range-based循环语法介绍
//大多数语言都支持for-each语法遍历一个数组或集合中的元素，C++ 11中才支持这种语法，可谓姗姗来迟。
//在C++ 98/03规范中，对于一个数组int arr[10]，如果我们想要遍历这个数组，只能使用递增的计数去引用数组中每个元素：
void func1() {
    int arr[10] = {0};
    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << std::endl;
    }
}

//在C++ 11规范中有了for-each语法，我们可以这么写：
void func2() {
    int arr[10] = {0};
    for (int i: arr) {
        std::cout << arr[i] << std::endl;
    }
}

//对于auto关键字遍历std::map，我们也可以使用这种语法
void func3() {
    std::map<std::string, std::string> seasons;
    seasons["spring"] = "123";
    seasons["summer"] = "456";
    seasons["autumn"] = "789";
    seasons["winter"] = "101112";

    for (auto iter: seasons) {
        std::cout << iter.first << ":" << iter.second << std::endl;
    }
}

// for-each语法虽然很强大，但是有两个需要注意的地方：
// 1. for-each中的迭代器类型与数组或集合中的元素的类型完全一致，而原来使用老式语法迭代stl容器（如 std::map）时，迭代器是类型的取地址类型。
// 因此，在上面的例子中，老式语法中，iter是一个指针类型（std::pair<std::string, std::string>*），使用iter->second去引用键值；
// 而在for-each语法中，iter是数据类型（std::pair<std::string, std::string>），使用iter.second直接引用键值。
//
// 2. for-each语法中对于复杂数据类型，迭代器是原始数据的拷贝，而不是原始数据的引用。什么意思呢？我们来看一个例子：
void func4() {
    std::vector<std::string> v;
    v.push_back("zhangsan");
    v.push_back("lisi");
    v.push_back("maowu");
    v.push_back("maliu");
    for (auto iter: v) {
        iter = "hello";
    }
    // 遍历容器v，意图将v中的元素的值都修改成“hello”，但是实际执行时我们却达不到我们想要的效果。
    // 这就是上文说的for-each中的迭代器是元素的拷贝，所以这里只是将每次拷贝修改成“hello”，原始数据并不会被修改。
    std::cout << "==============val===============" << std::endl;
    for (auto iter: v) {
        std::cout << iter << std::endl;
    }
    // 可以将迭代器修改成原始数据的引用：
    for (auto &iter: v) {
        iter = "hello";
    }
    // 这样就达到修改原始数据的目的了。
    // 这是使用for-each比较容易出错的地方，对于容器中是复杂数据类型的场景，
    // 应该尽量使用这种引用原始数据的方式，以避免复杂数据类型不必要的调用拷贝构造函数的开销。
    std::cout << "============ref=================" << std::endl;
    for (auto iter: v) {
        std::cout << iter << std::endl;
    }
}

class A {
public:
    A() {

    }

    ~A() = default;

    A(const A &rhs) {

    }

public:
    int m = 0;
};

void func5() {
    A a1;
    A a2;
    std::vector<A> v;
    v.push_back(a1);
    v.push_back(a2);
    for (auto iter: v) {
        //由于iter是v中的元素的拷贝，所以每一次循环，iter都会调用A的拷贝构造函数生成一份
        //实际使用for-each循环时应该尽量使用v中元素的引用，减少不必要的拷贝函数的调用开销
        iter.m = 7;
    }
    for (auto iter: v) {
        std::cout << iter.m << std::endl;
    }
}

// 自定义对象如何支持Range-based循环
// 如何让自定义的对象支持Range-based循环语法呢？为了让一个对象支持这种语法，这个对象至少需要实现如下两个方法：
//需要返回第一个迭代子的位置
//Iterator begin();
//需要返回最后一个迭代子的下一个位置
//Iterator end();
//
//上面的Iterator是自定义数据类型的迭代子类型，这里的Iterator类型必须支持如下三种操作（原因下文会解释）：
//
//operator++ （即自增）操作，即可以自增之后返回下一个迭代子的位置；
//
//operator != （即判不等操作）操作；
//
//operator* 即解引用（dereference）操作。
//
//下面是一个自定义对象支持for-each循环的例子：
//
template<typename T, size_t N>
class CRB {
public:
    CRB() {
        for (int i = 0; i < N; i++) {
            m_elements[i] = i;
        }
    }

    ~CRB() {

    }

    T *begin() {
        return m_elements + 0;
    }

    T *end() {
        return m_elements + N;
    }

private:
    T m_elements[N];
};

void func6() {
    CRB<int, 8> crb;
    for (auto iter: crb) {
        std::cout << iter << std::endl;
    }
}

//注意：上述代码中，迭代子Iterator是 T*，这是指针类型，本身就支持operator ++和operator !=操作，
// 所以这里并没有提供这两个方法的实现。那么为什么迭代子要支持operator ++和operator !=操作呢？
// 来看一下编译器是如何实现这种for-each循环的。

//for-each循环的实现原理
//
//上述for-each循环可抽象成如下公式：
//for (for-range-declaration : for-range-initializer)
//	statement;
//
//C++14标准是这样解释上面的公式的：
//auto && __range = for-range-initializer;
//for ( auto __begin = begin-expr, __end = end-expr; __begin != __end; ++__begin )
//{
//    for-range-declaration = *__begin;
//    statement;
//}
//
//在这个循环中，begin-expr返回的迭代子__begin需要支持自增操作，
// 且每次循环时会与end-expr返回的迭代子__end做判不等比较，在循环内部，通过调用迭代子的解引用（*）操作取得实际的元素。
// 这就是上文说的迭代子对象需要支持operator++、operator !=和operator* 的原因了。
//
//但是上面的公式中，在一个逗号表达式中auto __begin = begin-expr, __end = end-expr;
// 由于只使用了一个类型符号auto导致起始迭代子__begin和结束迭代子__end是同一个类型，这样不太灵活，在某些设计中，可能希望结束迭代子是另外一种类型。
//
//因此到了C++17标准时，要求编译器解释for-each循环成如下形式：
//auto && __range = for-range-initializer;
//auto __begin = begin-expr;
//auto __end = end-expr;
//for ( ; __begin != __end; ++__begin ) {
//     for-range-declaration = *__begin;
//     statement;
//}
//
//将获取起始迭代子__begin和结束迭代子__end分开来写，这样这两个迭代子就可以是不同的类型了。
// 虽然类型可以不一样，但这两种类型之间仍然要支持operator !=操作。
// C++17就C++14的这种改变，对旧的代码不会产生任何影响，但可以让后来的开发更加灵活。

int main() {
    func1();
    std::cout << "=============================" << std::endl;
    func2();
    std::cout << "=============================" << std::endl;
    func3();
    std::cout << "=============================" << std::endl;
    func4();
    std::cout << "=============================" << std::endl;
    func5();
    std::cout << "=============================" << std::endl;
    func6();
    std::cout << "=============================" << std::endl;

    return 0;
}
