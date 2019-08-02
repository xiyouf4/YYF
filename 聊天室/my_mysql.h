#ifndef _MY_MYSQL_H
#define _MY_MYSQL_H
#include "my_mysql.c"
#include <mysql/mysql.h>

MYSQL accept_mysql(void);
int use_mysql(const char *string, MYSQL mysql);
int close_mysql(MYSQL mysql);

#endif
