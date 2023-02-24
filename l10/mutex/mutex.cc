#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include<string>

using namespace std;
static int NUM = 5;
int ticketNum = 10000;

class Ticket{
private:
    int tickets;
    pthread_mutex_t mtx;

public:
    Ticket()
        :tickets(1000)
    {
        pthread_mutex_init(&mtx, NULL);
    }
    ~Ticket()
    {
        pthread_mutex_destroy(&mtx);
    }
    bool GetTicket()
    {
        bool ret = true;
        pthread_mutex_lock(&mtx);
        if(tickets > 0)
        {
            usleep(1000);
            cout << "I am [" << getpid() << "] I want to buy ticket[" << tickets << "]" <<endl;
            tickets--;
        }
        else{
            cout << "empty tickets" << endl;
            ret = false;
        }
        pthread_mutex_unlock(&mtx);
        return ret;
    }
};

void* threadRun(void* args)
{
    Ticket* t = (Ticket*)args;
    // string threadName = (char*)args;
    while(1)
    {   
        bool flag = t->GetTicket();
        if(flag == 0)
        {
            break;
        }
        else{
            continue;
        }
    }
        // cout << threadName << "is running !" << endl;
        // sleep(1);
}
int main()
{
    Ticket *t = new Ticket();

    pthread_t tid[NUM];
    for(int i = 0; i < NUM; ++i)
    {
        int* id = new int(i);
        pthread_create(tid + i, NULL, threadRun, (void*)t);
    }
    for(int i = 0; i < NUM; ++i)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}