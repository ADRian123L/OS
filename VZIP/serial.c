#include <dirent.h> 
#include <pthread.h>
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1048576 // 1MB
#define THRDS_F 1
#define THRDS_C 1
#define TH_TOTAL (THRDS_C + THRDS_F)

// Queue.h
pthread_cond_t not_empty;

// Structure declarations:
typedef struct _temp temp_t;
typedef struct _queue_t queue_t;
typedef struct _arg_t arg_t;

typedef struct _temp {
	unsigned char *ptr;
	z_stream *strm;
	char *name;
	int done;

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
temp_t *dequeue(queue_t *queue);
int isEmptyT(queue_t *q);

// Queue.c
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


//
int done;
pthread_mutex_t done_lock;
extern pthread_cond_t not_empty;

void *file_producer(void *arg) {
	arg_t *t = (arg_t *) arg;
	struct dirent *dir = NULL;
	unsigned char *image = NULL;
	dir = readdir(t->dir);
	int que = 0;
	while (dir != NULL) {
		// Copy the image into a struct then enqueue:
		if (strstr(dir->d_name, ".ppm") != NULL) {
			image = (unsigned char *) strdup(dir->d_name);
			assert(image != NULL);
			temp_t *new = (temp_t *) malloc(sizeof(temp_t));
			new->ptr = image;
			enqueue(new, &t->que1[que]);
			que++;
			que %= 18;
		}
		dir = readdir(t->dir);
	}
	pthread_mutex_lock(&done_lock);
	done = 1;
	pthread_mutex_unlock(&done_lock);
	free(arg);
	return (void *) NULL;
}

int cmp(const void *a, const void *b) {
	temp_t *a1 = *(temp_t **) a;
    temp_t *b1 = *(temp_t **) b;
	return strcmp(a1->name, b1->name);
}

void *compressor2(void *arg) {
	arg_t *t = (arg_t *) arg;
	pthread_mutex_lock(&done_lock);
	while (done == 0 || !isEmpty(t->que1)) { 
		pthread_mutex_unlock(&done_lock);
		temp_t *tmp = dequeue(t->que1);
		char *ptr = tmp->ptr;
		char *name = (char *) calloc(sizeof(char), 100);
		assert(name != NULL);
		int counter = 0;
		while (ptr != NULL && *ptr != '.') {
			name[counter] = *ptr;
			counter++;
			ptr++;
		}
		unsigned char *buffer = tmp->ptr;

		int len = strlen(t->argv[1]) + strlen(buffer) + 2; // Get the size of the name + size of the image
	
		char *full_path = malloc(len * sizeof(char)); // Allocate memory with the same size
		assert(full_path != NULL); 
		
		strcpy(full_path, t->argv[1]); // Copy the file into the new allocated space
		strcat(full_path, "/");
		strcat(full_path, buffer);
		
		unsigned char *buffer_in = (unsigned char*) malloc(sizeof(unsigned char) * BUFFER_SIZE);		
		
		unsigned char *buffer_out;
		buffer_out = (unsigned char *) malloc(sizeof(unsigned char) * BUFFER_SIZE);
		
		// load file
		
		FILE *f_in = fopen(full_path, "r");
		
		assert(f_in != NULL);
		
		int nbytes = fread(buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
		fclose(f_in);
		
		// zip file
		z_stream *strm = (z_stream *) malloc(sizeof(z_stream));
		assert(strm != NULL);
		int ret = deflateInit(strm, 9);
		assert(ret == Z_OK);
		strm->avail_in = nbytes;
		strm->next_in = buffer_in;
		strm->avail_out = BUFFER_SIZE;
		strm->next_out = buffer_out;
		
		ret = deflate(strm, Z_FINISH);
		assert(ret == Z_STREAM_END);

		tmp = (temp_t *) malloc(sizeof(temp_t));
		assert(tmp != NULL);
		tmp->ptr = buffer_out;
		tmp->strm = strm;
		tmp->name = name;
		enqueue(tmp, t->que2);
		free(full_path);
		
		pthread_mutex_lock(&done_lock);
	}
	pthread_mutex_unlock(&done_lock);
	return (void *) NULL;
}


int main(int argc, char **argv) { 
	// time computation header
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// end of time computation header

	// do not modify the main function before this point!
	assert(argc == 2);

	// Conditional variable init:
	pthread_cond_init(&not_empty, NULL);

	// Declared the threads and locks:
	done = 0;
	pthread_mutex_init(&done_lock, NULL);
	pthread_t t[19]; 
	// Create the queues:
	queue_t ques[19];
	queue_t que_outs[19];
	for (int i = 0; i < 19; ++i)
		init(&ques[i]);
    for (int i = 0; i < 19; i++){
        init(&que_outs[i]);
    }
	// Open the image directory:
	DIR *entries = opendir(argv[1]);
	assert(entries != NULL);
	// Create the copy thread
	arg_t *arg = (arg_t *) malloc(sizeof(arg_t));
	assert(arg != NULL);
	arg->dir = entries;
	arg->que1 = (queue_t *) &ques;
	pthread_create(&t[0], NULL, file_producer, arg);
	
	// create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
	//call the compressor;
	//pthread_join(t[0], NULL);

	for (int i = 1; i < 19; i++) {
	    arg_t *arg2 = (arg_t *) malloc(sizeof(arg_t));
		//assert(arg2 != NULL);
	    arg2->que1 = &ques[i - 1];
	    arg2->que2 = &que_outs[i];
	    arg2->argv = argv;
	    pthread_create(&t[i], NULL, compressor2, arg2);
	}
	// Join threads:
    for (int i = 0; i < 19; i++){
        pthread_join(t[i], NULL);
    }
	
	
	// Close the directory:
	closedir(entries);

    for (int i = 1; i < 19; i++){

        while (!isEmpty(&que_outs[i])){
            temp_t *a = dequeue(&que_outs[i]);
            enqueue(a, &que_outs[0]);
        }
    }
	
	// Write the files:

	qsort(que_outs[0].array, que_outs[0].size, sizeof(temp_t *), cmp);
   
    
	while(!isEmpty(&que_outs[0])) {
		temp_t *tmp = dequeue(&que_outs[0]);
		// dump zipped file
		int nbytes_zipped = BUFFER_SIZE - tmp->strm->avail_out;
		fwrite(&nbytes_zipped, sizeof(int), 1, f_out);
		fwrite(tmp->ptr, sizeof(unsigned char), nbytes_zipped, f_out);
		total_out += nbytes_zipped;
	}
	fclose(f_out);
	
	// do not modify the main function after this point!

	// time computation footer
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.10f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	// end of time computation footer

	return 0;
}