#Compiler:
CC=clang

# Flags:
CFLAGS=-I. -Wall

# Target:
TARGET=main

# Objects:
OBJS=main.o

# Default:
all:	$(TARGET)

# Compiling:
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Object files:
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean (Move the files to the trash bin).
clean:
	mv $(TARGET) $(OBJS) /Users/adrianlozada/.Trash
