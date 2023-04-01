#pragma once

#include<iostream>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;
class Sock{
    public:
        static int Socket(){
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if(sock < 0){
                cerr << "creating socket failed " << endl;
                exit(2);
            }
            cout << "Creating socket is successful !" << endl;
            return sock;
        }
        static void Bind(int sock, uint16_t port){
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_port = htons(port);
            local.sin_addr.s_addr = INADDR_ANY;
            if(bind(sock, (struct sockaddr*)&local, sizeof(local)) == 0){
                cerr << "binding error " << endl;
                exit(3);
            }else{
                cout << "binding is successful !" << endl;
            }
        }
        static void Listen(int sock){
            if(listen(sock, 5) < 0){
                cerr << "listening failed" <<endl;
                exit(4);
            }
        }
        static int Accept(int sock){
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int fd = accept(sock, (struct sockaddr*)&peer, &len);
            if(fd >= 0){
                cout << "Accepting is successful !" << endl;
                return fd;
            }else{
                cout << "Accepting failed" << endl;
                return -1;
            }
        }
    private:

};