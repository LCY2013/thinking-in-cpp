//
// Created by fufeng on 2024/2/19.
//
#include <vector>
#include <cstdint>

class TestClass {
public:
    //TestClass()= default;

    //~TestClass() = default;

    TestClass(const TestClass&) = delete;

    int32_t getValue() const {
        return 0;
    }
};

void c10() {
    std::vector<TestClass> v(10);
    v[0] = v[1];
}

int main() {
    c10();

    return 0;
}
