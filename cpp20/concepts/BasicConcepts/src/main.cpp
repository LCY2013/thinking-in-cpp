//
// Created by fufeng on 2024/2/20.
//
#include <concepts>
#include <iostream>

class BaseClass {
public:
    [[nodiscard]] int getValue() const {
        return 1;
    }
};

class DerivedClass : BaseClass {
public:
    [[nodiscard]] int getValue() const {
        return 2;
    }
};

class NonDerivedClass {
public:
    [[nodiscard]] int getValue() const {
        return 3;
    }
};

//derived_from(C++20)，你可以在编译时确定一个类是否是另一个类的派生类，而不必手动检查继承层次结构。
//derived_from 是 C++20 中引入的一个概念，它是一个类型特性（type trait），用于检查一个类是否是另一个类的派生类。derived_from 提供了一种简洁的方法来检查类之间的继承关系，而不必直接访问类的继承层次结构。
template<class T>
concept DerivedFromBase = std::derived_from<T, BaseClass>;

template<DerivedFromBase T>
void getFromValue(T &t) {
    std::cout << "Get Value: " << t.getValue() << std::endl;
}

//is_base_of_v 是 C++17 中引入的一个类型特性（type trait），用于检查一个类是否是另一个类的基类。它返回一个 bool 类型的值，表示第一个类是否是第二个类的基类。
template <class T>
concept DerivedOfBaseClass = std::is_base_of_v<BaseClass, T>;  //与下面这行等价
//concept DerivedOfBaseClass = std::is_base_of<BaseClass, T>::value;

template<DerivedOfBaseClass T>
void getValue(T &t) {
    std::cout << "Get Value: " << t.getValue() << std::endl;
}

int main() {
    BaseClass base;
    getValue(base);

    DerivedClass derivedClass;
    getValue(derivedClass);

    // 解除注释会引发错误
    //NonDerivedClass nonDerivedClass;
    //getValue(nonDerivedClass);
    return 0;
}