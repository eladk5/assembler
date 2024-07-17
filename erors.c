#include<stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
Handles and prints error messages based on the error number.
by setting the flag to F to indicate that an error has occurred. Prints the file name, line number, and content of the line where the error occurred.
And based on the error number, prints the appropriate error message.
Parameters:
file_name: The name of the file where the error occurred.
line_num: The line number where the error occurred.
flag: Pointer to an integer flag to indicate an error occurred.
line: The line where the error occurred.
eror_num: The error number indicating the type of error.
*/
void eror(char *file_name, int line_num, int *flag,char *line, int eror_num){
    *flag = F;
    printf("\neror: in file: %s,line number %d\nin the line:%s\n",file_name,line_num,line );
    switch (eror_num)
    {
    case LINE_LENGTH:
        printf("the Line is too long.\n");
        break;
    case MAC_NAME_AGAIN:
        printf("the name of the macro is taken by other macro\n");
        break;
    case MAC_NAME_COMMAND:
        printf("the name of the macro is illegal because it name of command\n");
        break;
    case EXTEA_CHARS:
         printf("There are extra characters at the end of the line\n");\
          break;
    case MAK_NAME_LONG:
        printf("The name of the makro is to long, it must not be more than 31 characters\n");
         break;
    case NO_MAC_NAME:
        printf("There is a macro declaration without a macro name\n");
        break;
    case MACRO_NOT_CLOSE:
        printf("You finished the file, and there is still a call line for the macro that was not followed by \"endmacr\"\n");
        break;
    case OVER_SIZE:
        printf("You wrote too many commands, the computer doesn't have enough space for it.\nRemember, there is a maximum of 3096 words\n");
        break;
    case ONLY_LABEL:
        printf("In this line only a label appears, after a label there must come a prompt or instruction line\n");
        break;
        case NOT_VALID_COMMAND:
        printf("This line does not have a valid command or prompt\n");
        break; 
    default:
        break;
    }
    printf("-------------------------------------------------------");
}