#include <iostream>
#include <string>
#include <regex>

int main()
{
    //HTTP请求行格式： GET /myweb/index.html HTTP/1.1\r\n
    // std::string str = "GET /myweb/index.html?user=xiaoming&pass=12345 HTTP/1.1\r\n";
    std::string str = "GET /myweb/index.html?user=xiaoming&pass=12345 HTTP/1.1\r\n";
    std::smatch matches;
    //请求方法的匹配： GET HEAD POST PUT DELETE
    // std::regex e("(GET|HEAD|POST|PUT|DELETE) .*");

    // std::regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*).*");
    // [^?] 非问号字符
    // (GET|HEAD|POST|PUT|DELETE) 匹配任意一个字符串
    // std::regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*)\\?(.*) (HTTP/1\\.[01])");
    //[01]，代表0或者1
    // ?有特殊含义，需要添加\转义，但是\也需要添加\转义，所以两个\
    //？之后，空格之前的所有字符，所以用\\?(.*)加空格
    // . 除了\r \n的任意字符
    std::regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*)(?:\\?(.*))? (HTTP/1\\.[01])(?:\n|\r\n)?");
    bool ret = std::regex_match(str, matches, e);
    if(ret == false) {perror("regex_match error\n"); return -1;}
    for(auto& s : matches) std::cout << s << std::endl;
    return 0;
}