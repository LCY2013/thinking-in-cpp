//
// Created by fufeng on 2023/12/17.
//

#include <iostream>
using namespace std;

int main() {
  int a = 7, b = -1;
  float c = 3.14f;
  int *d = &a;
  float *e = &c;
  cout << d << endl;
  cout << e << endl;
  cout << *d << endl;
  cout << *e << endl;

    return 0;
}
