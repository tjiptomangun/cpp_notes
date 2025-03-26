#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "Vector.h"
#include "ParserTri.h"

#define MAXNAME 80

char tri_Outfile[MAXNAME];
char tri_Infile[MAXNAME];
int tri_MaxLevel = 2;
double tri_Std,tri_Ratio;
double tri_H = .3;
double tri_Scale = 2;
double tri_SnowLine = 3.4e38;
double tri_RockGradient = 3.4e38;
FILE *tri_Infp,*tri_Outfp;
TriColor tri_Rock = {.7,.6,.6};
unsigned int tri_NumObjects = 0;
double tri_Random(double tX,double tY,double tZ);

struct ATRIANGLE
{

	VECTOR Loc,V1,V2;

};
void tri_Usage(void);
void tri_Fractal(VECTOR *Loc,VECTOR *V1,VECTOR *V2,int Iteration,double Std,double Ratio);
double tri_Gauss(unsigned int Seed);
void tri_MakeBoundingBox(FILE *Outfp,VECTOR *Loc,VECTOR *V1,VECTOR *V2,double Std);
double tri_GetMaxGradient(VECTOR *V1,VECTOR *V2);
int tri_ReadObjectTerms(FILE *fp);
double tri_MaxOf(double A,double B,double C);
double tri_MinOf(double A,double B,double C);
int triParserMain(char *szInfile, char *szOutfile)
{


	strcpy(tri_Infile,szInfile);
	strcpy(tri_Outfile,szOutfile);
	tri_Infp=fopen(tri_Infile,"r");
	if(tri_Infp ==0)
	{
	
		fprintf(stderr,"InFile: doesnot exist\n");
		return 0;
	
	}
	tri_Outfp=fopen(tri_Outfile,"w");
	if(tri_Outfp ==0)
	{
		
		fprintf(stderr,"OutFile:Cannot Create\n");
		return 0;
	
	}
	tri_Ratio = 1.0/pow(2.0,tri_H);
	tri_Std = tri_Scale *tri_Ratio;
	tri_Ia1.r = tri_Ia1.g = tri_Ia1.b = 0;
	tri_ka = .6; tri_kd = 1.0; tri_ks = .0; tri_kt = .0; tri_NO = 4;
	do
	{
	
		tri_ReadObjectTerms(tri_Infp);
	
	}while(!feof(tri_Infp));
	printf("Created %u objects\n",tri_NumObjects);
	fclose(tri_Outfp);
	fclose(tri_Infp);
	return 0;

}



