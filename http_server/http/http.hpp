#include "../source/server.hpp"

class Utility {
public:
    //字符串分割函数
    //将src字符串按照sep字符进行分割，得到的这个子串放到array中，最终返回子串的数量
    static size_t Split(const std::string& src, const std::string& sep, std::vector<std::string>& array) {
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
    //读取文件内容, 将读取的内容放到一个buffer中
    static bool ReadFile(const std::string& filename, std::string& buf) {
        std::ifstream ifs(filename, std::ios::binary);
        if (ifs.is_open() == false) {
            ERR_LOG("OPEN %s FILE ERROR!\n", filename.c_str());
            return false;
        }
        size_t fsize = 0;
        //将文件跳转到指定位置
        ifs.seekg(0, ifs.end);
        fsize = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        buf.resize(fsize);
        ifs.read(&buf[0], fsize);
        if( ifs.good() == false) {
            ERR_LOG("READ %s FILE ERROR!\n", filename.c_str());
            ifs.close();
            return false;
        }
        ifs.close();
        return true;
    }
    //向文件写入数据
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
    //URL编码:避免URL中资源路径与查询字符串中的特殊字符与HTTP请求种特殊字符产生歧义
    //编码格式：将特殊字符的ASCII码值，转换为两个16进制字符，前缀%， C++ -> C%2B%2B （+转为16进制的两位是：2B
    //有不编码的特殊字符：. - _ ~ 以及字母和数字 （RFC3986文档规定的）
    // RFC3986文档规定的： 编码格式%HH
    // W3C 文档中要求，查询字符串中的空格，必须编码转化为+，解码+转空额

    // bool is_convert_space表示是否需要转换空格
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
    //URL解码
    static std::string UrlDecode(const std::string& url, bool is_convert_space) {
        //遇到了% 将后面的两个字符换成数字，第一个数字<<4（*16)，加上第二个数字
        std::string res;
        for (int i = 0; i < url.size(); ++i) {
            if (url[i] == '+' && is_convert_space) {
                res += ' ';
                continue;
            }
            if (url[i] == '%' && (i + 2 < url.size())) {
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
    //响应状态码 的描述信息获取
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
    //判断一个文件是否是一个目录
    static bool IsDirectory(const std::string& filename) {
        struct stat st;
        int ret = stat(filename.c_str(), &st);
        if (ret < 0) {
            return false;
        }
        return S_ISDIR(st.st_mode);
    }
    //判断一个文件是否是一个普通文件
    static bool IsRegular(const std::string& filename) {
        struct stat st;
        int ret = stat(filename.c_str(), &st);
        if (ret < 0) {
            return false;
        }
        return S_ISREG(st.st_mode);
    }
    //HTTP请求的资源路径有效性判断
    // /index.html -- 前面的叫做相对根目录，映射的事某个服务器上的子目录
    // 想表达的意思就是客户端只能请求相对根目录中的资源，其他地方的资源都不予理会
    // /../login，这个路径中的..会让路径的查找跑到相对根目录之外，不合理而且不安全
    static bool ValidPath(const std::string& path) {
        //思想：按照/进行路径分割，根据有多少子目录，计算目录深度，有多少层。
        int level = 0;
        std::vector<std::string> subdir;
        Split(path, "/", subdir);
        for (auto& dir : subdir) {
            if (dir == "..") {
                level--;
                if (level < 0) {
                    return false;
                }
            }
            else {
                level++;
            }
        }
        return true;
    }
};

class HttpRequest {
public:
    std::string _method; //请求方法 GET / POST
    std::string _path;   //请求资源路径
    std::string _version;//HTTP协议版本 HTTP/1.1
    std::string _body;   //请求正文
    std::string _matches;//资源路径的正则提取数据
    std::unordered_map<std::string, std::string> _headers; //头部字段
    std::unordered_map<std::string, std::string> _params;  //查询字符串
public:
    //插入头部字段
    void SetHeader(const std::string& key, const std::string& val) {
        _headers.insert({key, val});
    }
    //判断是否存在指定头部字段
    bool HasHeader(const std::string& key) {
        auto it = _headers.find(key);
        if (it == _headers.end()) {
            return false;
        }
        return true;
    }
    //获取指定头部字段值
    std::string GetHeader(const std::string& key) {
        auto it = _headers.find(key);
        if (it == _headers.end()) {
            return "";
        }
        return it->second;
    }
    //插入查询字符串
    void SetParam(const std::string& key, const std::string& val) {
        _params.insert({key, val});
    }
    //判断是否存在指定查询字符串
    bool HasParam(const std::string& key) {
        auto it = _params.find(key);
        if (it == _params.end()) {
            return false;
        }
        return true;
    }
    //获取指定查询字符串
    std::string GetParam(const std::string& key) {
        auto it = _params.find(key);
        if (it == _params.end()) {
            return "";
        }
        return it->second;
    }
    //获取正文长度
    size_t ContentLength() {
        //Content-Length :123456\r\n
        bool ret = HasHeader("Content-Length");
        if (ret == false) {
            return 0;
        }
        return std::stol(GetHeader("Content-Length"));
    }
    //判断是否为长链接 长-true 短-false
    bool IsPersistentConnection() {
        //没有Connection字段或者有但是设置值为close 
        bool ret = HasHeader("Connection") && GetHeader("Connection") == "keep-alive";
        if (ret == true) {
            return true;
        }
        return false;
    }
};

class HttpResponse {
private:
    int _statu;
    bool _redirect_flag;
    std::string _redirect_url;
    std::string _body;
    std::unordered_map<std::string, std::string> _headers;
public:
    HttpResponse() :_redirect_flag(false), _statu(200) {}
    HttpResponse(int statu) :_redirect_flag(false), _statu(statu) {}
    //重置
    void Reset() {
        _statu = 200;
        _redirect_flag = false;
        _redirect_url.clear();
        _body.clear();
        _headers.clear();
    }
    //插入头部字段
    void SetHeader(const std::string& key, const std::string& val) {
        _headers.insert({key, val});
    }
    //判断是否存在指定头部字段
    bool HasHeader(const std::string& key) {
        auto it = _headers.find(key);
        if (it == _headers.end()) {
            return false;
        }
        return true;
    }
    //获取指定头部字段值
    std::string GetHeader(const std::string& key) {
        auto it = _headers.find(key);
        if (it == _headers.end()) {
            return "";
        }
        return it->second;
    }
    void SetContent(std::string& body, std::string& type) {
        _body = body;
        SetHeader("Content-Type", type);
    }
    //设置重定向
    void SetRedirect(std::string& new_url, int statu = 302) {
        _statu = statu;
        _redirect_flag = true;
        _redirect_url = new_url;
    }
    //判断是否为长连接 是 - true
    bool IsPersistentConnection() {
        //没有Connection字段或者有但是设置值为close 
        bool ret = HasHeader("Connection") && GetHeader("Connection") == "keep-alive";
        if (ret == true) {
            return true;
        }
        return false;
    }
};
typedef enum {
    RECV_HTTP_ERROR,
    RECV_HTTP_LINE,
    RECV_HTTP_HEAD,
    RECV_HTTP_BODY,
    RECV_HTTP_OVER
}HttpRecvStatu;

#define MAX_LINE_SIZE 8192
class HttpContext {
private:
    int _resp_statu; //响应状态码
    HttpRecvStatu _recv_statu; //当前接收和解析的状态
    HttpRequest _request; //已经解析得到的请求信息
private:
    //接收http请求行
    bool RecvHttpLine(Buffer* buf) {
        if (_recv_statu != RECV_HTTP_LINE) {
            return false;
        }
        //1. 获取一行数据
        //但是需要考虑：缓冲区的数据不足一行，获取的一行数据量非常大
        
        std::string line = buf->GetLine();
        if (line.size() == 0) {
            //缓冲区的数据不足一行，需要判断缓冲区的可读数据长度
            //如果很长了都不足一行，有问题
            if (buf->ReadAbleSize() > MAX_LINE_SIZE) {
                _recv_statu = RECV_HTTP_ERROR;
                _resp_statu = 414; //URI TOO LONG
                return false;
            }
            //缓冲区的数据不足一行，但是也不够多，等到新的数据的到来
            return true;
        }
        if (line.size() > MAX_LINE_SIZE) {
            _recv_statu = RECV_HTTP_ERROR;
            _resp_statu = 414; //URI TOO LONG
            return false;
        }
        bool ret = ParseHttpLine(line);
        if(ret == false) {
            return false;
        }
        buf->MoveReadOffset(line.size());
        //首行处理完毕，进入头部获取阶段
        _recv_statu = RECV_HTTP_HEAD;
        return true;
    }
    //解析Http请求行
    bool ParseHttpLine(const std::string& line) {
        std::smatch matches;
        std::regex e("(GET|HEAD|POST|PUT|DELETE) ([^?]*)(?:\\?(.*)) (HTTP/1\\.[01])(?:\n|\r\n)?");
        bool ret = std::regex_match(line, matches, e);
        if(ret == false)
        {
            _recv_statu = RECV_HTTP_ERROR;
            _resp_statu = 400; //bad request
            return false;
        }
        //第0个：url本身
        //第1个：请求方法
        //第2个：资源路径
        //第3个：查询字符串
        //第4个：协议版本

        //获取请求方法
        _request._method = matches[1];
        //获取资源路径，需要进行URL解码操作，但是不用+转space
        _request._path = Utility::UrlDecode(matches[2], false);
        //获取协议版本
        _request._version = matches[4];
        //查询字符串的获取和处理
        std::string query_string = matches[3];  
        std::vector<std::string> query_string_array;
        //按照等号分割字符串
        //查询字符串格式： key=val&key=val ....
        //先& 进行分割，针对各个子串 以=进行分割，得到后也需要进行url解码
        Utility::Split(query_string, "&", query_string_array);
        for(auto& s : query_string_array) { 
            //按照等号来进行分割
            size_t pos = str.find("=");
            if (pos == std::string::npos) {
                _recv_statu = RECV_HTTP_ERROR;
                _resp_statu = 400; //bad request
                return false;
            }
            //
            std::string key = Utility::UrlDecode(s.substr(0, pos), true);
            std::string val = Utility::UrlDecode(s.substr(pos + 1), true);
            _request.SetParam(key, val);
        }
        return true;
    }
    //接收http头部
    bool RecvHttpHead(Buffer* buf) {
        if (_recv_statu != RECV_HTTP_HEAD) {
            return false;
        }
        //一行一行的取出数据，直到遇到空行为止，头部的格式 key: val\r\nkey: val\r\n...
        while (1) {
            std::string line = buf->GetLine();
            if (line.size() == 0) {
                //缓冲区的数据不足一行，需要判断缓冲区的可读数据长度
                //如果很长了都不足一行，有问题
                if (buf->ReadAbleSize() > MAX_LINE_SIZE) {
                    _recv_statu = RECV_HTTP_ERROR;
                    _resp_statu = 414; //URI TOO LONG
                    return false;
                }
                //缓冲区的数据不足一行，但是也不够多，等到新的数据的到来
                return true;
            }
            if (line.size() > MAX_LINE_SIZE) {
                _recv_statu = RECV_HTTP_ERROR;
                _resp_statu = 414; //URI TOO LONG
                return false;
            }
            if (line == "\n" || line == "\r\n") {
                break;
            }
            bool ret = ParseHttpHead(line);
            if (ret == false) {
                return false;
            }
        }
        _recv_statu = RECV_HTTP_BODY;
        return true;
    }
    //解析http头部
    bool ParseHttpHead(const std::string& line) {
        //key: val
        size_t pos = line.find(": ");
        if (pos == std::string::npos) {
            _recv_statu = RECV_HTTP_ERROR;
            _resp_statu = 400; //bad request
            return false;
        }
        std::string key = s.substr(0, pos);
        std::string val = s.substr(pos + 2);
        _request.SetHeader(key, val);
    }
    //接收http正文
    bool RecvHttpBody(Buffer* buf) {
        if (_recv_statu != RECV_HTTP_BODY) {
            return false;
        }  
        //1. 获取正文长度
        size_t content_length = _request.ContentLength();
        if (content_length == 0) {
            //无正文，请求接收解析完毕
            _recv_statu = RECV_HTTP_OVER;
            return true;
        }
        //2. 当前已经接收到了多少正文 其实就是往_request._body 里放了多少数据，
        // 其实就是_request._body.size()
        size_t content_len_wait_recv = content_length - _request._body.size();
        //3. 接收正文放到body中，也要考虑当前缓冲区的数据是否是全部正文
        //  3.1 缓冲区中的数据，如果包含了当前数据的所有正文，取出所需数据
        if (buf->ReadAbleSize() >= content_len_wait_recv) {
            _request._body.append(buf->ReadPosition(), content_len_wait_recv);
            buf->MoveReadOffset(content_len_wait_recv);
            _recv_statu = RECV_HTTP_OVER;
            return true;
        }
        //  3.2 缓冲区的数据，若无法满足当前正文需要，数据不足，取出全部数据，等待新数据到来
        _request._body.append(buf->ReadPosition(), buf->ReadAbleSize());
        buf->MoveReadOffset(buf->ReadAbleSize());
        //不改状态，直接返回
        return true;
    }
public:
    HttpContext() :_resp_statu(200), _recv_statu(RECV_HTTP_LINE) {}
    int ResponseStatu();
    HttpRecvStatu RecvStatu();
    HttpRequest& Request(); //
    void RecvRequest(); //接收并解析HTTP请求
};