#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


typedef struct Node {
    void *data;
    struct Node *next;
} Node;

/* 任务队列 */
typedef struct Queue {
    Node *head;
    Node *tail;
    size_t size;
} Queue;

/* 线程池 */
typedef struct ThreadPool {
    Queue *queue;
    /* 标志位用来判断线程池是否开始运行 */
    int is_start;
    /* 线程池里面的线程数量 */
    size_t cont;
    /* 锁和条件变量 */
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    /* 线程列表 */
    pthread_t *pthreads;
} ThreadPool;

/* 函数指针 */
typedef struct Task {
    void (*callback)(void *arg);
    void *arg;
} Task;

/* 初始化任务队列 */
Queue *QueueInit() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

/* 销毁任务队列 */
void QueueDestroy(Queue *queue, void (*dataDestroy)(void *)) {
    Node *node = queue->head;
    while (node != NULL) {
        Node *tmp = node->next;
        if (dataDestroy) {
            dataDestroy(node->data);
        }
        free(node);
        node = tmp;
    }
    free(queue);
}

/* 入队 */
void QueuePush(Queue *queue, void *data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    if (queue->tail) {
        queue->tail->next = node;
        queue->tail = node;
    } else {
        queue->head = queue->tail = node;
    }
    queue->size++;
}

/* 出队 */
void *QueuePop(Queue *queue) {
    void *data = NULL;
    if(queue->size == 1) {
        data = queue->head->data;
        free(queue->head);
        queue->head = queue->tail = NULL;
    } else {
        Node *tmp = NULL;
        if (queue->head == NULL) {
            exit(0);
        }
        tmp = queue->head->next;
        data = queue->head->data;
        free(queue->head);
        queue->head = tmp;
    }
    queue->size--;

    return data;
}

/* 线程池的初始化 */
ThreadPool *ThreadPoolInit(size_t cont) {
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    pool->queue = QueueInit();
    pthread_mutex_init(&(pool->mutex), NULL);
    pthread_cond_init(&(pool->cond), NULL);
    pool->is_start = 0;
    pool->cont = cont;
    pool->pthreads = (pthread_t *)malloc(sizeof(pthread_t) * cont);

    return pool;
}

/* 启动线程 线程池的回调函数? */
void *threadStart(void *args) {
    ThreadPool *pool = (ThreadPool *)args;
    while (pool->is_start) {
        pthread_mutex_lock(&pool->mutex);
        while (pool->queue->size < 1 && pool->is_start) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
        Task *task = (Task *)QueuePop(pool->queue);
        pthread_mutex_unlock(&pool->mutex);
        if (task && task->callback) {
            task->callback(task->arg);
            free(task);
        }
    }

    return NULL;
}

/* 线程池开始 */
void ThreadPoolStart(ThreadPool *pool) {
    if (pool->is_start == 1) {
        return;
    }
    pool->is_start = 1;
    int i;
    for (i = 0; i < pool->cont; i++) {
        pthread_create(&(pool->pthreads[i]), NULL, threadStart, (void *)pool);
    }
}

/* 线程池停止 */
void ThreadStop(ThreadPool *pool) {
    pool->is_start = 0;
}

/* 销毁线程池 */
void ThreadPoolDestroy(ThreadPool *pool) {
    ThreadStop(pool);
    /* 唤醒所有线程 */
    pthread_cond_broadcast(&pool->cond);
    int i;
    for (i = 0; i < pool->cont; i++) {
        pthread_join(pool->pthreads[i], NULL);
    }
    QueueDestroy(pool->queue, free);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

/* 传递回调函数 */
void PushTask(ThreadPool *pool, void (*callback)(void *), void *arg) {
    Task *task = (Task *)malloc(sizeof(Task));
    task->callback = callback;
    task->arg = arg;
    pthread_mutex_lock(&pool->mutex);
    QueuePush(pool->queue, task);
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->cond);
}

void sum(void *arg) {
    int i;
    int cont = 0;
    for (i = 0; i < 100; i++) {
        cont += i;
    }
    printf("%d\n", cont);
}


int main()
{
        ThreadPool *pool = ThreadPoolInit(4);
        ThreadPoolStart(pool);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);

        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        PushTask(pool, sum, NULL);
        sleep(1);
        ThreadPoolDestroy(pool);
        return 0;
}
