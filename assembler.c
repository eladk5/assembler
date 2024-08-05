#include "assembler.h"

/*the work of Elad Karo

This program converts assembly code files, received from the user as explained in the course booklet.
The program expands the macros, in the pre_proses.c file, If no errors are detected at this stage, 
proceed to an initial analysis of the code that is carried out in the file first_pass.
After that we will update the label addresses and we can check if all the labels we received as arguments
in the various commands and directives fit with the declarations of the external and internal labels in the file.
If errors were detected in the passages they will be printed in detail. If everything is correct in the print.c file, 
we will create the object files and if necessary also the externals and entrys file.
Assumption: the macro names will not be more than 31 characters long, and the first character in them will be a letter, 
as stated in the forum on the course website.
*/

/*Goes through the files received on the command line, and sends them to the step of the expends the macros, 
if the opening of the file is not successful, an error is printed, and it go to the next file.*/
int main(int argc,char *argv[])
{
    char *temp;/*temporary string*/
    char *new_file_name;/* To store the name of the current file being processed */
	FILE *ifp;/* File pointer for reading the input files */

	/* Check if there are any input files */
	if( argc == 1){
		fprintf(stderr, "there are no arguments\n");
		return 1;
	}
    /* Process each input file */
	while (--argc > 0){
        temp = *++argv;
		new_file_name = malloc(strlen(temp)+MAX_SIZE_ENDINDG);
		if (new_file_name == NULL) {
			fprintf(stderr,"Failed to allocate memory\n");/*memorey alocatin fails*/
			return 1;
		}
        sprintf(new_file_name, "%s.as", temp);
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