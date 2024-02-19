//
// Created by fufeng on 2024/2/19.
//
#include <boost/lexical_cast.hpp>
#include <iostream>
using namespace std;
using boost::lexical_cast;

int main()
{
    int i = lexical_cast<int>("123");
    cout << i << endl;

    string s = lexical_cast<string>(1.23);
    cout << s << endl;

    int ii;
    try
    {
        ii = lexical_cast<int>("abcd");
    }
    catch (boost::bad_lexical_cast& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

