//
// Created by fufeng on 2024/1/1.
//

#include "Complex.h"
#include <iostream>
using namespace std;

int main() {
    Complex c;
    cout << c.GetReal() << " " << c.GetImage() << endl;
    c.SetReal(1.0);
    c.SetImage(2.0);

    Complex a(1.0, 2.0);
    cout << a.GetReal() << " " << a.GetImage() << endl;
    a.SetReal(3.0);
    a.SetImage(4.0);
    cout << a.GetReal() << " " << a.GetImage() << endl;

    Complex b = a + c;
    cout << b.GetReal() << " " << b.GetImage() << endl;

    Complex d;
    d = b+c;
    cout << b.GetReal() << " " << b.GetImage() << endl;

    Complex e(d);
    cout << e.GetReal() << " " << e.GetImage() << endl;

    e++;

    ++d;
    return 0;
}
