#include<stdio.h>
#include<iostream>
#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;


//1.
// int main()
//{
//	close(1);
//	const char* msg = "hello stdout\n";
//	int fd = open("./log.txt", O_WRONLY | O_CREAT, 0644);
//	printf(" fd = %d ", fd);
//	fprintf(stdout,"%s",msg);
//	printf("%s",msg);
//	fputs(msg,stdout);
//	
//	fflush(stdout);
//	close(fd);
//	return 0;
//}
//
//2.
 int main()
{
	const char* msg = "hello stdout\n";
	write(1,msg,strlen(msg));

	printf("hello printf\n");
	fprintf(stdout, "hello fprintf\n");
	fputs("hello fputs\n", stdout);
	
	fork();
	return 0;
}
