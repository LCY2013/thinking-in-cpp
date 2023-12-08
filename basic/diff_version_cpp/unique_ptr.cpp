//
// Created by fufeng on 2023/12/8.
//

#include <iostream>
#include <memory>

//stl中的智能指针类详解

// C/C++语言最为人所诟病的特性之一就是存在内存泄露问题，因此后来的大多数语言都提供了内置内存分配与释放功能，
// 有的甚至干脆对语言的使用者屏蔽了内存指针这一概念。这里不置贬褒，手动分配内存与手动释放内存有利也有弊，
// 自动分配内存和自动释放内存亦如此，这是两种不同的设计哲学。有人认为，内存如此重要的东西怎么能放心交给用户去管理呢？
// 而另外一些人则认为，内存如此重要的东西怎么能放心交给系统去管理呢？
// 在C/C++语言中，内存泄露的问题一直困扰着广大的开发者，
// 因此各类库和工具也一直在努力尝试各种方法去检测和避免内存泄露，如boost，因此智能指针技术应运而生。
//

//C++98/03的尝试——std::auto_ptr
//
//在2020年讨论std::auto_ptr不免让人怀疑是不是有点过时了，确实如此，随着C++11标准的出现（最新标准是 C++20），
// std::auto_ptr已经被彻底废弃了，取而代之是std::unique_ptr。
// 然而，我之所以还向你介绍std::auto_ptr的用法以及它在设计上的不足之处是想让你了解C++语言中智能指针的发展历程，
// 一项技术如果我们了解它过去的样子和发展的轨迹，我们就能更好地掌握它，不是吗？
//
//std::auto_ptr的基本用法如下所示：
//g++ -Wall -g unique_ptr.cpp -std=c++11
//#define __auto_ptr__
#ifdef __auto_ptr__
void func1() {
    // 初始化方法一
    std::auto_ptr<int> sp1(new int(10));
    // 初始化方法二
    std::auto_ptr<int> sp2;
    sp2.reset(new int(8));
}
//智能指针对象sp1和sp2均持有一个在堆上分配的int对象，值都是8，这两块堆内存都在sp1和sp2释放时得到释放。这是std::auto_ptr的基本用法。

//sp是smart pointer（智能指针）的简写。

//std::auto_ptr真正容易让人误用的地方是其不常用的复制语义，即当复制一个std::auto_ptr对象时（拷贝复制或operator =复制），
//原对象所持有的堆内存对象也会转移给复制出来的对象。示例代码如下：
void func2() {
    //测试拷贝构造
    std::auto_ptr<int> sp1(new int(20));
    std::auto_ptr<int> sp2(sp1);

    if (sp1.get() != NULL) {
        std::cout << "sp1 is not empty." << std::endl;
    } else {
        std::cout << "sp1 is empty." << std::endl;
    }

    if (sp2.get() != NULL) {
        std::cout << "sp2 is not empty." << std::endl;
    } else {
        std::cout << "sp2 is empty." << std::endl;
    }

    if (sp1.get() == sp2.get()) {
        std::cout << "sp1和sp2所持有的堆内存对象是同一个" << std::endl;
    } else {
        std::cout << "sp1和sp2所持有的堆内存对象不是同一个" << std::endl;
    }

    //测试赋值构造
    std::auto_ptr<int> sp3(new int(30));
    std::auto_ptr<int> sp4;
    sp4 = sp3;
    if (sp3.get() != NULL) {
        std::cout << "sp3 is not empty." << std::endl;
    } else {
        std::cout << "sp3 is empty." << std::endl;
    }
    if (sp4.get() != NULL) {
        std::cout << "sp4 is not empty." << std::endl;
    } else {
        std::cout << "sp4 is empty." << std::endl;
    }
    if (sp3.get() == sp4.get()) {
        std::cout << "sp3和sp4所持有的堆内存对象是同一个" << std::endl;
    } else {
        std::cout << "sp3和sp4所持有的堆内存对象不是同一个" << std::endl;
    }
}

//上述代码中分别利用拷贝构造（sp1=>sp2）和赋值构造（sp3=>sp4）来创建新的std::auto_ptr对象，
// 因此sp1 持有的堆对象被转移给sp2，sp3持有的堆对象被转移给sp4。得到程序执行结果如下：
//sp1 is empty.
//sp2 is not empty.
//sp1和sp2所持有的堆内存对象不是同一个
//sp3 is empty.
//sp4 is not empty.
//sp3和sp4所持有的堆内存对象不是同一个

//由于std::auto_ptr这种不常用的复制语义，应该避免在stl容器中使用std::auto_ptr，例如绝不应该写出如下代码：
void func3() {
    std::vector<std::auto_ptr<int>> myvectors;
}
#endif

//当用算法对容器操作的时候（如最常见的容器元素遍历），很难避免不对容器中的元素实现赋值传递，这样便会使容器中多个元素被置为空指针，这不是我们希望看到的，可能会造成一些意想不到的错误。
//
//以史为鉴，作为std::auto_ptr的替代者std::unique_ptr吸取了这个经验教训，下文会来详细介绍。
//
//正因为std::auto_ptr的设计存在如此重大缺陷，C++11标准在充分借鉴和吸收了boost库中智能指针的设计思想，引入了三种类型的智能指针，即std::unique_ptr、std::shared_ptr和std::weak_ptr。

//boost还有scoped_ptr，C++11并没有全部照搬，而是选择了三个最实用的指针类型。
// 在C++11中可以通过std::unique_ptr达到与boost::scoped_ptr一样的效果。

//所有的智能指针类（包括std::unique_ptr）均包含于头文件<memory>中。
//
//正因为存在上述设计上的缺陷，在C++11及后续语言规范中std::auto_ptr已经被废弃，你的代码不应该再使用它。

//std::unique_ptr
//
//std::unique_ptr对其持有的堆内存具有唯一拥有权，也就是说引用计数永远是1，std::unique_ptr对象销毁时会释放其持有的堆内存。
//可以使用以下方式初始化一个std::unique_ptr对象：
//g++ -Wall -g unique_ptr.cpp -std=c++17
void func4() {
    //初始化方式1
    std::unique_ptr<int> sp1(new int(123));

    //初始化方式2
    std::unique_ptr<int> sp2;
    sp2.reset(new int(123));

    //初始化方式3
    std::unique_ptr<int> sp3 = std::make_unique<int>(123);
}


int main() {
    //func1();
    //func2();
    //func3();
    func4();
}
