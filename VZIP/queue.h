#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zlib.h>
#include <dirent.h>
#include <assert.h>


// Structure declarations:
typedef struct _temp temp_t;
typedef struct _queue_t queue_t;
typedef struct _arg_t arg_t;

typedef struct _temp {
	unsigned char *ptr;
	z_stream * strm;

} temp_t;

// Thread Argument
typedef struct _arg_t {
	queue_t *que1;
	queue_t *que2;
	DIR *dir;
	char ** argv;
} arg_t;

typedef struct _queue_t {
	temp_t **array;
	size_t insert_index;
	size_t delete_index;
	size_t size;
	size_t capacity;
	pthread_mutex_t lock;
	pthread_cond_t not_empty;
} queue_t;


// Function declarations:
int isFull(queue_t *q);
int isEmpty(queue_t *q);
int enqueue(temp_t *obj, queue_t *queue);
int init(queue_t *queue);


#endif // QUEUE_H