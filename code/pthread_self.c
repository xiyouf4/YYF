#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>


void *f(void *arg)
{
	if(*(int*)arg)
		printf("子进程id = %d\n", pthread_self());
	else
		printf("父进程id = %d\n", pthread_self());
	pthread_exit(NULL);
}

int main()
{
	pid_t pid1;
	pthread_t tid1;
	pthread_t tid2;
	int a;
	pid1 = fork();
	if(pid1 == 0)
	{
		a = 0;
		printf("子进程主线程id = %d\n", pthread_self());
		pthread_create(&tid1, NULL, f, (void*)&a);
		sleep(1);
	}
	else
	{
		a = 1;
		printf("父进程主线程id = %d\n", pthread_self());
		pthread_create(&tid2, NULL, f, (void*)&a);
		sleep(1);
	}
}
