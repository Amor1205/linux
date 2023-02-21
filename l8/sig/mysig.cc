#include<iostream>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
using namespace std;
void handler(int signo)
{
    while(1){
        cout << "get a singo : " << signo << endl;
        sleep(1);
    }
}
int main()
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    sigemptyset(&act, sa_mask);
    sigaddset(&act, sa_mask, 3);
    sigaction(2, &act, NULL);

    while(1)
    {
        cout << "hello world! " << endl;
        sleep(1);
    }
    return 0;
}
// void handler(int signo)
// {
//     cout << "signo 2 has been delivered! ~~" << endl;
// }
// void showPending(sigset_t* set)
// {
//     for(int i = 1; i <= 31; i++)
//     {
//         if(sigismember(set, i))
//         {
//             cout << "1";
//         }
//         else{
//             cout << "0";
//         }
//     }
//     cout << endl;
//     sleep(2);
// }
// int main()
// {
//     signal(2, handler);
//     sigset_t set, oset;
//     sigemptyset(&set);
//     sigemptyset(&oset);
    
//     sigaddset(&set, 2);

//     sigprocmask(SIG_SETMASK, &set, &oset);
//     sigset_t pending;
//     int count = 0;
//     while(1)
//     {
//         sigemptyset(&pending);
//         sigpending(&pending);

//         showPending(&pending);
//         sleep(1);
//         count ++ ;
//         if(count >= 10)
//         {
//             //sigprocmask(SIG_UNBLOCK, &set, &oset);
//             sigprocmask(SIG_SETMASK, &oset, NULL);
//             cout << "Recovery from Oset (No 2 gisno block)";
//         }
//     }
//     return 0;
// }