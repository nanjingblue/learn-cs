
//
// Created by nanjingblue on 2022/12/5.
//
#include "winsock2.h"
#include <cstdio>

void _ERROR(char *s) {
    printf("%s��������Ϊ��%d \n", s, WSAGetLastError());
}

int count(char *s) {
    int cc = 0;
    for (int i = 0; s[i]; i++) {
        if (isalpha(s[i])) {
            cc++;
        }
    }//for(i)
    return cc;
}


int main() {
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        _ERROR("WSAStartup()");
        return 0;
    }

    SOCKET ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss == INVALID_SOCKET) {
        _ERROR("socket()");
        WSACleanup();
        return 0;
    }

    sockaddr_in sip;
    memset(&sip, 0, sizeof(sip));
    sip.sin_family = AF_INET;
    sip.sin_port = htons(9999);
    sip.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ss, (sockaddr *) &sip, sizeof(sip)) == SOCKET_ERROR) {
        _ERROR("bind()");
        closesocket(ss);
        WSACleanup();
        return 0;
    }

    if (listen(ss, 5) == SOCKET_ERROR) {
        _ERROR("listen()");
        closesocket(ss);
        WSACleanup();
        return 0;
    }

    printf("TCP��������9999�˿ں�������.....\n");

    sockaddr_in cip;
    int len = sizeof(cip);
    SOCKET cs = accept(ss, (sockaddr *) &cip, &len);


    printf("�ͻ���IP��%s �˿ںţ�%d �����ӽ�����\n", inet_ntoa(cip.sin_addr), ntohs(cip.sin_port));

    char data[254];
    do {
        int n = recv(cs, data, 253, 0);
        if (n == 0) {
            goto end;
        }
        if (n == SOCKET_ERROR) {
            _ERROR("recv()");
            goto end;
        }
        data[n] = '\0';
        printf("�ͻ������͵������ǣ�%s\n", data);
        char rs[254];
        sprintf(rs, "��ĸ������%d", count(data));
        send(cs, rs, strlen(rs), 0);
    } while (strcmp(data, "quit") != 0);
    printf("�ټ���\n");
    end:
    closesocket(cs);
    closesocket(ss);
    WSACleanup();
    return 0;

}//main
