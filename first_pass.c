#define MACRO
#include "all.h"
#include "passes.h"
#include "first_pass.h"

/*
This function checks if the entry labels are defined in the label list.
It iterates over the entry labels, verifies their existence in the label list, and marks them as entries.

Parameters:
eror_node - The error handling structure.
enters - The array of entry labels.
enters_line_num - The array of line numbers where the entry labels are defined.
en_c - The count of entry labels.
labels - The head of the label list.
*/
static void check_entey(erors_node eror_node, char (**enters)[MAX_LINE_LENGTH],int **enters_line_num, int en_c,head *labels)
{
    int i;
    label_node temp; /* Temporary variable for label node */
    for(i=0; i<en_c ; i++){
        temp = is_label_name((*enters)[i],*labels);
        if(temp)
            (temp->is_entry) = T;/* Mark label as entry */
        else
            eror_label(eror_node.file_name,eror_node.flag,(*enters_line_num)[i],ENTRY_NOT_EXIXT); /* Report error if label not found */
    }
}
/*
This function checks if the extern labels are defined as regular labels in the label list.
It iterates over the extern labels and reports an error if any extern label is found as a regular label.

Parameters:
eror_node - The error handling structure.
exters - The array of extern labels.
exters_line_num - The array of line numbers where the extern labels are defined.
ex_c - The count of extern labels.
labels - The head of the label list.*/
static void check_extern(erors_node eror_node, char (**exters)[MAX_LINE_LENGTH],int **exters_line_num, int ex_c,head *labels)
{
    int i;
    for(i=0; i<ex_c ; i++){
        if(is_label_name((*exters)[i],*labels))
            /* Report error if extern label is found as a regular label */
            eror_label(eror_node.file_name,eror_node.flag,(*exters_line_num)[i],EXTERN_IS_LABEL);
    }

}
/*
This function checks if a label is valid according to the rules.
These rules include length, character types, and uniqueness.
If the label is valid, it is added to the list of labels, Otherwise the error is handled accordingly.
Parameters:
eror_node - The structure used for error handling.
label - The label to check.
head_mac - The head of the macro list.
head_label - The head of the label list.
Returns: A pointer to the new label node if the label is valid, NULL otherwise.
Assumption: a label that comes here is not NULL and ends with ':'*/
static label_node valid_label(erors_node eror_node ,char *label,head *head_mac,head *head_label)
{
    label_node temp, new_label = NULL;/*for temporary and new labels nose*/
    int i, len= strlen(label);/*for the length of label*/
    label[--len]= '\0'; /*removing the ':' and reducing len by 1*/
    if( len > MAX_LABEL_LENGTH ){/*cheks if the length of the label is over 31*/
        eror(eror_node ,LABEL_LENGTH);
        return new_label;
    }
    if( (len == 0) || !isalpha(*label))/*cheks if the first char is alphabetic*/
    {
        eror(eror_node ,NOT_ALPHA);
        return new_label;
    }
    for ( i = 1; i < len; i++) {/*cheks if all the others chars are latters or number*/
        if (!isalnum(label[i])){
            eror(eror_node ,NOT_ALPHA_NUM);
            return new_label;
        }   
    }
    if(search_command(label) != NOT_COMMAND||strcmp(label,"macr")==0 ||strcmp(label,"endmacr")==0 )/*cheks if it is a command name*/
    {
        eror(eror_node ,LABEL_IS_COMMAND);
        return new_label;
    }
    if (is_macro_name(label,*head_mac))/*cheks if it is a macro name*/
    {
        eror(eror_node ,LABEL_IS_MACRO);
        return new_label;
    }
    if (is_reg(label)!= NOT_COMMAND)
    {
        eror(eror_node ,LABEL_IS_REG);
        return new_label;
    }
    
    new_label = (label_node)malloc(sizeof(struct Label));
    if (new_label==NULL)
    {   /*memorey allocation fail*/
        fprintf(stderr,"Failed to allocate memory\n");
        exit(1);
    }
    strcpy(new_label->name,label);
    new_label->adress = 0; /* Initialize address */
    new_label->flag_type = RESRERVE; /* Initialize flag_type */
    new_label->is_entry = F; /* Initialize is_entry */
    new_label->next = NULL;
    if ((head_label->head_of_list) == NULL )/*if its the first label*/
    {
        (head_label->head_of_list) = new_label;
        return new_label;
    }
    temp = (label_node)(head_label->head_of_list);
    while(temp){ /* Go through the list of labels */
        if( strcmp( (temp->name) , label ) == 0 ){  
            /* If the new label name is already found */
            eror(eror_node ,LABEL_NAME_AGAIN);
            free(new_label);
            return  NULL;
        }
        if (temp->next == NULL){
             /* Add the new node to the end of the list */
            temp->next = new_label ;
            return new_label;
        }
        temp = (temp -> next);
    }
    return NULL;
}

