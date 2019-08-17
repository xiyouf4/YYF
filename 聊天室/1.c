#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "my_err.h"

int main()
{
        char need[100] = "hello world" ;
    while (1) {
       int sock_fd;
     int ret;
      struct sockaddr_in seve;
     if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            my_err("socket", __LINE__);
        }
     memset(&seve, 0, sizeof(struct sockaddr_in));
       seve.sin_family = AF_INET;
       seve.sin_port = htons(8888);
     inet_aton("192.168.3.5", &seve.sin_addr);
      printf("sss\n");
     if (connect(sock_fd, (struct sockaddr*)&seve, sizeof(struct sockaddr_in)) < 0) {
         my_err("connect", __LINE__);
      }
     if (send(sock_fd, need, sizeof(need), 0) < 0) {
        my_err("send", __LINE__);
     }
     printf("sssss\n");
     close(sock_fd);
    }
}
