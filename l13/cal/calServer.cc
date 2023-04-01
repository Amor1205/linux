#include "protocol.hpp"
#include "sock.hpp"
#include<pthread.h>

static void Usage(string proc){
    cout << "How to use : calServer ?" << endl;
    cout << "Usage:" << proc << " port" << endl;
    exit(1);
}
void* HandlerRequest(void* args){
    int sock = *(int*) args;
    delete (int*) args;
    pthread_detach(pthread_self());
    //业务逻辑

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
            int* pram = new int(sock);
            pthread_t tid;
            pthread_create(&tid, nullptr, HandlerRequest, pram);

        }
    }
    return 0;
}