#ifndef _MY_GROUP_H
#define _MY_GROUP_H

#include "my_pack.h"

#include <mysql/mysql.h>

// 创建群
int create_group(PACK *pack, MYSQL mysql1);
// 加群
int add_group(PACK *pack, MYSQL mysql1);
// 退群
int exit_group(PACK *pack, MYSQL mysql1);

#endif
