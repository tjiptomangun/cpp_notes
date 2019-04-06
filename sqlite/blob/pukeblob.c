#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define STMLEN 1024
#define key_size 64

unsigned char cipher_key[] = {'m','3', 7 , 4 ,'l','o','g','I','x', 2 , 0 , 1 , 3 ,'@','$','#',
                              '%','&','*', 1 ,'F','R','0',230,240,155,11 ,'R','h', 20, 21, 21,
                              '6','^','&', 6 ,'8','b','2',244, 40, 55, 1 , 11,101, 11,'5','1',
                               0 , 77,175, 33,111,'b',122,101, 12, 44, 3 ,  2,  1, 12,'5','4',
                                0,0,0,0
                              };

extern int sqlite3_key(sqlite3 *db, const void *pKey, int nKey);
int main (int argc, char **argv)
{
	char szQuery[200];
	sqlite3 * db = NULL;
	sqlite3_open(argv[1], &db);
	sqlite3_key(db, cipher_key, key_size);
	sprintf(szQuery, "SELECT %s from %s %s", argv[2],argv[3],argv[4]);
	printf("%s\n",szQuery);

	sqlite3_stmt* res = NULL;
	sqlite3_prepare_v2(db, szQuery,-1, &res, 0);

	sqlite3_step(res);

	const unsigned char  *pHead = (unsigned char *) sqlite3_column_blob(res, 0);

	int nBytes = sqlite3_column_bytes(res, 0);

	FILE *fp = fopen(argv[5],"wb");
	fwrite(pHead, 1, nBytes, fp);
	fclose(fp);
	
	return 0;
}

