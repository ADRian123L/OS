#include <dirent.h> 
#include <pthread.h>
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>
#include <string.h>
#include "queue.h"

#define BUFFER_SIZE 1048576 // 1MB
#define THRDS_F 5
#define THRDS_C 5
#define TH_TOTAL (THRDS_C + THRDS_F)


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
			que %= 5;
		}
		dir = readdir(t->dir);
	}
	free(arg);
	return (void *) NULL;
}

int cmp(const void *a, const void *b) {
	return strcmp(*(char **) a, *(char **) b);
}


/*
void *compressor(void *arg){
	arg_t *t = (arg_t *) arg;
	
	while (!isEmpty(t->que1)){
		unsigned char *buffer = dequeue(t->que1)->ptr;

		int len = strlen(t->argv[1]) + strlen(buffer) + 2; // Get the size of the name + size of the image
		char *full_path = malloc(len * sizeof(char)); // Allocate memory with the same size
		assert(full_path != NULL); 
		strcpy(full_path, t->argv[1]); // Copy the file into the new allocated space
		strcat(full_path, "/");
		strcat(full_path, buffer);
		unsigned char buffer_in[BUFFER_SIZE];
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

		temp_t *tmp = (temp_t *) malloc(sizeof(temp_t));
		assert(tmp != NULL);
		tmp->ptr = buffer_out;
		tmp->strm = strm;
		enqueue(tmp, t->que2);
		free(full_path);
	}
	return (void *) NULL;
}
*/

int main(int argc, char **argv) { 

	// time computation header
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// end of time computation header

	// do not modify the main function before this point!
	assert(argc == 2);

	// Declared the threads:
	pthread_t t[20]; 
	// Create the queues:
	queue_t que[5];
	assert(que != NULL);
	for (int i = 0; i < 5; ++i){
		init(&que[i]);
	}
	// Open the image directory:
	DIR *entries = opendir(argv[1]);
	assert(entries != NULL);

	arg_t *arg = (arg_t *) malloc(sizeof(arg_t));
	assert(arg != NULL);
	arg->dir = entries;
	arg->que1 = (queue_t *) &que;
	pthread_create(&t[0], NULL, file_producer, arg);
	

	/*
	// create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
	//call the compressor;
	for (int i = THRDS_F; i < TH_TOTAL; i++){
		arg_t *arg = (arg_t *) malloc(sizeof(arg_t));
		assert(arg != NULL);
		arg->que1 = &q1;
		arg->que2 = &q2;
		pthread_create(&t[i], NULL, compressor, arg);
	}
	*/
	// Join threads:
	for (int i = 0; i < 1; ++i)
		pthread_join(t[i], NULL);
	// Close the directory:
	closedir(entries);
	/*
	// Write the files:
	while(!isEmpty(&q2)) {
		temp_t *tmp = dequeue(&q2);
		// dump zipped file
		int nbytes_zipped = BUFFER_SIZE - tmp->strm->avail_out;
		fwrite(&nbytes_zipped, sizeof(int), 1, f_out);
		fwrite(tmp->ptr, sizeof(unsigned char), nbytes_zipped, f_out);
		total_out += nbytes_zipped;
	}
	fclose(f_out);
	*/
	// do not modify the main function after this point!

	// time computation footer
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.10f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	// end of time computation footer

	return 0;
}
