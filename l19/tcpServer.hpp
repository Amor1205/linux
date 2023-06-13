#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <functional>
#include <cerrno>
#include <cassert>
#include "log.hpp"
#include "sock.hpp"
#include "epoll.hpp"
#include "protocol.hpp"

class TcpServer;
class Connection;

using func_t = std::function<void(Connection *)>;
using callback_t = std::function<void(Connection*, std::string &request)>; //server 和上层解耦
class Connection
{
public:
    Connection(int sock) : _sock(sock), _tsvr(nullptr)
    {
    }
    void SetCallBackFunc(func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        _recv_cb = recv_cb;
        _send_cb = send_cb;
        _except_cb = except_cb;
    }
    ~Connection()
    {
    }

public:
    int _sock;
    func_t _recv_cb; // 读取回调函数
    func_t _send_cb;
    func_t _except_cb;

    std::string _inbuffer; // 没有办法处理二进制流，可以处理文本信息。
    std::string _outbuffer;
    // 设置TcpServer的回指指针
    TcpServer *_tsvr;
};

class TcpServer
{
    const static int gport = 8080;
    const static int gnum = 128;

private:
    int _listensock;
    int _port;
    Epoll _poll;
    // sock :: Connection
    std::unordered_map<int, Connection *> _connections;
    struct epoll_event *_revs;
    int _revs_num;
    callback_t _cb;

public:
    bool isConnExists(int sock)
    {
        auto iter = _connections.find(sock);
        if (iter == _connections.end())
            return false;
        return true;
    }
    TcpServer(int port = gport) : _port(port), _revs_num(gnum)
    {
        // 创建listensock
        _listensock = Sock::Socket();
        Sock::Bind(_listensock, _port);
        Sock::Listen(_listensock);
        // 创建多路转接对象
        _poll.CreateEpoll();

        // 添加listensock到服务器中
        AddConncetion(_listensock, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr); // TODO
        // 构建一个获取就绪事件的缓冲区
        _revs = new struct epoll_event[_revs_num];
    }
    // 对sock进行添加入tcpServer
    void AddConncetion(int sock, func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        Sock::SetNonBlock(sock);
        // 设置回调，除了listensock 还会存在大量的socket，每个sock都会被封装成connection
        // TcpServer 必须对所有的connection进行管理

        // 1.构建connection对象 封装sock
        Connection *conn = new Connection(sock);
        conn->SetCallBackFunc(recv_cb, send_cb, except_cb); // 只设置读回调
        conn->_tsvr = this;

        // 添加sock到epoll模型中
        _poll.AddSockToEpoll(sock, EPOLLIN | EPOLLET); // 任何多路转接的服务器，一般默认只会打开对读取时间的关心，写入事件按需进行
        // 对应的Connection* 对象指针添加到映射表中
        _connections.insert(std::make_pair(sock, conn));
    }
    void Accepter(Connection *conn)
    {
        // logMessage(DEBUG, "Accepter has been called.");
        // 一定是listensock 就绪了，此次读取不会阻塞
        while (true)
        {
            std::string clientIp;
            uint16_t clientPort;
            // 一定是常规IOsock,但是并不一定只有一个，更多可能有多个。如果有多个而只Accept一个，就会丢失其他的。
            int accept_errno = 0;
            int sock = Sock::Accept(conn->_sock, &clientIp, &clientPort, &accept_errno);
            if (sock < 0)
            {
                // 可能是没有链接了
                if (accept_errno == EAGAIN || accept_errno == EWOULDBLOCK)
                    break;
                else if (accept_errno == EINTR) // 信号中断了IO处理，概率极低
                    continue;
                // 可能是真的失败
                else
                {
                    logMessage(WARNING, "Accept error, %d : %s", accept_errno, strerror(accept_errno));
                    break;
                }
            }
            else
            {
                // 将sock 托管给tcpserver
                AddConncetion(sock, std::bind(&TcpServer::Recver, this, std::placeholders::_1), std::bind(&TcpServer::Sender, this, std::placeholders::_1), std::bind(&TcpServer::Excepter, this, std::placeholders::_1));
                logMessage(DEBUG, "Accept client [%s:%d] : %d successfully, add to epoll&tcpServer successfully.", clientIp.c_str(), clientPort, sock);
            }
        }
    }
    void Recver(Connection *conn)
    {
        bool err = false;
        const int num = 1024;
        //logMessage(DEBUG, "Recver event exists, Recver() has been called. ");
        //v1 先不处理，面向字节流
        while(true){
            char buffer[num];            
            ssize_t n = recv(conn->_sock, buffer, sizeof(buffer)-1, 0);
            if(n < 0){
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;//正常
                else if(errno == EINTR) continue;
                else{
                    logMessage(ERROR, "recv error, %d : %s", errno, strerror(errno));
                    //异常都去交给except_cb
                    conn->_except_cb(conn);
                    err = true;
                    break;
                }
            }else if(n == 0){
                logMessage(DEBUG, "client[%d] quits, server closes [%d]", conn->_sock, conn->_sock);
                conn->_except_cb(conn);
                err = true;
                break;
            }
            else{
                //读取成功
                buffer[n] = 0;
                conn->_inbuffer += buffer;
            }
        }//end while
        logMessage(DEBUG, "conn->_inbuffer [sock : %d]: %s", conn->_sock, conn->_inbuffer.c_str());
        if(!err){
            std::vector<std::string> messages;
            SpliteMessage(conn->_inbuffer, &messages);
            for(auto &msg : messages){
                //能保证是一个完整报文
                _cb(conn, msg); //可以将message 封装成task，然后push到任务队列，任务处理后交给后端线程
            }
        }
    }
    //最开始的时候conn 没有被触发
    void Sender(Connection *conn)
    {
        while(true){
            ssize_t n = send(conn->_sock, conn->_outbuffer.c_str(), conn->_outbuffer.size(), 0);
            if(n > 0){
                conn->_outbuffer.erase(0, n);
                if(conn->_outbuffer.empty()) break;
            }else{
                if(errno == EAGAIN || errno == EWOULDBLOCK) break;
                else if(errno == EINTR) continue;
                else{
                    logMessage(ERROR, "send error, %d : %s", errno, strerror(errno));
                    break;
                }
            }
        }
        //不确定是否发完了，但是保证如果没有出错那么要么是发完了，要么是发送条件不满足。
        if(conn->_outbuffer.empty()) EnableReadWrite(conn, true, false);
        else EnableReadWrite(conn, true, true);//满了，下次再发。
    }
    void Excepter(Connection *conn)
    {
        if(!isConnExists(conn->_sock)) return;
        //1. 移除epoll中的sock
        bool res = _poll.DelEpoll(conn->_sock);
        assert(res);//需要判断

        //2. 从map中移除sock
        _connections.erase(conn->_sock);
        //3. 关闭sock
        close(conn->_sock);
        //4. delete conn
        delete conn;
        logMessage(DEBUG, "Excepter: recycling completed");
    }
    void EnableReadWrite(Connection* conn, bool readable, bool writable){
        //修改
        uint32_t events = (readable ? EPOLLIN : 0) | (writable ? EPOLLOUT : 0);
        bool res = _poll.CtlEpoll(conn->_sock, events);
        assert(res);
    }
    void LoopOnce()
    {
        int n = _poll.WaitEpoll(_revs, _revs_num);
        for (int i = 0; i < n; ++i)
        {
            int sock = _revs[i].data.fd;
            uint32_t revents = _revs[i].events;

            //将所有的异常全部都交给read 或者write 来统一处理
            if(revents & EPOLLERR) revents |= (EPOLLIN | EPOLLOUT);
            if(revents & EPOLLHUP) revents |= (EPOLLIN | EPOLLOUT);
            
            if (revents & EPOLLIN)  
            {
                if (isConnExists(sock) && _connections[sock]->_recv_cb != nullptr)
                {
                    _connections[sock]->_recv_cb(_connections[sock]); // 参数类型是Connection*
                }
                if (revents & EPOLLOUT)
                {
                    if (isConnExists(sock) && _connections[sock]->_send_cb != nullptr)
                    {
                        _connections[sock]->_send_cb(_connections[sock]); // 参数类型是Connection*
                    }
                }
            }
        }
    }
    // 根据就绪的时间，进行特定事件的派发
    void Dispatcher(callback_t cb)
    {   
        _cb = cb;
        while (true)
        {
            LoopOnce();
        }
    }

    ~TcpServer()
    {
        if (_listensock >= 0)
            close(_listensock);
        if (_revs)
            delete[] _revs;
    }
};