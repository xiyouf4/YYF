#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int 		     i;
	int 		     ret;
	unsigned int         num_fields;
	MYSQL                mysql;
	MYSQL_RES            *res = NULL;
	MYSQL_ROW	     row;
	MYSQL_FIELD	     *field;
	
	if(NULL == mysql_init(&mysql)){
		printf("mysql_init()fail\n");
		return -1;
	}

	//初始化数据库
	if(mysql_library_init(0, NULL, NULL) != 0){
		printf("mysql_library_init()failed\n");
		return -1;	
	}

	//连接数据库
	if(NULL == mysql_real_connect(&mysql, "127.0.0.1", "root", "520520cw...", "talk", 0, NULL, 0)){
		printf("mysql_real_connect()fail\n");
		return -1;
	}

	//设置中文字符集
	if(mysql_set_character_set(&mysql, "utf8") < 0){
		printf("set fail\n");
		return -1;
	}

	//查询
	char string[100] = "update user_data set 昵称 = \"xzwb\" where 账号 = \"00000000\"";
	ret = mysql_query(&mysql, string);
	if(!ret){
		res = mysql_store_result(&mysql);
		if(res){
			num_fields = mysql_num_fields(res);
			while((field = mysql_fetch_field(res))){
				if(strcmp(field->name, "密码") == 0){
					continue;
				}
				printf("%-20s", field->name);
			}
			printf("\n");
				
			while(row = mysql_fetch_row(res)){
				for(i = 0; i < num_fields; i++){
					if(i == 2){
						continue;
					}
					if(row[i]){
						printf("%-20s",row[i]);
					}
				}
				printf("\n");
			}
		}
		mysql_free_result(res);
	}
	else{
		printf("query fail\n");
	}
	
	mysql_close(&mysql);
	mysql_library_end();
	printf("end\n");
	return 0;

}
