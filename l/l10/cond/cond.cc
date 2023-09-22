#include<iostream>
#include<string>
#include<unistd.h>
#include<pthread.h>
using namespace std;
static int NUM = 3;

pthread_mutex_t mtx;
pthread_cond_t cond;

//control the derived thread
void* ctrl(void* args)
{
    string name = (char*) args;
    while(1)
    {
        pthread_cond_broadcast(&cond);
        sleep(1);
    }

}

void* work(void* args)
{
    int number = *(int*)args;
    delete (int*)args;
    while(1)
    {
        pthread_cond_wait(&cond, &mtx);
        cout << "Worker["<< number <<"] is working!" << endl;
    }
}
int main()
{    
    pthread_mutex_init(&mtx,nullptr);
    pthread_cond_init(&cond,nullptr);
    pthread_t main;
    pthread_t derive[NUM];
    pthread_create(&main, nullptr, ctrl, (void*)"main");
    for(int i = 0 ; i < NUM; ++i)
    {
        int* number = new int(i);
        pthread_create(derive+i, nullptr, work, (void*)number);
    }
    for(int i= 0;i < NUM; ++i)
    {
        pthread_join(derive[i], nullptr);
    }
    pthread_join(main,nullptr);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    return 0;
}