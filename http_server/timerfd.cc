#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/timerfd.h>

int main(){
    //int timerfd_create(int clockid, int flags);
    int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if(timerfd < 0) perror("timerfd_create error");
    struct itimerspec itime;
    itime.it_value.tv_sec = 1;
    itime.it_value.tv_nsec = 0;
    itime.it_interval.tv_sec = 1;
    itime.it_interval.tv_nsec = 0;
    timerfd_settime(timerfd, 0, &itime, nullptr);
    while(1)
    {
        uint64_t times;
        int ret = read(timerfd, &times, 8);
        if(ret < 0)
        {
            perror("read error");
            return -1;
        }
        std::cout << "超时：距离上次操作过去了" << times << " seconds"<< std::endl; 
    }
    return 0;
}