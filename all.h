#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_ENDINDG 12
#define MAX_SIZE_MEMOREY 3996  
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define  MAX_FILE_NAME 255
#define  NUM_OF_COMMANDS 16 
#define NOT_COMMAND -1
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
        {".entry",16},
        {".extern", 17},
        {".data", 18},
        {".string",19}  
   };

enum{
    ENTRY = 16,
    EXTERN,
    DATA,
    STRING
};

/* Structure to represent a macro node in the linked list */
struct MacroNode {
    char name[MAX_LABEL_LENGTH]; /* Name of the macro */
    int offset; /* offset of the macro definition in the file (from the beginning of the file) */
    int line_number; /* Line number where the macro is defined */
    struct MacroNode *next;
};

/* Structure to represent the head of the macro linked list */
typedef struct MacroNode *macro_node;

struct Generic_head{
    void *head_of_list;
};
typedef struct Generic_head head;

enum{
    NOT_MACRO,
    MACRO_READ,
    MACRO_CALL
};

/* Enumeration for different error types */
enum{
        LINE_LENGTH,
        MAC_NAME_AGAIN,
        MAC_NAME_COMMAND,
        EXTEA_CHARS,
        MAC_NAME_LONG,
        NO_MAC_NAME,
        MACRO_NOT_CLOSE,
        OVER_SIZE,
        ONLY_LABEL,
        NOT_VALID_COMMAND,
        LABEL_LENGTH,
        NOT_ALPHA,
        NOT_ALPHA_NUM,
        LABEL_IS_COMMAND,
        LABEL_IS_MACRO,
        LABEL_NAME_AGAIN,
        MAC_START,
        FAIL_STRING
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

union Instruction {
    struct {
        unsigned int e: 1;
        unsigned int r: 1;
        unsigned int a: 1;
        unsigned int second_operand_method: 4;
        unsigned int first_operand_method: 4;
        unsigned int opcode: 4;
    } first_command;

    struct {
        unsigned int e: 1;
        unsigned int r: 1;
        unsigned int a: 1;
        int value: 12;/*not unsigned beacuse the value can be negative*/
    } number_word;

    struct {
        unsigned int e: 1;
        unsigned int r: 1;
        unsigned int a: 1;
        unsigned int adress: 12;
    } label_word;
    struct {
        unsigned int e: 1;
        unsigned int r: 1;
        unsigned int a: 1;
        unsigned int second_operand: 3;
        unsigned int first_operand: 3;
        unsigned int reserved: 6;/*not in use*/
    } register_word;
};
typedef union Instruction instruction;

struct Command
{
        int num_of_commands;
        int which_num_of_command;
        int type_of_command;/*first command, namber,label or register */
        instruction com;
};
typedef struct Command command;

/* 
Frees the memory allocated for the macros in the head_node.
Parameters:
head_node: The head of the macro linked list.
*/
static void free_the_mac(head head_node);


macro_node is_macro_name(char *str,head head_node);