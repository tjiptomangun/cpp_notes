
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "Vector.h"
#include "ParserEll.h"

#define ELLMAXNAME 80
char ell_Outfile[ELLMAXNAME];
char ell_Infile[ELLMAXNAME];

FILE *ell_Infp,*ell_Outfp;
double ell_T0,ell_T1,ell_T2;
char ell_IsFractal=0;
int ell_Fractal(VECTOR *Loc,VECTOR *V1,VECTOR *V2,int NumLevels,double Amp,double Frek,double Dim);
double ell_NoiseX(double Val,double Amp,double Frek);
double ell_NoiseY(double Val,double Amp,double Frek);
double ell_NoiseZ(double Val,double Amp,double Frek);
void ell_WriteCloud(FILE *fp,VECTOR *Loc,VECTOR *V1,VECTOR *V2);
void ell_WriteFrCloud(FILE *fp,VECTOR *Loc,VECTOR *V1,VECTOR *V2);
void ell_Usage(void);
int ell_ReadObjectTerms(FILE *fp)
{

	int Token;
	char Ch;
	while((Token = ell_GetToken(fp))!=EOF)
	{
		switch(Token)
		{
		
		case ELLRIGHTPAREN:
			return 1;
		case ELLCOMMA:
			break;
		case ELLLOC:
			ell_ReadTripple(fp,&ell_Loc.x,&ell_Loc.y,&ell_Loc.z);
			break;
		case ELLAMBIENT:
			ell_ReadFloat(fp,&ell_ka);
			break;
		case ELLDIFFUSE:
			ell_ReadFloat(fp,&ell_kd);
			break;
		case ELLSPECULAR:
			ell_ReadFloat(fp,&ell_ks);
			break;
		case ELLSPECSPREAD:
			ell_ReadInt(fp,&ell_NO);
			break;
		case ELLTRANSPARENCY:
			ell_ReadFloat(fp,&ell_kt);
			break;
		case ELLCOLORVAL:
			ell_ReadTripple(fp,&ell_Ia1.r,&ell_Ia1.g,&ell_Ia1.b);
			break;
		case ELLVECTOR1:
			ell_ReadTripple(fp,&ell_V1.x,&ell_V1.y,&ell_V1.z);
			break;
		case ELLVECTOR2:
			ell_ReadTripple(fp,&ell_V2.x,&ell_V2.y,&ell_V2.z);
			break;
		case ELLCLOUD:
			ell_ReadDouble(fp,&ell_T0,&ell_T1);
			ell_IsFractal = 0;
			break;
		case ELLCLOUDFRACTAL:
			ell_ReadTripple(fp,&ell_T0,&ell_T1,&ell_T2);
			ell_IsFractal = 1;
			break;
		case ELLNUMBEROFLEVELS:
			ell_ReadInt(fp,&ell_NumLevels);
			break;
		case ELLAMP:
			ell_ReadFloat(fp,&ell_Amp);
			break;
		case ELLFREK:
			ell_ReadFloat(fp,&ell_Frek);
			break;
		case ELLDIM:
			ell_ReadFloat(fp,&ell_Dim);
			break;
		case ELLELLIPSOIDOBJ:
			Token = ell_GetToken(fp);
			if(Token == ELLLEFTPAREN)
			{
			
				if(ell_ReadObjectTerms(fp))
				{
				
					ell_Fractal(&ell_Loc,&ell_V1,&ell_V2,ell_NumLevels,ell_Amp,ell_Frek,ell_Dim);

				
				}
			
			}
			break;
		case ELLOPENCURLYBRACKET:
			Ch  = fgetc(fp);
			while(!feof(fp))
			{
			
				if((Ch = fgetc(fp))=='}')
				{
				
					fprintf(ell_Outfp,"\n");
					return 0;
				
				}
				else
					fprintf(ell_Outfp,"%c",Ch);
			
			}
			break;

		default:
			return 0;
		}
	
	}
	return 0;

}

