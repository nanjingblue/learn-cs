//
// Created by nanjingblue on 2022/10/31.
//
#include <winsock2.h>
#include <cstdio>


void _ERROR(char *s) {
    printf("%s����������룺%d\n", s, WSAGetLastError());
}

int main(void) {
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        _ERROR("WSAStartup()");
        exit(-1);
    }

    SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        _ERROR("socket()");
        WSACleanup();
        exit(-1);
    }
    struct sockaddr_in saddr{};
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(9999);

    printf("���������127.0.0.1 �˿ںţ�9999 ��������...");
    if (connect(sd, (struct sockaddr *) &saddr, sizeof(saddr)) == SOCKET_ERROR) {
        _ERROR("connect()");
        closesocket(sd);
        WSACleanup();
        exit(-1);
    }

    printf("�����ӳɹ���\n");

    char data[254];
    char rbuf[254];
    do {
        gets(data);
        send(sd, data, strlen(data), 0);

        int rc = recv(sd, rbuf, 254, 0);
        rbuf[rc] = 0;
        printf("��������Ӧ��%s\n", rbuf);
    } while (strcmp(data, "quit") != 0);
    printf("�ټ���\n");
    closesocket(sd);
    WSACleanup();
}
