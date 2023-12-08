//
// Created by fufeng on 2023/12/7.
//
#include <iostream>
#include <string>
#include <map>
#include <tuple>

//C++17结构化绑定
//std::map容器很多读者应该都很熟悉，map容器提供了一个insert方法，用该方法向map中插入元素，
// 但是应该很少有人记得insert方法的返回值是什么类型，来看一下C++98/03提供的insert方法的签名：
//std::pair<iterator,bool> insert( const value_type& value );
//
//这里仅关心其返回值，这个返回值是一个std::pair<T1, T2>类型，由于map中元素的key不允许重复，
// 所以如果insert方法调用成功，T1是被成功插入到map中的元素的迭代器，T2的类型为bool，此时其值为true（表示插入成功）；
// 如果insert由于key重复，T1是造成insert插入失败、已经存在于map中的元素的迭代器，此时T2的值为false（表示插入失败）。
//
//在C++98/03标准中可以使用std::pair<T1, T2>的first和second属性来分别引用T1和T2的值。如下面熟悉的代码所示：

void func1() {
    std::map<std::string, int> cities;
    cities["beijing"] = 0;
    cities["shanghai"] = 1;
    cities["shenzhen"] = 2;
    cities["guangzhou"] = 3;

    //for (const auto& [key, value] : m)
    //{
    //    std::cout << key << ": " << value << std::endl;
    //}

    //这一行在C++11之前的写法实在太麻烦了！
    //std::pair<std::map<std::string, int>::iterator, int> insertResult = cities.insert(std::pair<std::string, int>("shanghai", 2));
    //C++11中我们写成：
    auto insertResult = cities.insert(std::pair<std::string, int>("shanghai", 2));

    std::cout << "Is insertion successful ? " << (insertResult.second ? "true" : "false")
              << ", element key: " << insertResult.first->first << ", value: " << insertResult.first->second
              << std::endl;

}

//std::pair一般只能表示两个元素，C++11标准中引入了std::tuple类型，有了这个类型，就可以放任意个元素了，
// 原来需要定义成结构体的POD对象可以直接使用std::tuple表示，例如下面表示用户信息的结构体：
struct UserInfo {
    std::string username;
    std::string password;
    int gender;
    int age;
    std::string address;
};

void func2() {
    UserInfo userInfo = {"jerry", "123456", 1, 18, "cd"};
    std::string username = userInfo.username;
    std::string password = userInfo.password;
    int gender = userInfo.gender;
    int age = userInfo.age;
    std::string address = userInfo.address;

    // g++ -g -Wall c17_structured_binding.cpp -std=c++11
    // 不再需要定义struct UserInfo这样的对象，可以直接使用std::tuple表示：
    std::tuple<std::string, std::string, int, int, std::string> ui("Tom", "123456", 0, 25, "Pudong Street");

    std::string username1 = std::get < 0 > (ui);
    std::string password1 = std::get < 1 > (ui);
    int gender1 = std::get < 2 > (ui);
    int age1 = std::get < 3 > (ui);
    std::string address1 = std::get < 4 > (ui);
}

//从std::tuple中获取对应位置的元素，我们使用std::get<N> ，其中N是元素的序号（从0开始）。
//
//与定义结构体相比，无论是通过std::pair的first和second还是std::tuple的std::get<N>方法来获取元素子属性，
// 这些代码都是非常难以维护的，其根本原因是first和second这样的命名不能做到见名知意。
//
//C++17引入的结构化绑定（Structured Binding ）将我们从这类代码中解放出来。结构化绑定使用语法如下：
//auto [a, b, c, ...] = expression;
//auto [a, b, c, ...] { expression };
//auto [a, b, c, ...] ( expression );
//右边的expression可以是一个函数调用、花括号表达式或者支持结构化绑定的某个类型的变量。例如：
void func3() {
    //形式1
    std::map<std::string, int> someMap;
    auto [iterator, inserted] = someMap.insert(std::pair<std::string, int>("shanghai", 2));
    //形式2
    double myArray[3] = {1.0, 2.0, 3.0};
    auto [a, b, c] = myArray;
    //形式3
    struct Point {
        Point(double x, double y) : x(x), y(y) {}

        double x;
        double y;
    };
    Point myPoint(10.0, 20.0);
    auto [myX, myY] = myPoint;
}

//这样，可以给用于绑定到目标的变量名（语法中的a、b、c）起一个有意义的名字。
//
//需要注意的是，绑定名称a、b、c是绑定目标的一份拷贝，当绑定类型不是基础数据类型时，如果你的本意不是想要得到绑定目标的副本，
// 为了避免拷贝带来的不必要开销，建议使用引用，如果不需要修改绑定目标建议使用const引用。示例如下：
void func4() {
    double myArray[3] = {1.0, 2.0, 3.0};
    auto &[a, b, c] = myArray;
    //形式3
    struct Point {
        Point(double x, double y) : x(x), y(y) {}

        double x;
        double y;
    };
    Point myPoint(10.0, 20.0);
    const auto &[myX, myY] = myPoint;
}

//结构化绑定（Structured Binding ）是C++17引入的一个非常好用的语法特性。
// 有了这种语法，在遍历像map这样的容器时，可以使用更简洁和清晰的代码去遍历这些容器了：
void func5() {
    std::map<std::string, int> cities;
    cities["beijing"] = 0;
    cities["shanghai"] = 1;
    cities["shenzhen"] = 2;
    cities["guangzhou"] = 3;
    for (const auto &[cityName, cityNumber]: cities) {
        std::cout << cityName << ": " << cityNumber << std::endl;
    }
}

// 上述代码中cityName和cityNumber可以更好地反映出这个map容器的元素内容。
//
//再来看一个例子，某 WebSocket 网络库中有如下代码：
//std::pair<int, bool> uncork(const char *src = nullptr, int length = 0, bool optionally = false) {
//        LoopData *loopData = getLoopData();
//
//        if (loopData->corkedSocket == this) {
//            loopData->corkedSocket = nullptr;
//
//            if (loopData->corkOffset) {
//                /* Corked data is already accounted for via its write call */
//                auto [written, failed] = write(loopData->corkBuffer, loopData->corkOffset, false, length);
//                loopData->corkOffset = 0;
//
//                if (failed) {
//                    /* We do not need to care for buffering here, write does that */
//                    return {0, true};
//                }
//            }
//
//            /* We should only return with new writes, not things written to cork already */
//            return write(src, length, optionally, 0);
//        } else {
//            /* We are not even corked! */
//            return {0, false};
//        }
//    }

//代码的write函数返回类型是std::pair<int, bool>，被绑定到[written, failed]这两个变量中去。
// 前者在写入成功的情况下表示实际写入的字节数，后者表示是否写入成功。

//std::pair<int, bool> write(const char *src, int length, bool optionally = false, int nextLength = 0) {
//	//具体实现省略...
//}

//结构化绑定的限制
//
//结构化绑定不能使用constexpr修饰或被声明为static，例如：
void func6() {
    //正常编译
    auto [first, second] = std::pair<int, int>(1, 2);
    //无法编译通过
    //constexpr auto [first, second] = std::pair<int, int>(1, 2);
    //无法编译通过
    //static auto [first, second] = std::pair<int, int>(1, 2);
}
//有些编译器也不支持在lamda表达式捕获列表中使用结构化绑定语法。

// g++ -Wall -g c17_structured_binding.cpp -std=c++11
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

