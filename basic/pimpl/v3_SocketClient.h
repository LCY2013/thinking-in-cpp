//
// Created by fufeng on 2023/12/6.
//

#ifndef THINKING_IN_CPP_V3_SOCKETCLIENT_H
#define THINKING_IN_CPP_V3_SOCKETCLIENT_H

//for std::unique_ptr
#include <memory>
#include <iostream>

class CSocketClient {
public:
    CSocketClient();

    ~CSocketClient();

private:
    struct Impl;
    std::unique_ptr<Impl> m_pImpl;
};


#endif //THINKING_IN_CPP_V3_SOCKETCLIENT_H
