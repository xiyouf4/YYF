#define _MY_DEAL_C
#include "my_pack.h"
#include <stdio.h>
#include "my_mysql.h"
#include "my_send.h"
#include "my_login.h"
#include "my_err.h"

void *deal(void *recv_pack)
{
	PACK               *pack;
	int                i;
	MYSQL              mysql;
	mysql = accept_mysql();
	pack = (PACK*)recv_pack;
	switch(pack->type)
	{
		case LOGIN:
			{
				if(login(pack, mysql) == 0){
					printf("匹配成功\n");
				}
				else{
					printf("密码错误\n");
				}
			}
	}
	close_mysql(mysql);
}
