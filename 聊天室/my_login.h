#ifndef _MY_LOGIN_H
#define _MY_LOGIN_H
#include "my_login.c"
#include "my_pack.h"

int login(PACK *pack, MYSQL mysql1);
int registered(PACK *pack, MYSQL mysql1);
int change_password(PACK *pack, MYSQL mysql1);
int find_password(PACK *pack, MYSQL mysql1);

#endif
