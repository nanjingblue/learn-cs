//
// Created by nanjingblue on 2022/10/31.
//
#include "winsock2.h"
#include <cstdio>
#include "parser.h"
#include "global.h"
#include "response.h"

void init();
void _error(const std::string& msg);
int count(char *s);

void start(SOCKET& clisd, SOCKET& lsd, sockaddr_in& server, sockaddr_in& caddr) {
    char data[1024];
    int cc;

    do {
        int rc = recv(clisd, data, sizeof(data), 0);
        if (rc == 0) {
            printf("客户端已退出，任务结束\n");
            break;
        }
        if (rc == SOCKET_ERROR) {
            _error("recv()");
        }
        data[rc] = 0;
        printf("客户机数据：%s \n", data);

        std::vector<std::string> v = parser::getCommand(data);
        parser::Token token = v[0];
        int cmd = parser::tokenCase[token];
        switch (cmd) {
            case 0: {
                std::string response = parser::handleDirCMD(v[1]);
                std::cout << "response：" << response << std::endl;
                DirResult dirResult(response.c_str());
                rc = send(global::clisd, (char*)&dirResult, sizeof(DirResult), 0);
                if (rc == SOCKET_ERROR) {
                    _error("send()");
                }
                break;
            }
            case 1: {

                std::string filepath = v[1];
                std::string filename;

                for (auto i = filepath.length() - 1; i >= 0; i--) {
                    if (filepath[i] == '/' || filepath[i] == '\\' || filepath[i] == ' ') {
                        break;
                    }
                    filename = filepath[i] + filename;
                }

                Upload upload;
                recv(clisd, (char*)&upload, sizeof(Upload), 0);
                std::cout << "状态码：" << upload.status << std::endl;
                if (upload.status != 200) {
                    std::cout << "upload failed." << std::endl;
                    break;
                }

                std::ofstream ofs;
                ofs.open("./data/" + filename, std::ios::out|std::ios::binary);
                if(!ofs.is_open()){
                    std::cout<<"cant open output file."<< std::endl;
                }
                ofs << upload.file << std::endl;
                std::cout<<"out to file success.\n"<< std::endl;
                ofs.close();
                std::cout << "上传成功" << std::endl;
            }
            case 2: {
                std::string response = parser::handleDownLoadCMD(v[1]);
                std::cout << response << std::endl;
                DownloadResult downloadResult(200, response.c_str());
                rc = send(clisd, (char*)&downloadResult, sizeof(DownloadResult), 0);
                if (rc == SOCKET_ERROR) {
                    _error("send()");
                }
                break;
            }
        }

//        char rdata[254];
//        sprintf(rdata, "字母个数：%d \n", cc);
//        rc = send(global::clisd, rdata, strlen(rdata), 0);
//        if (rc == SOCKET_ERROR) {
//            _error("send()");
//        }
    } while (strcmp(data, "quit") != 0);
    printf("谢谢，再见！");
    closesocket(clisd);
    closesocket(lsd);
    WSACleanup();
}

int main() {
    init();
    start(global::clisd, global::lsd, global::server, global::caddr);
}//main

void init() {
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        _error("WSAStartup()");
        exit(-1);
    }

    global::lsd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (global::lsd == INVALID_SOCKET) {
        _error("socket");
        WSACleanup();
        exit(-1);
    }

    //server IP:127.0.0.1  port : 9999
    memset(&global::server, 0, sizeof(global::server));
    global::server.sin_family = AF_INET;
    global::server.sin_port = htons(9999);
    global::server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(global::lsd, (struct sockaddr *) &global::server, sizeof(global::server)) == SOCKET_ERROR) {
        _error("bind()");
    }

    if (listen(global::lsd, 5) == SOCKET_ERROR) {
        _error("listen()");
        closesocket(global::lsd);
        WSACleanup();
        exit(-1);
    }

    printf("TCP服务器程序在端口号：9999上等待连接......\n");
    int len = sizeof(global::caddr);
    global::clisd = accept(global::lsd, (struct sockaddr *) &global::caddr, &len);
    if (global::clisd == INVALID_SOCKET) {
        _error("accept()");
        closesocket(global::lsd);
        WSACleanup();
        exit(-1);
    }
    printf("客户机IP：%s 端口号：%d 已连接进来！\n", inet_ntoa(global::caddr.sin_addr), ntohs(global::caddr.sin_port));
}

void _error(const std::string& msg) {
    std::cout << "%s 出错！"<< msg << ", 出错代码: "<<  WSAGetLastError() << std::endl;
    closesocket(global::clisd);
    closesocket(global::lsd);
    WSACleanup();
    exit(-1);
}

int count(char *s) {
    int cc = 0;
    for (int i = 0; s[i] != 0; i++) {
        if (isalpha(s[i])) {
            cc++;
        }
    }
    return cc;
}