#include <stdio.h>
#include <pthread.h>
#include "my_pack.h"
#include "my_socket.h"


int main()
{
	int               sock_fd;
	sock_fd = my_accept_cli();
	PACK              pack;
	pack.type = LOGIN;
	pack.data.send_account = 1000000;
	strcpy(pack.data.messages, "520520cw...");
	getchar();
	send(sock_fd, &pack, sizeof(PACK), 0);
	while(1)
	{}
	return 0;
}
