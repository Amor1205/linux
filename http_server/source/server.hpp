#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <functional>
#include <sys/epoll.h>
#include <unordered_map>
#define INF 0
#define DBG 1
#define ERR 2
#define LOG_LEVEL DBG
#define LOG(level, format, ...) do{\
	if(level < LOG_LEVEL) break;\
	time_t t = time(nullptr);\
	struct tm* ltm = localtime(&t);\
	char tmp[32] = {0};\
	strftime(tmp, 31, "%H:%M:%S", ltm);\
	fprintf(stdout, "[%p %s %s:%d] " format "\n", (void*)pthread_self(), tmp, __FILE__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define INF_LOG(format, ...) LOG(INF, format, ##__VA_ARGS__)
#define DBG_LOG(format, ...) LOG(DBG, format, ##__VA_ARGS__)
#define ERR_LOG(format, ...) LOG(ERR, format, ##__VA_ARGS__)




const uint64_t BUFFER_DEFAULT_SIZE = 1024;
class Buffer{
private:
    std::vector<char> _buffer; //使用vector 进行内存空间管理
    uint64_t _reader_idx; //读偏移
    uint64_t _writer_idx; //写偏移

public:
    Buffer() :_reader_idx(0), _writer_idx(0), _buffer(BUFFER_DEFAULT_SIZE){}
    //对buffer 的起始地址做封装
    char* Begin() {return &*_buffer.begin();}
    //获取当前写入起始地址
    //_buffer 空间起始地址 + 写偏移量
    char* WritePosition(){ return Begin() + _writer_idx;}
    //获取当前读取起始地址
    char* ReadPosition(){ return Begin() + _reader_idx;}

    // _buffer : [ ... _reader_idx .. _writer_idx ...]
    //获取前沿空闲空间大小（读偏移之前
    uint64_t HeadIdleSize() {return _reader_idx;}
    //获取后续空闲空间大小（写偏移之后
    uint64_t TailIdleSize() { return _buffer.size() - _writer_idx;}
    //获取可读数据大小
    uint64_t ReadAbleSize() { return _writer_idx - _reader_idx;}
    //将读偏移向后移动
    void MoveReadOffset(uint64_t len) {
        //向后移动的大小必须小于可读空间大小
        assert(len <= ReadAbleSize());
        _reader_idx += len;
    }
    //将写偏移向后移动
    void MoveWriteOffset(uint64_t len){
        //向后移动的大小必须小于当前后边的空闲大小。
        assert(len <= TailIdleSize());
        _writer_idx += len;
    }
    //确保可写空间足够（整体空闲空间足够：移动，否则：扩容）
    void EnsureWriteSpaceEnough(uint64_t len){
        //如果末尾空闲空间足够，直接返回；
        if(TailIdleSize() >= len) return;
        //如果不够，看加上前面的空闲空间大小后是否足够,够了就将数据移动到起始位置。
        if(len <= TailIdleSize() + HeadIdleSize()){
            //将数据移动到起始位置
            uint64_t rsz = ReadAbleSize(); //把数据大小先保存起来。
            //把可读数据拷贝到起始位置
            std::copy(ReadPosition(), ReadPosition() + rsz, Begin()); 
            _reader_idx = 0; //读偏移归0
            _writer_idx = rsz; //写偏移置为rsz
        }else{
            //总体空间不够，需要扩容，不移动数据，直接给写偏移之后扩容足够的空间即可。
            _buffer.resize(_writer_idx + len);
        }
    }
    //写入数据
    void Write(const void* data, uint64_t len){
        //确保足够空间
        //拷贝数据进缓冲区
        EnsureWriteSpaceEnough(len);
        const char* d = (const char*) data;
        std::copy(d, d+len, WritePosition());
    }
    void WriteString(const std::string& data){
        return Write(&data[0], data.size());
    }
    //读取数据
    void WriteBuffer(Buffer& data){
        return Write(data.ReadPosition(),data.ReadAbleSize());
    }
    void Read(void* buf, uint64_t len){
        //要求获取数据大小必须小于可读数据大小
        assert(len <= ReadAbleSize());
        //ReadPosition() 是void*，+len是没有办法确定步长，需要强转
        //我们在这里直接修改ReadPosition 及其相关接口的返回类型为char* 方便偏移
        //copy 要求第三个参数是char*类型
        std::copy(ReadPosition(), ReadPosition() + len, (char*)buf);
    }
    std::string ReadAsString(uint64_t len){
        assert(len <= ReadAbleSize());
        std::string str;
        str.resize(len);
        //不可以用str.c_str()，因为这个的返回值类型是const char* 无法修改
        Read(&str[0], len);
        return str;
    }

    //读取和弹出同时进行
    void ReadAndPop(void* buf, uint64_t len){
        Read(buf, len);
        MoveReadOffset(len);
    }
    std::string ReadAsStringAndPop(uint64_t len){
        assert(len <= ReadAbleSize());
        std::string str = ReadAsString(len);
        MoveReadOffset(len);
        return str;
    }
    void WriteAndPush(void* data, uint64_t len){
        Write(data, len);
        MoveWriteOffset(len);
    }
    void WriteStringAndPush(std::string& data){
        WriteString(data);
        MoveWriteOffset(data.size());
    }
    void WriteBufferAndPush(Buffer& data){
        WriteBuffer(data);
        MoveWriteOffset(data.ReadAbleSize());
    }
    //获取一行
    char* FindCRLF(){
        //字符查找
        void* res = memchr(ReadPosition(), '\n', ReadAbleSize());
        return (char*)res;
        //找到了返回地址，找不到返回空
    }
    /*通常获取一行数据，这种情况一般针对ASCII码，当作字符串返回没有问题*/
    std::string GetLine(){
        char* pos = FindCRLF();
        if(pos == nullptr) return "";
        //pos 是找到的换行符，减去可读的起始位置，+1是为了把换行符也读取出来
        return ReadAsString(pos - ReadPosition() + 1);
    }
    std::string GetLineAndPop(){
        std::string str = GetLine();
        MoveReadOffset(str.size());
        return str;
    }
    //清理功能:偏移量归0
    void Clear(){ _reader_idx = 0; _writer_idx = 0;}
};
#define MAX_LISTEN 1024
class Socket{
private:
    int _sockfd;

public:
    Socket() :_sockfd(-1){}
    Socket(int fd) :_sockfd(fd){}
    ~Socket() {Close();}
    int Fd() {return _sockfd;}
    bool CreateFd(){
        //int socket(int domain, int type, int protocol);
        _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd < 0){
            ERR_LOG("CREATE SOCKET FAILED\n");
            return false;
        }
        return true;
    }
    bool Bind(const std::string& ip, uint16_t port){
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        //int bind(int sockfd, struct sockaddr* addr, socklen_t len)
        int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
        if(ret < 0){
            ERR_LOG("BIND ADDRESS FAILED!\n");
            return false;
        }
        return true;
    }
    bool Listen(int backlog = MAX_LISTEN){
        //int listen(int backlog)
        //限制同一时间的并发连接数
        int ret = listen(_sockfd, backlog);
        if(ret < 0){
            ERR_LOG("SOCKET LISTEN FAILED");
            return false;
        }
        return true;
    }
    bool Connect(const std::string& ip, uint16_t port){
        // int connect(int sockfd, struct sockaddr* addr, socklen_t len);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        //int bind(int sockfd, struct sockaddr* addr, socklen_t len)
        int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
        if(ret < 0){
            ERR_LOG("CONNECT SERVER FAILED!\n");
            return false;
        }
        return true;
    }
    //获取文件描述符，而不是socket
    int Accept(){
        //int accept(int sockfd, struct sockaddr* addr, socklen_t* len);
        int newfd = accept(_sockfd, nullptr, nullptr);
        if (newfd < 0){
            ERR_LOG("SOCKET ACCEPT FAILED!\n");
            return -1;
        }
        return newfd;
    }
    //有符号长整型:ssize_t, flag用来设置是否为阻塞操作
    ssize_t Recv(void* buf, size_t len, int flag = 0){
        //ssize_t recv(int sockfd, void* buf, size_t len, int flag);
        ssize_t ret = recv(_sockfd, buf, len, flag);
        if (ret <= 0){
            if (errno == EAGAIN || errno == EINTR){
                return 0; //表示这次接受没有接收到数据
                //EAGAIN ： socket 接收缓冲区没有数据了，只在非阻塞情况下有这个报错
                //EINTR ： 当前socket阻塞等待被信号打断了
            }
            ERR_LOG("RECV FAILED!\n");
            return -1;
        }
        return ret;//实际接收到的数据长度
    }
    //
    ssize_t Send(const void* buf, size_t len, int flag = 0){
        //ssize_t send (int sockfd, void* data, size_t len, int flag);
        ssize_t ret = send(_sockfd, buf, len ,flag);
        if(ret < 0){
            ERR_LOG("SOCKET SEND FAILED!\n");
            return -1;
        }
        return ret; //实际发送的数据长度
        //有可能没发送完
    }
    ssize_t NonBlockRecv(void* buf, size_t len){
        return Recv(buf, len, MSG_DONTWAIT); 
        //MSG_DONTWAIT:当前接收为非阻塞，没数据就会报错返回
    }
    ssize_t NonBlockSend(void* buf, size_t len){
        return Send(buf, len, MSG_DONTWAIT);
    }
    void Close(){
        if(_sockfd != -1){
            close(_sockfd);
            _sockfd = -1;
        }
    }
    //创建服务器不用了ip，默认是0.0.0.0
    bool CreateServer(uint16_t port, const std::string& ip = "0.0.0.0", bool flag = false){
        //1.创建套接字 1.5 设置非阻塞 2.绑定地址 3.开始监听 4.启动地址重用
        if (CreateFd() == false) return false;
        if (flag) NonBlock(); //flag false时不设置非阻塞
        if (Bind(ip, port) == false) return false;
        if (Listen() == false) return false;
        ReuseAddress();
        return true;
    }
    bool CreateClient(uint16_t port, const std::string& ip){
        //1.创建套接字 2.指向连接服务器
        if (CreateFd() == false) return false;
        if (Connect(ip, port) == false) return false;
        return true;
    }
    void ReuseAddress(){
       //int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
       //int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
       int val = 1;
       setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&val, sizeof(int));
       val = 1;
       setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, (void*)&val, sizeof(int));
    }
    void NonBlock(){
        //int fcntl(int fd, int cmd, ... /* arg */ );
        //先获取再设置
        int flag = fcntl(_sockfd, F_GETFL, 0);
        fcntl(_sockfd, F_SETFL, flag | O_NONBLOCK);
    }
};
class Poller;
class Channel{
private:
    int _fd;
    Poller* _poller;
    uint32_t _events; //当前需要监控的事件
    uint32_t _revents; //当前连接触发的事件
    // using EventCallBack = std::function<void(Connection*)>
    using EventCallBack = std::function<void()>;
    EventCallBack _read_callback; //可读事件被触发的回调函数
    EventCallBack _write_callback;//可写事件被触发的回调函数
    EventCallBack _error_callback;//错误事件被触发的回调函数
    EventCallBack _close_callback;//连接断开事件被触发的回调函数
    EventCallBack _event_callback;//任意事件被触发的回调函数
public:
    Channel(Poller* poller, int fd) :_fd(fd), _events(0), _revents(0), _poller(poller){}
    int Fd(){return _fd;}
    //设置实际就绪的事件
    void SetREvents(uint32_t events) {_revents = events;}
    //获取想要监控的事件
    uint32_t Events() {return _events; }
    //当前是否监控了可读
    bool ReadAble(){ return (_events & EPOLLIN); }
    //当前是否监控了可写
    bool WriteAble(){ return (_events & EPOLLOUT); }
    
