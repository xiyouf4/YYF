#ifndef _MY_SEND_H
#define _MY_SEND_H
#include "my_send.c"


#define MAXIN 1024


void my_send(const char *sed_string, int sock_fd);
char *s_gets(char *str);

#endif
