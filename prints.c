#include "all.h"
#include "passes.h"


int ob_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY] ,int dc )
{
    FILE *ob_file;
    int memorey = FIRST_MEM;
    int i;
    short temp;
    change_extension(file_name,"ob");
    if ((ob_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* the file dont open */
        return F;
		}
    fprintf(ob_file, "\t%d %d\n",ic,dc);
    for(i=0; i<ic ; i++ ,memorey++){
        temp =0;
        switch ((*coms)[i].type_of_instraction)
        {
        case FIRST:
            printf("first command in line:%d a:%d source: %d",(*coms)[i].line_number,(*coms)[i].ins.first_command.a,(*coms)[i].ins.first_command.source_method);
            printf("target:%d opc:%d\n",(*coms)[i].ins.first_command.target_method,(*coms)[i].ins.first_command.opcode);
            temp |=  (*coms)[i].ins.first_command.a << 2;
            temp |=  (*coms)[i].ins.first_command.target_method << 3;
            temp |=  (*coms)[i].ins.first_command.source_method << 7;
            temp |=  (*coms)[i].ins.first_command.opcode << 11;
            break;
        case NUMBER:
            printf("num command in line:%d a:%d value: %d\n",(*coms)[i].line_number,(*coms)[i].ins.number_word.a,(*coms)[i].ins.number_word.value);
            temp |=  (*coms)[i].ins.number_word.a << 2;
            temp |=  (*coms)[i].ins.number_word.value << 3;
            break;
        case LABEL:
            printf("label in line:%d e:%d r: %d adress: %d\n",(*coms)[i].line_number,(*coms)[i].ins.label_word.e,(*coms)[i].ins.label_word.r,(*coms)[i].ins.label_word.adress);
            temp |=  (*coms)[i].ins.label_word.e;
            temp |=  (*coms)[i].ins.label_word.r << 1;
            temp |=  (*coms)[i].ins.label_word.adress << 3;
            break;
        case P_REGISTER:
        case D_REGISTER:
            printf("register in line:%d a:%d source:%d ",(*coms)[i].line_number,(*coms)[i].ins.register_word.a,(*coms)[i].ins.register_word.source_operand);
            printf("target:%d\n",(*coms)[i].ins.register_word.target_operand);
            temp |=  (*coms)[i].ins.register_word.a << 2;
            temp |=  (*coms)[i].ins.register_word.target_operand << 3;
            temp |=  (*coms)[i].ins.register_word.source_operand << 7;
            break;
        default:
            break;
        }
        fprintf(ob_file,"%05d %05o\n",memorey, (unsigned short)temp & 0x7FFF);
    }
    for(i=0; i < dc ; i++ ,memorey++)
    {
        printf("data adress:%d data:%d \n",memorey, (*data)[i]);
        fprintf(ob_file,"%05d %05o\n",memorey,(unsigned short)(*data)[i] & 0x7FFF);
    }
    return T;
    fclose(ob_file);
}

int ent_print(char *file_name, head labels )
{
    label_node temp;
    FILE *ent_file;
    if (labels.head_of_list == NULL)
    {
        return T;/*there is no labels*/
    }
    change_extension(file_name,"ent");
    if ((ent_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* the file dont open */
        return F;
		}
    temp =labels.head_of_list;
    while(temp)
    {
        if (temp->is_entry)
            fprintf(ent_file,"%s %d\n",temp->name, temp->adress);
        temp = (temp->next);  
    }
    fclose(ent_file);
    return T;
}

int ext_print(char *file_name, command (*coms)[MAX_SIZE_MEMOREY] , int ic)
{
    FILE *ext_file;
    int i;
    change_extension(file_name,"ext");
    if ((ext_file=fopen(file_name,"w")) == NULL){
 		fprintf(stderr, "Error opening file: %s\n",file_name);/* the file dont open */
        return F;
		}
    for(i=0; i<ic ; i++ )
    {
        if ((*coms)[i].type_of_instraction == LABEL )
        {
            if((*coms)[i].ins.label_word.e)/*its an external label*/
                fprintf(ext_file,"%s %d\n",(*coms)[i].label_name,i+FIRST_MEM);
        }
    }
    fclose(ext_file);
    return T;
}