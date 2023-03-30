#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<cerrno>
#include<cstdio>


// const int port = 43214 ;
std::string Usage(std::string proc)
{
    std:: cout << "Usage: " <<  proc << "port " << std::endl;
}
int main(int argc, char* argv[]){
    if(argc != 2){
        Usage(argv[0]);
        return -1;
    }
    uint16_t port = atoi(argv[1]);
    
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
        std::cerr << "bind2 error : " << errno << std::endl;
        return 2;
    }
    //提供服务
    bool quit = false;
    #define NUM 1024
    char buffer[NUM];
    while(!quit){
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t cnt = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer,&len);
        if(cnt > 0){
            buffer[cnt] = 0; //当成字符串命令
            FILE* fp = popen(buffer, "r");
            std::string echo;
            char line[1024] = {0};
            while(fgets(line, sizeof(line),fp)!= NULL){
                echo += line;
            }
            /*if(feof(fp)){
                //读取结果完成
            }*/
            pclose(fp);
            std::cout << "client# "<< buffer << std::endl;
            sendto(sock, echo.c_str(), echo.size(), 0, (struct sockaddr*)&peer, len );      
        }
    }
    return 0;
}