#ifndef _MY_SEND_H
#define _MY_SEND_H
#include "my_pack.h"
#include <stdio.h>
#include "my_mysql.h"
#include "my_send.h"
#include "my_login.h"
#include "my_err.h"
#include "my_friends.h"
#include <mysql/mysql.h>


#define MAXIN 1024

char *s_gets(char *str) {
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

#endif
