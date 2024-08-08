 
   /* This function processes a line of assembly code with a single operand, reads the operand, verifies its method,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operand.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
void one_operand(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] );

/*  This function processes a line of assembly code with two operands, reads the operands, verifies their methods,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operands.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.*/
void two_operands(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] );

/*This function processes a line of entry or extern. The function adds the label name to the appropriate array, 
and allocates more memory to the array if necessary. 
In case of eror, an error is reported and marked accordingly.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the name to be processed.
counter: Pointer to an integer that keeps track of the number of names added to the array.
names_arrey: Pointer to the dynamic array of strings where the names of the extern or entry label will be stored.
size: Pointer to an integer representing the current size of the dynamic array.*/
void add_ext_en(erors_node eror_node,char *rest_line,int *counter ,char (**names_arrey)[MAX_LINE_LENGTH],int *size,int **name_line_num);

/*This function processes a line of assembly code to add numerical data to the data array,
In case of eror, an error is reported and marked accordingly.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the data to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the numerical data will be stored.*/
void add_data(erors_node eror_node,char *rest,int *dc,short (*data)[]);

/*Processes the argument part of ".string" . The function checks that all the characters are 
between the first and the last quotes and enters them into data.
Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the string to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the string will be stored.
*/
void add_string(erors_node eror_node, char *rest_line,int *dc,short (*data)[]);

/* 
Frees the memory allocated for the labels list.
Parameters:
head_node: The head of the labels linked list.
*/
void free_the_labels(head head_node);

/*
The function performs the second pass of the assembly process.
It updates all label references in the command list and checks for undefined labels.
It's prints if needed the object code, entry labels, and extern labels to their respective files.

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
void second_pass(erors_node erors_node,command (*coms)[MAX_SIZE_MEMOREY] , int ic, short (*data)[MAX_SIZE_MEMOREY], int dc  ,char (**exters)[],int ex_c,head *labels,int en_c);

