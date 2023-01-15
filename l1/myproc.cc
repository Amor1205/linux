#include<iostream>
#include<unistd.h>
#include<sys/types.h>

using namespace std;

int main()
{
	fork();
	cout << "PID: " << getpid() << " " << "PPID: " << getppid() << endl;
	sleep(1);
//	while(1)
//	{
//		cout << "HELLO WORLD!" << " PID: " << getpid() << endl;
//		sleep(1);
//	}
	return 0;
}
