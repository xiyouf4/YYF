#define _MY_GROUP_C

#include "my_group.h"

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "my_err.h"
#include "my_pack.h"
#include "my_mysql.h"


int create_group(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    char            need[200];
    PACK            *recv_pack = pack;
    FILE            *fp;
    int             group_number;

    pthread_mutex_lock(&mutex);
    fp = fopen("group_number.txt", "r");
    fread(&group_number, sizeof(int), 1, fp);
    fclose(fp);
    sprintf(need, "insert into groups values(%d,\"%s\",1)", group_number++, recv_pack->data.recv_user);
    mysql_query(&mysql, need);
    recv_pack->data.recv_account = group_number - 1;
    fp = fopen("group_number.txt", "w");
    fwrite(&group_number, sizeof(int), 1, fp);
    fclose(fp);
    memset(need, 0, sizeof(need));
    sprintf(need, "insert into group_members values(%d,\"%s\",%d,\"%s\",1)", recv_pack->data.recv_account, recv_pack->data.recv_user, recv_pack->data.send_account, recv_pack->data.send_user);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int add_group(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[200];
    PACK            *recv_pack = pack;
    int             num;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from groups where group_account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    strcpy(recv_pack->data.recv_user, row[1]);
    num = atoi(row[2]);
    memset(need, 0, sizeof(need));
    sprintf(need, "select *from group_members where group_account = %d and group_member_account = %d", recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "insert into group_members values(%d,\"%s\",%d,\"%s\",0)", recv_pack->data.recv_account, recv_pack->data.recv_user, recv_pack->data.send_account, recv_pack->data.send_user);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "update groups set group_state = %d where group_account = %d", num+1, recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int exit_group(PACK *pack, MYSQL mysql1) {
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    char            need[100];
    int             num;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from groups where group_account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    num = atoi(row[2]);
    memset(need, 0, sizeof(need));
    sprintf(need, "select *from group_members where group_account = %d and group_member_account = %d", recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    memset(need, 0, sizeof(need));
    sprintf(need, "delete from group_members where group_account = %d and group_member_account = %d", recv_pack->data.recv_account, recv_pack->data.send_account);
    mysql_query(&mysql, need);
    memset(need, 0, sizeof(need));
    sprintf(need, "update groups set group_mumber_number = %d where group_account = %d", num-1, recv_pack->data.recv_account);
    mysql_query(&mysql, need);

    pthread_mutex_unlock(&mutex);
    
    return 0;
}
