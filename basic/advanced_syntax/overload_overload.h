//
// Created by fufeng on 2023/12/30.
//

#ifndef THINKING_IN_CPP_OVERLOAD_OVERLOAD_H
#define THINKING_IN_CPP_OVERLOAD_OVERLOAD_H


class overload_overload {

};

#include <stdio.h>



int test(int a);
int test(double a);
int test(int a=1, double b=2.0);

namespace quickzhao
{
    int test(int a);
}

#endif //THINKING_IN_CPP_OVERLOAD_OVERLOAD_H
