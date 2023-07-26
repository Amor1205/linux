#include "../source/server.hpp"
#include <cstdlib>
// #include "../source/baseSource.hpp"
//写入->HandleRead->向Send 先压入任务池中


void HandleClose(Channel* channel){
    DBG_LOG("Close: %d", channel->Fd());
    channel->Remove(); //移除监控
    delete channel;
}
void HandleRead(Channel* channel){
    int fd = channel->Fd();
    char buf[1024] = {0};
    int ret = recv(fd, buf, 1023, 0);
    if(ret <= 0){
        HandleClose(channel); //关闭释放
    }
    DBG_LOG("%s", buf);
    channel->EnableWrite(); //启动可写事件
}
void HandleWrite(Channel* channel){
    int fd = channel->Fd();
    const char* data = "Test: HandleWrite \n";
    int ret = send(fd, data, strlen(data), 0);
    if(ret < 0){
        HandleClose(channel);
    }
    channel->DisableWrite(); //关闭写监控
}

void HandleError(Channel* channel){
    HandleClose(channel);
}
void HandleEvent(EventLoop* loop, Channel* channel, uint64_t id){
    loop->TimerRefresh(id);
}
void Acceptor(EventLoop* loop, Channel* lst_channel){
    int fd = lst_channel->Fd();
    int newfd = accept(fd, nullptr, nullptr);
    if(newfd < 0) return ;

    uint64_t timerid = rand() % 10000;
    Channel* channel = new Channel(loop, newfd);
    //为通信套接字设置可读事件回调函数
    channel->SetReadCallBack(std::bind(HandleRead, channel)); 
    channel->SetWriteCallBack(std::bind(HandleWrite, channel));
    channel->SetCloseCallBack(std::bind(HandleClose, channel));
    channel->SetErrorCallBack(std::bind(HandleError, channel));
    channel->SetEventCallBack(std::bind(HandleEvent, loop, channel, timerid));
    channel->EnableRead();
    
    //非活跃连接的超时释放操作,10s后关闭连接
    /*注意定时销毁任务必须在启动读事件之前，因为有可能启动了事件监控后，立刻就有了事件，但是这时候还没有任务*/
    loop->TimerAdd(timerid, 10, std::bind(HandleClose, channel));
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