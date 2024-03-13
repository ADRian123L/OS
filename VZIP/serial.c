#include <dirent.h> 
#include <pthread.h>
#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>

#define BUFFER_SIZE 1048576 // 1MB

// Declarations:
typedef struct __arg_t;

typedef struct __queue_t {
	char **array;
	size_t start_index;
	size_t end_index;
	size_t size;
	size_t capacity;
	pthread_mutex_t lock;
	pthread_cond_t empty;
	pthread_cond_t full;
} queue_t;
typedef struct __arg_t {
	char **files;
} arg_t;

// Functions:
int init(queue_t *queue) {
	queue->array = (char **) malloc(sizeof(char *) * 15);
	assert(queue->array != NULL);
	queue->capacity = 15;
	queue->size = 0;
	queue->start_index = 0;
	queue->end_index = 0;
	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->empty, NULL);
	pthread_cond_init(&queue->full, NULL);
}

int enqueue(arg_t val, queue_t *queue) {
	arg_t *new = (arg_t *) malloc(sizeof(arg_t));	
	assert(new != NULL);
	*new = val;
	pthread_mutex_lock(&queue->lock);
		
}

void *producer(void *arg){
	return NULL;	
}

int cmp(const void *a, const void *b) {
	return strcmp(*(char **) a, *(char **) b);
}

int main(int argc, char **argv) { 
	// time computation header
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	// end of time computation header

	// do not modify the main function before this point!

	assert(argc == 2);

	DIR *d;
	struct dirent *dir;
	char **files = (char **) malloc(sizeof(char *) * 100);
	assert(files != NULL);
	int nfiles = 0;

	// Declare the threads:
	pthread_t t[20];

	d = opendir(argv[1]);
	if(d == NULL) {
		printf("An error has occurred\n");
		return 0;
	}

	// Read images:
	while ((dir = readdir(d)) != NULL) {
		//files = realloc(files, (nfiles + 1) * sizeof(char *));
		//assert(files != NULL);

		int len = strlen(dir->d_name);
		if(dir->d_name[len - 4] == '.' && dir->d_name[len - 3] == 'p' && dir->d_name[len - 2] == 'p' && dir->d_name[len - 1] == 'm') {
			files[nfiles] = strdup(dir->d_name);
			assert(files[nfiles] != NULL);

			nfiles++;
		}
	}
	closedir(d);
	qsort(files, nfiles, sizeof(char *), cmp);

	// create a single zipped package with all PPM files in lexicographical order
	int total_in = 0, total_out = 0;
	FILE *f_out = fopen("video.vzip", "w");
	assert(f_out != NULL);
	for(int i = 0; i < nfiles; i++) {
		int len = strlen(argv[1]) + strlen(files[i]) + 2; // Get the size of the name + size of the image
		char *full_path = malloc(len * sizeof(char)); // Allocate memory with the same size
		assert(full_path != NULL); 
		strcpy(full_path, argv[1]); // Copy the file into the new allocated space
		strcat(full_path, "/");
		strcat(full_path, files[i]);

		unsigned char buffer_in[BUFFER_SIZE];
		unsigned char buffer_out[BUFFER_SIZE];

		// load file
		FILE *f_in = fopen(full_path, "r");
		assert(f_in != NULL);
		int nbytes = fread(buffer_in, sizeof(unsigned char), BUFFER_SIZE, f_in);
		fclose(f_in);
		total_in += nbytes;

		// zip file
		z_stream strm;
		int ret = deflateInit(&strm, 9);
		assert(ret == Z_OK);
		strm.avail_in = nbytes;
		strm.next_in = buffer_in;
		strm.avail_out = BUFFER_SIZE;
		strm.next_out = buffer_out;

		ret = deflate(&strm, Z_FINISH);
		assert(ret == Z_STREAM_END);

		// dump zipped file
		int nbytes_zipped = BUFFER_SIZE-strm.avail_out;
		fwrite(&nbytes_zipped, sizeof(int), 1, f_out);
		fwrite(buffer_out, sizeof(unsigned char), nbytes_zipped, f_out);
		total_out += nbytes_zipped;

		free(full_path);
	}
	fclose(f_out);

	printf("Compression rate: %.2lf%%\n", 100.0*(total_in-total_out)/total_in);

	// release list of files
	for(int i=0; i < nfiles; i++)
		free(files[i]);
	free(files);

	// do not modify the main function after this point!

	// time computation footer
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("Time: %.2f seconds\n", ((double)end.tv_sec+1.0e-9*end.tv_nsec)-((double)start.tv_sec+1.0e-9*start.tv_nsec));
	// end of time computation footer

	return 0;
}