    //移除监控（从epoll里取出）
    void Remove();
    void Update();
    //启动读事件监控
    void EnableRead(){ _events |= EPOLLIN; Update(); }
    //启动写事件监控
    void EnableWrite(){ _events |= EPOLLOUT; Update(); }
    //关闭读事件监控
    void DisableRead(){ _events &= ~EPOLLIN; Update(); }
    //关闭写事件监控
    void DisableWrite(){ _events &= ~EPOLLOUT; Update(); }
    //关闭所有事件监控 
    void DisableAll(){ _events = 0; Update();}

    //设置回调函数
    void SetReadCallBack(const EventCallBack& cb) { _read_callback = cb; }
    void SetWriteCallBack(const EventCallBack& cb){ _write_callback = cb; }
    void SetErrorCallBack(const EventCallBack& cb){ _error_callback = cb; }
    void SetCloseCallBack(const EventCallBack& cb){ _close_callback = cb; }
    void SetEventCallBack(const EventCallBack& cb){ _event_callback = cb; }
    //事件处理，一旦链接出触发事件就调用这个函数，自己触发什么事件如何处理由组件使用者自己定。
    void HandleEvent(){
        if ((_revents & EPOLLIN) || (_revents & EPOLLRDHUP) || (_revents & EPOLLPRI)){
            if (_event_callback) _event_callback();
            if (_read_callback) _read_callback();
        }
        //有可能会释放连接的操作，一次只能处理一个
        if (_revents & EPOLLOUT){
            if (_event_callback) _event_callback();
            if (_write_callback) _write_callback();
        }
        //一旦出错，就会释放连接，因此要放到前面调用任意回调
        else if (_revents & EPOLLERR){
            if (_error_callback) _error_callback(); 
            if (_event_callback) _event_callback();
        }
        else if (_revents & EPOLLHUP){
            if (_close_callback) _close_callback();
            if (_event_callback) _event_callback();
        }
    }   
};

