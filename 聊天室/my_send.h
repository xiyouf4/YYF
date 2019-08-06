#ifndef _MY_SEND_H
#define _MY_SEND_H
#include "my_send.c"


#define MAXIN 1024


void MySend(const char *sed_string, int sock_fd);
char *SGets(char *str);

#endif