void tri_Fractal(VECTOR *Loc,VECTOR *V1,VECTOR *V2,int Level,double Std,double Ratio)
{
	
	struct ATRIANGLE Triangles[4];
	int Tri0 = 0,Tri1 =1,Tri2 = 2,Tri3=3,Tri;
	VECTOR Mid01,Mid12,Mid02;
	
	struct ATRIANGLE *Obj;

	if(!Level) return;
	Triangles[Tri0].Loc = *Loc;
	Triangles[Tri1].Loc = Add(Loc,V1);
	Triangles[Tri2].Loc = Add(Loc,V2);

	Mid01.x = Loc->x+V1->x/2.0;
	Mid01.y = Loc->y+V1->y/2.0;
	Mid01.z = Loc->z+V1->z/2.0;

	Mid02.x = Loc->x+V2->x/2.0;
	Mid02.y = Loc->y+V2->y/2.0;
	Mid02.z = Loc->z+V2->z/2.0;

	Mid12.x = Triangles[Tri1].Loc.x + (V2->x - V1->x)/2.0;
	Mid12.y = Triangles[Tri1].Loc.y + (V2->y - V1->y)/2.0;
	Mid12.z = Triangles[Tri1].Loc.z + (V2->z - V1->z)/2.0;
	
	if(Std == .00) Std = 0.000000001;
	/*Seed = (Mid12.x+Mid12.y+Mid12.z)/Std;
	Seed = 1/pow(2,fabs(Seed));
	Seed = sin(Seed);*/	
	Mid12.z+=(Std+tri_Random(Mid12.x,Mid12.y,Mid12.z)*tri_Scale);

	/*Seed = (Mid01.x+Mid01.y+Mid01.z)/Std;
	Seed = 1/pow(2.0,fabs(Seed));
	Seed = sin(Seed);	*/
	Mid01.z+=(Std+tri_Random(Mid01.x,Mid01.y,Mid01.z)*tri_Scale);

	/*Seed = (Mid02.x+Mid02.y+Mid02.z)/Std;
	Seed = 1/pow(2.0,fabs(Seed));
	Seed = sin(Seed);	*/
	Mid02.z+=(Std+tri_Random(Mid02.x,Mid02.y,Mid02.z)*tri_Scale);


	Triangles[Tri0].V1 = Substract(&Mid01,&Triangles[Tri0].Loc);
	Triangles[Tri0].V2 = Substract(&Mid02,&Triangles[Tri0].Loc);

	Triangles[Tri1].V1 = Substract(&Mid12,&Triangles[Tri1].Loc);
	Triangles[Tri1].V2 = Substract(&Mid01,&Triangles[Tri1].Loc);

	Triangles[Tri2].V1 = Substract(&Mid02,&Triangles[Tri2].Loc);
	Triangles[Tri2].V2 = Substract(&Mid12,&Triangles[Tri2].Loc);

	Triangles[Tri3].Loc = Mid01;
	Triangles[Tri3].V1 = Substract(&Mid12,&Mid01);
	Triangles[Tri3].V2 = Substract(&Mid02,&Mid01);

	Level --;

	for(Tri = Tri0;Tri<=Tri3;Tri++)
	{
	
		Obj = &Triangles[Tri];		
		if(!Level)
		{
		
			fprintf(tri_Outfp,"Triangle(Loc(%3.5f,%3.5f,%3.5f),",Obj->Loc.x,Obj->Loc.y,Obj->Loc.z);
			fprintf(tri_Outfp,"V1(%3.5f,%3.5f,%3.5f),",Obj->V1.x,Obj->V1.y,Obj->V1.z);
			fprintf(tri_Outfp,"V2(%3.5f,%3.5f,%3.5f),",Obj->V2.x,Obj->V2.y,Obj->V2.z);
			fprintf(tri_Outfp,"Ambient(%3.5f),Diffuse(%3.5f),Specular(%3.5f),SpecSpread(%d),Transparency(%3.5f),",tri_ka,tri_kd,tri_ks,tri_NO,tri_kt);
			if(Obj->Loc.z>tri_SnowLine)
			{
				fprintf(tri_Outfp,"Color(.8,.8,.8))\n");
			
			}	
			else if(tri_GetMaxGradient(&Obj->V1,&Obj->V2) >tri_RockGradient)
			{
			
				fprintf(tri_Outfp,"Color(%3.5f,%3.5f,%3.5f))\n",tri_Rock.r,tri_Rock.g,tri_Rock.b);
				tri_NumObjects ++;

			}
			else
			{
			
				fprintf(tri_Outfp,"Color(%3.5f,%3.5f,%3.5f))\n",tri_Ia1.r,tri_Ia1.g,tri_Ia1.b);
			
			}
			tri_NumObjects ++;
					
		}
		if(Level)		
			tri_Fractal(&Obj->Loc,&Obj->V1,&Obj->V2,Level,Std*Ratio,Ratio);
			
	}

}

#define GAUSSADD 12
#define GAUSSSHIFT 6

double tri_Gauss(unsigned int Seed)
{

	double Sum = 0;
	int i;
	srand(Seed);
	for (i=0;i<GAUSSADD;i++)
		Sum+=rand()/(double)RAND_MAX;
	return Sum - GAUSSSHIFT;

}

void tri_MakeBoundingBox(FILE *Outfp,VECTOR *Loc,VECTOR *V1,VECTOR *V2,double Std)
{

	double MinX,MaxX,MinY,MaxY,MinZ,MaxZ;
	VECTOR LV1,LV2;

	LV1 = Add(Loc,V1);
	LV2 = Add(Loc,V2);

	MinX = tri_MinOf(Loc->x,LV1.x,LV2.x);
	MaxX = tri_MaxOf(Loc->x,LV1.x,LV2.x);
	MinY = tri_MinOf(Loc->y,LV1.y,LV2.y);
	MaxY = tri_MaxOf(Loc->y,LV1.y,LV2.y);
	MinZ = tri_MinOf(Loc->z,LV1.z,LV2.z);
	MaxZ = tri_MaxOf(Loc->z,LV1.z,LV2.z);

	MinZ-=Std;
	MaxZ+=Std;

	tri_NumObjects ++;

}

