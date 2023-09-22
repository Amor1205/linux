#ifndef __SELECT_SVR_H__
#define __SELECT_SVR_H__

#include <iostream>
#include <errno.h>
#include <string>
#include <sys/select.h>
#include "sock.hpp"
#include "log.hpp"

const int BITS = 8;
const int TOTALNUM = (sizeof(fd_set) * BITS);
const int FD_NONE = -1;
using namespace std;
class SelectServer
{
public:
    SelectServer(const uint16_t &port = 8080) : _port(port)
    {
        _listensock = Sock::Socket();
        Sock::Bind(_listensock, _port);
        Sock::Listen(_listensock);
        logMessage(DEBUG, "%s", "Successfully create base socket");
        for (int i = 0; i < TOTALNUM; i++)
        {
            _array[i] = FD_NONE;
        }
        _array[0] = _listensock; // 约定
    }
    void Start()
    {
        // struct timeval timeout{5,0};//放到外面 五秒后会一直刷
        while (true)
        {
            // 获取新链接也是IO事件，读事件，如果没有链接到来直接调用accept会阻塞。
            //  struct timeval timeout{5,0};//放到里面，每隔五秒刷一次
            //  FD_SET(_listensock, &rfds);
            DebugPrint();

            fd_set rfds;
            FD_ZERO(&rfds);

            int maxFd = _listensock;
            for (int i = 0; i < TOTALNUM; i++)
            {
                if (_array[i] == FD_NONE)
                    continue;
                FD_SET(_array[i], &rfds);
                if (maxFd < _array[i])
                    maxFd = _array[i];
            }

            int n = select(maxFd + 1, &rfds, nullptr, nullptr, nullptr);
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
                HandlerEvent(rfds);
                break;
            }
        }
    }
    ~SelectServer()
    {
        if (_listensock >= 0)
            close(_listensock);
    }
    void DebugPrint()
    {
        cout << "_fd_array[]: ";
        for (int i = 0; i < TOTALNUM; i++)
        {
            if (_array[i] != FD_NONE)
                cout << _array[i] << " ";
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
        for (; pos < TOTALNUM; pos++)
        {
            if (_array[pos] == FD_NONE)
                break; // 找非空
        }
        if (pos == TOTALNUM)
        {
            logMessage(WARNING, "%s, %d", "Select Server is already full, close : %d", sock);
            close(sock);
        }
        else
        {
            _array[pos] = sock;
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
            _array[pos] = FD_NONE;
        }
        else
        {
            // 读取出错了
            logMessage(WARNING, "%d sock recv error, %d : %s", sock, errno, strerror(errno));
            close(sock);
            _array[pos] = FD_NONE;
        }
    }
    void HandlerEvent(const fd_set &rfds)
    { // fd_set 是一个位图集合里面可能存在多个sock
        for (int i = 0; i < TOTALNUM; i++)
        {
            if (_array[i] == FD_NONE)
                continue; // 非合法
            // 寻找合法且就绪的
            if (FD_ISSET(_array[i], &rfds))
            {
                // 读事件就绪，不一定用read接收
                if (i == 0 && _array[i] == _listensock)
                {
                    // 链接事件到来，Accept
                    Accepter();
                }
                else
                {
                    Recver(_array[i], i);
                }
            }
        }
    }

    uint16_t _port;
    int _listensock;
    int _array[TOTALNUM]; // 规定_array[0] = _listensock
};

#endif
