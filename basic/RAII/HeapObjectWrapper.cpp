//
// Created by fufeng on 2023/12/6.
//
#include <iostream>
using namespace std;

// 指定c++语言规范：g++ -g -o heap HeapObjectWrapper.cpp -std=c++11
// Makefile文件编译项目时可以这样指定：make CXXFLAGS="-g -O0 -std=c++11"
// 使用cmake编译项目时可以在CMakeLists.txt文件中添加如下行：
// set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -O0 -Wno-unused-variable")
// -std 选项的值可以按需设置成 c++14 或 c++17 或 c++20。

class HeapObjectWrapper {
private:
    char *m_p;
public:
    HeapObjectWrapper(int size) {
        m_p = new char(size);
        std::cout << "init" << std::endl;
    }

    ~HeapObjectWrapper() {
        delete[]m_p;
        m_p = NULL;
        std::cout << "destroy" << std::endl;
    }

    int GetHeapObjectSize() {
        return sizeof(m_p);
    }
};

int main(int argc, char *argv[]) {
    HeapObjectWrapper heapObjectWrapper(1024);
    if (heapObjectWrapper.GetHeapObjectSize() > 0)
        return 0;
    if (heapObjectWrapper.GetHeapObjectSize() > 0)
        return 0;
    if (heapObjectWrapper.GetHeapObjectSize() > 0)
        return 0;
    return 0;
}
