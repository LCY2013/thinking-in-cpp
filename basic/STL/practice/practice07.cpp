//
// Created by fufeng fufeng on 2024/2/19.
//
#include <functional>
#include <stack>
#include <queue>
#include <iostream>
using namespace std;




int main()
{
    //stack<int> s;
    //queue<int> q;


    priority_queue<int> pq;  // 默认优先级队列从大到小
    priority_queue<int, vector<int>, less<int> > pq2; //  从大到小
    priority_queue<int, vector<int>, greater<int> > pq3; // 从小到大

    pq.push(2);
    pq.push(1);
    pq.push(3);
    pq.push(0);
    while (!pq.empty())
    {
        int top = pq.top();
        cout << " top is: " << top<< endl;
        pq.pop();
    }
    cout << endl;


    pq2.push(2);
    pq2.push(1);
    pq2.push(3);
    pq2.push(0);
    while (!pq2.empty())
    {
        int top = pq2.top();
        cout << " top is: " << top << endl;
        pq2.pop();
    }
    cout << endl;


    pq3.push(2);
    pq3.push(1);
    pq3.push(3);
    pq3.push(0);
    while (!pq3.empty())
    {
        int top = pq3.top();
        cout << " top is: " << top << endl;
        pq3.pop();
    }
    cout << endl;

    return 0;
}

