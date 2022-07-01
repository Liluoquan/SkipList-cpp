#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PROT 4242
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFERSIZE 4096

int main(int argc, char* argv[]) {
    std::cout << "starting client... " << std::endl;
    char sendBuff[BUFFERSIZE], recvBuff[BUFFERSIZE];
    bzero(sendBuff, sizeof(sendBuff));
    bzero(recvBuff, sizeof(recvBuff));

    int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1) {
        std::cout << "create socket error! " << std::endl;
        return 0;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PROT);
    inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr);

    std::cout << "connecting to Light-DB... " << std::endl;
    if (connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cout << "connect to server failed!" << std::endl;
        return 0;
    }

    while (true) {
        std::cout << "Light-DB>";
        std::string command;
        getline(std::cin, command);
        strcpy(sendBuff, command.c_str());
        // std::cout << std::endl;

        // 发送指令到 DB
        send(socketFd, sendBuff, strlen(sendBuff), 0);
        
        if (recv(socketFd, recvBuff, sizeof(recvBuff), 0) <= 0) {
            std::cout << "recv error! " << std::endl;
            break;
        }
        
        // 输出接收到的数据
        std::cout << "Light-DB>" << recvBuff << std::endl;
        memset(recvBuff, '\0', sizeof(recvBuff));
    }

    close(socketFd);
    std::cout << "client quit " << std::endl;
    return 0;
}

