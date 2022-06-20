#pragma once


#include <string>
#include <unordered_map>


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
    static parseState parseCommand(std::string cmd);

private:
    static std::unordered_map<std::string, parseState> _cmdMap;
};
