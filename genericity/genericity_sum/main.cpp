//
// Created by fufeng on 2024/1/29.
//
#include <iostream>
using namespace std;
// 1+2+3...+100 ==> n*(n+1)/2

template<int n>
struct Sum
{
    enum Value {N = Sum<n-1>::N+n}; // Sum(n) = Sum(n-1)+n
};
template<>
struct Sum<1>
{
    enum Value {N = 1};    // n=1
};

int main()
{
    cout << Sum<100>::N << endl;

    return 0;
}

/*
main:
        push    rbp
        mov     rbp, rsp
        mov     esi, 5050
        mov     edi, OFFSET FLAT:_ZSt4cout
        call    std::basic_ostream<char, std::char_traits<char> >::operator<<(int)
        mov     esi, OFFSET FLAT:_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
        mov     rdi, rax
        call    std::basic_ostream<char, std::char_traits<char> >::operator<<(std::basic_ostream<char, std::char_traits<char> >& (*)(std::basic_ostream<char, std::char_traits<char> >&))
        mov     eax, 0
        pop     rbp
        ret
 */

