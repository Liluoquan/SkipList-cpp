#include <algorithm>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Server.h"
#include "../base/SkipList.h"

#define BUFFSIZE 1024
#define LEVELOFSKIPLIST 12

// API for debug
#define DEBUG

static SkipList<std::string, std::string> myDB(LEVELOFSKIPLIST);

std::unordered_map<std::string, parseState> SkipListWorker::_cmdMap = {
    {"SET", parseState::SET},
    {"GET", parseState::GET},
    {"DEL", parseState::DEL},
    {"LOAD", parseState::LOAD},
    {"DUMP", parseState::DUMP}
};

std::vector<std::function<std::string(std::vector<std::string>&)>> SkipListWorker::_handler = {
    {SkipListWorker::handlerERR},
    {SkipListWorker::handlerSET},
    {SkipListWorker::handlerGET},
    {SkipListWorker::handlerDEL},
    {SkipListWorker::handlerLOAD},
    {SkipListWorker::handlerDUMP}
};

std::string SkipListWorker::handlerERR(std::vector<std::string>& cmdList) {
#ifdef DEBUG

    std::cout << "handlerERR : " << std::endl;
    for(auto& str : cmdList) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

#endif
    return "please enter a correct command : ";
}

std::string SkipListWorker::handlerSET(std::vector<std::string>& cmdList) {
    // myDB.insertElement();
}


parseState SkipListWorker::parseCommand(std::string cmd, std::vector<std::string>& cmdList) {
    std::string res;
    // 按照空格分割 cmd
    int left = 0, right = 0;
    while(right < cmd.size()) {
        while(right < cmd.size() && cmd[right] == ' ') {  // 跳过前导空格
            ++right;
        }
        left = right;
        while(right < cmd.size() && cmd[right] != ' ') {  // 读取 非空格字符
            ++right;
        }
        std::string&& str = cmd.substr(left, right - left);
        if(str.size() != 0) {
            cmdList.emplace_back(str);
        }
        ++right;
    }

    if(cmdList.size() == 1) return parseState::ERR;

    std::string cmdName = cmdList.front();
    // 先将 命令名 转换为大写
    std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), toupper);
    if(_cmdMap.count(cmd) == 0) {  // 命令不存在
        return parseState::ERR;
    }

#ifdef DEBUG

    std::cout << "cmdList is : " << std::endl;
    for(auto& str : cmdList) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

#endif

    return _cmdMap[cmd];
}

std::string SkipListWorker::dealWithCmd(std::string cmd) {
    std::vector<std::string> cmdList;
    parseState state = SkipListWorker::parseCommand(cmd, cmdList);
    return _handler[state](cmdList);
}

void SkipListWorker::threadFunc(int acceptFd) {
    std::cout << "enter threadFunc now" << std::endl;
    char msgBuff[BUFFSIZE];
    bzero(msgBuff, sizeof(msgBuff));

    while(recv(acceptFd, msgBuff, sizeof(msgBuff), 0) > 0) {
        std::string cmd(msgBuff);
        bzero(msgBuff, sizeof(msgBuff));
        std::string res = dealWithCmd(cmd);
        send(acceptFd, res.c_str(), res.size(), 0);
    }

    std::cout << "connection closed" << std::endl;
}



