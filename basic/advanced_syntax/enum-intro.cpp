//
// Created by fufeng on 2023/12/30.
//

#include <iostream>
using namespace std;
int main()
{
    enum wT{Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
    wT weekday;
    weekday = Monday;
    weekday = Tuesday;
    //weekday = 1;
    weekday = wT(1);  //不建议使用
    cout << weekday << endl;
    //Monday = 0;
    int a = Wednesday;
    cout << a << endl;

    return 0;
}
