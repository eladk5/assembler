#include "all.h"


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
void eror(erors_node eror_node,int eror_num){
    *eror_node.flag = F;
    printf("\neror: in file: %s,line number %d\nin the line:%s\n",eror_node.file_name,*eror_node.line_num,eror_node.line);
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
         printf("There are extra characters at the end of the line. Maybe you inserted extra operands?\n");\
          break;
    case MAC_NAME_LONG:
        printf("The name of the macro is to long, it must not be more than 31 characters\n");
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
    case LABEL_LENGTH:
        printf("The label defined in this line contains more than 31 characters and is therefore too long\n");
        break;
    case NOT_ALPHA:
        printf("Valid label must begin with alphabetic char\n");
        break;
    case NOT_ALPHA_NUM:
        printf("Valid label must have only alphabetic or numbers chars\n");
        break;
    case LABEL_IS_COMMAND:
        printf("A label name must not be the name of a command\n");
        break;
    case LABEL_IS_MACRO:
        printf("A label name must not be the name of a macro\n");
        break;
    case LABEL_NAME_AGAIN:
        printf("the name of the macro is taken by other label\n");
        break;
    case MAC_START:
        printf("Valid macro name must begin with alphabetic char\n");
        break;
    case FAIL_STRING:
        printf("Invalid string value, the entire string must be between \" \"\n");
        break;
    case NOT_NUMBER:
        printf("Invalid value,No numeric value was received in the intended location \n");
        break;
    case COMMA:
        printf("An invalid comma was found. Note that the line cannot start or end with a comma, and 2 commas in a row without a numerical value in between are not allowed.\n");
        break;
    case NO_PARAMS:
        printf("promote or instraction was declared without the right amuont of parameters\n");
        break;
    case MISS_COMMA:
        printf("All two numbers must be separated by a comma\n");
        break;
    case NO_VALID_METHOD:
        printf("A command was sent with an operand that does not match the command type\n");
        break;
    default:
        break;
    case NUM_REP: 
        printf("The number you entered is too large or too small, and is not suitable for representation in the appropriate number of bits\n");
        break;
    
    }
    printf("-------------------------------------------------------");
}
void eror_label(char *file_name, int *eror_flag, int line_num, int eror_num)
{
    *eror_flag = F;
    printf("\neror: in file: %s,line number %d\n",file_name,line_num);
    switch(eror_num)
    {
        case ENTRY_NOT_EXIXT:
        printf("In this line you declared a label as an entry but this label does not exist in the file.\n");
        break;
        case EXTERN_IS_LABEL:
        printf("In this line you declared a label as an extern but this label also regular label in your file.\n");
        break;
        case FAIL_LABEL:
        printf("In this line you declared the insertion of an argument that was not declared as an outer or normal label. and is not a valid number or register.\n");
        break;
    }
    printf("-------------------------------------------------------");

}