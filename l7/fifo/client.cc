#include"com.h"

int main()
{
	int fd = open(MY_FIFO, O_WRONLY);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}	
	while(1)
	{
		cout << "Please input sth:> " << endl;
		fflush(stdout);
		char buffer[64] = {0};
		ssize_t s = read(0, buffer ,sizeof(buffer)-1);
		if(s > 0)
		{
			buffer[s-1] = 0;
			cout << buffer << endl;

			write(fd, buffer, strlen(buffer));

		}
 	}
	close(fd);

	return 0;
}
