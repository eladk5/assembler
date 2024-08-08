#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#define MAX_SIZE_ENDINDG 5 /*The largest size a file extension can be*/

/* 
Processes the input assembly file and expands the macros. Writes the expanded code to a new file with a .am extension.
 Handles errors and sets flags accordingly.if no errors were found during the preassembler send to the first pass of the assemly.
Parameters:
as_file: The input assembly file.
file_name: The name of the input file.
*/
void pre_pros(FILE *, char *);
