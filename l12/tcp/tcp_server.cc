#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<cerrno>
#include<cstring>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>

void Usage(std::string proc){
    std::cout << "usage : " << proc << " port " << std::endl;
}
//如下方式运行： ./tcp_server 8081
int main(int argc, char* argv[]){
    if(argc != 2){
        Usage(argv[0]);
        return 1;
    }
    //1.创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        std::cerr << "socket error: "<< errno << std::endl;
        return 2;
    }
    //2.bind
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[1]));
    local.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr*)&local, sizeof(local))<0 ){
        std::cerr << "bind error: " << errno << std::endl;
        return 3;
    }
    //因为tcp面向连接，在通信前，需要建立连接。然后才能通信。
    
    return 0;
}