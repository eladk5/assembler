#include "all.h"

int pre_pros(FILE *, char *);

int main(int argc,char *argv[])
{
    char *file_name;
    char new_file_name[MAX_FILE_NAME];/* To store the name of the current file being processed */
	FILE *ifp;/* File pointer for reading the input files */
	/* Check if there are any input files */
	if( argc == 1){
		fprintf(stderr, "there are no arguments\n");
		return 1;
	}
    /* Process each input file */
	while (--argc > 0){
        file_name = *++argv;
        sprintf(new_file_name, "%s.as", file_name);
		if ((ifp=fopen(new_file_name,"r")) == NULL){
 			fprintf(stderr, "Error opening file: %s\n",new_file_name);/* the file dont open */
		} else {
           /* pre_pros(ifp, new_file_name);*/
            fclose(ifp);
        } 
		   
	}
    return 0;
}