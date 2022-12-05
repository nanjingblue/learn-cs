//
// Created by nanjingblue on 2022/12/5.
//
#include "winsock2.h"
#include <cstdio>

int main() {
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sip;
    memset(&sip, 0, sizeof(sip));
    sip.sin_family = AF_INET;
    sip.sin_port = htons(9999);
    sip.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(s, (sockaddr *) &sip, sizeof(sip));
    char data[254];
    do {
        gets(data);
        send(s, data, strlen(data), 0);
        char rd[254];
        int n = recv(s, rd, 253, 0);
        rd[n] = '\0';
        printf("TCP服务器的响应是：%s\n", rd);
    } while (strcmp(data, "quit") != 0);
    printf("再见！\n");
    closesocket(s);
    WSACleanup();
}//main