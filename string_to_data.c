#include "all.h"
#define NOT_FIRST
#define STRING_TO_DATA
#include "passes.h"

/*  This function processes a single operand from a line of assembly code, determining its type and storing
    it in the provided instruction structure. It handles numeric literals, direct registers, and pointer registers.
    If the operand is a number, it checks if it fits within 12 bits and sets the appropriate fields in the instruction.
    If it is a register, it determines if it's a direct or pointer register and sets the corresponding fields.
    It should be emphasized that this function does not handle the case of a label,
    and in fact everything it does not know how to identify it defines as a label. Labeling will be handled later.
    
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    first_word: The operand to be read.
    operand: A pointer to the instruction structure to store the operand.
    which_operand: Indicates if the operand is a source or destination operand.
    Returns: The type of the operand read (NUMBER, P_REGISTER, D_REGISTER, or LABEL).
*/
static int read_operand(erors_node eror_node, char *first_word,instruction *operand,int which_operand)
{
    char *temp = first_word; /* Temporary pointer to traverse the operand string */
    int type = LABEL;   /* Initialize the default type to LABEL */
    int num;/*if number method to store the number that recived*/
    if (*temp == '#'){ /* Check if the operand is a number */
        type = NUMBER;
        temp++;
        num =get_num(eror_node,temp);
        if(IS_WITHIN_12_BITS(num)){
            (operand->number_word).a=T;
            (operand->number_word).value = num;
            return type;
        }
        else{/*The number cannot be represented in 12 bits*/
            eror(eror_node,NUM_REP);
        }
    }
    if(*temp == '*'){/* Check if the operand is a pointer register */
        type = P_REGISTER;
        temp++;
    }
    if(*temp == 'r'){ /* Check if the operand is a register */
        if( type != P_REGISTER )
            type = D_REGISTER;/*if it isn't pointer to register set as direct one*/
        temp++;
        if ( (strlen(temp)== ONE_CHAR) && ( ('0' <=(*temp)) &&( (*temp) <= 'g') ) ){ /* after 'r' apeers one char that it register number*/
            (operand->number_word).a=T;
            if(which_operand == TARGET)
                (operand->register_word).target_operand = CHAR_TO_NAM(*temp);
            else 
                (operand->register_word).source_operand = CHAR_TO_NAM(*temp);
            return type;
        }
    } 
    /*if we got here we stay only whith label option*/ 
    return LABEL;
}
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
void one_operand(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] )
{
    int method;/* Variable to store the addressing method of the operand */
    instruction operand= {0};/* Instruction structure to store the operand */
    char first_word[MAX_LINE_LENGTH]= {0};/* Array to store the operand string */
    if (!sscanf(rest_line,"%s",first_word) || chek_end_line(first_word))/* Check if there's a operand */
    {
        eror(eror_node,NO_PARAMS);
        return;
    }
    method = read_operand(eror_node,first_word,&operand,TARGET);/* Read the operand and determine its method */
    if(!cmd[command_type].method_allowed[TARGET][method]){
        eror(eror_node,NO_VALID_METHOD);/*case of operand that does not match the command type*/
        return;
    }
    (*coms)[(*ic)].ins.first_command.target_method |= METHOD(method); /* Update the first word of the command with the operand method */
    if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the second word*/
            eror(eror_node,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
    (*coms)[(*ic)].type_of_instraction=method; /*update the next word*/
    (*coms)[(*ic)].line_number = *eror_node.line_num;
    if(method != LABEL){
        (*coms)[(*ic)].ins = operand;
    }else
        strcpy( (*coms)[(*ic)].label_name,first_word);
    rest_line = strstr(rest_line,first_word)+strlen(first_word);/*Promotes rest_line*/
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(eror_node,EXTEA_CHARS);
    (*ic)++;/*advances to the next word for the next time*/
}
/*  This function processes a line of assembly code with two operands, reads the operands, verifies their methods,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operands.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
void two_operands(erors_node eror_node, int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] )
{
    int method_1,method_2;/* Variables to store the addressing methods of the operands */
    instruction operand_1={0},operand_2= {0};/* Instruction structures to store the operands */
    char first_word[MAX_LINE_LENGTH]={0},second_word[MAX_LINE_LENGTH]={0};/* Arrays to store the operand strings */
    if (!sscanf(rest_line," %[^, ] ,%s",first_word,second_word) || chek_end_line(first_word) || chek_end_line(second_word))
    { 
        eror(eror_node,NO_PARAMS);/*If not enough arguments were received*/
        return;
    }
    method_1 = read_operand(eror_node,first_word,&operand_1,SOURCE);
    method_2 = read_operand(eror_node,second_word,&operand_2,TARGET);
    if( !cmd[command_type].method_allowed[SOURCE][method_1] || !cmd[command_type].method_allowed[TARGET][method_2])
    /*case of operand that does not match the command type*/
    {
        eror(eror_node,NO_VALID_METHOD);
        return;
    }
    /* Update the first word of the command with the operand method */
    (*coms)[(*ic)].ins.first_command.source_method |= METHOD(method_1);
    (*coms)[(*ic)].ins.first_command.target_method |= METHOD(method_2);
    if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the second word*/
            eror(eror_node,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
    }
    (*coms)[(*ic)].type_of_instraction=method_1;
    (*coms)[(*ic)].line_number = *eror_node.line_num;
    if( ( (method_1 == D_REGISTER) || (method_1 == P_REGISTER) ) && ( (method_2 == D_REGISTER) || (method_2 == P_REGISTER) ) ){
    /*if both of the operands are registers*/
        (*coms)[(*ic)].ins.register_word.a = T;
        (*coms)[(*ic)].ins.register_word.source_operand = operand_1.register_word.source_operand ;
        (*coms)[(*ic)].ins.register_word.target_operand = operand_2.register_word.target_operand;
    }else{ 
        if(method_1 != LABEL)
            (*coms)[(*ic)].ins = operand_1;
        else
            strcpy( (*coms)[(*ic)].label_name,first_word);
        if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the third word*/
            eror(eror_node,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
        (*coms)[(*ic)].type_of_instraction=method_2;
        (*coms)[(*ic)].line_number = *eror_node.line_num;
        if(method_2 != LABEL)
            (*coms)[(*ic)].ins = operand_2;
        else
            strcpy( (*coms)[(*ic)].label_name,second_word);
    }
    (*ic)++;/*advances to the next word for the next time*/
    /*Promotes rest_line*/
    rest_line = strstr(rest_line,first_word)+strlen(first_word);
    rest_line = strstr(rest_line,second_word)+strlen(second_word);
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(eror_node,EXTEA_CHARS);
}

/*This function processes a line of entry or extern. The function adds the label name to the appropriate array, 
and allocates more memory to the array if necessary. 
In case of a lack of a parameter or unnecessary characters, an error is reported and marked accordingly.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the name to be processed.
counter: Pointer to an integer that keeps track of the number of names added to the array.
names_arrey: Pointer to the dynamic array of strings where the names will be stored.
size: Pointer to an integer representing the current size of the dynamic array.*/
void add_ext_en(erors_node eror_node,char *rest_line,int *counter ,char (**names_arrey)[MAX_LINE_LENGTH],int *size,int **name_line_num)
{
    int new_size = *size * DOUBLE; /*if reallocate is needed*/
    char name[MAX_LINE_LENGTH];/*the name of the label*/
    if ( !sscanf(rest_line,"%s",name) || chek_end_line(rest_line))
    {/*if the line miss arguments*/
        eror(eror_node,NO_PARAMS);
        return;
    }
    strcpy((*names_arrey)[*counter], name);
    (*name_line_num)[*counter] = *eror_node.line_num;
    rest_line = strstr(rest_line,name)+strlen(name);
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(eror_node,EXTEA_CHARS);
    (*counter)++;
    if((*counter) >= (*size)-NULL_SIZE){/*if realloc is nedded for the next time*/
        *names_arrey=realloc(*names_arrey, new_size * MAX_LINE_LENGTH * sizeof(char));
        *name_line_num = realloc(*name_line_num , new_size  * sizeof(int));
        if (  *names_arrey == NULL || *name_line_num == NULL ) {
            printf("\nFailed to allocate memory\n");/*memorey allocation fails*/
            exit(1);
        }
        *size = new_size;
    }
}

/*This function processes a line of assembly code to add numerical data to the data array,
ensuring the input format is correct. It first checks if the commas in the input string are correctly placed,
then tokenizes the string based on commas, converts each token to a number using get_num, 
and adds each number to the data array. If an error is detected, it sets an error flag and returns.

Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest: The portion of the line containing the data to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the numerical data will be stored.*/
void add_data(erors_node eror_node,char *rest,int *dc,short (*data)[])
{
    int num;/*the number that's been read*/
    int ans;/* will contain the answer of check_numbers_saperate regarding the integrity of the row*/
    char *cp/*temporary char pointer*/,*comma=",",rest_line[MAX_LINE_LENGTH]/*A copy of the rest of the line*/;
    strcpy(rest_line,rest);
    ans  = check_numbers_saperate(rest_line);/*Removes spaces and checks whether the commas sparate betwween all nums,and writted properly */
    if(ans  != T){
        eror(eror_node,ans );
        return;
        }
    cp = strtok(rest_line,comma);/* Tokenize the rest of the line */
    if (cp == NULL){/*cheks if the data didnt get arguments*/
        eror(eror_node,NO_PARAMS);
        return;
    }
    while (cp)
    {
        if((*dc) > MAX_SIZE_MEMOREY){/*If the memorey is full*/
            eror(eror_node,OVER_SIZE);
            (*dc)=0;/*To avoid overflow*/
        }
        num = get_num(eror_node,cp);
        if (IS_WITHIN_15_BITS(num)){
            (*data)[(*dc)++] = num;/* Add number to data array and increment dc */
            cp = strtok(NULL,comma);
        }
        else{/*The number cannot be represented in 15 bits*/
            eror(eror_node,NUM_REP);
        }
        
        
    }
}

/*Processes the argument part of ".string" directive. The function counts with the help of how_many_c the amount of qoutes.
 And so while going over the line, it checks that all the characters are between the first and the last " and enters them into data.
Parameters:
eror_node: The information required to read the error such as a pointer to an error flag, line number, etc.
rest_line: The portion of the line containing the string to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the string will be stored.
*/
void add_string(erors_node eror_node, char *rest_line,int *dc,short (*data)[])
{
    int quotes_c,count =0;/*to store the amount of quotes in the string and how many of them we have reached so far respectively*/
    quotes_c = how_many_c(rest_line,'"');
    if(quotes_c < MIN_QUOTES )/*If too little ' " ' */
        {
            eror(eror_node, FAIL_STRING);
            return;
        }
    while(*rest_line)
    {
        if(count == 0)/*we haven't reach " yet */
        {
            if (*rest_line == '"')
                count++;
            else if (!isspace(*rest_line)){/*cheks that there are only blank char before quote */
                eror(eror_node, FAIL_STRING);
                return;
            }
        } else if (count < quotes_c ){ /*we inside the string case*/
            if((*dc) > MAX_SIZE_MEMOREY){/*If the memorey is full*/
                eror(eror_node,OVER_SIZE);
                (*dc)=0;/*To avoid overflow*/
            }
            (*data)[(*dc)++] = *rest_line;/* Add character to data array and increment dc */
            if (*rest_line == '"')
                count++;
        } 
        if (count == quotes_c)/*we have reched the final "*/
        {
            (*data)[(*dc)- NULL_SIZE ] = '\0';/* Add NULL charcter instead of the last " */
            if(!chek_end_line(++rest_line))/*check if the end of the line is empty*/
                    eror(eror_node,FAIL_STRING);
                return;/* End of string processing */
        }
        rest_line++;
    }
}