#include <iostream>
#include <string>
#include <regex>

int main()
{
    std::string str = "/numbers/1234";
    std::regex e("/numbers/(\\d+)");
    // std::regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*)\\?(.*) (HTTP/1\\.[01])(?:\n|\r\n)?");
    //匹配以/numbers/起始，后面跟了一个或者多个数字字符的字符串，并且在匹配的过程中提取这个匹配到的数字字符串
    // \d代表数字字符，加括号表示想要提取出来，+表示前面的字符出现一次或者多次
    //如果没有+，只会提取出1次，如果加上，就会继续匹配，直到出现的不是数字字符。

    std::smatch matches;
    bool ret = std::regex_match(str, matches, e);
    if(ret == false)
    {
        perror("regex_match error");
        return -1;
    }
    for(auto& s: matches)
    {
        std::cout << s << std::endl;
    }
    return 0;
}


