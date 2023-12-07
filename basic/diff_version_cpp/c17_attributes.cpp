//
// Created by fufeng on 2023/12/6.
//

#include <iostream>

//属性标签（attributes）在其他语言中又叫注解（annotations），
// 在C++98/03时代，不同的编译器使用不同的注解去为代码增加一些额外的说明，
// 读者可能在各种C/C++代码中见过像**#pragma**、__declspec、__attribute等注解。
// 然而不同的编译器对于同一功能可能使用不同的注解，这样导致我们需要为不同的编译器编写针对那个编译器的注解代码。
// 从C++11开始，新的语言标准开始统一制定了一些常用的注解标签。使用注解标签的语法是：
//
// [[attribute]] types/functions/enums/etc
//
//这些标签可用于修饰任意类型、函数或者enumeration，在C++17之前不能用于修饰命名空间（namespace）和enumerator，在C++17标准中这个限制也被取消了。

// C++98/03的enumeration和C++11的enumerator
// 对enumeration和enumerator这两个词感到困惑，前者指的从 C 时代就存在的不限定作用域的枚举。例如下面的枚举类型就是一个enumeration：
//一个enumeration的例子
/*enum Color {
    black,
    white,
    red
};

//无法编译通过
bool white = true;*/

//枚举类型之所以称之为不限定作用域的枚举是因为一旦定义了这样一种枚举，在其所在的作用域内，不能再定义与之同名的变量了。
// 例如如果定义了上述Color枚举，此时再定义一个名叫white的变量，是无法编译通过的。
// 而enumerator指的是从C++11开始引入的、如下形式定义的枚举变量：
//一个enumerator的例子
enum class Color {
    black,
    white,
    red
};

//可以编译通过
bool white = true;

//此时，由于枚举值white对外部不可见（必须通过Color::white来引用），可以定义一个同名的white变量。
// 这种枚举变量被称之为限定作用域的枚举。

//C++17的注解标签
//C++11引入了的常用的注解标签有**[[noreturn]]**，这个注解的含义是告诉编译器某个函数没有返回值，例如：
//
[[noreturn]] void terminate();
//

//这个标签一般在设计一些系统函数时使用，例如std::abort()和std::exit()。
//
//C++14引入了**[[deprecated]]**标签用于表示一个函数或者类型等已经被弃用，当你使用这些被弃用的函数或者类型，编译时编译器会给出相应的警告，有的编译器直接产生编译错误。
//
[[deprecated]] void funcX();
//

//这个标签在实际开发中非常有用，尤其在设计一些库代码时，如果库的作者希望某个函数或者类型不想再被用户使用可以使用该标注标记。当然，你也可以使用以下语法给出编译时的具体警告或者出错信息：
//[[deprecated("use funY instead")]] void funcX();

[[deprecated("use funcY instead")]] void funcX() {
    //实现省略...
}

//#define __deprecated__
#ifdef __deprecated__
int main(int argc, char *argv[]) {
    funcX();
    return 0;
}
#endif

// g++ -g -o c17_attributes c17_attributes.cpp -std=c++11
//c17_attributes.cpp:65:5: warning: 'funcX' is deprecated: use funcY instead [-Wdeprecated-declarations]
//    funcX();
//    ^
//c17_attributes.cpp:59:3: note: 'funcX' has been explicitly marked deprecated here
//[[deprecated("use funcY instead")]] void funcX()
//  ^
//1 warning generated.

//C++17提供了如下三个实用注解：
//
//[[fallthrough]]
//[[nodiscard]]
//[[maybe_unused]]

