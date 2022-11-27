//
// Created by nanjingblue on 2022/11/1.
//

#ifndef TCP_CS_PARSER_H
#define TCP_CS_PARSER_H

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <io.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "response.h"

namespace parser {

    typedef std::string Token;

    Token DIR = "DIR";
    Token UPLOAD = "UPLOAD";
    Token DOWNLOAD = "DOWNLOAD";

    std::map<Token , int> tokenCase = {
            {DIR, 0},
            {UPLOAD, 1},
            {DOWNLOAD, 2}
    };

    void getFiles(const std::string& path, std::vector<std::string>& files) {
        //文件句柄
        intptr_t hFile = 0;
        //文件信息，声明一个存储文件信息的结构体
        struct _finddata_t fileinfo{};
        std::string p;  //字符串，存放路径
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
            do {
                if (!(fileinfo.attrib & _A_SUBDIR)) {
                    files.emplace_back(fileinfo.name);
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            _findclose(hFile);
        }
    }

    std::string handleDirCMD(std::string& body) {
        std::vector<std::string> fileList;
        getFiles(body, fileList);
        std::string response;
        for (const auto& it : fileList) {
            std::cout << it << std::endl;
            response = response + it + "\n";
        }
        return response;
    }

    std::string handleDownLoadCMD(std::string& filepath) {
        std::ifstream ifs(filepath, std::ios::in|std::ios::binary);
        if (!ifs) {
            std::cout << "can not read file: " << filepath << std::endl;
        }
        std::ostringstream buf;
        char ch;
        while(buf && ifs.get(ch)) buf.put(ch);
        return buf.str();
    }

    void parseUpload(std::string& body) {

    }

    void parseCommand(char* data) {
        std::string cmd(data);
        std::string token;

        // 获取指令类型, 规定第一个空格前的字符串是指令命 后面是 body
        for (char ch : cmd) {
            if (ch == ' ') {
                break;
            }
            token += ch;
        }
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);
        std::string body = cmd.substr(token.length() + 1);
        int choose = tokenCase[token];
        switch (choose) {
            case 0:
                handleDirCMD(body);
            case 1:
                parseUpload(body);
            default:
                return;
        }
    }

    std::vector<std::string> getCommand(const char* data) {
        std::vector<std::string> v;
        std::istringstream ss(data);
        std::string word;
        ss >> word;
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        v.push_back(word);
        while (ss >> word) {
            v.push_back(word);
        }
        return v;
    }

} // namespace parser


#endif //TCP_CS_PARSER_H
