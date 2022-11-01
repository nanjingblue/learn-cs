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
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
        {
            do {
                //如果是目录,迭代之（即文件夹内还有文件夹）
                if ((fileinfo.attrib & _A_SUBDIR)) {
                    //文件名不等于"."&&文件名不等于".."
                    //.表示当前目录
                    //..表示当前目录的父目录
                    //判断时，两者都要忽略，不然就无限递归跳不出去了！
//                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
//                        getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                }
                    //如果不是,加入列表
                else {
//                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                    files.emplace_back(fileinfo.name);
                }
            } while (_findnext(hFile, &fileinfo) == 0);
            //_findclose函数结束查找
            _findclose(hFile);
        }
    }

    std::string parseDir(std::string& body) {
//        body = "C:\\Users\\nanjingblue\\Documents\\default\\Course\\Grid programming\\winsocket";
        std::vector<std::string> fileList;
        getFiles(body, fileList);
        for (const auto& it : fileList) {
            std::cout << it << std::endl;
        }
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
        std::cout << body << std::endl;
        int choose = tokenCase[token];
        switch (choose) {
            case 0:
                parseDir(body);
            default:
                return;
        }
    }

} // namespace parser


#endif //TCP_CS_PARSER_H
