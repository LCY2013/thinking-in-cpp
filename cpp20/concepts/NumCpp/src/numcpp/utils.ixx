//
// Created by fufeng on 2024/2/21.
//
export module numcpp:utils;

import :types;

namespace numcpp {
    using std::size_t;

    export size_t calcShapeSize(const Shape &shape) {
        if (shape.empty()) {
            return 0;
        }

        size_t size = 1;
        for (auto &dim : shape) {
            size *= dim;
        }

        return size;
    }
}
