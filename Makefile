# Define the compiler and the flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

# Define the target executable
TARGET = assembler

# Define the source files
SRC = assembler.c \
      erors.c \
      first_pass.c \
      help_funcs.c \
      pre_proses.c \
      prints.c \
      second_pass.c \
      string_to_data.c

# Define the header files
HEADERS = all.h passes.h

# Define the object files
OBJ = $(SRC:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule to run the program (optional)
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
