#include"BlockingQueue.hpp"
using namespace ns_blockingQueue;

void *consumer(void* args)
{
    BlockingQueue<int> *bq = (BlockingQueue<int> *)args;
    while(1)
    {
        int data = 0;
        bq->Pop(&data);
        std::cout << "Consumers buy the datas : " << data << std::endl;
    }

}
void *producer(void* args)
{
    BlockingQueue<int> *bq = (BlockingQueue<int> *)args;
    
    while(1)
    {
        int data = rand()%50 + 1;
        std::cout << "Producer produce the datas : " << data << std::endl;
        bq->Push(data);

        sleep(1);
    }
}

int main()
{
    srand((long long)time(nullptr));
    BlockingQueue<int>* bq = new BlockingQueue<int>;

    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, (void*)bq);
    pthread_create(&p, nullptr, producer, (void*)bq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);

    return 0;
}