#include <stdio.h>
#include <pthread.h>
#include "my_pack.h"
#include "my_socket.h"
#include "my_err.h"
#include "my_mune.h"
#include <netinet/in.h>
#include <arpa/inet.h>

PACK *send_pack;
PACK *recv_pack;

/* 用来发送数据的线程 */
void *thread_read(void *sock_fd) {
    int               choose;
    char              password[20];
    
    send_pack = (PACK *)malloc(sizeof(PACK));
    /* 套接字编号放进结构体 */
    send_pack->data.send_fd = *(int *)sock_fd;
    /* 1为登录,2为注册,3为退出 */
    while (1) {
        login_mune();
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
}

void *thread_write(void *sock_fd) {
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
