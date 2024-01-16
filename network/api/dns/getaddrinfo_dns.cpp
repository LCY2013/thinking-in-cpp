//
// Created by fufeng on 2024/1/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    const char *hostname = "www.baidu.com";
    const char *port = "80";

    struct addrinfo hints, *result, *rp;

    // 初始化 hints 结构体
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // 获取地址信息
    int status = getaddrinfo(hostname, port, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // 遍历获取的地址信息
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        char addrstr[INET6_ADDRSTRLEN];
        void *addr;

        if (rp->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // 将地址转换为字符串形式
        inet_ntop(rp->ai_family, addr, addrstr, sizeof(addrstr));

        // 输出地址信息
        printf("Family: %d, Address: %s\n", rp->ai_family, addrstr);
    }

    // 释放地址信息
    freeaddrinfo(result);

    return 0;
}

