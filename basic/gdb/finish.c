//
// Created by fufeng on 2023/12/20.
//

#include <stdio.h>

int func() {
    int a = 9;
    printf("a=%d.\n", a);

    int b = 8;
    printf("b=%d.\n", b);
    return a + b;
}

int main() {
    int c = func();
    printf("c=%d.\n", c);

    return 0;
}
