#define MY_SPCKET_C

#include "my_err.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888 //端口号
#define IP 192.168.3.61 //服务器ip地址

int MyAcceptSeve(void) {
    int sock_fd;
    int ret;
    int len;
    int opt;
    int i;
    struct sockaddr_in seve;
    opt = 1;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        MyErr("socket",__LINE__);
    }
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&seve, 0, sizeof(struct sockaddr_in));
    seve.sin_family = AF_INET;
    seve.sin_port = htons(PORT);
    seve.sin_addr.s_addr = htonl(INADDR_ANY);
    if (fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFD, 0) | O_NONBLOCK) < 0) {
        MyErr("fcntl",__LINE__);
    }
    if (bind(sock_fd, (struct sockaddr*)&seve, sizeof(struct sockaddr_in)) < 0) {
        MyErr("bind",__LINE__);
    }
    if (listen(sock_fd, 15) < 0) {
        MyErr("listen", __LINE__);
    }
    
    return sock_fd;
}

int MyAcceptCli(void) {
    int sock_fd;
    int ret;
    struct sockaddr_in seve;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        MyErr("socket", __LINE__);
    }
    memset(&seve, 0, sizeof(struct sockaddr_in));
    seve.sin_family = AF_INET;
    seve.sin_port = htons(PORT);
    inet_aton("IP", &seve.sin_addr);
    if (connect(sock_fd, (struct sockaddr*)&seve, sizeof(struct sockaddr_in)) < 0) {
        MyErr("connect", __LINE__);
    }
    
    return sock_fd;
}
