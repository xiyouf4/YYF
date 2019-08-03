#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

int a = 0;
pthread_mutex_t mutex;

void *f(void *arg){
	for (int i = 0; i < 10000; i++){
		pthread_mutex_lock(&mutex);
		a++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}


int main()
{
	pthread_t tid1, tid2;
	pthread_mutex_init(&mutex,NULL);

	pthread_create(&tid1, NULL, f, NULL);
	pthread_create(&tid2, NULL, f, NULL);

	sleep(1);

	printf("%d\n",a);
	pthread_mutex_destroy(&mutex);
	return 0;
}
