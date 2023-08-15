#include "../source/server.hpp"

std::unordered_map<uint64_t, PtrConnection> _conns;
EventLoop base_loop; //主线程中使用这个
uint64_t conn_id = 0;
LoopThreadPool* loop_pool;

void ConnectionDestroy(const PtrConnection& conn){
    _conns.erase(conn->Id());
}
void OnConnected(const PtrConnection& conn){
    DBG_LOG("NEW CONNECTION : %p", conn.get());
}
/*----------------我的------------------*/

void OnMessage(const PtrConnection& conn, Buffer* buf){
    DBG_LOG("%s", buf->ReadPosition());
    buf->MoveReadOffset(buf->ReadAbleSize());
    std::string str = "HELLO WROLD!";
    conn->Send(str.c_str(), str.size());
    // conn->ShutDown();
}
void NewConncection(int fd){
    conn_id++;
    PtrConnection conn(new Connection(loop_pool->NextLoop(), conn_id, fd));
    conn->SetMessageCallBack(std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2)); 
    conn->SetServerClosedCallBack(std::bind(ConnectionDestroy, std::placeholders::_1));
    conn->SetConnectedCallBack(std::bind(OnConnected, std::placeholders::_1));
    
    //启动非活跃连接销毁
    conn->EnableInactiveRelease(10);
    //就绪初始化
    conn->Established();
    //用map管理起来
    _conns.insert({conn_id, conn});

    DBG_LOG("-------NEW------CONNECTION-----");
}
int main(){
    srand(time(nullptr));
    loop_pool = new LoopThreadPool(&base_loop);
    loop_pool->SetThreadCount(2);
    loop_pool->Create();
    Acceptor acceptor(&base_loop, 8080);
    acceptor.SetAcceptCallBack(std::bind(NewConncection, std::placeholders::_1));
    acceptor.Listen();
    while (1) base_loop.Start();
    return 0;
    // Channel channel(&loop, lst_sock.Fd());
    // channel.SetReadCallBack(std::bind(Acceptor, &loop, &channel));//回调中，获取新连接，为新连接创建channel并添加监控
    // channel.EnableRead(); //启动可读事件监控
    //while(1){
        // std::vector<Channel*> actives;
        // poller.Poll(&actives);
        // for(auto &a : actives){
        //     a->HandleEvent();
        // }
    //}

    // while(1){
    //     int newfd = lst_sock.Accept();
    //     if(newfd < 0) continue;
    //     Socket cli_sock(newfd);
    //     char buf[1024] = {0};
    //     int ret = cli_sock.Recv(buf, 1023);
    //     if(ret < 0){
    //         cli_sock.Close();
    //         continue;
    //     }
    //     cli_sock.Send(buf, ret);
    //     cli_sock.Close();
    // }
    // lst_sock.Close();    
}


/*----------------TEST------------------*/

// void OnMessage(const PtrConnection& conn, Buffer* buf){
//     DBG_LOG("%s", buf->ReadPosition());
//     buf->MoveReadOffset(buf->ReadAbleSize());
//     std::string str = "HELLO WROLD!";
//     conn->Send(str.c_str(), str.size());
//     conn->ShutDown();
// }
// void NewConncection(int fd){
//     conn_id++;
//     next_loop = (next_loop + 1) % 2;
//     PtrConnection conn(new Connection(threads[next_loop].GetLoop(), conn_id, fd));

//     conn->SetMessageCallBack(std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2)); 
//     conn->SetServerClosedCallBack(std::bind(ConnectionDestroy, std::placeholders::_1));
//     conn->SetConnectedCallBack(std::bind(OnConnected, std::placeholders::_1));
    
//     //启动非活跃连接销毁
//     conn->EnableInactiveRelease(10);
//     //就绪初始化
//     conn->Established();
//     //用map管理起来
//     _conns.insert({conn_id, conn});

//     DBG_LOG("-------NEW------CONNECTION-----");
// }
// int main(){
//     srand(time(nullptr));
//     Acceptor acceptor(&base_loop, 8080);
//     // lst_sock.CreateServer(8080);
//     //为监听套接字创建一个channel进行事件的管理，以及事件的处理
//     acceptor.SetAcceptCallback(std::bind(NewConncection, std::placeholders::_1));
//     acceptor.Listen();
//     while (1) base_loop.Start();
//     return 0;
// }