int ell_Fractal(VECTOR *Loc,VECTOR *V1,VECTOR *V2,int NumLevels,double Amp,double Frek,double Dim)
{

	double LenX,LenY,LenZ,Tx,Ty,Tz,Rx,Ry,Rz,Rad,tempx,tempy,tempz;
	double Tex,Tey,Tez;
	VECTOR tLoc,tV1,tV2;
	if(NumLevels)
	{
		LenX = sqrt(ell_V2.x/ell_V1.x);
		LenY = sqrt(ell_V2.x/ell_V1.y);
		LenZ = sqrt(ell_V2.x/ell_V1.z);
		Rad = sqrt(ell_V2.x);
		Tex=Tx = (Loc->x - ell_Loc.x)/LenX;
		Tey=Ty = (Loc->y - ell_Loc.y)/LenY;
		Tez=Tz = (Loc->z - ell_Loc.z)/LenZ;

		Tx = ell_NoiseX(Tx,Amp,Frek);
		Ty = ell_NoiseY(Ty + Tx,Amp,Frek);
		Tz = ell_NoiseZ(Tz + Ty,Amp,Frek);

		Tx = acos(Tx);
		Ty = acos(Ty);
		Tz = acos(Tz);
	
		Tx = pow(Tx,Dim);
		Ty = pow(Ty,Dim);
		Tz = pow(Tz,Dim);

		tempx = Tx;
		Tx = sin(Tx);
		Rx = cos(tempx);

		tempy = Ty;
		Ty = sin(Ty);
		Ry = cos(tempy);

		tempz = Tz;
		Tz = sin(Tz);
		Rz = cos(tempz);

		tLoc.x = ell_Loc.x + Tx*LenX;
		tLoc.y = ell_Loc.y + Ty*LenY;
		tLoc.z = ell_Loc.z + Tz*LenZ;

		LenX=(LenX*Rx);
		LenY=(LenY*Ry);
		LenZ=(LenZ*Rz); 

		LenX*=LenX;
		LenY*=LenY;
		LenZ*=LenZ;

		tV1.x = V2->x/LenX;
		tV1.y = V2->x/LenY;
		tV1.z = V2->x/LenZ;
		tV2.y = V2->y;
		tV2.z = V2->z;
		tV2.x = ell_V2.x;//*.10;
		if(ell_IsFractal)
		{
			//ell_T2 = ell_T2+sin(sqrt((LenX+LenY+LenZ)/V2->x));
			ell_WriteFrCloud(ell_Outfp,&tLoc,&tV1,&tV2);
	
		}
		else
		{
	
			ell_WriteCloud(ell_Outfp,&tLoc,&tV1,&tV2);
	
		}

		return ell_Fractal(&tLoc,&tV1,&ell_V2,NumLevels-1,Amp,Frek,Dim);
	}
	return 1;



}

double ell_NoiseX(double Val,double Amp,double Frek)
{

	int i,n=10;
	double tPhase,tFrek,tAmp;	
	double retVal;
	tAmp = 0.707*Amp;			//Amplitudo awal
	tFrek =Frek;
	
	tPhase = 1.5708*sin(0.5*Val*tFrek);//Pergeseran fase;
	retVal = tAmp*sin(tFrek*Val+tPhase);
	tFrek*=2;
	tPhase = 1.5708*sin(0.5*Val*tFrek);

	for(i=0;i<n;i++)
	{
	
	
		retVal+=tAmp*sin(tFrek*Val+tPhase);
		tFrek*=2;
		tPhase = 1.5708*sin(Val*tFrek);
		tAmp*=0.707;
	
	}
	double sign=1.0;
	if(retVal<0.0)sign=-1.0;
	retVal = fabs(retVal);
	while (retVal>1.0)
	{
		retVal-=1.0;
	
	}

	return retVal*sign;

}
double ell_NoiseY(double Val,double Amp,double Frek)
{

	int i,n=10;
	double tPhase,tFrek,tAmp;	
	double retVal;
	tAmp = 0.707*Amp;			//Amplitudo awal
	tFrek =Frek;
	
	tPhase = 1.5708*sin(0.5*Val*tFrek );//Pergeseran fase;
	retVal = tAmp*sin(tFrek*Val+tPhase);
	tFrek*=2;
	tPhase = 1.5708*sin(0.5*Val*tFrek);

	for(i=0;i<n;i++)
	{
	
	
		retVal+=tAmp*sin(tFrek*Val+tPhase );
		tFrek*=2;
		tPhase = 1.5708*sin(Val*tFrek);
		tAmp*=0.707;
	
	}
	double sign=1.0;
	if(retVal<0.0)sign=-1.0;
	retVal = fabs(retVal);
	while (retVal>1.0)
	{
		retVal-=1.0;
	
	}

	return retVal*sign;



}
double ell_NoiseZ(double Val,double Amp,double Frek)
{
	int i,n=10;
	double tPhase,tFrek,tAmp;	
	double retVal;
	tAmp = 0.707*Amp;			//Amplitudo awal
	tFrek =Frek;
	
	tPhase = 1.5708*sin(0.5*Val*tFrek );//Pergeseran fase;
	retVal = tAmp*sin(tFrek*Val+tPhase);
	tFrek*=2;
	tPhase = 1.5708*sin(0.5*Val*tFrek);

	for(i=0;i<n;i++)
	{
	
	
		retVal+=tAmp*sin(tFrek*Val+tPhase );
		tFrek*=2;
		tPhase = 1.5708*sin(Val*tFrek);
		tAmp*=0.707;
	
	}
	double sign=1.0;
	if(retVal<0.0)sign=-1.0;
	retVal = fabs(retVal);
	while (retVal>1.0)
	{
		retVal-=1.0;
	
	}

	return retVal*sign;


}

