CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
TARGET = assembler
SRC = assembler.c \
      erors.c \
      first_pass.c \
      help_funcs.c \
      pre_proses.c \
      prints.c \
      second_pass.c \
      string_to_data.c
OBJ = $(SRC:.c=.o)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

assembler.o: assembler.c assembler.h
	$(CC) $(CFLAGS) -c assembler.c -o assembler.o

erors.o: erors.c all.h
	$(CC) $(CFLAGS) -c erors.c -o erors.o

first_pass.o: first_pass.c all.h passes.h first_pass.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o

help_funcs.o: help_funcs.c all.h passes.h
	$(CC) $(CFLAGS) -c help_funcs.c -o help_funcs.o

pre_proses.o: pre_proses.c all.h
	$(CC) $(CFLAGS) -c pre_proses.c -o pre_proses.o

prints.o: prints.c all.h passes.h
	$(CC) $(CFLAGS) -c prints.c -o prints.o

second_pass.o: second_pass.c all.h passes.h
	$(CC) $(CFLAGS) -c second_pass.c -o second_pass.o

string_to_data.o: string_to_data.c all.h passes.h
	$(CC) $(CFLAGS) -c string_to_data.c -o string_to_data.o

clean:
	rm -f $(OBJ)

