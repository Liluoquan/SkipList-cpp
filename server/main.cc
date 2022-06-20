#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

// 使用给定的端口设置套接字，返回监听到的套接字
int socket_bind_listen(int port) {
    // 检查port，取正确区间范围
    if(port < 0 || port > 65535) return -1;

    // 创建socket(IPV4 + TCP)，返回监听描述符
    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) return -1;

    // 消除bind时"Address already in use"错误
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, 
                sizeof(optval)) == -1) {
        close(listen_fd);
        return -1;
    }

    // 设置服务器IP和port，和监听描述符绑定
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(listen_fd);
        return -1;
    }

    // 开始监听，最大等待队列长为LISTENQ
    if(listen(listen_fd, 2048) == -1) {
        close(listen_fd);
        return -1;
    }

    // 无效监听描述符
    if (listen_fd == -1) {
        close(listen_fd);
        return -1;
    }
    return listen_fd;
}



void threadFunc(int socketFd) {
    
}


int main(int argc, char* argv[]) {
    int threadNum = 2;
    int port = 4242;

    int opt;
    const char* str = "p:";
    while((opt = getopt(argc, argv, str)) != -1) {
        switch(opt) {
            case 'p' : {
                port = atoi(optarg);
                break;
            }
            case 'v' : {
                printf("skiplist-store version 1.0 \n");
                break;
            }
            default : {
                printf("Usage: %s [-p port] [-v] \n", argv[0]);
                break;
            }
        }
    }
    
    int listen_fd = socket_bind_listen(port);
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof(client_addr);
    int accept_fd = 0;
    while((accept_fd = accept(listen_fd, (struct sockaddr *)&client_addr, 
                              &client_addr_len)) > 0) {
        //pthread_create();
        std::thread workThread(threadFunc, accept_fd);
    }

}



