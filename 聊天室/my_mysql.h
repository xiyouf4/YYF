#ifndef _MY_MYSQL_H
#define _MY_MYSQL_H
#include "my_mysql.c"
#include <mysql/mysql.h>

MYSQL AcceptMysql(void);
int CloseMysql(MYSQL mysql);
int UseMysql(const char *string, MYSQL mysql1);

#endif
