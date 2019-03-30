#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stdafx.h"
#include "ParserEll.h"
#include "Vector.h"

char *ell_KeyWords[] = 
{

	"LOC","AMBIENT","DIFFUSE","SPECULAR","SPECSPREAD",
		"TRANSPARENCY","COLOR","ELLIPSOID","V1","V2",
		"CLOUD","CLOUDFRACTAL","NUMLEVEL","AMP",
		"FREK","DIM"

};
char ell_Buffer[256];
double ell_ka=.1,ell_kd=.1,ell_ks=.1,ell_kr=.1,ell_kt=.1;
int ell_NumLevels =2;
double ell_Amp = 1;
double ell_Frek = .5;
double ell_Dim = 1;
int ell_NO=1;

VECTOR ell_Loc,ell_V1,ell_V2;
EllColor ell_Ia1;
int ell_GetToken(FILE *fp)
{
	char Ch;
	int Ndx,i;
	Ndx=0;
	for(i=0;i<180;i++)
	{
	
		ell_Buffer[i]=0;
	
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
		
			ell_Buffer[Ndx++]=toupper(Ch);
			while((Ch=fgetc(fp))!=EOF &&(isalpha(Ch)||Ch=='_'||isdigit(Ch)||Ch=='.'))
				ell_Buffer[Ndx++]=toupper(Ch);
			ell_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			for(i=0;i<ELLNUMKEYWORDS;i++)
			{
			
				if(stricmp(ell_KeyWords[i],ell_Buffer)==0) 
					return i;

			
			}
			return ELLUNKNOWN;
		}
		else if(Ch=='(')
			return ELLLEFTPAREN;
		else if(Ch==')')
			return ELLRIGHTPAREN;
		else if(Ch==',')
			return ELLCOMMA;
		else if(isdigit(Ch)||Ch=='.'||Ch=='-')
		{
	
			ell_Buffer[Ndx++]=Ch;

			while( ((Ch=fgetc(fp))!=EOF)   && (isdigit(Ch) || Ch=='.'))
				ell_Buffer[Ndx++]=Ch;			
			
			ell_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			return ELLNUMBER;
	
		}
		else if(Ch == '{')
			return ELLOPENCURLYBRACKET;
		
		else if(Ch=='/')//comment
		{
		
			while((Ch=fgetc(fp))!='/' && Ch!=EOF);
		
		}
	
	}

}

int ell_ReadInt(FILE *fp, int *val)
{
	int Token;
	Token = ell_GetToken(fp);
	if (Token == ELLLEFTPAREN) {
		Token = ell_GetToken(fp);
		if (Token == ELLNUMBER) {
			*val = atoi(ell_Buffer);
			Token = ell_GetToken(fp);
			if (Token == ELLRIGHTPAREN)
				return 1;
		}
	}
	return 0;
}

int ell_ReadFloat(FILE *fp,double *val)
{

	int Token;
	Token = ell_GetToken(fp);
	if (Token == ELLLEFTPAREN) {
		Token = ell_GetToken(fp);
		if (Token == ELLNUMBER) {
			*val =(double) atof(ell_Buffer);
			Token = ell_GetToken(fp);
			if (Token == ELLRIGHTPAREN)
				return 1;
		}
	}
	return 0;

}

int ell_ReadDouble(FILE *fp, double *val1, double *val2)
{
	int Token;
	Token = ell_GetToken(fp);
	if (Token == ELLLEFTPAREN) {
		Token = ell_GetToken(fp);
		if (Token == ELLNUMBER) {
			*val1 = (double)atof(ell_Buffer);
			Token = ell_GetToken(fp);
			if (Token == ELLCOMMA) {
				Token = ell_GetToken(fp);
				if (Token == ELLNUMBER) {
					*val2 = (double)atof(ell_Buffer);
					Token = ell_GetToken(fp);
					if (Token ==ELLRIGHTPAREN)
						return 1;
				}
			}
		}
	}
	return 0;
}

int ell_ReadTripple(FILE *fp, double *val1, double *val2, double *val3)
{
	int Token;
	Token = ell_GetToken(fp);
	if (Token == ELLLEFTPAREN) {
		Token = ell_GetToken(fp);
		if (Token == ELLNUMBER) {
			*val1 =(double) atof(ell_Buffer);
			Token = ell_GetToken(fp);
			if (Token == ELLCOMMA) {
				Token = ell_GetToken(fp);
				if (Token == ELLNUMBER) {
					*val2 =(double) atof(ell_Buffer);
					Token = ell_GetToken(fp);
					if (Token == ELLCOMMA) {
						Token = ell_GetToken(fp);
						if (Token == ELLNUMBER) {
							*val3 = (double)atof(ell_Buffer);
							Token = ell_GetToken(fp);
							if (Token == ELLRIGHTPAREN)
								return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
