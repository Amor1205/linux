#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
using namespace std;

void * thread_run(void* args)
{
    const char* id = (const char*) args;
    while(1)
    {
        cout << "I am a thread, no is : " << getpid() << endl;
        sleep(1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_run, (void*)"thread 1");

    while(1)
    {
        cout << "I am the main thread, id is : " << getpid() << endl;
        sleep(1);
    }

    return 0;
}