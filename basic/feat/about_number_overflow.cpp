//
// Created by fufeng on 2023/12/10.
//
#include <iostream>
using namespace std;

//https://www.boost.org/doc/libs/1_83_0/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

//c++ -Wall -g -std=c++11 -I/usr/local/include/boost/ -I/usr/local/include/boost/multiprecision/ -I/usr/local/include/boost/multiprecision/cpp_int.hpp -o about_number_overflow about_number_overflow.cpp
int main() {
    // 溢出一
    // C语言方式
    /*int i = 2147483640;
    for (; i > 0; i++) {
        cout << "adding " << i << endl;
    }
    cout << "exit " << i << endl;*/
    //adding 2147483640
    //adding 2147483641
    //adding 2147483642
    //adding 2147483643
    //adding 2147483644
    //adding 2147483645
    //adding 2147483646
    //adding 2147483647
    //exit -2147483648

    // boost version 无限循环
    /*cpp_int i2 = 2147483640;
    for (; i2 > 0; i2++) {
        cout << "adding " << i2 << endl;
    }
    cout << "exit " << i2 << endl;*/

    // 溢出二
    // C语言方式
    int a = 500;
    int b = 400;
    int c = 300;
    int d = 200;
    cout << a * b * c * d << endl;
    //-884901888

    // boost version
    cpp_int ba = 500;
    cpp_int bb = 400;
    cpp_int bc = 300;
    cpp_int bd = 200;
    cout << ba * bb * bc * bd << endl;
    //12000000000

    cpp_int baa = 500;
    int bbb = 400;
    int bcc = 300;
    int bdd = 200;
    cout << baa * bbb * bcc * bdd << endl;
    //12000000000

    // c语言整数溢出问题
    // c语言关于整数表示 32-1 = 31 位
    // c语言的整数不等于数学上的整数
    // 问题原因
    // 与系统设计有关
    // c++中的解决方案
    // 拓展库的使用，如boost库 https://www.boost.org

    return 0;
}