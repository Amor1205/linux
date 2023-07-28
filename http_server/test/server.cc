#include "../source/server.hpp"


void OnClosed(const PtrConnection& conn){
    DBG_LOG("CLOSE CONNECTION : %p", conn.get());
}
void OnConnected(const PtrConnection& conn){
    DBG_LOG("NEW CONNECTION : %p", conn.get());
}
void OnMessage(const PtrConnection& conn, Buffer* buf){
    DBG_LOG("%s", buf->ReadPosition());
    buf->MoveReadOffset(buf->ReadAbleSize());
    std::string str = "HELLO WROLD!";
    conn->Send(str.c_str(), str.size());
    // conn->ShutDown();
}

int main() {
    TcpServer server(8080);

    server.SetThreadCount(3);
    server.EnableInactiveRelease(10);
    server.SetMessageCallBack(OnMessage);
    server.SetConnectedCallBack(OnConnected);
    server.SetClosedCallBack(OnClosed);
    server.Start();
    return 0;
}