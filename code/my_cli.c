#include <stdio.h>
#include "my_socket.h"
#include "my_err.h"


struct heihei{
	char sendname[20];
	char recvname[20];
	char mes[1024];
	int sendfd;
	int recvfd;
};

struct haha{
	int type;
	struct heihei data;
};

int main()
{
	struct haha a;
	strcpy(a.data.mes, "hello world");
	int sock_fd;
	sock_fd = my_accept_cli();
	send(sock_fd, &a, sizeof(struct haha), 0);
	scanf("%s", a.data.mes);
	send(sock_fd, &a, sizeof(struct haha), 0);
	while(1)
	{}
	return 0;
}
