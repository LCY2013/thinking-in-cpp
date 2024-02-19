//
// Created by fufeng on 2024/2/19.
//

#include <list>
#include <iostream>
using namespace std;

int main()
{
    list<int> v;
    v.push_back(3);
    v.push_back(4);
    v.push_front(2);
    v.push_front(1);  // 1, 2, 3, 4

    list<int>::const_iterator it;
    for (it = v.begin(); it != v.end(); it++)
    {
        //*it += 1;
        cout << *it << " ";
    }
    cout << endl;

    //遍历
    //for (it = v.begin(); it < v.end(); it++)
    //{
    //	cout << *it << " ";
    //}
    cout <<v.front() << endl;
    v.pop_front();  // 从前面弹出一个元素

    //逆序遍历
    list<int>::reverse_iterator it2;
    for (it2 = v.rbegin(); it2 != v.rend(); it2++)
    {
        *it2 += 1;
        cout << *it2 << " ";                          // 5 4 3
    }
    cout << endl;



    return 0;
}