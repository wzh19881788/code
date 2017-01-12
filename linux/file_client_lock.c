#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define oops(m, x) {perror(m); exit(x);}
#define BUFLEN 1024

void lock_operation(int fd, int op)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len = 0;
	lock.l_pid = getpid();
	lock.l_type = op;

	if (fcntl(fd, F_SETLKW, &lock) == -1)
		oops("lock operation",-3);
}

int main(int ac, char*av[])
{
	int fd;
	int nread;
	char buf[BUFLEN];

	if (ac != 2){
		printf("the arg number should be 2 arguments.\n");
	}

	if ((fd = open(av[1], O_RDONLY, 06444)) == -1)
	{
		oops("open", -1);
	}
		lock_operation(fd, F_RDLCK);
		
		while((nread = read(fd, buf, BUFLEN)) > 0)
			write(1, buf, nread);
		
		lock_operation(fd, F_UNLCK);
		
		sleep(1);	
	
	
	return 0;
}

