#include <stdio.h> 
#include <string.h> 
int main(void)
 {
     char buf[128] = {0};      //����������
     FILE *fp = fopen("foo.txt", "r");  
if(fp==NULL)
{
printf("file does not right");
return1;
}
     while (0 != fread(buf, 1, 127, fp))      
{
         printf("%s",  buf);     //��ʾbuf���������
         memset(buf, 0, 128);      //  ��ʾ����Ҫ����   
}
fclose(fp);
return 0;
}
