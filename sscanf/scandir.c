#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#define DT_TYPE_STR  1
#define DT_TYPE_NUM  2

#define LOCAL_BUFF_SIZE 40


/***
scandir
This program will read a pattern from first argument of command line.
The second argument is considered as directory.
It then  will interpret the file name in the directory and assigned 
to appropriate variable.
There are 5 string of integer variable and 5 string of alphanumneric variable
$CC -Wall -ggdb scandir.c -o scandir

*/



//make list /array of parsed argc string here
//it should be dynamic structure instead of array of static size.
//We may be need it next time, or it will be combinable in the future.


int setInt ( int *nOut,  char *szIn )
{
	* nOut = atoi ( szIn );
	return 0;
}
int setIntTempl( int *nOut, int nSize, char *szIn, int nLen)
{
	setInt (nOut, szIn);
	return 0;
}
int setString ( char *szOut , int nOutSize, char *szIn , int nInLen)
{
	strncpy ( szOut, szIn, (nInLen>nOutSize )? nOutSize:nInLen);
	return 0;
}
int setVoid ( void *Out, void *In)
{
	return 0;
}
int setVoid2 (unsigned char *out, char *szIn)
{
	return 0;
}
//int (* setValFun []) (unsigned char * out , int outLen, char *szIn, int inLen)  =
int  n1stInt,n2ndInt,n3rdInt,n4thInt,n5thInt;

char szPKeyFieldName[LOCAL_BUFF_SIZE]={'\0'};
char szStringFieldName[LOCAL_BUFF_SIZE]={'\0'};
char sz2ndStringFieldName[LOCAL_BUFF_SIZE]={'\0'};
char sz3rdStringFieldName[LOCAL_BUFF_SIZE]={'\0'};
char sz4thStringFieldName[LOCAL_BUFF_SIZE]={'\0'};
char sz5thStringFieldName[LOCAL_BUFF_SIZE]={'\0'};
char szBlobFieldName[LOCAL_BUFF_SIZE]={'\0'};
char szPKeyValue[LOCAL_BUFF_SIZE]={'\0'};
char szStringValue[LOCAL_BUFF_SIZE]={'\0'};
char sz2ndStringValue[LOCAL_BUFF_SIZE]={'\0'};
char sz3rdStringValue[LOCAL_BUFF_SIZE]={'\0'};
char sz4thStringValue[LOCAL_BUFF_SIZE]={'\0'};
char sz5thStringValue[LOCAL_BUFF_SIZE]={'\0'};
char szExprtFlnm[255]={'\0'};
char sz1stIntName[LOCAL_BUFF_SIZE]={'\0'};
char sz1stIntValue[LOCAL_BUFF_SIZE]={'\0'};
char sz2ndIntName[LOCAL_BUFF_SIZE]={'\0'};
char sz2ndIntValue[LOCAL_BUFF_SIZE]={'\0'};
char sz3rdIntName[LOCAL_BUFF_SIZE]={'\0'};
char sz3rdIntValue[LOCAL_BUFF_SIZE]={'\0'};
char sz4thIntName[LOCAL_BUFF_SIZE]={'\0'};
char sz4thIntValue[LOCAL_BUFF_SIZE]={'\0'};
char sz5thIntName[LOCAL_BUFF_SIZE]={'\0'};
char sz5thIntValue[LOCAL_BUFF_SIZE]={'\0'};

typedef struct var_struct
{
	char * optPtrVal;
	int varsout_len;
	int (* setValFun )();
	char replacement_char;
} VAR_STRUCT;

