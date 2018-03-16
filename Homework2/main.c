#include <stdio.h> 
#include <string.h> 
int main(void)
 {
     char buf[128] = {0};      //创建缓冲区
     FILE *fp = fopen("foo.txt", "r");  
if(fp==NULL)
{
printf("file does not right");
return1;
}
     while (0 != fread(buf, 1, 127, fp))      
{
         printf("%s",  buf);     //显示buf数组的内容
         memset(buf, 0, 128);      //  显示过后要清零   
}
fclose(fp);
return 0;
}
