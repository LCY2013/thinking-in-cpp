//
// Created by fufeng on 2024/1/29.
//

// C++中 nullptr 与 NULL区别？
//
//在C++中，nullptr 和 NULL 都用于表示空指针，但它们有一些关键的区别。在C++11及更高版本中，推荐使用 nullptr 而不是 NULL。
//
//1. 类型安全性：
//
//nullptr 是一个空指针常量，它有自己的类型std::nullptr_t。这使得在类型检查中更加安全，因为它不能隐式地转换为其他指针类型。
//NULL 是一个宏，通常被定义为整数零，它可以被隐式转换为任何指针类型，这可能导致一些不安全的行为。
//
//2. 上下文适用性：
//
//nullptr 通常更适用于现代C++，尤其是在使用新的标准库和功能时。
//NULL 是C++早期的传统做法，但在C++11引入了 nullptr 后，建议在新代码中使用 nullptr。

#include <iostream>
using namespace std;

void func(void* i) {
    cout << "func(void* i)" << endl;
}

void func(int i) {
    cout << "func(int i)" << endl;
}

int main() {
    int* pi = NULL;
    int* pi2 = nullptr;
    char* pc = NULL;
    char* pc2 = nullptr;
    func(NULL);                   // func(int i)
    func(nullptr);                 // func(void* i)
    func(pi);                         // func(void* i)
    func(pi2);                       // func(void* i)
    func(pc);                        // func(void* i)
    func(pc2);                      // func(void* i)
    return 0;
}
