#pragma once
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
#include <signal.h>
#include <pthread.h>

namespace ns_task
{
    class Task
    {
    private:
        int sock_;

    public:
        Task()
            : sock_(-1)
        {
        }
        Task(int sock) : sock_(sock) {}
        ~Task()
        {
        }
        int operator()()
        {
            return Run();
        }
        int Run()
        {
            // while (true)
            // {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            ssize_t s = read(sock_, buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                // 读取成功
                buffer[s] = 0;
                std::cout << "client# " << buffer << std::endl;
                std::string echo_string = ">>>server<<<, ";
                echo_string += buffer;
                write(sock_, echo_string.c_str(), echo_string.size());
            }
            else if (s == 0)
            {
                std::cout << "client quit " << std::endl;
                // break;
            }
            else
            {
                std::cerr << "read error : " << errno << std::endl;
                // break;
            }
            
            close(sock_);
            //}
        }
    };
};