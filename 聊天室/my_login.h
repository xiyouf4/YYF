#ifndef _MY_LOGIN_H
#define _MY_LOGIN_H
#include "my_login.c"
#include "my_pack.h"

int Login(PACK *pack, MYSQL mysql1);
int Registered(PACK *pack, MYSQL mysql1);
int ChangePassword(PACK *pack, MYSQL mysql1);
int FindPassword(PACK *pack, MYSQL mysql1);

#endif
