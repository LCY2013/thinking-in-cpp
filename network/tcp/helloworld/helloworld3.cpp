//
// Created by fufeng on 2023/12/6.
//

#include "helloworld3.h"
#include "common.h"

int main(int argc, char *argv[]) {
    // 1、创建IPV4的socket套接字， socket fd
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket error!");
        return -1;
    }

    // 2、允许本地的地址和端口快速重用
    int b_reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &b_reuse, sizeof(b_reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    do {
        struct sockaddr_in sin;
        //2. 绑定
        //2.1 填充struct sockaddr_in结构体变量
        bzero(&sin, sizeof(sin));
        //接收从本机任意IP地址过来的发给TCP，SERV_PORT的数据
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        //该函数将sin结构体中的sin_family成员赋值为AF_INET，AF_INET是地址族标识，表示使用IPv4地址族。
        sin.sin_family = AF_INET;
        //网络字节序的端口号
        sin.sin_port = htons(HW_PORT);

        //2.2绑定
        if (bind(fd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            perror("bind");
            break;
        }

        /*3. 调用listen()把主动套接字变成被动套接字 */
        if (listen(fd, BACKLOG) < 0) {
            perror("listen");
            break;
        }

        printf("Server starting....OK!\n");

        char msg[] = "HelloWorld";
        struct cli_info *pcinfo = NULL;
        struct sockaddr_in cin;
        socklen_t clen = sizeof(cin);
        int client_fd;
        while (true) {
            //等待客户请求到来，如果有客户端连接，则接受连接
            client_fd = accept(fd, (struct sockaddr *) &cin, &clen);;
            if (client_fd < 0)
                break;

            //给客户端发送”HelloWorld“消息
            send(client_fd, msg, strlen(msg), 0);
            close(client_fd);
        }// end while-loop
    } while (0); //end outer-while-loop

    if (fd > 0) {
        close(fd);
    }

    return 0;
}

