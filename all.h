#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define START_SIZE 50
#define MAX_SIZE_ENDINDG 12
#define MAX_SIZE_MEMOREY 3996  
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define  MAX_FILE_NAME 255
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
#define IS_WITHIN_12_BITS(num) ((num) >= -2048 && (num) <= 2047)
#define IS_WITHIN_15_BITS(num) ((num) >= -16384 && (num) <= 16383)



/* Structure to hold command names and their corresponding opcodes */
struct {
    char name[MAX_LABEL_LENGTH];
    int method_allowed[MAX_OPERAND][NUM_OF_METODS];/*Each place in the array represents an addressing method 
    for a particular operand in the command, and says whether it is allowed or not to use it*/
} cmd[]={/*Each command here is placed in the place of its opcode in the array */
        {"mov", { {T,T,T,T}, {F,T,T,T} } },
        {"cmp", { {T,T,T,T}, {T,T,T,T} } },
        {"add", { {T,T,T,T}, {F,T,T,T} } },
        {"sub", { {T,T,T,T}, {F,T,T,T} } },
        {"lea", { {F,T,F,F}, {F,T,T,T} } },
        {"clr", { {F,F,F,F}, {F,T,T,T} } },
        {"not", { {F,F,F,F}, {F,T,T,T} } },
        {"inc", { {F,F,F,F}, {F,T,T,T} } },
        {"dec", { {F,F,F,F}, {F,T,T,T} } },
        {"jmp", { {F,F,F,F}, {F,T,T,F} } },
        {"bne", { {F,F,F,F}, {F,T,T,F} } },
        {"red", { {F,F,F,F}, {F,T,T,T} } },
        {"prn", { {F,F,F,F}, {T,T,T,T} } },
        {"jsr", { {F,F,F,F}, {F,T,T,F} } },
        {"rts", { {F,F,F,F}, {F,F,F,F} } },
        {"stop",{ {F,F,F,F}, {F,F,F,F} } },
        {".entry",  {{F}, {F}}},
        {".extern", {{F}, {F}}},
        {".data",   {{F}, {F}}},
        {".string", {{F}, {F}}}  
   };

enum {
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    ENTRY,
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
        NUM_REP
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
        unsigned int e_r: 2;/*will be 0 because its the first command*/
        unsigned int a: 1;/*will be 1 because its the first command*/
        unsigned int target_method: 4;
        unsigned int source_method: 4;
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
        unsigned int target_operand: 3;
        unsigned int source_operand: 3;
        unsigned int reserved: 6;/*not in use*/
    } register_word;
};
typedef union Instruction instruction;
enum method{
    
    NUMBER,/*for numbers instraction*/
    LABEL,/*for label instraction*/
    P_REGISTER,/*for register pointer*/
    D_REGISTER,/*for direct register*/
    FIRST/*for first instraction in command*/
};
struct Command
{
        int type_of_instraction;/*first command, namber,label or register (from enum instraction_type)*/
        instruction ins;/*the struct of instruction*/
};
typedef struct Command command;

struct Label{
    char name[MAX_LABEL_LENGTH+1];/*1 for the null*/
    int flag_type; /*promote or inctraction*/
    int adress;
    int is_entry;/*1 if he was daclared as entry otherwize 0*/
    struct Label *next;
};
typedef struct Label *label_node;

/* 
Frees the memory allocated for the macros in the head_node.
Parameters:
head_node: The head of the macro linked list.
*/
static void free_the_mac(head head_node);


macro_node is_macro_name(char *str,head head_node);

/*The get_num function converts a string representing a number into an integer. 
It handles optional leading '+' or '-' signs and sets an error flag if the string contains non-numeric characters.

Parameters:
am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
str: The string to be converted to an integer.
return: The number read from the string. (If there is an error, junk is returned)*/
int get_num(char *am_name,int line_num,int *eror_flag,char *line,char *str);


/*
This function verifies that a string of numbers separated by commas is correctly formatted.
It ensures numbers are properly separated by commas, handles misplaced commas, and removes extra spaces if the string is valid.
(A misplaced comma is one that appears at the beginning, at the end, or if there are two or more consecutive commas.)
Note: This function treats any character that is not white or a comma as a number

Parameters:
str: A pointer to the input string containing numbers separated by commas.
Return Values:
COMMA (19): Misplaced or consecutive commas without numbers in between.
MISS_COMMA (21): Missing comma between numbers.
T (1): The string is valid and properly formatted.*/
int check_numbers_saperate(char *str);
