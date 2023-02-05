#include<iostream>
#include<string>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;
int main()
{
	//cout << "hello world" << endl;
//	for(int i = 0; i < 100; ++i)
//	l                                                                                                                                                                                                                                               
//	{
//		cout << i << " : " << perror(i)  << endl;
//	}

	pid_t id = fork();
	if(id == 0)
	{
		//child
		int cnt = 5;
		while(cnt)
		{
			cout << "child" << getpid() <<" is running, cnt is : " << cnt << endl;
			cnt --;
			sleep(1);
		}
		exit(0);
	}
	//parent
	pid_t ret = waitpid(id,NULL,0);
	if(ret >= 0)
		cout << "father is waiting, ret is : " << ret << endl;
	else 
		cout << "father failed to wait!" << endl; 
	return 0;
}
