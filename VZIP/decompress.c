#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define BUFFER_SIZE 1048576 

int main(int argc, char **argv) {
    assert(argc == 2);

    FILE *f_in = fopen("video.vzip", "r");
    assert(f_in != NULL);

    unsigned char buffer_in[BUFFER_SIZE];
    unsigned char buffer_out[BUFFER_SIZE];

    int image_index = 0;
    while (1) {
        int nbytes_zipped;
        // Read the size of the compressed data
        if (fread(&nbytes_zipped, sizeof(int), 1, f_in) != 1) {
            break; // End of file reached
        }

        // Read the compressed data
        assert(fread(buffer_in, sizeof(unsigned char), nbytes_zipped, f_in) == nbytes_zipped);

        // Prepare decompression
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = nbytes_zipped;
        strm.next_in = buffer_in;
        strm.avail_out = BUFFER_SIZE;
        strm.next_out = buffer_out;

        // Initialize decompression
        int ret = inflateInit(&strm);
        assert(ret == Z_OK);

        // Decompress
        ret = inflate(&strm, Z_NO_FLUSH);
        assert(ret == Z_STREAM_END);

        // Finish decompression
        inflateEnd(&strm);

        // Write the decompressed data to a file
        char filename[256];
        sprintf(filename, "%s/image_%d.ppm", argv[1], image_index++);
        FILE *f_out = fopen(filename, "w");
        assert(f_out != NULL);
        fwrite(buffer_out, sizeof(unsigned char), BUFFER_SIZE - strm.avail_out, f_out);
        fclose(f_out);
    }

    fclose(f_in);

    return 0;
}
