#include <stdlib.h>
#include "Checkers.h"
#include "Assembler.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/*declaration of all the extern variables*/

/*
extern Symbol ** symbol_table;               
extern int * data_table;
extern Instruction ** instructions_table;
extern unsigned IC;
extern unsigned SC;
extern unsigned EC;
extern unsigned LC;
extern unsigned DC;
extern unsigned Total_IC;
*/







/*Private Aux function: which indicate if the given char is an alpha-bet charcter*/
int isGoodLetter (char toTest){
    int tester;
    
    tester = (int)toTest;
    if( (tester>= 65 && tester<=90) || (tester>= 97 && tester<=122) || (tester>= 48 && tester<=57))
        return 1;
    
    return 0;
}




/*******************************************************************************************************************************/



/*Bool Function that checks if the label is valid by return boolean value, if will be an error the function will insert the match error into ErrorsAssembler table */
int isValidLabel(char * label,int flagDotDot)
{
    int flag;             /*flag=0 is not valid label; flag=1 is valid label */
    size_t length;
    length= strlen(label);
    flag=1;
    
    if(isInstruction(label,0)>=0)
        flag=0;
    else
    {
        int k;
        
        if ((length>30)||(!isGoodLetter(label[0])))
            return 0;
        
        for (k = 0;k < (length-1+(!flagDotDot))&&(flag); k++)
        {
            if (!isGoodLetter(label[k]))
                flag=0;
        }
        if ((flagDotDot)&&(label[k]!=':'))
            flag=0;
    }
    
    return flag;
}




/*int Function that checks if the given order string is an vald instruction/data defining order: if is .data then ->16, if is .string then ->17, if is .mat then ->18 ,if is .entry then -> 19, if is .extern then -> 20 ,if is mov ->func.mov,otherwise the function will return -1*/
/*flagMessage: indicate if to print an error message or not*/
int isInstruction(char * order, int flagMessage)
{
    if(!order)
        goto Failure;
    
    if (strcmp(order,"mov")==0){
        return 0;
    }
    if (strcmp(order,"cmp")==0){
        return 1;
    }
    if (strcmp(order,"add")==0){
        return 2;
    }
    if (strcmp(order,"sub")==0){
        return 3;
    }
    if (strcmp(order,"not")==0){
        return 4;
    }
    if (strcmp(order,"clr")==0){
        return 5;
    }
    if (strcmp(order,"lea")==0){
        return 6;
    }
    if (strcmp(order,"inc")==0){
        return 7;
    }
    if (strcmp(order,"dec")==0){
        return 8;
    }
    if (strcmp(order,"jmp")==0){
        return 9;
    }
    if (strcmp(order,"bne")==0){
        return 10;
    }
    if (strcmp(order,"red")==0){
        return 11;
    }
    if (strcmp(order,"prn")==0){
        return 12;
    }
    if (strcmp(order,"jsr")==0){
        return 13;
    }
    if (strcmp(order,"rts")==0){
        return 14;
    }
    if (strcmp(order,"stop")==0){
        return 15;
    }
    if (strcmp(order,".data")==0){
        return 16;
    }
    if (strcmp(order,".string")==0){
        return 17;
    }
    if (strcmp(order,".mat")==0){
        return 18;
    }
    if (strcmp(order,".entry")==0){
        return 19;
    }
    if (strcmp(order,".extern")==0){
        return 20;
    }
    
Failure:if(flagMessage)
            insertNewError("Invalid or missing instruction in line: %d");
    return -1;
}



/*Int Function: search and return the index of the given label from the symbol table, if not exist return -1*/
int findSymbol(char * data)
{
    int i;
    
    if(!data)
        return -1;
    
    i=0;
    
    while((i<SC)&&(symbol_table[i]))
    {
        if(strcmp(symbol_table[i]->label_name,data)==0)
            return i;
        i++;
    }
    
    return -1;
}




/*Int function that check if the given string is an int value and return is value otherwise the function will return -1*/
int * isNumeric(char * data)
{
    int * value;
    int ans,i,signMinus,length;
    
    i=0;
    ans=0;
    signMinus=0;
    length=(int)strlen(data);
    value=(int *)malloc(sizeof(int));
    allocate_check(value);
    
    if(data[0]=='-')
        signMinus++;
    
    while((i+signMinus<length)&&(data[i+signMinus]>='0')&&(data[i+signMinus]<='9'))
    {
        ans=ans+((data[i+signMinus])-48)*pow(10,length-signMinus-i-1);
        
        i++;
    }
    if((i+signMinus)==length)
        *value= (signMinus) ? ans*(-1) : ans;
    else
    {
        free(value);
        value=NULL;
    }
    return value;
    
    
    
    
}





/*Boolean Function: check of the given string which type of addressing type it is*/
int isDirectOrRegister(char * data)
{
    if((data[0]=='#')||(data[0]=='r'))
    {
        int *value;
        value=isNumeric(data+1);
        
        
        if((!value)||((data[0]=='r')&&((*value<0)||(*value>7)))) /*if the operand is reg and the domain value is out of bounds-> return -1*/
            return -1;
        
        return (data[0]=='#') ? 0 : 3;
    }
    return -1;
    
}





