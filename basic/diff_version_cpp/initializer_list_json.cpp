//
// Created by fufeng on 2023/12/6.
//

#include <iostream>
#include <string>
#include <initializer_list>
#include <vector>
#include <array>

template<typename T, size_t N>
using GenericArray = std::array<T, N>;

//某C++ json库支持如下语法创建一个json对象：
// a way to express an _array_ of key/value pairs
// [["currency", "USD"], ["value", 42.99]]
//json array_not_object = json::array({ {"currency", "USD"}, {"value", 42.99} });

//那么这个json::array()方法是如何实现的呢？
// 这利用std::initializer_list<T>也很容易实现，首先花括号中有两个元素{"currency", "USD"}和{"value", 42.99}，且这两个元素的值不一样，
// 前者是两个字符串类型，后者是一个字符串和一个浮点型，因此我们可以创建两个构造函数分别支持这两种类型的构造函数，构造的对象类型为jsonNode，
// 然后创建一个类型为json的对象，实现其array()方法，该方法接收一个参数，参数类型为std::initializer_list<jsonNode>，完整的代码如下所示：

//简单地模拟json支持的几种数据类型
enum class jsonType {
    jsonTypeNull,
    jsonTypeInt,
    jsonTypeLong,
    jsonTypeDouble,
    jsonTypeBool,
    jsonTypeString,
    jsonTypeArray,
    jsonTypeObject
};

struct jsonNode {
    jsonNode(const char *key, const char *value) :
            m_type(jsonType::jsonTypeString),
            m_key(key),
            m_value(value) {
        std::cout << "jsonNode jsonTypeString called." << std::endl;
    }

    jsonNode(const char *key, double value) :
            m_type(jsonType::jsonTypeDouble),
            m_key(key),
            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeDouble called." << std::endl;
    }

    jsonNode(const char *key) :
            m_type(jsonType::jsonTypeNull),
            m_key(key),
            m_value() {
        std::cout << "jsonNode jsonTypeNull called." << std::endl;
    }

    jsonNode(const char *key, int value) :
            m_type(jsonType::jsonTypeInt),
            m_key(key),
            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeInt called." << std::endl;
    }

    jsonNode(const char *key, long value) :
            m_type(jsonType::jsonTypeLong),
            m_key(key),
            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeLong called." << std::endl;
    }

    jsonNode(const char *key, bool value) :
            m_type(jsonType::jsonTypeBool),
            m_key(key),
            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeBool called." << std::endl;
    }

    /*jsonNode(const char *key, GenericArray value) :
            m_type(jsonType::jsonTypeArray),
            m_key(key),
            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeArray called." << std::endl;
    }

    jsonNode(const char *key, GenericObject value) :
            m_type(jsonType::jsonTypeObject),
            m_key(key),

            m_value(std::to_string(value)) {
        std::cout << "jsonNode jsonTypeObject called." << std::endl;
    }*/

    jsonType m_type;
    std::string m_key;
    //始终使用string类型保存值是避免浮点类型因为精度问题而显示不同的结果
    std::string m_value;
};

class json {
public:
    static json &array(std::initializer_list<jsonNode> nodes) {
        m_json.m_nodes.clear();
        m_json.m_nodes.insert(m_json.m_nodes.end(), nodes.begin(), nodes.end());
        std::cout << "json::array() called." << std::endl;
        return m_json;
    }

    json() {

    }

    ~json() {


    }

    std::string toString() {
        size_t size = m_nodes.size();
        for (size_t i = 0; i < size; ++i) {
            switch (m_nodes[i].m_type) {
                //根据类型，组装成一个json字符串，代码省略...
                case jsonType::jsonTypeDouble:
                    break;
                case jsonType::jsonTypeString:
                    break;
                default:
                    break;
            }
        }
        return std::string("");
    }

private:
    std::vector<jsonNode> m_nodes;
    static json m_json;
};

json json::m_json;

int main() {
    json array_not_object = json::array({{"currency", "USD"},
                                         {"value",    42.99}});

    return 0;
}