double tri_GetMaxGradient(VECTOR *V1,VECTOR *V2)
{

	double DeltaZ1,DeltaZ2,DeltaZ3;
	DeltaZ1 = fabs(V1->z);
	DeltaZ2 = fabs(V2->z);
	DeltaZ3 = fabs(V1->z - V2->z);
	return tri_MaxOf(DeltaZ1,DeltaZ2,DeltaZ3);

}

int tri_ReadObjectTerms(FILE *fp)
{

	int Token;
	char Ch;
	while((Token = tri_GetToken(fp))!=EOF)
	{
	
		switch(Token)
		{
		
		case TRIRIGHTPAREN:
			return 1;
		case TRICOMMA:
			break;
		case TRILOC:
			tri_ReadTripple(fp,&tri_Loc.x,&tri_Loc.y,&tri_Loc.z);
			break;
		case TRIAMBIENT:
			tri_ReadFloat(fp,&tri_ka);
			break;
		case TRIDIFFUSE:
			tri_ReadFloat(fp,&tri_kd);
			break;
		case TRISPECULAR:
			tri_ReadFloat(fp,&tri_ks);
			break;
		case TRISPECSPREAD:
			tri_ReadInt(fp,&tri_NO);
			break;
		case TRITRANSPARENCY:
			tri_ReadFloat(fp,&tri_kt);
			break;
		case TRICOLORVAL:
			tri_ReadTripple(fp,&tri_Ia1.r,&tri_Ia1.g,&tri_Ia1.b);
			break;
		case TRIVECTOR1:
			tri_ReadTripple(fp,&tri_V1.x,&tri_V1.y,&tri_V1.z);
			break;
		case TRIVECTOR2:
			tri_ReadTripple(fp,&tri_V2.x,&tri_V2.y,&tri_V2.z);
			break;
		case TRIFRACTALH:
			tri_ReadFloat(fp,&tri_H);
			tri_Ratio = 1.0/pow(2.0,tri_H);
			tri_Std = tri_Scale*tri_Ratio;
			break;
		case TRIFRACTALSCALE:
			tri_ReadFloat(fp,&tri_Scale);
			tri_Std = tri_Scale*tri_Ratio;
			break;
		case TRINUMBEROFLEVELS:
			tri_ReadInt(fp,&tri_MaxLevel);
			break;
		case TRITRIANGLEOBJ:
			Token = tri_GetToken(fp);
			if(Token == TRILEFTPAREN)
			{
			
				if(tri_ReadObjectTerms(fp))
				{
				
					tri_Fractal(&tri_Loc,&tri_V1,&tri_V2,tri_MaxLevel,tri_Std,tri_Ratio);
				
				}
			
			}
			break;
		case TRIOPENCURLYBRACKET:
			Ch  = fgetc(fp);
			while(!feof(fp))
			{
			
				if((Ch = fgetc(fp))=='}')
				{
				
					fprintf(tri_Outfp,"\n");
					break;
				
				}
				else
					fprintf(tri_Outfp,"%c",Ch);
			
			}
			break;
		case TRISNOWLINE:
			tri_ReadFloat(fp,&tri_SnowLine);
			break;
		case TRIROCKGRADIENT:
			tri_ReadFloat(fp,&tri_RockGradient);
			break;
		default:
			return 0;
		}
	
	}
	return 0;

}

double tri_MaxOf(double A,double B, double C)
{

	A = (B>A)?B:A;
	return(C>A)?C:A;

}

double tri_MinOf(double A,double B,double C)
{

	A = (B<A)?B:A;
	return (C<A)?C:A;

}

#define TRIRAND_MAX 0x5fb5dd
#define TRIRAND_MULTIPLIER 0x58445
#define TRIRAND_SUBSTRACTOR 0x375d9

double tri_Random(double tX,double tY,double tZ)
{

	unsigned int vX,vY,vZ,T,Tz=TRIRAND_MAX;
	vX = (unsigned int)(tX*1000.0);
	vY = (unsigned int)(tY*100.0);
	vZ = (unsigned int)(tZ*10.0);
	T = vX+vY+vZ;
	T = (T*TRIRAND_MULTIPLIER - TRIRAND_SUBSTRACTOR)%TRIRAND_MAX;
	return ((double)T/(double)Tz - 0.5);
	

}
