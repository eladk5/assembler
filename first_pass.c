#include "all.h"
#include "help_funcs.c"

#define PROMPTE 1
#define INCTRACTION 2

struct Label{
    char name[MAX_LABEL_LENGTH+1];/*1 for the null*/
    int flag_type; /*promote or inctraction*/
    int adress;
    struct Label *next;
};
typedef struct Label *label_node;

/*Processes the argument part of .string promote. 
It ensures the string is properly enclosed in double quotes and checks for extra characters after the closing quote.
The function iterates through the line, identifying the start and end of the string, 
and storing each character within the quotes into the data array. 
If the string is not properly formatted, it sets an error flag and returns.

Parameters:

am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
rest_line: The portion of the line containing the string to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the string will be stored.
*/
static void add_string(char *am_name,int line_num,int *eror_flag,char *line,char *rest_line,int *dc,short *(data[]))
{
    char flag = F;/*flag to mark if we between ""*/
    while(*rest_line){ /* Loop until end of the string */
        if(flag){
            if(*rest_line != '\"')
                (*data)[(*dc)++] = *rest_line;/* Add character to data array and increment dc */
            else{
                if(!chek_end_line(++rest_line))/*check if the end of the line is empty*/
                    eror(am_name,line_num,eror_flag,line,EXTEA_CHARS);
                return;/* End of string processing */
            }
        }
        else{/* We haven't reached the opening quote yet */
            if (*rest_line == '\"' )
                flag = T;
            else{
                    if (!isspace(*rest_line)){/*cheks that there are only blank char before quote */
                        eror(am_name,line_num,eror_flag,line,FAIL_STRING);
                        return;
                    }
            }
        }
        rest_line++; 
    }
}

/*Assumption: a label that comes here is not NULL and ends in ':'*/
static label_node valid_label(char *am_name,int line_num,int *eror_flag,char *line,char *label,head *head_mac,head *head_label,FILE *am_file)
{
    label_node temp, new_label = NULL;/*for temporary and new labels nose*/
    int i, len= strlen(label);
    label[--len]= '\0'; /*removing the ':' and reducing len by 1*/
    if( len > MAX_LABEL_LENGTH ){/*cheks if the length of the label is over 31*/
        eror(am_name,line_num,eror_flag,line,LABEL_LENGTH);
        return new_label;
    }
    if( (len == 0) || !isalpha(*label))/*cheks if the first char is alphabetic*/
    {
        eror(am_name,line_num,eror_flag,line,NOT_ALPHA);
        return new_label;
    }
    for ( i = 1; i < len; i++) {/*cheks if all the others chars are latters or number*/
        if (!isalnum(str[i])){
            eror(am_name,line_num,eror_flag,line,NOT_ALPHA_NUM);
            return new_label;
        }   
    }
    if(search_command(label) != NOT_COMMAND)/*cheks if it is a command name*/
    {
        eror(am_name,line_num,eror_flag,line,LABEL_IS_COMMAND);
        return new_label;
    }
    if (is_macro_name(label,head_mac))/*cheks if it is a macro name*/
    {
        eror(am_name,line_num,eror_flag,line,LABEL_IS_MACRO);
        return new_label;
    }
    new_label = (label_node)malloc(sizeof(struct Label));
    if (new_label==NULL)/*memorey allocation fail*/
    {
        fprintf(stderr,"Failed to allocate memory/n");
        fclose(am_file);
        free_the_label(head_label);
        free_the_mac(head_mac);
        exit(1);
        }
    strcpy(new_label->name,label);
    new_label->next = NULL;
    if ((head_node->head_of_list) == NULL )/*if its the first label*/
    {
        (head_node->head_of_list) = new_label;
        return new_label;
    }
    temp = (label_node)(head_node->head_of_list);
    while(temp){/*Going through the list of labels, if the new label name is already found, an error is reported.
     If not, adds the new node to the end of the list*/
        if( strcmp( (temp->name) , label ) == 0 ){  
            eror(am_name,line_num,eror_flag,line,LABEL_NAME_AGAIN);
            free(new_label);
            return  NULL;
        }
        if (temp->next == NULL){
            temp->next = new_label ;
            return new_label;
        }
        temp = (temp -> next);
    }
    return NULL;
}

void first_pass(char *am_name,head *head_node_mac,FILE *am_file)
{
    head label_head;
    command coms[MAX_SIZE_MEMOREY];/*whill contain all the regular commands words*/
	short data[MAX_SIZE_MEMOREY];/*whill contain all the data or string words*/
	int eror_flag=T;
    char line[MAX_LINE_LENGTH],rest_line[MAX_LINE_LENGTH];/* To store new line evry time */
    int ic=0,dc=0;/*to count the num of instraction and data words */
    int line_num =0;/*lines counter*/
    char label[MAX_LABEL_LENGTH+2];/*to contain the labels for each line +2 for the ':' and '\0'*/
    int temp;
    int command_type; /*will contain the opcode of the command or the enum value if its promote  */
    char first_word[MAX_LABEL_LENGTH]; /*to read the first word after the label*/
    label_node new_label;
    
    label_head.head_of_list =NULL;
    while (fgets(line, MAX_LINE_LENGTH ,am_file) ) /*reading new line */
    {
        new_label=NULL
        *label = '\0';/*reset the label*/
        *first_word = '\0';/*reset the string*/
        line_num++;
        if (line[strlen(line) - 1] != '\n' && !feof(am_file))/*cheks if the size of the line is to big*/
            eror(am_name,line_num,&eror_flag,line,LINE_LENGTH); 
        if ( (ic+dc) > MAX_SIZE_MEMOREY )  /*cheking if we heave to many words*/
        {
            eror(am_name,line_num,&eror_flag,line,OVER_SIZE);
            /*And so that there is no deviation from the size of the arrays*/
            ic=0;
            dc=0;
        }
        if( ! ( *line == ';' || chek_end_line(line) ) )/*cheks if it is not blank or note line*/
        {
            sscanf(line,"s",label); 
            temp = strlen(label);
            if( temp > 0 && label[temp-1] != ':' )/*if the first word of the line isnt a lable*/
                {
                    *label = '\0';/*reset the label*/
                    rest_line = line;
                }
            else
                rest_line = strstr(line,label)+temp;/*now rest line is the part of the line after the label*/
            if (  sscanf(rest_line,"s",first_word)==0 || (temp = strlen(first_word))==0 )  
            {
             /*If we got here, it means that the line is not empty, but rest_line is empty, which means that only a label appeared*/
                eror(am_name,line_num,&eror_flag,line, ONLY_LABEL );
            }
            command_type = search_command(first_word);
            if (command_type == NOT_COMMAND)
                eror(am_name,line_num,&eror_flag,line, NOT_VALID_COMMAND);
            if(*label != '\0')
                new_label = valid_label(am_name,line_num,&eror_flag,line,label,head_node_mac,label_head,am_file);
            if(( (*label== '\0') || new_label ) && eror_flag )
            {
                
                temp = strlen(first_word);
                rest_line = strstr(rest_line,first_word)+temp;/*now rest_line is the part of the arguments*/
                switch (command_type)
                {
                    case STRING:
                        if(*label != '\0'){/*if there is label*/
                        (new_label->adress) = dc;
                        (new_label->flag_type)=PROMPTE;
                        }
                        add_string(am_name,line_num,&eror_flag,line,rest_line,&dc,&data);
                        break:
                    case DATA:
                        break:
                    

                
                }
            }
            

            
            


              
        }
        
        
    }

}