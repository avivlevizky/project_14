
#include "Insertion.h"
#include "Checkers.h"
#include "Assembler.h"
#include <string.h>
#include <stdio.h>

/*Function that insert the symbol into the symbols tabel by the type: 16 or 17 or 18 is data;19 is .entry and 20 is .extern otherwise is instruction type. and update SC counter too*/
void insertSymbolToTable(char *data,int type)
{
    
    
}


/* Function that insert the given data into the instructions_table, the function will update too the IC counter by matching to the length of the given data */
void insertToIT(char **data,int Instruc_type)
{
    if ((Instruc_type<=SUB)||(Instruc_type==LEA))
    {
        
        
        
        return;
    }
    if (((Instruc_type>=NOT)&&(Instruc_type<=CLR))||((Instruc_type>=INC)&&(Instruc_type<=JSR)))
    {
        
        
        
    }
    else  /*In the condition of Instruc_type==rts or Instruc_type==stop */
    {
        
        
    }
    
    
    
    
    
}

/*Function that insert data by the given Instruc_type argument into the data table*/
void insertToDT(char **data,int Instruc_type)
{
    
    
}

/*Update the given instruction command for the second checking*/
void updateInstruction(char **data,int Instruc_type)
{
    
    
}










