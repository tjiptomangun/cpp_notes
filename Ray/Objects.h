#ifndef OBJECTSH
#define OBJECTSH
#include "stdafx.h"
#include "Vector.h"
#define SPHERE    1
#define RECTANGLE 2
#define ELLIPSOID 3
#define TRIANGLE 4
#define CYLINDER  5	
#define BOUNDINGBOX 98
#define BOUNDINGSPHERE 99

#define CHECKERPATTERN 0x01
#define IMAGEPATTERN 0x02
#define WATERPATTERN 0x04
#define CLOUDPATTERN 0x08
#define MARBLEPATTERN 0x10
#define BLENDPATTERN 0x20
#define FRACTALCLOUD 0x40

struct COLOR
{

	double r,g,b;

};

//Base class of all objects
class Object
{

public:
	unsigned int Shape;
	int ImageNdx;
	int Sibling,Group;	//Indexes of sibling and groupped object
	double MaxT0;
	double ka,kd,ks,kt;	//Constant for lighting model
	int NO;
	COLOR Ia1,Ia2;		
	VECTOR Loc;
	Object();
	virtual double ComputeT(VECTOR *from,VECTOR *dir){return 0;}
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N){}
	virtual void MapToUV(VECTOR *Q,double& U,double& V){}
	COLOR (*DeterminePattern)(Object *Obj,VECTOR *Q);

};

class RectangleObj:public Object
{

public:
	VECTOR Normal;
	double A,B,C,D;
	VECTOR V1,V2;
	double Len1,Len2;
	double MinX,MaxX,MinY,MaxY,MinZ,MaxZ;
	double T0,T1;
	RectangleObj(VECTOR *LocV,VECTOR *Vect1,VECTOR *Vect2);
	virtual double ComputeT(VECTOR *From,VECTOR *Dir);
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N);
	virtual void MapToUV(VECTOR *Q,double& U,double& V);

};

class TriangleObj:public RectangleObj
{

public:
	TriangleObj(VECTOR *LocV,VECTOR *V1,VECTOR *V2);
	virtual double ComputeT(VECTOR *From,VECTOR *Dir);

};

class BoundingBoxObj:public Object
{

public:
	double MinX,MaxX,MinY,MaxY,MinZ,MaxZ;
	BoundingBoxObj(double XMin,double XMax,double YMin,double YMax,double ZMin,double ZMax);
	virtual double ComputeT(VECTOR *From,VECTOR *Dir);

};

class SphereObj:public Object
{

public:
	double RSquared;
	SphereObj(double X,double Y,double Z,double R);
	virtual double ComputeT(VECTOR * From,VECTOR *Dir);
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N);
	virtual void MapToUV(VECTOR *Q,double& U,double& V);

};

class EllipsoidObj:public SphereObj
{

public:
	double A,B,C,D,Dim;
	VECTOR V1,V2;
	/*
	
	V1.z = Amplitude 
	V2.z = Frequency
	Dim = Dimension
	*/
	EllipsoidObj(double X,double Y,double Z,double AVal,double BVal,double CVal,double DVal);
	virtual double ComputeT(VECTOR *From,VECTOR *Dir);
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N);
};

class CylinderObj:public SphereObj
{

public:
	double Radius,Height;
	VECTOR V1;				//Height Vector
	VECTOR NV1;
	VECTOR NX,NZ;
	VECTOR Ref;
	double NHeight;
	CylinderObj(VECTOR LocV,VECTOR Vect1,double R);
	virtual double ComputeT(VECTOR * From,VECTOR *Dir);
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N);
	virtual void MapToUV(VECTOR *Q,double& U,double& V);	

};

class WaterObj:public RectangleObj
{

public:
	double Ht;
	double Rough;	//controls roughness of the water
	int NumWaveCenters;
	WaterObj::WaterObj(VECTOR *LocV,VECTOR *Vect1,VECTOR *Vect2):
	RectangleObj(LocV,Vect1,Vect2){}
	virtual void ComputeNormal(VECTOR *Q,VECTOR *N);

};

int AddObject(Object *Obj,int Prev,int Group);
void FreeObjects();
COLOR PatterOnSurface(Object * Obj,VECTOR *Q);
COLOR Blend(Object * Obj,VECTOR *Q);
COLOR MapImage(Object *Obj,VECTOR *Q);
COLOR ThreeDCloudPattern(Object *ObjPtr,VECTOR *Q);
COLOR TwoDCloudPattern(Object * ObjPtr,VECTOR *Q);
COLOR FractalCloudPattern(Object * ObjPtr,VECTOR *Q);
COLOR NoPattern(Object * Obj,VECTOR * Q);
COLOR PatternOnSurface(Object *Obj,VECTOR*Q);
void SetObjectTexture(int ObjId,int PatternType,int Ndx);
double Noise(VECTOR *Q,double TS1,double TS2,double *MaxTO,unsigned char TwoD);
double Turb(VECTOR *Q,double TS1,double TS2,double *MaxTO,unsigned char TwoD);
double MaxOf(double V1,double V2,double V3,double V4);
double MinOf(double V1,double V2,double V3,double V4);
#endif