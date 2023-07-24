#include <iostream>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <functional>
#include <cstdint>
#include <memory>

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
private:
    void RemoveTimer(uint64_t id)
    {
        auto it = _timers.find(id);
        if(it != _timers.end()) {
            _timers.erase(it);
        }
    }
public:
    TimerWheel() :_capacity(60), _tick(0), _wheel(_capacity) {}
    void TimerAdd(uint64_t id, uint32_t delay, const TaskFunc &cb)//添加定时任务
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
    void TimerRefresh(uint64_t id)//刷新/延迟定时任务
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

    void TimerCancel(uint64_t id)
    {
        auto it = _timers.find(id);
        if(it == _timers.end()) {
            return; //没找着定时任务，没必要刷新和延迟。
        }
        PtrTask pt = it->second.lock();
        if (pt) pt->Cancel();
    }

    //这个函数每秒钟执行一次，相当于秒针向后走一步
    void RunTimerTask()
    {   
        _tick = (_tick + 1) % _capacity;
        _wheel[_tick].clear(); 
        //清空指定位置的数组，会把数组中保存的所有管理定时器对象的sharedptr释放掉
    }
};

class Test
{
public:
    Test() {std::cout << "构造" << std::endl;}
    ~Test() {std::cout << "析构" << std::endl;}
};
void DeleteTest(Test* t){
    delete t;
}

int main()
{
    TimerWheel tw;
    Test *t = new Test();
    tw.TimerAdd(888,5,std::bind(DeleteTest, t));
    for(int i = 0; i < 5; ++i)
    {
        tw.TimerRefresh(888);
        tw.RunTimerTask();//向后移动秒针
        std::cout << "刷新了定时任务，需要5s后才会销毁" << std::endl;
        sleep(1);
    }
    tw.TimerCancel(888);
    while(1)
    {
        sleep(1);
        std::cout << "---------------------------\n";
        tw.RunTimerTask();
    }
    return 0;
}