#ifndef PARSERELLH
#define PARSERELLH

#include "Vector.h"

#define ELLNUMKEYWORDS 16
#define ELLUNKNOWN -1
#define ELLLOC 0
#define ELLAMBIENT 1
#define ELLDIFFUSE 2
#define ELLSPECULAR 3
#define ELLSPECSPREAD 4
#define ELLTRANSPARENCY 5
#define ELLCOLORVAL 6
#define ELLELLIPSOIDOBJ 7
#define ELLVECTOR1 8
#define ELLVECTOR2 9
#define ELLCLOUD 10
#define ELLCLOUDFRACTAL 11
#define ELLNUMBEROFLEVELS 12
#define ELLAMP 13
#define ELLFREK 14
#define ELLDIM 15
#define ELLLEFTPAREN 100
#define ELLRIGHTPAREN 101
#define ELLCOMMA 102
#define ELLNUMBER 103
#define ELLOPENCURLYBRACKET 104
#define ELLCLOSECURLYBRACKET 105

typedef struct EllClr
{

	double r,g,b;

}EllColor;
extern const char *ell_KeyWords[16];
extern double ell_ka,ell_kd,ell_ks,ell_kt;
extern int ell_NO;
extern VECTOR ell_Loc,ell_V1,ell_V2;
extern int ell_NumLevels;
extern EllColor ell_Ia1;
extern double ell_Amp,ell_Frek,ell_Dim;
int ell_GetToken(FILE *fp);
int ell_ReadFloat(FILE *fp,double *val);
int ell_ReadDouble(FILE *fp,double *val1,double *val2);
int ell_ReadTripple(FILE *fp,double *val1,double *val2,double *val3);
int ell_ReadInt(FILE *fp,int *val);
int ellParserMain(char *szInFile, char *szOutFile);
#endif
