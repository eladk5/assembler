#include <all.h>
#include "erors.c"


static int check_if_extern(char (**exters)[MAX_LINE_LENGTH] ,int ex_c, char *name)
{
    int i;
    for (i=0; i<ex_c; i++)
    {
        if(strcmp(name,(*exters)[i]) == 0) 
            return T;
    }
    return F;
}

void second_pass(erors_node erors_node,command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY], int dc  ,char (**exters)[],int ex_c,head *labels)
{
    int i;
    label_node temp;\
    /*Updates all the words of the labels received as arguments, 
    and checks whether these labels were declared as a label or .extern*/
    for(i=0; i < ic ; i++){
        if ( (*coms)[i].type_of_instraction == LABEL ){
            temp = is_label_name((*coms)[i].label_name,*labels);
            if(temp){
                (*coms)[i].ins.label_word.r =T;
                if( (temp->flag_type) == PROMPTE)
                    (*coms)[i].ins.label_word.adress = (temp->adress) + ic;
                    /*The addition of the ic because the promotes are after the commands in the data image*/
                else
                     (*coms)[i].ins.label_word.adress = (temp->adress);
            }
            else {/*this label isn't define in this file*/
                if (check_if_extern(exters,ex_c,temp)){
                    (*coms)[i].ins.label_word.e=T;
                    (*coms)[i].ins.label_word.adress = F;
                }
                else{
                    eror_label(erors_node.file_name,erors_node.flag,(*coms)[i].line_number,FAIL_LABEL);
                } 

            }  
        }
        
    }
}