#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
int main()
{
	if(fork()==0)
	{
	cout << "Process begins" << endl;
	execl("/bin/ls","ls","-a","-l","-i",NULL);
	cout << "Process ends" << endl;
	exit(1);
	}
	waitpid(-1,NULL,0);
	cout << "waiting is successful!" << endl;
	return 0;






	//cout << "hello world" << endl;
//	for(int i = 0; i < 100; ++i)
//	l                                                                                                                                                                                                                                               
//	{
//		cout << i << " : " << perror(i)  << endl;
//	}
//
//	pid_t id = fork();
//	if(id == 0)
//	{
//		//child
//		int cnt = 5;
//		while(cnt)
//		{
//			cout << "child" << getpid() <<" is running, cnt is : " << cnt << endl;
//			cnt --;
//			sleep(1);
//		}
//		exit(0);
//	}
//	//parent
//	pid_t ret = waitpid(id,NULL,0);
//	if(ret >= 0)
//		cout << "father is waiting, ret is : " << ret << endl;
//	else 
//		cout << "father failed to wait!" << endl; 
//	return 0;
}