VAR_STRUCT optvars[] = 
{
	{  (char *) &n5thInt,  sizeof (int ), setIntTempl, 'd'},  //a
	{  0, -1, 0, 0 },//b
	{  0, -1, 0, 0 },//c
	{  (char *) sz2ndStringValue, LOCAL_BUFF_SIZE, setString, 's'}, //d
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  (char *) &n4thInt, sizeof (int ), setIntTempl, 'd'}, //g
	{  0, -1, 0, 0 }, 
	{  0, -1, 0, 0 },
	{  (char *) sz4thStringValue, LOCAL_BUFF_SIZE, setString, 's'},//j
	{  (char *) sz5thStringValue, LOCAL_BUFF_SIZE, setString, 's'},//k
	{  0, -1, 0, 0 },
	{  (char *) &n1stInt, sizeof (int ), setIntTempl ,'d'},//m
	{  (char *) &n2ndInt, sizeof (int ), setIntTempl ,'d'},//n
	{  (char *) &n3rdInt, sizeof (int ), setIntTempl ,'d'},//o
	{  (char *) szPKeyValue, LOCAL_BUFF_SIZE, setString, 's'}, //p
	{  0, -1, 0, 0 },
	{  (char *) sz3rdStringValue, LOCAL_BUFF_SIZE, setString , 's'},//r
	{  (char *) szStringValue, LOCAL_BUFF_SIZE, setString, 's'},//s
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 },
	{  0, -1, 0, 0 }
};
typedef struct evaluate_vars
{
	char replacement_char;
	char funIndex;
	int  nvarsout_len;
	char *vars_out;
	char *strIn;
} EVALUATE_VARS;
#define MAX_EVALVARS 20 
typedef struct evalvarlist 
{
	struct evalvarlist * this;
	int numEvaluationVars ;
	EVALUATE_VARS vars[MAX_EVALVARS];
	int ( *getNumEvalVars )(struct evalvarlist *);
	void ( *add) (struct evalvarlist *, char , char *, int, char); 
	void ( *resetIndex ) (struct evalvarlist *, int );
	void ( *setStrIn )(struct evalvarlist *, int, char *, int );	
	void ( *reset )(struct evalvarlist *);
}
EVALVARLIST , *PEVALVARLIST ;

EVALVARLIST a;

int __getNumEvalVars (PEVALVARLIST k )
{
	return k->numEvaluationVars;
}

void __add(PEVALVARLIST k, char funIndex, char *vars_out, int nvarsout_len, char replacement_char)
{
	k->vars[k->numEvaluationVars].funIndex = funIndex ;
	k->vars[k->numEvaluationVars].strIn = (char *) calloc ( 1, LOCAL_BUFF_SIZE );
	k->vars[k->numEvaluationVars].vars_out = vars_out ;
	k->vars[k->numEvaluationVars].nvarsout_len = nvarsout_len ;
	k->vars[k->numEvaluationVars].replacement_char = replacement_char ;
	k->numEvaluationVars ++;
}

void  __resetIndex(PEVALVARLIST k, int nIndex )
{
	int i = nIndex;
	k->vars[i].funIndex = 0;
	if (k->vars[i].strIn )
	{
		free (k->vars[i].strIn );
		k->vars[i].strIn = NULL;
	}
	k->vars[i].vars_out = 0;
	k->vars[i].nvarsout_len = 0;
	k->vars[i].replacement_char = 0;
	
}

void  __setStrIn(PEVALVARLIST k, int nIndex, char *szIn , int nInLen)
{
	int i = nIndex;
	memcpy ( k->vars[i].strIn, szIn, (nInLen > (LOCAL_BUFF_SIZE - 1) )?LOCAL_BUFF_SIZE -1 : nInLen);
} 

void  __reset(PEVALVARLIST k)
{
	int i;
	for ( i = 0; i < k->numEvaluationVars; i ++ )
	{
		__resetIndex(k, i);
	}
}

PEVALVARLIST  __makeEvalVarList ()
{
	PEVALVARLIST k = (PEVALVARLIST ) calloc (1, sizeof (EVALVARLIST ));
	k->numEvaluationVars = 0;
	k->getNumEvalVars = __getNumEvalVars;
	k->add = __add;
	k->resetIndex = __resetIndex;
	k->setStrIn = __setStrIn;
	k->reset = __reset;
	return k;
}

int set1stInt( char * szIn )
{
	return setInt ( &n1stInt, sz1stIntValue );
}
/*
 * a static array of array of chars
 * with a pointer to function that will assign/cast stream into appropriate variable
 *  
 */

