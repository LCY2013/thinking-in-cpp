//
// Created by fufeng on 2023/12/30.
//

#include "overload_overload.h"

int test(int a)
{
    return a;
}
namespace quickzhao
{
    int test(int a)
    {
        return a+1;
    }
}
int test(double a)
{
    return int(a);
}
int test(int a, double b)
{
    return a + b;
}

//?test@@YAHH@Z
//?test@@YAHN@Z
//?test@@YAHHN@Z
int main()
{
    int(*p)(int);
    //p = test;
    p = quickzhao::test;
    int result = (*p)(1);

    result = quickzhao::test(1);

    result = test(2.0);
    result = test(1, 2.0);
    return 0;
}

