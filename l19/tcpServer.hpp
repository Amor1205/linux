#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <functional>
#include "log.hpp"
#include "sock.hpp"
#include "epoll.hpp"
class TcpServer;
class Connection;

using func_t = std::function<void(Connection *)>;

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
    struct epoll_event* _revs;
    int _revs_num;

public:
    bool isConnExists(int sock){
        auto iter = _connections.find(sock);
        if(iter == _connections.end()) return false;
        return true;
    }
    TcpServer(int port = gport) : _port(port),_revs_num(gnum)
    {
        // 创建listensock
        _listensock = Sock::Socket();
        Sock::Bind(_listensock, _port);
        Sock::Listen(_listensock);
        // 创建多路转接对象
        _poll.CreateEpoll();
        
        //添加listensock到服务器中
        AddConncetion(_listensock, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr); // TODO
        //构建一个获取就绪事件的缓冲区
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
        logMessage(DEBUG, "Accepter has been called.");

    }
    void LoopOnce(){
        int n = _poll.WaitEpoll(_revs, _revs_num);
        for(int i = 0; i < n; ++i){
            int sock = _revs[i].data.fd;
            uint32_t revents = _revs[i].events;
            if(revents & EPOLLIN){
                if(isConnExists(sock) && _connections[sock]->_recv_cb != nullptr){
                    _connections[sock]->_recv_cb(_connections[sock]); //参数类型是Connection*
                }
                if(revents & EPOLLOUT){
                    if(isConnExists(sock) && _connections[sock]->_send_cb != nullptr){
                        _connections[sock]->_send_cb(_connections[sock]); //参数类型是Connection*
                    }
                }
            }
        }
    }
    //根据就绪的时间，进行特定事件的派发
    void Dispatcher(){
        while(true){
            LoopOnce();
        }
    }

    ~TcpServer()
    {
        if (_listensock >= 0)
            close(_listensock);
        if(_revs) delete []_revs;
    }
};