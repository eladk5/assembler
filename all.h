#include<stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_ENDINDG 5/*The largest size a file extension can be*/
#define MAX_SIZE_MEMOREY 3996 /*The size of the memorey that we can use in the computer*/ 
#define MAX_LINE_LENGTH 81 /*The largest size a line*/
#define MAX_LABEL_LENGTH 31 /*The largest size a label*/
#define NOT_COMMAND (-1) /*a flag that we get an eror finding command*/
#define DECIMAL 10 /*becimal base is base of 10*/
#define  F 0 /*false*/
#define  T 1 /*true*/
#define ONE_SCAN 1 /*if we scan only 1 word*/
#define MAX_OPERAND 2 /*the maximum of operand in command*/
#define NUM_OF_METODS 4 /*the number of adresing methods */
#define NULL_SIZE 1 /*the size of null in string*/

/*struct for the information we need to handle an error*/
struct ErorNode{
    char *file_name; /*the name of the file*/
    int *line_num; /*the line number that we checking*/
    int *flag; /*the flag which indicates if an error has occurred*/
    char *line; /*the text of the line (optional)*/
} ;
typedef struct ErorNode erors_node;

/* Structure to represent a macro node in the linked list */
struct MacroNode {
    char name[MAX_LABEL_LENGTH]; /* Name of the macro */
    int offset; /* offset of the macro definition in the file (from the beginning of the file) */
    int line_number; /* Line number where the macro is defined */
    struct MacroNode *next;
};
typedef struct MacroNode *macro_node;

/* Structure to represent the head of generic linked list */
struct Generic_head{
    void *head_of_list;
};
typedef struct Generic_head head;


/* Enumeration for different error types */
enum erors{
        LINE_LENGTH,/*the line is to long*/
        MAC_NAME_AGAIN,/*macro name is allrady in use*/
        MAC_NAME_COMMAND,/*the name of the macro is name of command*/
        EXTEA_CHARS,/*extra chars in the end of line*/
        MAC_NAME_LONG,/*the name of the macro is too long*/
        NO_MAC_NAME,/*miss the name of the macro*/
        OVER_SIZE,/*the space in the computer is over*/
        ONLY_LABEL,/*line with onley label*/
        NOT_VALID_COMMAND,/*not valid name of command*/
        LABEL_LENGTH,/*the lable is too long*/
        NOT_ALPHA,/*dont alphabetic char in the start of label*/
        NOT_ALPHA_NUM,/*dont alphabetic or numeric char in the label*/
        LABEL_IS_COMMAND,/*the lacel is name of command*/
        LABEL_IS_MACRO,/*the label is name of macro*/
        LABEL_NAME_AGAIN,/*repeat name of label*/
        MAC_START,/*the macro start with invalid char (only alphabetic is alowed)*/
        FAIL_STRING,/*string that not all the string betwin ""*/
        NOT_NUMBER,/*not numeric value*/
        COMMA,/*invalid comma*/
        NO_PARAMS,/*miss arguments*/
        MISS_COMMA,/*numbers that not saperates by comma*/
        NO_VALID_METHOD,/*the adressing method of argument does not match command type */
        NUM_REP,/*The number cannot be represented in the appropriate number of bits*/
        ENTRY_NOT_EXIXT,/* an .entry whith undeclared label in file */
        EXTERN_IS_LABEL,/*an .exten whith allredy declared label in file*/
        FAIL_LABEL, /*an argument that does not fit any addressing method and was not declared as a label in any form in the file*/
        LABEL_IS_REG, /*the label is name of register*/
        MAC_NAME_REG /*the name of the macro is name of register*/
};

/*Handles various cases of errors by printing the appropriate error, as well as changing the flag.
parameters:
eror_node: the information we need to handle an error.
eror_num: the type of the eror from the enum erors  
*/
void eror(erors_node eror_node,int eror_num);

/* 
get string and checks if it is name of command, if so return the opcode of the command (or for promote the enum value), 
otherwise return -1 (NOT_COMMAND).
Parameters:
str: The string to check.
Returns: The opcode of the command if found (or for promote the enum value) , otherwise NOT_COMMAND (-1).
*/
int search_command (char *str);

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

/*cheks if str is name of macro
Parameters:
str: the string to chek
head_node: The head of the macro linked list.*/
macro_node is_macro_name(char *str,head head_node);

/*change the part of filename after the dot to new_ext
Parameters:
file_name: the name to change
new_ext: the new extension.*/
void change_extension(char *file_name, const char *new_ext);

/*
This function checks if the given string is a register name.
parameters:
str - The string to check.
Return: the num of the register if found, otherwise NOT_COMMAND(-1).
*/
int is_reg(char *str);

#ifdef PRE_PROSES/*only for the pre_proses file*/

/*
 This function processes the assembly file to collect information about labels, instructions,
 and data declarations. It builds intermediate structures necessary for the second pass,
 and handles special directives such as `.entry` and `.extern`.
 parameters:
 am_name: The name of the assembly file.
 head_node_mac: The head of the macro list.
 */
void first_pass(char *am_name,head *head_node_mac);

#endif /*end of PRE_PROSES */
