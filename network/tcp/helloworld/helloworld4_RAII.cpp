//
// Created by fufeng on 2023/12/6.
//

/*
RAII是英文Resource Acquisition Is Initialization的缩写，翻译成中文是“资源获取就是初始化”，这个翻译仍然令人费解。
 通俗地说，所谓RAII就是资源在你拿到时就已经初始化好了，一旦你不再需要这个资源，该资源可以自动释放。

对于C++语言来说，即资源在构造函数中初始化（可以在构造函数中调用单独的初始化函数），在析构函数中释放或清理。
 常见的情形就是函数调用中，创建C++对象时分配资源，当C++对象出了作用域时会自动被清理和释放（不管这个这个对象是如何出作用域的——中间某个步骤不满足提前return掉还是正常走完全部流程后 return）。

还是以上面网络通信的例子来说，程序初始化时我们需要分配两种资源：

资源一： 初始化好Windows socket网络库；
资源二： 创建一个用于侦听的socket。

 c++ helloworld4_RAII.cpp -o helloworld4_RAII
 */

#include "helloworld4_RAII.h"

class ServerSocket {
private:
    bool m_bInit;
    int m_ListenSocket;

public:
    ServerSocket() {
        m_bInit = false;
        m_ListenSocket = -1;
    }

    ~ServerSocket() {
        if (m_ListenSocket > 0) {
            close(m_ListenSocket);
        }
        if (m_bInit) {
            exit(-1);
        }
    }

    bool DoInit() {
        // 1、创建IPV4的socket套接字， socket fd
        m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_ListenSocket < 0) {
            perror("socket error!");
            return false;
        }
        // 2、允许本地的地址和端口快速重用
        int b_reuse = 1;
        if (::setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, (void *) &b_reuse, sizeof(b_reuse)) < 0) {
            perror("setsockopt");
            return false;
        }
        return true;
    }

    bool DoBind(const char *ip, short port = 6000) {
        struct sockaddr_in sin;
        //2. 绑定
        //2.1 填充struct sockaddr_in结构体变量
        bzero(&sin, sizeof(sin));
        //接收从本机任意IP地址过来的发给TCP，SERV_PORT的数据
        //sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_addr.s_addr = inet_addr(ip);
        //该函数将sin结构体中的sin_family成员赋值为AF_INET，AF_INET是地址族标识，表示使用IPv4地址族。
        sin.sin_family = AF_INET;
        //网络字节序的端口号
        sin.sin_port = htons(port);

        //2.2绑定
        if (::bind(m_ListenSocket, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("bind");
            return false;
        }
        return true;
    }

    bool DoListen(int backlog = 15) {
        /*3. 调用listen()把主动套接字变成被动套接字 */
        if (listen(m_ListenSocket, backlog) < 0) {
            perror("listen");
            return false;
        }
        return true;
    }

    bool DoAccept() {
        printf("Server starting....OK!\n");
        char msg[] = "HelloWorld";
        struct cli_info *pcinfo = NULL;
        struct sockaddr_in cin;
        socklen_t clen = sizeof(cin);
        int client_fd;
        while (true) {
            //等待客户请求到来，如果有客户端连接，则接受连接
            client_fd = accept(m_ListenSocket, (struct sockaddr *) &cin, &clen);;
            if (client_fd < 0)
                break;

            //给客户端发送”HelloWorld“消息
            send(client_fd, msg, strlen(msg), 0);
            close(client_fd);
        }// end while-loop

        return false;
    }
};

int main(int argc, char *argv[]) {
    ServerSocket serverSocket;

    if (!serverSocket.DoInit())
        return -1;

    if (!serverSocket.DoBind("0.0.0.0", 6000))
        return -1;

    if (!serverSocket.DoListen(15))
        return -1;

    if (!serverSocket.DoAccept())
        return -1;

    return 0;
}
