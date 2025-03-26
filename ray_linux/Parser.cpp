#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "Trace.h"
#include "Vector.h"
#include "Lights.h"
#include "Objects.h"
//To Add New keywords
//	1.Increment the value for the constant NUMKEYWORDS
//	2.Add the keyword to the keywords array
//	3.Define a value for the token in the #define section
//	4.Add the code to process the keyword

#define NUMKEYWORD 52
#define STRING -1
#define RESOLUTION 0
#define VMODE 1
#define FROM 2
#define UP 3
#define AT 4
#define LITE 5
#define RADIUS 6
#define LOC 7
#define AMBIENT 8
#define DIFFUSE 9
#define SPECULAR 10
#define SPECSPREAD 11
#define LEVELS 12
#define TRANSPARENCY 13
#define COLORVAL 14
#define SPHEREOBJ 15
#define TRIANGLEOBJ 16
#define RECTANGLEOBJ 17
#define CYLINDEROBJ 18
#define ELLIPSOIDOBJ 19
#define BLENCOLORS 20
#define VECTOR1 21
#define VECTOR2 22
#define BACKGROUNDCOLOR 23
#define BEGINGROUPOBJ 24
#define ENDGROUPOBJ 25
#define INCLUDE 26
#define SAVETO 27
#define NUMBEROFFRAMES 28
#define STARTMOVE 29
#define ENDMOVE 30
#define ANTI_ALIASING 31
#define POINTLITE 32
#define DIRECTEDLITE 33
#define CONELITE 34
#define CONEANGLE 35
#define LIGHTFOCUS 36
#define LIGHTDIR 37
#define MAPIMAGE 38
#define CHECKERTEXTURE 39
#define WATERTEXTURE 40
#define CLOUDTEXTURE 41
#define DMINVAL 42
#define DMAXVAL 43
#define LMINVAL 44
#define LMAXVAL 45
#define CHECKERCOLOR 46
#define MARBLETEXTURE 47
#define FOGMINVAL 48
#define FOGMAXVAL 49
#define FOGCOLORVAL 50
#define CLOUDFRACTAL 51
#define LEFTPAREN 100
#define RIGHTPAREN 101
#define COMMA 102
#define NUMBER 103

#define MAXNESTEDGROUPS 10

const char *Keywords[]=
{

	"RESOLUTION","MODE","FROM","UP","AT","LIGHT","RADIUS",
	"LOC","AMBIENT","DIFFUSE","SPECULAR","SPECSPREAD","MAXLEVEL",
	"TRANSPARENCY","COLOR","SPHERE","TRIANGLE","RECTANGLE","CYLINDER",
	"ELLIPSOID","BLEND","V1","V2","BACKGROUND","BEGINGROUP","ENDGROUP",
	"INCLUDE","SAVETO","NUMFRAMES","PATHFROM","PATHTO","ANTIALIASING",
	"POINT","DIRECTED","CONE","SPREADANGLE","SPREADPOWER","LIGHTDIR","MAP","CHECKER",
	"WATER","CLOUD","DMIN","DMAX","LMIN","LMAX","COLOR2","MARBLE","FOGMIN",
	"FOGMAX","FOGCOLOR","CLOUDFRACTAL"

};

#define LITECONEANGLE 0x01
#define LITEDIRECTPOW 0x02
#define LITEDIR 0x04

char Buffer[180];
double ka,kd,ks,kt;
double Radius,T0,T1,T2;
int NO,NumWaves;
VECTOR Loc,V1,V2;
COLOR Ia1,Ia2;
char Filename[80];
PointLight *Light;
VECTOR LightDir;
int Params;
int CheckerMultX,CheckerMultY;
int GroupList[MAXNESTEDGROUPS];
int PrevList[MAXNESTEDGROUPS];
int GroupNdx;
unsigned char BeginGroup;
unsigned char EllipseObj;



