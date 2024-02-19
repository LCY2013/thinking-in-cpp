//
// Created by fufeng on 2024/2/18.
//
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include <utility>
#include <iostream>

using namespace std;

struct Display {
    void operator()(int i) {
        cout << i << " ";
    }
};

struct DisplayPair {
    void operator()(const pair<string, double> &p) {
        cout << p.first << ": " << p.second << " ";
    }
};

struct cmpMap {
    bool operator()(pair<string, double> a, pair<string, double> b) {
        return a.first.length() < b.first.length();
    }
};

int main() {
    int iArr[] = {1, 2, 3, 4, 5};
    vector<int> iVector(iArr, iArr + 4);
    list<int> iList(iArr, iArr + 4);
    deque<int> iDeque(iArr, iArr + 4);
    queue<int> iQueue(iDeque);
    stack<int> iStack(iDeque);
    priority_queue<int> iPriorityQueue(iArr, iArr + 4);

    for_each(iVector.begin(), iVector.end(), Display());
    cout << endl;
    for_each(iList.begin(), iList.end(), Display());
    cout << endl;
    for_each(iDeque.begin(), iDeque.end(), Display());
    cout << endl;

    while (!iQueue.empty()) {
        cout << iQueue.front() << " ";  // 1 2 3 4
        iQueue.pop();
    }

    cout << endl;

    while (!iStack.empty()) {
        cout << iStack.top() << " ";   // 4 3 2 1
        iStack.pop();
    }

    cout << endl;

    while (!iPriorityQueue.empty()) {
        cout << iPriorityQueue.top() << " ";  // 4 3 2 1
        iPriorityQueue.pop();
    }

    cout << endl;

    return 0;
}
