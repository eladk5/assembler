#include "all.h"
#define NOT_FIRST
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
Note: This function treats any character that is not white or a comma as a number

Parameters:
str: A pointer to the input string containing numbers separated by commas.
Return Values:
COMMA (19): Misplaced or consecutive commas without numbers in between.
MISS_COMMA (21): Missing comma between numbers.
T (1): The string is valid and properly formatted.*/
int check_numbers_saperate(char *str){
    char *temp=str;
    enum{ WAIT_NUM, WAIT_COMMA, IN_NUM};
    int status = WAIT_NUM; 
    while (*temp)
    {
        switch (status)
        {
        case  WAIT_NUM:/*Comma is not allowed until ew get to not blank*/
            if( !isspace(*temp) ){
                if((*temp) == ',')
                    return COMMA;
                else
                    status = IN_NUM;
            }
            break;
        case IN_NUM:
            if(isspace(*temp))
                status = WAIT_COMMA;
            if(*temp == ',')
                status = WAIT_NUM;
            break;
        case WAIT_COMMA:
            if(*temp == ',')
                status = WAIT_NUM;
            else{
                if(!isspace(*temp))
                    return MISS_COMMA;
            }
            break;
        }
        temp++;
    }
    if (status == WAIT_NUM)/*the string ends with comma*/
    {
        return COMMA;
    }
    remove_blanks(str);
    return T;
}

/*Accepts a string and checks if it contains only white characters, if so returns T otherwise F.
This is done by removing the white characters and checking the length of the remaining string*/
int chek_end_line(char * str)
{
	char temp[MAX_LINE_LENGTH];/* Temporary string to store the modified input */
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
Frees the memory allocated for the macros in the head_node.
Parameters:
head_node: The head of the macro linked list.
*/
void free_the_mac(head head_node)
{
    macro_node temp, next_temp;
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
    macro_node temp;
    temp = (head_node.head_of_list);
    while(temp){
        if( strcmp( (temp->name) , str ) == 0 ) 
            return temp;
        temp = temp -> next;
    }
    return NULL;
}

label_node is_label_name(char *str,head head_node)
{
    label_node temp;
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
If an error is detected, it sets an error flag and returns 0.
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
void change_extension(char *filename, const char *new_ext) {
    char *dot = strrchr(filename, '.'); /* Find the last dot in the filename*/
    if (dot != NULL) {
        dot++;
        strcpy(dot, new_ext); /* Replace the extension*/
    }
}
