//
// Created by fufeng on 2023/12/6.
//
// 智能指针用于pimpl惯用法
//
// C++11标准引入了智能指针对象，我们可以使用std::unique_ptr对象来管理上述用于隐藏具体实现的m_pImpl指针。
//

#include "v3_SocketClient.h"

// C++11标准
CSocketClient::CSocketClient() {
    //C++11标准并未提供std::make_unique()，该方法是C++14提供的
    m_pImpl.reset(new Impl());
}

// C++14标准
CSocketClient::CSocketClient() : m_pImpl(std::make_unique<Impl>()) {

}

CSocketClient::~CSocketClient()
{
    //不再需要显式 delete 了
    //delete m_pImpl;
}
