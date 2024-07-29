#include "all.h"
#include "passes.h"

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

void second_pass(erors_node erors_node,command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY], int dc  ,char (**exters)[],int ex_c,head *labels,int en_c)
{
    char *new_file_name;
    int i;
    int len = strlen(erors_node.file_name);
    int exterens_flag=F;/*will contain T if use of the extern label is detected in the file*/
    label_node temp;\
    /*Updates all the words of the labels received as arguments, 
    and checks whether these labels were declared as a label or .extern*/
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
    }/*end of for*/
    if(*erors_node.flag)/*start printing*/
    {
        new_file_name = malloc(len+MAX_SIZE_ENDINDG);
		if (new_file_name == NULL) {
			fprintf(stderr,"\nFailed to allocate memory");
			exit(1);
		}
        sprintf(new_file_name, "%s", erors_node.file_name);
        new_file_name[len-2]= '\0';/*remove the am ending*/
        if ( (*erors_node.flag = ob_print(new_file_name,coms,ic,data,dc)) ){
            if(en_c)
                if (!(*erors_node.flag = ent_print(new_file_name,*labels)) ){
                    change_extension(new_file_name,"ob");
                    remove(new_file_name);
                }
            if ( *erors_node.flag && exterens_flag ){
                if (! (*erors_node.flag = ext_print(new_file_name,coms,ic) ) ){
                    change_extension(new_file_name,"ob");
                    remove(new_file_name);
                    if(en_c){
                        change_extension(new_file_name,"ent");
                        remove(new_file_name);  
                    }
                }
            }
        }
    }
}