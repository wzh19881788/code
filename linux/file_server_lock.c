#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define oops(m, x) {perror(m); exit(x);}
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
	time_t now;
	char *message;

	if (ac != 2){
		printf("the arg number should be 2 arguments.\n");
	}

	if ((fd = open(av[1], O_CREAT|O_TRUNC|O_WRONLY,06444)) == -1)
	{
		oops("open", -1);
	}

	while(1)
	{
		time(&now);
		message = ctime(&now);

		lock_operation(fd, F_WRLCK);
		
		if (lseek(fd, 0L, SEEK_SET) == -1)
			oops("lseek", -2);

		if (write(fd, message, strlen(message)) == -1)
			oops("write", -3);
		
		lock_operation(fd, F_UNLCK);
		sleep(1);	
	}
	
	return 0;
}




