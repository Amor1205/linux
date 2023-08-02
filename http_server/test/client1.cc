#include "../source/server.hpp"
#include "../http/newhttp.hpp"
/*长连接测试1: 创建一个客户端持续给服务器发送数据，知道超过超时时间看是否正常*/
// int main(){
//     Socket cli_sock;
//     cli_sock.CreateClient(8080, "127.0.0.1");
//     std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//     while (1){
//         assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//         char buf[1024] = {0};
//         assert(cli_sock.Recv(buf, 1023));
//         DBG_LOG("[%s]", buf);
//         sleep(2);
//     }
//     cli_sock.Close();

//     return 0;
// }
/*长连接测试2: 创建一个客户端给服务器发送1个数据，直到超过超时时间看是否正常退出*/
// int main(){
//     Socket cli_sock;
//     cli_sock.CreateClient(8080, "127.0.0.1");
//     std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//     assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//     char buf[1024] = {0};
//     assert(cli_sock.Recv(buf, 1023));
//     DBG_LOG("[%s]", buf);
//     sleep(15);
//     cli_sock.Close();

//     return 0;
// }
/*给服务器发送数据，告诉服务器发送n字节的数据，但是实际上发送的数据不足n，查看服务器的处理结果*/
/*
1. 数据只发送一次，服务器得到不完整请求，就不会进行业务处理，客户端得不到响应，最终超时关闭连接
2. 数据发送多次（小请求）服务器会将后面的请求当作前面请求的正文进行处理，后边处理可能会因为处理错误关闭连接
*/
// int main(){
//     Socket cli_sock;
//     cli_sock.CreateClient(8080, "127.0.0.1");
//     std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 100\r\n\r\n";
//     while (1) {
//         assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//         assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//         assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//         char buf[1024] = {0};
//         assert(cli_sock.Recv(buf, 1023));
//         DBG_LOG("[%s]", buf);
//         sleep(3);
//     }
//     cli_sock.Close();

//     return 0;
// }


/*业务处理超时，查看服务器的处理情况*/
/*当服务器达到了性能瓶颈，在一次业务处理中花费了太多的时间，超过了服务器设置的非活跃超时时间*/
/*1. 在一次业务处理中，其他的连接也被连累超时了，其他的连接有可能会被拖累超时释放
    假设有12345描述符就绪，处理1的时候花费太长时间导致超时，2345也会长时间没刷新活跃度
    1. 如果接下来的2345描述符是通信连接描述符，如果都就绪了，并不影响，因为接下来就会进行处理并刷新活跃度
    2. 如果接下来的2 是定时器事件描述符，定时器出发超时，执行定时任务，就会将345描述符给释放
        这时候一旦345对应连接被释放，接下来处理345就会内存访问错误。
    所以本次事件处理中，不能直接对连接进行释放，而是将释放操作压入到任务池中，等到事件处理完了在执行任务池中的任务，从而释放。
*/
/*2. */

// int main(){
//     signal(SIGCHLD, SIG_IGN);
//     for (int i = 0; i < 10; ++i) {
//         pid_t pid = fork();
//         if (pid < 0) {
//             DBG_LOG("FORK ERROR");
//             return -1;
//         }else if (pid == 0) {
//             Socket cli_sock;
//             cli_sock.CreateClient(8080, "127.0.0.1");
//             std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//             while (1) {
//                 assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//                 char buf[1024] = {0};
//                 assert(cli_sock.Recv(buf, 1023));
//                 DBG_LOG("[%s]", buf);
//                 sleep(3);
//             }
//             cli_sock.Close();
//             exit(0);
//         }
//     }
//     while (1) sleep(1);
//     return 0;
// }

/*一次性给服务器发送多条数据，看服务器的回应*/
/**/

// int main(){
//     Socket cli_sock;
//     cli_sock.CreateClient(8080, "127.0.0.1");
//     std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
//     req += req;
//     req += req;
//     while (1) {
//         assert(cli_sock.Send(req.c_str(), req.size()) != -1);
//         char buf[1024] = {0};
//         assert(cli_sock.Recv(buf, 1023));
//         DBG_LOG("[%s]", buf);
//         sleep(3);
//     }
//     cli_sock.Close();

//     return 0;
// }

/*大文件传输测试，给服务器上传一个大文件，服务器将文件保存下来，观察处理结果*/
/*预期结果：上传文件和服务器保存的文件一致*/

int main(){
    Socket cli_sock;
    cli_sock.CreateClient(8080, "127.0.0.1");
    std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\n";
    std::string body;
    Util::ReadFile("./hello.txt", &body);
    req += "Content-Length: " + std::to_string(body.size()) + "\r\n";
    {
        assert(cli_sock.Send(req.c_str(), req.size()) != -1);
        assert(cli_sock.Send(body.c_str(), body.size()) != -1);
        char buf[1024] = {0};
        assert(cli_sock.Recv(buf, 1023));
        DBG_LOG("[%s]", buf);
        sleep(3);
    }
    cli_sock.Close();

    return 0;
}