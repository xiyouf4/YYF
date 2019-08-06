#define _MY_LOGIN_C

#include "my_pack.h"
#include "my_err.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>


/* 服务器端登录函数 */
int Login(PACK *pack, MYSQL mysql1) {	
    int                i;
    int                ret;
    int                cont = 0;
    char               inedx[100];	
    PACK               *recv_pack = pack;
    MYSQL              mysql = mysql1;
    MYSQL_RES          *result = NULL;
    MYSQL_ROW          row;

    sprintf(inedx, "select *from user_data where account = %d", recv_pack->data.send_account);
	
    ret = mysql_query(&mysql, inedx);

    if (!ret) {
        result = mysql_store_result(&mysql);
        while (row = mysql_fetch_row(result)) {
            if (strcmp(row[2], pack->data.read_buff) == 0) {
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
        if (cont == 0) {
           
            return -1;
        }   
    } else {
        printf("query fail\n");
        
        return -1;
    }
    
    return -1;
}


/* 服务器端注册函数 */
int Registered(PACK *pack, MYSQL mysql1) {
    FILE               *fp;
    MYSQL              mysql = mysql1;	
    char               need[100];
    PACK               *recv_pack = pack;
    int                user_number;
	
    /* 在user_number.txt文件中存放着用户的个数,数量从1000000开始 */
    if ((fp = fopen("user_number.txt", "r")) == NULL) {
        printf("打开文件失败\n");
        exit(-1);
    }
    fread(&user_number, sizeof(int), 1, fp);
	
    sprintf(need, "insert into user_data values(%d,\"%s\",\"%s\",%d,%d)", user_number++, recv_pack->data.send_user, recv_pack->data.read_buff, 0, recv_pack->data.send_fd);
	
    mysql_query(&mysql, need);
    fclose(fp);
    if ((fp = fopen("user_number.txt", "w")) == NULL) {
        printf("打开文件失败\n");
        exit(-1);
    }
    fwrite(&user_number, sizeof(int), 1, fp);
    fclose(fp);
    
    return 0;
}



/* 服务器端修改密码函数 */
int ChangePassword(PACK *pack, MYSQL mysql1) {
    int                ret;
    int                cont = 0;
    MYSQL_RES          *result = NULL;
    PACK               *recv_pack = pack;
    MYSQL              mysql = mysql1;
    char               need[100];
    MYSQL_ROW          row;

    sprintf(need, "select *from user_data where account = %d", recv_pack->data.send_account);
    ret = mysql_query(&mysql, need);
    if (!ret) {
        result = mysql_store_result(&mysql);
        if (result) {
            while (row = mysql_fetch_row(result)) {
                if (strcmp(recv_pack->data.read_buff, row[2]) == 0) {
                    memset(need, 0, sizeof(need));
                    sprintf(need, "update user_data set password = \"%s\" where account = %d", recv_pack->data.write_buff, recv_pack->data.send_account);	
                    mysql_query(&mysql, need);		
                    mysql_free_result(result);
                    return 0;
                }
                cont++;
            }
            if (cont == 0) {
                return -1;
            }
        }
    } else {
        printf("query fail!\n");
        return -1;
    }
	
    return -1;
}

/*服务器端找回密码*/
int FindPassword(PACK *pack, MYSQL mysql1)
{
	 
}
