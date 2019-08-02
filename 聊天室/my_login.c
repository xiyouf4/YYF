#define _MY_LOGIN_C
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "my_pack.h"
#include "my_err.h"

int login(PACK *pack, MYSQL mysql1)
{	
	int                i;
        int                ret;
	int                cont = 0;
	unsigned int       num_fields;
	char               inedx[100];	
	PACK               *recv_pack = pack;
	MYSQL              mysql = mysql1;
	MYSQL_RES          *result = NULL;
	MYSQL_ROW          row;
	MYSQL_FIELD        *field;

	sprintf(inedx, "select *from user_data where account = %d", recv_pack->data.send_account);
	
	ret = mysql_query(&mysql, inedx);

	if(!ret){
		result = mysql_store_result(&mysql);
		num_fields = mysql_num_fields(result);

		while(row = mysql_fetch_row(result)){
			if(strcmp(row[2], pack->data.messages) == 0){
				strcpy(pack->data.send_user, row[1]);
				pack->data.send_fd = atoi(row[4]);
				memset(inedx, 0, sizeof(inedx));
				sprintf(inedx, "update user_data set user_state = 1 where account = %d", pack->data.send_account);
				mysql_query(&mysql, inedx);
				mysql_free_result(result);
				return 0;
			}
			cont++;
		}
		if(cont == 0){
			return -1;
		}
	}
	else{
		printf("query fail\n");
		return -1;
	}
}

int registered(PACK *packi, MYSQL mysql)
{
	FILE               fp;
	int	
}
