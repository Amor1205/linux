#include "thread_pool.hpp"

using namespace ns_threadpool;
using namespace ns_task;
int main()
{
    srand((long long)time(nullptr));
    std::string ops = "+-*/%";
    while(1){
        //produce the task
        Task t(rand()%20+1, rand()%10+1, ops[rand()%5]);
        ThreadPool<Task>::GetInstance()->PushTask(t);
        sleep(1);
    }
    return 0;
}