//
// Created by fufeng on 2023/12/26.
//

#include <iostream>
#include <assert.h>
using namespace std;

void swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
void swap2(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main()
{
    //int x = 1, x2 = 3;
    //int& rx = x;
    //rx = 2;
    //cout << x << endl;
    //cout << rx << endl;
    //rx = x2;
    //cout << x << endl;
    //cout << rx << endl;

    int a = 3, b = 4;
    swap(a, b);
    assert(a == 4 && b == 3);

    a = 3, b = 4;
    swap2(&a, &b);
    assert(a == 4 && b == 3);


    return 0;
}


