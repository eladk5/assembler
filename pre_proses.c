#define PRE_PROSES
#define MACRO
#include "all.h"


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
    char line[MAX_LINE_LENGTH+NULL_SIZE]/*to store the lines*/, temp[MAX_LABEL_LENGTH]/*temporary string*/;
    fseek(as_file,( macr -> offset) , SEEK_SET);/*reach the place in the file of the macro contant*/
    while (fgets(line, MAX_LINE_LENGTH, as_file) ){/*read line*/
        if(sscanf(line,"%s",temp) && strcmp(temp,"endmacr") == 0){/*checks if we end the macro*/
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
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
*/
static void read_macr(FILE *as_file, char *name, head *head_node,erors_node eror_node)
{
    macro_node temp;/*temporary*/
    macro_node new_node; /*the node of the new macro*/
    if(!isalpha(*name)){/*Cheks if the first char is latter*/
        eror(eror_node, MAC_START);
        return;
    }
    if ( search_command(name) != NOT_COMMAND ||strcmp(name,"macr")==0 ||strcmp(name,"endmacr")==0 )/*Cheks if it is a command name*/
    {
         eror(eror_node,MAC_NAME_COMMAND);
         return;
    }
    if ( is_reg(name) != NOT_COMMAND )/*Cheks if it is a register name*/
    {
         eror(eror_node,MAC_NAME_REG);
         return;
    }
    new_node = (macro_node)malloc(sizeof(struct MacroNode));
    if (new_node == NULL) {
        fprintf(stderr,"Failed to allocate memory\n");/*memory alucation fail */
        fclose(as_file);
        free_the_mac(*head_node);
        exit(1);
    }
    /*update the node*/
    strcpy(new_node->name, name);
    new_node->offset = ftell(as_file);
    new_node->next = NULL;
    new_node-> line_number = *eror_node.line_num;
    if ((head_node->head_of_list) == NULL )/*if its the first macro*/
    {
        (head_node->head_of_list) = new_node;
        return;
    }
    temp = (macro_node)(head_node->head_of_list);
    while(temp){/*Going through the list of macros, if the new macro name is already found, an error is reported.
     If not, adds the new node to the end of the list*/
        if( strcmp( (temp->name) , name ) == 0 ){  
            eror(eror_node,MAC_NAME_AGAIN);/*the name taken*/
            free(new_node);
            return;
        }
        if (temp->next == NULL){
            temp->next = new_node ;/*add the new node to the end of the list*/
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
Parameters:
as_file: The input assembly file.
file_name: The name of the input file.
*/
void pre_pros(FILE *as_file, char *file_name)
{
    enum{ NOT_MACRO, MACRO_READ, MACRO_CALL};/*the states of the state machine*/
    head head_node_macro,*head_node= &head_node_macro;/* Head of the macro linked list */
    int state = NOT_MACRO;/* Current state of the state machine */
    macro_node temp_node;/* Temporary macro node */
    char first_word[MAX_LINE_LENGTH],second_word[MAX_LINE_LENGTH]; /* First and second word in the current line */
    char *rest; /*For the remaining part of the line after the first two words */
    int flag = T; /* flag to indicate if no errors were found*/
    char line[MAX_LINE_LENGTH+NULL_SIZE];/* To store new line evry time */
    FILE *am_file; /* Output file with .am extension */
    int line_number = 0;/* Counter for line numbers */
    int temp,temp_buffer; /* Temporary variables*/
    char *new_file_name;/* To store the name of the current file whith .am ending */
    
    /*update the eror node and labels list*/
    erors_node eror_node;
    eror_node.file_name= file_name;
    eror_node.flag = &flag;
    eror_node.line_num = &line_number;
    (head_node->head_of_list) = NULL;
    new_file_name = malloc(strlen(file_name)+MAX_SIZE_ENDINDG);
    if (new_file_name == NULL) {
			fprintf(stderr,"\nFailed to allocate memory\n");/*memorey allocatin fails*/
			exit(1);
		}
    strcpy(new_file_name,file_name);
    change_extension(new_file_name,"am"); /* Change the extension to .am */
    if ((am_file=fopen(new_file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",new_file_name);/* the file didnt open */
        return;
		} 
    while (fgets(line, MAX_LINE_LENGTH ,as_file) ) {/*reading new line*/
        eror_node.line=line;
        first_word[0] = '\0';
        second_word[0] = '\0';
        line_number++;
        switch (state)
        {
        case NOT_MACRO:/*The normal state in which a macro is not read or called*/
            if ( (temp = sscanf(line, "%s%s", first_word, second_word)) ){
                if ( (strcmp(first_word, "macr") )== 0){/*Is a macro statement*/
                    if  ( temp > ONE_SCAN && strlen(second_word) <= MAX_LABEL_LENGTH) {
                        /*If 2 words were read and the label name is the correct size*/
                        rest= strstr(line, first_word) + strlen(first_word);
                        rest= strstr(rest, second_word) + strlen(second_word);
                        if ( chek_end_line(rest) ){
                          state=MACRO_READ;
                          /*A call to a function that will add this macro to the list of macros*/
                          read_macr(as_file,second_word,head_node,eror_node);
                        }
                        else/* extra chars case*/
                            eror( eror_node,EXTEA_CHARS );  
                    }
                    else{ /*else to ( temp>1 && strlen(second_word) <= MAX_LABEL_LENGTH)*/
                        if(temp == ONE_SCAN)/*there is no macro name*/
                            eror( eror_node,NO_MAC_NAME );
                        else/*the name of the macro is to long*/
                            eror( eror_node,MAC_NAME_LONG );
                    }
                }
                if((temp_node = is_macro_name(first_word, *head_node))){/*if the line calls out to macro*/
                    if (temp > ONE_SCAN)/* extra chars case*/
                        eror(eror_node, EXTEA_CHARS );
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
                state = NOT_MACRO;/*Because we have already written the macro content to the file*/
            }   
            break;
            
            case MACRO_READ:/*When declaring a macro, reads the contents of the macro without printing to an output file 
            until a notification of the end of the macro*/
                if(sscanf(line,"%s",first_word) && strcmp(first_word,"endmacr") == 0){
                    rest= strstr(line, first_word) + strlen(first_word);
                    if ( chek_end_line(rest) == F )/*extra chars*/
                        eror(eror_node,EXTEA_CHARS);
                    state = NOT_MACRO;
                }
                break;
            default:
                break;
            }
    
    }
    fclose(am_file);
    if(flag)
        /*If everything went well until now, an am file has been created and can be sent for the first pass*/
		first_pass(new_file_name,head_node);
    else
        remove(new_file_name);
    free(new_file_name);
    free_the_mac(head_node_macro);  
}
