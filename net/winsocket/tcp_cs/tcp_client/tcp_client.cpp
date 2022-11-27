//
// Created by nanjingblue on 2022/10/31.
//]
#include <iostream>
#include <winsock2.h>
#include <cstdio>
#include <fstream>
#include "parser.h"
#include "response.h"


void _error(char *s) {
    printf("%s����������룺%d\n", s, WSAGetLastError());
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(0x0202, &wsaData) != 0) {
        _error("WSAStartup()");
        exit(-1);
    }

    SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        _error("socket()");
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
        _error("connect()");
        closesocket(sd);
        WSACleanup();
        exit(-1);
    }

    printf("�����ӳɹ���\n");

    char data[4096];
    char rbuf[4096];

    do {
        gets(data);

        std::vector<std::string> v = parser::getCommand(data);
        if (v.size() != 2) {
            std::cout << "ָ�����" << std::endl;
            continue;
        }
        parser::Token token = v[0];
        auto it = parser::tokenCase.find(token);
        if (it == parser::tokenCase.end()) {
            std::cout << "�޷�ʶ������" << data << std::endl;
            continue;
        }

        int cmd = it->second;
        std::string rec(data);
        std::string body = rec.substr(token.length() + 1);

        switch (cmd) {
            case 0: {
                send(sd, data, strlen(data), 0);

                DirResult dirResult;
                recv(sd, (char*)&dirResult, sizeof(dirResult), 0);
                std::cout << dirResult.fileList << std::endl;

                break;
            }
            case 1: {
                send(sd, data, strlen(data), 0);

                std::string response = parser::handleDownLoadCMD(v[1]);
                std::cout << response << std::endl;
                Upload upload(200, response.c_str());
                send(sd, (char*)&upload, sizeof(Upload), 0);
                std::cout << "�ϴ��ɹ�" << std::endl;
                break;
            }
            case 2: {
                send(sd, data, strlen(data), 0);

                std::string filepath = v[1];
                std::string filename;

                for (auto i = filepath.length() - 1; i >= 0; i--) {
                    if (filepath[i] == '/' || filepath[i] == '\\' || filepath[i] == ' ') {
                        break;
                    }
                    filename = filepath[i] + filename;
                }
//                std::cout << "·����" <<  filepath << std::endl;
                std::cout << "�ļ�����" <<  filename << std::endl;

                DownloadResult downloadResult;
                recv(sd, (char*)&downloadResult, sizeof(DownloadResult), 0);
                std::cout << "״̬�룺" << downloadResult.status << std::endl;
                if (downloadResult.status != 200) {
                    std::cout << "download failed." << std::endl;
                    break;
                }

                std::ofstream ofs;
                ofs.open("./data/" + filename,std::ios::out|std::ios::binary);
                if(!ofs.is_open()){
                    std::cout<<"cant open output file."<< std::endl;
                }
                ofs << downloadResult.file << std::endl;
                std::cout<<"out to file success.\n"<< std::endl;
                ofs.close();
                break;
            }
            default: {
                std::cout << "�޷�ʶ������" << token << std::endl;
                break;
            }
        }
    } while (strcmp(data, "quit") != 0);
    printf("�ټ���\n");
    closesocket(sd);
    WSACleanup();
}