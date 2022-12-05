//
// Created by nanjingblue on 2022/12/5.
//
//接受时间服务器每3秒的时间广播包

#include <winsock2.h>
#include <cstdio>

int main() {

    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in sip = {};
    memset(&sip, 0, sizeof(sip));


    sip.sin_family = AF_INET;
    sip.sin_port = htons(9000);
    sip.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (sockaddr *) &sip, sizeof(sip)) == SOCKET_ERROR) {
        printf("bind() error\n");
        closesocket(s);
        WSACleanup();
    }

    int cc = 10;
    do {
        char rd[254];
        int n = recvfrom(s, rd, 253, 0, NULL, NULL);
        rd[n] = 0;
        printf("接受到的时间广播包内容是：%s\n", rd);
        cc--;
    } while (cc > 0);

    printf("再见！");
    closesocket(s);
    WSACleanup();
}
