#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "trace.h"
#include "vector.h"
#include "objects.h"
#include "tiff.h"
#include "lights.h"
#pragma warning (disable:4244)
Object *Objects[MAXOBJECTS];
int LastObj;
PointLight *Lights[MAXLIGHTS];
double LMin,LMax,DMin,DMax;
int NumLights;
int FrameNum;
int NumFrames;
char OutFilename[80];
unsigned char Antialiasing;
IMAGE Images[MAXIMAGES];
int NumImages;
int x,y;
VECTOR RightV,UpV;
VECTOR CenterV,Dir;
VECTOR From,At,Up,A1,A2,A3;
double DVal,OffsX,OffsY,OffsZ;
double VuAngle;
COLOR Background;
int ImWd,ImHt;
int VMode;
int MaxLevel;
unsigned char SaveTo;
int FogMin,FogMax;
COLOR FogColor;
double HorizonOffset;
FILE *OutputFile;

struct BYTECOLOR
{

	unsigned char r,g,b;

};

ANIMATE Animate[MAXANIMATEOBJ];
int NumObjToAnimate;

time_t _grphicstime;
#define randomize() srand((unsigned int)time(&_grphicstime))
void Setup(char *Str)
{

	randomize();
	Antialiasing=0;
	ImWd=640;
	ImHt=480;
	LastObj=0;
	NumLights=0;
	NumFrames=1;
	NumObjToAnimate=0;
	NumImages=0;
	MaxLevel=MAXLEVEL;
	strcpy(OutFilename,"");
	SaveTo=0;
	LMin=0;LMax=10;
	DMin=0;DMax=100;
	From.x=15; From.y=15;	From.z=0;
	At.x=0.0;  At.y=0.0;	At.z=0.0;
	Up.x=0.0;  Up.y=0.0;	Up.z=1.0;
	VuAngle=(double)(50.0*0.017453293);
	FogMin=50;
	FogMax=1000;
	FogColor.r=1.0; FogColor.g=1.0;	FogColor.b=1.0;
	HorizonOffset=0;
	if(Str[0]=='\0') return;
	Cleanup();
	ReadSceneFile(Str);


}

void Cleanup()
{

	RemoveLights();
	FreeObjects();
	FreeImages();

}

inline void ClampTo(COLOR *I,double Max)
{

	if(I->r>Max) I->r=Max;
	if(I->g>Max) I->g=Max;
	if(I->b>Max) I->b=Max;

}

