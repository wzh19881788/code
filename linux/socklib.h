#ifndef _SOCKLIB_H_
#define _SOCKLIB_H_

int make_server_socket(int);
int make_server_socket_q(int, int);
int connect_to_server(char *host, int portnum);

#endif
