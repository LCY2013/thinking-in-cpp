//
// Created by fufeng on 2024/1/21.
//

#include "String.h"

int main()
{
    String s1("Hello");                          // 构造函数
    cout << s1 << endl;
    //String s2 = s1;                             // 赋值构造
    String s2(s1);                                  // 拷贝构造
    cout << s2 << endl;
    String s2A(std::move(s1));              // move构造
    cout << s2A << endl;
    String s3;                                        // 默认构造
    cout << s3 << endl;
    s3 = s2;                                           // 赋值构造
    cout << s3 << endl;
    String s3A;                                      // 默认构造
    s3A = std::move(s2A);                    // 赋值move构造
    cout << s3A << endl;

    return 0;
}


