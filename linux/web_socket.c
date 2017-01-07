/*
*	web_socket learnt from the book
* 	gcc web_socket.c socklib.c -lpthread -o twebserv 
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "web_socket.h"
void header(FILE* fp, char* content_type)
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
	if (content_type)
		fprintf(fp, "Content_type: %s\r\n", content_type);
}

void read_til_crn(FILE *fp)
{
	char buf[BUFSIZ];	
	while(fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n") != 0);
}

int process_rq(char* rq, int fd)
{
	char cmd[BUFSIZ], arg[BUFSIZ];

	if (fork() != 0) //¸¸½ø³ÌÍË³ö
	{
		return 2;
	}
	strcpy(arg,"./");

	if (sscanf(rq, "%s %s", cmd, arg+2) != 2)
	{
		return -1;
	}
	if (strcmp(cmd, "GET") != 0)
		cannot_do(fd);
	else if(not_exist(arg))
		do_404(arg, fd);
	else if(isadir(arg) )
		do_ls(arg, fd);
	else if(ends_in_cgi(arg))
		do_exec(arg, fd);
	else
		do_cat(arg, fd);

	return 0;
}

void cannot_do(int fd)
{
	FILE *fp = fdopen(fd, "w");	
		
	fprintf(fp, "HTTP/1.0 501 NOT Implemented\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");

	fprintf(fp, "That command is not yet implented\r\n");
	fclose(fp);
}

int not_exist(char *f)
{
	struct stat info;
	return (stat(f, &info) == -1);
}

void do_404(char *item, int fd)
{
	FILE *fp = fdopen(fd, "w");
	
	fprintf(fp, "HTTP/1.0 404 NOT FOUND.\n");
	fprintf(fp, "Content-type: text plain\r\n");
	fprintf(fp, "\r\n");
	
	fprintf(fp, "The item you requested :%s\r\n is not found.\r\n", item);
	fclose(fp);
}

int isadir(char *f)
{
	struct stat info;
	return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}


void do_ls(char *dir, int fd)
{
	FILE *fp;
	fp = fdopen(fd, "w");

	header(fp, "text/plain");
	fprintf(fp, "\r\n");
	fflush(fp);

	dup2(fd, 1);
	dup2(fd, 2);

	close(fd);
	execlp("ls", "ls", "-l", dir, NULL);
	perror(dir);
	exit(1);
}

char *file_type(char *f)
{
	char *cp;
	if ( (cp = strchr(strchr(f, '.')+1, '.')) != NULL )
	{
		return cp+1;
	}
	return NULL;
}

int ends_in_cgi(char *f)
{
	return (strcmp(file_type(f), "cgi") == 0);
}

void do_exec(char *prog, int fd)
{
	FILE *fp;
	fp = fdopen(fd, "w");

	header(fp, NULL);
	fflush(fp);

	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	execl(prog, prog, NULL);
	perror(prog);
}

void do_cat(char *f, int fd)
{
	char *extension = file_type(f);
	char *content = "text/plain";
	FILE *fpsock, *fpfile;
	int c;

	if (strcmp(extension, "html") == 0)
		content = "text/html";
	else if (strcmp(extension, "gif") == 0)
		content = "image/gif";
	else if (strcmp(extension, "jpg") == 0)
		content = "image/jpg";
	else if (strcmp(extension, "jpeg") == 0)
		content = "image/jpeg";
	
	fpsock = fdopen(fd, "w");
	fpfile = fopen(f,"r");

	if (fpsock != NULL && fpfile != NULL)
	{
		header(fpsock, content);
		fprintf(fpsock, "\r\n");

		while( (c = getc(fpfile)) != EOF)
			putc(c, fpsock);
	
		fclose(fpsock);
		fclose(fpfile);
	}
	exit(0);
}

int main(int ac, char *av[])
{
	
	int sock, fd;
	FILE *fpin;
	char requests[BUFSIZ];
	
	if(ac == 1) {
		fprintf(stderr, "usage: ws portnum.\n");
		exit(1);
	}
		
	sock = make_server_socket(atoi(av[1]));
	if (sock == -1) exit(2);

	while(1)
	{
		fd = accept(sock, NULL, NULL);
		fpin = fdopen(fd, "r");
	
		fgets(requests, BUFSIZ, fpin);
		
		printf("got a call: request = %s", requests);
	
		read_til_crn(fpin);
		process_rq(requests, fd);

		fclose(fpin);
	}
}