int ReadSceneFile(char * filename)
{

	FILE *fp;
	GroupNdx=0;
	PrevList[GroupNdx]=-1;
	GroupList[GroupNdx]=-1;
	fp=fopen(filename,"r");
	if(fp==NULL)
		return 0;

	ProcessFile(fp);
	fclose(fp);
	return 1;

}
int ReadSceneFileInc(char *filename)
{

	FILE *fp;
	fp=fopen(filename,"r");
	if(fp==NULL)
		return 0;

	ProcessFile(fp);
	fclose(fp);
	return 1;

}
int ProcessFile(FILE *fp)
{

	int Token,ObjId,Code,LiteId;
	Object *Obj;

	while((Token=GetToken(fp)) < NUMKEYWORD)
	{
	
		switch(Token)
		{
		
		case AT:
			GetTripple(fp,ANIMATEAT,&At.x,&At.y,&At.z);
			break;	
		case COMMA:
			break;
		case FROM:
			GetTripple(fp,ANIMATEFROM,&From.x,&From.y,&From.z);
			break;
		case UP:
			ReadTripple(fp,&Up.x,&Up.y,&Up.z);
			break;
		case LITE:
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				ReadObjectTerms(fp,ANIMATELIGHTLOC);
				LiteId=AddLight(Light);
				SetLightParams(LiteId);
			
			}
			break;
		case LEFTPAREN:
			break;
		case VMODE:
			ReadInt(fp,&VMode);
			break;
		case NUMBER:
			break;
		case LEVELS:
			ReadInt(fp,&MaxLevel);
			break;
		case RESOLUTION:
			ReadIntPair(fp,&ImWd,&ImHt);
			break;
		case BEGINGROUPOBJ:
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				ReadObjectTerms(fp,ANIMATEOBJLOC);
				if((Obj=new BoundingBoxObj(V1.x,V2.x,V1.x,V2.y,V1.z,V2.z))==NULL)
					return -1;
				if(BeginGroup)
				{
				
					ObjId=AddObject(Obj,-1,GroupList[GroupNdx]);
					PrevList[GroupNdx]=ObjId;
					BeginGroup=0;
				
				}
				else
				{
				
					ObjId=AddObject(Obj,PrevList[GroupNdx],-1);
					PrevList[GroupNdx]=ObjId;
				
				}
				GroupNdx++;
				PrevList[GroupNdx]=ObjId;
				GroupList[GroupNdx]=ObjId;
				BeginGroup=1;
			
			}
			break;
		case ENDGROUPOBJ:
			GroupNdx--;
			BeginGroup=0;
			if(GroupNdx<0) return -1;
			break;

		case SPHEREOBJ:
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				Code=ReadObjectTerms(fp,ANIMATEOBJLOC);
				if((Obj=new SphereObj(Loc.x,Loc.y,Loc.z,Radius))==NULL)
					return -1;
				ObjId=AppendObject(Obj);
				SetObjectParameters(ObjId);
				if(Code==2)
					Animate[NumObjToAnimate-1].Ndx=ObjId;
			
			}
			break;
		case TRIANGLEOBJ:
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				Code=ReadObjectTerms(fp,ANIMATEOBJLOC);
				if((Obj=new TriangleObj(&Loc,&V1,&V2))==NULL)
					return -1;
				ObjId=AppendObject(Obj);
				SetObjectParameters(ObjId);
				if(Code==2)
					Animate[NumObjToAnimate-1].Ndx=ObjId;
			
			}
			break;
		case RECTANGLEOBJ:
			EllipseObj=0;
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				Code=ReadObjectTerms(fp,ANIMATEOBJLOC);
				if(Params==WATERPATTERN)
				{
				
					if((Obj=new WaterObj(&Loc,&V1,&V2))==NULL)
						return -1;	
					if(NumObjToAnimate==0) NumObjToAnimate=1;
				}
				else
				{
					if((Obj=new RectangleObj(&Loc,&V1,&V2))==NULL)
						return -1;															
				}
					ObjId=AppendObject(Obj);
					SetObjectParameters(ObjId);
					if(Code==2)
						Animate[NumObjToAnimate-1].Ndx=ObjId;
			
			}
			break;
		case CYLINDEROBJ:
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				Code=ReadObjectTerms(fp,ANIMATEOBJLOC);
				if((Obj=new CylinderObj(Loc,V1,Radius))==NULL)
					return -1;
				ObjId=AppendObject(Obj);
				SetObjectParameters(ObjId);
				if(Code==2)
					Animate[NumObjToAnimate-1].Ndx=ObjId;
			
			}
			break;
		case ELLIPSOIDOBJ:
			EllipseObj=1;
			Token=GetToken(fp);
			if(Token==LEFTPAREN)
			{
			
				strcpy(Filename,"");
				Code=ReadObjectTerms(fp,ANIMATEOBJLOC);
				if((Obj=new EllipsoidObj(Loc.x,Loc.y,Loc.z,V1.x,V1.y,V1.z,V2.x))==NULL)
					return -1;
				ObjId=AppendObject(Obj);
				SetObjectParameters(ObjId);
				if(Code==2)
					Animate[NumObjToAnimate-1].Ndx=ObjId;
			
			}
			break;

		case BACKGROUNDCOLOR:
			ReadTripple(fp,&Background.r,&Background.g,&Background.b);
			break;
		case INCLUDE:
			ReadString(fp,Filename);
			ReadSceneFileInc(Filename);
			break;
		case SAVETO:
			ReadString(fp,OutFilename);
			SaveTo=1;
			break;
		case NUMBEROFFRAMES:
			ReadInt(fp,&NumFrames);
			break;
		case ANTI_ALIASING:
			Antialiasing=1;
			ReadParenPair(fp);
			break;
		case DMINVAL:
			Readdouble(fp,&DMin);
			break;
		case DMAXVAL:
			Readdouble(fp,&DMax);
			break;
		case LMINVAL:
			Readdouble(fp,&LMin);
			if(LMin<1) LMin=(double)TOL;
			break;
		case LMAXVAL:
			Readdouble(fp,&LMax);
			break;
		case FOGMINVAL:
			ReadInt(fp,&FogMin);
			if(FogMin<0) FogMin=0;
			break;
		case FOGMAXVAL:
			ReadInt(fp,&FogMax);
			if(FogMax<1)FogMax=0;
			break;
		case FOGCOLORVAL:
			ReadTripple(fp,&FogColor.r,&FogColor.g,&FogColor.b);
			break;

		}
	
	}

	return 1;
}