void GenImage(HDC hDC)
{

	COLOR I;
	int Red,Green,Blue,sp;
	char Filename[13];
	double UInc,VInc,U,V;
	BYTECOLOR C;
	BYTECOLOR *LastRow;
	BYTECOLOR PrevColor;
	char Buffer[32];
	if(LastObj<=0) return;

	LastRow=new BYTECOLOR[ImWd+1];
	if(LastRow==NULL) return;
	strcpy(Filename,OutFilename);
	SetEye();

	VInc=(double)2.0/(ImHt-1);
	UInc=(double)2.0/(ImWd-1);

	if(NumFrames>1)
	{
		
		OutputFile=fopen(OutFilename,"w");
		itoa(NumFrames,Buffer,10);
		fwrite(Buffer,10,1,OutputFile);
		fclose(OutputFile);
	
	}

	for(FrameNum=0;FrameNum<NumFrames;FrameNum++)
	{
	
		if(SaveTo)
		{
		
			
			if(NumObjToAnimate)
			{
				AppendFrameNumber(Filename,OutFilename,FrameNum);
			}
		
		

			if((OutputFile=fopen(Filename,"wb"))==NULL)
			{
		
				delete LastRow;
				return;
		
			}

			if(!WriteRGBTIFFHeader(OutputFile,(LONG)ImWd,(LONG)ImHt,12000L))
			{
		
				delete LastRow;
				return;
		
			}

	
		}

		RepositionObjects(FrameNum);
		if(Antialiasing)
		{
	
			y=ImHt/2;
			V=((double)y-0.5)*VInc;
			x=-ImWd/2;
			U=((double)x-0.5)*UInc;

			Dir.x=CenterV.x + U*RightV.x + V*UpV.x;
			Dir.y=CenterV.y + U*RightV.y + V*UpV.y;
			Dir.z=CenterV.z + U*RightV.z + V*UpV.z;

			Intensity(1,From,Dir,&I);

			ClampTo(&I,1.0);

			LastRow[0].r=I.r*255;
			LastRow[0].g=I.g*255;
			LastRow[0].b=I.b*255;


			for(x=-ImWd/2,sp=1;x<=ImWd/2;x++,sp++)
			{		
				U=((double)x+0.5)*UInc;
				Dir.x=CenterV.x + U*RightV.x + V*UpV.x;
				Dir.y=CenterV.y + U*RightV.y + V*UpV.y;
				Dir.z=CenterV.z + U*RightV.z + V*UpV.z;
				Intensity(1,From,Dir,&I);

				ClampTo(&I,1.0);

				LastRow[sp].r=I.r*255;
				LastRow[sp].g=I.g*255;
				LastRow[sp].b=I.b*255;
		
			}
	
		}//end antialiasing
		for(y=ImHt/2;y>-ImHt/2;y--)
		{
	
			if(Antialiasing)
			{
		
				V=((double)y+0.5)*VInc;
				x=-ImWd/2;
				U=((double)x-0.5)*UInc;

				Dir.x=CenterV.x + U*RightV.x + V*UpV.x;
				Dir.y=CenterV.y + U*RightV.y + V*UpV.y;
				Dir.z=CenterV.z + U*RightV.z + V*UpV.z;
				Intensity(1,From,Dir,&I);

				ClampTo(&I,1.0);

				PrevColor.r=I.r*255;
				PrevColor.g=I.g*255;
				PrevColor.b=I.b*255;
		
			}//End antialiasing
			else
			V=y*VInc;
			for(x=-ImWd/2,sp=0;x<ImWd/2;x++,sp++)
			{
			
				if(Antialiasing)
					U=((double)x+0.5)*UInc;
				else
					U=x*UInc;

				Dir.x=CenterV.x +U*RightV.x + V*UpV.x;
				Dir.y=CenterV.y +U*RightV.y + V*UpV.y;
				Dir.z=CenterV.z +U*RightV.z + V*UpV.z;
				Intensity(1,From,Dir,&I);
				ClampTo(&I,1.0);
				Red=I.r*255;
				Green=I.g*255;
				Blue=I.b*255;

				if(Antialiasing)
				{
			
					Red=((int)LastRow[sp].r+(int)LastRow[sp+1].r + (int)PrevColor.r + (int)Red)/4.0;

					Green=((int)LastRow[sp].g+(int)LastRow[sp+1].g+(int)PrevColor.g+(int)Green)/4.0;

					Blue=((int)LastRow[sp].b+(int)LastRow[sp+1].b+(int)PrevColor.b+(int)Blue)/4.0;

					if(Red>255) Red=255;
					if(Green>255) Green=255;
					if(Blue>255) Blue=255;

					LastRow[sp].r=PrevColor.r;
					LastRow[sp].g=PrevColor.g;
					LastRow[sp].b=PrevColor.b;

					PrevColor.r=I.r*255.0;
					PrevColor.g=I.g*255.0;
					PrevColor.b=I.b*255,0;				
			
				}
				SetPixel(hDC,x+ImWd/2,-y+ImHt/2,RGB(Red,Green,Blue));

				if(SaveTo)
				{
			
					C.r=Red;C.g=Green;C.b=Blue;
					fwrite(&C.r,1,1,OutputFile);
					fwrite(&C.g,1,1,OutputFile);
					fwrite(&C.b,1,1,OutputFile);
			
				}


			}
	
		}
		if(SaveTo)
		fclose(OutputFile);
	}
//	free (LastRow);

}

void AppendFrameNumber(char *Filename,char *OutFilename,int Frame)
{

	char Buffer[10];
	itoa(Frame,Buffer,10);
	strcpy(Filename,OutFilename);
	strcat(Filename,".");
	strcat(Filename,Buffer);

}

void Usage()
{

	exit(1);

}

