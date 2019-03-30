
#ifndef PARSERTRIH
#define PARSERTRIH
#include "stdafx.h"
#include "vector.h"

#define TRINUMKEYWORDS 15
#define TRIUNKNOWN -1
#define TRILOC 0
#define TRIAMBIENT 1
#define TRIDIFFUSE 2
#define TRISPECULAR 3
#define TRISPECSPREAD 4
#define TRITRANSPARENCY 5
#define TRICOLORVAL 6
#define TRITRIANGLEOBJ 7
#define TRIVECTOR1 8
#define TRIVECTOR2 9
#define TRINUMBEROFLEVELS 10
#define TRIFRACTALH 11
#define TRIFRACTALSCALE 12
#define TRISNOWLINE 13
#define TRIROCKGRADIENT 14
#define TRILEFTPAREN 100
#define TRIRIGHTPAREN 101
#define TRICOMMA 102
#define TRINUMBER 103
#define TRIOPENCURLYBRACKET 104
#define TRICLOSECURLYBRACKET 105

typedef struct TriClr
{

	double r,g,b;

}TriColor;
extern char *tri_KeyWords[];
extern double tri_ka,tri_kd,tri_ks,tri_kt;
extern double tri_Radius;
extern int tri_NO;
extern VECTOR tri_Loc,tri_V1,tri_V2;
extern TriColor tri_Ia1;
int tri_GetToken(FILE *fp);
int tri_ReadTripple(FILE *fp,double *val1,double *val2,double *val3);
int tri_ReadInt(FILE *fp,int *val);
int tri_ReadFloat(FILE *fp,double *val);
int triParserMain(char *szInFile, char *szOutFile);
#endif