#define MAX_EPOLLEVENTS 1024
class Poller{
private:
    int _epfd;
    struct epoll_event _evs[MAX_EPOLLEVENTS];
    std::unordered_map<int, Channel*> _channels;
private:
    //对epoll的直接操作，第一层封装
    void Update(Channel* channel, int opt){
        //int epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);
        int fd = channel->Fd();
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = channel->Events();
        int ret = epoll_ctl(_epfd, opt, fd, &ev);
        if(ret < 0){
            ERR_LOG("EPOLL_CTL FAILED!\n");
            abort();//异常退出
        }
        return;
    }
    //判断channel 是否已经添加到事件监控中
    bool HasChannel(Channel* channel){
        auto it = _channels.find(channel->Fd());
        if (it == _channels.end()){
            return false;
        }
        return true;
    }
public:
    Poller() {
        _epfd = epoll_create(MAX_EPOLLEVENTS);
        if(_epfd < 0){
            ERR_LOG("EPOLL CREATE FAILED!\n");
            abort();//退出程序
        }
    }
    //channel 内又有操作符，又有要操作的事件，所以传channel
    //修改或添加修改事件
    void UpdateEvent(Channel* channel){
        bool ret = HasChannel(channel);
        if(ret == false){
            //不存在则添加
            _channels.insert(std::make_pair(channel->Fd(), channel));
            return Update(channel, EPOLL_CTL_ADD);
        }
        return Update(channel, EPOLL_CTL_MOD);
    }
    //移除监控
    void RemoveEvent(Channel* channel){
        auto it = _channels.find(channel->Fd());
        if (it != _channels.end()) _channels.erase(it);
        Update(channel, EPOLL_CTL_DEL);
    }
    //开始监控，返回活跃连接
    void Poll(std::vector<Channel*> * active){
        //int epoll_wait(int epfd, struct epoll_event* evs, int maxevents, int timeout)
        //-1 :阻塞监控
        int nfds = epoll_wait(_epfd, _evs, MAX_EPOLLEVENTS, -1);
        if (nfds < 0){
            if (errno == EINTR) return; //EINTR 被信号打断了
            ERR_LOG("EPOLL WAIT ERROR:%s\n", strerror(errno));
            abort();//退出程序
        }
        for (int i = 0; i < nfds; ++i){
            auto it = _channels.find(_evs[i].data.fd);
            assert(it != _channels.end());
            it->second->SetREvents(_evs[i].events); //设置实际就绪的事件
            active->push_back(it->second);
        }
        return;
    }
};

//用到了poller 类，需要在poller类后进行定义
void Channel::Remove() { return _poller->RemoveEvent(this);}
void Channel::Update(){ return _poller->UpdateEvent(this);}


class EventLoop{
private:
    using Functor = std::function<void()>;
    std::thread::id _thread_id; //线程id
    int _event_fd; //eventfd 唤醒IO事件监控有可能导致的阻塞
    Poller _poller;
    std::vector<Functor> _tasks;//任务队列（任务池）
    std::mutex _mutex; //实现任务池操作的线程安全
public:
    //逻辑： 事件监控->就绪事件处理->执行任务
    void RunAllTask();
public:
    //判断将要执行的任务是在当前线程中还是不在，如果是则执行，否则压入队列中。
    void RunInLoop(const Functor& cb); 
    //将操作压入队列中
    void QueueInLoop(const Functor& cb);
    //判断当前线程是否是EventLoop
    bool IsInLoop();
    //修改/添加描述符的事件监控
    void UpdateEvent(Channel* channel);
    //移除描述符的监控
    void RemoveEvent(Channel* channel);
    void Start();
};