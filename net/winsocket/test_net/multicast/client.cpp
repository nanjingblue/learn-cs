
//
// Created by nanjingblue on 2022/12/5.
//
#include "winsock2.h"
#include <cstdio>
#include "ws2tcpip.h"

#define BUFLEN 255

int main(int argc, char **argv) {
    struct sockaddr_in localaddr = {}, fromaddr = {};
    struct ip_mreq mcaddr = {};

    SOCKET sockfd;
    int n, len;

    char msg[BUFLEN + 1];

    if (argc < 3) {
        printf("Usage: %s <multicast ip> <port> [<local ip>]\n", argv[0]);
        exit(0);
    }

    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        printf("WSAStartup() error!\n");
        return 0;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("socket creating error \n");
        exit(1);
    }//if

    memset(&localaddr, 0, sizeof(struct sockaddr_in));

    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(atoi(argv[2]));

    if (argc == 4) {
        if ((localaddr.sin_addr.s_addr = inet_addr(argv[3])) == INADDR_NONE) {
            printf("Wrong local IP address\n");
            exit(0);
        }
    } else {
        localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(sockfd, (sockaddr *) &localaddr, sizeof(struct sockaddr_in)) < 0) {
        printf("bind error \n");
        exit(2);
    }

    memcpy(&mcaddr.imr_interface, &localaddr.sin_addr, sizeof(struct in_addr));

    if ((mcaddr.imr_multiaddr.s_addr = inet_addr(argv[1])) == INADDR_NONE) {
        printf("Wrong multicast IP address\n");
        exit(0);
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mcaddr, sizeof(mcaddr)) == SOCKET_ERROR) {
        printf("setsockopt error, join multicast failed.code:%lu \n", GetLastError());
        exit(4);
    }

    /* get multicast msg */
    int cc = 10;
    do {
        len = sizeof(struct sockaddr_in);
        n = recvfrom(sockfd, msg, BUFLEN, 0, (sockaddr *) &fromaddr, &len);
        printf("Datagram from %s %d received\n", inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port));
        msg[n] = 0;
        printf("%s", msg);
        cc--;
    } while (cc > 0);

    /* leave multicast group */

    if (setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *) &mcaddr, sizeof(struct ip_mreq)) < 0) {
        printf("setsockopt error in mtcli.c, drop multicast failed\n");
        exit(4);
    }

    closesocket(sockfd);
    WSACleanup();
}/* main end */