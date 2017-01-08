#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define oops(m,x){perror(m); exit(x);}

void showdata( char *fname ,int fd)
{
	char buf[BUFSIZ];
	int n;

	printf("%s: ", fname);
	fflush(stdout);	

	n = read(fd, buf, BUFSIZ) ;
	if (n == -1)
		oops("read", 5);

	write(1, buf, n);
	write(1, "\n", 1);

}

int main(int ac, char*av[])
{
	int fd1, fd2;
	struct timeval timeout;
	fd_set readfds;
	int maxfd;
	int retval;
	
	if (ac != 4)
		oops("not enough args", 2);

	fd1 = open(av[1], O_RDONLY);
	if (fd1 == -1)
		oops("not enough args", 3);

	
	fd2 = open(av[2], O_RDONLY);
	if (fd2 == -1)
		oops("not enough args", 4);

	maxfd = fd1>fd2 ? fd1 : fd2;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(fd1, &readfds);
		FD_SET(fd2, &readfds);
		
		timeout.tv_sec  = atoi(av[3]);
		timeout.tv_usec = 0;

		retval = select(maxfd, &readfds, NULL, NULL, &timeout);
		if (retval == -1)
		{
			oops("select", 5);
		}
		else if (retval > 0)
		{
			if (FD_ISSET(fd1, &readfds))
				showdata(av[1], fd1);
			
			if (FD_ISSET(fd2, &readfds))
				showdata(av[2], fd2);
		}
		else 
		{
			printf("no input data for %d seconds.\n", atoi(av[3]) );	
		}
	}

	return 0;
}






