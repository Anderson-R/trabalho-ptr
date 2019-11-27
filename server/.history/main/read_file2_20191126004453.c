#include<stdio.h>
#include<string.h>

int main(){
    char mystring[40];
    FILE *fp = fopen("teste2.txt", "r");
    if ( fgets (mystring , 40 , fp) != NULL ) puts (mystring);
    fclose(fp);
    
}