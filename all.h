#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MIN_QOUTES 2
#define START_SIZE 50
#define MAX_SIZE_ENDINDG 12
#define MAX_SIZE_MEMOREY 3996  
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define  NUM_OF_COMMANDS 16 
#define NOT_COMMAND (-1)
#define DECIMAL 10
#define  F 0
#define  T 1
#define MAX_OPERAND 2
#define NUM_OF_METODS 4
#define SOURCE 0
#define TARGET 1
#define CHAR_TO_NAM(A) ( (A)-'0' )

#define PROMPTE 1
#define INCTRACTION 2


struct ErorNode{
    char *file_name;
    int *line_num; 
    int *flag; 
    char *line; 
} ;
typedef struct ErorNode erors_node;


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


/* Enumeration for different error types */
enum erors{
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
        FAIL_STRING,
        NOT_NUMBER,
        COMMA,
        NO_PARAMS,
        MISS_COMMA,
        NO_VALID_METHOD,
        NUM_REP,
        ENTRY_NOT_EXIXT,
        EXTERN_IS_LABEL,
        FAIL_LABEL
};

void eror(erors_node eror_node,int eror_num);

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

void eror(char *file_name, int line_num, int *flag,char *line, int eror_num);*/

/*
Checks if a string contains only white characters. If so, returns T, otherwise, returns F.
Parameters:
str: The string to check.
Returns: T if the string contains only white characters, otherwise F.
*/
int chek_end_line(char * str);

/* 
Frees the memory allocated for the macros in the head_node.
Parameters:
head_node: The head of the macro linked list.
*/
void free_the_mac(head head_node);


macro_node is_macro_name(char *str,head head_node);

void change_extension(char *filename, const char *new_ext);

#ifdef PRE_PROSES

void first_pass(char *am_name,head *head_node_mac);

#endif /*end of PRE_PROSES */