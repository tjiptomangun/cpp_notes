#include <stdio.h>
#include "Parse.h"


char * KeyWords[]=
{
		"","COMPARE","CONCAT","CONTAIN","LENGTH","SUBSTRING",
		"TOLOWERCASE","TOUPPERCASE","PARSEFLOAT","PARSEINT",
		"FORMAT"
};

int GetToken(char **strToParse)
{
	char *strParsed=*strToParse;
	char Ch;
	int nDx,i,j;
	nDx=0;
	for(i=0;i<128;i++)
	{
		Buffer[i]=0;
	}
	i=0;
	while(1)
	{
		Ch=strParsed[i++];
		if(Ch=='\t'||Ch==' '||Ch=='\n')
		{
			Ch=strParsed[i++];
		}
		if(Ch=='\0') 
		{
			*strToParse=&strParsed[i];
			return 0;
		}
		if(isalpha(Ch)||Ch=='_')
		{
			Buffer[nDx++]=toupper(Ch);			
			while((Ch=strParsed[i++])!='\0'&&(isalpha(Ch)||Ch=='_'||isdigit(Ch)||Ch=='.'))
				Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			i--;
			for(j=0;j<NUMKEYWORDS;j++)
			{
			
				if(stricmp(KeyWords[j],Buffer)==0) 
				{
					*strToParse=&strParsed[i];
					return j;			
				}
			}
			*strToParse=&strParsed[i];
			return STRING;
		}
		else if(Ch=='(')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return LEFTPAREN;
		}
		else if(Ch==')')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return RIGHTPAREN;
		}
		else if(Ch==',')
		{	
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return COMMA;
		}
		else if(Ch=='"')
		{
			Buffer[nDx++]=Ch;
			while((Ch=strParsed[i++])!='"')
				Buffer[nDx++]=Ch;
			Buffer[nDx++]=Ch;
			*strToParse=&strParsed[i];
			return STRING;
		}
		else if(isdigit(Ch)||Ch=='.')
		{
			Buffer[nDx++]=Ch;
			while( ((Ch=strParsed[i++])!=EOF) && (isdigit(Ch) || Ch=='.'))
				Buffer[nDx++]=Ch;						
			Buffer[nDx]='\0';
			i--;
			*strToParse=&strParsed[i];
			return NUMBER;	
		}
		else if(Ch=='+')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return PLUS;
		}
		else if(Ch=='-')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return MINUS;
		}
		else if(Ch=='*')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return ASTERIX;
		}
		else if(Ch=='/')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return DIVIDE;
		}
		else if(Ch=='%')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return MODULUS;
		}
		else if(Ch=='^')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return EXPONENT;
		}
		else if(Ch=='&')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			Ch=strParsed[i++];
			if(Ch=='&')
			{
				Buffer[nDx++]=toupper(Ch);
				Buffer[nDx]='\0';
				*strToParse=&strParsed[i];
				return AND;
			}
			else
			{
				i--;
				*strToParse=&strParsed[i];
				return BITWISE_AND;
			}

		}
		else if(Ch=='|')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			Ch=strParsed[i++];
			if(Ch=='|')
			{
				Buffer[nDx++]=toupper(Ch);
				Buffer[nDx]='\0';
				*strToParse=&strParsed[i];
				return OR;
			}
			else
			{
				i--;
				*strToParse=&strParsed[i];
				return BITWISE_OR;
			}

		}
		else if(Ch==':')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return BITWISE_XOR;

		}
		else if(Ch=='=')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			Ch=strParsed[i++];
			if(Ch=='=')
			{
				Buffer[nDx++]=toupper(Ch);
				Buffer[nDx]='\0';
				*strToParse=&strParsed[i];
				return EQUALS;
			}
			else
			{
				i--;
				*strToParse=&strParsed[i];
				return ASSIGN;
			}
		}
		else if(Ch=='>')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			Ch=strParsed[i++];
			if(Ch=='=')
			{
				Buffer[nDx++]=toupper(Ch);
				Buffer[nDx]='\0';
				*strToParse=&strParsed[i];
				return GREATEROREQUALS;
			}
			else
			{
				i--;
				*strToParse=&strParsed[i];
				return GREATERTHAN;
			}
		}
		else if(Ch=='<')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			Ch=strParsed[i++];
			if(Ch=='=')
			{
				Buffer[nDx++]=toupper(Ch);
				Buffer[nDx]='\0';
				*strToParse=&strParsed[i];
				return LESSOREQUALS;
			}
			else
			{
				i--;
				*strToParse=&strParsed[i];
				return LESSTHAN;
			}
		}
		else if(Ch=='"')
		{
			Buffer[nDx++]=toupper(Ch);
			Buffer[nDx]='\0';
			*strToParse=&strParsed[i];
			return DOUBLE_QUOTES;
		}
	}
}
