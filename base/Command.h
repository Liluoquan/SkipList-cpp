#pragma once

#include <string>
#include <iostream>
#include <vector>


const std::vector<std::string> _cmdList = { "set","get","del","load","dump" };
const std::string DELIMITER = " ";

class Command {
public:
    Command(std::string cmd) : _command(cmd) {};
    std::string get_command();
    bool is_valid_command();
    void split_command();
    void print_error(int);
    std::vector<std::string> _arg;

private:
    std::string _command;
};

std::string Command::get_command() {
    return _command;
}

void Command::split_command() {
    if(_command.empty())    return;

    std::string strs = get_command() + DELIMITER;

    size_t pos = strs.find(DELIMITER);
    size_t size = strs.size();

    while(pos != std::string::npos) {
        std::string cmd = strs.substr(0, pos);
        std::cout << cmd << " ";
        _arg.emplace_back(cmd);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(DELIMITER);
    }
    std::cout << std::endl;

    return;
}

bool Command::is_valid_command() {
    split_command();
    if(_arg.size() == 0) {
        return false;
    }

    std::string cmd = _arg[0];
    bool isValid = false;
    for(int i = 0; i < _cmdList.size(); ++i) {
        if(cmd == _cmdList[i]) {
            isValid = true;
        }
    }

    // unknown cmd
    if(!isValid) {
        print_error(1);
        return false;
    }

    // set key value
    if(cmd == _cmdList[0] && _arg.size() != 3) {
        print_error(2);
        return false;
    }
    // get key
    else if(cmd == _cmdList[1] && _arg.size() != 2) {
        print_error(3);
        return false;
    }
    // del key
    else if(cmd == _cmdList[2] && _arg.size() != 2) {
        print_error(4);
        return false;
    }
    // load
    else if(cmd == _cmdList[3] && _arg.size() != 1) {
        print_error(5);
        return false;
    }
    // dump
    else if(cmd == _cmdList[4] && _arg.size() != 1) {
        print_error(6);
        return false;
    }

    return true;
}

void Command::print_error(int errorNum) {
    switch(errorNum) {
        case 1/* constant-expression */:
            std::cout << "(error) ERR unknown command" << std::endl;
            break;

        case 2/* constant-expression */:
            std::cout << "(error) ERR wrong number of arguments for 'set' command" << std::endl;
            break;

        case 3/* constant-expression */:
            std::cout << "(error) ERR wrong number of arguments for 'get' command" << std::endl;
            break;
        case 4/* constant-expression */:
            std::cout << "(error) ERR wrong number of arguments for 'del' command" << std::endl;
            break;

        case 5/* constant-expression */:
            std::cout << "(error) ERR wrong number of arguments for 'load' command" << std::endl;
            break;
        case 6/* constant-expression */:
            std::cout << "(error) ERR wrong number of arguments for 'dump' command" << std::endl;
            break;
        default:
            break;
    }
}




