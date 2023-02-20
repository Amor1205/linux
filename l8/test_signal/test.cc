#include<unistd.h>
#include<iostream>
#include<signal.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
using namespace std;

static void Usage(const char* proc)
{
	cout << "Usage:\n\t " << proc << " signal who" << endl; 
}
// ./test signal who
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		//error
		Usage(argv[0]);
		return 1;
	}
	int signal = atoi(argv[1]);
	int who = atoi(argv[2]);
	kill(who,signal);
	cout << "signal: " << signal << " who: " << who << endl;
	return 0;
}



// int main(){

// 	if(fork() == 0){
// 		while(1){
// 			cout << "I am a child !" << endl;
// 			int a = 10;
// 			a /=0;
// 		}
// 	}
// 	int status = 0;
// 	waitpid(-1 ,&status, 0);

// 	cout << "exit code: " << ((status >> 8) & 0xFF)  << " exit signal: " << ((status)& 0x7F) << endl;
// 	cout << "core dump: " << ((status >> 7) & 1) << endl;
	
// }
//void handler(int signo)
//{
//	switch(signo)
//	{
//		case 2:
//			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;	
//			cout << "have gotten a signal, signal no is : " << signo << " My pid is : " << getpid() << endl;
//			break;
//		case 3:
//			cout << "--------------------------------------------" << endl;
//			cout << "have gotten a signal, signal no is : " << signo << " My pid is : " << getpid() << endl;
//			break;
//		case 9:
//			cout << "============================================" << endl;
//			cout << "have gotten a signal, signal no is : " << signo << " My pid is : " << getpid() << endl;
//			break;
//		default:
//			cout << "default" << endl;
//	}
//	exit(1);
//}
//
//
//int main()
//{
//	//signal(2, handler);
//	int sig = 1;
//	while(sig <= 31)
//	{
//		signal(sig, handler);
//		sig++;	
//	}		
//	while(1)
//	{
//		cout << "look! I am running! " << " My pid is : " << getpid() << endl;
//		sleep(1);
//	}
//
//	return 0;
//}
