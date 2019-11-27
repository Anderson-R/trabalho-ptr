#include<stdio.h>
#include<string.h>

int main(){
    char buffer[18];
    FILE *fp = fopen("teste2", "r");
    int a = 18+18;
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( cliente[0] = fgets (buffer , 18 , fp) != NULL )
        {
            
            puts (buffer);
            fgets(buffer, 18, fp);
            puts(buffer);
            fgets(buffer, 18, fp);
            puts(buffer);
            
        }
        printf("Clientes: %s", cliente);
    }
    fclose(fp);
    
}