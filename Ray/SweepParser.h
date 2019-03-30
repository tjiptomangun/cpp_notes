#ifndef SWEEPPARSERH
#define SWEEPPARSERH

#include "Vector.h"	

#define SWPNUMKEYWORDS 7
#define SWPUNKNOWN -1
#define SWPOFFSET 0
#define SWPAMBIENT 1
#define SWPDIFFUSE 2
#define SWPSPECULAR 3
#define SWPSPECSPREAD 4
#define SWPTRANSPARENCY 5
#define SWPCOLORVAL 6
#define SWPLEFTPAREN 100
#define SWPRIGHTPAREN 101
#define SWPCOMMA 102
#define SWPNUMBER 103
#define SWPOPENCURLYBRACKET 104
#define SWPCLOSECURLYBRACKET 105
#include "stdafx.h"
typedef struct SwpClr
{

	double r,g,b;

}SwpColor;
struct SPOINT
{

	float x,y;

};


extern char *swp_KeyWords[7];
extern double swp_ka,swp_kd,swp_kt,swp_ks;
extern int swp_NO;
extern VECTOR swp_offset;
extern SwpColor swp_clr;
int swp_GetToken(FILE *fp);
int swp_ReadFloat(FILE *fp,double *val);
int swp_ReadDouble(FILE *fp,double *val1,double *val2);
int swp_ReadTripple(FILE *fp,double *val1,double *val2,double *val3);
int swp_ReadInt(FILE *fp,int *val);
int swpParserMain(char *szInFile, char *szOutFile);
extern VECTOR swp_Offset;
extern SwpColor swp_Ia1;
extern FILE *swp_Outfp;
void Sweep(FILE *Outfp,int np,struct SPOINT p[],int nrot);
int ReadPoints(FILE *fp,int *NumPoints,int *NumRotInc,struct SPOINT Points[]);

#endif