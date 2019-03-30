#ifndef TRACEH
#define TRACEH
#include "stdafx.h"
#include <time.h>
#include "Vector.h"
#include "Objects.h"
#include "windows.h"

#define TRUE 1
#define FALSE 0
#define MAXOBJECTS 11000
#define MAXLIGHTS 5
#define MAXLEVEL 1000
#define TOL 0.0001

extern int x,y;
extern Object *Objects[MAXOBJECTS];
extern int LastObj;
extern int PrevObj;
extern int GroupObj;

#define MAXIMAGES 4
#define GRAYSCALE 1
#define RGBIMAGE 2

struct IMAGE
{

	char FileName[13];
	int ImWd,ImHt;
	unsigned char ImType;
	unsigned char *Image;
};

extern IMAGE Images[MAXIMAGES];
extern int NumImages;

extern VECTOR From,At,Up;
extern COLOR Background;
extern int ImWd,ImHt;
extern char OutFilename[];
extern unsigned char SaveTo;
extern unsigned char Antialiasing;
extern int VMode;
extern int MaxLevel;
extern int FogMin,FogMax;
extern COLOR FogColor;
extern double HorizonOffset;
int ReadString(FILE *fp,char *Str);
int ReadInt(FILE *fp,int *val);
int ReadIntPair(FILE *fp,int *val1,int *val2);
int Readdouble(FILE *fp,double *val);
int ReaddoublePair(FILE *fp,double *val1,double *val2);
int GetTripple(FILE *fp,int Type,double *val1,double *val2,double *val3);
int ReadTripple(FILE *fp,double *val1,double *val2,double *val3);
int ReadQuad(FILE *fp,int *t,double *val1,double *val2,double *val3);
int ReadParenPair(FILE *fp);
int ReadSceneFile(char *filename);
int ReadSceneFileInc(char *filename);
int ReadObjectTerms(FILE *fp,int type);
int SetObjectParameters(int ObjId);
int ProcessFile(FILE *fp);
int GetToken(FILE *fp);
void SetLightParams(int i);
int AppendObject(Object *Obj);
struct STEP
{

	int T0,T1;
	double x0,y0,z0;
	double x1,y1,z1;
	STEP *Next;

};

#define ANIMATEOBJLOC 1
#define ANIMATEFROM 2
#define ANIMATEAT 3
#define ANIMATELIGHTLOC 4
#define MAXANIMATEOBJ 10

struct ANIMATE
{

	unsigned char Type;
	int Ndx;
	STEP *Steps;
	STEP *CurrStep;

};

extern ANIMATE Animate[MAXANIMATEOBJ];
extern int NumObjToAnimate;
extern int FrameNum;
extern int NumFrames;

void InitializeObjects();
int FindClosestIntersection(Object *Objects[],VECTOR *Base,VECTOR *Dir,VECTOR *Q,double NearestT);
unsigned char InShadow(Object *Objects[],int Index,VECTOR *Q,VECTOR *LRay);
COLOR *ComputeLight(int Index,VECTOR *Q,VECTOR NormalN,VECTOR *From,VECTOR *R,COLOR *I,COLOR *ObjColor,VECTOR *Dir);
void Transparency(int Level,VECTOR Base,VECTOR Dir,COLOR *I);
void Intensity(int Level,VECTOR Base,VECTOR Dir,COLOR *I);
void SetEye();
void Setup(char *Str);
void GenImage(HDC hDC);
void RepositionObjects(int FrameNum);
int AddImage(char *FileName);
void FreeImages();
void WorldToScreen(VECTOR& V,double& X,double& Y);
inline void AddFog(COLOR *I,double Dist);
void Cleanup();
void AppendFrameNumber(char *FileName,char *OutFileName,int Frame);
#endif

