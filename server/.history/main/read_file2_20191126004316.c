#include<stdio.h>
#include<string.h>

int main(){
    char mystring[100];
    FILE *fp = fopen("teste2.txt", "r");
    if ( fgets (mystring , 100 , fp) != NULL ) puts (mystring);
    fclose(fp);
    
}