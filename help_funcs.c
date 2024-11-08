#define MACRO
#include "all.h"
#define HELP_FUNCS
#include "passes.h"


/*Gets string and remuving all the blanks from the string
by loop that index j goes all over the string and the index i is promoted only when j finds a character that is not a blunk and puts in the i position what is in the j position */
static void remove_blanks(char str[])
	{
	int i=0, j=0;/* for index */
	int l = strlen(str);/* for the length of the string */
	while (j < l) 
		{
        	if (!isspace(str[j])) 
            		str[i++] = str[j]; 
        	j++;
    		}
    	str[i] = '\0';  /* Add null */
	}


/*
This function verifies that a string of numbers separated by commas is correctly formatted using a state machine,
which follows whether we are now expecting a number, a comma or within a number. and changes situations accordingly 
It ensures numbers are properly separated by commas, handles misplaced commas, and removes extra spaces if the string is valid.
Note: This function treats any character that is not white or a comma as a number(if its not number it will be handals later)
Parameters:
str: A pointer to the input string containing numbers separated by commas.
Return Values:
COMMA (18): Misplaced or consecutive commas without numbers in between.
MISS_COMMA (20): Missing comma between numbers.
T (1): The string is valid and properly formatted.*/
int check_numbers_saperate(char *str){
    char *temp=str;
    enum{ WAIT_NUM, WAIT_COMMA, IN_NUM};
    int status = WAIT_NUM; 
    while (*temp)
    {
        switch (status)
        {
        case  WAIT_NUM:/*Comma is not allowed until we get to not blank*/
            if( !isspace(*temp) ){
                if((*temp) == ',')
                    return COMMA;/*the string start with comma*/
                else
                    status = IN_NUM;
            }
            break;
        case IN_NUM:/*number reading wait for space or comma*/
            if(isspace(*temp))
                status = WAIT_COMMA;
            if(*temp == ',')
                status = WAIT_NUM;
            break;
        case WAIT_COMMA:/*after number wait for comma*/
            if(*temp == ',')
                status = WAIT_NUM;
            else{
                if(!isspace(*temp))/*if two numbers dont saperatte by comma*/
                    return MISS_COMMA;
            }
            break;
        }
        temp++;
    }
    if (status == WAIT_NUM)
    {
        return COMMA;/*the string ends with comma*/
    }
    remove_blanks(str);
    return T;
}

/*Accepts a string and checks if it contains only white characters, if so returns T otherwise F.
This is done by removing the white characters and checking the length of the remaining string*/
int chek_end_line(char * str)
{
	char temp[MAX_LINE_LENGTH+NULL_SIZE];/* Temporary string to store the modified input */
	strcpy(temp, str);
    remove_blanks(temp);
    if(strlen(temp)>0)
        return  F;
    return T;
}

/*get string and checks if it is name of command, if so return the opcode of the command (or for promote the enum value), 
otherwise return -1 (NOT_COMMAND) */
int search_command (char *str)
{
	int i;
	for(i=0;i <= STRING; i++){
		if(strcmp( str , cmd[i].name ) == 0 )
			return i;
	}
	return NOT_COMMAND;
}
/* 
Frees the memory allocated for the macros list.
Parameters:
head_node: The head of the macro linked list.
*/
void free_the_mac(head head_node)
{
    macro_node temp, next_temp;/*temporary nodes*/
    if (head_node.head_of_list )
    {
        temp = head_node.head_of_list;
        while(temp){
            next_temp = (temp -> next);
            free(temp);
            temp = next_temp;   
        }
    }
}
/* 
Frees the memory allocated for the labels list.
Parameters:
head_node: The head of the labels linked list.
*/
void free_the_labels(head head_node)
{
    label_node temp, next_temp;/*temporary nodes*/
    if (head_node.head_of_list )
    {
        temp = head_node.head_of_list;
        while(temp){
            next_temp = (temp -> next);
            free(temp);
            temp = next_temp;   
        }
    }
}

/* 
Checks if a given string is the name of a defined macro by going through the list of macros
Parameters:
str: The string to check.
head_node: The head of the macro linked list.
Returns: The macro node if the string is a macro name, otherwise NULL.
*/
macro_node is_macro_name(char *str,head head_node)
{
    macro_node temp;/*temporary node*/
    temp = (head_node.head_of_list);
    while(temp){
        if( strcmp( (temp->name) , str ) == 0 ) 
            return temp;
        temp = temp -> next;
    }
    return NULL;
}
/* 
Checks if a given string is the name of a defined label by going through the list of labels
Parameters:
str: The string to check.
head_node: The head of the labels linked list.
Returns: The macro node if the string is a macro name, otherwise NULL.
*/
label_node is_label_name(char *str,head head_node)
{
    label_node temp;/*temporary node*/
    temp = (head_node.head_of_list);
    while(temp){
        if( strcmp( (temp->name) , str ) == 0 ) 
            return temp;
        temp = temp -> next;
    }
    return NULL;
}


/*Converts a string representing a number into an integer, 
handling optional leading '+' or '-' signs. 
It uses strtol for conversion and checks for errors such as non-numeric characters or out-of-range values. 
The function processes the string, skips an optional sign, converts the numeric part, and negates the result if necessary.
Parameters:
am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
str: The string to be converted to an integer.
return: The number read from the string. (If there is an error, junk is returned)
*/
int get_num(erors_node eror_node, char *str)
{
    int num ;
    char *temp, c = *str;
    if( c == '+' || c == '-')
        str++;
    num = strtol(str,&temp,DECIMAL);
    if(*temp)/*Checks if the end of the converted string is NULL*/
        eror(eror_node,NOT_NUMBER);
    if(c == '-')
        num = (-num);
    return num;
}

/*change the extension of filename to new_ext */
void change_extension(char *file_name, const char *new_ext) {
    char *dot = strrchr(file_name, '.'); /* Find the last dot in the filename*/
    if (dot != NULL) {
        dot++;
        strcpy(dot, new_ext); /* Replace the extension*/
    }
}
/*cheks how many times the char c exixt in str
parameters;
str: the string to chek
c: the char to look for*/
int how_many_c(char *str, char c)
{
    char *temp = str;/*pointer*/
    int count = 0;/*counter to char c*/
    while (temp && *temp)
    {
        if(*temp == c)
            count++;
        temp++;
    }
    return count;
}

/*
This function checks if the given string is a register name.
It compares the input string to an array of register names.
parameters:
str - The string to check.
Return: the num of the register if found, otherwise NOT_COMMAND(-1).
*/
int is_reg(char *str)
{
    char regs[][SIZE_NAME_REG] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}; /* Array of register names by order*/
    int i;
    for(i=0;i<NUM_OF_REGS;i++){
        if(strcmp(str,regs[i])==0)
            return i;/* Return the num of the register if a match is found */
    }
    return NOT_COMMAND;
}