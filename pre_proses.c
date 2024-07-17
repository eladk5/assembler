#include "all.h"
#include "help_funcs.c"
#include "erors.c"


/* 
Checks if a given string is the name of a defined macro by going through the list of macros
Parameters:
str: The string to check.
head_node: The head of the macro linked list.
Returns: The macro node if the string is a macro name, otherwise NULL.
*/
static macro_node is_makro_name(char *str,head head_node)
{
    macro_node temp;
    if ( search_command(str) != NOT_COMMAND )
        return NULL;
    temp = head_node.head_mac;
    while(temp){
        if( strcmp( (temp->name) , str ) == 0 ) 
            return temp;
        temp = temp -> next;
    }
    return NULL;
}
/* 
writes the macro content called to the am file.
By looking for the location of the macro definition in the input file,
reading lines from the input file to the end of the macro definition 
and writing each line of the macro to the output file.
Parameters:
as_file: The input assembly file.
am_file: The output assembly file.
macr: The macro node containing the macro to expand.
*/
static void macro_call(FILE *as_file, FILE *am_file, macro_node macr )
{
    char line[MAX_LINE_LENGTH], temp[MAX_LABEL_LENGTH];
    int line_num =  (macr -> line_number); 
    fseek(as_file,( macr -> offset) , SEEK_SET);
    while (fgets(line, MAX_LINE_LENGTH, as_file) ){
        line_num++;
        if(sscanf(line,"%s",temp) && strcmp(temp,"endmacr") == 0){
            return; 
        }
        fprintf(am_file,"%s",line);    
    }
}
/* 
After declaring a macro, checks the correctness of the macro statement, puts it in the list of macros if it is correct 
(if the macro name is not the name of another command or macro).
if not correct, sends Error Message.
Parameters:
as_file: The input as file.
name: The name of the macro.
head_node: Pointer to the head of the macro linked list.
flag: Pointer to an integer flag to indicate an error occurred.
line_num: The line number where the macro is defined.
line: The line where the macro is defined.
*/
static void read_macr(FILE *as_file,char *name, head *head_node,int *flag,int line_num,char *line)
{
    macro_node temp;/*the node of the new macro*/
    macro_node new_node = (macro_node)malloc(sizeof(macro_node));
    if (new_node == NULL) {/*memory alucation fail */
        fprintf(stderr,"Failed to allocate memory/n");
        fclose(as_file);
        free_the_mac(*head_node);
        exit(1);
    }
    strcpy(new_node->name, name);
    new_node->offset = ftell(as_file);
    new_node->next = NULL;
    new_node-> line_number = line_num;
    if ( search_command(name) != NOT_COMMAND )/*Cheks if it is a command name*/
    {
         eror(name, line_num, flag,line, MAC_NAME_COMMAND);
         return;
    }
    if ((head_node->head_mac) == NULL )/*if its the first macro*/
    {
        (head_node->head_mac) = new_node;
        return;
    }
    temp = (head_node->head_mac);
    while(temp){/*Going through the list of macros, if the new macro name is already found, an error is reported.
     If not, adds the new node to the end of the list*/
        if( strcmp( (temp->name) , name ) == 0 ){  
            eror(name,line_num,flag,line,MAC_NAME_AGAIN);
            return;
        }
        if (temp->next == NULL){
            temp->next = new_node ;
            return;
        }
        temp = temp -> next;
    }     
}

