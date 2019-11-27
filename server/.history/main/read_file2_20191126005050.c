#include<stdio.h>
#include<string.h>

int main(){
    char mystring[60];
    FILE *fp = fopen("teste2", "r");
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( fgets (mystring , 60 , fp) != NULL )
        {
            puts (mystring);
        }
    }
    fclose(fp);
    
}