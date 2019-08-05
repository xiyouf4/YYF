#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//任务队列
typedef struct work {
	/*任务函数指针*/
	void *(process)(void *arg); 
	/*函数参数*/
	void *arg;
	/*指向下一个任务*/
	struct work *next;
}WORK;


//线程池管理器
typedef struct threadpoll {
	/*工作线程列表*/
	pthread_t *pthreads;
	/*任务队列*/
	WORK *tasks;
	/*互斥锁*/
	pthread_mutex_t mutex;
	/*条件变量*/
	pthread_cond_t cond;
	/*标识线程池的状态*/
	int stop;
	/*线程池允许的最大活动线程*/
	int thread_max_num;
	/*任务队列当前最大任务数*/
	int cur_taks;
}POOL;

POOL pool;

/*初始化线程池*/
void initpool(size_t t_num) {
	pool = (POOL*)malloc(sizeof(POOL));

	/*初始化条件变量和互斥量*/
	pthread_mutex_init(&(pool->mutex), NULL);
	pthread_cond_init(&(pool->cond), NULL);

	/*初始化任务队列*/
	pool->tasks = NULL;
	pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * t_num);
	pool->thread_max_num = t_num;

	for (size_t i = 0; i < t_num; i++) {
		pthread_create(&(pool->threads[i]), NULL, run, NULL);
	}
}

/*工作线程*/
void *run(void *arg) {
	while(!pool->stop) {
		pthread_mutex_lock(&(pool->mutex));

		/*若队列为０处于阻塞状态*/
		while(pool->cur_tasks == 0 && !pool->stop) {
			pthread_cond_wait(&(pool->cond), &(pool->mutex));
		}

		/*销毁线程池*/
		if(pool->stop) {
			pthread_mutex_unlock(&(pool->mutex));
			pthread_exit(NULL);
		}

		/*从任务列表里取出任务函数*/
		pool->cur_tasks--;
		WORK *temp = pool->tasks;
		pool->tasks = pool->tasks->next;
		pthread_mutex_unlock(&(pool->mutex));
		temp->process(temp->arg);
		free(temp);
	}

}

/*添加任务*/
int append(void *(*process)(void *arg), void *arg) {
	WORK *newwork = (WORK*)malloc(sizeof(WORK));
	newwork->process = process;
	newwork->arg = arg;
	newwork->next = NULL;

	pthread_mutex_lock(&(poll->mutex));

	WORK *p = pool->tasks;
	if(p) {
		while(p->next) {
			p = p->next;
		}
		p->next = newwork;
	}
	else {
		pool->tasks = newwork;
	}
	pool->cur_tasks++;
	pthread_cond_signal(&(pool->cond));
	pthread_mutex_unlock(&(pool->mutex));
	return 0;
}

/*销毁线程池*/
int desory() {
	if(pool->stop) {
		return -1;
	}
	pool->stop = 1;
	
	/*唤醒所有线程*/
	pthread_cond_broadcast(&(pool->cond));

	for (int i = 0; i < pool->thread_max_num; i++) {
		pthread_join(poll->thread[i], NULL);
	}

	free(pool->threads);

	while(pool->tasks) {
		WORK *temp = pool->tasks;
		pool->tasks = pool->tasks->next;
		free(temp);
	}

	pthread_mutex_destroy(&(pool->mutex));
	pthread_cond_destroy(&(pool->cond));
	free(pool);
}
