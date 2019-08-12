#define _MY_DEAL_C

#include "my_deal.h"

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
#include "my_talk.h"

void *deal(void *recv_pack) {
    pthread_detach(pthread_self());
	PACK               *pack;
	int                i;
    BOX                *tmp = box_head;
	MYSQL              mysql;
	mysql = accept_mysql();
	pack = (PACK*)recv_pack;
	switch(pack->type)
	{
		case LOGIN:
			{
				if (login(pack, mysql) != 0) {
                    pack->type = ACCOUNT_ERROR;
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
					strcpy(pack->data.write_buff, "password error");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
				} else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "good");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                    while (tmp != NULL) {
                        if (tmp->recv_account == pack->data.send_account) {
                            break;
                        }
                        tmp = tmp->next;
                    }
                    if (tmp == NULL) {
                        tmp = (BOX *)malloc(sizeof(BOX));
                        tmp->recv_account = pack->data.send_account;
                        tmp->talk_number = tmp->friend_number = 0;
                        tmp->next = NULL;
                        if (box_head == NULL) {
                            box_head = box_tail = tmp;
                        } else {
                            box_tail->next = tmp;
                            box_tail = tmp;
                        }
                        if (send(pack->data.recv_fd, tmp, sizeof(BOX), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                    } else {
                        if (send(pack->data.recv_fd, tmp, sizeof(BOX), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        tmp->friend_number = 0;
                        tmp->talk_number = 0;
                    }
                }
                break;
			}
        case REGISTERED:
            {
                registered(pack, mysql);
                memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                strcpy(pack->data.write_buff, "registered success!!");
                if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                    my_err("send", __LINE__);
                }
                
                break;
            }
        case CHANGE_PASSWORD:
            {
                if (change_password(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);        
                    }
                }
                break;
            }
        case ADD_FRIEND:
            {
                if (add_fir(pack, mysql) == 0) {
                    pack->type = ADD_FRIEND;
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK) , 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.recv_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);        
                    } 
                }
                break;
            }
        case FRIENDS_PLZ:
            {
                friends_plz(pack, mysql);
                break;
            }
        case DEL_FRIEND:
            {
                if (del_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
            }
        case BLACK_FRIEND:
            {
                if (black_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case WHITE_FRIEND:
            {
                if (white_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case CARE_FRIEND:
            {
                if (care_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case DISCARE_FRIEND:
            {
                if (discare_friend(pack, mysql) == 0) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    } 
                }
                break;
            }
        case LOOK_LIST:
            {
                FRIEND *list;
                if ((list = look_list(pack, mysql)) != NULL) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "success");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                    if (send(pack->data.send_fd, list, sizeof(FRIEND), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                    if (send(pack->data.send_fd, list, sizeof(FRIEND), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;       
            }
        case SEND_FMES:
            {
                if (send_fmes(pack, mysql) == -1) {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#fail");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                } else {
                    memset(pack->data.write_buff, 0, sizeof(pack->data.write_buff));
                    strcpy(pack->data.write_buff, "#succss");
                    if (send(pack->data.send_fd, pack, sizeof(PACK), 0) < 0) {
                        my_err("send", __LINE__);
                    }
                }
                break;
            }
	}
	close_mysql(mysql);
}

