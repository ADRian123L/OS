Welcome to the VZIP folder!

This folder contains the necessary files for compressing and decompressing video files using the VZIP algorithm.

To compress a video file, use the compress.c file. Simply run the program and provide the path to the video file you want to compress as a command-line argument.

After compression, you can use the decompress.c file to decompress the compressed video file. Make sure to provide the path to the compressed file as a command-line argument when running the program.

Please note that the compress.c and decompress.c files should be compiled before running. You can use a C compiler such as GCC to compile them.

Feel free to explore the other files in this folder for additional information and examples.

Happy compressing and decompressing!


Compressing Instructions:

To compress the images located in the folder 'frames', simple run 'make' which will compile the compressor. 


Decompressing Instructions:

The visuilizer.py is the code to decompress the images and show the images one by one to simulate a video.

To run it, you can use the following command:
    
    python3 visuilizer.py


Playing The Video:

To play the video, you must have the decompressed images. After you have the decompressed images, you can run the video.py file and it will show video.

Since the python script using cv2 library, you might need download the library first.

The command for downloading the library:

    pip install opencv-python
    
