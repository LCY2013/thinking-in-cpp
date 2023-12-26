//
// Created by fufeng on 2023/12/26.
//

#include <iostream>
#include <memory>
using namespace std;
int main()
{
    //// shared_ptr
    //{
    //	//shared_ptr  shared_ptr
    //	auto wA = shared_ptr<int>(new int(20));
    //	{
    //		auto wA2 = wA;
    //		cout << ((wA2.get() != nullptr) ? (*wA2.get()) : -1) << endl;       // 20
    //		cout << ((wA.get() != nullptr) ? (*wA.get()) : -1) << endl;           // 20
    //		cout << wA2.use_count() << endl;                                              // 2
    //		cout << wA.use_count() << endl;                                                // 2
    //	}
    //	//cout << wA2.use_count() << endl;
    //	cout << wA.use_count() << endl;                                                    // 1
    //	cout << ((wA.get() != nullptr) ? (*wA.get()) : -1) << endl;               // 20
    //	//shared_ptr  shared_ptr
    //	//shared_ptr  - 1
    //}

    // move
    auto wAA = std::make_shared<int>(30);
    auto wAA2 = std::move(wAA); // 1
    cout << ((wAA.get() != nullptr) ? (*wAA.get()) : -1) << endl;          // -1
    cout << ((wAA2.get() != nullptr) ? (*wAA2.get()) : -1) << endl;      // 30
    cout << wAA.use_count() << endl;                                                  // 0
    cout << wAA2.use_count() << endl;                                                // 1

    return 0;
}