void RepositionObjects(int FrameNum)
{

	int i;
	double FInc;
	VECTOR V;

	for(i=0;i<NumObjToAnimate;i++)
	{
	
		if(Animate[i].CurrStep!=NULL && Animate[i].CurrStep->T0<=FrameNum && Animate[i].CurrStep->T1>=FrameNum)
		{
		
			if(Animate[i].CurrStep->T0!=Animate[i].CurrStep->T1)
				FInc=(double)(FrameNum-Animate[i].CurrStep->T0/Animate[i].CurrStep->T1-Animate[i].CurrStep->T0);
			else
				FInc=0;
			V.x=Animate[i].CurrStep->x0+(Animate[i].CurrStep->x1-Animate[i].CurrStep->x0)*FInc;
			V.y=Animate[i].CurrStep->y0+(Animate[i].CurrStep->y1-Animate[i].CurrStep->y0)*FInc;
			V.z=Animate[i].CurrStep->z0+(Animate[i].CurrStep->z1-Animate[i].CurrStep->z0)*FInc;

			switch(Animate[i].Type)
			{
			
				case ANIMATEFROM:
					From.x=V.x;	From.y=V.y;	From.z=V.z;
					SetEye();
					break;

				case ANIMATEAT:
					At.x=V.x;	At.y=V.y;	At.z=V.z;
					SetEye();
					break;

				case ANIMATEOBJLOC:
					Objects[Animate[i].Ndx]->Loc.x=V.x;
					Objects[Animate[i].Ndx]->Loc.y=V.y;
					Objects[Animate[i].Ndx]->Loc.z=V.z;
					break;			
			}
		
		}
		if(NumFrames>1 && Animate[i].CurrStep!=NULL && Animate[i].CurrStep->T1==FrameNum)
		{
		
			Animate[i].CurrStep=Animate[i].CurrStep->Next;
		
		}
	
	}


}


void FreeImages()
{

	int i;
	for(i=0;i<NumImages;i++)
		free(Images[i].Image);

}

int AddImage(char *Filename)
{

	int T,i;
	for(i=0;i<NumImages;i++)
		if(stricmp(Images[i].FileName,Filename)==0)
			return i;

	if(NumImages>=MAXIMAGES-1)
		return -1;

	if(GetImageInfo(Filename,(SHORT *)&Images[NumImages].ImWd,(SHORT *)&Images[NumImages].ImHt,(SHORT*)&T)<=0)
		return -1;
	T&=0x0000FFFF;
	if(T==RGBIMAGE)
		Images[NumImages].Image=(UCHAR *)malloc((long)Images[NumImages].ImWd*(long)Images[NumImages].ImHt*3);

	if(Images[NumImages].Image==NULL)
		return -1;

	Images[NumImages].ImType=T;

	if(!ReadImage(Filename,Images[NumImages].Image))
		return -1;

	strcpy(Images[NumImages].FileName,Filename);
	NumImages++;
	return NumImages-1;

}

int FindClosestIntersection(Object *Objects[],int I,VECTOR *Base,VECTOR *Dir,VECTOR *Q,double *NearestT)
{

	int J,Closest=-1,Tmp;
	double T;

	while(I!=-1)
	{
	
		if(Objects[I]->Shape==BOUNDINGBOX)
		{
		
			if(Objects[I]->ComputeT(Base,Dir)==1)
			{
			
				J=Objects[I]->Group;
				while(J!=-1)
				{
				
					T=*NearestT;
					Tmp=FindClosestIntersection(Objects,J,Base,Dir,Q,&T);
					if(Tmp!=-1)
					{
					
						if(T<*NearestT && T>TOL)
						{
						
							*NearestT=T;
							Closest=Tmp;
						
						}
					
					}
					J=Objects[J]->Group;
				
				}

			
			}
		
		}
		else
		{
		
			T=Objects[I]->ComputeT(Base,Dir);
			if(T<*NearestT && T>TOL)
			{
			
				*NearestT=T;
				Closest=I;
			
			}
		
		}
		I=Objects[I]->Sibling;
	
	}
	if(Closest>=0)
	{
	
		Q->x=Base->x+Dir->x * *NearestT;
		Q->y=Base->y+Dir->y * *NearestT;
		Q->z=Base->z+Dir->z * *NearestT;
	
	}
	return Closest;

}


