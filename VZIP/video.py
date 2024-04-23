import cv2
import os
import time

# Create a VideoCapture object
image_directory = 'decompress_images'
image_files = [img for img in os.listdir(image_directory) if img.endswith(".ppm")]

# Sort the files
image_files.sort(key=lambda x: int(x.split('_')[1].split('.')[0]))
frame_rate = 15

# Loop through the files
for image_file in image_files:
    # Load the image
    image_path = os.path.join(image_directory, image_file)
    image = cv2.imread(image_path)
    # Check if the image has been successfully loaded
    if image is not None:
        # Display the image
        cv2.imshow('Frame', image)
        cv2.waitKey(int(1000 / frame_rate)) 
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        print(f"Failed to load image: {image_path}")

# Release the VideoCapture object
cv2.destroyAllWindows()