#pragma once


#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

#include "../base/SkipList.h"

enum parseState {
    ERR  =  0,
    SET  =  1,
    GET  =  2,
    DEL  =  3,
    LOAD =  4,
    DUMP =  5
};

class SkipListWorker {
public:
    static void threadFunc(int acceptFd);
    static parseState parseCommand(std::string cmd, std::vector<std::string>& cmdList);
    static std::string dealWithCmd(std::string cmd);
    static SkipList<std::string, std::string> _myDB;

private:
    static std::unordered_map<std::string, parseState> _cmdMap;
    static std::vector<std::function<std::string(std::vector<std::string>&)>> _handler;
    
    static std::string handlerERR(std::vector<std::string>& cmdList);
    static std::string handlerSET(std::vector<std::string>& cmdList);
    static std::string handlerGET(std::vector<std::string>& cmdList);
    static std::string handlerDEL(std::vector<std::string>& cmdList);
    static std::string handlerLOAD(std::vector<std::string>& cmdList);
    static std::string handlerDUMP(std::vector<std::string>& cmdList);
};