unsigned char InShadow(Object *Objects[],int Index,VECTOR *Q,VECTOR *Ray)
{

	int I=Index,J;
	double T;

	while(I!=-1)
	{
	
		if(I!=Index)
		{
		
			
			if(Objects[I]->Shape==BOUNDINGBOX)
			{
			
				if((T=Objects[I]->ComputeT(Q,Ray))>TOL && T<1)
				{
				
					J=Objects[I]->Group;
					while(J!=-1)
					{
					
						if(InShadow(Objects,J,Q,Ray))
							return TRUE;
						J=Objects[J]->Group;
					
					}
				
				}
			
			}
			else
			{
			
				T=Objects[I]->ComputeT(Q,Ray);
				if(T>TOL && T<1)
				{
					
					if(Objects[I]->kt<0.1)
						return TRUE;
				
				}
			
			}
		
		}
		I=Objects[I]->Sibling;
	
	}
	return FALSE;

}

COLOR *ComputeLight(int Index,VECTOR *Q,VECTOR *NormalN,VECTOR *From,VECTOR *R,COLOR *I,COLOR *ObjColor,VECTOR *Dir)
{

	VECTOR L;
	VECTOR S;
	double NDotL;
	double CosAlpha;
	Object *Obj;
	double WValue;
	double Dist;
	double DistScale,Temp;
	int i;

	Obj=Objects[Index];
	S=Substract(From,Q);
	Dist=Dot(&S,&S);

	Dist=sqrt(Dist);
	Normalize(&S);

	I->r=0; I->g=0; I->b=0;
	for(i=0;i<NumLights;i++)
	{
	
		L=Substract(&Lights[i]->LFW,Q);
		DistScale=Dot(&L,&L);
		Normalize(&L);

		if(!InShadow(Objects,Index,Q,&L))
		{
		
			DistScale=ScaleLight(sqrt(DistScale));
			NDotL=Dot(&L,NormalN);
			if(NDotL>0)
			{
			
				Temp=Lights[i]->LightFactor(&L)*Obj->kd*NDotL/DistScale;
				I->r+=Temp*Lights[i]->Color.r*ObjColor->r;
				I->g+=Temp*Lights[i]->Color.g*ObjColor->g;
				I->b+=Temp*Lights[i]->Color.b*ObjColor->b;
			
			}

			R->x=-L.x+2*NDotL*NormalN->x;
			R->y=-L.y+2*NDotL*NormalN->y;
			R->z=-L.z+2*NDotL*NormalN->z;
			Normalize(R);
			CosAlpha=-Dot(R,Dir);
			if(CosAlpha>0.0)
			{
				WValue=Lights[i]->W(NDotL,Obj->ks);
				if(WValue>0)
				{
				
					Temp=Lights[i]->LightFactor(&L)*WValue*pow(CosAlpha,Obj->NO)/DistScale;
					I->r+=Temp*Lights[i]->Color.r;
					I->g+=Temp*Lights[i]->Color.g;
					I->b+=Temp*Lights[i]->Color.b;
				
				}
			
			}
		}
		
	}
	I->r+=Obj->ka*ObjColor->r;
	I->g+=Obj->ka*ObjColor->g;
	I->b+=Obj->ka*ObjColor->b;

	AddFog(I,Dist);
	ClampTo(I,1.0);
	return I;

}

inline void AddFog(COLOR *I,double Dist)
{

	double LightScale=1.0;
	if(FogMax>0)
	{
	
		if(Dist>FogMin)
		{
		
			if(Dist<FogMax)
			{
				LightScale=(Dist-FogMin)/(FogMax-FogMin);
				I->r=FogColor.r*LightScale + I->r*(1-LightScale);
				I->g=FogColor.g*LightScale + I->g*(1-LightScale);
				I->b=FogColor.b*LightScale + I->b*(1-LightScale);

			}
		
		}
	
	}

}

