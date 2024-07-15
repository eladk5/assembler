#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define  MAX_FILE_NAME 255
#define  NUM_OF_COMMANDS 16 
#define NOT_COMMAND 16
#define  F 0
#define  T 1

/* Structure to hold command names and their corresponding opcodes */
struct {
    char name[MAX_LABEL_LENGTH];
    int opcode;
} cmd[]={
        {"mov", 0},
        {"cmp", 1},
        {"add", 2},
        {"sub", 3},
        {"lea", 4},
        {"clr", 5},
        {"not", 6},
        {"inc", 7},
        {"dec", 8},
        {"jmp", 9},
        {"bne", 10},
        {"red", 11},
        {"prn", 12},
        {"jsr", 13},
        {"rts", 14},
        {"stop", 15},
        {"null",16}
   };

/* Enumeration for different error types */
enum{
        LINE_LENGTH,
        MAC_NAME_AGAIN,
        MAC_NAME_COMMAND,
        EXTEA_CHARS,
        MAK_NAME_LONG,
        NO_MAC_NAME,
        MACRO_NOT_CLOSE
};

/* 
Checks if a string is the name of a command. If so, returns the opcode of the command.
 otherwise, returns NOT_COMMAND (16).
Parameters:
str: The string to check.
Returns: The opcode of the command if found, otherwise NOT_COMMAND (16).
*/
int search_command (char *str);
/* 
Handles and prints error messages based on the error number.
Parameters:
file_name: The name of the file where the error occurred.
line_num: The line number where the error occurred.
flag: Pointer to an integer flag to indicate an error occurred.
line: The line where the error occurred.
eror_num: The error number indicating the type of error.
*/
void eror(char *file_name, int line_num, int *flag,char *line, int eror_num);
/*
Checks if a string contains only white characters. If so, returns T, otherwise, returns F.
Parameters:
str: The string to check.
Returns: T if the string contains only white characters, otherwise F.
*/
int chek_end_line(char * str);
