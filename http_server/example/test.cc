#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../source/server.hpp"


size_t Split(const std::string& src, const std::string& sep, std::vector<std::string>& array) {
    int offset = 0;
    //offset 是查找的起始位置，应该小于size，==size已经越界了
    while (offset < src.size()) {
        ssize_t pos = src.find(sep, offset); //在src字符串偏移量offset处，开始向后查找sep字符/子串，返回查找到的位置
        if (pos == std::string::npos) { //没找到特定字符
            if (pos == src.size()) break; //pos到了末尾，不用添加了
            array.push_back(src.substr(offset));
            return array.size();
        }
        if (pos == offset) {//如果二者相等，是空串，不用添加了
            offset = offset + sep.size();
            continue;
        }
        array.push_back(src.substr(offset, pos - offset));
        //更新offset
        offset = pos + sep.size();
    }

    return array.size();
}

     bool ReadFile(const std::string& filename, std::string& buf) {
        std::ifstream ifs(filename, std::ios::binary);
        if (ifs.is_open() == false) {
            ERR_LOG("OPEN %s FILE ERROR!\n", filename.c_str());
            return false;
        }
        size_t fsize = 0;
        //将文件跳转到指定位置(end)
        ifs.seekg(0, ifs.end);
        //获取当前读写位置相对于起始位置的偏移量，从末尾便宜就刚好是文件的大小
        fsize = ifs.tellg();
        //跳转到起始位置
        ifs.seekg(0, ifs.beg);
        //开辟文件大小的空间
        buf.resize(fsize);
        ifs.read(&buf[0], fsize);
        if( ifs.good() == false) {
            ERR_LOG("READ %s FILE ERROR!\n", filename.c_str());
            ifs.close();
            return false;
        }
        //关闭文件
        ifs.close();
        return true;
    }
    static bool WriteFile(const std::string& filename, const std::string& buf) {
        // std::ios::binary  二进制打开
        // std::ios::trunc 截断，原有的内容不要了。
        std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
        if (ofs.is_open() == false) {
            ERR_LOG("OPEN %s FILE ERROR!\n", filename.c_str());
            return false;
        }
        ofs.write(buf.c_str(), buf.size());
        if (ofs.good() == false) {
            ERR_LOG("WRITE TO %s FILE ERROR!\n", filename.c_str());
            ofs.close();
            return false;
        }
        ofs.close();
        return true;
    }
        static std::string UrlEncode(const std::string url, bool is_convert_space) {
        std::string str;
        for (auto& c : url) {
            //isdigit(c) || isalpha(c) 分别判断的是是否是数字或者是字母字符
            // isalnum(c) 等价上面两个
            if (c == '.' || c == '-' || c == '~' || c == '_' || isalnum(c)) {
                //直接添加
                str += c;
                continue;
            }
            if (c == ' ' && is_convert_space) {
                str += '+';
                continue;
            }
            //剩下的字符都是需要编码的
            char tmp[4] = {0};
            //%X 表示输出大写十六进制
            //%02X表示如果不够2位则占位，占位填充0
            snprintf(tmp, 4, "%%%02X", c);
            str += tmp;
        }
        return str;
    }
        static char HTOI(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        else if (c >= 'a' && c <= 'z') {
            return c - 'a' + 10;
        }else if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 10;
        }
        //f 代表错误
        return 'f';
    }
    static std::string UrlDecode(const std::string& url, bool is_convert_space) {
        //遇到了% 将后面的两个字符换成数字，第一个数字<<4（*16)，加上第二个数字
        std::string res;
        for (int i = 0; i < url.size(); ++i) {
                if (url[i] == '%') {
                    if (url[i] == '+' && is_convert_space) {
                    res += ' ';
                    continue;
                }
                //需要解码[i+1, i+2]位置
                char c1 = HTOI(url[i + 1]);
                char c2 = HTOI(url[i + 2]);
                if (c1 != 'f' && c2 != 'f') {
                    char v = (c1 << 4) + c2;
                    res += v;
                    i += 2;
                    continue;
                }else {
                    ERR_LOG("URLDECODE [%s] ERROR", url.c_str());
                    abort();
                }
            }
            res += url[i];

        }
        return res;
    }
    static std::string StatuDesc(int statu) {
        std::unordered_map<int, std::string> _statu_map = {
            {100,  "Continue"},
            {101,  "Switching Protocol"},
            {102,  "Processing"},
            {103,  "Early Hints"},
            {200,  "OK"},
            {201,  "Created"},
            {202,  "Accepted"},
            {203,  "Non-Authoritative Information"},
            {204,  "No Content"},
            {205,  "Reset Content"},
            {206,  "Partial Content"},
            {207,  "Multi-Status"},
            {208,  "Already Reported"},
            {226,  "IM Used"},
            {300,  "Multiple Choice"},
            {301,  "Moved Permanently"},
            {302,  "Found"},
            {303,  "See Other"},
            {304,  "Not Modified"},
            {305,  "Use Proxy"},
            {306,  "unused"},
            {307,  "Temporary Redirect"},
            {308,  "Permanent Redirect"},
            {400,  "Bad Request"},
            {401,  "Unauthorized"},
            {402,  "Payment Required"},
            {403,  "Forbidden"},
            {404,  "Not Found"},
            {405,  "Method Not Allowed"},
            {406,  "Not Acceptable"},
            {407,  "Proxy Authentication Required"},
            {408,  "Request Timeout"},
            {409,  "Conflict"},
            {410,  "Gone"},
            {411,  "Length Required"},
            {412,  "Precondition Failed"},
            {413,  "Payload Too Large"},
            {414,  "URI Too Long"},
            {415,  "Unsupported Media Type"},
            {416,  "Range Not Satisfiable"},
            {417,  "Expectation Failed"},
            {418,  "I'm a teapot"},
            {421,  "Misdirected Request"},
            {422,  "Unprocessable Entity"},
            {423,  "Locked"},
            {424,  "Failed Dependency"},
            {425,  "Too Early"},
            {426,  "Upgrade Required"},
            {428,  "Precondition Required"},
            {429,  "Too Many Requests"},
            {431,  "Request Header Fields Too Large"},
            {451,  "Unavailable For Legal Reasons"},
            {501,  "Not Implemented"},
            {502,  "Bad Gateway"},
            {503,  "Service Unavailable"},
            {504,  "Gateway Timeout"},
            {505,  "HTTP Version Not Supported"},
            {506,  "Variant Also Negotiates"},
            {507,  "Insufficient Storage"},
            {508,  "Loop Detected"},
            {510,  "Not Extended"},
            {511,  "Network Authentication Required"}
        };
        auto it = _statu_map.find(statu);
        if (it != _statu_map.end()) {
            return it->second;
        }
        return "Unkown statu";
    }
    //根据文件后缀名获取文件mime (extention_mime)
    static std::string ExtMime(const std::string& filename) {
        std::unordered_map<std::string, std::string> _mime_map = {
            {".aac",        "audio/aac"},
            {".abw",        "application/x-abiword"},
            {".arc",        "application/x-freearc"},
            {".avi",        "video/x-msvideo"},
            {".azw",        "application/vnd.amazon.ebook"},
            {".bin",        "application/octet-stream"},
            {".bmp",        "image/bmp"},
            {".bz",         "application/x-bzip"},
            {".bz2",        "application/x-bzip2"},
            {".csh",        "application/x-csh"},
            {".css",        "text/css"},
            {".csv",        "text/csv"},
            {".doc",        "application/msword"},
            {".docx",       "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
            {".eot",        "application/vnd.ms-fontobject"},
            {".epub",       "application/epub+zip"},
            {".gif",        "image/gif"},
            {".htm",        "text/html"},
            {".html",       "text/html"},
            {".ico",        "image/vnd.microsoft.icon"},
            {".ics",        "text/calendar"},
            {".jar",        "application/java-archive"},
            {".jpeg",       "image/jpeg"},
            {".jpg",        "image/jpeg"},
            {".js",         "text/javascript"},
            {".json",       "application/json"},
            {".jsonld",     "application/ld+json"},
            {".mid",        "audio/midi"},
            {".midi",       "audio/x-midi"},
            {".mjs",        "text/javascript"},
            {".mp3",        "audio/mpeg"},
            {".mpeg",       "video/mpeg"},
            {".mpkg",       "application/vnd.apple.installer+xml"},
            {".odp",        "application/vnd.oasis.opendocument.presentation"},
            {".ods",        "application/vnd.oasis.opendocument.spreadsheet"},
            {".odt",        "application/vnd.oasis.opendocument.text"},
            {".oga",        "audio/ogg"},
            {".ogv",        "video/ogg"},
            {".ogx",        "application/ogg"},
            {".otf",        "font/otf"},
            {".png",        "image/png"},
            {".pdf",        "application/pdf"},
            {".ppt",        "application/vnd.ms-powerpoint"},
            {".pptx",       "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
            {".rar",        "application/x-rar-compressed"},
            {".rtf",        "application/rtf"},
            {".sh",         "application/x-sh"},
            {".svg",        "image/svg+xml"},
            {".swf",        "application/x-shockwave-flash"},
            {".tar",        "application/x-tar"},
            {".tif",        "image/tiff"},
            {".tiff",       "image/tiff"},
            {".ttf",        "font/ttf"},
            {".txt",        "text/plain"},
            {".vsd",        "application/vnd.visio"},
            {".wav",        "audio/wav"},
            {".weba",       "audio/webm"},
            {".webm",       "video/webm"},
            {".webp",       "image/webp"},
            {".woff",       "font/woff"},
            {".woff2",      "font/woff2"},
            {".xhtml",      "application/xhtml+xml"},
            {".xls",        "application/vnd.ms-excel"},
            {".xlsx",       "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
            {".xml",        "application/xml"},
            {".xul",        "application/vnd.mozilla.xul+xml"},
            {".zip",        "application/zip"},
            {".3gp",        "video/3gpp"},
            {".3g2",        "video/3gpp2"},
            {".7z",         "application/x-7z-compressed"}
        };
        //abc.b.b.txt
        //根据文件获取拓展名
        size_t pos = filename.find_last_of('.');
        if (pos == std::string::npos) {
            return "application/octet-stream"; //二进制流
        }
        std::string ext = filename.substr(pos);
        //根据拓展名获取mime
        auto it = _mime_map.find(ext);
        if (it == _mime_map.end()) {
            return "application/octet-stream";
        }
        return it->second;
    }   
int main() {
    /*test1*/
    /*
    std::string str = "abc,efg,hij,aaacdbe,,,";
    std::vector<std::string> array;
    Split(str, ",", array);
    for(auto & s : array) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    */

    /*test2*/
    /*
    std::string buf;
    bool ret = ReadFile("./eventfd.c", buf);
    if (ret == false) {
        return false;
    }
    // std::cout << buf.c_str() << std::endl;
    ret = WriteFile("./test_write.c", buf);
    if (ret == false) {
        return false;
    }
    buf.clear();
    ret = ReadFile("./test_write.c", buf);
    if (ret == false) {
        return false;
    }
    std::cout << buf.c_str()  << std::endl;
    //md5sum 可以来判断文件内容是否完全相同，如果完全相同md5值是完全相同的
    */

   /*test3*/
   /*
    std::string str = "+";
    std::string res;
    for(auto& c : str) {
        char tmp[4] = {0};
        //%X 表示输出大写十六进制
        //%02X表示如果不够2位则占位，占位填充0
        snprintf(tmp, 4, "%%%02X", c);
        std::cout << tmp << std::endl;
    }
    */

   /*test4*/
    /*
    std::string str = "/login?user=amor7788&pass=amor9911xx++   ";
    // std::string str = "c++  ";
    std::string res = UrlEncode(str, true);
    std::cout << res << ']' << std::endl;
    std::string tmp = UrlDecode(res, true);
    std::cout << tmp << ']' << std::endl;
    */

   std::cout << ExtMime("a.png") << std::endl;

    return 0;
}