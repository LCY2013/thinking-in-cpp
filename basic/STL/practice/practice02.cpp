//
// Created by fufeng on 2024/2/18.
//
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

struct DisplayPair {
    void operator()(pair<string, double> info)
    {
        cout << info.first << ": " << info.second << endl;
    }
};

int main() {
    map<string, double> studentSocres;
    studentSocres["LiMing"] = 95.0;
    studentSocres["LiHong"] = 98.5;
    studentSocres.insert(pair<string, double>("LiXue", 99.5));
    studentSocres.insert(pair<string, double>("Lisi", 98.6));
    studentSocres.insert(pair<string, double>("wangwu", 94.5));
    studentSocres.insert(pair<string, double>("wangwu", 95.5));  // 如果存在一样的key，这种方式就不会替换，只能用=赋值
    studentSocres.insert(map<string, double>::value_type("zhaoliu", 95.5) );
    studentSocres["wangwu"] = 88.5;
    for_each(studentSocres.begin(), studentSocres.end(), DisplayPair());
    cout << endl;

    map<string, double>::iterator iter;
    iter = studentSocres.find("zhaoliu");
    if (iter != studentSocres.end())
    {
        cout << "Found the score is: " << iter->second << endl << endl;
    }
    else
    {
        cout << "Didn't find the key." << endl << endl;
    }

    // 迭代器
    iter = studentSocres.begin();
    while (iter != studentSocres.end())
    {
        if (iter->second < 98.0)  // 小于98的移除
        {
            studentSocres.erase(iter++);  // 移除后的迭代器要加1
        }
        else
        {
            iter++;
        }
    }
    for_each(studentSocres.begin(), studentSocres.end(), DisplayPair());
    cout << endl;


    for (iter = studentSocres.begin(); iter != studentSocres.end(); iter++)
    {
        if (iter->second <= 98.5)
        {
            iter = studentSocres.erase(iter);  //使用新的迭代器
        }
    }
    for_each(studentSocres.begin(), studentSocres.end(), DisplayPair());
    cout << endl;

    // find 指定对应查找，不通过遍历
    //iter = studentSocres.find("LiHong");
    //studentSocres.erase(iter);
    //for_each(studentSocres.begin(), studentSocres.end(), Display());

    //int n = studentSocres.erase("LiHong1");
    //cout << n << endl;
    //for_each(studentSocres.begin(), studentSocres.end(), Display());

    studentSocres.erase(studentSocres.begin(), studentSocres.end());
    for_each(studentSocres.begin(), studentSocres.end(), DisplayPair());
    cout << endl;

    return 0;
}
