//
// Created by nanjingblue on 2022/12/5.
//
#include "winsock2.h"
#include <cstdio>

#define BUFLEN 255

int main(int argc, char **argv) {
    struct sockaddr_in localaddr{};
    int sockfd, n;
    char msg[BUFLEN + 1];
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(-1);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "socket creating error in tcli.c\n");
        exit(1);
    }
    memset(&localaddr, 0, sizeof(struct sockaddr_in));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(atoi(argv[1]));
    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr*)&localaddr, sizeof(struct sockaddr_in)) < 0) {
        fprintf(stderr, "bind error in tcli.c\n");
        exit(2);
    }

    int cc = 10;
    do {
        char rd[254];
        int n = recvfrom(sockfd, rd, 253, 0, nullptr, nullptr);
        rd[n] = 0;
        printf("接受到的内容是：%s\n", rd);
        cc--;
    } while (cc > 0);

    printf("再见");
    closesocket(sockfd);
    WSACleanup();
}
