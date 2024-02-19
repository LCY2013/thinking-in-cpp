export module helloworld;

import <iostream>;
import <cstdint>;

void private_hello() {
    std::cout << "Hello, World!" << std::endl;
}

export void hello() {
    std::cout << "Hello world!" << std::endl;
}

namespace hname {
    export int32_t getNumber() {
        return 10;
    }
}