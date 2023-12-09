//
// Created by fufeng on 2023/12/9.
//
#include <iostream>
#include <string>

using namespace std;

//g++ -Wall -g diff_c_cpp.cpp -std=c++17
int main() {
    char c1 = 'yes';
//   char c2 = "yes";
    cout << c1 << endl;  //s
//   cout << c2 << endl;

    const char *slash1 = "/";  // 等价于 '/' '\0'
//   const char* slash2 = '/ ';
    cout << slash1 << endl;
//   cout << slash2 << endl;

    cout << "cpp string" << endl;
    string s1(1, 'yes');
    cout << s1 << endl;
    string s2(3, 'yes');
    cout << s2 << endl;
    string s3(1, 'y');
    cout << s3 << endl;
    string s4("/");
    cout << s4 << endl;
    string s5(1, '/');
    cout << s5 << endl;
    string s6("yes");
    cout << s6 << endl;

    return 0;
}
