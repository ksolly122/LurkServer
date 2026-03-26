# Define the compiler
CC = gcc

# Define the compiler flags
CFLAGS = -Wall -g

# Define the source files
SRCS = character.c client_handler.c main.c room.c utils.c

# Define the output executable name
TARGET = server

# Define the object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