/*function that checks if the given string is a valid matrix defining : return the the number of places that the matrix is takes otherwise return -1 if the defining is not valid*/
int isValidMatrixToData(char * mat)
{
    int * value;
    char * num;
    int balance,i,ans,chars_len,numOfBrac;
    char reader,prevReader;
    
    i=0;
    value=NULL;
    chars_len=1;
    balance=0;
    prevReader = '\0';
    reader='\0';
    ans=1;
    numOfBrac=0;
    
    num=(char *)calloc(1,sizeof(char));
    allocate_check(num);
    
    
    while((balance>=0)&&((reader=mat[i])!='\0'))
    {
        if(reader=='[')
        {
            if((i>0)&&(prevReader!=']'))
            {
                goto Failure;
            }
        }
        if(reader==']')
        {
            value=isNumeric(num);
            if((!value)||(*value<=0)) /*if value ==null and the next char after '#' isn't '0' then value is an error*/
            {
                goto Failure;
                
            }
            else
            {
                ans=ans*(*value);
                free(num);
                num=(char *)calloc(1,sizeof(char));
                allocate_check(num);
                chars_len=1;
            }
        }
        if((reader==']')||(reader=='['))
        {
            balance=(reader=='[') ? balance+1 : balance-1;
            numOfBrac++;
        }
        else
        {
            num=(char *)realloc((char *)(num), (chars_len+1)*sizeof(char));
            allocate_check(num);
            num[chars_len-1]=reader;
            num[chars_len]='\0';
            chars_len++;
        }
        prevReader=reader;
        i++;
    }
    
    
    if((balance!=0)||(numOfBrac!=4))
    {
    Failure: insertNewError("Invalid declaring matrix in Line: %d");
        free(num);
        if(value)
            free(value);
        return -1;
    }
    
    return ans;
}










/*function that checks if the given string is a valid matrix : then return the string as array of strings otherwise return null*/
char ** isValidMatrix(char * mat)
{
    char ** matFixed;           /*dynamic matrix of strings*/
    
    int wordCounter,balance,i,chars_len;
    char reader,prevReader ;
    
    i=0;
    balance=0;
    prevReader = '\0';
    matFixed=NULL;
    reader='\0';
    chars_len=1;
    wordCounter=0;
    
    matFixed=(char **)malloc(sizeof(char *));
    allocate_check(matFixed);
    
    matFixed[0]=(char *)calloc(1,sizeof(char));
    allocate_check(matFixed[0]);
    
    while((balance>=0)&&((reader=mat[i])!='\0'))
    {
        if(reader=='[')
        {
            if(wordCounter==0)
            {
                if(!isValidLabel(matFixed[0],0))
                {
                    /*insertNewError("Invalid Label of matrix in line: %d");*/
                    freeLinkedList(matFixed,wordCounter);
                    return NULL;
                }
            }
            else
            {
                if(prevReader!=']')
                {
                    /*insertNewError("Invalid syntex in line: %d");*/
                    freeLinkedList(matFixed,wordCounter);
                    return NULL;
                }
            }
        }
        
        if(reader==']')
        {
            if(isDirectOrRegister(matFixed[wordCounter])!=3)
            {
                /*insertNewError("Invalid register in index array line: %d");*/
                freeLinkedList(matFixed,wordCounter);
                return NULL;
            }
            
        }
        
        if((reader==']')||(reader=='['))
        {
            if(chars_len>1)
            {
                wordCounter++;
                matFixed=(char **)realloc((char **)matFixed, (wordCounter+1)*sizeof(char *));
                allocate_check(matFixed);
                matFixed[wordCounter]=(char *)calloc(1,sizeof(char));
                allocate_check(matFixed[wordCounter]);
                chars_len=1;
            }
            balance=(reader=='[') ? balance+1 : balance-1;
        }
        else
        {
            matFixed[wordCounter]=(char *)realloc((char *)(matFixed[wordCounter]), (chars_len+1)*sizeof(char));
            allocate_check(matFixed[wordCounter]);
            matFixed[wordCounter][chars_len-1]=reader;
            matFixed[wordCounter][chars_len]='\0';
            chars_len++;
        }
        prevReader=reader;
        i++;
    }
    
    if(chars_len>1)
    {
        wordCounter++;
        matFixed=(char **)realloc((char **)matFixed, (wordCounter+1)*sizeof(char *));
        allocate_check(matFixed);
    }
    matFixed[wordCounter]=NULL;
    
    if((balance!=0)||(wordCounter!=3))
    {
        /*insertNewError("Invalid syntex in line: %d");*/
        freeLinkedList(matFixed,wordCounter);
        return NULL;
    }
    
    return matFixed;
}






int checkAddressingType(char * data)
{
    int addrType;
    char ** mat_data = NULL;
    
    if(!data)
        return -2;
    
    
    addrType=isDirectOrRegister(data);
    
    if(addrType==-1)
    {
        if((mat_data=isValidMatrix(data)))
            addrType=2;
        else
            addrType=(isValidLabel(data, 0))  ? 1 : -1;
    }
    
    
    freeLinkedList(mat_data,3);
    return addrType;
    
}



















