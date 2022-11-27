//
// Created by nanjingblue on 2022/11/26.
//

#ifndef TCP_CS_RESPONSE_H
#define TCP_CS_RESPONSE_H

#include <cstring>

struct DataHeader
{
    int dataLength;
    int status;
};

struct Dir:public DataHeader
{
    Dir() : DataHeader() {
        dataLength = sizeof(Dir);
    }
    char filepath[1025]{};
};

struct DirResult:public DataHeader
{
    DirResult() : DataHeader(){}
    explicit DirResult(const char* data) : DataHeader() {
        dataLength = sizeof(DirResult);
        strcpy(fileList, data);
    }
    char fileList[4096]{};
};


struct Upload:public DataHeader {
    Upload() : DataHeader(){}
    explicit Upload(int s, const char* data) : DataHeader() {
        dataLength = sizeof(Upload);
        status = s;
        strcpy(file, data);
    }
    char file[4096]{};
};

struct DownloadResult:public DataHeader {
    DownloadResult() : DataHeader(){}
    explicit DownloadResult(int s, const char* data) : DataHeader() {
        dataLength = sizeof(DownloadResult);
        status = s;
        strcpy(file, data);
    }
    char file[4096]{};
};

#endif //TCP_CS_RESPONSE_H
