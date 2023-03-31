#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include<signal.h>

void ServiceIO(int new_sock)
{
    while (true)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t s = read(new_sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            // 读取成功
            buffer[s] = 0;
            std::cout << "client# " << buffer << std::endl;
            std::string echo_string = ">>>server<<<, ";
            echo_string += buffer;
            write(new_sock, echo_string.c_str(), echo_string.size());
        }
        else if (s == 0)
        {
            std::cout << "client quit " << std::endl;
            break;
        }
        else
        {
            std::cerr << "read error : " << errno << std::endl;
            break;
        }
    }
}
void Usage(std::string proc)
{
    std::cout << "usage : " << proc << " port " << std::endl;
}
// 如下方式运行： ./tcp_server 8081
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    // 1.创建套接字
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
    {
        std::cerr << "socket error: " << errno << std::endl;
        return 2;
    }
    // 2.bind
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[1]));
    local.sin_addr.s_addr = INADDR_ANY;
    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        std::cerr << "bind error: " << errno << std::endl;
        return 3;
    }
    // 因为tcp面向连接，在通信前，需要建立连接。然后才能通信。
    const int back_log = 5;
    if (listen(listen_sock, back_log) < 0)
    {
        std::cerr << "listen error : " << errno << std::endl;
        return 4;
    }
    //signal(SIGCHLD, SIG_IGN);//父进程会自动忽略子进程退出，子进程资源自动释放

    for (;;)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock, (struct sockaddr *)&peer, &len);
        if (new_sock < 0)
        {
            continue;
        }
        uint16_t cli_port = ntohs(peer.sin_port); //拿到客户端port- 4字节
        std::string cli_ip = inet_ntoa(peer.sin_addr);
        
        std::cout << "get a new link " << ":[" << cli_ip <<":"<< cli_port << "]# " << new_sock<< std::endl;
        pid_t id = fork();
        if (id < 0)
        {
            continue;
        }
        else if (id == 0)
        {
            // child
            close(listen_sock);
            if(fork() > 0) exit(0);
            ServiceIO(new_sock);
            close(new_sock);
            exit(0);
        }else{
            //father
            //do nothing
            waitpid(id, nullptr, 0); //等待的时候不会被阻塞。
            close(new_sock);
        }
    }
    return 0;
}