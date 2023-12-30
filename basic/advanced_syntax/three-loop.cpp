//
// Created by fufeng on 2023/12/30.
//

#include <iostream>
using namespace std;
int main()
{   // TODO: 1+2+3+4...+100
    // while
    int sum = 0;
    int index = 1;
    while (index <= 100)
    {
        sum += index;
        index += 1;
    }
    //cout << sum << endl;

    // for
    //index = 1;
    sum = 0;
    for (index = 1; index <= 100; ++index)
    {
        sum += index;
    }
    //cout << sum << endl;

    // do-while
    sum = 0;
    index = 1;
    do
    {
        sum += index;
        index += 1;
    } while (index <= 100);
    //cout << sum << endl;

    return 0;
}