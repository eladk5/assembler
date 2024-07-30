#include "all.h"

/* 
Processes the input assembly file and expands the macros. Writes the expanded code to a new file with a .am extension.
 Handles errors and sets flags accordingly.if no errors were found during the preassembler send to the first pass of the assemly.
Parameters:
as_file: The input assembly file.
file_name: The name of the input file.
*/
void pre_pros(FILE *, char *);


int main(int argc,char *argv[])
{
	
    char *file_name;
    char *new_file_name;/* To store the name of the current file being processed */
	FILE *ifp;/* File pointer for reading the input files */

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
		if ((ifp=fopen(new_file_name,"r")) == NULL)
 			fprintf(stderr, "Error opening file: %s\n",new_file_name);/* the file dont open */
		else{ 
            pre_pros(ifp, new_file_name);
			fclose(ifp);
		}
		free(new_file_name);
	}
    return 0;
}