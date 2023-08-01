#include "../source/server.hpp"
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

int main(){
    Socket cli_sock;
    cli_sock.CreateClient(8080, "127.0.0.1");
    std::string req = "Get /hello HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 100\r\n\r\n";
    while (1) {
        assert(cli_sock.Send(req.c_str(), req.size()) != -1);
        assert(cli_sock.Send(req.c_str(), req.size()) != -1);
        assert(cli_sock.Send(req.c_str(), req.size()) != -1);
        char buf[1024] = {0};
        assert(cli_sock.Recv(buf, 1023));
        DBG_LOG("[%s]", buf);
        sleep(3);
    }
    cli_sock.Close();

    return 0;
}