#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include "my_socket.h"
#include "my_err.h"


struct heihei{
	char send_name[20];
	char recv_name[20];
	char mes[1024];
	int send_fd;
	int recv_fd;
};



struct haha{
	int type;
	struct heihei data;
};

void *deal(void *hei)
{
	int 			i;
	struct haha		*p;
	p = (struct haha*)hei;
	printf("%s\n", p->data.mes);
}

int main()
{
	int 			i;
	int 			ret;
	int           	 	socklen;
	int 			sock_fd;
	int 			conn_fd;
	int 			nfds;
	int			kdpfd;
	int 			curfds;
	int 			acceptcont = 0;
	struct sockaddr_in	seve;
	struct sockaddr_in	cli;
	struct epoll_event	ev;
	struct epoll_event	events[1024];
	struct haha             hen;
	struct haha		*hei;

	socklen = sizeof(struct sockaddr_in);

	sock_fd = my_accept_seve();

	kdpfd = epoll_create(1024);
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
				printf("连接成功\n");
				acceptcont++;
				
				if(curfds >= 1024){
					fprintf(stderr, "太多的连接了\n");
					close(conn_fd);
					continue;
				}

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_fd;
				
				if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, conn_fd, &ev) < 0){
					my_err("epoll_ctl", __LINE__);
				}
				curfds++;
				continue;
			}
			else if(events[i].events & EPOLLIN){
				ret = recv(events[i].data.fd, &hen, sizeof(struct haha), 0);
				if(ret < 0){
					close(events[i].data.fd);
					perror("recv");
					continue;
				}
				hei = (struct haha*)malloc(sizeof(struct haha));
				memcpy(hei, &hen, sizeof(struct haha));
				pthread_t pid;
				pthread_create(&pid, NULL, deal, (void*)hei);
			}
		}
	}
}
