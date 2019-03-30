#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ParserTri.h"
#include "Vector.h"

char *tri_KeyWords[] = 
{

	"LOC","AMBIENT","DIFFUSE","SPECULAR","SPECSPREAD",
		"TRANSPARENCY","COLOR","TRIANGLE","V1","V2",
		"NUMLEVELS","H","SCALE","SNOWLINE","ROCKGRADINET"

};

char tri_Buffer[256];
double tri_ka,tri_kd,tri_ks,tri_kr,tri_kt;
double tri_Radius;
int tri_NO;
VECTOR tri_Loc,tri_V1,tri_V2;
TriColor tri_Ia1;

int tri_GetToken(FILE *fp)
{
		char Ch;
	int Ndx,i;
	Ndx=0;
	for(i=0;i<180;i++)
	{
	
		tri_Buffer[i]=0;
	
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
		
			tri_Buffer[Ndx++]=toupper(Ch);
			while((Ch=fgetc(fp))!=EOF &&(isalpha(Ch)||Ch=='_'||isdigit(Ch)||Ch=='.'))
				tri_Buffer[Ndx++]=toupper(Ch);
			tri_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			for(i=0;i<TRINUMKEYWORDS;i++)
			{
			
				if(stricmp(tri_KeyWords[i],tri_Buffer)==0) 
					return i;

			
			}
			return TRIUNKNOWN;
		}
		else if(Ch=='(')
			return TRILEFTPAREN;
		else if(Ch==')')
			return TRIRIGHTPAREN;
		else if(Ch==',')
			return TRICOMMA;
		else if(isdigit(Ch)||Ch=='.'||Ch=='-')
		{
	
			tri_Buffer[Ndx++]=Ch;

			while( ((Ch=fgetc(fp))!=EOF)   && (isdigit(Ch) || Ch=='.'))
				tri_Buffer[Ndx++]=Ch;			
			
			tri_Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			return TRINUMBER;
	
		}
		else if(Ch == '{')
			return TRIOPENCURLYBRACKET;
		
		else if(Ch=='/')//comment
		{
		
			while((Ch=fgetc(fp))!='/' && Ch!=EOF);
		
		}
	
	}

}

int tri_ReadInt(FILE *fp, int *val)
{
	int Token;
	Token = tri_GetToken(fp);
	if (Token == TRILEFTPAREN) {
		Token = tri_GetToken(fp);
		if (Token == TRINUMBER) {
			*val = atoi(tri_Buffer);
			Token = tri_GetToken(fp);
			if (Token == TRIRIGHTPAREN)
				return 1;
		}
	}
	return 0;
}

int tri_ReadFloat(FILE *fp,double *val)
{

	int Token;
	Token = tri_GetToken(fp);
	if (Token == TRILEFTPAREN) {
		Token = tri_GetToken(fp);
		if (Token == TRINUMBER) {
			*val =(double) atof(tri_Buffer);
			Token = tri_GetToken(fp);
			if (Token == TRIRIGHTPAREN)
				return 1;
		}
	}
	return 0;

}

int tri_ReadTripple(FILE *fp, double *val1, double *val2, double *val3)
{
	int Token;
	Token = tri_GetToken(fp);
	if (Token == TRILEFTPAREN) {
		Token = tri_GetToken(fp);
		if (Token == TRINUMBER) {
			*val1 =(double) atof(tri_Buffer);
			Token = tri_GetToken(fp);
			if (Token == TRICOMMA) {
				Token = tri_GetToken(fp);
				if (Token == TRINUMBER) {
					*val2 =(double) atof(tri_Buffer);
					Token = tri_GetToken(fp);
					if (Token == TRICOMMA) {
						Token = tri_GetToken(fp);
						if (Token == TRINUMBER) {
							*val3 = (double)atof(tri_Buffer);
							Token = tri_GetToken(fp);
							if (Token == TRIRIGHTPAREN)
								return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
