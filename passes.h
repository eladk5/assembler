#define IS_WITHIN_12_BITS(num) ((num) >= -2048 && (num) <= 2047)
#define IS_WITHIN_15_BITS(num) ((num) >= -16384 && (num) <= 16383)
#define METHOD(A) ( 1 << A )
#define FIRST_MEM 100


/* Structure to hold command names and their corresponding opcodes */
typedef struct {
    char name[MAX_LABEL_LENGTH];
    int method_allowed[MAX_OPERAND][NUM_OF_METODS];/*Each place in the array represents an addressing method 
    for a particular operand in the command, and says whether it is allowed or not to use it*/
} Cmd;
#ifdef FIRST_PASS
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
   void free_the_labels(head head_node);
#endif
#ifdef NOT_FIRST
extern Cmd cmd[];
#endif

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
        int line_number;
        int type_of_instraction;/*first command, namber,label or register (from enum instraction_type)*/
        char label_name[MAX_LINE_LENGTH];/*only for label commands, will contain the label*/
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
/* This function processes a line of assembly code with a single operand, reads the operand, verifies its method,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operand.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
void one_operand(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] );
/*  This function processes a line of assembly code with two operands, reads the operands, verifies their methods,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operands.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
void two_operands(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] );
/*This function processes a line of entry or extern. The function adds the label name to the appropriate array, 
and allocates more memory to the array if necessary. 
In case of a lack of a parameter or unnecessary characters, an error is reported and marked accordingly.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the name to be processed.
counter: Pointer to an integer that keeps track of the number of names added to the array.
names_arrey: Pointer to the dynamic array of strings where the names will be stored.
size: Pointer to an integer representing the current size of the dynamic array.*/
void add_ext_en(erors_node eror_node,char *rest_line,int *counter ,char (**names_arrey)[MAX_LINE_LENGTH],int *size,int **name_line_num);

/*This function processes a line of assembly code to add numerical data to the data array,
ensuring the input format is correct. It first checks if the commas in the input string are correctly placed,
then tokenizes the string based on commas, converts each token to a number using get_num, 
and adds each number to the data array. If an error is detected, it sets an error flag and returns.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the data to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the numerical data will be stored.*/
void add_data(erors_node eror_node,char *rest,int *dc,short (*data)[]);
/*Processes the argument part of ".string" promote. The function counts with the help of how_many_c the amount of qoutes.
 And so while going over the line,
it checks that all the characters are between the first and the last " and enters them into data.
Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the string to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the string will be stored.
*/
void add_string(erors_node eror_node, char *rest_line,int *dc,short (*data)[]);
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



label_node is_label_name(char *str,head head_node);
void eror_label(char *file_name, int *eror_flag, int line_num, int eror_num);
int ob_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY] ,int dc );
int ent_print(char *file_name, head labels );
int ext_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic);