#define MY_ERR_C

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void my_err(char *err_string , int line)
{
	fprintf(stdout,"line:%d",line);
	perror(err_string);
	exit(1);
}
