//
// Created by nanjingblue on 2022/12/5.
//
#include "winsock2.h"
#include <cstdio>

int count(char *s) {
    int cc = 0;
    for (int i = 0; s[i]; i++) {
        if (isalpha(s[i])) {
            cc++;
        }
    }//for(i)
    return cc;
}

int main(void) {
    WSADATA wsaData;
    WSAStartup(0x0202, &wsaData);
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in sip;
    memset(&sip, 0, sizeof(sip));
    sip.sin_family = AF_INET;
    sip.sin_port = htons(9999);
    sip.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(s, (sockaddr *) &sip, sizeof(sip));
    printf("UDP��������9999�Ͻ���UDP��....\n");
    int stopIt = 0;
    while (!stopIt) {
        char rd[254];
        int len = sizeof(sip);
        int n = recvfrom(s, rd, 253, 0, (sockaddr *) &sip, &len);
        rd[n] = '\0';
        printf("�ͻ���IP��%s �˿ںţ�%d ����UDP���������ǣ�%s\n",
               inet_ntoa(sip.sin_addr), ntohs(sip.sin_port), rd
        );
        char sd[254] = "";
        sprintf(sd, "��ĸ������%d", count(rd));
        sendto(s, sd, strlen(sd), 0, (sockaddr *) &sip, sizeof(sip));
    }
    closesocket(s);
    WSACleanup();
}
