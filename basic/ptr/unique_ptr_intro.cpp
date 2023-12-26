//
// Created by fufeng on 2023/12/26.
//
#include <memory>
#include <iostream>
using namespace std;
int main()
{
    {
        auto i = unique_ptr<int>(new int(10));
        cout << *i << endl;
    }

    // unique_ptr
    auto w = std::make_unique<int>(10);
    cout << *(w.get()) << endl;                             // 10

    auto w2 = std::move(w); // w2 nullptr
    cout << ((w.get() != nullptr) ? (*w.get()) : -1) << endl;       // -1
    cout << ((w2.get() != nullptr) ? (*w2.get()) : -1) << endl;   // 10
    return 0;
}
