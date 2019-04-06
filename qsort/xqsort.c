#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define XL_OBJECT_NAME_SIZE 20
#define XIM_DNAM_SIZE  20
#define MAX_DEVICE 10

/** 
 * The _strnicmp() function compare two strings starting from offset 0
 * up to a certain length.
 * 
 * @param s1, s2   	The strings that you want to compare.
 * @param len		The maximum number of characters that you want to
 *					compare.
 * @return		  	< 0 s1 is less than s2.
 *					0 s1 is equal to s2.
 *					> 0 s1 is greater than s2.
 *
 */
int _strnicmp (const char* s1, const char* s2, size_t len) {
	int i, j;

	do {
		if (((i = (unsigned char)(*(s1++))) >= 'A') && (i <= 'Z')) {
			i -= 'A' - 'a';
		}
		
		if (((j = (unsigned char)(*(s2++))) >= 'A') && (j <= 'Z')) {
			j -= 'A' - 'a';
		}
	} while (--len && i && (i == j));

	return i - j;
}


/** 
 * The _stricmp() function compares two strings, ignoring case.
 * 
 * @param s1, s2   	The strings that you want to compare.
 * @return		  	< 0 s1 is less than s2.
 *					0 s1 is equal to s2.
 *					> 0 s1 is greater than s2.
 *
 */
int _stricmp( const char* s1, const char* s2 ) {
	char i, j;

	do {
		i = ((*s1 <= 'Z') && (*s1 >= 'A')) ? *s1 + 'a' - 'A' : *s1;
		j = ((*s2 <= 'Z') && (*s2 >= 'A')) ? *s2 + 'a' - 'A' : *s2;
		s1++;
		s2++;
	} while ((i) && (i == j));

	return (int) (i - j);
}

#define strnicmp _strnicmp
typedef struct dev_table_struct 
{
	int	nIndex;
	char	sDevName [XL_OBJECT_NAME_SIZE+1];
	char	sCashAcctDevName [XL_OBJECT_NAME_SIZE+1];
	char	sDUMMY [10];
	char	sUNUSED [198];
	
}DEV_TABLE_STRUCT, *PDEV_TABLE_STRUCT;

DEV_TABLE_STRUCT DevTable[MAX_DEVICE];
DEV_TABLE_STRUCT  * DevTableByName[MAX_DEVICE];
int devTableIdx = 0;

static int CompareDevName (const void *st1, const void *st2)
{
        return (strnicmp (((DEV_TABLE_STRUCT *) st1)->sDevName,
                ((DEV_TABLE_STRUCT *) st2)->sDevName, XIM_DNAM_SIZE));

} /* End of CompareDevName */



int devTableAdd (char *devTableName, char *sCashAcctDevName)
{
	DevTable[devTableIdx].nIndex = devTableIdx;
	sprintf (DevTable[devTableIdx].sDevName,"%s", devTableName);
	sprintf (DevTable[devTableIdx].sCashAcctDevName,"%s", sCashAcctDevName);
	DevTableByName[devTableIdx] = &DevTable[devTableIdx];
	devTableIdx++;
	return 0;
}

int initDevTable()
{
	memset (DevTable, 0, 10* sizeof (DEV_TABLE_STRUCT));
	devTableAdd ("boersma", "van kempen");
	devTableAdd ("ruud", "van andel");
	devTableAdd ("edsger", "djikstra");
	devTableAdd ("rijn", "van kujt");
	devTableAdd ("piter", "anselma");
	devTableAdd ("annelies", "malema");
	devTableAdd ("bondewijn", "zenden");
	devTableAdd ("edward", "zwick");
	devTableAdd ("", "van asdalm");
	devTableAdd ("donald", "knuth"); 
	return 0;
}

int main (int argc, char **argv)
{
	int i = 0;
	initDevTable();	


	qsort (DevTableByName, MAX_DEVICE, 
		sizeof (DEV_TABLE_STRUCT *), CompareDevName); 

	for (i = 0; i < MAX_DEVICE; i++)
	{
		fprintf (stdout, "%s\n", DevTableByName[i]->sDevName);
	}
	exit (0);
}
