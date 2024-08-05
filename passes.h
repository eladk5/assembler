#define FIRST_MEM 100/*the first usable place in memorey*/
#define START_SIZE 50 /*the first size of dinamic arrey*/
#define ONE_CHAR 1 /*size of one character*/
#define CHAR_TO_NAM(A) ( (A)-'0' ) /*convert one digit char to number*/
#define IS_WITHIN_12_BITS(num) ((num) >= -2048 && (num) <= 2047)/* if num fits in 12 bits */
#define IS_WITHIN_15_BITS(num) ((num) >= -16384 && (num) <= 16383)/* if num fits in 15 bits */
#define METHOD(A) ( 1 << A )

#ifdef PRINT
/*numbers of bits*/
#define ONE_BIT 1 
#define TWO_BITS 2
#define THREE_BITS 3
#define SIX_BITS 6
#define SEVEN_BITS 7
#define ELEVEN_BITS 11
#define FULL_BIT_INSTRACTION (0x7FFF) /*in binary its 0111111111111111*/
#endif
/*flags for diffrent cases*/
enum flags{
    SOURCE,/*soursce operand*/
    TARGET,/*target operand*/
    INCTRACTION,/*inctractin line*/
    DIRECTIVE,/*directive line*/
    RESRERVE/*for extra labels*/
};

/* Structure to hold command names and their allowed methods */
typedef struct {
    char name[MAX_LABEL_LENGTH];/*command name*/
    int method_allowed[MAX_OPERAND][NUM_OF_METODS];/*Each place in the array represents an addressing method 
    for a particular operand in the command, and says whether it is allowed or not to use it*/
} Cmd;

/*Contains the appropriate command word according to each type of the word*/
union Instruction {
    struct {
        unsigned int e_r: 2;/*will be 0 because its the first command*/
        unsigned int a: 1;/*will be 1 because its the first command*/
        unsigned int target_method: 4;/*The addressing method of the target operand*/
        unsigned int source_method: 4;/*The addressing method of the source operand*/
        unsigned int opcode: 4;/*opcode*/
    } first_command;/*the first word in command*/

    struct {
        unsigned int e_r: 2;/*will be 0 because its the number command*/
        unsigned int a: 1;/*will be 1 because its the number command*/
        int value: 12;/*not unsigned beacuse the value can be negative*/
    } number_word;/*the word for number method*/

    struct {
        unsigned int e: 1;/*e fild*/
        unsigned int r: 1;/*r fild*/
        unsigned int a: 1;/*will be 0 because its the label command*/
        unsigned int adress: 12;/*adress if it's local otherwize 0*/
    } label_word;/*the word for label method*/
    struct {
        unsigned int e_r: 2;/*will be 0 because its the register command*/
        unsigned int a: 1;/*will be 1 because its the register command*/
        unsigned int target_operand: 3;/*The register number of the destination operand*/
        unsigned int source_operand: 3;/*The register number of the source operand*/
        unsigned int reserved: 6;/*not in use*/
    } register_word;/*the word for register method*/
};
typedef union Instruction instruction;

enum method{/*flags for each type of method*/
    NUMBER,/*for numbers instraction*/
    LABEL,/*for label instraction*/
    P_REGISTER,/*for register pointer*/
    D_REGISTER,/*for direct register*/
    FIRST/*for first instraction in command*/
};
/* Structure for evry inctraction word*/
struct Command
{
        int line_number;/*the line number of the command*/
        int type_of_instraction;/*first command, namber,label or register (from enum instraction_type)*/
        char label_name[MAX_LINE_LENGTH];/*only for label commands, will contain the label*/
        instruction ins;/*the struct of instruction*/
};
typedef struct Command command;
/*struct for label node in list*/
struct Label{
    char name[MAX_LABEL_LENGTH+ONE_CHAR];/*the label name*/
    int flag_type; /*dirctive or inctraction (or reserve)*/
    int adress;/*the adress of tje label*/
    int is_entry;/*T if he was daclared as entry otherwize F*/
    struct Label *next;
};
typedef struct Label *label_node;

#ifdef HELP_FUNCS
extern Cmd cmd[];/*An array in which each command is in its opcode (and at the end there are also the diractive)*/
#endif

enum { /*for each command it's opcode*/
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
/* 
Checks if a given string is the name of a defined label by going through the list of labels
Parameters:
str: The string to check.
head_node: The head of the labels linked list.
Returns: The macro node if the string is a macro name, otherwise NULL.
*/
label_node is_label_name(char *str,head head_node);
/*Handles some of label associated errors by printing the appropriate error, as well as changing the eror flag.
parameters:
file_name: The name of the file where the error occurred
eror_flag: pointer to eror flag
line_num: the number of the line where the error occurred
eror_num: the type of the eror from the enum erors  
*/
void eror_label(char *file_name, int *eror_flag, int line_num, int eror_num);

#ifdef SECOND_PASS /*only for second_pass file*/
/*
The function generates the object file from the commands and data sections with ob extension.
The object code is printed in octal format along with its memory address.

Parameters:
file_name: The name of the origin file.
coms: The array of commands.
ic: The instruction counter.
data: The array of data.
dc: The data counter.
Returns:T if the operation is successful, F otherwise.
*/
int ob_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY] ,int dc );
/*
The function writes the entry labels and their addresses to a file with ent extension.

Parameters:
file_name: The name of the origin file.
labels: The head of the label list.
Returns:T if the operation is successful, F otherwise.
*/
int ent_print(char *file_name, head labels );
/*
The function writes the external labels and the addresses of lines thats use them to  file whith ext extensions.

Parameters:
file_name: The name of the origin file.
coms: The array of commands.
ic: The instruction counter.
Returns:T if the operation is successful, F otherwise.
*/
int ext_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic);
#endif
#ifdef STRING_TO_DATA /*for string_to_data file*/
#define DOUBLE 2 /*double size*/
#define MIN_QUOTES 2 /*the minimum amount of qoutse in string */
Cmd cmd[]={/*Each command here is placed in the place of its opcode in the array */
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
/*
This function verifies that a string of numbers separated by commas is correctly formatted 
It ensures numbers are properly separated by commas and removes extra spaces if the string is valid.
Note: This function treats any character that is not white or a comma as a number(if its not number it will be handals later)
Parameters:
str: A pointer to the input string containing numbers separated by commas.
Return Values:
COMMA (19): Misplaced or consecutive commas without numbers in between.
MISS_COMMA (21): Missing comma between numbers.
T (1): The string is valid and properly formatted.*/
int check_numbers_saperate(char *str);

/*The get_num function converts a string representing a number into an integer. 
It handles optional leading '+' or '-' signs and sets an error flag if the string contains non-numeric characters.
Parameters:
am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
str: The string to be converted to an integer.
return: The number read from the string. (If there is an error, junk is returned)*/
int get_num(erors_node eror_node,char *str);

/*cheks how many times the char c exixt in str
parameters;
str: the string to chek
c: the char to look for
retuens: the times that char c exixt in str*/
int how_many_c(char *str, char c);
#endif/*string_to_data*/

