#include<signal.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
using namespace std;

void handler(int signo)
{
    cout << "I have received the singo: SIGCHLD. " << endl;
    cout << "LET IT DIE." << endl;
}
int main()
{
    signal(SIGCHLD, handler);
    if(fork()==0)
    {
        int cnt = 3;
        while(cnt--)
        {
            cout << "I am the child." << "My id is " << getpid() << endl;
            sleep(1);
        }
        exit(0);
    }
    while(1);
    return 0;
}