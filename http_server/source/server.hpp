#ifndef __SERVER_H__
#define __SERVER_H__
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <cstring>
#include <typeinfo>
#include <cassert>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <ctime>   
#include <cstdlib>
#include <functional>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define INF 0
#define DBG 1
#define ERR 2
#define LOG_LEVEL ERR
#define LOG(level, format, ...) do{\
	if(level < LOG_LEVEL) break;\
	time_t t = time(NULL);\
	struct tm* ltm = localtime(&t);\
	char tmp[32] = {0};\
	strftime(tmp, 31, "%H:%M:%S", ltm);\
	fprintf(stdout, "[%p %s %s:%d] " format "\n", (void*)pthread_self(), tmp, __FILE__, __LINE__, ##__VA_ARGS__);\
    }while(0)

#define INF_LOG(format, ...) LOG(INF, format, ##__VA_ARGS__)
#define DBG_LOG(format, ...) LOG(DBG, format, ##__VA_ARGS__)
#define ERR_LOG(format, ...) LOG(ERR, format, ##__VA_ARGS__)




#define BUFFER_DEFAULT_SIZE 1024
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
        if(len == 0) return;
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
            DBG_LOG("RESIZE %ld", _writer_idx + len);
            _buffer.resize(_writer_idx + len);
        }
    }
    //写入数据
    void Write(const void* data, uint64_t len){
        if (len == 0) return;
        //确保足够空间
        //拷贝数据进缓冲区
        EnsureWriteSpaceEnough(len);
        const char* d = (const char*) data;
        std::copy(d, d + len, WritePosition());
    }
    void WriteString(const std::string& data){
        return Write(&data[0], data.size());
    }
    //读取数据
    void WriteBuffer(Buffer& data){
        return Write(data.ReadPosition(),data.ReadAbleSize());
    }
    void WriteAndPush(const void* data, uint64_t len){
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
    ~Socket() { Close(); }
    int Fd() { return _sockfd; }
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
            ERR_LOG("SOCKET RECV FAILED!\n");
            return -1;
        }
        return ret;//实际接收到的数据长度
    }

    ssize_t Send(const void* buf, size_t len, int flag = 0){
        //ssize_t send (int sockfd, void* data, size_t len, int flag);
        ssize_t ret = send(_sockfd, buf, len ,flag);
        if(ret < 0){
            if (errno == EAGAIN || errno == EINTR) { 
                return 0; 
            }
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
        if (len == 0) return 0;
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
class EventLoop;
class Channel{
private:
    int _fd;
    EventLoop* _loop;
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
    Channel(EventLoop* loop, int fd) :_fd(fd), _events(0), _revents(0), _loop(loop){}
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
            if (_read_callback) _read_callback();
        }
        //有可能会释放连接的操作，一次只能处理一个
        if (_revents & EPOLLOUT){
            if (_write_callback) _write_callback();
        }
        //一旦出错，就会释放连接，因此要放到前面调用任意回调
        else if (_revents & EPOLLERR){
            if (_error_callback) _error_callback(); 
        }
        else if (_revents & EPOLLHUP){
            if (_close_callback) _close_callback();
        }
        if (_event_callback) _event_callback();
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
        if (it != _channels.end()) {
            _channels.erase(it);
        }
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


//void返回类型的一个函数指针，用来保存执行的定时任务
using TaskFunc = std::function<void()>; 
using ReleaseFunc = std::function<void()>;

class TimerTask{
private:
    uint64_t _id; //定时器任务对象id
    uint32_t _timeout; //定时任务超时时间
    bool _canceled; //false 表示没被取消， true 表示被取消了
    //取消需要在任务里面被取消，因为是提前执行任务了
    TaskFunc _task_cb; //定时器对象要执行的定时任务；
    ReleaseFunc _release; //额外的释放操作，用于删除timerwheel 中保存的定时器对象信息
public:
    //id不是用自己生成，而是使用统一生成，因为自己生成可能会有重复
    TimerTask(uint64_t id, uint32_t delay, const TaskFunc &cb)
        :_id(id),_timeout(delay),_task_cb(cb),_canceled(false)
    {}
    ~TimerTask() 
    {
        if(_canceled == false) _task_cb();
        _release();
    }//析构时调用
    void Cancel() {_canceled = true;}
    void SetRelease(const ReleaseFunc& cb) {_release = cb;}
    uint32_t DelayTime() {return _timeout;}
};

class TimerWheel{
private:
    using WeakTask = std::weak_ptr<TimerTask>;
    using PtrTask = std::shared_ptr<TimerTask>; 
    //只用一个shared_ptr 会产生循环引用无法释放的问题

    int _capacity; //表盘的最大数量 -- 最大延迟时间
    int _tick; //当前的秒针，走到哪里释放哪里(执行这里的的任务)
    std::vector<std::vector<PtrTask>> _wheel;
    std::unordered_map<uint64_t, WeakTask> _timers; 
    EventLoop* _loop;
    int _timerfd; //定时器描述符 -- 可读事件回调就是读取计时器，执行定时任务
    std::unique_ptr<Channel> _timer_channel; //loop需要channel
private:
    void RemoveTimer(uint64_t id)
    {
        auto it = _timers.find(id);
        if(it != _timers.end()) {
            _timers.erase(it);
        }
    }
    static int CreateTimerFd(){
        //int timerfd_create(int clockid, int flags);
        int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
        if(timerfd < 0){
            ERR_LOG("TIMERFD CRAETE FAILED!\n");
            abort();
        }
        struct itimerspec itime;
        itime.it_value.tv_sec = 1;
        itime.it_value.tv_nsec = 0;
        itime.it_interval.tv_sec = 1;
        itime.it_interval.tv_nsec = 0;
        timerfd_settime(timerfd, 0, &itime, nullptr);
        return timerfd;
        //每秒钟触发一次超时
    }
    int ReadTimerFd(){
        uint64_t times;
        //有可能因为其他描述符的事件处理花费时间比较长，然后再处理定时器描述符时间的时候，有可能已经超时了很多次了
        //read读取到的数据times就是从上一次read之后超时的次数
        int ret = read(_timerfd, &times, 8);
        if(ret < 0)
        {
            ERR_LOG("READ TIMERFD FAILED");
            abort();
        }
        return times;
    }
    void OnTime(){
        //接收实际的超时次数
        int times = ReadTimerFd();
        for (int i = 0; i < times; ++i) {
            RunTimerTask();
        }
    }
    //添加定时任务
    void TimerAddInLoop(uint64_t id, uint32_t delay, const TaskFunc &cb)
    {
        PtrTask pt(new TimerTask(id, delay, cb));
        // pt->SetRelease(std::bind(RemoveTimer, this, id));
        // 成员函数不能这么给，需要加类名修饰 + 取地址&
        pt->SetRelease(std::bind(&TimerWheel::RemoveTimer, this, id));
        _timers[id] = WeakTask(pt);
        //tick + delay可能会越界，圆形需要取模
        int pos = (_tick + delay) % _capacity;
        _wheel[pos].push_back(pt);
    }
    void TimerRefreshInLoop(uint64_t id)//刷新/延迟定时任务
    {
        //通过id - weakptr 映射关系找到weakptr， 通过weakptr再实例化一个shared_ptr放入到时间轮当中
        auto it = _timers.find(id);
        if(it == _timers.end()) {
            return; //没找着定时任务，没必要刷新和延迟。
        }
        PtrTask pt = it->second.lock(); //通过lock获取weakptr 管理的对象的对应的sharedptr
        int delay = pt->DelayTime();
        //tick + delay可能会越界，圆形需要取模
        int pos = (_tick + delay) % _capacity;
        _wheel[pos].push_back(pt);
    }
    void TimerCancelInLoop(uint64_t id)
    {
        auto it = _timers.find(id);
        if(it == _timers.end()) {
            return; //没找着定时任务，没必要刷新和延迟。
        }
        PtrTask pt = it->second.lock();
        if (pt) pt->Cancel();
    }
public:
    TimerWheel(EventLoop* loop) :_capacity(60), _tick(0), _wheel(_capacity),_loop(loop), _timerfd(CreateTimerFd()), _timer_channel(new Channel(_loop,_timerfd)){
        _timer_channel->SetReadCallBack(std::bind(&TimerWheel::OnTime, this));
        _timer_channel->EnableRead(); //启动读事件监控
    }
    /*定时器中有一个_timers成员，定时器信息的操作可能在多个线程中进行，因此需要考虑线程安全*/
    /*如果不想加锁，就把定期的所有操作放到一个线程当中去*/
    /*这样就保证了在一个线程中进行，串行操作，保证线程安全和效率*/
    /*因为用到了后面的EventLoop，需要在EventLoop之后进行定义*/
    void TimerAdd(uint64_t id, uint32_t delay, const TaskFunc &cb);
    void TimerRefresh(uint64_t id);
    void TimerCancel(uint64_t id);
    //这个函数每秒钟执行一次，相当于秒针向后走一步
    void RunTimerTask()
    {   
        _tick = (_tick + 1) % _capacity;
        _wheel[_tick].clear(); 
        //清空指定位置的数组，会把数组中保存的所有管理定时器对象的sharedptr释放掉
    }
    //判断是否有一个定时任务，如果先停止过后一段时间再启动，如果有就直接重新启动，如果没有就得重新添加。
    /*存在线程安全问题，不能被外界使用者调用，而是在模块内，对应的eventloop线程内执行*/
    bool HasTimer(uint64_t id){
        auto it = _timers.find(id);
        if(it == _timers.end()) {
            return false; //没找着定时任务
        }
        return true;
    }

};

class EventLoop{
private:
    using Functor = std::function<void()>;
    std::thread::id _thread_id; //线程id
    int _event_fd; //eventfd 唤醒IO事件监控有可能导致的阻塞
    std::unique_ptr<Channel> _event_channel; //指针需要析构的时候被释放，可用智能指针
    Poller _poller; //对所有描述符进行监控
    std::vector<Functor> _tasks;//任务队列（任务池）
    std::mutex _mutex; //实现任务池操作的线程安全
    TimerWheel _timer_wheel; //
public:
    //逻辑： 事件监控->就绪事件处理->执行任务
    void RunAllTask(){
        std::vector<Functor> functor;
        {
            //作用域内加锁，出作用域解锁
            std::unique_lock<std::mutex> _lock(_mutex);
            _tasks.swap(functor);
        }
        //任务执行过程中不需要加锁
        for(auto & f : functor){
            f();
        }
        return;
    }
    static int CreateEventFd(){
        int efd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
        if (efd < 0){
            ERR_LOG("CREATE EVENTFD FAILED!\n");
            abort();//异常退出
        }
        return efd;
    }
    void ReadEventFd(){
        uint64_t res = 0;
        int ret = read(_event_fd, &res, sizeof(res));
        if (res < 0){
            //EINTR 被信号打断 EAGAIN 无数据可读
            if (errno == EINTR || errno == EAGAIN) return;
            ERR_LOG("READ EVENTFD FAILED!\n");
            abort();//异常退出
        }
        return;
    }
    void WeakUpEventFd(){
        uint64_t val = 1;
        int ret = write(_event_fd, &val, sizeof(val));
        if (ret < 0){
            if(errno == EINTR) return;
            ERR_LOG("WRITE EVENTFD FAILED!\n");
            abort();
        }
        return;
    }

public:
    EventLoop()
    :_thread_id(std::this_thread::get_id()),
    _event_fd(CreateEventFd()),
    _event_channel(new Channel(this, _event_fd)),
    _timer_wheel(this)
    {
        //给eventfd 添加可读事件回调函数，读取eventfd 事件通知次数
        _event_channel->SetReadCallBack(std::bind(&EventLoop::ReadEventFd, this));
        //启动event fd的读事件监控
        _event_channel->EnableRead();
    }
    //判断当前线程是否是EventLoop
    bool IsInLoop(){
        return _thread_id == std::this_thread::get_id();
    }
    void AssertInLoop() {
        assert(_thread_id == std::this_thread::get_id());
    }
    //判断将要执行的任务是在当前线程中还是不在，如果是则执行，否则压入队列中。
    void RunInLoop(const Functor& cb){
        if (IsInLoop()){
            return cb();
        }
        return QueueInLoop(cb);
    }
    //将操作压入队列中
    void QueueInLoop(const Functor& cb){
        //如果我把任务压入任务池后，需要唤醒有可能因为没有事件就绪而导致的epoll阻塞
        {
            std::unique_lock<std::mutex> _lock(_mutex);
            _tasks.push_back(cb);
        }
        //其实唤醒就是向eventfd 写入一个数据，eventfd 就会触发可读事件，就不会再阻塞了
        WeakUpEventFd();
    }

    //修改/添加描述符的事件监控
    void UpdateEvent(Channel* channel) {
        return _poller.UpdateEvent(channel);
    }
    //移除描述符的监控
    void RemoveEvent(Channel* channel) {
        return _poller.RemoveEvent(channel);
    }
    //1.事件监控，事件处理，执行任务池
    void Start(){
        while(1){
            std::vector<Channel*>  actives;
            _poller.Poll(&actives);
            for(auto &channel : actives){
                channel->HandleEvent();
            }
            RunAllTask();
        }
    }
    void TimerAdd(uint64_t id, uint32_t delay, const TaskFunc& cb){ 
        return _timer_wheel.TimerAdd(id, delay, cb);
    }
    void TimerRefresh(uint64_t id){
        return _timer_wheel.TimerRefresh(id);
    }
    void TimerCancel(uint64_t id){
        return _timer_wheel.TimerCancel(id);
    }
    //判断是否有一个定时任务，如果先停止过后一段时间再启动，如果有就直接重新启动，如果没有就得重新添加。
    bool HasTimer(uint64_t id){
        return _timer_wheel.HasTimer(id);
    }
};

// //用到了poller 类，需要在poller类后进行定义
// //用到了Loop 类，需要在Loop类后进行定义
void Channel::Remove() { return _loop->RemoveEvent(this);}
void Channel::Update() { return _loop->UpdateEvent(this);}
void TimerWheel::TimerAdd(uint64_t id, uint32_t delay, const TaskFunc &cb){
    _loop->RunInLoop(std::bind(&TimerWheel::TimerAddInLoop,this, id, delay, cb));
}
void TimerWheel::TimerRefresh(uint64_t id){
    _loop->RunInLoop(std::bind(&TimerWheel::TimerRefreshInLoop, this, id));
}

void TimerWheel::TimerCancel(uint64_t id){
    _loop->RunInLoop(std::bind(&TimerWheel::TimerCancelInLoop, this, id));
}


class Any{
private:
    class holder{
    public:
        virtual ~holder(){}
        virtual const std::type_info& type() = 0;
        virtual holder* clone() = 0;
    };
    template<class T>
    class placeholder : public holder{
    public:
        placeholder(const T& val) :_val(val){}

        //获取子类对象保存的数据类型
        virtual const std::type_info& type() {return typeid(T);} 

        //针对当前的对象自身，克隆出一个新的子类对象
        virtual holder* clone() {return new placeholder(_val);}
    public:
        T _val;
    };

    holder* _content;
public:
    Any() :_content(nullptr){}
    //任意类型的构造
    template<class T>
    Any(const T& val) :_content(new placeholder<T>(val)) {}

    Any(const Any& other) 
    :_content(other._content ? other._content->clone() : nullptr){}

    ~Any() {delete _content;}
    Any& swap(Any& other){
        std::swap(_content, other._content);
        return *this;
    }
    //获取/返回 子类对象所保存的数据的指针
    template<class T>
    T* get() {
        //想要获取的数据类型必须要和保存的数据类型一致
        assert(typeid(T) == _content->type());
        // if(typeid(T) != _content->type()) return nullptr;

        //content是父类指针，我们需要强转成子类指针才可以访问_val
        return &((placeholder<T>*)_content)->_val;
    }

    //重载等号运算符
    template<class T>
    Any& operator=(const T& val){
        //为val 构造一个临时的通用容器，然后与当前容器自身进行一个指针的交换。
        //临时对象释放的时候，原先保存的数据也就被释放了。
        Any(val).swap(*this);
        return *this;
    }
    //可能赋值一个其他的通用容器
    Any& operator=(const Any& other){
        Any(other).swap(*this);
        return *this;
    }
};

class Connection;
//DISCONNECTED -- 连接关闭, CONNECTING 连接建立成功，待处理
//CONNECTED -- 连接建立完成，各种设置已经完成，可以通信, DISCONNECTING -- 待关闭状态
typedef enum{ DISCONNECTED, CONNECTING, CONNECTED, DISCONNECTING}ConnStatu;
//对外使用智能指针
using PtrConnection = std::shared_ptr<Connection>;
class Connection: public std::enable_shared_from_this<Connection> {
private:
    uint64_t _conn_id; //连接的唯一id，便于连接的管理和查找

    //定时器id，有事件触发会刷新活跃度，必须是唯一的，为了简化操作，使用conn_id作为定时器id
    //uint64_t _timer_id 

    int _sockfd; //连接关联的文件描述符
    bool _enable_inactive_release = false; //判断连接是否启动非活跃销毁的判断标志，默认为false
    EventLoop* _loop; //连接所关联的一个EventLoop
    ConnStatu _statu; //连接的状态，标记连接是正常通信状态、待关闭状态（处理完数据后就关闭
    Socket _socket; //套接字操作管理
    Channel _channel; //连接的事件管理
    Buffer _in_buffer; //输入缓冲区 -- 存放从socket中读取到的数据 （可能拿到的不完整）
    Buffer _out_buffer; //输出缓冲区 -- 存放要发送给对端的数据
    Any _context; //请求的接收处理上下文

    /* 这四个回调函数，让组件使用者（上层是服务器）来使用的，其实服务器模块的处理回调也是组件使用者来设置的*/
    /*这几个回调都是组件使用者使用的*/
    using ConnectedCallBack = std::function<void(const PtrConnection&)>;
    using MessageCallBack = std::function<void(const PtrConnection&, Buffer*)>;
    using ClosedCallBack = std::function<void(const PtrConnection&)>;
    using AnyEventCallBack = std::function<void(const PtrConnection&)>;
    ConnectedCallBack _connected_callback;
    MessageCallBack _message_callback;
    ClosedCallBack _closed_callback;
    AnyEventCallBack _event_callback;
    /*组件内的连接关闭回调，组件内设置的，因为服务器组件内会把所有的连接管理起来，*/
    /*一旦某个连接要关闭，就应该从管理的地方移除掉自己的信息*/
    ClosedCallBack _server_closed_callback;
//私有接口
private:
    /*五个channel 的事件回调函数*/
    //描述符触发可读事件后调用的函数，接收socket数据放到接收缓冲区中，
    //然后调用_message_callback
    void HandleRead(){
        //1.接收socket 的数据，放到缓冲区 2.调用messagecallback 进行业务处理
        //1.
        char buf[65536];
        //调用非阻塞Recv，没有接收到返回0，否则>0
        int ret = _socket.NonBlockRecv(buf, 65535);
        if (ret < 0){
            //recv出错，不能直接关闭连接，看发送缓冲区有无数据待处理
            //非直接关闭接口
            return ShutDownInLoop();
        }
        //数据放入输入缓冲区,写入之后顺便将写偏移向后移动
        _in_buffer.WriteAndPush(buf, ret);
        //2.
        if (_in_buffer.ReadAbleSize() > 0){
            //shared_from_this() ： 从当前this对象本身获取一个智能指针shared_ptr管理对象
            //需要继承一个模版类才能使用这个接口：enable_shared_from_this<Connection>
            return _message_callback(shared_from_this(), &_in_buffer);
        }
    }
    //描述符触发可写事件后调用的函数，将发送缓冲区中的数据进行发送
    void HandleWrite(){
        //_out_buffer 保存的数据就是要发送的数据
        ssize_t ret = _socket.NonBlockSend(_out_buffer.ReadPosition(), _out_buffer.ReadAbleSize());
        if (ret < 0){
            //发送数据失败了，应该关闭连接了，
            //先判断是否inbuffer 中还有数据，有数据就读取一下
            if (_in_buffer.ReadAbleSize() > 0){
                if (_message_callback){
                    _message_callback(shared_from_this(), &_in_buffer);
                }
            }
            //然后调用关闭接口（实际关闭）
            return Release(); 
        }
        _out_buffer.MoveReadOffset(ret); //一定要向后偏移
        if (_out_buffer.ReadAbleSize() == 0){
            _channel.DisableWrite(); //关闭写事件监控
            //如果当前是连接待关闭状态，有数据发送完数据后，释放连接，没有数据就直接释放
            if (_statu == DISCONNECTING) {
                return Release();
            }
        }
        return;
    }
    //描述符触发挂断事件后调用的函数，
    void HandleClose(){
        /*一旦连接挂断了，套接字就什么也干不了了，因此有数据待处理就处理一下，完毕关闭连接*/
        if (_in_buffer.ReadAbleSize() > 0){
            _message_callback(shared_from_this(), &_in_buffer);
        }
        return ReleaseInLoop();
    }
    //描述符触发出错事件后调用的函数，
    void HandleError(){
        return HandleClose();
    }
    //描述符触发任意事件后调用的函数，
    void HandleEvent(){
        /*需要查看*/
        /*1. 刷新连接活跃度（延迟销毁） 2.调用组件使用者的任意事件回调*/
        if (_enable_inactive_release == true){
            _loop->TimerRefresh(_conn_id);
        }
        if(_event_callback){
            _event_callback(shared_from_this());
        }
    }
    //实际的释放接口，在线程中被调用
    void ReleaseInLoop(){
        //1.修改连接状态，将其置为DISCONNECTED
        _statu = DISCONNECTED;
        //2.移除连接事件监控
        _channel.Remove();
        //3.关闭描述符(socket)
        _socket.Close();
        //4.如果当前定时器队列中还有定时销毁任务，取消任务，否则也指针操作了
        if (_loop->HasTimer(_conn_id)) {
            CancelInactiveReleaseInLoop();
        }
        //5.调用关闭回调函数
        //避免先移除服务器管理的连接信息导致connection被释放，再处理会出错，因此先调用户的。
        if(_closed_callback) {
            _closed_callback(shared_from_this());
        }
        if(_server_closed_callback) {
            _server_closed_callback(shared_from_this());
        }
    }
    //连接获取之后，所处的状态下要进行各种设置，给channel 设置事件回调，启动读监控
    void EstablishedInLoop(){
        /*1. 修改连接状态 2. 启动读监控 3.调用事件回调函数*/
        //断言当前状态必须是上层的半连接状态，连接还没设置完全完毕
        /*启动读事件监控，不能放在构造函数中。*/
        /*一旦启动读事件监控就可能会立即触发读事件，如果这时候启动了非活跃连接销毁*/
        /*刷新活跃度，延迟销毁任务的执行*/
        assert(_statu == CONNECTING); 
        //当前函数执行完毕，连接会进入到已经连接状态
        _statu = CONNECTED;
        _channel.EnableRead();
        if (_connected_callback) {
            _connected_callback(shared_from_this());
        }
    }
    /*接口并不是世纪的发送接口，而只是吧数据放到了发送缓冲区，启动了可写事件监控*/
    void SendInLoop(Buffer& buf){
        //如果已经关闭了就直接return
        if (_statu == DISCONNECTED) return;
        //数据放到缓冲区
        _out_buffer.WriteBufferAndPush(buf);
        if (_channel.WriteAble() == false){
            //启动写事件监控
            _channel.EnableWrite();
        }
    }
    //关闭并非实际的连接释放操作，需要判断还有没有数据待处理，待发送
    void ShutDownInLoop(){
        //1. 设置链接为半关闭状态
        _statu = DISCONNECTING;
        if (_in_buffer.ReadAbleSize() > 0){
            //存在一种情况，接收缓冲区有数据，但数据不完整， 组件使用者不想处理，要关闭连接
            if (_message_callback) {
                _message_callback(shared_from_this(), &_in_buffer);
            }
        }
        // 要么就是写入数据的时候出错导致关闭，要么就是没有待发送数据直接关闭
        if (_out_buffer.ReadAbleSize() > 0){
            if (_channel.WriteAble() == false){
                _channel.EnableWrite();
            }
        }
        if (_out_buffer.ReadAbleSize() == 0){
            return Release();
        }
    }
    //启动非活跃连接超时释放规则
    void EnableInactiveReleaseInLoop(int sec) {
        //1. 判断标志 _enable_inactive_release 设置为true
        _enable_inactive_release = true;
        //2. 添加定时销毁任务（如果当前已经有定时销毁任务了，就刷新延迟
        if (_loop->HasTimer(_conn_id) == true) {
            return _loop->TimerRefresh(_conn_id);
        }
        //3. 如果不存在，就新增
        _loop->TimerAdd(_conn_id, sec, std::bind(&Connection::Release, this));
    }
    void CancelInactiveReleaseInLoop(){
        _enable_inactive_release = false;
        if (_loop->HasTimer(_conn_id) == true) {
            _loop->TimerCancel(_conn_id);
        }
    }
    void AssertInLoop(){
        assert(_loop->IsInLoop() == true);
    }
    void UpgradeInLoop(const Any& context, 
                       const ConnectedCallBack& conncb, 
                       const MessageCallBack& msgcb, 
                       const ClosedCallBack& closecb, 
                       const AnyEventCallBack& anycb){
        //修改回调函数和上下文。
        _context = context;
        _connected_callback = conncb;
        _message_callback = msgcb;
        _closed_callback = closecb;
        _event_callback = anycb;
    }
//暴露接口
public:
    Connection(EventLoop* loop, uint64_t conn_id, int sockfd) :_conn_id(conn_id),_sockfd(sockfd), _enable_inactive_release(false),_loop(loop),_statu(CONNECTING),_socket(sockfd),_channel(loop, _sockfd){
        //在channel中设置构造函数
        /*启动读事件监控，不能放在构造函数中。*/
        /*一旦启动读事件监控就可能会立即触发读事件，如果这时候启动了非活跃连接销毁*/
        /*刷新活跃度，延迟销毁任务的执行*/
        _channel.SetCloseCallBack(std::bind(&Connection::HandleClose, this));
        _channel.SetEventCallBack(std::bind(&Connection::HandleEvent, this));
        _channel.SetReadCallBack(std::bind(&Connection::HandleRead, this));
        _channel.SetWriteCallBack(std::bind(&Connection::HandleWrite, this));
        _channel.SetErrorCallBack(std::bind(&Connection::HandleError, this));
    }
    ~Connection(){
        DBG_LOG("RELEASE CONNCECTION: %p", this);
    }
    //获取管理的连接描述符
    int Fd(){
        return _sockfd;
    }
    //获取Id
    int Id(){
        return _conn_id;
    }
    //返回状态
    ConnStatu Statu(){
        return _statu;
    }
    //是否是连接建立状态
    bool IsConnected(){
        return (_statu == CONNECTED);
    }
    //设置上下文 -- 连接建立完成时，需要设置上下文和回调函数
    void SetContext(const Any& context){
        _context = context;
    }
    //获取上下文，返回的是指针，因为可能需要修改，需要每次访问到同一块地址。
    Any* GetContext(){
        return &_context;
    }
    //设置回调函数
    void SetConnectedCallBack(const ConnectedCallBack& cb){
        _connected_callback = cb;
    }
    void SetMessageCallBack(const MessageCallBack& cb){
        _message_callback = cb;
    }
    void SetClosedCallBack(const ClosedCallBack& cb){
        _closed_callback = cb;
    }
    void SetAnyEventCallBack(const AnyEventCallBack& cb){
        _event_callback = cb;
    }
    void SetServerClosedCallBack(const ClosedCallBack& cb){
        _server_closed_callback = cb;
    }
    //连接建立就绪之后，进行channel 回调设置，启动读监控，调用_connected_callback;
    void Established(){
        _loop->RunInLoop(std::bind(&Connection::EstablishedInLoop, this));
    }
    //发送数据，将数据放到发送缓冲区，启动写事件监控
    void Send(const char* data, size_t len) {
        //外界传入的data，可能是个临时空间，我们现在只是把发送操作压入了任务池
        //有可能没有被立即执行，因此有可能执行的时候空间已经被释放掉了
        //创建新的临时对象buffer
        Buffer buf;
        buf.WriteAndPush(data, len);
        //将buffer传入SendInLoop
        _loop->RunInLoop(std::bind(&Connection::SendInLoop, this, std::move(buf)));
    }
    //提供给组件使用者的关闭接口，不会直接关闭，先处理完业务
    void ShutDown() {
        _loop->RunInLoop(std::bind(&Connection::ShutDownInLoop,this));
    }
    void Release() {
        _loop->QueueInLoop(std::bind(&Connection::ReleaseInLoop, this));
    }
    //启动非活跃销毁，并定义多长时间无通信就是非活跃，添加定时任务
    void EnableInactiveRelease(int sec){
        _loop->RunInLoop(std::bind(&Connection::EnableInactiveReleaseInLoop,this, sec));
    }
    //取消非活跃销毁
    void CancelInactiveRelease(int sec){
        _loop->RunInLoop(std::bind(&Connection::CancelInactiveReleaseInLoop,this));
    }
    //切换协议 -- 重置上下文以及阶段性回调处理函数 --- 非线程安全，所以必须在同一线程调用
    void Upgrade(const Any& context, 
                 const ConnectedCallBack& conncb, 
                 const MessageCallBack& msgcb, 
                 const ClosedCallBack& closecb, 
                 const AnyEventCallBack& anycb){
        /*这个接口必须在EventLoop*/
        AssertInLoop();
        _loop->RunInLoop(std::bind(&Connection::UpgradeInLoop, this, context, conncb, msgcb, closecb, anycb));
    }
};

class Acceptor{
private:
    Socket _socket; //用于创建监听套接字
    EventLoop* _loop; //用于对监听套接字进行事件监控
    Channel _channel; //用于对监听套接字进行事件管理

    using AcceptCallBack = std::function<void(int)>;
    AcceptCallBack _accept_callback;
private:
    /*监听套接字的读事件回调处理函数 -- 获取新连接，调用_accept_cb函数进行新连接初始化*/
    void HandleRead() {
        int newfd = _socket.Accept();
        if (newfd < 0){
            return;
        }
        if (_accept_callback) _accept_callback(newfd);
    }
    int CreateServer(int port) {
        //创建服务器
        bool ret = _socket.CreateServer(port);
        assert(ret == true);
        return _socket.Fd();
    }
public:
    /*不能将启动读事件监控放到构造函数中，必须在设置回调函数后再去启动*/
    /*否则有可能造成启动监控后立即有事件，处理的时候回调函数还没有设置，新连接得不到处理，而且资源有泄漏*/
    Acceptor(EventLoop* loop, int port) :_socket(CreateServer(port)),_loop(loop),_channel(loop, _socket.Fd()) {
        _channel.SetReadCallBack(std::bind(&Acceptor::HandleRead,this));
    }
    void SetAcceptCallBack(const AcceptCallBack& cb){
        _accept_callback = cb;
    }
    void Listen() {
        _channel.EnableRead();
    }
};

class LoopThread{
private:
    /*用于实现_loop获取的同步关系，避免线程创建了但是_loop还没实例化前就去获取_loop*/
    std::mutex _mutex; //互斥锁
    std::condition_variable _cond; //条件变量
    //我们不能直接实例化EventLoop,否则是在主线程内部进行实例化和绑定threadid
    EventLoop* _loop; //EventLoop 对象指针变量，这个对象需要在线程内部实例化
    std::thread _thread; //EventLoop对应的线程

private:
    /*实例化EventLoop对象，并且开始运行EventLoop*/
    /*需要唤醒cond上有可能阻塞的线程*/
    void ThreadEntry() {
        EventLoop loop;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _loop = &loop;
            _cond.notify_all();
        }
        //循环运行
        loop.Start();
    }
public:
    /*创建线程，设定线程入口函数*/
    LoopThread() :_loop(nullptr), _thread(std::thread(&LoopThread::ThreadEntry, this)){}
    /*返回当前线程关联的EventLoop指针*/
    /*当我们的线程还没有实例化，直接获取是获取到nullptr的，需要加互斥锁和条件变量*/
    EventLoop* GetLoop(){
        EventLoop* loop = nullptr;
        {
            //加锁
            std::unique_lock<std::mutex> lock(_mutex);
            //lambda表达式：loop为空就一直阻塞等待
            _cond.wait(lock, [&](){return _loop != nullptr;});
            loop = _loop;
        }
        return loop;
    }
};

class LoopThreadPool{
private:
    int _thread_count; //线程数量
    int _next_loop_idx; //线程控制idx
    EventLoop* _base_loop; //运行在主线程，从属线程数量为0时，所有操作都在baseloop中进行
    std::vector<LoopThread*> _threads; //保存所有的LoopThread 对象
    std::vector<EventLoop*> _loops; //从属线程数量大于0，从loops进行线程eventloop分配
public:
    LoopThreadPool(EventLoop* baseloop):_thread_count(0), _next_loop_idx(0),_base_loop(baseloop){}
    void SetThreadCount(int count){
        _thread_count = count;
    }
    void Create(){
        if (_thread_count > 0) {
            _threads.resize(_thread_count);
            _loops.resize(_thread_count);
            for (int i = 0; i < _thread_count; ++i) {
                //创建thread之前，有操作都是会阻塞的
                _threads[i] = new LoopThread();
                _loops[i] = _threads[i]->GetLoop();
                //先去Create 然后再去启动baseloop
            }
        }
        return;
    }
    EventLoop* NextLoop() {
        if (_thread_count == 0) {
            return _base_loop;
        }
        _next_loop_idx = (_next_loop_idx + 1) % _thread_count;
        return _loops[_next_loop_idx];
    }
};

class TcpServer{
private:
    uint64_t _next_id; //自动增长的连接id
    int _port;
    int _timeout; //非活跃连接的统计时间 -- 多长时间未通信是非活跃连接
    bool _enable_inactive_release; //是否启动非活跃超时连接销毁
    EventLoop _base_loop; //主线程的EventLoop对象，负责监听事件的处理
    Acceptor _acceptor; //监听套接字的管理对象
    LoopThreadPool _pool; //从属线程池
    std::unordered_map<uint64_t, PtrConnection> _conns; //保存管理所有连接对应的shared_ptr对象
    using Functor = std::function<void()>;
    using ConnectedCallBack = std::function<void(const PtrConnection&)>;
    using MessageCallBack = std::function<void(const PtrConnection&, Buffer*)>;
    using ClosedCallBack = std::function<void(const PtrConnection&)>;
    using AnyEventCallBack = std::function<void(const PtrConnection&)>;
    ConnectedCallBack _connected_callback;
    MessageCallBack _message_callback;
    ClosedCallBack _closed_callback;
    ClosedCallBack _server_closed_callback;
    AnyEventCallBack _event_callback;

private:
    //为新连接构造一个Connection 进行管理
    void NewConnection(int fd) {
        _next_id++;
        PtrConnection conn(new Connection(_pool.NextLoop(), _next_id, fd));
        conn->SetMessageCallBack(_message_callback); 
        conn->SetConnectedCallBack(_connected_callback);
        conn->SetClosedCallBack(_closed_callback);
        conn->SetAnyEventCallBack(_event_callback);
        conn->SetServerClosedCallBack(std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
        
        //启动非活跃连接销毁
        if(_enable_inactive_release == true) {
            conn->EnableInactiveRelease(_timeout);
        }
        //就绪初始化
        conn->Established();
        //用map管理起来
        _conns.insert({_next_id, conn});
    }
    void RemoveConnectionInLoop(const PtrConnection& conn) {
        int id = conn->Id();
        auto it = _conns.find(id);
        if (it != _conns.end()) {
            _conns.erase(it);
        }
    }
    //从管理Connection的_conns 移除连接信息，如果不移除连接将不会被释放
    void RemoveConnection(const PtrConnection& conn) {
        _base_loop.RunInLoop(std::bind(&TcpServer::RemoveConnectionInLoop, this, conn));
    } 
    void RunAfterInLoop(const Functor& task, int delay) {
        _next_id ++;
        _base_loop.TimerAdd(_next_id, delay, task);
    }
public:
    TcpServer(int port) 
        :_port(port),
        _next_id(0),
        _enable_inactive_release(false),
        _acceptor(&_base_loop, port),
        _pool(&_base_loop)
    {
        _acceptor.SetAcceptCallBack(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1));
        _acceptor.Listen(); //将监听套接字挂到baseloop上开始监控事件
    }
    void SetThreadCount(int count) {
        return _pool.SetThreadCount(count);
    }
    void EnableInactiveRelease(int timeout) {
        _timeout = timeout;
        _enable_inactive_release = true;
    }
    void Start() {
        _pool.Create();     //创建线程池中的从属线程
        //构造函数已经完成了创建和监听
        _base_loop.Start(); //开始处理
    }
    //用来添加一个定时任务
    void RunAfter(const Functor& task, int delay) {
        _base_loop.RunInLoop(std::bind(&TcpServer::RunAfterInLoop, this, task, delay));
    }

    //设置回调函数
    void SetConnectedCallBack(const ConnectedCallBack& cb){
        _connected_callback = cb;
    }
    void SetMessageCallBack(const MessageCallBack& cb){
        _message_callback = cb;
    }
    void SetClosedCallBack(const ClosedCallBack& cb){
        _closed_callback = cb;
    }
    void SetAnyEventCallBack(const AnyEventCallBack& cb){
        _event_callback = cb;
    }
    void SetServerClosedCallBack(const ClosedCallBack& cb){
        _server_closed_callback = cb;
    }

};

class NetWork {
public:
    NetWork(){
        DBG_LOG("SIGPIPE SIG_IGN INIT");
        signal(SIGPIPE, SIG_IGN);
    }
}; 
static NetWork nw;
#endif