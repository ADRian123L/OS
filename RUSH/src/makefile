CC=gcc
CFLAGS=-Wall -Wextra -std=c11
TARGET=rush

# List of source files
SOURCES=rush.c read_line.c execute.c vector.c
# Generate object file names by replacing the .c extensions with .o
OBJECTS=$(SOURCES:.c=.o)

# Default target
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to generate object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Additional dependencies for object files can be specified here
# For example, if read_line.o depends on read_line.h, you could add:
# read_line.o: read_line.h

# You can also specify additional flags or libraries needed for linking, if necessary,
# by adding them to the command in the $(TARGET) rule.
