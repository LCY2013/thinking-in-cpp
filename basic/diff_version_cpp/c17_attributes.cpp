//
// Created by fufeng on 2023/12/6.
//

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

[[deprecated("use funcY instead")]] void funcX()
{
    //实现省略...
}

int main(int argc, char *argv[]) {
    funcX();
    return 0;
}

// g++ -g -o c17_attributes c17_attributes.cpp -std=c++11
//c17_attributes.cpp:65:5: warning: 'funcX' is deprecated: use funcY instead [-Wdeprecated-declarations]
//    funcX();
//    ^
//c17_attributes.cpp:59:3: note: 'funcX' has been explicitly marked deprecated here
//[[deprecated("use funcY instead")]] void funcX()
//  ^
//1 warning generated.


