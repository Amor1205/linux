#include<stdio.h>
#include<iostream>
#include<signal.h>
using namespace std;

volatile int flag = 0;

void handler(int signo)
{
    flag = 1;
    cout << "change flag 0 to 1" << endl;
}
int main()
{
    signal(2, handler);
    while(!flag);
    cout << "normal exit" << endl;
    return 0;
}