int printRegCompErr(int reti)
{
  switch(reti)
  {
    case REG_BADBR:
      printf("Invalid use of back reference operator.\n");
      break;
    case REG_BADPAT:
      printf("Invalid use of pattern operators such as group or list.\n");
      break;
    case REG_BADRPT:
      printf(" Invalid use of repetition operators such as using '*' as the first character.\n");
      break;
    case REG_EBRACE:
      printf("Un-matched brace interval operators.\n");
      break;
    case REG_EBRACK:
      printf("Un-matched bracket list operators.\n");
      break;
    case REG_ECOLLATE:
      printf("Invalid collating element.\n");
      break;
    case REG_ECTYPE:
      printf("Unknown character class name.\n");
      break;
    case REG_EEND:
      printf("Non specific error.  This is not defined by POSIX.2.\n");
      break;
    case REG_EESCAPE:
      printf("Trailing backslash.\n");
      break;
    case REG_EPAREN:
      printf("Un-matched parenthesis group operators.\n");
      break;
    case REG_ERANGE:
      printf("Invalid  use  of  the range operator, e.g., the ending point of the range \n");
      printf("occurs prior to the starting point.\n");
      break;
    case REG_ESIZE:
      printf("Compiled regular expression requires a pattern blobfilename_ptrer larger than 64Kb.\n");
      break;
    case REG_ESPACE:
      printf("The regex routines ran out of memory.\n");
      break;
    case REG_ESUBREG:
      printf("Invalid back reference to a subexpression.\n");
      break;

  }
  return 0;

}//printRegCompErr


int sscanstring ( char * str, char * format, EVALVARLIST *pEl, unsigned int toPrint )
{
	int i = 0;
	char  * szOutFormat  = NULL ;
	EVALUATE_VARS *vars = pEl->vars;
	int numEvaluationVars = pEl->numEvaluationVars;
	for( i = 0 ; i < numEvaluationVars; i ++)
	{
		memset (vars[i].strIn, 0, LOCAL_BUFF_SIZE );
	}
	sscanf (str,format, vars[0].strIn, vars[1].strIn, vars[2].strIn, 
	vars[3].strIn, vars[4].strIn, vars[5].strIn, vars[6].strIn, vars[7].strIn,
	vars[8].strIn, vars[9].strIn, vars[10].strIn, vars[11].strIn);
	if ( toPrint )
	{
		printf ("\neval %s \n", str);
		szOutFormat =  (char *) malloc (40 * sizeof (char ));
	}
	for( i = 0 ; i < numEvaluationVars; i ++)
	{
		(optvars[(int )vars[i].funIndex]).setValFun(vars[i].vars_out, vars[i].nvarsout_len, vars[i].strIn, 
			strlen ( vars[i].strIn) );
		if ( toPrint )
		{
			memset (szOutFormat, 0, 40);
			sprintf(szOutFormat,"%%%c", vars[i].replacement_char);
	
			if (vars[i].replacement_char == 's' )
			{
				printf (szOutFormat, vars[i].vars_out);
				printf (",");
			}
			else if (vars[i].replacement_char == 'd' )
			{
				printf (szOutFormat,  * ((int * )vars[i].vars_out) );
				printf (",");
	
			}
			
		}
	}
	if (toPrint )
	{
		printf ("\n");
	}
	return 0;

}

/*
 this function concatenates a directory with a filename
 adding '/' if the directory does not ended with one
*/

char *path_cat (const char *parent, char *child) {
	int i,j;
	size_t str1_len = strlen(parent);
	size_t str2_len = strlen(child);
	char *result;
	result = (char *)malloc((str1_len+str2_len+2)*sizeof (char));
	memset(result ,0,(str1_len+str2_len+2)*sizeof (char));
	if(parent[str1_len - 1]=='/')
	{
		strncpy (result,parent,str1_len-1);
	}
	else
	{
		strcpy (result,parent);
	} 
	strcat(result,"/");
	
        str1_len = i=strlen(result);
	for(j=0; j<str2_len;i++, j++) {
		result[i]=child[j];
	}
	result[str1_len+str2_len]='\0';
	return result;
}

