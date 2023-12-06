//
// Created by fufeng on 2023/12/6.
//
#include <iostream>
#include <initializer_list>
#include <vector>

// 在C++11标准中，无论是局部变量还是类变量，使用花括号（{}）初始化的语法被统一起来，写法也变得简洁起来。
//
// 那么这种语法是如何实现的呢？如何在自定义类中也支持这种花括号呢？
// 这就需要用到C++11引入的新对象std::initializer_list<T>，这是一个模板对象，
// 接收一个自定义参数类型T，T既可以是基础数据类型（如编译器内置的bool、char、int 等）也可以是自定义复杂数据类型。
// 为了使用std::initializer_list<T>，需要包含**<initializer_list>**头文件
class A {
public:
    A(std::initializer_list<int> integers) {
        m_vecIntegers.insert(m_vecIntegers.end(), integers.begin(), integers.end());
    }

    ~A() {

    }

    void append(std::initializer_list<int> integers) {
        m_vecIntegers.insert(m_vecIntegers.end(), integers.begin(), integers.end());
    }

    void print() {
        size_t size = m_vecIntegers.size();
        for (size_t i = 0; i < size; i++) {
            std::cout << m_vecIntegers[i] << std::endl;
        }
    }

private:
    std::vector<int> m_vecIntegers;
};

int main(int argc, char *argv[]) {
    A a{6, 7, 8};
    a.print();
    std::cout << "After appending...." << std::endl;

    a.append({8, 9, 0});
    a.print();

    return 0;
}

//上述代码，自定义了一个类A，为了让A的构造函数和append方法同时支持花括号语法，给这两个方法同时设置了一个参数integers，参数类型均为std::initializer_list<int>，程序执行结果如下：
//6
//7
//8
//After appending....
//6
//7
//8
//8
//9
//0
