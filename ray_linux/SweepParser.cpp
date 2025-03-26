#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "SweepParser.h"
#include "Vector.h"
#include "Trace.h"

const char *swp_KeyWords[] = 
{

	"OFFSET","AMBIENT","DIFFUSE","SPECULAR","SPECSPREAD",
		"TRANSPARENCY","COLOR"

};
char swp_Buffer[256];
double swp_ka=.1,swp_kd=.1,swp_ks=.1,swp_kr=.1,swp_kt=.1;


int swp_NO=1;

VECTOR swp_Offset,swp_V1,swp_V2;
SwpColor swp_Ia1;
int swp_GetToken(FILE *fp)
{
	char Ch;
	int Ndx,i;
	Ndx=0;
	for(i=0;i<180;i++)
	{
	
		swp_Buffer[i]=0;
	
	}
	while(1)
	{
	
		Ch=fgetc(fp);
		if(Ch=='\t'||Ch==' '||Ch=='\n')
		{
		
			Ch=fgetc(fp);
		
		}

		if(Ch==EOF)
			return EOF;
		if(isalpha(Ch)||Ch=='_')
		{
		
			swp_Buffer[Ndx++]=toupper(Ch);
			while((Ch=fgetc(fp))!=EOF &&(isalpha(Ch)||Ch=='_'||isdigit(Ch)||Ch=='.'))
				swp_Buffer[Ndx++]=toupper(Ch);
			swp_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			for(i=0;i<SWPNUMKEYWORDS;i++)
			{
			
				if(stricmp(swp_KeyWords[i],swp_Buffer)==0) 
					return i;

			
			}
			return SWPUNKNOWN;
		}
		else if(Ch=='(')
			return SWPLEFTPAREN;
		else if(Ch==')')
			return SWPRIGHTPAREN;
		else if(Ch==',')
			return SWPCOMMA;
		else if(isdigit(Ch)||Ch=='.'||Ch=='-')
		{
	
			swp_Buffer[Ndx++]=Ch;

			while( ((Ch=fgetc(fp))!=EOF)   && (isdigit(Ch) || Ch=='.'))
				swp_Buffer[Ndx++]=Ch;			
			
			swp_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			return SWPNUMBER;
	
		}
		else if(Ch == '{')
			return SWPOPENCURLYBRACKET;
		
		else if(Ch=='/')//comment
		{
		
			while((Ch=fgetc(fp))!='/' && Ch!=EOF);
		
		}
	
	}

}

int swp_ReadInt(FILE *fp, int *val)
{
	int Token;
	Token = swp_GetToken(fp);
	if (Token == SWPLEFTPAREN) {
		Token = swp_GetToken(fp);
		if (Token == SWPNUMBER) {
			*val = atoi(swp_Buffer);
			Token = swp_GetToken(fp);
			if (Token == SWPRIGHTPAREN)
				return 1;
		}
	}
	return 0;
}

int swp_ReadFloat(FILE *fp,double *val)
{

	int Token;
	Token = swp_GetToken(fp);
	if (Token == SWPLEFTPAREN) {
		Token = swp_GetToken(fp);
		if (Token == SWPNUMBER) {
			*val =(double) atof(swp_Buffer);
			Token = swp_GetToken(fp);
			if (Token == SWPRIGHTPAREN)
				return 1;
		}
	}
	return 0;

}

int swp_ReadDouble(FILE *fp, double *val1, double *val2)
{
	int Token;
	Token = swp_GetToken(fp);
	if (Token == SWPLEFTPAREN) {
		Token = swp_GetToken(fp);
		if (Token == SWPNUMBER) {
			*val1 = (double)atof(swp_Buffer);
			Token = swp_GetToken(fp);
			if (Token == SWPCOMMA) {
				Token = swp_GetToken(fp);
				if (Token == SWPNUMBER) {
					*val2 = (double)atof(swp_Buffer);
					Token = swp_GetToken(fp);
					if (Token ==SWPRIGHTPAREN)
						return 1;
				}
			}
		}
	}
	return 0;
}

int swp_ReadTripple(FILE *fp, double *val1, double *val2, double *val3)
{
	int Token;
	Token = swp_GetToken(fp);
	if (Token == SWPLEFTPAREN) {
		Token = swp_GetToken(fp);
		if (Token == SWPNUMBER) {
			*val1 =(double) atof(swp_Buffer);
			Token = swp_GetToken(fp);
			if (Token == SWPCOMMA) {
				Token = swp_GetToken(fp);
				if (Token == SWPNUMBER) {
					*val2 =(double) atof(swp_Buffer);
					Token = swp_GetToken(fp);
					if (Token == SWPCOMMA) {
						Token = swp_GetToken(fp);
						if (Token == SWPNUMBER) {
							*val3 = (double)atof(swp_Buffer);
							Token = swp_GetToken(fp);
							if (Token == SWPRIGHTPAREN)
								return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
