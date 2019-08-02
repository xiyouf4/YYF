#define _MY_SEND_C

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "my_err.h"

#define MAXIN 1024

char *s_gets(char *str)
{
	int i = 0;
	char *ret;
	ret = fgets(str, MAXIN, stdin);
	if(ret)
	{
		while(str[i] != '\n' && str[i] != '\0')
			i++;
		if(str[i] == '\n')
			str[i] = '\0';
		else
			while(getchar() != '\n')
				continue;
	}
	return ret;
}

void my_send(const char *sed_string, int sock_fd)
{
	if(send(sock_fd, sed_string, sizeof(sed_string), 0) < 0)
		my_err("send", __LINE__);
}

