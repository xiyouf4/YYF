#ifndef _MY_ERR_H
#define _MY_ERR_H
#include <stdio.h>

void my_err(char *err_string , int line) {
    fprintf(stdout,"line:%d",line);
    perror(err_string);
    exit(1);
}

#endif
