#include "all.h"
#define PRINT
#include "passes.h"

/*
The function generates the object file from the commands and data sections with ob extension.
The object code is printed to file in octal format along with its memory address.

Parameters:
file_name: The name of the origin file.
coms: The array of commands.
ic: The instruction counter.
data: The array of data.
dc: The data counter.
Returns:T if the operation is successful, F otherwise.
*/

int ob_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY] ,int dc )
{
    FILE *ob_file;/* File pointer for the object file */
    int memorey = FIRST_MEM;/* Memory address counter, starting from the first available memory location */
    int i;
    short temp;/* Temporary variable for storing the current instruction or data word */
    change_extension(file_name,"ob"); /* Change the file extension to .ob */
    if ((ob_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* the file dont open */
        return F;
		}
    fprintf(ob_file, "  %d %d\n",ic,dc);/* Print the instruction and data counters */
    /* Print the instruction words */
    for(i=0; i<ic ; i++ ,memorey++){
        temp =0;
        switch ((*coms)[i].type_of_instraction)
        {/*Updates temp according to the word type and the bits that need to be changed*/
        case FIRST:
            temp |=  (*coms)[i].ins.first_command.a << TWO_BITS;
            temp |=  (*coms)[i].ins.first_command.target_method << THREE_BITS;
            temp |=  (*coms)[i].ins.first_command.source_method << SEVEN_BITS;
            temp |=  (*coms)[i].ins.first_command.opcode << ELEVEN_BITS;
            break;
        case NUMBER:
            temp |=  (*coms)[i].ins.number_word.a << TWO_BITS;
            temp |=  (*coms)[i].ins.number_word.value << THREE_BITS;
            break;
        case LABEL:
            temp |=  (*coms)[i].ins.label_word.e;
            temp |=  (*coms)[i].ins.label_word.r << ONE_BIT;
            temp |=  (*coms)[i].ins.label_word.adress << THREE_BITS;
            break;
        case P_REGISTER:
        case D_REGISTER:
            temp |=  (*coms)[i].ins.register_word.a << TWO_BITS;
            temp |=  (*coms)[i].ins.register_word.target_operand << THREE_BITS;
            temp |=  (*coms)[i].ins.register_word.source_operand << SIX_BITS;
            break;
        default:
            break;
        }
        fprintf(ob_file,"%04d %05o\n",memorey, (unsigned short)temp & FULL_BIT_INSTRACTION);
    }
     /* Print the data words */
    for(i=0; i < dc ; i++ ,memorey++)
    {
        fprintf(ob_file,"%04d %05o\n",memorey,(unsigned short)(*data)[i] & FULL_BIT_INSTRACTION);
    }
    fclose(ob_file);
    return T;
}
/*
The function writes the entry labels and their addresses to a file with ent extension.
It iterates over the list of labels and writes the ones marked as entries to the file.
Parameters:
file_name: The name of the origin file.
labels: The head of the label list.
Returns:T if the operation is successful, F otherwise.
Assumption: The function is activated only if there are labels defined as entrys
*/
int ent_print(char *file_name, head labels )
{
    label_node temp;/* Temporary variable for label node */
    FILE *ent_file;/* File pointer for the entry file */
    change_extension(file_name,"ent");/* Change the file extension to .ent */
    if ((ent_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* the file dont open */
        return F;
		}
    temp =labels.head_of_list;
    while(temp)
    {
        if (temp->is_entry)
            fprintf(ent_file,"%s\t%04d\n",temp->name, temp->adress);/* Print the entry label and its address */
        temp = (temp->next);  
    }
    fclose(ent_file);
    return T;
}
/*
The function iterates over the list of commands and writes the external labels and the addresses of lines thats 
use them, to file whith ext extensions.
Parameters:
file_name: The name of the origin file.
coms: The array of commands.
ic: The instruction counter.
Returns:T if the operation is successful, F otherwise.
Assumption: The function is activated only if there are labels defined as extern
*/
int ext_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic)
{
    FILE *ext_file;/* File pointer for the external file */
    int i;
    change_extension(file_name,"ext");/* Change the file extension to .ext */
    if ((ext_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* The file didn't open */
        return F;
		}
    for(i=0; i<ic ; i++ )
    { /* Iterate through the commands and print external labels */
        if ((*coms)[i].type_of_instraction == LABEL )
        {
            if((*coms)[i].ins.label_word.e)/*its an external label*/
                fprintf(ext_file,"%s\t%04d\n",(*coms)[i].label_name,i+FIRST_MEM);
        }
    }
    fclose(ext_file);
    return T;
}