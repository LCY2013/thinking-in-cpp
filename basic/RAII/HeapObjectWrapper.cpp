//
// Created by fufeng on 2023/12/6.
//
#include <iostream>

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