int GetToken(FILE *fp)
{
	char Ch;
	int Ndx,i;
	Ndx=0;
	memset(Buffer, 0, sizeof(Buffer));
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
		
			Buffer[Ndx++]=toupper(Ch);
			while((Ch=fgetc(fp))!=EOF &&(isalpha(Ch)||Ch=='_'||isdigit(Ch)||Ch=='.'))
				Buffer[Ndx++]=toupper(Ch);
			Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			for(i=0;i<NUMKEYWORD;i++)
			{
			
				if(stricmp(Keywords[i],Buffer)==0) 
					return i;
			}	
			return STRING;
		}
		else if(Ch=='(')
			return LEFTPAREN;
		else if(Ch==')')
			return RIGHTPAREN;
		else if(Ch==',')
			return COMMA;
		else if(isdigit(Ch)||Ch=='.'||Ch=='-')
		{

			Buffer[Ndx++]=Ch;
			while( ((Ch=fgetc(fp))!=EOF) && (isdigit(Ch) || Ch=='.'))
				Buffer[Ndx++]=Ch;
			Buffer[Ndx]='\0';
			ungetc(Ch,fp);
			return NUMBER;
	
		}
		
		else if(Ch=='/')//comment
		{
		
			while((Ch=fgetc(fp))!='/' && Ch!=EOF);
		
		}
	
	}

}

int AppendObject(Object *Obj)
{
	int ObjId;
	if (BeginGroup) {
		ObjId = AddObject(Obj, -1, GroupList[GroupNdx]);
		PrevList[GroupNdx] = ObjId;
		BeginGroup = 0;
	}
	else {
		ObjId = AddObject(Obj, PrevList[GroupNdx], -1);
		PrevList[GroupNdx] = ObjId;
	}
	SetObjectParameters(ObjId);
	return ObjId;
}

