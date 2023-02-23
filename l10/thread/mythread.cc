#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
void* thread_run(void* argc)
{
    while(1)
    {
        printf("new thread id is 0x%x\n", pthread_self());
        sleep(1);
    }   
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_run, (void*)"new thread");

    while(1)
    {
        printf("main thread id is 0x%x\n", pthread_self());
        sleep(1);
    }

    return 0;
}







// #define NUM 1
// void* thread_run(void* args)
// {
//     int cnt = 3;
//     while(cnt--){
//         cout << "I am a new thread, name is : " << (const char*) args << " My thread_id is : " << pthread_self() << endl;
//         sleep(1);
//     }
//     return (void*)999;
// }
// int main()
// {
//     pthread_t tid[NUM];
//     for(int i = 0; i < NUM; ++i)
//     {
//         pthread_create(tid + i, NULL, thread_run, (void*)"new_thread");
//         sleep(1);
//     }
//     void* status = NULL;
//     pthread_join(tid[NUM-1], &status);
//     cout << "ret : " << (long long)status << endl;
//     // while(1)
//     // {
//     //     cout << "I am the main(). My thread_id is " << pthread_self() << endl;
//     //     cout << "######################### begin #########################" << endl;
//     //     for(int i = 0; i < NUM; ++i)
//     //     {   
//     //         printf("The thread[%d] I created is %lu\n", i, tid[i]);

//     //     }
//     //     cout << "######################### begin #########################" << endl;

//     //     sleep(5);
//     // }
//     return 0;
// }

// // int main()
// // {
// //     pthread_t tid;
// //     pthread_create(&tid, NULL, thread_run, (void*)"new thread");
    
// //     while(1)
// //     {
// //         cout << "I am the main, the thread_id I created is : " << tid << " My thread_id is : " << pthread_self() <<  endl; 
// //         sleep(5);
// //     }
// //     return 0;
// // }


// // void * thread_run(void* args)
// // {
// //     const char* id = (const char*) args;
// //     while(1)
// //     {
// //         cout << "I am a thread, no is : " << getpid() << endl;
// //         sleep(1);
// //     }
// // }
// // int main()
// // {
// //     pthread_t tid;
// //     pthread_create(&tid, NULL, thread_run, (void*)"thread 1");

// //     while(1)
// //     {
// //         cout << "I am the main thread, id is : " << getpid() << endl;
// //         sleep(1);
// //     }

// //     return 0;
// // }