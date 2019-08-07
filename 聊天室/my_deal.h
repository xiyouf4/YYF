#ifndef _MY_DEAL_H
#define _MY_DEAL_H
#include <stdio.h>
#include "my_pack.h"
#include "my_friends.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "my_err.h"
#include "my_login.h"
#include "my_mysql.h"


void *deal(void *recv_pack) {
    pthread_detach(pthread_self());
	PACK               *pack;
	int                i;
	MYSQL              mysql;
	mysql = accept_mysql();
	pack = (PACK*)recv_pack;
	switch(pack->type)
	{
		case LOGIN:
			{
				if(login(pack, mysql) != 0){
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
					strcpy(pack->data.write_buff, "password error");
                    printf("%s\n",pack->data.write_buff);
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
				} else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "good");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
			}
        case REGISTERED:
            {
                registered(pack, mysql);
                printf("注册成功\n");
            }
        case ADD_FRIEND:
            {
                   if (add_fir(pack, mysql) == 0) {
                        send(pack->data.recv_fd, pack, sizeof(PACK), 0);
                   }
            }
	}
	close_mysql(mysql);}


#endif
