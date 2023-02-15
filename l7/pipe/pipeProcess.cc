#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<sys/wait.h>
#include<sys/types.h>

#include<string>
using namespace std;

int main()
{
    int pipefd[2] = {0};
    if(pipe(pipefd) != 0)
    {
        perror("pipe");
        return 1;
    }
    // cout << pipefd[0] << " " << pipefd[1] << endl;

    //parent process to read, child process to write
    if(fork() == 0)
    {
        //child
        close(pipefd[0]);
        const char* msg = "hello world!";
        int cn = 0;
        
        while(1)
        {
            
            // write(pipefd[1],"a",1);
            write(pipefd[1], msg, strlen(msg));
            // cn ++;
            // cout << " count = " << cn;
            // sleep(10);
            // break;
        }
        // close(pipefd[1]);
        exit(0);
    }
    else{
        //parent
        close(pipefd[1]);
        sleep(10);
        while(1)
        {
            char ch[64] = {0};
            ssize_t s = read(pipefd[0], ch , sizeof(ch));
            if(s > 0){
                ch[s] = 0;
                cout << "father: " << ch;
            }
            break;
        }
        close(pipefd[0]);
        int status = 0;
        waitpid(-1, &status, 0);
        cout << "status : > " << status << endl;
        cout << "exit code : " << ((status >> 8) & 0xFF )<< endl;
        cout << "exit signal : " << ((status) & 0x7F) << endl ;
        return 0;
        // while(1)
        // {
        //     // sleep(10);
        //     char ch[64] = {0};
        //     ssize_t s = read(pipefd[0], ch, 63);
        //     // cout <<  "father read one char : " << ch << endl;
        //     if(s > 0)
        //     {
        //         ch[s] = 0;
        //         cout << "father read something: " << ch << endl;
        //     }
        //     else if ( s == 0)
        //     {
        //         cout << "writer has quitted" << endl;
        //         break;
        //     }
        //     else {
        //         break;
        //     }
        // }
        // char buffer[64] = {};
        // while(1){
        //     // sleep(1);
        //     buffer[0] = 0;
        //     ssize_t s = read(pipefd[0],buffer,sizeof(buffer)-1);
        //     if(s == 0)
        //     {
        //         break;
        //     }
        //     else if(s > 0)
        //     {
        //         buffer[s] = 0;
        //         cout << "Child process write: " << buffer << endl;

        //     }
        // }
    }
    return 0;
}