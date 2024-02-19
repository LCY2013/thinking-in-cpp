//
// Created by fufeng on 2024/2/19.
//
#include <algorithm>
#include <iostream>

using namespace std;

//输入一组字符，输出它的全排列
//排列数量: "123"   3*2*1 = 3!
//排列结果:  123
//          132
//          213
//          231
//          321
//          312
//f(123) = 1+f(23), f(23) = 2+f(3), f(3)  = 3
void swap(char &a, char &b) {
    char tmp = a;
    a = b;
    b = tmp;
}

void swapPtr(char *a, char *b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void Permutation(char *pStr, char *pPosition) {
    if (*pPosition == '\0') {
        cout << pStr << endl;
    } else {
        for (char *pChar = pPosition; *pChar != '\0'; pChar++) {
            swap(*pPosition, *pChar);
            //swapPtr(pPosition, pChar);
            Permutation(pStr, pPosition + 1);
            swap(*pPosition, *pChar);
            //swapPtr(pPosition, pChar);
        }
    }
}

int main() {
    char test[] = "132";
    Permutation(test, test);
    cout << endl;

    do{
        cout << test[0] << test[1] << test[2] << endl;
    } while (next_permutation(test, test+3));
    cout << endl;

    // 顺序只能从小到大排列
    char test2[] = "321";
    do{
        cout << test2[0] << test2[1] << test2[2] << endl;
    } while (next_permutation(test2, test2+3));
    cout << endl;

    char test3[] = "123";
    do{
        cout << test3[0] << test3[1] << test3[2] << endl;
    } while (next_permutation(test3, test3+3));

    // 结论：next_permutation 只能从小到大排列
    return 0;
}
