import zlib
import cv2
import numpy as np

# Constants
BUFFER_SIZE = 1048576
FILE_PATH = "video.vzip"  # Path to the compressed file
FRAME_RATE = 15  # Frames per second for the video

def decompress_image(compressed_data):
    """
    Decompresses a single image using zlib.
    """
    decompressor = zlib.decompressobj()
    decompressed_data = decompressor.decompress(compressed_data, BUFFER_SIZE)
    return np.frombuffer(decompressed_data, dtype=np.uint8)

def display_image(image_data):
    """
    Displays a single image.
    """
    image = cv2.imdecode(image_data, cv2.IMREAD_COLOR)
    if image is not None:
        cv2.imshow('Frame', image)
        cv2.waitKey(int(1000 / FRAME_RATE))
    else:
        print("Failed to display the image.")

def main():
    with open(FILE_PATH, "rb") as f:
        while True:
            # Read the size of the compressed data
            size_data = f.read(4)
            if len(size_data) < 4:
                break  # End of file reached

            nbytes_zipped = int.from_bytes(size_data, 'little')
            
            # Read the compressed data
            compressed_data = f.read(nbytes_zipped)
            if len(compressed_data) != nbytes_zipped:
                break  # End of file or read error
            
            # Decompress and display the image
            image_data = decompress_image(compressed_data)
            display_image(image_data)
    
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
