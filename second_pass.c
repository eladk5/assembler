#include "all.h"
#define SECOND_PASS
#include "passes.h"
/*
This function updates the addresses of labels based on their type.
For labels corresponding to directive lines, it adds the instruction counter (ic) and the first available memory location (FIRST_MEM).
For other labels, it adds only the first available memory location (FIRST_MEM).
Parameters:
label_head - The head of the label list.
ic - The instruction counter.
*/
static void change_adress(head label_head,int ic)
{   
    label_node temp;/* Temporary variable for label node */
    temp = label_head.head_of_list;
    while(temp){
        if((temp->flag_type) == DIRECTIVE)
            temp->adress += (FIRST_MEM+ic);/* Update address for directive labels */
        else if ( (temp->flag_type) == INCTRACTION)
            temp->adress += FIRST_MEM;/* Update address for instraction labels */
        temp = (temp->next);
    }
}
/*
The function checks if a label is declared as extern.
It iterates over the list of externs and compares each with the given label name.

Parameters:
exters: The array of extern labels.
ex_c: The count of extern labels.
name: The name of the label to check.

Returns:T if the label is found in the extern list, F otherwise.
*/
static int check_if_extern(char (**exters)[MAX_LINE_LENGTH] ,int ex_c, char *name)
{
    int i;
    for (i=0; i<ex_c; i++)
    {
        if(strcmp(name,(*exters)[i]) == 0){
            return T;
        } 
    }
    return F;
}

/*
The function performs the second pass of the assembly process.
It updates all label references in the command list and checks for undefined labels.
It then prints if needed the object code, entry labels, and extern labels to their respective files.

Parameters:
erors_node - The error handling structure.
coms - The array of commands.
ic - The instruction counter.
data - The array of data.
dc - The data counter.
exters - The array of extern labels.
ex_c - The count of extern labels.
labels - The head of the label list.
en_c - The count of entry labels.
*/
void second_pass(erors_node erors_node,command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY], int dc  ,char (**exters)[],int ex_c,head *labels,int en_c)
{
    char *new_file_name;/*for the name of the file*/
    int i;
    int len = strlen(erors_node.file_name);/*for the length of name of the file*/
    int exterens_flag=F;/*will contain T if use of the extern label is detected in the file*/
    label_node temp;
    change_adress(*labels,ic);/*update the addresses of the labels*/
    /* Update label addresses in commands and check if they are defined or extern */
    for(i=0; i < ic ; i++){
        if ( (*coms)[i].type_of_instraction == LABEL ){
            temp = is_label_name((*coms)[i].label_name,*labels);
            if(temp){
                (*coms)[i].ins.label_word.r =T;
                (*coms)[i].ins.label_word.adress = (temp->adress);
            }
            else {/*this label isn't define in this file*/
                if (check_if_extern(exters,ex_c,(*coms)[i].label_name)){
                    (*coms)[i].ins.label_word.e=T;
                    exterens_flag= T;
                }
                else
                    eror_label(erors_node.file_name,erors_node.flag,(*coms)[i].line_number,FAIL_LABEL);                 
            }  
        }
    }/*close the for*/
    if(*erors_node.flag)/*start printing*/
    {
        new_file_name = malloc(len+MAX_SIZE_ENDINDG);
		if (new_file_name == NULL) {
			fprintf(stderr,"\nFailed to allocate memory");/*memorey allocation fails*/
			exit(1);
		}
        strcpy(new_file_name, erors_node.file_name);
        if ( (*erors_node.flag = ob_print(new_file_name,coms,ic,data,dc)) ){
            if(en_c)/*if thre is entry's*/
                if (!(*erors_node.flag = ent_print(new_file_name,*labels)) ){
                    /*If creating ob succeeded and ent did not */
                    change_extension(new_file_name,"ob");
                    remove(new_file_name);
                }
            if ( *erors_node.flag && exterens_flag ){
                if (! (*erors_node.flag = ext_print(new_file_name,coms,ic) ) ){
                    /*If creating ext failed*/
                    change_extension(new_file_name,"ob");
                    remove(new_file_name);
                    if(en_c){
                        /*If an ent file is created*/
                        change_extension(new_file_name,"ent");
                        remove(new_file_name);  
                    }
                }
            }
        }
        free(new_file_name);
    }
}