#include<stdio.h>
#include<string.h>

int main(){
    FILE *fp = fopen("teste2.txt", "r");
    if ( fgets (mystring , 100 , pFile) != NULL ) puts (mystring);
    fclose(fp);
    
}