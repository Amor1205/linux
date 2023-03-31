#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<cerrno>
#include<cstring>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

void Usage(std::string proc){
    std::cout << "Usage : " << proc << "server_ip server_port" << std::endl;
}
// ./udp_client server_ip server_port
int main(int argc, char* argv[]){
    if(argc != 3){
        Usage(argv[0]);
        return 1;
    }
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        std::cerr << "SOCK error : " << errno << std::endl;
        return 2;
    }
    //2.bind
    //客户端不需要显式绑定
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        std::cout << "connect server failed " << std::endl;
        return 3;
    }
    std::cout << "connect success !" << std::endl;
    //正常业务请求
    while(1){
        std::cout<< "Please Enter# ";
        char buffer[1024];
        fgets(buffer, sizeof(buffer)-1, stdin);
        write(sock, buffer, strlen(buffer));

        ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
        if(s > 0){
            buffer[s] = 0;
            std::cout << "server echo# " << buffer << std::endl;
        }
    }
    //3.listen
    //4.accept
    return 0;
}