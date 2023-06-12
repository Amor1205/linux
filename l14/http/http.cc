#include"sock.hpp"
#include<string>
#include<iostream>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fstream>
#define WWWROOT "./wwwroot/"
#define HOME_PAGE "index.html"
void* HandlerHttpRequest(void* args){
    int sock = *(int*)args;
    delete (int*) args;
    pthread_detach (pthread_self());

#define SIZE 1024*10
    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t s = recv(sock, buffer, sizeof(buffer), 0);
    if(s > 0){
        //读取成功
        buffer[s] = 0;
        std::cout << buffer << std::endl;
        
        std::string html_file = WWWROOT;
        html_file += HOME_PAGE;
        struct stat st;
        stat(html_file.c_str(), &st);

        std::string http_response = "http/1.0 200 OK\n";
        http_response += "Content-Type: text/html; charset=utf8\n";
        http_response += "Content-Length: ";
        http_response += std::to_string(st.st_size);
        http_response += "\n";
        http_response += "\n"; //空行
        //正文
        std::ifstream in(html_file);
        if(!in.is_open()){
            std::cerr << "Opening html error" << std::endl;
            exit(2);
        }else{
            std::string content;
            std::string line;
            while(std::getline(in, line)){
                content += line;
            }
            http_response += content;
            in.close();
        }
        
        
        // std::string http_response = "http/1.0 200 OK\n";
        // http_response += "Content-Type: text/plain\n";
        // http_response += "\n";
        // http_response += "Hello! ";
        send(sock, http_response.c_str(), http_response.size(), 0);
        
        }
    close(sock);
    return nullptr;
}
void Usage(std::string proc){
    std::cout << "Usage: " << proc << " port" << std::endl;
}
int main(int argc, char* argv[]){
    if(argc != 2){
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    for(;;){
        int sock = Sock::Accept(listen_sock);
        if(sock > 0){
            pthread_t tid;
            int* parm = new int(sock);
            pthread_create(&tid, nullptr, HandlerHttpRequest, parm);
        }
    }
}