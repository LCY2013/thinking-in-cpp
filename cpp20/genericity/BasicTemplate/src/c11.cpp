//
// Created by fufeng on 2024/2/19.
//

#include "c11.h"

#include <cstdint>
#include <vector>
#include <list>
#include <deque>
using namespace std;

template<>
void fillContainer<vector<int32_t>, float>(vector<int32_t>& collection, float value, size_t size) {
    cout << "Explicit (full) template specialization" << endl;
    for (size_t i=0; i != size; ++i) {
        collection.push_back(static_cast<int32_t>(value));
    }
}

template
        void fillContainer<vector<int32_t>, int32_t>(vector<int32_t>& collection, int32_t value, size_t size);

void c11() {
    vector<int32_t> vec;
    fillContainer<10>(vec, 0);
    fillContainer(vec, 0, 10);

    list<int32_t> lst;
    fillContainer<10, list<int32_t>>(lst, 1);

    deque<int32_t> deq;
    fillContainer<10, deque<int32_t>, int32_t>(deq, 2);
}
