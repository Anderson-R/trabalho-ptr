#include<stdio.h>
#include<string.h>

int main(){
    const int b = 18*3;
    char cliente[b], buffer[18];
    FILE *fp = fopen("teste2", "r");
    int a = 18+18;
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( cliente[0] = fgets (buffer , 18 , fp) != NULL )
        {
            
            puts (cliente[0]);
            fgets(cliente[18], 18, fp);
            puts(cliente[18]);
            fgets(cliente[a], 18, fp);
            puts(cliente[a]);
            
        }
        printf("Clientes: %s", cliente);
    }
    fclose(fp);
    
}