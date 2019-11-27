#include<stdio.h>
#include<string.h>

int main(){
    char buffer[20];
    FILE *fp = fopen("teste2", "r");
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if(fgets (buffer , 20 , fp) != NULL )
        {
            puts (buffer);
            fgets(buffer, 20, fp);
            puts(buffer);
            fgets(buffer, 20, fp);
            puts(buffer);
            
        }
        printf("Clientes: %s", cliente);
    }
    fclose(fp);
    
}