int ReadString(FILE *fp, char *Str)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == STRING) {
			strcpy(Str, Buffer);
			Token = GetToken(fp);
			if (Token ==RIGHTPAREN)
				return 1;
		}
	}
	return 0;
}

int ReadInt(FILE *fp, int *val)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*val = atoi(Buffer);
			Token = GetToken(fp);
			if (Token ==RIGHTPAREN)
				return 1;
		}
	}
	return 0;
}

int Readdouble(FILE *fp, double *val)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*val =(double) atof(Buffer);
			Token = GetToken(fp);
			if (Token ==RIGHTPAREN)
				return 1;
		}
	}
	return 0;
}


int GetTripple(FILE *fp,int Type,double *Val1,double *Val2,double *Val3)
{

	int Token;
	STEP *S,*R;
	
	Token=GetToken(fp);
	
	if(Token==LEFTPAREN)
	{
	
		Token=GetToken(fp);
		if(Token==NUMBER)
		{
		
			*Val1=(double)atof(Buffer);
			Token=GetToken(fp);
			if(Token==COMMA)
			{
			
				Token=GetToken(fp);
				if(Token==NUMBER)
				{
					*Val2=(double)atof(Buffer);
					Token=GetToken(fp);
					if(Token==COMMA)
					{
					
						Token=GetToken(fp);
						if(Token==NUMBER)
						{
						
							*Val3=(double)atof(Buffer);
							Token=GetToken(fp);
							if(Token==RIGHTPAREN)
								return 1;
						
						}
					
					}
				}

			
			}
		
		}
		else
		{
		
ReadStep:
			if(Token==STARTMOVE)
			{
		
				Animate[NumObjToAnimate].Type=Type;
				S=(struct STEP *)malloc(sizeof(struct STEP));
				if(S==NULL)
					return 0;
				S->Next=NULL;
				if(Animate[NumObjToAnimate].Steps==NULL)
				{
				
					Animate[NumObjToAnimate].Steps=S;
					Animate[NumObjToAnimate].CurrStep=S;
				
				}
				else
				{
				
					R=Animate[NumObjToAnimate].Steps;
					while(R->Next!=NULL) R=R->Next;
					R->Next=S;
				
				}

				ReadQuad(fp,&S->T0,&S->x0,&S->y0,&S->z0);

				*Val1=S->x0;
				*Val2=S->y0;
				*Val3=S->z0;

				Token=GetToken(fp);
				if(Token==COMMA)
				{
				
					Token=GetToken(fp);
					if(Token==ENDMOVE)
					{
					
						ReadQuad(fp,&S->T1,&S->x1,&S->y1,&S->z1);
						Token=GetToken(fp);
						if(Token==RIGHTPAREN)
						{
						
							NumObjToAnimate++;
							return 2;
						
						}
						else if(Token==COMMA)
						{
						
							Token=GetToken(fp);
							goto ReadStep;
						
						}
						else
							return 0;

					
					}
				
				}
		
			}
		
		}
	
	}
	return 0;

}


