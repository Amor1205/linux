#include <stdio.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int efd = eventfd(0, EFD_CLOEXEC|EFD_NONBLOCK);
	if(efd < 0){
		perror("eventfd failed");
		return -1;
	}
	uint64_t val = 1;
	write(efd, &val, sizeof(val));
	write(efd, &val, sizeof(val));
	write(efd, &val, sizeof(val));
	uint64_t res = 0;
	read(efd, &res, sizeof(res));
	printf("%d", res);	
	close(efd);
	return 0;
}
