#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<cerrno>

void Usage(std::string proc){
    std::cout << "Usage: \n\t" << proc << "server_ip server_port" << std::endl;
}
int main(int argc, char* argv[]){
    if( argc != 3){
        Usage(argv[0]);
        return 0;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        std::cerr << "socket error : " << errno << std::endl;
        return 1;
    }
    //客户端需要bind 吗？
    //客户端也需要有ip 和 port
    //但是客户端不需要显式绑定，一旦显式绑定，必须要明确，client要和哪个port关联。
    //client 指明的端口号在client 不一定有。

    //使用服务
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    bool quit = false;
    while(!quit){
        //数据从哪来
        std::string message;
        std::cout << "Please input something# ";
        std::cin>> message;

        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server,sizeof(server));
        //此处tmp 就是占位符
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        char buffer[1024];
        ssize_t cnt = recvfrom(sock, buffer,sizeof(buffer)-1, 0, (struct sockaddr*)&tmp, &len);
        if(cnt > 0){
            buffer[cnt] = 0;
            std::cout << "server echo# " <<buffer << std::endl;
        }
    }
    return 0;
}