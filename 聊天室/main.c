#include <mysql/mysql.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "my_err.h"
#include "my_socket.h"
#include "my_login.h"
#include "my_mysql.h"
#include "my_send.h"
#include "my_deal.h"


#define MAXEPOLL 1024


int main()
{
	int                        i;
	int                        sock_fd;
	int                        conn_fd;
	int                        socklen;
	int                        acceptcont = 0;
	int                        kdpfd;
	int                        curfds;
	int                        nfds;
	char                       need[MAXIN];
	MYSQL                      mysql;
	struct sockaddr_in         cli;
	struct epoll_event         ev;
	struct epoll_event         events[MAXEPOLL];
	PACK                       recv_pack;
	PACK                       *pack;
	pthread_t                  pid;

	socklen = sizeof(struct sockaddr_in);
	mysql = accept_mysql();
	sock_fd = my_accept_seve();

	kdpfd = epoll_create(MAXEPOLL);

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sock_fd;

	if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, sock_fd, &ev) < 0){
		my_err("epoll_ctl", __LINE__);
	}

	curfds = 1;

	while(1){
		if((nfds = epoll_wait(kdpfd, events, curfds, -1)) < 0){
			my_err("epoll_wait", __LINE__);
		}

		for(i = 0; i < nfds; i++){
			if(events[i].data.fd == sock_fd){
				if((conn_fd = accept(sock_fd, (struct sockaddr*)&cli, &socklen)) < 0){
					my_err("accept", __LINE__);
				}
				printf("连接成功,套接字编号%d\n", conn_fd);
				acceptcont++;

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_fd;

				if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, conn_fd, &ev) < 0){
					my_err("epoll_ctl", __LINE__);
				}
				curfds++;
				continue;
			}
			else if(events[i].events & EPOLLIN){
				memset(&recv_pack, 0, sizeof(PACK));
				if(recv(events[i].data.fd, &recv_pack, sizeof(PACK), 0) < 0){
					close(events[i].data.fd);
					perror("recv");
					continue;
				}
			        
				memset(need, 0, sizeof(need));	
				printf("star\n");
				sprintf(need, "update user_data set user_socket = %d where account = %d", events[i].data.fd, recv_pack.data.send_account);
				printf("\n%s\n",need);
				printf("star\n");
				mysql_query(&mysql, need);
				pack = (PACK*)malloc(sizeof(PACK));
				memcpy(pack, &recv_pack, sizeof(PACK));
				pthread_create(&pid, NULL, deal, (void*)pack);

			}
		}
	}
}
