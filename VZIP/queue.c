#include "queue.h"

pthread_mutex_t dir_lock;


int isFull(queue_t *q) {
	if (q->size >= q->capacity - 1)
		return 1;
	return 0;
}

int isEmpty(queue_t *q) {
	if (q->size == 0)
		return 1;
	else 
		return 0;
}


int enqueue(temp_t *obj, queue_t *queue) {
	pthread_mutex_lock(&queue->lock);
	if (isFull(queue)){
		queue->array = (temp_t **) realloc(queue->array, sizeof(temp_t *) * queue->capacity * 2);
		assert(queue->array != NULL);
		queue->capacity *= 2;
	}
	queue->size++;
	queue->array[queue->insert_index] = obj;
	queue->insert_index++;
	queue->insert_index %= queue->capacity;
	pthread_cond_signal(&queue->not_empty);
	pthread_mutex_unlock(&queue->lock);	
	return 1;
}

int init(queue_t *queue) {
    queue->array = (temp_t **) malloc(sizeof(temp_t *) * 15);
	assert(queue->array != NULL);
	queue->capacity = 15;
	queue->size = 0;
	queue->insert_index = 0;
	queue->delete_index = 0;
	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->not_empty, NULL);
	return 1;
}

temp_t *dequeue(queue_t *queue) {
	pthread_mutex_lock(&queue->lock);
	while (isEmpty(queue)) 
        pthread_cond_wait(&not_empty, &queue->lock);

	temp_t *tmp = queue->array[queue->delete_index];
	queue->size--;
	queue->delete_index++;
	queue->delete_index %= queue->capacity;
	pthread_mutex_unlock(&queue->lock);
	return tmp;
}

/*
void *f(void *arg) {
    arg_t *t = (arg_t *) arg;
    for (int i = 0; i < 2e8 / 30; ++i) {
        temp_t *new = (temp_t *) malloc(sizeof(temp_t));
	    enqueue(new, t->que1);
    }
    return (void *) NULL;
}

int main(){
	queue_t q1[5];
    init(&q1);
	temp_t t1;

	t1.ptr = (unsigned char *) "1";

    struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t t[30];

    for (int i = 0; i < 30; ++i) {
		arg_t *arg = (arg_t *) malloc(sizeof(arg_t));
		assert(arg != NULL);
		arg->que1 = &q1[i];
		pthread_create(&t[i], NULL, f, arg);
	}


	for (int i = 0; i < 30; ++i){
		pthread_join(t[i], NULL);
	}

    printf("The size of the queue: %d\n", q1.size);

    temp_t *a;
    for (int i = 0; i < 2e8; ++i)
	    a = dequeue(&q1);

    	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.2f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	return 0;
}
*/