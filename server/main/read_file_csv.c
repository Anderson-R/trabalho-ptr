#include<stdio.h>
#include<string.h>

void getData(char *buff);
int main()
{
	FILE *fp = fopen("./teste.csv","r");
    int count=0;
	char buff[5000];
	do
	{
	    fgets(buff, 5000, (FILE*)fp);
	    getData(buff);
	    
	    count++;
	}while((getc(fp))!=EOF);
    
    fclose(fp);
    printf("%s", buff);
	return 0;
}

void getData(char buff[])
{
   char *token = strtok(buff,",");
   int counter=0;
 
   while( token != NULL ) 
   {
        counter++; 
        printf( " %s\n",token);
        token = strtok(NULL,",");
   }	  
}