void ell_WriteCloud(FILE *fp,VECTOR *Loc,VECTOR *V1,VECTOR *V2)
{
	
	fprintf(fp,"Ellipsoid(Loc(%5.5f,%5.5f,%5.5f),V1(%5.5f,%5.5f,%5.5f),V2(%5.5f,%5.5f,%5.5f),Ambient(%5.5f),Diffuse(%5.5f),Transparency(%5.5f),Specular(%5.5f),SpecSpread(%d),\
		Color(%5.5f,%5.5f,%5.5f),Cloud(%5.5f,%5.5f))\n",Loc->x,Loc->y,Loc->z,V1->x,V1->y,V1->z,V2->x,V2->y,V2->z,ell_ka,ell_kd,ell_kt,ell_ks,ell_NO,ell_Ia1.r,ell_Ia1.g,ell_Ia1.b,ell_T0,ell_T1);

}
void ell_WriteFrCloud(FILE *fp,VECTOR *Loc,VECTOR *V1,VECTOR *V2)
{
	
		fprintf(fp,"Ellipsoid(Loc(%5.5f,%5.5f,%5.5f),V1(%5.5f,%5.5f,%5.5f),V2(%5.5f,%5.5f,%5.5f),Ambient(%5.5f),Diffuse(%5.5f),Transparency(%5.5f),Specular(%5.5f),SpecSpread(%d),\
		Color(%5.5f,%5.5f,%5.5f),CloudFractal(%5.5f,%5.5f,%5.5f))\n",Loc->x,Loc->y,Loc->z,V1->x,V1->y,V1->z,V2->x,V2->y,V2->z,ell_ka,ell_kd,ell_kt,ell_ks,ell_NO,ell_Ia1.r,ell_Ia1.g,ell_Ia1.b,ell_T0,ell_T1,ell_T2);

}

int ellParserMain(char *szInFile, char *szOutFile)
{

	strcpy(ell_Infile,szInFile);
	strcpy(ell_Outfile,szOutFile);
	ell_Infp = fopen(ell_Infile,"r");
	if(!ell_Infp)
	{
	
		MessageBox(NULL,"InFile: doesnot exist","Non Exist File name",MB_OK);
		return 0;
	
	}

	ell_Outfp= fopen(ell_Outfile,"w");
	if(!ell_Outfp)
	{
	
		MessageBox(NULL,"OutFile:Cannot Create","Error OutFile",MB_OK);
		return 0;
	
	}
	do
	{
	
		ell_ReadObjectTerms(ell_Infp);
	
	}while(!feof(ell_Infp));	
	fclose(ell_Outfp);
	fclose(ell_Infp);
	return 1;
}
