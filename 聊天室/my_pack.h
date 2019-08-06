#ifndef _MY_PACK_H
#define _MY_PACK_H
#include "my_send.h"

#define MAX_FRIEND 500

#define LOGIN 1
#define REGISTERED 2
#define FIND_PASSWORD 3
#define CHANGE_PASSWORD 4
#define ADD_FRIEND 5
#define DEL_FRIEND 6
#define LOOK_LIST 7
#define SEND_FMES 8
#define CREATE_GROUP 9
#define ADD_GROUP 10
#define EXIT_GROUP 11
#define DEL_MEMBER 12
#define SET_ADMIN 13
#define DEL_ADMIN 14
#define SEND_GMES 15
#define LOOK_GROUP 16
#define DIS_GROUP 17
#define SEND_FILE 18


typedef struct {
    int               send_fd;              
    int               recv_fd;
    int               send_account;
    int               recv_account;
    char              send_user[20];
    char              recv_user[20];
    char              messages[MAXIN];
} DATA;


typedef struct {
    int               friend_number;
    int               friend_account[500];
    char              friend_nickname[500][20];
    int               friend_state[500];
} FRIEND;


typedef struct {
    int               group_account;
    int               group_member_number;
    int               group_member_account[500];	
    char              group_member_nickname[500][20];
    int               group_member_state[500];
} GROUP;


typedef struct {
    int               type;
    DATA              data;
} PACK;


typedef struct {
    char              filename[30];
    int               filesize;
    char              file[MAXIN];
    int               send_account; 	
    char              send_nickname[20];
} FLE;


int               user_number;
int               group_number;


#endif
