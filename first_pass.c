#include "all.h"


void first_pass(char *am_name,head *head_node_mac,FILE *am_file)
{
    command coms[MAX_SIZE_MEMOREY];/*whill contain all the regular commands words*/
	short data[MAX_SIZE_MEMOREY];/*whill contain all the data or string words*/
	int eror_flag=T;
    char line[MAX_LINE_LENGTH],rest_line[MAX_LINE_LENGTH];/* To store new line evry time */
    int ic=0,dc=0;/*to count the num of instraction and data words */
    int line_num =0;/*lines counter*/
    char label[MAX_LABEL_LENGTH+2];/*to contain the labels for each line +2 for the ':' and '\0'*/
    int temp;
    char first_word[MAX_LABEL_LENGTH]; /*to read the first word after the label*/
    
    while (fgets(line, MAX_LINE_LENGTH ,am_file) ) /*reading new line */
    {
        *label = '/0';/*reset the label*/
        *first_word = '/0';/*reset the string*/
        line_num++;
        if (line[strlen(line) - 1] != '\n' && !feof(am_file))/*cheks if the size of the line is to big*/
            eror(am_name,line_num,&eror_flag,line,LINE_LENGTH); 
        if ( (ic+dc) > MAX_SIZE_MEMOREY )  /*cheking if we heave to many words*/
        {
            eror(am_name,line_num,&eror_flag,line,OVER_SIZE);
            /*And so that there is no deviation from the size of the arrays*/
            ic=0;
            dc=0;
        }
        if( ! ( *line == ';' || chek_end_line(line) ) )/*cheks if it is not blank or note line*/
        {
            sscanf(line,"s",label); 
            temp = strlen(label);
            if( temp > 0 && label[temp-1] != ':' )/*if the first word of the line isnt a lable*/
                {
                    *label = '/0';/*reset the label*/
                    rest_line = line;
                }
            else
                rest_line = strstr(line,label)+temp/*now rest line is the part of the line after the label*/
            sscanf(rest_line,"s",first_word);
            
            


              
        }
        
        
    }

}