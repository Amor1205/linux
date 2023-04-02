#include "protocol.hpp"
#include "sock.hpp"
#include<pthread.h>

static void Usage(string proc){
    cout << "How to use : calServer ?" << endl;
    cout << "Usage: " << proc << " port" << endl;
    exit(1);
}
void* HandlerRequest(void* args){
    int sock = *(int*) args;
    delete (int*) args;
    pthread_detach(pthread_self());
    //业务逻辑
    //version 1 :
    request_t req;
    ssize_t s = read(sock, &req, sizeof(req));
    if(s == sizeof(req)){
        // 读取到了完整的请求，待定
        //
        //req.x, req.y , req.op
        response_t resp = {0, 0};
        switch(req.op){
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
                if(req.y == 0) resp.code = -1; //除0错误
                else resp.result = req.x / req.y;
                break;            
            case '%':
                if(req.y == 0) resp.code = -2; //摸运算模0错误
                else resp.result = req.x % req.y;
                break;    
            default:
                resp.code = -3; //代表请求方法异常
                break;
        }
        write(sock, &resp, sizeof(resp));
    }
    close(sock);
}
int main(int argc, char* argv[]){
    if(argc != 2) Usage(argv[0]);
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    for(;;){
        int sock = Sock::Accept(listen_sock);
        if(sock >= 0){
            //多线程
            cout << "New Client is coming..." << endl;
            int* pram = new int(sock);
            pthread_t tid;
            pthread_create(&tid, nullptr, HandlerRequest, pram);

        }
    }
    return 0;
}