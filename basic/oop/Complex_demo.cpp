//
// Created by fufeng on 2024/1/1.
//

#include "Complex.h"
#include <iostream>
using namespace std;

int main() {
    Complex c;
    cout << c.GetReal() << " " << c.GetImag() << endl;
    c.SetReal(1.0);
    c.SetImag(2.0);

    Complex a(1.0, 2.0);
    cout << a.GetReal() << " " << a.GetImag() << endl;
    a.SetReal(3.0);
    a.SetImag(4.0);
    cout << a.GetReal() << " " << a.GetImag() << endl;

    Complex b = a + c;
    cout << b.GetReal() << " " << b.GetImag() << endl;

    Complex d;
    d = b+c;
    cout << b.GetReal() << " " << b.GetImag() << endl;
    return 0;
}
