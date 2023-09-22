#include"BlockingQueue.hpp"
#include"task.hpp"
using namespace ns_blockingQueue;
using namespace ns_task;
void *consumer(void* args)
{
    BlockingQueue<Task> *bq = (BlockingQueue<Task> *)args;
    while(1)
    {
        Task t;
        bq->Pop(&t); // Completed the first step of mission comsumption
        t.Run();
        // int data = 0;
        // bq->Pop(&data);
        // std::cout << "Consumers buy the datas : " << data << std::endl;
    }

}
void *producer(void* args)
{
    BlockingQueue<Task> *bq = (BlockingQueue<Task> *)args;
    std::string ops = "+-*/%";
    while(1)
    {
        int x = rand()%20 + 1; //[1,20]
        int y = rand()%10 + 1; //[1,10]
        char op = ops[rand()%5]; //[0,4]
        Task t(x,y,op);
        std::cout << "Producer arranges a task: " << x << op << y << "=? " << std::endl;
        bq->Push(t);
        // int data = rand()%50 + 1;
        // std::cout << "Producer produce the datas : " << data << std::endl;
        // bq->Push(data);

        sleep(1);
    }
}

int main()
{
    srand((long long)time(nullptr));
    BlockingQueue<Task>* bq = new BlockingQueue<Task>;

    pthread_t c, p;
    pthread_t c1,c2,c3,c4;
    pthread_create(&c, nullptr, consumer, (void*)bq);
    pthread_create(&c1, nullptr, consumer, (void*)bq);
    pthread_create(&c2, nullptr, consumer, (void*)bq);
    pthread_create(&c3, nullptr, consumer, (void*)bq);
    pthread_create(&c4, nullptr, consumer, (void*)bq);
    pthread_create(&p, nullptr, producer, (void*)bq);

    pthread_join(c,nullptr);
    pthread_join(c1,nullptr);
    pthread_join(c2,nullptr);
    pthread_join(c3,nullptr);
    pthread_join(c4,nullptr);
    pthread_join(p,nullptr);

    return 0;
}