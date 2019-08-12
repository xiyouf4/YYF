#define _MY_TALK_C

#include "my_talk.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql/mysql.h>

#include "my_pack.h"
#include "my_err.h"
#include "my_mysql.h"
#include "my_friends.h"
#include "my_login.h"

int send_fmes(PACK *pack, MYSQL mysql1) {
    char            need[100];
    MYSQL           mysql = mysql1;
    PACK            *recv_pack = pack;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    BOX             *tmp;

    pthread_mutex_lock(&mutex);
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    mysql_query(&mysql, need);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if (!row) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    strcpy(recv_pack->data.recv_user, row[1]);
    if ((atoi(row[3]) == 1)) {
        recv_pack->type = RECV_FMES;
        recv_pack->data.recv_fd = atoi(row[4]);
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from friends where user = %d", recv_pack->data.recv_account);
        mysql_query(&mysql, need);
        result = mysql_store_result(&mysql);
        while (row = mysql_fetch_row(result)) {
            if (atoi(row[1]) != recv_pack->data.send_account) {
                continue;
            }
            if (atoi(row[2]) == BLACK) {
               recv_pack->type = SEND_FMES;
               pthread_mutex_unlock(&mutex);
               return -1;
            } else if (atoi(row[2]) == CARE) {
                memset(recv_pack->data.write_buff, 0, sizeof(recv_pack->data.read_buff));
                strcpy(recv_pack->data.write_buff, "ohyeah");
                if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                memset(recv_pack->data.write_buff, 0, sizeof(recv_pack->data.write_buff));
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\")", recv_pack->data.send_account, recv_pack->data.recv_account,recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            } else if(atoi(row[2]) == OK){
                if (send(recv_pack->data.recv_fd, recv_pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\")", recv_pack->data.send_account, recv_pack->data.recv_account, recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            }
        }
        if (!row) {
            pthread_mutex_unlock(&mutex);
            recv_pack->type = SEND_FMES;
            return -1;
        }
    } else {
        tmp = box_head;
        while (tmp != NULL) {
            if (tmp->recv_account = recv_pack->data.recv_account) {
                tmp->send_account[tmp->talk_number] = recv_pack->data.send_account;
                strcpy(tmp->read_buff[tmp->talk_number++], recv_pack->data.read_buff);
                memset(need, 0, sizeof(need));
                sprintf(need, "insert into chat_messages values(%d,%d,\"%s\")", recv_pack->data.send_account, recv_pack->data.recv_account,recv_pack->data.read_buff);
                mysql_query(&mysql, need);
                break;
            }
            tmp = tmp->next;
        }
    }
    pthread_mutex_unlock(&mutex);
    recv_pack->type = SEND_FMES;

    return 0;
}
