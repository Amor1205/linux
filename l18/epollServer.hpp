#ifndef __EPOLL_SERVER_HPP__
#define __EPOLL_SERVER_HPP__

#include "sock.hpp"
#include "epoll.hpp"
#include "log.hpp"
#include <string>
#include <iostream>
#include <functional>
#include <cassert>

namespace amor
{
    const static int default_port = 8080;
    const static int gnum = 64;
    class EpollServer
    {
    public:
        using func_t = std::function<void(std::string)>;

    public:
        EpollServer(func_t handlerRequest, const int &port = default_port) : _port(port), _revs_num(gnum), _handlerRequest(handlerRequest)
        {
            // 申请对应的空间
            _revs = new struct epoll_event[_revs_num];
            // 创建listensock
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);
            // 创建epoll模型
            _epfd = Epoll::CreateEpoll();
            logMessage(DEBUG, "Init successfully, listensock : %d, epfd: %d", _listensock, _epfd);
            // listensock 放到epoll中，让epoll帮我们管理起来
            if (!Epoll::CtlEpoll(_epfd, EPOLL_CTL_ADD, _listensock, EPOLLIN))
            {
                exit(6);
            }
            logMessage(DEBUG, "Add listensock to epoll successfully.");
        }
        void LoopOnce(int timeout)
        {
            int n = Epoll::WaitEpoll(_epfd, _revs, _revs_num, timeout);
            switch (n)
            {
            case 0:
                logMessage(DEBUG, "Timeout...");
                break;
            case -1:
                logMessage(WARNING, "epoll wait error... : %s",strerror(errno));
                break;
            default:
                // 等待成功
                logMessage(NORMAL, "Get a event...");
                HandlerEvents(n);
                break;
            }
        }
        void HandlerEvents(int n)
        {
            // 处理n个
            for (int i = 0; i < n; i++)
            {
                uint32_t revents = _revs[i].events;
                int sock = _revs[i].data.fd;
                if (revents & EPOLLIN)
                {
                    // 读事件就绪
                    // 1. listen sock 就绪
                    if (sock == _listensock)
                        Accepter(_listensock);
                    // 2. 普通sock 就绪
                    else
                        Revcer(sock);
                }
            }
        }
        void Accepter(int listensock)
        {
            std::string clientIp;
            uint16_t clientPort;
            int sock = Sock::Accept(listensock, &clientIp, &clientPort);
            if (sock < 0)
            {
                logMessage(WARNING, "accept error!");
                return;
            }
            if (!Epoll::CtlEpoll(_epfd, EPOLL_CTL_ADD, sock, EPOLLIN))
                return;
            logMessage(DEBUG, "Add new sock : %d to epoll successfully", sock);
        }
        void Revcer(int sock)
        {
            // 读取数据
            char buffer[1024];
            ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                // 假设读取到了完整报文
                buffer[n] = 0;
                _handlerRequest(buffer);
            }
            else if (n == 0)
            {
                // 链接关了
                // 先去除epoll的关心，再去close，否则会报错
                bool res = Epoll::CtlEpoll(_epfd, EPOLL_CTL_DEL, sock, 0);
                assert(res);
                (void)res;
                close(sock);
                logMessage(NORMAL, "Client [%d] quits, i need to close it...", sock);
            }
            else
            {
                //接收报错
                bool res = Epoll::CtlEpoll(_epfd, EPOLL_CTL_DEL, sock, 0);
                assert(res);
                (void)res;
                close(sock);
                logMessage(NORMAL, "recv error (client[%d]), i need to close it...", sock);
            }
            // 处理数据
        }
        void Start()
        {
            int timeout = -1;
            while (true)
            {
                // 等
                LoopOnce(timeout);
            }
        }
        ~EpollServer()
        {
            if (_listensock >= 0)
                close(_listensock);
            if (_epfd >= 0)
                close(_epfd);
            if (_revs)
                delete[] _revs;
        }

    private:
        int _listensock;
        int _epfd; // 充当epoll模型
        uint16_t _port;
        struct epoll_event *_revs;
        int _revs_num;
        int _timeout = 1000;
        func_t _handlerRequest;
    };
}; // namespace amor

#endif