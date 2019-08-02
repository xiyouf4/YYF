#define _MY_SQL_C

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include "my_mysql.h"
#include "my_err.h"

MYSQL accept_mysql(void)
{
	MYSQL               mysql;
	
	if(NULL == mysql_init(&mysql)){
		my_err("mysql_init", __LINE__);
	}

	//初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0){
		my_err("mysql_library_init", __LINE__);
	}

	//连接数据库
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "520520cw...", "happy", 0, NULL, 0)){
		my_err("mysql_real_connect", __LINE__);
	}

	//设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		my_err("mysql_set_character_set", __LINE__);
	}
	
	printf("连接mysql数据库成功!\n");
	return mysql;
}

int use_mysql(const char *string, MYSQL mysql1)
{
	int                 i;
	int                 ret;
	unsigned int        num_fields;
	MYSQL               mysql = mysql1;
	MYSQL_RES           *result = NULL;
	MYSQL_ROW           row;
	MYSQL_FIELD         *field;

	ret = mysql_query(&mysql, string);
	if(!ret){
		result = mysql_store_result(&mysql);
		if(result){
			num_fields = mysql_num_fields(result);
			while((field = mysql_fetch_field(result))){
				printf("%-20s", field->name);
			}
			printf("\n");

			while(row = mysql_fetch_row(result)){
				for(i = 0; i < num_fields; i++){
					if(row[i]){
						printf("%-20s", row[i]);
					}
				}
				printf("\n");
			}
		}
		mysql_free_result(result);
	}
	else{
		printf("query fail\n");
		return -1;
	}
	return 0;
}

int close_mysql(MYSQL mysql)
{
	mysql_close(&mysql);
	mysql_library_end();
	printf("end\n");
	return 0;
}
