#pragma once

#include <functional>


class Server {
public:
    Server(int threadNum, int port);
    ~Server();
    void start();

private:
    int _threadNUm;
    int _port;
    int _listenFd;

    std::function<void()> threadFunc;
};
