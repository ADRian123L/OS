#ifndef SERIAL_H
#define SERIAL_H
#include <assert.h>
#include <dirent.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>

#define BUFFER_SIZE 1048576 // 1MB
#define numberOfT 20

// Structure declarations:
typedef struct _temp    temp_t;
typedef struct _queue_t queue_t;
typedef struct _arg_t   arg_t;

typedef struct _temp {
    unsigned char  *ptr;
    z_stream       *strm;
    char           *name;
    int             done;
    int             empty;
    pthread_mutex_t node_lock;
    pthread_cond_t  wake;
} temp_t;

// Thread Argument
typedef struct _arg_t {
    queue_t *que1;
    queue_t *que2;
    DIR     *dir;
    char   **argv;
} arg_t;

typedef struct _queue_t {
    temp_t **array;
    size_t   insert_index;
    size_t   delete_index;
    size_t   size;
    size_t   capacity;
} queue_t;

#endif
