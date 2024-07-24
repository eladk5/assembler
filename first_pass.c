#include "erors.c"
#include "help_funcs.c"


#define PROMPTE 1
#define INCTRACTION 2
#define METHOD(A) ( 1 << A )
/*  This function processes a single operand from a line of assembly code, determining its type and storing
    it in the provided instruction structure. It handles numeric literals, direct registers, and pointer registers.
    If the operand is a number, it checks if it fits within 12 bits and sets the appropriate fields in the instruction.
    If it is a register, it determines if it's a direct or pointer register and sets the corresponding fields.
    It should be emphasized that this function does not handle the case of a label,
    and in fact everything it does not know how to identify it defines as a label. Labeling will be handled later.
    
    Parameters:
    am_name: The name of the assembly file.
    line_num: The current line number being processed.
    eror_flag: Pointer to an error flag.
    line: The current line of assembly code.
    first_word: The operand to be read.
    operand: A pointer to the instruction structure to store the operand.
    which_operand: Indicates if the operand is a source or destination operand.
    Returns: The type of the operand read (NUMBER, P_REGISTER, D_REGISTER, or LABEL).
*/
static int read_operand(char *am_name,int line_num,int *eror_flag,char *line,char *first_word,instruction *operand,int which_operand)
{
    char *temp = first_word; /* Temporary pointer to traverse the operand string */
    int type = LABEL;   /* Initialize the default type to LABEL */
    int num;/*store the number that recived*/
    if (*temp == '#'){ /* Check if the operand is a number */
        type = NUMBER;
        temp++;
        num =get_num(am_name,line_num,eror_flag,temp,temp);
        if(IS_WITHIN_12_BITS(num)){
            (operand->number_word).a=T;
            (operand->number_word).value = num;
            return type;
        }
        else{/*The number cannot be represented in 12 bits*/
            eror(am_name,line_num,eror_flag,line,NUM_REP);
        }
    }
    if(*temp == '*'){/* Check if the operand is a pointer register */
        type = P_REGISTER;
        temp++;
    }
    if(*temp == 'r'){ /* Check if the operand is a register */
        if( type != P_REGISTER )
            type = D_REGISTER;
        temp++;
        if ( (strlen(temp)==1) && ( ('0' <=(*temp)) &&( (*temp) <= '7') ) ){ /* after 'r' apeers one char that it register number*/
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
    am_name: The name of the assembly file.
    line_num: The current line number being processed.
    eror_flag: Pointer to an error flag.
    line: The current line of assembly code.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operand.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
static void one_operand(char *am_name,int line_num,int *eror_flag,char *line,int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] )
{
    int method;/* Variable to store the addressing method of the operand */
    instruction operand= {0};/* Instruction structure to store the operand */
    char first_word[MAX_LINE_LENGTH]= {0};/* Array to store the operand string */
    if (!sscanf(rest_line,"%s",first_word) || chek_end_line(first_word))/* Check if there's a operand */
    {
        eror(am_name,line_num,eror_flag,line,NO_PARAMS);
    }
    method = read_operand(am_name,line_num,eror_flag, line,first_word,&operand,TARGET);/* Read the operand and determine its method */
    if(!cmd[command_type].method_allowed[TARGET][method]){
        eror(am_name,line_num,eror_flag,line,NO_VALID_METHOD);/*case of operand that does not match the command type*/
        return;
    }
    (*coms)[(*ic)].ins.first_command.target_method |= METHOD(method); /* Update the first command with the operand method */
    if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the next word*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
    (*coms)[(*ic)].type_of_instraction=method; /*update the next word*/
    if(method != LABEL){
        (*coms)[(*ic)].ins = operand;
    }
    rest_line = strstr(rest_line,first_word)+strlen(first_word);
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(am_name,line_num,eror_flag,line,EXTEA_CHARS);
     if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the next word*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
}
/*  This function processes a line of assembly code with two operands, reads the operands, verifies their methods,
    and updates the instruction structure. It checks for errors such as missing or invalid operands, ensures the memory
    is not full, and verifies there are no extra characters at the end of the line.
    Parameters:
    am_name: The name of the assembly file.
    line_num: The current line number being processed.
    eror_flag: Pointer to an error flag.
    line: The current line of assembly code.
    command_type: The type of the command.
    rest_line: The rest of the line containing the operands.
    ic: Pointer to the instruction counter.
    coms: Pointer to the command array.
*/
static void two_operands(char *am_name,int line_num,int *eror_flag,char *line,int command_type,char *rest_line,int *ic,command (*coms)[MAX_SIZE_MEMOREY] )
{
    int method_1,method_2;/* Variables to store the addressing methods of the operands */
    instruction operand_1={0},operand_2= {0};/* Instruction structures to store the operands */
    char first_word[MAX_LINE_LENGTH]={0},second_word[MAX_LINE_LENGTH]={0};/* Arrays to store the operand strings */
    if (!sscanf(rest_line," %[^, ] ,%s",first_word,second_word) || chek_end_line(first_word) || chek_end_line(second_word))
    { /* Check if there are valid operands */
        eror(am_name,line_num,eror_flag,line,NO_PARAMS);/*If not enough arguments were received*/
    }
    method_1 = read_operand(am_name,line_num,eror_flag, line,first_word,&operand_1,SOURCE);
    method_2 = read_operand(am_name,line_num,eror_flag, line,second_word,&operand_2,TARGET);
    if( !cmd[command_type].method_allowed[SOURCE][method_1] || !cmd[command_type].method_allowed[TARGET][method_2])
    /*case of operand that does not match the command type*/
    {
        eror(am_name,line_num,eror_flag,line,NO_VALID_METHOD);
        return;
    }
    (*coms)[(*ic)].ins.first_command.source_method |= METHOD(method_1);
    (*coms)[(*ic)].ins.first_command.target_method |= METHOD(method_2);
    if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the next word*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
    (*coms)[(*ic)].type_of_instraction=method_1;
    if( ( (method_1 == D_REGISTER) || (method_1 == P_REGISTER) ) && ( (method_2 == D_REGISTER) || (method_2 == P_REGISTER) ) ){
    /*if both of the operands are registers*/
        (*coms)[(*ic)].ins.register_word.target_operand = operand_1.register_word.target_operand ;
        (*coms)[(*ic)].ins.register_word.source_operand = operand_2.register_word.source_operand;
    }else{ 
        if(method_1 != LABEL)
            (*coms)[(*ic)].ins = operand_1;
        if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the next word*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
        (*coms)[(*ic)].type_of_instraction=method_2;
        if(method_2 != LABEL){
            (*coms)[(*ic)].ins = operand_2;
        }
    }
    rest_line = strstr(rest_line,first_word)+strlen(first_word);
    rest_line = strstr(rest_line,second_word)+strlen(second_word);
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(am_name,line_num,eror_flag,line,EXTEA_CHARS);
    if(++(*ic) > MAX_SIZE_MEMOREY){/*advances to the next word*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);/*If the memorey is full*/
            (*ic)=0;/*To avoid overflow*/
        }
}
/*This function processes a line of entry or extern. The function adds the label name to the appropriate array, 
and allocates more memory to the array if necessary. 
In case of a lack of a parameter or unnecessary characters, an error is reported and marked accordingly.

Parameters:
am_name: The name of thefile being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
rest_line: The portion of the line containing the name to be processed.
counter: Pointer to an integer that keeps track of the number of names added to the array.
names_arrey: Pointer to the dynamic array of strings where the names will be stored.
size: Pointer to an integer representing the current size of the dynamic array.*/
static void add_ext_en(char *am_name,int line_num,int *eror_flag,char *line,char *rest_line,int *counter ,char (**names_arrey)[MAX_LINE_LENGTH],int *size)
{
    int i, new_size = *size * 2;
    char temp[MAX_LINE_LENGTH];
    if ( !sscanf(rest_line,"%s",temp) || chek_end_line(rest_line))
    {
        eror(am_name,line_num,eror_flag,line,NO_PARAMS);
        return;
    }
    (*counter)++;
    if((*counter) == (*size)){/*if realloc is nedded*/
        *names_arrey=realloc(*names_arrey, new_size * MAX_LINE_LENGTH * sizeof(char));
        if (names_arrey == NULL) {
        printf("\nFailed to allocate memory");
        exit(1);
        }
        for ( i = (*size); i < new_size; i++) {
            (**names_arrey)[i] = '\0';
        }
        *size = new_size;
    }
    strcpy((*names_arrey)[*counter-1], temp);/*-1 for the array to start from 0*/
    rest_line = strstr(rest_line,temp)+strlen(temp);
    if(!chek_end_line(rest_line))/*Checks if there are extra characters at the end of the line*/
        eror(am_name,line_num,eror_flag,line,EXTEA_CHARS);
}

/*This function processes a line of assembly code to add numerical data to the data array,
ensuring the input format is correct. It first checks if the commas in the input string are correctly placed,
then tokenizes the string based on commas, converts each token to a number using get_num, 
and adds each number to the data array. If an error is detected, it sets an error flag and returns.

Parameters:

am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
rest_line: The portion of the line containing the data to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the numerical data will be stored.*/
static void add_data(char *am_name,int line_num,int *eror_flag,char *line,char *rest,int *dc,short (*data)[])
{
    int num, temp;
    char *cp,*comma=",",rest_line[MAX_LINE_LENGTH];
    strcpy(rest_line,rest);
    temp = check_numbers_saperate(rest_line);/*Removes spaces and checks whether the commas sparate betwween all nums,and writted properly */
    if(temp != T){
        eror(am_name,line_num,eror_flag,line,temp);
        return;
        }
    cp = strtok(rest_line,comma);/* Tokenize the rest of the line */
    if (cp == NULL){/*cheks if the data didnt get arguments*/
        eror(am_name,line_num,eror_flag,line,NO_PARAMS);
        return;
    }
    while (cp)
    {
        if((*dc) > MAX_SIZE_MEMOREY){/*If the memorey is full*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);
            (*dc)=0;/*To avoid overflow*/
        }
        num = get_num(am_name,line_num,eror_flag,line,cp);
        if (IS_WITHIN_15_BITS(num)){
            (*data)[(*dc)++] = num;/* Add number to data array and increment dc */
            cp = strtok(NULL,comma);
        }
        else{/*The number cannot be represented in 15 bits*/
            eror(am_name,line_num,eror_flag,line,NUM_REP);
        }
        
        
    }
}
/*Processes the argument part of .string promote. 
It ensures the string is properly enclosed in double quotes and checks for extra characters after the closing quote.
The function iterates through the line, identifying the start and end of the string, 
and storing each character within the quotes into the data array. 
If the string is not properly formatted, it sets an error flag and returns.

Parameters:

am_name: The name of the file being processed.
line_num: The current line number in the file.
eror_flag: Pointer to an integer flag used to indicate if an error occurred.
line: The original line of code being processed.
rest_line: The portion of the line containing the string to be processed.
dc: Pointer to the data counter, which tracks the current position in the data array.
data: Pointer to the data array where the string will be stored.
*/
static void add_string(char *am_name,int line_num,int *eror_flag,char *line,char *rest_line,int *dc,short (*data)[])
{
    char flag = F;/*flag to mark if we between ""*/
    while(*rest_line){ /* Loop until end of the string */
        if((*dc) > MAX_SIZE_MEMOREY){/*If the memorey is full*/
            eror(am_name,line_num,eror_flag,line,OVER_SIZE);
            (*dc)=0;/*To avoid overflow*/
        }
        if(flag){
            if(*rest_line != '\"')
                (*data)[(*dc)++] = *rest_line;/* Add character to data array and increment dc */
            else{
                if(!chek_end_line(++rest_line))/*check if the end of the line is empty*/
                    eror(am_name,line_num,eror_flag,line,EXTEA_CHARS);
                return;/* End of string processing */
            }
        }
        else{/* We haven't reached the opening quote yet */
            if (*rest_line == '\"' )
                flag = T;
            else{
                    if (!isspace(*rest_line)){/*cheks that there are only blank char before quote */
                        eror(am_name,line_num,eror_flag,line,FAIL_STRING);
                        return;
                    }
            }
        }
        rest_line++; 
    }
}

/*Assumption: a label that comes here is not NULL and ends in ':'*/
static label_node valid_label(char *am_name,int line_num,int *eror_flag,char *line,char *label,head *head_mac,head *head_label,FILE *am_file)
{
    label_node temp, new_label = NULL;/*for temporary and new labels nose*/
    int i, len= strlen(label);
    label[--len]= '\0'; /*removing the ':' and reducing len by 1*/
    if( len > MAX_LABEL_LENGTH ){/*cheks if the length of the label is over 31*/
        eror(am_name,line_num,eror_flag,line,LABEL_LENGTH);
        return new_label;
    }
    if( (len == 0) || !isalpha(*label))/*cheks if the first char is alphabetic*/
    {
        eror(am_name,line_num,eror_flag,line,NOT_ALPHA);
        return new_label;
    }
    for ( i = 1; i < len; i++) {/*cheks if all the others chars are latters or number*/
        if (!isalnum(label[i])){
            eror(am_name,line_num,eror_flag,line,NOT_ALPHA_NUM);
            return new_label;
        }   
    }
    if(search_command(label) != NOT_COMMAND)/*cheks if it is a command name*/
    {
        eror(am_name,line_num,eror_flag,line,LABEL_IS_COMMAND);
        return new_label;
    }
    if (is_macro_name(label,*head_mac))/*cheks if it is a macro name*/
    {
        eror(am_name,line_num,eror_flag,line,LABEL_IS_MACRO);
        return new_label;
    }
    new_label = (label_node)malloc(sizeof(struct Label));
    if (new_label==NULL)/*memorey allocation fail*/
    {
        fprintf(stderr,"Failed to allocate memory/n");
        fclose(am_file);
        /*free_the_label(head_label);*/
        free_the_mac(*head_mac);
        exit(1);
        }
    strcpy(new_label->name,label);
    (new_label->is_entry)=F;/*By default, later it will be checked if it was set that way.*/
    new_label->next = NULL;
    if ((head_label->head_of_list) == NULL )/*if its the first label*/
    {
        (head_label->head_of_list) = new_label;
        return new_label;
    }
    temp = (label_node)(head_label->head_of_list);
    while(temp){/*Going through the list of labels, if the new label name is already found, an error is reported.
     If not, adds the new node to the end of the list*/
        if( strcmp( (temp->name) , label ) == 0 ){  
            eror(am_name,line_num,eror_flag,line,LABEL_NAME_AGAIN);
            free(new_label);
            return  NULL;
        }
        if (temp->next == NULL){
            temp->next = new_label ;
            return new_label;
        }
        temp = (temp -> next);
    }
    return NULL;
}

void first_pass(char *am_name,head *head_node_mac,FILE *am_file)
{
    int size_en = START_SIZE,size_ex = START_SIZE ;/*Contains the size of the data structures assigned to .extern and .entry respectively*/
    char (*enters)[MAX_LINE_LENGTH],(*exters)[MAX_LINE_LENGTH];/*to contain the names of the externs and entry*/
    head label_head;
    command coms[MAX_SIZE_MEMOREY]={0};/*whill contain all the regular instraction words*/
	short data[MAX_SIZE_MEMOREY]={0};/*whill contain all the data or string words*/
	int eror_flag=T;
    char line[MAX_LINE_LENGTH+1],*rest_line;/* To store new line evry time (the 1 for NULL)*/
    int ic=0,dc=0,ex_c=0,en_c=0;/*to count the num of instraction and data words and .extern or .entry respectively*/
    int line_num =0;/*lines counter*/
    char label[MAX_LINE_LENGTH];/*to contain the label evry line*/
    int temp,i;
    int command_type; /*will contain the opcode of the command or the enum value if its promote  */
    char first_word[MAX_LINE_LENGTH]; /*to read the first word after the label*/
    label_node new_label;
    
    enters = malloc(START_SIZE * MAX_LINE_LENGTH * sizeof(char));
    exters = malloc(START_SIZE * MAX_LINE_LENGTH * sizeof(char));
    if (enters == NULL || exters == NULL) {
        printf("\nFailed to allocate memory");
        exit(1);
    }
    for (i = 0; i < START_SIZE; i++) {
        *(enters[i]) = '\0';
        *(exters[i]) = '\0';
    }
    label_head.head_of_list =NULL;
    while (fgets(line, MAX_LINE_LENGTH+1 ,am_file) ) /*reading new line */
    {
        new_label=NULL;
        *label = '\0';/*reset the label*/
        *first_word = '\0';/*reset the string*/
        line_num++;
        if (line[strlen(line) - 1] != '\n' && !feof(am_file)){/*cheks if the size of the line is to big*/
            eror(am_name,line_num,&eror_flag,line,LINE_LENGTH);
            while (fgetc(am_file) != '\n' && !feof(am_file));/*Reach a new line*/
            } 
        
        if( ! ( *line == ';' || chek_end_line(line) ) )/*cheks if it is not blank or note line*/
        {
            sscanf(line,"%s",label); 
            temp = strlen(label);
            if( temp > 0 && label[temp-1] != ':' )/*if the first word of the line isnt a lable*/
                {
                    *label = '\0';/*reset the label*/
                    rest_line = line;
                }
            else
                rest_line = strstr(line,label)+temp;/*now rest line is the part of the line after the label*/
            if (  sscanf(rest_line,"%s",first_word)==0 || (temp = strlen(first_word))==0 )  
            {
             /*If we got here, it means that the line is not empty, but rest_line is empty, which means that only a label appeared*/
                eror(am_name,line_num,&eror_flag,line, ONLY_LABEL );
            }
            command_type = search_command(first_word);
            if ( temp && (command_type == NOT_COMMAND))
                eror(am_name,line_num,&eror_flag,line, NOT_VALID_COMMAND);
            if(*label != '\0')
                new_label = valid_label(am_name,line_num,&eror_flag,line,label,head_node_mac,&label_head,am_file);
            if(( (*label== '\0') || new_label ) )
            {
                
                temp = strlen(first_word);
                rest_line = strstr(rest_line,first_word)+temp;/*now rest_line is the part of the arguments*/
                if (command_type >= MOV && command_type <= STOP){/*the That is, it belongs to the opcode of a command*/
                coms[ic].type_of_instraction = FIRST;
                coms[ic].ins.first_command.a=T;
                coms[ic].ins.first_command.opcode = (unsigned int)command_type;
                }
                switch (command_type)
                {
                    case STRING:
                        if(*label != '\0'){/*if there is label*/
                            (new_label->adress) = dc;
                            (new_label->flag_type)=PROMPTE;
                        }
                        add_string(am_name,line_num,&eror_flag,line,rest_line,&dc,&data);
                        break;
                    case DATA:
                        if(*label != '\0'){/*if there is label*/
                            (new_label->adress) = dc;
                            (new_label->flag_type)=PROMPTE;
                        }
                        add_data(am_name,line_num,&eror_flag,line,rest_line,&dc,&data);
                        break;
                    case ENTRY:
                        if(*label != '\0'){
                            printf("\nwarning: in file: %s,line number %d\nin the line:%s",am_name,line_num,line );
                            printf("Declaring a label on .extern or .entry lines is unnecessary.\n\n");
                        }
                        add_ext_en(am_name,line_num,&eror_flag,line,rest_line,&en_c,&enters,&size_en);
                        break;
                    case EXTERN:
                    if(*label != '\0'){
                            printf("\nwarning: in file: %s,line number %d\nin the line:%s",am_name,line_num,line );
                            printf("Declaring a label on .extern or .entry lines is unnecessary.\n\n");
                        }
                        add_ext_en(am_name,line_num,&eror_flag,line,rest_line,&ex_c,&exters,&size_ex);
                        break;
                    case RTS:
                    case STOP:/*the 0 operands comands*/
                        ic++;
                        if(!chek_end_line(rest_line))
                            eror(am_name,line_num,&eror_flag,line,EXTEA_CHARS);
                        break;
                    case CLR:
                    case NOT:
                    case INC:
                    case DEC:
                    case JMP:
                    case BNE:
                    case RED:
                    case PRN:
                    case JSR:/*the 1 operands comands*/
                        one_operand(am_name,line_num,&eror_flag,line,command_type,rest_line,&ic,&coms);
                        break; 
                    case NOT_COMMAND:
                        break;
                    default:/*all the two operand commands*/
                        two_operands(am_name,line_num,&eror_flag,line,command_type,rest_line,&ic,&coms);
                        break;
                }
            }     
        }
        if ( (ic+dc) > MAX_SIZE_MEMOREY )  /*cheking if we heave to many words*/
        {
            eror(am_name,line_num,&eror_flag,line,OVER_SIZE);
            /*And so that there is no deviation from the size of the arrays*/
            ic=0;
            dc=0;
        } 
    }/*close the while*/
}


int main()
{
    head h;
    FILE *ifp;
    h.head_of_list =NULL;
    ifp=fopen("test_data.am","r");
    first_pass("test_data.am",&h,ifp);
    return 0;
}