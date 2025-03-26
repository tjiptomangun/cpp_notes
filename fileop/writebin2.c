/*
gcc -Wall -ggdb2 writebin2.c -o writebin2
*/
#include <stdio.h>


int main (int argc,char *argv[])
{
   unsigned char toWrite1[]={"00:00:00.000   0  XSockWrite   0\n              0010  \x31\x30\x00\x01\x20\x20\x03\x23\158\155\x20\x20\n"};
   unsigned char toWrite2[]={"00:00:10.000   0  XSockRead   0\n              0010  \x01\x0\x30\x01\x20\x20\x03\x23\158\155\x20\x20\n"};
   unsigned char toWrite3[]={"              0010  \x01\x0\x30\x01\x20\x20\x03\x23\158\155\x20\x20\n"};
   unsigned char read1[65],read2[65],read3[32];
   FILE *fp = fopen("binfile","wb");
   FILE *fpNull;
   while(1) { 
	   for (int i = 0; i < 10000000; i++) {
		   fwrite(toWrite1,1,65,fp);
		   fwrite(toWrite2,1,65,fp);
		   fwrite(toWrite3,1,32,fp);
		}
		fseek(fp, 0, SEEK_SET);
	}

   fclose(fp);
/*   
   fp = fopen("asciifile","w");
   fwrite(toWrite1,1,65,fp);
   fwrite(toWrite2,1,65,fp);
   fwrite(toWrite3,1,32,fp);
   fclose(fp); 
   
   fp = fopen("binfile","r");
   fread(read1,1,65,fp);
   fwrite(read1,65,1,stdout);
   fseek(fp, 3,SEEK_CUR);
   fread(read2,1,62,fp);
   fwrite(read2,62,1,stdout);
   fclose(fp);
   
   fputs("\n\n",stdout);
   fp = fopen("binfile","r");
   fread(read1,1,65,fp);
   fwrite(read1,65,1,stdout);
   fread(read2,1,62,fp);
   fwrite(read2,62,1,stdout);
   fclose(fp);
   
   fpNull = fopen("/dev/null","w");
   fputs("\n\n",stdout);  
   fp = fopen("binfile","r");
   fread(read1,1,65,fp);
   fwrite(read1,65,1,stdout);
   fseek(fp, 3,SEEK_CUR);
   fread(read2,1,62,fp);
   fwrite(read2,62,1,fpNull);
   fseek(fp, -68,SEEK_CUR);
   fread(read2,1,65,fp);
   fwrite(read2,65,1,stdout);
   fclose(fp);   
   fputs("\n\n",stdout);  
   fclose(fpNull);
 */
}