/*
This function performs the first pass of the assembly process.
It reads the assembly file line by line, parses the labels and instructions, and stores them in appropriate data structures.
It also checks for errors in labels and instructions.
At the end, it verifies the validity of extern and entry directives and send to the second pass.
Parameters:
am_name - The name of the assembly file.
head_node_mac - The head of the macro list.*/
void first_pass(char *am_name,head *head_node_mac)
{
    int size_en = START_SIZE,size_ex = START_SIZE ;/*Contains the size of the data structures assigned to .extern and .entry respectively*/
    char (*enters)[MAX_LINE_LENGTH],(*exters)[MAX_LINE_LENGTH];/*to contain the names of the externs and entry*/
    int *enters_line_num,*exters_line_num;/*evrey (exters/enters)_line_num[i] will be the line num of (exters/enters)[i]*/
    head label_head;/*the head of labels list*/
    command coms[MAX_SIZE_MEMOREY]={0};/* Array to store all the instruction words */
	short data[MAX_SIZE_MEMOREY]={0};/* array to store all the data or string words */
	int eror_flag=T;/*the flag to mark if eror occurred*/
    char line[MAX_LINE_LENGTH+NULL_SIZE],*rest_line;/* buffers to store lines and remaining parts of lines */
    int ic=0,dc=0,ex_c=0,en_c=0;/* counters for instructions, data words, .extern, and .entry respectively */
    int line_num =0;/*lines counter*/
    char label[MAX_LINE_LENGTH];/*to contain the label evry line*/
    int temp;/*temporary int*/
    FILE *am_file;/*source assembly file*/
    int command_type;/* to store the opcode of the command or the enum value if it's a directive */
    char first_word[MAX_LINE_LENGTH];  /*buffer to read the first word after the label */
    label_node new_label;/*temporary node for label*/
    erors_node eror_node;/*the node for the data needed to to handle eror*/
    /*eror_node update*/
    eror_node.file_name= am_name;
    eror_node.flag = &eror_flag;
    eror_node.line_num = &line_num;

    *line = '\0';
    enters = malloc(START_SIZE * MAX_LINE_LENGTH * sizeof(char));
    exters = malloc(START_SIZE * MAX_LINE_LENGTH * sizeof(char));
    enters_line_num = malloc(START_SIZE *  sizeof(int));
    exters_line_num = malloc(START_SIZE *  sizeof(int));
    if (enters == NULL || exters == NULL || enters_line_num == NULL || enters_line_num == NULL)  {
        printf("\nFailed to allocate memory");/*memorey allocaton fails*/
        exit(1);
    }
    label_head.head_of_list =NULL;
    if ((am_file=fopen(am_name,"r")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",am_name);/* the file dont open */
        return;
		} 
    fseek(am_file, 0, SEEK_SET); 
    while (fgets(line, MAX_LINE_LENGTH+NULL_SIZE, am_file) ) /*reading new line */
    {
        eror_node.line=line;
        new_label=NULL;
        *label = '\0';/*reset the label*/
        *first_word = '\0';/*reset the string*/
        line_num++;
        if (line[strlen(line) - NULL_SIZE ] != '\n' && !feof(am_file)){/*cheks if the size of the line is to long*/
            eror(eror_node ,LINE_LENGTH);
            while (fgetc(am_file) != '\n' && !feof(am_file));/*Reach a new line*/
            } 
        
        if( ! ( *line == ';' || chek_end_line(line) ) )/* Check if it is not a blank or comment line */
        {
            sscanf(line,"%s",label); 
            temp = strlen(label);
            if( temp > 0 && label[temp - NULL_SIZE ] != ':' )/*if the first word of the line isnt a lable*/
                {
                    *label = '\0';/*reset the label*/
                    rest_line = line;
                }
            else
                rest_line = strstr(line,label)+temp;/*now rest line is the part of the line after the label*/
            if (  sscanf(rest_line,"%s",first_word)==0 || (temp = strlen(first_word))==0 )  
            {
             /* If the line is not empty but rest_line is empty, which means only a label appeared */
                eror(eror_node , ONLY_LABEL );
            }
            command_type = search_command(first_word);
            if ( temp && (command_type == NOT_COMMAND))
                eror(eror_node , NOT_VALID_COMMAND);
            if(*label != '\0')
                new_label = valid_label(eror_node, label,head_node_mac,&label_head);
            if(( (*label== '\0') || new_label ) )
            {
                
                temp = strlen(first_word);
                rest_line = strstr(rest_line,first_word)+temp;/*now rest_line is the part of the arguments*/
                if ( (ic+dc) > MAX_SIZE_MEMOREY )  /*cheking if we heave to many words*/
                {
                    eror(eror_node,OVER_SIZE);
                    /* Prevent deviation from the size of the arrays */
                    ic=0;
                    dc=0;
                } 
                if (command_type >= MOV && command_type <= STOP){ /*if it belongs to the opcode of a command */
                    coms[ic].type_of_instraction = FIRST;
                    coms[ic].line_number = line_num;
                    coms[ic].ins.first_command.a=T;
                    coms[ic].ins.first_command.opcode = (unsigned int)command_type;
                }
                switch (command_type)
                {
                    case STRING:
                        if(*label != '\0'){/*if there is label*/
                            (new_label->adress) = dc;
                            (new_label->flag_type)=DIRECTIVE;
                        }
                        add_string(eror_node,rest_line,&dc,&data);
                        break;
                    case DATA:
                        if(*label != '\0'){/*if there is label*/
                            (new_label->adress) = dc;
                            (new_label->flag_type)=DIRECTIVE;
                        }
                        add_data(eror_node,rest_line,&dc,&data);
                        break;
                    case ENTRY:
                        if(*label != '\0'){
                            printf("\nwarning: in file: %s,line number %d\nin the line:%s",am_name,line_num,line );
                            printf("Declaring a label on .extern or .entry lines is unnecessary.\n\n");
                        }
                        add_ext_en(eror_node,rest_line,&en_c,&enters,&size_en,&enters_line_num);
                        break;
                    case EXTERN:
                    if(*label != '\0'){
                            printf("\nwarning: in file: %s,line number %d\nin the line:%s",am_name,line_num,line );
                            printf("Declaring a label on .extern or .entry lines is unnecessary.\n\n");
                        }
                        add_ext_en(eror_node,rest_line,&ex_c,&exters,&size_ex,&exters_line_num);
                        break;
                    case RTS:
                    case STOP:/*the 0 operands comands*/
                        if(*label != '\0'){/*if there is label*/
                                (new_label->adress) = ic;
                                (new_label->flag_type)=INCTRACTION;
                        }
                        ic++;
                        if(!chek_end_line(rest_line))
                            eror(eror_node,EXTEA_CHARS);
                        break;
                    case CLR:
                    case NOT:
                    case INC:
                    case DEC:
                    case JMP:
                    case BNE:
                    case RED:
                    case PRN:
                    case JSR:/*the 1 operands comands*/
                        if(*label != '\0'){/*if there is label*/
                                (new_label->adress) = ic;
                                (new_label->flag_type)=INCTRACTION;
                        }
                        one_operand(eror_node,command_type,rest_line,&ic,&coms);
                        break; 
                    case NOT_COMMAND:
                        break;
                    default:/*all the two operand commands*/
                        if(*label != '\0'){/*if there is label*/
                                (new_label->adress) = ic;
                                (new_label->flag_type)=INCTRACTION;
                        }
                        two_operands(eror_node,command_type,rest_line,&ic,&coms);
                        break;
                }
            }     
        }
        if ( (ic+dc) > MAX_SIZE_MEMOREY )  /*cheking if we heave to many words*/
        {
            eror(eror_node,OVER_SIZE);
             /* Prevent deviation from the size of the arrays */
            ic=0;
            dc=0;
        } 
    }/*close the while*/
    check_entey(eror_node,&enters,&enters_line_num,en_c,&label_head); /* Check validity of entry directives */
    check_extern(eror_node,&exters,&exters_line_num,ex_c,&label_head); /* Check validity of extern directives */
    free(exters_line_num);
    free(enters_line_num);
    fclose(am_file);
    second_pass(eror_node,&coms,ic,&data,dc,&exters,ex_c,&label_head,en_c);/*send to second pass*/
    free_the_labels(label_head);
    free(exters);
    free(enters);
}
