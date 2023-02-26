#include"circular_queue.hpp"
using namespace ns_circular_queue;
void* consumer(void* args)
{
    CircularQueue<int>* rq = (CircularQueue<int>*) args;
    while(1)
    {
        int data = 0;
        rq->Pop(&data);
        std::cout << "The data consumed by the ["<< pthread_self()<<"]is : " << data << std::endl;
        //consume pop
    }
}
void* producer(void* args)
{
    CircularQueue<int>* rq = (CircularQueue<int>*) args;
    while(1)
    {
        int data = rand()%20 + 1;
        std::cout << "The data produced is : " << data << std::endl;
        rq->Push(data);
        //produce push
        sleep(1);
    }
}
int main()
{
    srand((unsigned int)time(nullptr));
    CircularQueue<int>* rq = new CircularQueue<int>();
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