int main ( int argc, char **argv )
{
	char * stringlist = NULL ;
	char szPatt[] = "[%%][0-9]*[adgjkmnoprs]";
	int reti = 0, i = 0, x = 0, inLen = 0;
	int ret  = 0;
	regex_t regex_patt;
	const size_t max_groups = 9;
	regmatch_t pm[max_groups];
	char strErr[256] = {'\0'};
	char *blobfilename_ptr;
	char *stringlist_ptr  = NULL;
	PEVALVARLIST pEval = __makeEvalVarList();
	struct dirent *dp;
	DIR *dir;
	const char *dir_path;	
	
		
	if ( argc < 2 )
	{
		printf ("usage:  %s stringlist directory_toevaluate\n", argv[0]);
		printf (" stringlist format %%[0-9]*spec\n");
		printf (" valid spec values are a,d,g,j,k,m,n,o,p,r,s\n");
		printf (" a,g,m,n,o is espected to receive string of integer values\n");
		printf (" d,j,k,r,s is espected to receive string of alphanumeric values\n");
		exit ( -1);
	}
	inLen = x = strlen (argv[1]);
	stringlist = (char * ) calloc (1, x + ((x & 1) ?1: 2 ));
	stringlist_ptr = stringlist;
	blobfilename_ptr = argv[1];

	reti = regcomp (&regex_patt, szPatt, REG_EXTENDED );
	if ( reti )
	{
		printRegCompErr( reti );
		regfree ( &regex_patt );
		return -1;

	
/*
* argv -> regex parse -> replace last char with %d or %s in respect to last char. 
* Put the last char to vars list last index, increase vars list index;
*/
		
	reti = regexec ( &regex_patt, argv[1], max_groups, pm, 0 );
	while ( reti == 0 )
	{
		for ( i = 0; i < max_groups; i++ )
		{
			if ( pm[i].rm_so == -1 )
				break;	

			if ( x > 0 )	
			{
				memcpy ( stringlist_ptr, blobfilename_ptr, pm[i].rm_so );
				memcpy ( stringlist_ptr + pm[i].rm_so, blobfilename_ptr + pm[i].rm_so, pm[i].rm_eo - pm[i].rm_so);
				pEval->add (pEval, stringlist_ptr[pm[i].rm_eo - 1] - 'a', 
					optvars[ stringlist_ptr[pm[i].rm_eo -1 ] -'a'].optPtrVal,
					optvars[ stringlist_ptr[pm[i].rm_eo -1] - 'a'].varsout_len,
					optvars[ stringlist_ptr[pm[i].rm_eo -1] - 'a'].replacement_char);
					stringlist_ptr[pm[i].rm_eo - 1] = 's'; 
         	blobfilename_ptr+=pm[i].rm_eo;
         	x-=pm[i].rm_eo;
          	stringlist_ptr+=pm[i].rm_eo;
			}
			stringlist_ptr [pm[i].rm_eo +1] = 0;
		}
     	reti = regexec(&regex_patt,blobfilename_ptr,max_groups,pm,REG_NOTBOL);
	}
	if ( x > 0)	
	{
		memcpy ( stringlist_ptr, blobfilename_ptr, x );
		stringlist_ptr[x] = 0;
	}
	dir_path = argv[2];
	ret = access ( dir_path, F_OK);
	if (ret != 0)
	{
		perror (strErr);
		printf ("%s\n", strErr );
		exit (0);
	}
	dir = opendir( dir_path );
	while ( ( dp = readdir (dir ) ) != NULL )
	{
		//stringlist_ptr = path_cat (dir_path, dp->d_name );
		if (strcmp (dp->d_name, ".") && strcmp(dp->d_name,"..") )
			sscanstring (dp->d_name, stringlist, pEval, 1);

	}
	//sscanstring ( argv[2] , stringlist,  pEval, 1 );
	return 0;
}


