//
// Created by nanjingblue on 2022/11/26.
//

#ifndef TCP_CS_GLOBAL_H
#define TCP_CS_GLOBAL_H

#include "winsock2.h"

namespace global {
    static SOCKET clisd;
    static SOCKET lsd;

    struct sockaddr_in server{};
    struct sockaddr_in caddr{};
}

#endif //TCP_CS_GLOBAL_H
