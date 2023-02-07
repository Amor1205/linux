#include<iostream>
#include<string>
#include <unistd.h>
#include <stdlib.h>
#include<cstring>
#include<sys/wait.h>
#include<vector>
using namespace std;

static int NUM = 128;
int main()
{
	char command[NUM];

	for(;;){
		//1 print
		command[0] = 0;//clear the string
		cout << "[user@amor1205 minishell]# ";
		//2. get string
		fgets(command,NUM,stdin);
		//delete the \n
		command[strlen(command) - 1] = '\0';
		string str;
		str += command;
		//cout << "echo : " << command << endl;
		//fflush(stdout);
		//sleep(100);
		//
		//3. get char* argv[]
		const char* sep = " ";
		char* argv[NUM] = {NULL};
		argv[0] = strtok(command, sep);
		int i = 1;
		while(argv[i++] = strtok(NULL, sep)){;}
		
	//	for(int j = 0; j < i; ++j)
	//	{
	//		cout << argv[j] << endl;
	//	}
	//
		if(strcmp(argv[0],"cd") == 0)
		{
			if(argv[1]!= NULL) chdir(argv[1]);
			continue;
		}
		if(fork() == 0)
		{
			execvp(argv[0],argv);
			exit(1);
		}
		int status = 0;
		waitpid(-1,&status,0);
		cout << "exit code : " << ((status >> 8)& 0xFF);
	}
}


