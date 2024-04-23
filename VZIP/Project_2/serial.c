#include "serial.h"

// Check if the queue is empty
int isEmpty(queue_t *q) {
	if (q->size == 0)
		return 1;
	else 
		return 0;
}


// Function declarations:
int enqueue(temp_t *obj, queue_t *queue);
int init(queue_t *queue);
temp_t dequeue(queue_t *queue);

// Enqueue function:
int enqueue(temp_t *obj, queue_t *queue) {
	pthread_mutex_lock(&(queue->array[queue->insert_index]->node_lock));
	queue->array[queue->insert_index]->done = obj->done;
	queue->array[queue->insert_index]->empty = obj->empty;
	queue->array[queue->insert_index]->ptr = obj->ptr;
	queue->array[queue->insert_index]->name = obj->name;
	queue->array[queue->insert_index]->strm = obj->strm;
	queue->insert_index++;
	queue->size++;
	pthread_cond_signal(&queue->array[queue->insert_index - 1]->wake);
	pthread_mutex_unlock(&queue->array[queue->insert_index - 1]->node_lock);	
	return 1;
}

// Initialize the queue
int init(queue_t *queue) {
    queue->array = (temp_t **) malloc(750 * sizeof(temp_t *));
	assert(queue->array != NULL);
	queue->capacity = 750;
	queue->size = 0;
	queue->insert_index = 0;
	queue->delete_index = 0;
	for (int i = 0; i < 750; ++i) {
		temp_t *new = (temp_t *) malloc(sizeof(temp_t));
		assert(new != NULL);
		new->done = 0;
		new->empty = 1;
		queue->array[i] = new;
		pthread_mutex_init(&new->node_lock, NULL);
		pthread_cond_init(&new->wake, NULL);
	}
	return 1;
}

// Dequeue function
temp_t dequeue(queue_t *queue) {
	pthread_mutex_lock(&queue->array[queue->delete_index]->node_lock);
	while (queue->array[queue->delete_index]->empty && !queue->array[queue->delete_index]->done)
		pthread_cond_wait(&queue->array[queue->delete_index]->wake, &queue->array[queue->delete_index]->node_lock);
	temp_t tmp = *queue->array[queue->delete_index];
	queue->array[queue->delete_index]->empty = 1;
	queue->delete_index++;
	queue->size--;
	pthread_mutex_unlock(&queue->array[queue->delete_index - 1]->node_lock);
	return tmp;
}

// File producer function
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
			temp_t new;
			new.ptr = image;
			new.empty = 0;
			new.done = 0;
			enqueue(&new, &t->que1[que]);
			que++;
			que %= numberOfT - 1;
		}
		dir = readdir(t->dir);
	}
	// Signal the compressors when the copy function is done
	for (int i = 0; i < numberOfT; ++i) {
		temp_t new;
		new.done = 1;
		enqueue(&new, &t->que1[i]);
		pthread_cond_signal(&t->que1[i].array[t->que1[i].delete_index]->wake);
	}
	free(arg);
	return (void *) 0;
}

// Compare function
int cmp(const void *a, const void *b) {
	temp_t *a1 = *(temp_t **) a;
    temp_t *b1 = *(temp_t **) b;
	return strcmp(a1->name, b1->name);
}

