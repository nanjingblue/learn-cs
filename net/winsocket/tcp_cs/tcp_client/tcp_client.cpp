//
// Created by nanjingblue on 2022/10/31.
//]
#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include "parser.h"
#include "response.h"


void _error(char *s) {
    printf("%s出错！错误代码：%d\n", s, WSAGetLastError());
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        _error("WSAStartup()");
        exit(-1);
    }

    SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        _error("socket()");
        WSACleanup();
        exit(-1);
    }
    struct sockaddr_in saddr{};
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(9999);

    printf("向服务器：127.0.0.1 端口号：9999 发起连接...");
    if (connect(sd, (struct sockaddr *) &saddr, sizeof(saddr)) == SOCKET_ERROR) {
        _error("connect()");
        closesocket(sd);
        WSACleanup();
        exit(-1);
    }

    printf("已连接成功！\n");

    char data[4096];
    char rbuf[4096];

    do {
        gets(data);

        parser::Token token = parser::getCommand(data);
        int cmd = parser::tokenCase[token];
        std::string rec(data);
        std::string body = rec.substr(token.length() + 1);
        switch (cmd) {
            case 0: {
                send(sd, data, strlen(data), 0);

                DirResult dirResult;
                recv(sd, (char*)&dirResult, sizeof(dirResult), 0);
                std::cout << dirResult.fileList << std::endl;

                break;
            }
            case 1:
                send(sd, data, strlen(data), 0);
        }
    } while (strcmp(data, "quit") != 0);
    printf("再见！\n");
    closesocket(sd);
    WSACleanup();
}