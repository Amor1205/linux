#include"com.h"

int main()
{
	umask(0);
	if(mkfifo(MY_FIFO,0666)<0)
	{
		perror("mkfifo");
		return 1;
	}
	int fd = open(MY_FIFO, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}
	while(1)
	{
		char buffer[64] = {0};
		ssize_t s = read(fd, buffer, sizeof(buffer)-1);
		if(s > 0)
		{
			buffer[s] = 0;
			if(strcmp(buffer, "show") == 0)
			{
				if(fork()==0)
				{
					//child
					execl("/bin/ls","ls","-l", NULL);
					exit(1);
				}
				else{
					waitpid(-1, NULL, 0);
				}
			}
			else if(strcmp(buffer, "run") == 0)
			{
				if(fork() == 0)
				{
					execl("/bin/sl","sl", NULL);
					exit(2);
				}
				else{
					waitpid(-1,NULL,0);
				}
			}
			else{
				cout << "client # " << buffer << endl;
			}
		}
		else if(s == 0)
		{
			cout << "client has quitted" << endl;
			break;
		}
		else 
		{
			cout << "sth failed" << endl;
			//error
			break;
		}
	}
	close(fd);
	return 0;
}
