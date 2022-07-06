#include <algorithm>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#include "Server.h"

#define BUFFSIZE 1024
#define LEVELOFSKIPLIST 12

// API for debug
#define DEBUG

SkipList<std::string, std::string> SkipListWorker:: _myDB(LEVELOFSKIPLIST);
ThreadPool SkipListWorker::_readThreadPool(2);

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
    return "error : please enter a correct command : SET/DEL/GET/LOAD/DUMP\n";
}

std::string SkipListWorker::handlerSET(std::vector<std::string>& cmdList) {
    // _myDB.insertElement();
    // SET key value
    if (cmdList.size() != 3) {
        return "error : please enter a correct command : SET key value\n";
    }
    if (!_myDB.insertElement(cmdList[1], cmdList[2])) {
        return "ok : SET successfully\n";
    }
    else {
        return "warning : " + cmdList[1] + " is already existed\n";
    }
}

std::string SkipListWorker::handlerDEL(std::vector<std::string>& cmdList) {
    // DEL key
    if (cmdList.size() != 2) {
        return "error : please enter a correct command : DEL key\n";
    }
    if (_myDB.deleteElement(cmdList[1])) {
        return "ok : DEL successfully\n";
    }
    else {
        return "warning : " + cmdList[1] + " is not existed\n";
    }
}

std::string SkipListWorker::handlerGET(std::vector<std::string>& cmdList) {
    // GET key
    if (cmdList.size() != 2) {
        return "please enter a correct command : GET key\n";
    }
    std::string value;
    bool searchResult = _readThreadPool.enqueue([&](){
        return _myDB.searchElement(cmdList[1], value);
    }).get();

    if (searchResult) {
        return "ok : " + value + "\n";
    }
    else {
        return "warning : " + cmdList[1] + " is not existed\n";
    }
}

std::string SkipListWorker::handlerDUMP(std::vector<std::string>& cmdList) {
    _myDB.dumpFile();
    return "ok : dump file successfully\n";
}

std::string SkipListWorker::handlerLOAD(std::vector<std::string>& cmdList) {
    _myDB.loadFile();
    return "ok : load file successfully\n";
}

parseState SkipListWorker::parseCommand(std::string cmd, std::vector<std::string>& cmdList) {
    std::string res;
    // 按照空格分割 cmd
    int left = 0, right = 0;
    while (right < cmd.size()) {
        while (right < cmd.size() && cmd[right] == ' ') {  // 跳过前导空格
            ++right;
        }
        left = right;
        while (right < cmd.size() && cmd[right] != ' ') {  // 读取 非空格字符
            ++right;
        }
        std::string&& str = cmd.substr(left, right - left);
        if (str.size() != 0) {
            cmdList.emplace_back(str);
        }
        ++right;
    }

    // if(cmdList.size() == 1) return parseState::ERR;

    std::string cmdName = cmdList.front();
    // 先将 命令名 转换为大写
    std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), toupper);
    if (_cmdMap.count(cmdName) == 0) {  // 命令不存在
        std::cout << "cmdName do not exist..." << std::endl;
        return parseState::ERR;
    }

#ifdef DEBUG

    std::cout << "cmdList in parseCommand(2) is : " << std::endl;
    for(auto& str : cmdList) {
        std::cout << str << " ";
    }
    std::cout << std::endl;

#endif

    std::cout << "cmdName in parseCommand(2) is '" << cmdName << "'" << std::endl;
    return _cmdMap[cmdName];
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
#ifdef DEBUG

        std::cout << "res is '" << res << "'" << std::endl;

#endif
        send(acceptFd, res.c_str(), res.size(), 0);
    }
    // 记得关闭连接描述符
    close(acceptFd);
    std::cout << "connection closed" << std::endl;
}



