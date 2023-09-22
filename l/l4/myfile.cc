#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string>
#include<cstring>
#include<iostream>
using namespace std;
//write
//int main()
//{
 //       int fd = open("./log.txt", O_WRONLY | O_CREAT, 0644);
 //       if(fd < 0)
 //       {
 //       	perror("open failed");
 //       	return 1;
 //       }
 //       const char* msg = "hello world!";
 //       write(fd,msg,strlen(msg));
 //       

 //       close(fd);
 //       return 0;
 //}
 //
 //
 //
 //read
// int main()
//{
//	int fd = open("./log.txt", O_RDONLY);
//	if(fd < 0)
//	{
//		perror("open");
//		return 1;
//	}
//	char buffer[1024];
//	size_t s = read(fd,buffer,sizeof(buffer)-1);
//	if(s > 0)
//	{
//		buffer[s] = 0;
//		cout << buffer;
//	}
//
//	close(fd);
//}
//
//
 int main()
{
	close(1);
	int fd = open("./log.txt", O_CREAT | O_WRONLY, 0644);
	printf("fd = %d\n", fd);
	printf("It should be printed in the screen.");


}
