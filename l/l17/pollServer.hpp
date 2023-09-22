#ifndef __POLL_SVR_H__
#define __POLL_SVR_H__

#include <iostream>
#include <errno.h>
#include <string>
#include <poll.h>
#include "sock.hpp"
#include "log.hpp"

const int FD_NONE = -1;
using namespace std;
class PollServer
{
public:
    static const int nfds = 100;
public:
    PollServer(const uint16_t &port = 8080) : _port(port), _nfds(nfds)
    {
        _listensock = Sock::Socket();
        Sock::Bind(_listensock, _port);
        Sock::Listen(_listensock);
        logMessage(DEBUG, "%s", "Successfully create base socket");

        _fds = new struct pollfd[_nfds];
        for(int i = 0; i < _nfds; i++){
            _fds[i].fd = FD_NONE; //置空
            _fds[i].events = _fds[i].revents = 0;
        }
        _fds[0].fd = _listensock;//添加listen套接字
        _fds[0].events = POLLIN;
    }
    void Start()
    {
        while (true)
        {
            int n = poll(_fds, _nfds, _timeout);
            // int n = select(_listensock+1, &rfds, nullptr, nullptr, &timeout);

            switch (n)
            {
            case 0:
                logMessage(DEBUG, "%s", "time out... ");
                break;
            case -1:
                logMessage(WARNING, "Select error : %d : %s", errno, strerror(errno));
                break;
            default:
                //
                logMessage(DEBUG, "get a new link event..."); // 一直打印，因为链接建立了（就绪了）但是没取走。
                HandlerEvent();
                break;
            }
        }
    }
    ~PollServer()
    {
        if (_listensock >= 0)
            close(_listensock);
        if(_fds) delete[]_fds;
    }
    void DebugPrint()
    {
        cout << "_fd_array[]: ";
        for (int i = 0; i < _nfds; i++)
        {
            if (_fds[i].fd != FD_NONE)
                cout << _fds[i].fd << " ";
        }
        cout << endl;
    }

private:
    void Accepter()
    {
        string clientIp;
        uint16_t clientPort;
        // 成功就说明listen sock读事件就绪，可以读取了
        // 获取新链接了
        int sock = Sock::Accept(_listensock, &clientIp, &clientPort); // 不会阻塞了
        if (sock < 0)
        {
            logMessage(WARNING, "Accepting failed");
            return;
        }
        logMessage(DEBUG, "Get a new line successfully : [%s:%d] : %d", clientIp.c_str(), clientPort, sock);
        int pos = 1;
        for (; pos < _nfds; pos++)
        {
            if (_fds[pos].fd == FD_NONE)
                break; // 找非空
        }
        if (pos == _nfds)
        {
            //可以进行自动扩容
            logMessage(WARNING, "%s, %d", "Select Server is already full, close : %d", sock);
            close(sock);
        }
        else
        {
            _fds[pos].fd = sock;
            _fds[pos].events = POLLIN;
            logMessage(DEBUG, "We have put sock into the array...");
        }
    }
    void Recver(int sock, int pos)
    {
        // input 事件到来 read recv
        logMessage(DEBUG, "message in , get an IO event : %d", sock);
        // 不一定读取完整报文
        char buffer[1024];
        int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        ;
        if (n > 0)
        {
            buffer[n] = 0;
            logMessage(DEBUG, "client[%d]# %s", sock, buffer);
        }
        else if (n == 0)
        {
            // 对端关闭连接了
            logMessage(DEBUG, "client[%d] quits, I need to close...", sock);
            // 1. 不要让select 关心了
            // 2. 我们关闭不用的fd
            close(sock);
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = 0;
        }
        else
        {
            // 读取出错了
            logMessage(WARNING, "%d sock recv error, %d : %s", sock, errno, strerror(errno));
            close(sock);
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = 0;
        }
    }
    void HandlerEvent()
    { // fd_set 是一个位图集合里面可能存在多个sock
        for (int i = 0; i < _nfds; i++)
        {
            if (_fds[i].fd == FD_NONE) continue; // 非合法
            // 寻找合法且就绪的
            if (_fds[i].revents & POLLIN)
            {
                // 读事件就绪，不一定用read接收
                if (_fds[i].fd == _listensock)
                {
                    // 链接事件到来，Accept
                    Accepter();
                }
                else
                {
                    Recver(_fds[i].fd, i);
                }
            }
        }
    }

    uint16_t _port;
    int _listensock;
    struct pollfd* _fds;
    int _nfds;
    int _timeout = 1000;
};

#endif
