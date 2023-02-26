#include"circular_queue.hpp"
using namespace ns_circular_queue;
void* consumer(void* args)
{
    CircularQueue<int>* rq = (CircularQueue<int>*) args;
    while(1)
    {
        int data = 0;
        rq->Pop(&data);
        std::cout << "The data consumed is : " << data << std::endl;
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
    pthread_t c,p;
    pthread_create(&c, nullptr, consumer, (void*)rq);
    pthread_create(&p, nullptr, producer, (void*)rq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}