/* 
The function processes an input assembly file to expand macros and write the expanded code to a new file with a .am extension.
It initializes necessary variables and states, then iterates through each line of the input file. 
Depending on the current state, it identifies macro definitions (macr) and expansions, handling each case appropriately. 
When a macro definition is detected, it reads and stores the macro. When a macro call is found, it expands the macro by writing its content to the output file. 
The function manages errors such as undefined macros, extra characters, and unclosed macros, and updates the state accordingly. 
Finally, if no errors were found during the preassembler send to the first pass of the assemly. 
else free the aloctar memory for the macro list.
Parameters:
as_file: The input assembly file.
file_name: The name of the input file.
Returns: T if no errors were found during the preassembler, otherwise F.
*/
void pre_pros(FILE *as_file, char *file_name)
{
    head head_node_macro,*head_node= &head_node_macro;/* Head of the macro linked list */
    int state = NOT_MACRO;/* Current state of the state machine */
    macro_node temp_node;/* Temporary macro node */
    char first_word[MAX_LINE_LENGTH],second_word[MAX_LINE_LENGTH]; /* First and second word in the current line */
    char *rest; /*For the remaining part of the line after the first two words */
    int flag = T; /* flag to indicate if no errors were found*/
    char line[MAX_LINE_LENGTH];/* To store new line evry time */
    FILE *am_file; /* Output file with .am extension */
    int line_number = 0;/* Counter for line numbers */
    int temp,temp_buffer; /* Temporary variables*/
    char new_file_name[MAX_FILE_NAME];/* To store the name of the current file whith .am ending */

    (head_node->head_mac) = NULL;
    strcpy(new_file_name,file_name);
    new_file_name[strlen(new_file_name)-1]='m'; /* Change the extension to .am */
    if ((am_file=fopen(new_file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",new_file_name);/* the file dont open */
        return;
		} 
    while (fgets(line, MAX_LINE_LENGTH ,as_file) ) {/*reading new line*/
        first_word[0] = '\0';
        second_word[0] = '\0';
        line_number++;
        switch (state)
        {
        case NOT_MACRO:/*The normal state in which a macro is not read or called*/
            if ( (temp = sscanf(line, "%s%s", first_word, second_word)) ){
                if ( (strcmp(first_word, "macr") )== 0){/*Is a macro statement*/
                    if  ( temp>1 && strlen(second_word) <= MAX_LABEL_LENGTH) {
                        rest= strstr(line, second_word) + strlen(second_word);
                        if ( chek_end_line(rest) ){
                          state=MACRO_READ;
                          /*A call to a function that will add this macro to the list of macros*/
                          read_macr(as_file,second_word,head_node,&flag,line_number,line);
                        }
                        else/* extra chars case*/
                            eror(file_name,line_number,&flag,line, EXTEA_CHARS );  
                    }
                 
                    else{ /*else to ( temp>1 && strlen(second_word) <= MAX_LABEL_LENGTH)*/
                        if(temp == 1)/*there is no macro name*/
                            eror(file_name,line_number,&flag,line, NO_MAC_NAME );
                        else/*the name of the macro is to long*/
                            eror(file_name,line_number,&flag,line, MAK_NAME_LONG );
                    }
                }
                if((temp_node = is_makro_name(first_word, *head_node))){/*if the line calls out to macro*/
                    if (temp > 1)/* extra chars case*/
                        eror(file_name,line_number,&flag,line, EXTEA_CHARS );
                    else{
                        /*Saves the current location in the file and calls a function that will copy the contents of the macro*/
                        temp_buffer = ftell(as_file);
                        macro_call(as_file,am_file,temp_node);
                        fseek(as_file, temp_buffer, SEEK_SET);
                        state = MACRO_CALL;
                    }

                }
            }
            if( state == NOT_MACRO )
                fprintf(am_file, "%s", line); 
            if(state == MACRO_CALL)   
            {
                state = NOT_MACRO;
            }   
            break;
            
            case MACRO_READ:/*When declaring a macro, reads the contents of the macro without printing to an output file 
            until a notification of the end of the macro*/
                if(sscanf(line,"%s",first_word) && strcmp(first_word,"endmacr") == 0){
                    rest= strstr(line, first_word) + strlen(first_word);
                    if ( chek_end_line(rest) == F )/*extra chars*/
                        eror(file_name,line_number,&flag,line, EXTEA_CHARS);
                    state = NOT_MACRO;
                }
                break;
            default:
                break;
            }
    
    }
    if(state == MACRO_READ)/*If we finished the file when there is a macro that never closed*/
        eror(file_name,line_number,&flag,line, MACRO_NOT_CLOSE);
    fclose(am_file);
    /*if(flag)
		first_pass(new_file_name);
        else*/
                free_the_mac(*head_node);
        
}


int main()
{
    FILE *ifp;
    ifp=fopen("test_fails_macro.as","r");
    pre_pros(ifp,"test_fails_macro.as");
    return 0;
}