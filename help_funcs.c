#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Gets string and remuving all the blanks from the string
by loop that index j goes all over the string and the index i is promoted only when j finds a character that is not a blunk and puts in the i position what is in the j position */
void remove_blanks(char str[])
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

/*get string and checks if it is name of command, if so return the opcode of the command, 
otherwise return 16 (NOT_COMMAND) */
int search_command (char *str)
{
	int i;
	for(i=0;i < NUM_OF_COMMANDS; i++){
		if(strcmp( str , cmd[i].name ) == 0 )
			return i;
	}
	return i;
}