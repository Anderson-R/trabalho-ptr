#include<stdio.h>
#include<string.h>

int main(){
    char mystring[8];
    FILE *fp = fopen("teste2", "r");
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( fgets (mystring , 8 , fp) != NULL )
        {
            puts (mystring);
        }
    }
    fclose(fp);
    
}