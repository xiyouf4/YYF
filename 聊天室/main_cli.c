#include <stdio.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "my_pack.h"
#include "my_socket.h"
#include "my_err.h"
#include "my_mune.h"

PACK *send_pack;
PACK *recv_pack;
BOX *box;

/* 用来发送数据的线程 */
void *thread_read(void *sock_fd) {
    int               choose;
    char              password[20];
    
    send_pack = (PACK *)malloc(sizeof(PACK));
    /* 1为登录,2为注册,3为退出 */
    while (1) {
        login_mune();
        printf("请选择你需要的功能:\n");
        scanf("%d", &choose);
        switch (choose) {
            case 1:
                   {
                       send_pack->type = LOGIN;
                       printf("请输入账号:\n");
                       scanf("%d", &send_pack->data.send_account);
                       getchar();
                       printf("请输入密码:\n");
                       scanf("%s", send_pack->data.read_buff);
                       if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                           my_err("send", __LINE__);
                       }
                       pthread_mutex_lock(&mutex_cli);
                       pthread_cond_wait(&cond_cli, &mutex_cli);
                       pthread_mutex_unlock(&mutex_cli);
                       printf("%s\n", send_pack->data.write_buff);
                       break;
                   }
           case 2:
                   {
                       send_pack->type = REGISTERED;
                       printf("请输入昵称:\n");
                       scanf("%s", send_pack->data.send_user);
                       getchar();
                       printf("请输入密码:\n");
                       scanf("%s", send_pack->data.read_buff);
                       getchar();
                       printf("请再次输入密码:\n");
                       scanf("%s", password);
                       if (strcmp(password, send_pack->data.read_buff) == 0) {
                           printf("两次输入一制OK\n");
                           if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                               my_err("send", __LINE__);
                           }
                           pthread_mutex_lock(&mutex_cli);
                           pthread_cond_wait(&cond_cli, &mutex_cli);
                           pthread_mutex_unlock(&mutex_cli);
                       } else {
                           printf("两次密码不一致!!!\n") ;
                           printf("按下回车继续.......");
                           choose = 5;
                           getchar();
                           getchar();
                       }
                       break;
                   }
           case 3:
                   {
                       send_pack->type = EXIT;
                       if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                           my_err("send", __LINE__);
                       }
                       pthread_exit(0);
                   }
           default:
                   {
                       printf("狗东西,好好输入!!!\n");
                       printf("输入回车继续......");
                       getchar();
                       getchar();
                       break;
                   }
        }
        if (choose > 3 || choose < 1) {
            continue;   
        } else if (choose == 1) {
            if (strcmp(send_pack->data.write_buff, "password error") == 0) {
                printf("密码错误或账号错误!!\n按下回车继续.....");
                getchar();
                getchar();
                continue;
            } else {
                printf("登陆成功!!\n");
                printf("按下回车继续......\n");
                getchar();
                getchar();
                break;
            }
        } else if (choose == 2) {
            printf("注册成功!!\n");
            printf("您的账号为:%d\n", send_pack->data.send_account);
            printf("按下回车继续.......");
            getchar();
            getchar();
            continue;
        }
    }
    while (1) {
        mune();
        printf("\n");
        scanf("%d", &choose);
        getchar();
        switch(choose) {
            case 1:
                    {
                        send_pack->type = CHANGE_PASSWORD;
                        printf("请输入原始密码:\n");
                        scanf("%s", send_pack->data.read_buff);
                        getchar();
                        printf("请输入修改后的密码:\n");
                        scanf("%s", send_pack->data.write_buff);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        printf("%s\n", recv_pack->data.write_buff);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("修改成功!!!\n");
                            printf("按下回车继续......\n");
                            getchar();
                        } else {
                            printf("修改失败!!!\n");
                            printf("按下回车继续.......\n");
                            getchar();
                        }
                        break;
                    }
            case 2:
                    {
                        send_pack->type = ADD_FRIEND;
                        printf("请输入你想添加好友的账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("发送成功等待对方确认!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        } else {
                            printf("对方已经是你的好友或者对方不存在!!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 3:
                    {
                        send_pack->type = DEL_FRIEND;
                        printf("请输入想要删除好友的账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("删除成功!!!\n");
                            printf("按下回车键继续..........");
                            getchar();
                        } else {
                            printf("你没有账号为%d的好友!!!\n", send_pack->data.recv_account);
                            printf("按下回车继续........");
                            getchar();
                        }
                        break;
                    }
            case 4:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        send_pack->type = FRIENDS_PLZ;
                        if (box->friend_number == 0) {
                            printf("消息盒子里没有好友请求!!\n");
                            printf("输入回车继续......");
                            pthread_mutex_unlock(&mutex);
                            getchar();
                        } else {
                            for (int i = 0; i < box->friend_number; ++i) {
                                printf("%s\n", box->write_buff[i]);
                                send_pack->data.recv_account = box->plz_account[i];
                                printf("请选择: 1. 接受 2. 拒绝 3. 忽略\n");
                                scanf("%d", &choose);
                                getchar();
                                if (choose == 3) {
                                    continue;
                                } else if (choose == 1) {
                                    strcpy(send_pack->data.read_buff, "agree");
                                    if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                        my_err("send", __LINE__);
                                    }
                                } else if (choose == 2) {
                                    strcpy(send_pack->data.read_buff, "disagree");
                                    if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                        my_err("send", __LINE__);
                                    }
                                }
                            }
                            box->friend_number = 0;
                            printf("处理完毕!!\n");
                            printf("回车键继续.......");
                            pthread_mutex_unlock(&mutex);
                            getchar();
                        }
                        break;
                    }
            case 5:
                    {
                        send_pack->type = BLACK_FRIEND;
                        printf("请输入你想要拉黑的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友已经被拉黑!!!\n", send_pack->data.recv_account);
                            printf("按下回车继续.......");
                            getchar();
                        } else {
                            printf("对方不是你的好友或对方不存在!!!\n");
                            printf("按下回车继续...........");
                            getchar();
                        }
                        break;
                    }
            case 6:
                    {
                        send_pack->type = WHITE_FRIEND;
                        printf("请输入你想从黑名单里去除的好友:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友从黑名单里剔除!!\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友或者这个好友不在黑名单!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 7:
                    {
                        send_pack->type = CARE_FRIEND;
                        printf("请输入你想要加入特别关心的好友账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友加入特别关心\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 8:
                    {
                        send_pack->type = DISCARE_FRIEND;
                        printf("请输入你想要取关的好友账号:\n");
                        scanf("%d", &send_pack->data.recv_account);
                        getchar();
                        if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                            my_err("send", __LINE__);
                        }
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_wait(&cond_cli, &mutex_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        if (strcmp(send_pack->data.write_buff, "success") == 0) {
                            printf("账号为%d的好友取消特别关心\n", send_pack->data.recv_account);
                            printf("按下回车继续......");
                            getchar();
                        } else {
                            printf("你没有这个好友!!\n");
                            printf("按下回车键继续.......");
                            getchar();
                        }
                        break;
                    }
            case 23:
                    {
                            send_pack->type = EXIT;
                            if (send(*(int *)sock_fd, send_pack, sizeof(PACK), 0) < 0) {
                                my_err("send", __LINE__);
                            }
                            pthread_exit(0);
                    }
        }
    }
}

void *thread_box(void *sock_fd) {
    if (recv(*(int *)sock_fd, box, sizeof(BOX), 0) < 0) {
        my_err("recv", __LINE__);
    }
    pthread_exit(0);
}

void *thread_write(void *sock_fd) {

    pthread_t pid;    
    box = (BOX *)malloc(sizeof(BOX));
    recv_pack = (PACK*)malloc(sizeof(PACK));
    while (1) {
        memset(recv_pack, 0, sizeof(PACK));
        if (recv(*(int *)sock_fd, recv_pack, sizeof(PACK), 0) < 0) {
            my_err("recv", __LINE__);
        }
        switch(recv_pack->type) {
            case EXIT:
                    {
                        printf("end.......\n");
                        pthread_exit(0);
                        break;
                    }
            case LOGIN:
                    {
                        strcpy(send_pack->data.send_user, recv_pack->data.send_user);
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        send_pack->data.send_fd = recv_pack->data.recv_fd;
                        pthread_create(&pid, NULL, thread_box, sock_fd);
                        pthread_join(pid, NULL);
                        printf("离线期间消息盒子中有%d条消息,%d个好友请求\n", box->talk_number, box->friend_number);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break; 
                    }   
            case ACCOUNT_ERROR:
                    {   
                        strcpy(send_pack->data.send_user, recv_pack->data.send_user);
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;  
                    }
            case REGISTERED:
                    {
                        send_pack->data.send_account = recv_pack->data.send_account;
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case CHANGE_PASSWORD:
                    {                          
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case ADD_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case FRIENDS_PLZ:
                    {
                        pthread_mutex_lock(&mutex_cli);
                        box->plz_account[box->friend_number] = recv_pack->data.send_account; 
                        strcpy(box->write_buff[box->friend_number], recv_pack->data.write_buff);
                        box->friend_number++;
                        printf("消息盒子中来了一条好友请求!!!\n");
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DEL_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case BLACK_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case WHITE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case CARE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
            case DISCARE_FRIEND:
                    {
                        memset(send_pack->data.write_buff, 0, sizeof(send_pack->data.write_buff));
                        strcpy(send_pack->data.write_buff, recv_pack->data.write_buff);
                        pthread_mutex_lock(&mutex_cli);
                        pthread_cond_signal(&cond_cli);
                        pthread_mutex_unlock(&mutex_cli);
                        break;
                    }
        }
    }
}



int main() {
    int               sock_fd;
    pthread_t         pid1;
    pthread_t         pid2;
    struct sockaddr_in seve;
	
    pthread_mutex_init(&mutex_cli, NULL);
    pthread_cond_init(&cond_cli, NULL);
    sock_fd = my_accept_cli();
    pthread_create(&pid1, NULL, thread_read, (void *)&sock_fd);
    pthread_create(&pid2, NULL, thread_write, (void *)&sock_fd);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    
	return 0;
}
