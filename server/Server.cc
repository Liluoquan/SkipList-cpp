#include <algorithm>

#include "Server.h"

std::unordered_map<std::string, parseState> SkipListWorker::_cmdMap = {
    {"SET", parseState::SET},
    {"GET", parseState::GET},
    {"DEL", parseState::DEL},
    {"LOAD", parseState::LOAD},
    {"DUMP", parseState::DUMP}
};

parseState SkipListWorker::parseCommand(std::string cmd) {
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);

}






