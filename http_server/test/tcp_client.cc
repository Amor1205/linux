// #include "../source/tsource.hpp"
#include "../source/server.hpp"

int main(){
    Socket cli_sock;
    cli_sock.CreateClient(8080, "127.0.0.1");
    std::string str = "hello!";
    for (int i = 0; i < 5; ++i){
        cli_sock.Send((void*)str.c_str(), str.size());
        char buf[1024] = {0};
        cli_sock.Recv(buf, 1023);
        DBG_LOG("%s", buf);
        sleep(2);
    }
    while (1) sleep(1);
    return 0;
}