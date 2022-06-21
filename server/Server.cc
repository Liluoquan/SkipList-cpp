#include <algorithm>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Server.h"
#include "../base/SkipList.h"

#define BUFFSIZE 1024
#define LEVELOFSKIPLIST 12

static SkipList<std::string, std::string> myDB(LEVELOFSKIPLIST);

std::unordered_map<std::string, parseState> SkipListWorker::_cmdMap = {
    {"SET", parseState::SET},
    {"GET", parseState::GET},
    {"DEL", parseState::DEL},
    {"LOAD", parseState::LOAD},
    {"DUMP", parseState::DUMP}
};

std::vector<std::function<std::string(std::string)>> SkipListWorker::_handler = {
    {SkipListWorker::handlerERR},
    {SkipListWorker::handlerSET},
    {SkipListWorker::handlerGET},
    {SkipListWorker::handlerDEL},
    {SkipListWorker::handlerLOAD},
    {SkipListWorker::handlerDUMP}
};

std::string SkipListWorker::handlerERR(std::string cmd) {
    std::cout << "cmd is " << cmd << std::endl;
    return "please enter a correct command : ";
}

std::string SkipListWorker::handlerSET(std::string cmd) {
    // myDB.insertElement();
}


parseState SkipListWorker::parseCommand(std::string& cmd) {
    std::string res;
    
    // 先将 cmd 转换为大写
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
    if(_cmdMap.count(cmd) == 0) {  // 命令不存在
        return parseState::ERR;
    }

    return _cmdMap[cmd];
}

std::string SkipListWorker::dealWithCmd(std::string cmd) {
    parseState state = SkipListWorker::parseCommand(cmd);
    return _handler[state](cmd);
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