// Compressor function
void *compressor2(void *arg) {
	arg_t *t = (arg_t *) arg;
	int total_in = 0;
	while (1) {
		// Dequeue the file
		temp_t tmp_c = dequeue(t->que1);
		// If the queue is empty, break
		if (tmp_c.done)
			break;

		char *ptr = (char *) tmp_c.ptr;
		char *name = (char *) calloc(sizeof(char), 100);
		assert(name != NULL);
		int counter = 0;
		while (ptr != NULL && *ptr != '.') {
			name[counter] = *ptr;
			counter++;
			ptr++;
		}
		unsigned char *buffer = tmp_c.ptr;
		int len = strlen(t->argv[1]) + strlen((char *) buffer) + 2; // Get the size of the name + size of the image
		char *full_path = malloc(len * sizeof(char)); // Allocate memory with the same size
		assert(full_path != NULL); 
		strcpy(full_path, t->argv[1]); // Copy the file into the new allocated space
		strcat(full_path, "/");
		strcat(full_path, (char *) buffer);
		unsigned char *buffer_in = (unsigned char*) malloc(sizeof(unsigned char) * BUFFER_SIZE);		
		unsigned char *buffer_out = (unsigned char *) malloc(sizeof(unsigned char) * BUFFER_SIZE);
		// Load file
		FILE *f_in = fopen(full_path, "r");
		assert(f_in != NULL);
		int nbytes = fread(buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
		fclose(f_in);
		total_in += nbytes;
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
		temp_t *tmp = (temp_t *) malloc(sizeof(temp_t));
		assert(tmp != NULL);
		tmp->ptr = buffer_out;
		tmp->strm = strm;
		tmp->name = name;
		enqueue(tmp, t->que2);
		free(full_path);
	}
	return (void *) (long long) total_in;
}

void free_queue(queue_t *queue){
    free(queue->array);
}

int main(int argc, char **argv) { 
	// time computation header
	 struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// end of time computation header

	// do not modify the main function before this point!
	assert(argc == 2);
	// Threads
	pthread_t t[numberOfT]; 
	// Create the queues:
	queue_t ques[numberOfT];
	queue_t que_outs[numberOfT];
	for (int i = 0; i < numberOfT; ++i)
		init(&ques[i]);
    for (int i = 0; i < numberOfT; i++){
        init(&que_outs[i]);
    }
	
	DIR *entries = opendir(argv[1]);
	assert(entries != NULL);
	// Create the copy thread, only one thead for copy
	arg_t *arg = (arg_t *) malloc(sizeof(arg_t));
	assert(arg != NULL);
	arg->dir = entries;
	arg->que1 = (queue_t *) &ques;
	pthread_create(&t[0], NULL, file_producer, arg);
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
	
	//Create thread for compressors
	for (int i = 1; i < numberOfT; i++) {
	    arg_t *arg2 = (arg_t *) malloc(sizeof(arg_t));
		assert(arg2 != NULL);
	    arg2->que1 = &ques[i - 1];
	    arg2->que2 = &que_outs[i];
	    arg2->argv = argv;
	    pthread_create(&t[i], NULL, compressor2, arg2);
	}
	int total_in = 0;

	// Join threads and calculate the total_in
	void *tmp = 0;
    for (int i = 0; i < numberOfT; i++){
        pthread_join(t[i], &tmp);
		total_in += (long long) tmp;
    }

	// Close the directory:
	closedir(entries);

	//Move all compressed files to the first queue
    for (int i = 1; i < numberOfT; i++){
        while (!isEmpty(&que_outs[i])){
            temp_t a = dequeue(&que_outs[i]);
            enqueue(&a, &que_outs[0]);
        }
    }
	
	// Sort the files
	qsort(que_outs[0].array, que_outs[0].size, sizeof(temp_t *), cmp);
    int total_out = 0;
	
	// Write the compressed files to the video.vzip
	while(!isEmpty(&que_outs[0])) {
		temp_t tmp = dequeue(&que_outs[0]);
		// dump zipped file
		int nbytes_zipped = BUFFER_SIZE - tmp.strm->avail_out;
		fwrite(&nbytes_zipped, sizeof(int), 1, f_out);
		fwrite(tmp.ptr, sizeof(unsigned char), nbytes_zipped, f_out);
		total_out = nbytes_zipped + total_out;
	}
	fclose(f_out);
    
	// Free the queues:
    for (int i = 0; i < numberOfT; i++){
        free_queue(&ques[i]);
        free_queue(&que_outs[i]);
    }
	printf("Compression rate: %.2lf%%\n", 100.0 * (total_in - total_out) / total_in);
	// do not modify the main function after this point!
	
	// time computation footer
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.2f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	// end of time computation footer
	return 0;
}