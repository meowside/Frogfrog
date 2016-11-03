#include <stdio.h>

main() {

   FILE *fp;
   char buff[255];

   fp = fopen("test.txt", "r");
   
   fputs("watafaka", fp);
   
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );

   fgets(buff, 255, (FILE*)fp);
   printf("2: %s", buff );
   
   fgets(buff, 255, (FILE*)fp);
   printf("3: %s", buff );
   fclose(fp);

}
