#include "../source/server.hpp"
#include <cstdlib>
// #include "../source/baseSource.hpp"

uint64_t conn_id = 0;
void OnMessage(const PtrConnection& conn, Buffer* buf){
    DBG_LOG("%s", buf->ReadPosition());
    buf->MoveReadOffset(buf->ReadAbleSize());
    std::string str = "HELLO WROLD!";
    conn->Send(str.c_str(), str.size());
    // conn->ShutDown();
}

std::unordered_map<uint64_t, PtrConnection> _conns;
void ConnectionDestroy(const PtrConnection& conn){
    _conns.erase(conn->Id());
}
void OnConnected(const PtrConnection& conn){
    DBG_LOG("NEW CONNECTION : %p", conn.get());
}
void Acceptor(EventLoop* loop, Channel* lst_channel){
    int fd = lst_channel->Fd();
    int newfd = accept(fd, nullptr, nullptr);
    if(newfd < 0) return ;

    PtrConnection conn(new Connection(loop, conn_id, newfd));

    conn->SetMessageCallBack(std::bind(OnMessage, std::placeholders::_1, std::placeholders::_2)); 
    conn->SetServerClosedCallBack(std::bind(ConnectionDestroy, std::placeholders::_1));
    conn->SetConnectedCallBack(std::bind(OnConnected, std::placeholders::_1));
    
    //启动非活跃连接销毁
    conn->EnableInactiveRelease(10);
    //就绪初始化
    conn->Established();
    //用map管理起来
    _conns.insert({conn_id, conn});
    conn_id++;
}
int main(){
    srand(time(nullptr));
    EventLoop loop;
    Socket lst_sock;
    lst_sock.CreateServer(8080);
    //为监听套接字创建一个channel进行事件的管理，以及事件的处理
    Channel channel(&loop, lst_sock.Fd());
    channel.SetReadCallBack(std::bind(Acceptor, &loop, &channel));//回调中，获取新连接，为新连接创建channel并添加监控
    channel.EnableRead(); //启动可读事件监控
    while(1){
        loop.Start();
        // std::vector<Channel*> actives;
        // poller.Poll(&actives);
        // for(auto &a : actives){
        //     a->HandleEvent();
        // }
    }

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
    lst_sock.Close();
    return 0;
}




// int main(){ 
//     Buffer buf;

//     for(int i = 0; i < 300; ++i){
//         std::string str = "hello!" + std::to_string(i) + '\n';
//         buf.WriteStringAndPush(str);
//     }
//     //可能会出现没有换行符，就会导致一直循环的问题，但是http对应 不能直接把半行的数据读取出来，因为可能http没有发送全部的数据，我们直接按照整体数据进行读取，会有问题。
//     while(buf.ReadAbleSize() > 0){
//         std::string line = buf.GetLineAndPop();
//         ERR_LOG("hello");
//     }

//     std::cout << buf.ReadAbleSize() << std::endl;
//     std::string tmp = buf.ReadAsStringAndPop(buf.ReadAbleSize());
//     std::cout << tmp << std::endl;
//     /*
//     buf.WriteStringAndPush(str);

//     Buffer buf1;
//     buf1.WriteBufferAndPush(buf);
//     std::string tmp = buf1.ReadAsStringAndPop(buf.ReadAbleSize());

//     std::cout << tmp << std::endl;
//     std::cout << buf.ReadAbleSize() << std::endl;
//     std::cout << buf1.ReadAbleSize() << std::endl;
//     */
//     return 0;
// }