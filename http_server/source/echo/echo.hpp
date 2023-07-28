#include "../server.hpp"

//回显
class EchoServer {
private:
    TcpServer _server;
private:
    void OnClosed(const PtrConnection& conn){
        DBG_LOG("CLOSE CONNECTION : %p", conn.get());
    }
    void OnConnected(const PtrConnection& conn){
        DBG_LOG("NEW CONNECTION : %p", conn.get());
    }
    void OnMessage(const PtrConnection& conn, Buffer* buf){
        // DBG_LOG("%s", buf->ReadPosition());
        conn->Send(buf->ReadPosition(), buf->ReadAbleSize());
        buf->MoveReadOffset(buf->ReadAbleSize());
        conn->ShutDown();
    }
public:
    EchoServer(int port) :_server(port) {
        _server.SetThreadCount(2);
        _server.EnableInactiveRelease(10);
        //因为在echo类中定义的成员函数是有隐藏this指针的，所以我们需要在使用bind传参
        _server.SetClosedCallBack(std::bind(&EchoServer::OnClosed, this, std::placeholders::_1));
        _server.SetConnectedCallBack(std::bind(&EchoServer::OnConnected, this, std::placeholders::_1));
        _server.SetMessageCallBack(std::bind(&EchoServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    }
    void Start() {
        _server.Start();
    }
};