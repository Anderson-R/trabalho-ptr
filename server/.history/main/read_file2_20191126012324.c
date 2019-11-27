#include<stdio.h>
#include<string.h>

int main(){
    char cliente[18][3];
    FILE *fp = fopen("teste2", "r");
    if(fp == NULL)
    {
        perror("Error opening file");
    }else
    {
        if( fgets (cliente[0] , 18 , fp) != NULL )
        {
            puts (cliente[0]);
            fgets(cliente[1], 18, fp);
            puts(cliente[1]);
            fgets(cliente[2], 18, fp);
            puts(cliente[2]);
            
        }
        printf("Clientes: %s, %s, %s", cliente[0], cliente[1], cliente[2]);
    }
    fclose(fp);
    
}