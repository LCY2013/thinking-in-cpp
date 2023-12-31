//
// Created by fufeng on 2023/12/8.
//

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

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

// 你应该尽量使用初始化方式3的方式去创建一个std::unique_ptr而不是方式1和2，因为形式3更安全，原因Scott Meyers在其《Effective Modern C++》中已经解释过了
//  1、异常安全性： std::make_unique 能够提供更好的异常安全性。如果在分配内存和构造对象的过程中发生异常，std::make_unique 会确保在异常抛出前释放已分配的内存，防止内存泄漏。直接使用 new 来创建 std::unique_ptr 的方式在异常发生时可能导致内存泄漏。
//
//  2、代码简洁： std::make_unique 语法更为简洁，不需要显式指定对象的类型，而是根据参数自动推导。这有助于减少代码重复和提高代码的可读性。
//
//  3、效率： 在某些情况下，使用 std::make_unique 可能会更加高效。编译器有时可以优化 std::make_unique 的实现，以减少构造函数调用的开销。

//让很多人对C++11规范吐槽的地方之一是，C++11新增了std::make_shared()方法创建一个std::shared_ptr对象，
//却没有提供相应的std::make_unique()方法创建一个std::unique_ptr对象，这个方法直到C++14才被添加进来。
//当然，在C++11中你很容易实现出一个这样的方法：
template<typename T, typename ... Ts>
std::unique_ptr<T> make_unique(Ts &&...params) {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

//鉴于std::auto_ptr的前车之鉴，std::unique_ptr禁止复制语义，为了达到这个效果，
// std::unique_ptr类的拷贝构造函数和赋值运算符（operator =）被标记为 =delete。
//template <class T>
//class unique_ptr
//{
//	//省略其他代码...
//
//	//拷贝构造函数和赋值运算符被标记为delete
//    unique_ptr(const unique_ptr&) = delete;
//    unique_ptr& operator=(const unique_ptr&) = delete;
//};

//因此，下列代码是无法通过编译的：
void func5() {
    std::unique_ptr<int> sp1(std::make_unique<int>(123));;

    //以下代码无法通过编译
    //std::unique_ptr<int> sp2(sp1);
    std::unique_ptr<int> sp3;
    //以下代码无法通过编译
    //sp3 = sp1;
}

//禁止复制语义也存在特例，即可以通过一个函数返回一个std::unique_ptr：
std::unique_ptr<int> func(int val) {
    std::unique_ptr<int> up(new int(val));
    return up;
}

void func6() {
    std::unique_ptr<int> up1 = func(123);
}

//上述代码从func函数中得到一个std::unique_ptr对象，然后返回给up1。
//
//既然std::unique_ptr不能复制，那么如何将一个std::unique_ptr对象持有的堆内存转移给另外一个呢？
// 答案是使用移动构造，示例代码如下：
void func7() {
    std::unique_ptr<int> sp1(std::make_unique<int>(567));
    std::unique_ptr<int> sp2(std::move(sp1));
    std::unique_ptr<int> sp3;
    sp3 = std::move(sp2);
}

//以上代码利用std::move将sp1持有的堆内存（值为123）转移给sp2，再把sp2转移给sp3。
//最后，sp1和sp2不再持有堆内存的引用，变成一个空的智能指针对象。
// 并不是所有的对象的std::move操作都有意义，只有实现了移动构造函数（Move Constructor）
// 或移动赋值运算符（operator =）的类才行，而std::unique_ptr正好实现了这二者，以下是实现伪码：
//template<typename T, typename Deletor>
//class unique_ptr
//{
//    //其他函数省略...
//public:
//    unique_ptr(unique_ptr&& rhs)
//    {
//        this->m_pT = rhs.m_pT;
//        //源对象释放
//        rhs.m_pT = nullptr;
//    }
//
//    unique_ptr& operator=(unique_ptr&& rhs)
//    {
//        this->m_pT = rhs.m_pT;
//        //源对象释放
//        rhs.m_pT = nullptr;
//        return *this;
//    }
//
//private:
//    T*    m_pT;
//};

//这是std::unique_ptr具有移动语义的原因，希望读者可以理解之。
//
//std::unique_ptr不仅可以持有一个堆对象，也可以持有一组堆对象，示例如下：
//g++ -g -o unique_ptr unique_ptr.cpp -std=c++17
void func8() {
    //创建10个int类型的堆对象
    //形式1
    std::unique_ptr<int[]> sp1(new int[10]);

    //形式2
    std::unique_ptr<int[]> sp2;
    sp2.reset(new int[10]);
    //形式3
    std::unique_ptr<int[]> sp3(std::make_unique<int[]>(10));

    for (int i = 0; i < 10; ++i) {
        sp1[i] = i;
        sp2[i] = i;
        sp3[i] = i;
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << sp1[i] << ", " << sp2[i] << ", " << sp3[i] << std::endl;
    }
}

//std::shared_ptr和std::weak_ptr也可以持有一组堆对象，用法与std::unique_ptr相同，下文不再赘述。
void func9() {
    std::shared_ptr<int[]> sp1(new int[10]);
    std::shared_ptr<int[]> sp2;
    sp2.reset(new int[10]);
    //std::shared_ptr<int> sp3(std::make_shared<int>(10));
    //std::shared_ptr<vector<int>> sp3(vector<int>());
    auto sp4 = sp2;
    auto sp5 = std::make_shared<vector<int>>();

    for (int i = 0; i < 10; ++i) {
        sp1[i] = i;
    }
}

//自定义智能指针对象持有的资源的释放函数
// 默认情况下，智能指针对象在析构时只会释放其持有的堆内存（调用delete 或者delete[]），
// 但是假设这块堆内存代表的对象还对应一种需要回收的资源（如操作系统的套接字句柄、文件句柄等），
// 我们可以通过自定义智能指针的资源释放函数。
// 假设现在有一个MySmartPtr类，对应着操作系统的套接字句柄，在回收时需要关闭该对象，
// 我们可以如下自定义智能指针对象的资源析构函数，这里以std::unique_ptr为例：
class MySmartPtr {
public:
    MySmartPtr() {
        std::cout << "MySmartPtr无参构造函数" << std::endl;
    }

    MySmartPtr(int *p) : m_p(p) {
        std::cout << "MySmartPtr构造函数" << std::endl;
    }

    ~MySmartPtr() {
        std::cout << "MySmartPtr析构函数" << std::endl;
        delete m_p;
    }

    //关闭资源句柄
    void close() {
        cout << "关闭资源句柄" << endl;
    }

    int *get() {
        return m_p;
    }

private:
    int *m_p;
};

void func10() {
    auto deletor = [](MySmartPtr *pSocket) {
        //关闭句柄
        pSocket->close();
        //TODO: 你甚至可以在这里打印一行日志...
        std::cout << "清理完成" << std::endl;
        delete pSocket;
    };

    std::unique_ptr<MySmartPtr, void (*)(MySmartPtr *pSocket)> spSocket(new MySmartPtr(), deletor);
}

//自定义std::unique_ptr的资源释放函数其规则是：
//std::unique_ptr<T, DeletorFuncPtr>
//其中，DeletorFuncPtr是个函数指针，该函数的第一个参数是指向T类型的指针，
//该函数的返回值是void，该函数的作用是释放T类型的资源。
// 注意：
// 1. 自定义的资源释放函数必须是静态的，即不能是类的成员函数。
// 2. 自定义的资源释放函数不能是虚函数。
// 3. 自定义的资源释放函数不能是重载的。

//其中T是你要释放的对象类型，DeletorFuncPtr是一个自定义函数指针。
// 上述代码表示DeletorFuncPtr有点复杂，我们可以使用decltype(deletor)让编译器自己推导deletor的类型，
// 因此可以将代码修改为：
//std::unique_ptr<MySmartPtr, decltype(deletor)> spSocket(new Socket(), deletor);
void func11() {
    auto deletor = [](MySmartPtr *pSocket) {
        //关闭句柄
        pSocket->close();
        //TODO: 你甚至可以在这里打印一行日志...
        std::cout << "清理完成" << std::endl;
        delete pSocket;
    };

    std::unique_ptr<MySmartPtr, decltype(deletor)> spSocket(new MySmartPtr(), deletor);
}

void funcUniquePtr() {
//    func1();
//    func2();
//    func3();
//    func4();
//    func5();
//    func6();
//    func7();
//    func8();
//    func9();
//    func10();
//    func11();
}

//std::shared_ptr
//
//std::unique_ptr对其持有的资源具有独占性，而std::shared_ptr持有的资源可以在多个std::shared_ptr之间共享，
//每多一个std::shared_ptr对资源的引用，资源引用计数将增加1，每一个指向该资源的std::shared_ptr对象析构时，
// 资源引用计数减1，最后一个std::shared_ptr对象析构时，发现资源计数为0，将释放其持有的资源。
// 多个线程之间，递增和减少资源的引用计数是安全的。（
// 注意：这不意味着多个线程同时操作std::shared_ptr引用的对象是安全的）。
// std::shared_ptr提供了一个use_count()方法来获取当前持有资源的引用计数。
// 除了上面描述的，std::shared_ptr用法和std::unique_ptr基本相同。
//
//下面是一个初始化std::shared_ptr的示例：
void func12() {
    //初始化方式1
    std::shared_ptr<int> sp1(new int(123));

    //初始化方式2
    std::shared_ptr<int> sp2;
    sp2.reset(new int(123));

    //初始化方式3
    std::shared_ptr<int> sp3;
    sp3 = std::make_shared<int>(123);
}

//和std::unique_ptr一样，你应该优先使用std::make_shared去初始化一个std::shared_ptr对象。
//
//再来看另外一段代码：
class SharedPtr {
public:
    SharedPtr() {
        std::cout << "SharedPtr constructor" << std::endl;
    }

    ~SharedPtr() {
        std::cout << "SharedPtr destructor" << std::endl;
    }
};

void func13() {
    //初始化方式1
    std::shared_ptr<SharedPtr> sp1(new SharedPtr());

    std::cout << "use count: " << sp1.use_count() << std::endl;

    //初始化方式2
    std::shared_ptr<SharedPtr> sp2(sp1);
    std::cout << "use count: " << sp1.use_count() << std::endl;

    sp2.reset();
    std::cout << "use count: " << sp1.use_count() << std::endl;

    {
        std::shared_ptr<SharedPtr> sp3 = sp1;
        std::cout << "use count: " << sp1.use_count() << std::endl;
    }

    std::cout << "use count: " << sp1.use_count() << std::endl;
}

//sp1构造时，同时触发对象SharedPtr的构造，因此SharedPtr的构造函数会执行；
//此时只有一个sp1对象引用new出来的SharedPtr对象，因此打印出来的引用计数值为1；
//利用sp1拷贝一份sp2，导致打印出来的引用计数为2；
//调用sp2的reset()方法，sp2释放对资源对象SharedPtr的引用，因此打印的引用计数值再次变为1；
//利用sp1再次创建sp3，因此打印的引用计数变为2；
//程序执行到sp3出了其作用域被析构，资源SharedPtr的引用计数递减1，因此代码打印的引用计数为1；
//程序执行到sp1出了其作用域被析构，在其析构时递减资源A的引用计数至0，并析构资源A对象，因此类A的析构函数被调用。

//std::enable_shared_from_this
//实际开发中，有时候需要在类中返回包裹当前对象（this）的一个std::shared_ptr对象给外部使用，C++新标准也为考虑到了这一点，
//有如此需求的类只要继承自std::enable_shared_from_this<T>模板对象即可。用法如下：
class A : public std::enable_shared_from_this<A> {
public:
    A() {
        std::cout << "A constructor" << std::endl;
    }

    ~A() {
        std::cout << "A destructor" << std::endl;
    }

    std::shared_ptr<A> getSelf() {
        return shared_from_this();
    }
};

void func14() {
    std::shared_ptr<A> spA(new A());
    std::shared_ptr<A> spB = spA->getSelf();
    std::cout << "use count: " << spA.use_count() << std::endl;
}

//上述代码中，类A继承自std::enable_shared_from_this<A>并提供一个getSelf()方法返回自身的std::shared_ptr对象，在getSelf()中调用shared_from_this()即可。
//
//std::enable_shared_from_this用起来比较方便，但是也存在很多不易察觉的陷阱。

//陷阱一：不应该共享栈对象的 this 给智能指针对象
// 假设我们将上面代码生成 A 对象的方式改成一个栈变量，即：
void func15() {
    A a;
    std::shared_ptr<A> spA = a.getSelf();
}
//运行修改后的代码会发现程序在std::shared_ptr<A> spA = a.getSelf();产生崩溃。
// 这是因为，智能指针管理的是堆对象，栈对象会在函数调用结束后自行销毁，因此不能通过shared_from_this()将该对象交由智能指针对象管理。
// 切记：智能指针最初设计的目的就是为了管理堆对象的（即那些不会自动释放的资源）。

//陷阱二：避免std::enable_shared_from_this的循环引用问题
class B : public std::enable_shared_from_this<B> {
public:
    B() {
        m_i = 9;
        //注意:
        //比较好的做法是在构造函数里面调用shared_from_this()给m_SelfPtr赋值
        //但是很遗憾不能这么做,如果写在构造函数里面程序会直接崩溃

        std::cout << "B constructor" << std::endl;
    }

    ~B() {
        m_i = 0;

        std::cout << "B destructor" << std::endl;
    }

    void func() {
        m_SelfPtr = shared_from_this();
    }

public:
    int m_i;
    std::shared_ptr<B> m_SelfPtr;

};

void func16() {
    {
        std::shared_ptr<B> spa(new B());
        spa->func();
        cout << spa.use_count() << endl;
    }
}

//我们发现在程序的整个生命周期内，只有B类构造函数的调用输出，没有B类析构函数的调用输出，这意味着new出来的B对象产生了内存泄漏了！
//
//我们来分析一下new出来的B对象为什么得不到释放。当程序执行到作用域后，spa出了其作用域准备析构，
//在析构时其发现仍然有另外的一个std::shared_ptr对象即B::m_SelfPtr 引用了B，因此spa只会将B的引用计数递减为1，
// 然后就销毁自身了。现在留下一个矛盾的处境：必须销毁B才能销毁其成员变量m_SelfPtr，而销毁B必须先销毁m_SelfPtr。
// 这就是所谓的std::enable_shared_from_this的循环引用问题。我们在实际开发中应该避免做出这样的逻辑设计，
// 这种情形下即使使用了智能指针也会造成内存泄漏。也就是说一个资源的生命周期可以交给一个智能指针对象，
// 但是该智能指针的生命周期不可以再交给该资源本身来管理。

// std::weak_ptr
//std::weak_ptr是一个不控制资源生命周期的智能指针，是对对象的一种弱引用，只是提供了对其管理的资源的一个访问手段，
// 引入它的目的为协助std::shared_ptr工作。
//
//std::weak_ptr可以从一个std::shared_ptr或另一个std::weak_ptr对象构造，
//std::shared_ptr可以直接赋值给std::weak_ptr ，也可以通过std::weak_ptr的lock()函数来获得std::shared_ptr。
// 它的构造和析构不会引起引用计数的增加或减少。
// std::weak_ptr可用来解决std::shared_ptr相互引用时的死锁问题，
// 即两个std::shared_ptr相互引用，那么这两个指针的引用计数永远不可能下降为0， 资源永远不会释放。
void func17() {
    //创建一个std::shared_ptr对象
    std::shared_ptr<int> sp1(new int(123));
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过构造函数得到一个std::weak_ptr对象
    std::weak_ptr<int> sp2(sp1);
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过赋值运算符得到一个std::weak_ptr对象
    std::weak_ptr<int> sp3 = sp1;
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过一个std::weak_ptr对象得到另外一个std::weak_ptr对象
    std::weak_ptr<int> sp4 = sp2;
    std::cout << "use count: " << sp1.use_count() << std::endl;
}

//无论通过何种方式创建std::weak_ptr都不会增加资源的引用计数，因此每次输出引用计数的值都是1。
//
//既然，std::weak_ptr不管理对象的生命周期，那么其引用的对象可能在某个时刻被销毁了，如何得知呢？
//std::weak_ptr提供了一个expired()方法来做这一项检测，返回true，说明其引用的资源已经不存在了；
// 返回false，说明该资源仍然存在，这个时候可以使用std::weak_ptr 的lock()方法得到一个std::shared_ptr对象然后继续操作资源，
// 以下代码演示了该用法：
void func18() {
    std::shared_ptr<int> sp1(new int(123));
    std::weak_ptr<int> sp2(sp1);

    std::cout << "use count: " << sp1.use_count() << std::endl;

    if (sp2.expired()) {
        std::cout << "sp2 is expired" << std::endl;
    } else {
        std::shared_ptr<int> sp3 = sp2.lock();
    }

//    //tmpConn_是一个std::weak_ptr<TcpConnection>对象
//    //tmpConn_引用的TcpConnection已经销毁，直接返回
//    if (tmpConn_.expired())
//        return;
//
//    std::shared_ptr<TcpConnection> conn = tmpConn_.lock();
//    if (conn)
//    {
//        //对conn进行操作，省略...
//    }

}

//有可能对上述代码产生疑问，既然使用了std::weak_ptr的expired()方法判断了对象是否存在，
//为什么不直接使用std::weak_ptr对象对引用资源进行操作呢？
// 实际上这是行不通的，std::weak_ptr类没有重写operator->和operator方法，
// 因此不能像std::shared_ptr或std::unique_ptr一样直接操作对象，
// 同时std::weak_ptr类也没有重写operator bool()操作，因此也不能通过std::weak_ptr*对象直接判断其引用的资源是否存在：

class Func19A {
public:
    void doSomething() {

    }
};

void func19() {
    std::shared_ptr<Func19A> sp1(new Func19A());
    std::weak_ptr<Func19A> sp2(sp1);

    // 正确
    if (sp1) {
        sp1->doSomething();
        (*sp1).doSomething();
    }
    if (!sp1) {

    }

    // 错误
    /*if (sp2) {
        sp2->doSomething();
        (*sp2).doSomething();
    }
    if (!sp2) {

    }*/
}

//之所以std::weak_ptr不增加引用资源的引用计数来管理资源的生命周期，是因为即使它实现了以上说的几个方法，调用它们仍然是不安全的，因为在调用期间，引用的资源可能恰好被销毁了，这样可能会造成比较棘手的错误和麻烦。
//
//因此，std::weak_ptr的正确使用场景是那些资源如果可用就使用，如果不可用则不使用的场景，它不参与资源的生命周期管理。例如，网络分层结构中，Session对象（会话对象）利用Connection对象（连接对象）提供的服务来进行工作，但是Session对象不管理Connection对象的生命周期，Session管理Connection的生命周期是不合理的，因为网络底层出错会导致Connection对象被销毁，此时Session对象如果强行持有Connection对象则与事实矛盾。
//
//std::weak_ptr的应用场景，经典的例子是订阅者模式或者观察者模式中。这里以订阅者为例来说明，消息发布器只有在某个订阅者存在的情况下才会向其发布消息，而不能管理订阅者的生命周期。
class Subscriber {
    //...
public:
    virtual void onMessage(const std::string &msg) = 0;
};

class SubscriberManager {
public:
    void publish(const std::string &msg) {
        for (auto &subscriber: subscribers_) {
            if (subscriber.expired()) {
                continue;
            }
            if (auto sp = subscriber.lock()) {
                // 给订阅者发送消息
                sp->onMessage(msg);
            }
        }
    }

private:
    std::vector<std::weak_ptr<Subscriber>> subscribers_;
};


void funcSharedPtr() {
    //func12();
    //func13();
    //func14();
    //func15();
    //func16();
    //func17();
}

void funcWeakPtr() {
    //func18();
    func19();
}

// 智能指针对象的大小
//一个std::unique_ptr对象大小与裸指针大小相同（即sizeof(std::unique_ptr<T>) == sizeof(void*)），
// 而std::shared_ptr的大小是std::unique_ptr的一倍。
//
//测试代码
void funcPrtSize() {
    shared_ptr<int> sp0;
    shared_ptr<string> sp1;
    sp1.reset(new string("hello"));
    unique_ptr<int> sp2;
    weak_ptr<int> sp3;

    cout << "sizeof(sp0): " << sizeof(sp0) << endl;
    cout << "sizeof(sp1): " << sizeof(sp1) << endl;
    cout << "sizeof(sp2): " << sizeof(sp2) << endl;
    cout << "sizeof(sp3): " << sizeof(sp3) << endl;

    //sizeof(sp0): 16
    //sizeof(sp1): 16
    //sizeof(sp2): 8
    //sizeof(sp3): 16
    //在64位机器上，std_unique_ptr占8字节，std::shared_ptr和std::weak_ptr占16字节。
    // 也就是说，std_unique_ptr的大小总是和原始指针大小一样，std::shared_ptr和std::weak_ptr大小是原始指针的一倍。
}

//智能指针使用注意事项

//C++新标准提倡的理念之一是不应该再手动调用delete或者free函数去释放内存了，而应该把它们交给新标准提供的各种智能指针对象。
// C++新标准中的各种智能指针是如此的实用与强大，在现代C++ 项目开发中，读者应该尽量去使用它们。
// 智能指针虽然好用，但稍不注意，也可能存在许多难以发现的bug，这里我根据经验总结了几条：

//一、一旦一个对象使用智能指针管理后，就不该再使用原始裸指针去操作；
//二、智能指针的拷贝构造函数和赋值运算符是禁用的，因此不应该使用它们；
//三、智能指针的移动构造函数和移动赋值运算符是默认的，因此应该使用它们；
//四、智能指针的析构函数是默认的，因此不应该重写它；
//五、智能指针的reset()方法是唯一可以重写的；
//六、智能指针的get()方法是唯一可以重写的；


//1、一旦一个对象使用智能指针管理后，就不该再使用原始裸指针去操作；
class TipsClass {

};

void tips_one() {
    TipsClass *p = new TipsClass();
    unique_ptr<TipsClass> up(p);
    // 错误，不能使用原始指针去操作
    //delete p;
    // 正确，使用智能指针去操作
    //up->~TipsClass();
}

//这段代码创建了一个堆对象TipsClass，然后利用智能指针up 去管理之，可是私下却利用原始指针销毁了该对象，这让智能指针对象up情何以堪啊？
//
//记住，一旦智能指针对象接管了你的资源，所有对资源的操作都应该通过智能指针对象进行，不建议再通过原始指针进行操作了。
// 当然，除了std::weak_ptr，std::unique_ptr和std::shared_ptr都提供了获取原始指针的方法——**get()**函数。
void tips_two() {
    TipsClass *p = new TipsClass();
    unique_ptr<TipsClass> up(p);
    // originPrt 和 p 是指向同一个对象
    TipsClass *originPrt = up.get();
}

//2. 分清楚场合应该使用哪种类型的智能指针；
//通常情况下，如果你的资源不需要在其他地方共享，那么应该优先使用std::unique_ptr，反之使用std::shared_ptr，
// 当然这是在该智能指针需要管理资源的生命周期的情况下；如果不需要管理对象的生命周期，请使用std::weak_ptr。

//3. 认真考虑，避免操作某个引用资源已经释放的智能指针；
//前面的例子，一定让读者觉得非常容易知道一个智能指针持有的资源是否还有效，但还是建议在不同场景谨慎一点，有些场景是很容易造成误判的。
// 例如下面的代码：
class Resource {
public:
    Resource() {
        cout << "Resource()" << endl;
    }

    ~Resource() {
        cout << "~Resource()" << endl;
    }

    void doSomething() {
        std::cout << "Resource do something..." << data << std::endl;
    }

private:
    int data;
};

void tips_three() {
    shared_ptr<Resource> up(new Resource());
    const auto &up1 = up;
    up.reset();

    //由于up1已经不再持有对象的引用，程序会在这里出现意外的行为
    up1->doSomething();
}

//上述代码中，up1是up的引用，up被置空后，up1也一同为空。这时候调用up1->doSomething()，
// up1->（即 operator->）在内部会调用**get()**方法获取原始指针对象，这时会得到一个空指针（地址为0），继续调用doSomething()导致程序崩溃。
//
//可能觉得上述代码片段存在的问题是显而易见的，ok，让把这个例子放到实际开发中再来看一下：
//连接断开
//void MonitorServer::OnClose(const std::shared_ptr<TcpConnection>& conn)
//{
//    std::lock_guard<std::mutex> guard(m_sessionMutex);
//    for (auto iter = m_sessions.begin(); iter != m_sessions.end(); ++iter)
//    {
//        //通过比对connection对象找到对应的session
//        if ((*iter)->GetConnectionPtr() == conn)
//        {
//            m_sessions.erase(iter);
//            //注意这里：程序崩溃
//            LOGI("monitor client disconnected: %s", conn->peerAddress().toIpPort().c_str());
//            break;
//        }
//    }
//}

//注意代码中我提醒注意的地方，该段程序会崩溃，崩溃原因是调用了conn->peerAddress()方法。为什么这个方法的调用可能会引起崩溃？现在还可以显而易见地看出问题吗？
//
//崩溃原因是传入的conn对象和上一个例子中的up1一样都是另外一个std::shared_ptr的引用，当连接断开时，对应的TcpConnection对象可能早已被销毁，而conn引用就会变成空指针（严格来说是不再持有一个TcpConnection对象的引用），此时调用TcpConnection的peerAddress()方法就会产生和上一个示例一样的错误。

//3、作为类成员变量时，应该优先使用前置声明（forward declarations）
//我们知道，为了减少编译依赖、加快编译速度和减小生成的二进制文件的大小，
// C/C++项目中一般在*.h文件对于指针类型尽量使用前置声明，而不是直接包含对应类的头文件。例如：
/*
//Test.h
//在这里使用A的前置声明，而不是直接包含A.h文件
class A;

class Test
{
public:
    Test();
    ~Test();

private:
    A*      m_pA;
};
 */

//同样的道理，在头文件中当使用智能指针对象作为类成员变量时，也应该优先使用前置声明去引用智能指针对象的包裹类，
// 而不是直接包含包裹类的头文件。
/*
//Test.h
#include <memory>

//智能指针包裹类A，这里优先使用A的前置声明，而不是直接包含A.h
class A;

class Test
{
public:
    Test();
    ~Test();

private:
    std::unique_ptr<A>  m_spA;
};
 */

void tips() {
    //tips_one();
    //tips_two();
    tips_three();
}

int main() {
    //funcUniquePtr();
    //funcSharedPtr();
    //funcWeakPtr();
    //funcPrtSize();
    tips();
}
