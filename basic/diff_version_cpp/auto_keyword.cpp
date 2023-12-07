//
// Created by fufeng on 2023/12/7.
//
#include <iostream>
#include <map>

// g++ -Wall -g auto_keyword.cpp -std=c++11
//auto关键字在C++98/03标准中与static关键字用途相反，用于修饰所有局部变量，即这个变量具有“自动”生命周期，
//但是这个规定没有任何实际的用处。因而在C++11新标准中修改了其用法，用于让编译器自己去推导一些变量的数据类型。如：
void func1() {
    int a = 1;
    auto b = a;
    auto c = false;
    std::cout << a << b << c << std::endl;
}


//这里的变量b的类型被声明为auto ，编译器会根据变量a的类型推导出变量b的类型也是int。
// 但是这样的写法在实际开发中实用性不高，所以auto一般用于让编译器自动推导一些复杂的模板数据类型，用以简化语法。如：
//[[noreturn]] void func2() {
void func2() {
    std::map<std::string, std::string> seasons;
    seasons["spring"] = "123";
    seasons["summer"] = "456";
    seasons["autumn"] = "789";
    seasons["winter"] = "101112";

    for (std::map<std::string, std::string>::iterator iter = seasons.begin(); iter != seasons.end(); ++iter) {
        std::cout << iter->first << ":" << iter->second << std::endl;
    }
}

//上面代码中迭代器变量iter其类型是std::map<std::string, std::string>::iterator，这一串的类型太长了，在C++ 11语法中我们可以使用auto关键字达到同样的效果：
void func3() {
    std::map<std::string, std::string> seasons;
    seasons["spring"] = "123";
    seasons["summer"] = "456";
    seasons["autumn"] = "789";
    seasons["winter"] = "101112";

    for (auto iter = seasons.begin(); iter != seasons.end(); ++iter) {
        std::cout << iter->first << iter->second << std::endl;
    }
}


int main() {
    func1();
    std::cout << "=============================" << std::endl;
    func2();
    std::cout << "=============================" << std::endl;
    func3();
    std::cout << "=============================" << std::endl;
}
