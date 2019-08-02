#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "my_socket.h"
#include "my_err.h"

struct haha{
	int name;
	char read_buf[1024];
};

int handle(int conn_fd)
{
	int nread;
	struct haha a;
	if((nread = read(conn_fd, &a, sizeof(struct haha))) < 0)
		my_err("read", __LINE__);
	if(nread == 0)
	{
		printf("客户端关闭了链接\n");
		close(conn_fd);
		return -1;
	} 
	printf("%d\n",a.name);
	printf("%s\n",a.read_buf);
	return 0;
}

int main()
{
	int socklen;
	int i;
	int sock_fd, conn_fd, nfds, kdpfd, curfds, acceptcont = 0;
	struct sockaddr_in seve, cli;
	struct epoll_event ev;
	struct epoll_event events[1024];

	socklen = sizeof(struct sockaddr_in);

	sock_fd = my_accept_seve();

	kdpfd = epoll_create(1024);	
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sock_fd;
	if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, sock_fd, &ev) < 0)
		my_err("epoll_ctl",__LINE__);

	curfds = 1;

	while(1)
	{
		if((nfds = epoll_wait(kdpfd, events, curfds, -1)) < 0)
			my_err("epoll_wait",__LINE__);

		for(i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == sock_fd)
			{
				if((conn_fd = accept(sock_fd, (struct sockaddr*)&cli, &socklen)) < 0)
					my_err("accept",__LINE__);
				printf("连接成功\n");
				acceptcont++;
			
				if(curfds >= 1024)
				{
					fprintf(stderr, "太多的连接了\n");
					close(conn_fd);
					continue;
				}
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_fd;
				if(epoll_ctl(kdpfd, EPOLL_CTL_ADD, conn_fd, &ev) < 0)
					my_err("epoll_ctl",__LINE__);
	
				curfds++;
				continue;
			}
			if(fork() == 0)
			{
				if(handle(events[i].data.fd) < 0)
				{
					epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
					curfds--;
				}
				exit;
			}
		}
	}
	close(sock_fd);
	return 0;
}

