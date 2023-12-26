//
// Created by fufeng on 2023/12/26.
//

#include <string>
#include <iostream>
#include <memory>
using namespace std;

struct B;
struct A {
    shared_ptr<B> pb;
    ~A()
    {
        cout << "~A()" << endl;
    }
};
struct B {
    shared_ptr<A> pa;
    ~B()
    {
        cout << "~B()" << endl;
    }
};

struct BW;
struct AW
{
    shared_ptr<BW> pb;
    ~AW()
    {
        cout << "~AW()" << endl;
    }
};
struct BW
{
    weak_ptr<AW> pa;
    ~BW()
    {
        cout << "~BW()" << endl;
    }
};

void Test()
{
    cout << "Test shared_ptr and shared_ptr:  " << endl;
    shared_ptr<A> tA(new A());                                               // 1
    shared_ptr<B> tB(new B());                                                // 1
    cout << tA.use_count() << endl;
    cout << tB.use_count() << endl;
    tA->pb = tB;
    tB->pa = tA;
    cout << tA.use_count() << endl;                                        // 2
    cout << tB.use_count() << endl;                                        // 2
}
void Test2()
{
    cout << "Test weak_ptr and shared_ptr:  " << endl;
    shared_ptr<AW> tA(new AW());
    shared_ptr<BW> tB(new BW());
    cout << tA.use_count() << endl;                                        // 1
    cout << tB.use_count() << endl;                                        // 1
    tA->pb = tB;
    tB->pa = tA;
    cout << tA.use_count() << endl;                                        // 1
    cout << tB.use_count() << endl;                                        // 2
}

int main()
{
    Test();
    Test2();


    return 0;
}
