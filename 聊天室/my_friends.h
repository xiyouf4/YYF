#ifndef _MY_FRIENDS_H
#define _MY_FRIENDS_H

#include "my_pack.h"
#include <stdio.h>
#include "my_mysql.h"
#include <string.h>
#include <stdlib.h>
#include "my_login.h"
#include "my_err.h"
#include <mysql/mysql.h>

int add_fir(PACK *pack, MYSQL mysql1) {
    PACK            *recv_pack = pack;
    MYSQL           mysql = mysql1;
    MYSQL_RES       *result;
    MYSQL_ROW       row;
    BOX             *tmp = box_head;
    int             ret;
    char            need[100];
    
    sprintf(need, "select *from user_data where account = %d", recv_pack->data.recv_account);
    pthread_mutex_lock(&mutex);
    ret = mysql_query(&mysql, need);
    if (!ret) {
        result = mysql_store_result(&mysql);
        if (result) {
            row = mysql_fetch_row(result);
        }
        memset(need, 0, sizeof(need));
        sprintf(need, "select *from friends where user = %d and friend_user = %d", recv_pack->data.send_account, recv_pack->data.recv_account);
        ret = mysql_query(&mysql, need);
        if (!ret) {
            // 如果可以查到数据说明对方已经是好友了 
            result = mysql_store_result(&mysql);
            if (result) {
                return 1;
            }
            if (atoi(row[3]) == 0) {
                while (tmp->recv_account != recv_pack->data.recv_account && tmp != NULL) {
                    tmp = tmp->next;
                }
                memset(need, 0, sizeof(need));
                sprintf(need, "账号为%d,昵称为%s的用户发来好友请求\n", recv_pack->data.send_account, recv_pack->data.send_user);
                if (tmp != NULL) {
                    tmp->plz_account[tmp->friend_number++] = recv_pack->data.send_account;
                    strcpy(tmp->write_buff[tmp->friend_number], need);
                } else {
                    tmp = (BOX *)malloc(sizeof(BOX));
                    tmp->recv_account = recv_pack->data.recv_account;
                    tmp->friend_number = 0;
                    tmp->talk_number = 0;
                    strcpy(tmp->write_buff[tmp->friend_number++], need);
                    if (box_head == NULL) {
                        box_head = box_tail = tmp;
                        box_tail->next = NULL;
                    } else {
                        box_tail->next = tmp;
                        box_tail = tmp;
                        box_tail->next = NULL;
                    }
                }
                pthread_mutex_unlock(&mutex);
            } else {
                recv_pack->data.send_fd = atoi(row[4]);
                strcpy(recv_pack->data.recv_user, row[1]);
                
                pthread_mutex_unlock(&mutex);
                
                return 0;
            }
        }
    }
}


//int del_friend(PACK *pack, MYSQL mysql1);
//int look_list(PACK *pack, MYSQL mysql1);

#endif
