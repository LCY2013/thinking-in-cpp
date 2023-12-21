//
// Created by fufeng on 2023/12/21.
//

#include <atomic>
#include <stdio.h>

int main()
{
    std::atomic<int> value;
    // c++11、14不行，c++17 开始没问题
    std::atomic<int> value1 = 100;
    value = 99;
    printf("%d\n", (int)value);

    //自增1，原子操作
    value++;
    printf("%d\n", (int)value);

    return 0;
}
