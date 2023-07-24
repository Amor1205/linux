#include "server.hpp"

int main(){
    Buffer buf;

    for(int i = 0; i < 300; ++i){
        std::string str = "hello!" + std::to_string(i) + '\n';
        buf.WriteStringAndPush(str);
    }
    //可能会出现没有换行符，就会导致一直循环的问题，但是http对应 不能直接把半行的数据读取出来，因为可能http没有发送全部的数据，我们直接按照整体数据进行读取，会有问题。
    while(buf.ReadAbleSize() > 0){
        std::string line = buf.GetLineAndPop();
        std::cout << line;
    }

    std::cout << buf.ReadAbleSize() << std::endl;
    std::string tmp = buf.ReadAsStringAndPop(buf.ReadAbleSize());
    std::cout << tmp << std::endl;
    /*
    buf.WriteStringAndPush(str);

    Buffer buf1;
    buf1.WriteBufferAndPush(buf);
    std::string tmp = buf1.ReadAsStringAndPop(buf.ReadAbleSize());

    std::cout << tmp << std::endl;
    std::cout << buf.ReadAbleSize() << std::endl;
    std::cout << buf1.ReadAbleSize() << std::endl;
    */
    return 0;
}