//[[fallthrough]]用于switch-case语句中，当某个case分支执行完毕后如果没有break语句，编译器可能会给出一条警告，
// 但有时候这可能是开发者故意为之的，为了让编译器明确的知道开发者的意图，
// 可以在需要某个case分支被“贯穿”处（上一个case没有break）显式设置[[fallthrough]]**标记。代码示例如下：
int func(int x) {
    switch (x) {
        case 1:
            std::cout << "x is 1" << std::endl;
            //break;
            //这个位置缺少break语句，且没有fallthrough标注，
            //可能是一个逻辑错误，编译时编译器可能会给出警告，以提醒修改之
        case 2:
            std::cout << "x is 2" << std::endl;
            [[fallthrough]];
            //这里也缺少break语法，但是使用了fallthrough标注，
            //说明是开发者有意为之，编译器不会给出任何警告
        case 3:
            std::cout << "x is 3" << std::endl;
            break;
        default:
            std::cout << "x is not 1, 2 or 3" << std::endl;
    }
    return 0;
}
//注意，在gcc/g++中**[[fallthrough]]**后面的分号不是必须的，在Visual Studio中必须加上分号，否则无法编译通过。
//g++ -g c17_attributes.cpp -std=c++17

//熟悉golang语言的读者，可能对fallthrough这一语法特性非常熟悉，golang中在switch-case后加上fallthrough是一个常用的告诉编译器意图的语法规则。例如：
////以下是golang语法
//s := "abcd"
//switch s[3] {
//    case 'a':
//    	fmt.Println("The integer was <= 4")
//    	fallthrough
//    case 'b':
//    	fmt.Println("The integer was <= 5")
//    	fallthrough
//    case 'c':
//    	fmt.Println("The integer was <= 6")
//    default:
//    	fmt.Println("default case")
//}

//[[nodiscard]]一般用于修饰函数，告诉函数调用者必须关注该函数的返回值（即不能丢弃该函数返回值）。
// 如果调用者未将该函数的返回值赋值给一个变量，编译器会给出一个警告。
// 例如假设有一个网络连接函数connect，我们通过返回值明确的说明了连接是否建立成功，为了防止调用者在使用时直接将其值丢弃，我们可以将该函数使用[[nodiscard]]**标注标记：
[[nodiscard]] int connect(const char *address, short port) {
    //实现省略...
    return 0;
}

//#define __nodiscard__
#ifdef __nodiscard__

int main() {
    connect("127.0.0.1", 8888);

    return 0;
}

#endif
//g++ -Wall -g c17_attributes.cpp -std=c++17
//c17_attributes.cpp:142:5: warning: ignoring return value of function declared with 'nodiscard' attribute [-Wunused-result]
//    connect("127.0.0.1", 8888);
//    ^~~~~~~ ~~~~~~~~~~~~~~~~~

// 到C++20中对于诸如operator new()、std::allocate()等库函数均使用了[[nodiscard]]进行了标记，以强调必须使用这些函数的返回值。

//[[maybe_unused]]有些编译器会对程序代码中未被使用的函数或变量给出警告，
// 在C++17之前，程序员们为了消除这些警告要么修改编译器警告选项设置，
// 要么定义一个类似于UNREFERENCED_PARAMETER的宏来显式调用这些未使用的变量一次以消除编译警告。

#define UNREFERENCED_PARAMETER(x) x

//int APIENTRY wWinMain(HINSTANCE hInstance,
//                      HINSTANCE hPrevInstance,
//                      LPWSTR lpCmdLine,
//                      int nCmdShow) {
//UNREFERENCED_PARAMETER(hPrevInstance);
//UNREFERENCED_PARAMETER(lpCmdLine);
////...无关代码省略
//return 0;
//}

//上述代码节选自一个标准的Win32程序的结构，其中函数参数hPrevInstance和lpCmdLine一般不会被用到，编译器会给出警告，
// 为了消除这类警告，定义了一个宏UNREFERENCED_PARAMETER，并调用之，造成这两个参数被使用到的假象。
//
//有了[[maybe_unused]]注解之后，再也不需要这类宏来“欺骗”编译器了。上述代码使用该注解可以修改如下：

//int APIENTRY wWinMain(HINSTANCE hInstance,
//                     [[maybe_unused]] HINSTANCE hPrevInstance,
//                     [[maybe_unused]] LPWSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//    //...无关代码省略
//}


