#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

char buff[1024];
pthread_mutex_t mutex;
pthread_cond_t cond, cond1;

void *callback(void *arg) {
	while (1) {
		pthread_mutex_lock(&mutex);
		if (strlen(buff) >= 1000) {
			pthread_cond_wait(&cond1, &mutex);
			strcat(buff, "hai");
			pthread_mutex_unlock(&mutex);
		} else {
		
			strcat(buff, "hello world");
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
		} 
	}
}

void *callback1(void *arg) {
	while (1) {
		pthread_mutex_lock(&mutex);
		if (strlen(buff) == 0) {
			pthread_cond_wait(&cond, &mutex);
			pthread_mutex_unlock(&mutex);
		} else {
			printf("%s\n", buff);
			memset(buff, 0, sizeof(char) * 1024);
			pthread_cond_signal(&cond1);
			pthread_mutex_unlock(&mutex);
		}
	}
}

int main()
{
	pthread_t pid1,pid2;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond,NULL);
	pthread_cond_init(&cond1,NULL);
	pthread_create(&pid1, NULL, callback, NULL);
	pthread_create(&pid2, NULL, callback1, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
}
