#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<cerrno>

const int port = 43214 ;
int main(){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        std::cerr << "socket creat error: " << sock << std::endl;
        return 1;
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port  = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
        std::cerr << "bind error : " << errno << std::endl;
        return 2;
    }
    //提供服务
    bool quit = false;
    #define NUM 1024
    char buffer[NUM];
    while(!quit){
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer,&len);
        std::cout << "client# "<< buffer << std::endl;
        std::string echo = "hello";
        sendto(sock, echo.c_str(), echo.size(), 0, (struct sockaddr*)&peer, len );
    }
    return 0;
}