int ReadTripple(FILE *fp, double *val1, double *val2, double *val3)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*val1 =(double) atof(Buffer);
			Token = GetToken(fp);
			if (Token == COMMA) {
				Token = GetToken(fp);
				if (Token == NUMBER) {
					*val2 =(double) atof(Buffer);
					Token = GetToken(fp);
					if (Token == COMMA) {
						Token = GetToken(fp);
						if (Token == NUMBER) {
							*val3 = (double)atof(Buffer);
							Token = GetToken(fp);
							if (Token ==RIGHTPAREN)
								return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

int ReadQuad(FILE *fp, int *t, double *val1, double *val2, double *val3)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*t = atoi(Buffer);
			Token = GetToken(fp);
			if (Token == COMMA) {
				Token = GetToken(fp);
				if (Token == NUMBER) {
					*val1 = (double)atof(Buffer);
					Token = GetToken(fp);
					if (Token == COMMA) {
						Token = GetToken(fp);
						if (Token == NUMBER) {
							*val2 = (double)atof(Buffer);
							Token = GetToken(fp);
							if (Token == COMMA) {
								Token = GetToken(fp);
								if (Token == NUMBER) {
									*val3 =(double) atof(Buffer);
									Token = GetToken(fp);
									if (Token ==RIGHTPAREN)
										return 1;
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}





int ReadIntPair(FILE *fp, int *val1, int *val2)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*val1 = atoi(Buffer);
			Token = GetToken(fp);
			if (Token == COMMA) {
				Token = GetToken(fp);
				if (Token == NUMBER) {
					*val2 = atoi(Buffer);
					Token = GetToken(fp);
					if (Token ==RIGHTPAREN)
						return 1;
				}
			}
		}
	}
	return 0;
}

int ReaddoublePair(FILE *fp, double *val1, double *val2)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token == NUMBER) {
			*val1 = (double)atof(Buffer);
			Token = GetToken(fp);
			if (Token == COMMA) {
				Token = GetToken(fp);
				if (Token == NUMBER) {
					*val2 = (double)atof(Buffer);
					Token = GetToken(fp);
					if (Token ==RIGHTPAREN)
						return 1;
				}
			}
		}
	}
	return 0;
}

int ReadParenPair(FILE *fp)
{
	int Token;
	Token = GetToken(fp);
	if (Token == LEFTPAREN) {
		Token = GetToken(fp);
		if (Token ==RIGHTPAREN)
			return 1;
	}
	return 0;
}

int ReadObjectTerms(FILE *fp, int Type)
{
	int Token, Code = 1;
	Params = 0;
	strcpy(Filename,"");

	while((Token=GetToken(fp)) != EOF) {
		switch (Token) {
		case RIGHTPAREN:
			return Code;
		case COMMA:
			break;
		case LOC:
			Code = GetTripple(fp, Type, &Loc.x, &Loc.y, &Loc.z);
			break;
		case RADIUS:
			Readdouble(fp, &Radius);
			break;
		case AMBIENT:
			Readdouble(fp, &ka);
			break;
		case DIFFUSE:
			Readdouble(fp, &kd);
			break;
		case SPECULAR:
			Readdouble(fp, &ks);
			break;
		case SPECSPREAD:
			ReadInt(fp, &NO);
			break;
		case TRANSPARENCY:
			Readdouble(fp, &kt);
			break;
		case COLORVAL:
			ReadTripple(fp, &Ia1.r, &Ia1.g, &Ia1.b);
			break;
		case CHECKERCOLOR:
			ReadTripple(fp, &Ia2.r, &Ia2.g, &Ia2.b);
			break;
		case VECTOR1:
			ReadTripple(fp, &V1.x, &V1.y, &V1.z);
			break;
		case VECTOR2:
			ReadTripple(fp, &V2.x, &V2.y, &V2.z);
			break;
		case POINTLITE:
			ReadParenPair(fp);
			Light = new PointLight(&Loc, &Ia1);
			break;
		case DIRECTEDLITE:
			ReadParenPair(fp);
			Light = new DirectedLight(&Loc, &Ia1);
			break;
		case CONELITE:
			ReadParenPair(fp);
			Light = new ConeLight(&Loc, &Ia1);
			break;
		case CONEANGLE:
			Readdouble(fp, &((ConeLight *)Light)->CosGamma);
			Params |= LITECONEANGLE;
			break;
		case LIGHTDIR:
			ReadTripple(fp, &LightDir.x, &LightDir.y, &LightDir.z);
			Params |= LITEDIR;
			break;
		case LIGHTFOCUS:
			ReadInt(fp, &((DirectedLight *)Light)->DirectPow);
			Params |= LITEDIRECTPOW;
			break;
		case BLENCOLORS:
			Readdouble(fp, &HorizonOffset);
			Params |= BLENDPATTERN;
			break;
		case MAPIMAGE:
			ReadString(fp, Filename);
			Params |= IMAGEPATTERN;
			break;
		case CHECKERTEXTURE:
			ReadIntPair(fp, &CheckerMultX, &CheckerMultY);
			Params |= CHECKERPATTERN;
			break;
		case WATERTEXTURE:
			ReaddoublePair(fp, &T0, &T1);
			GetToken(fp);			
			NumWaves=atoi(Buffer);
			Params |= WATERPATTERN;
			break;
		case CLOUDTEXTURE:
			ReaddoublePair(fp, &T0, &T1);
			Params |= CLOUDPATTERN;
			break;
		case CLOUDFRACTAL:
			ReadTripple(fp,&T0,&T1,&T2);
			Params |= FRACTALCLOUD;
			break;
		case MARBLETEXTURE:
			ReadParenPair(fp);
			Params |= MARBLEPATTERN;
			break;
		}
	}
	return Code;
}


int SetObjectParameters(int ObjId)
{
	int Id;

	Objects[ObjId]->ka = ka;
	Objects[ObjId]->kd = kd;
	Objects[ObjId]->ks = ks;
	Objects[ObjId]->NO = NO;
	Objects[ObjId]->kt = kt;
	Objects[ObjId]->Ia1.r = Ia1.r;
	Objects[ObjId]->Ia1.g = Ia1.g;
	Objects[ObjId]->Ia1.b = Ia1.b;
	Objects[ObjId]->Ia2.r = Ia2.r;
	Objects[ObjId]->Ia2.g = Ia2.g;
	Objects[ObjId]->Ia2.b = Ia2.b;

	if (Params & IMAGEPATTERN) 
	{
		Id = AddImage(Filename);
		if (Id >= 0) 
		{
			SetObjectTexture(ObjId, IMAGEPATTERN, Id);
			return 1;
		}
		else 
			return -1;
	}
	else if (Params & CHECKERPATTERN) 
	{
		CheckerMultX |= (CheckerMultY << 8);
		SetObjectTexture(ObjId, CHECKERPATTERN, CheckerMultX);
	}
	else if (Params & WATERPATTERN) 
	{
		((WaterObj *)Objects[ObjId])->Ht = T0;
		((WaterObj *)Objects[ObjId])->Rough = T1;
		((WaterObj *)Objects[ObjId])->NumWaveCenters=NumWaves;
	}
	else if (Params & CLOUDPATTERN) 
	{
		
		SetObjectTexture(ObjId, CLOUDPATTERN, Id=0x0000);
		if (EllipseObj) 
		{
			((EllipsoidObj *)Objects[ObjId])->V1.z = T0;
			((EllipsoidObj *)Objects[ObjId])->V2.z = T1;
		}
		else 
		{
			((RectangleObj *)Objects[ObjId])->T0 = T0;
			((RectangleObj *)Objects[ObjId])->T1 = T1;
		}
	}
	else if (Params & FRACTALCLOUD)
	{
	
		SetObjectTexture(ObjId, FRACTALCLOUD,Id=0x00);
		((EllipsoidObj *)Objects[ObjId])->V1.z = T0;
		((EllipsoidObj *)Objects[ObjId])->V2.z = T1;
		((EllipsoidObj *)Objects[ObjId])->Dim = T2;

	
	}
	else if (Params & MARBLEPATTERN) {
		SetObjectTexture(ObjId, MARBLEPATTERN, Id=0x00);
		((RectangleObj *)Objects[ObjId])->T0 = (double).2;
		((RectangleObj *)Objects[ObjId])->T1 = (double).1;
	}
	else if (Params & BLENDPATTERN) {
		SetObjectTexture(ObjId, BLENDPATTERN, Id=0x00);
	}
	return 1;
}

void SetLightParams(int i)
{
	if (Params & LITEDIRECTPOW) {
		((DirectedLight *)Lights[i])->DirectPow = ((DirectedLight *)Light)->DirectPow;
	}
	if (Params & LITECONEANGLE) {
		((ConeLight *)Lights[i])->SetConeLightAngle(((ConeLight *)Light)->CosGamma);
	}
	if (Params & LIGHTDIR)
		((DirectedLight *)Lights[i])->SetLightDirection(&LightDir);
}







