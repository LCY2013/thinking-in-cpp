//
// Created by fufeng on 2024/1/29.
//
#include <iostream>

using namespace std;

int Test() {
    return 0;
}

class Base {
public:
    Base() : _i(0) { ; };

    virtual void T() { cout << "Base::T" << _i << endl; }

private:
    int _i;
};

class Derived : public Base {
public:
    Derived() : _j(1) { ; }

    virtual void T() { cout << "Derived::T" << _j << endl; }

private:
    int _j;
};

int main() {
    // C++ const_cast
    const int a = 10;
    //int* pA = &a;
    int *pA = const_cast<int *>(&a);
    *pA = 100;

    //C++ reinterpret_cast
    typedef void(*FunPtr)();
    FunPtr funPtr;
    //funPtr = &Test;
    funPtr = reinterpret_cast<FunPtr>(&Test);

    //C++ static_cast
    int i = 7;
    double dd = static_cast<double>(i);
    double dd2 = 7.7;
    int i2 = static_cast<int>(dd2);

    Base cb;
    Derived cd;
    Base* pcb;
    Derived* pcd;

    // 子类 -> 父类
    pcb = static_cast<Base*>(&cd);
    if (pcb == NULL) {
        cout << "unsafe static_cast from Derived to Base" << endl;
    }

    pcb = dynamic_cast<Base*>(&cd);
    if (pcb == NULL) {
        cout << "unsafe dynamic_cast from Derived to Base" << endl;
    }

    // 父类 -> 子类
    pcd = static_cast<Derived*>(&cb);
    if (pcd == NULL) {
        cout << "unsafe static_cast from Base to Derived" << endl;
    }

    pcd = dynamic_cast<Derived*>(&cb);
    if (pcd == NULL) {
        cout << "unsafe dynamic_cast from Base to Derived" << endl;
    }

    return 0;
}
