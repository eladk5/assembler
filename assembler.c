#include "all.h"

/* 
Processes the input assembly file and expands the macros. Writes the expanded code to a new file with a .am extension.
 Handles errors and sets flags accordingly.
Parameters:
as_file: The input assembly file.
file_name: The name of the input file.
Returns: T if no errors were found during the preassembler, otherwise F.
*/
int pre_pros(FILE *, char *);

first_pass(char *file_name,command *(coms[]), sort *(data[]),int *eror_flag);

int main(int argc,char *argv[])
{
	head head_node_macro;/* Head of the macro linked list */
    char *file_name;
    char *new_file_name;/* To store the name of the current file being processed */
	FILE *ifp;/* File pointer for reading the input files */
	command coms[MAX_SIZE_MEMOREY];/*whill contain all the regular commands words*/
	short data[MAX_SIZE_MEMOREY];/*whill contain all the data or string words*/
	int eror_flag;

	/* Check if there are any input files */
	if( argc == 1){
		fprintf(stderr, "there are no arguments\n");
		return 1;
	}
    /* Process each input file */
	while (--argc > 0){
        file_name = *++argv;
		new_file_name = malloc(strlen(file_name)+MAX_SIZE_ENDINDG);
		if (new_file_name == NULL) {
			fprintf(stderr,"Failed to allocate memory");
			return 1;
		}
        sprintf(new_file_name, "%s.as", file_name);
		if ((ifp=fopen(new_file_name,"r")) == NULL){
 			fprintf(stderr, "Error opening file: %s\n",new_file_name);/* the file dont open */
		} else {
            eror_flag = pre_pros(ifp, new_file_name,&head_node_macro);
            fclose(ifp);
        }
		if(eror_flag)
			first_pass(new_file_name,&coms,&data,&eror_flag);
		   
	}
    return 0;
}