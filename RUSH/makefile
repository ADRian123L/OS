# Compiler:
CC=clang

# Flags:
CFLAGS=-I. -Wall

# Target:
TARGET=main

# Objects:
OBJS=main.o

# Compiling:
$(TARGET): $(OBJS)
             $(CC) -o $@ $< $(CFLAGS)

# Object files:
%.o: %.c
             $(CC) -c -o $@ $< $(CFLAGS)

# Clean
clean:
        rm -f $(TARGET) $(OBJS)
