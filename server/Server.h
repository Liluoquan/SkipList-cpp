#pragma once


#include <string>
#include <unordered_map>
#include <functional>
#include <vector>


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
    static parseState parseCommand(std::string& cmd);
    static std::string dealWithCmd(std::string cmd);

private:
    static std::unordered_map<std::string, parseState> _cmdMap;
    static std::vector<std::function<std::string(std::string)>> _handler;
    
    static std::string handlerERR(std::string);
    static std::string handlerSET(std::string);
    static std::string handlerGET(std::string);
    static std::string handlerDEL(std::string);
    static std::string handlerLOAD(std::string);
    static std::string handlerDUMP(std::string);
};



