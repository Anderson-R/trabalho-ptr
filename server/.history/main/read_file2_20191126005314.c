#include<stdio.h>
#include<string.h>

int main(){
    char mystring[40];
    FILE *fp = fopen("teste2", "r");
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( fgets (mystring , 40 , fp) != NULL )
        {
            puts (mystring);
            fgets(mystring, 40, fp);
            puts(mystring);
        }
    }
    fclose(fp);
    
}