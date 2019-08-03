#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *f(void *arg)
{
	printf("thread1   running\n");	
	int *a = (int*)malloc(sizeof(int));
	*a = 111;
	pthread_exit((void*)a);
}

int main()
{
	pthread_t pid1;
	pthread_create(&pid1, NULL, f, NULL);

	int t;
	int *p = &t;
	pthread_join(pid1, (void**)&p);
	printf("结束返回%d\n", *p);
	free(p);
	return 0;
}
