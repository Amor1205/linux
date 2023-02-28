#include"circular_queue.hpp"
using namespace ns_circular_queue;
using namespace ns_task;
void* consumer(void* args)
{
    CircularQueue<Task>* rq = (CircularQueue<Task>*) args;
    while(1)
    {
        Task t;
        rq->Pop(&t);
        std::cout << "The task consumed by the ["<< pthread_self()<<"]is : " << t.Run() << std::endl;
        //consume pop
    }
}
void* producer(void* args)
{
    const std::string ops = "+-*/%";
    CircularQueue<Task>* rq = (CircularQueue<Task>*) args;
    while(1)
    {
        int x = rand()%20 + 1;
        int y = rand()%10 + 1;
        int op = ops[rand()%ops.size()];
        Task t(x,y,op);

        std::cout << "The task produced is : " << t.Show() << std::endl;
        rq->Push(t);
        //produce push
        sleep(1);
    }
}
int main()
{
    srand((unsigned int)time(nullptr));
    CircularQueue<Task>* rq = new CircularQueue<Task>();
    pthread_t c,p,c1,c2,c3,c4;
    pthread_create(&c, nullptr, consumer, (void*)rq);
    pthread_create(&c1, nullptr, consumer, (void*)rq);
    pthread_create(&c2, nullptr, consumer, (void*)rq);
    pthread_create(&c3, nullptr, consumer, (void*)rq);
    pthread_create(&c4, nullptr, consumer, (void*)rq);
    pthread_create(&p, nullptr, producer, (void*)rq);

    pthread_join(c, nullptr);
    pthread_join(c1, nullptr);
    pthread_join(c2, nullptr);
    pthread_join(c3, nullptr);
    pthread_join(c4, nullptr);
    pthread_join(p, nullptr);

    return 0;
}