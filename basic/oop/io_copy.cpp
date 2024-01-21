//
// Created by fufeng on 2024/1/21.
//
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

static const int bufferLen = 2048;
bool CopyFile(const string& src, const string& dst)
{
    ifstream in(src.c_str(), ios::in | ios::binary);
    ofstream out(dst.c_str(), ios::out | ios::binary | ios::trunc);

    if (!in || !out)
    {
        return false;
    }

    char temp[bufferLen];
    while (!in.eof())
    {
        in.read(temp, bufferLen);
        streamsize count = in.gcount();
        out.write(temp, count);
    }

    in.close();
    out.close();

    return true;
}

int main()
{
    cout << CopyFile("./file/Blue Daube.mp3", "./file/Blue Daube2.mp3") << endl;

    //int a;
    //int index = 0;
    //fstream fout;
    //fout.open("testBuffer.txt", ios::app);
    ////if (fout.fail())
    //if (!fout)
    //{
    //	cout << "open file failed" << endl;
    //}
    //while (cin >> a)
    //{
    //	//cout << "The numbers are: " << a << endl;
    //	fout << "The numbers are: " << a << endl;
    //	index++;
    //	if (index == 5)
    //	{
    //		break;
    //	}
    //}
    //cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    //char ch;
    //cin >> ch;
    ////cout << "the last char is: " << ch << endl;
    //fout << "the last char is: " << ch << endl;
    //fout.close();



    return 0;

}



