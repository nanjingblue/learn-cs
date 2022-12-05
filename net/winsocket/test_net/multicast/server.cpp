//
// Created by nanjingblue on 2022/12/5.
//

#include <winsock2.h>
#include <cstdio>
#include <ctime>

#define BUFLEN 255

void get_cur_time(char *curtime) {
    time_t tm;
    time(&tm);
    sprintf(curtime, "%s\n", ctime(&tm));
}

int main(int argc, char **argv) {

    struct sockaddr_in peeraddr = {};
    int sockfd, on = 1;

    if (argc != 3) {
        printf("Usage: %s <ip address> <port>\n", argv[0]);
        exit(0);
    }

    //初始化库
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        printf("WSAStartup error!\n");
        return 0;
    }

    //创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "socket creating error \n");
        exit(1);
    }

    //设置广播地址
    memset(&peeraddr, 0, sizeof(peeraddr));
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_addr.s_addr = inet_addr(argv[1]);
    peeraddr.sin_port = htons(atoi(argv[2]));

    //允许发广播包
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *) &on, sizeof(on));

    char msg[BUFLEN + 1];
    while (true) {
        get_cur_time(msg);
        printf("发送时间:%s \n", msg);
        sendto(sockfd, msg, strlen(msg), 0, (sockaddr *)&peeraddr, sizeof(struct sockaddr_in));
        Sleep(3000);/* sleep 3 seconds between send */
    }

    closesocket(sockfd);
    WSACleanup();
}  /* main end */