void Transparency(int Level,int Index,VECTOR *Q,VECTOR *Dir,COLOR *Iq)
{

	COLOR I2;
	double kt;
	VECTOR Dir2,Q2;

	kt=Objects[Index]->kt;
	Q2.x=Q->x;
	Q2.y=Q->y;
	Q2.z=Q->z;

	Dir2.x=Dir->x;
	Dir2.y=Dir->y;
	Dir2.z=Dir->z;

	Intensity(Level,Q2,Dir2,&I2);

	Iq->r=(1-kt)*Iq->r+kt*I2.r;
	Iq->g=(1-kt)*Iq->g+kt*I2.g;
	Iq->b=(1-kt)*Iq->b+kt*I2.b;

}

void Intensity(int Level,VECTOR Base,VECTOR Dir,COLOR *I)
{

	int Index;
	COLOR Iq,Ir,ObjColor;
	VECTOR Q,N,R;
	double DotProd;
	double T=3.4e38;

	if(Level<=MaxLevel)
	{
	
		Index=FindClosestIntersection(Objects,0,&Base,&Dir,&Q,&T);
		if(Index>=0)
		{
		
			Objects[Index]->ComputeNormal(&Q,&N);
			if(Dot(&N,&Dir)>0)
			{
			
				N.x=-N.x;
				N.y=-N.y;
				N.z=-N.z;
			
			}
			ObjColor=(*Objects[Index]->DeterminePattern)(Objects[Index],&Q);
			ComputeLight(Index,&Q,&N,&Base,&R,&Iq,&ObjColor,&Dir);
			if(Objects[Index]->kt>TOL)
				Transparency(Level,Index,&Q,&Dir,&Iq);

			DotProd=-Dot(&Dir,&N);
			R.x=Dir.x+2*DotProd*N.x;
			R.y=Dir.y+2*DotProd*N.y;
			R.z=Dir.z+2*DotProd*N.z;

			if(((Level+1)<=MaxLevel)&&(Objects[Index]->ks>TOL))
			{
			
				Intensity(Level+1,Q,R,&Ir);												
					I->r=Iq.r+Objects[Index]->ks*Ir.r;
					I->g=Iq.g+Objects[Index]->ks*Ir.g;
					I->b=Iq.b+Objects[Index]->ks*Ir.b;				
			
			}
			else
			{
			
				I->r=Iq.r;
				I->g=Iq.g;
				I->b=Iq.b;
			
			}
		
		}
		else
		{
		
			I->r=Background.r;
			I->g=Background.g;
			I->b=Background.b;
		
		}
	}

}
//This Intensity is not yet clear
void SetEye()
{

	A3=Substract(&At,&From); //Orthonormal1
	A1=Cross(&A3,&Up);		 //Orthonormal2
	A2=Cross(&A1,&A3);       //Orthonormal3
	Normalize(&A1);
	Normalize(&A2);
	Normalize(&A3);

	RightV.x=A1.x;
	RightV.y=A1.y;
	RightV.z=A1.z;

	UpV.x=A2.x;
	UpV.y=A2.y;
	UpV.z=A2.z;

	DVal=cos(VuAngle/2.0)/sin(VuAngle/2.0);
	CenterV.x=A3.x*DVal;
	CenterV.y=A3.y*DVal;
	CenterV.z=A3.z*DVal;

	OffsX=-A1.x*From.x-A1.y*From.y-A1.z*From.z;
	OffsY=-A2.x*From.x-A2.y*From.y-A2.z*From.z;
	OffsZ=-A3.x*From.x-A3.y*From.y-A3.z*From.z;

}

void WorldToScreen(VECTOR& V,double& X, double& Y)
{

	VECTOR NewV;
	NewV.x=(V.x*A1.x+V.y*A1.y+V.z*A1.z+OffsX)*DVal;
	NewV.y=(V.x*A2.x+V.y*A2.y+V.z*A2.z+OffsY)*DVal;
	NewV.z=(V.x*A3.x+V.y*A3.y+V.z*A3.z+OffsZ);

	if(NewV.z!=0.0)
	{
	
		X=ImWd/2*NewV.x/NewV.z;
		Y=ImHt/2*NewV.y/NewV.z;
	
	}
	else
	{
	
		X=ImWd/2*NewV.x;
		Y=ImHt/2*NewV.y;
	
	}

}