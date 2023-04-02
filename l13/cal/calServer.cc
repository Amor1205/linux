#include "protocol.hpp"
#include "sock.hpp"
#include <pthread.h>

static void Usage(string proc)
{
    cout << "How to use : calServer ?" << endl;
    cout << "Usage: " << proc << " port" << endl;
    exit(1);
}
void *HandlerRequest(void *args)
{
    int sock = *(int *)args;
    delete (int *)args;
    pthread_detach(pthread_self());
    // 业务逻辑
    // version 1 :
    char buffer[1024];
    // ssize_t s = read(sock, &req, sizeof(req));
    ssize_t s = read(sock, buffer, sizeof(buffer));
    if (s > 0)
    {
        request_t req;
        memset(&req, 0, sizeof(req));
        buffer[s] = 0;
        cout << "get a new request: " << buffer << endl;
        std::string str = buffer;
        DeserializeRequest(str, req);//反序列化一个请求，把请求中的字符串变成数据结构，得到了req是一个数据结构
        // if(s == sizeof(req)){
        //  读取到了完整的请求，待定
        //
        // req.x, req.y , req.op
        response_t resp = {0, 0};
        switch (req.op)
        {
        case '+':
            resp.result = req.x + req.y;
            break;
        case '-':
            resp.result = req.x - req.y;
            break;
        case '*':
            resp.result = req.x * req.y;
            break;
        case '/':
            if (req.y == 0)
                resp.code = -1; // 除0错误
            else
                resp.result = req.x / req.y;
            break;
        case '%':
            if (req.y == 0)
                resp.code = -2; // 摸运算模0错误
            else
                resp.result = req.x % req.y;
            break;
        default:
            resp.code = -3; // 代表请求方法异常
            break;
        }
        //}
        // write(sock, &resp, sizeof(resp));
        std::string send_string = SerializeResponse(resp);//把响应序列化，变成字符串。
        write(sock, send_string.c_str(), send_string.size());
        cout << "结束服务:" << send_string << endl;
    }
    close(sock);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
        Usage(argv[0]);
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    for (;;)
    {
        int sock = Sock::Accept(listen_sock);
        if (sock >= 0)
        {
            // 多线程
            cout << "New Client is coming..." << endl;
            int *pram = new int(sock);
            pthread_t tid;
            pthread_create(&tid, nullptr, HandlerRequest, pram);
        }
    }
    return 0;
}