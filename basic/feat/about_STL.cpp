//
// Created by fufeng on 2023/12/10.
//
#include <iostream>
#include <vector>

using namespace std;

void vector_test() {
    // 创建动态数组
    vector<int> vec = {1, 2, 3, 4};
    cout << "size is " << vec.size() << endl;
    cout << "capacity is " << vec.capacity() << endl;
    // 遍历
    for (int index = 0; index < vec.size(); index++) {
        cout << vec[index] << endl;
    }

    // 尾部插入数据
    vec.push_back(5);

    cout << "size is " << vec.size() << endl;
    cout << "capacity is " << vec.capacity() << endl;
    // 遍历
    for (int index = 0; index < vec.size(); index++) {
        cout << vec[index] << endl;
    }

    // 中间插入
    //vec.insert(vec.begin() + 2, 6);
    vec.insert(--vec.end(), 6);
    cout << "size is " << vec.size() << endl;
    cout << "capacity is " << vec.capacity() << endl;
    // 遍历
    for (int index: vec) {
        cout << index << endl;
    }

    // 删除操作
    vec.pop_back();
    cout << "size is " << vec.size() << endl;
    cout << "capacity is " << vec.capacity() << endl;
    // 遍历
    for (int index: vec) {
        cout << index << endl;
    }

    // 因为数组是左开右闭，所以删除最后一个元素需要使用end() - 1
    vec.erase(vec.end() - 1);
    cout << "size is " << vec.size() << endl;
    cout << "capacity is " << vec.capacity() << endl;
    // 遍历
    for (int index: vec) {
        cout << index << endl;
    }
}

int main() {
    // vector test
    vector_test();

    return 0;
}
