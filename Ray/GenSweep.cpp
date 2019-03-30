#include "stdafx.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SweepParser.h"
#define MAXNAME 80
#define MAXPOINTS 100
char swp_Outfile[MAXNAME]="Some.sdf\0";
char swp_Infile[MAXNAME]="Some.odf\0";
struct SPOINT Points[MAXPOINTS];
int NumPoints,NumRotInc;
void Usage();
#define TOL 0.0000001f
extern VECTOR swp_Offset;
extern SwpColor swp_Ia1;
extern double swp_ka,swp_kd,swp_kt,swp_ks;
extern int swp_NO;
FILE *swp_Outfp, *swp_Infp;
int ReadPoints(FILE *fp,int *NumPoints,int *NumRotInc,struct SPOINT Points[])
{

	int i;
	fscanf(fp,"%d",NumRotInc);
	fscanf(fp,"%d",NumPoints);
	for(i=0;i<*NumPoints;i++)
		fscanf(fp,"%f %f",&Points[i].x, &Points[i].y);
	return 1;

}

void PrintToFile(FILE *Outfp,VECTOR *Loc,VECTOR *V1,VECTOR *V2)
{

	fprintf(Outfp,"Triangle(Loc(%3.5f,%3.5f,%3.5f), V1(%3.5f,%3.5f,%3.5f), V2(%3.5f, %3.5f, %3.5f), \n",Loc->x + swp_Offset.x,Loc->y + swp_Offset.y,Loc->z + swp_Offset.z,V1->x,V1->y,V1->z,V2->x,V2->y,V2->z);	
	fprintf(Outfp,"Ambient(%3.5f),Diffuse(%3.5f),Specular(%3.5f),SpecSpread(%5d),Transparency(%3.5f),Color(%3.5f,%3.5f,%3.5f))\n",swp_ka,swp_kd,swp_ks,swp_NO,swp_kt,swp_Ia1.r,swp_Ia1.g,swp_Ia1.b);

}

int swp_ReadObjectTerms(FILE *fp)
{

	int Token;
	char Ch;
	while((Token = swp_GetToken(fp))!=EOF)
	{
	
		switch(Token)
		{
		
		case SWPRIGHTPAREN:
			return 1;
		case SWPCOMMA:
			break;
		case SWPOFFSET:
			swp_ReadTripple(fp,&swp_Offset.x,&swp_Offset.y,&swp_Offset.z);
			break;
		case SWPAMBIENT:
			swp_ReadFloat(fp,&swp_ka);
			break;
		case SWPDIFFUSE:
			swp_ReadFloat(fp,&swp_kd);
			break;
		case SWPSPECULAR:
			swp_ReadFloat(fp,&swp_ks);
			break;
		case SWPSPECSPREAD:
			swp_ReadInt(fp,&swp_NO);
			break;
		case SWPTRANSPARENCY:
			swp_ReadFloat(fp,&swp_kt);
			break;
		case SWPCOLORVAL:
			swp_ReadTripple(fp,&swp_Ia1.r,&swp_Ia1.g,&swp_Ia1.b);
			break;					
		case SWPOPENCURLYBRACKET:
			Ch  = fgetc(fp);
			while(!feof(fp))
			{
			
				if((Ch = fgetc(fp))=='}')
				{
				
					fprintf(swp_Outfp,"\n");
					break;
				
				}
				else
					fprintf(swp_Outfp,"%c",Ch);
			
			}
			break;

		default:
			return 0;
		}
	
	}
	return 0;

}

void Sweep(FILE *Outfp,int np,struct SPOINT p[],int nrot)
{

	int i, start, last, rot;
	double radinc;	
	VECTOR Loc,V1,V2,P1,P2,P3;

	p[np].x = p[0].x;
	p[np].y = p[0].y;
	if(p[0].x == 0) start =1;
	else start = 0;

	if(p[np-1].x == 0) last = np -2;
	else last = np-1;
	radinc = 2*3.1416/nrot;
	for(rot =0;rot<nrot;rot ++)
	{
	
		for(i=start;i<last;i++)
		{
		
			Loc.x = p[i].x*cos(rot*radinc);
			Loc.y = -p[i].x*sin(rot*radinc);
			Loc.z = p[i].y;

			P1.x = p[i+1].x*cos(rot*radinc);
			P1.y = -p[i+1].x*sin(rot*radinc);
			P1.z = p[i+1].y;

			P2.x = p[i+1].x*cos((rot+1)*radinc);
			P2.y = -p[i+1].x*sin((rot+1)*radinc);
			P2.z = p[i+1].y;

			P3.x = p[i].x*cos((rot+1)*radinc);
			P3.y = -p[i].x*sin((rot+1)*radinc);
			P3.z = p[i].y;

			V1 = Substract(&P1,&Loc);
			V2 = Substract(&P2,&Loc);
			PrintToFile(Outfp,&Loc,&V1,&V2);
			V1 = Substract(&P3,&Loc);
			PrintToFile(Outfp,&Loc,&V2,&V1);
		
		}
	
	}
	if(p[0].x == 0)
	{
	
		for(i = 0;i<nrot; i++)
		{
		
			Loc.x = p[1].x * cos(i*radinc);
			Loc.y = -p[1].x*sin(i*radinc);
			Loc.z = p[1].y;
			V1.x = -Loc.x;
			V1.y = -Loc.y;
			V1.z = p[0].y - Loc.z;
			V2.x = p[1].x * cos((i+1)*radinc);
			V2.y = -p[1].x*sin((i+1)*radinc);
			V2.z = p[1].y;
			V2 = Substract(&V2,&Loc);
			PrintToFile(Outfp,&Loc,&V1,&V2);

		
		}
	
	}

	if(p[np-1].x==0)
	{
	
		for(i = 0;i<nrot;i++)
		{
		
			Loc.x = p[np-2].x*cos(i*radinc);
			Loc.y = -p[np-2].x*sin(i*radinc);
			Loc.z = p[np-2].y;
			V1.x = p[np-2].x*cos((i+1)*radinc);
			V1.y = -p[np-2].x*sin((i+1)*radinc);
			V1.z = p[np-2].y;
			V1 = Substract(&V1,&Loc);
			V2.x = -Loc.x;
			V2.y = -Loc.y;
			V2.z = p[np-1].y - Loc.z;
			PrintToFile(Outfp,&Loc,&V1,&V2);
		
		}
	
	}

}

int swpParserMain(char *szInfile,char *szOutfile)
{

	strcpy(swp_Infile,szInfile);
	strcpy(swp_Outfile,szOutfile);
	swp_Infp=fopen(swp_Infile,"r");
	if(swp_Infp==NULL)
	{
	
		MessageBox(NULL,"InFile: doesnot exist","Non Exist File name",MB_OK);
		return 0;
	
	}
	swp_Outfp=fopen(swp_Outfile,"w");
	if(swp_Outfp==NULL)
	{
	
		MessageBox(NULL,"InFile: doesnot exist","Non Exist File name",MB_OK);
		return 0;
	
	}

	swp_Ia1.r = swp_Ia1.g = swp_Ia1.b = 1.0;
	swp_Offset.x = swp_Offset.y = swp_Offset.z = 0.0;
	ReadPoints(swp_Infp,&NumPoints,&NumRotInc,Points);
	swp_ReadObjectTerms(swp_Infp);
	Sweep(swp_Outfp,NumPoints,Points,NumRotInc);
	fclose(swp_Outfp);
	fclose(swp_Infp);
	return 0;
}

