#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Trace.h"
#include "Objects.h"
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)

void FreeObjects()
{

	int i;
	for(i=0;i<LastObj;i++)
		delete Objects[i];

}

int AddObject(Object *Obj,int Prev,int Group)
{

	if(LastObj>=MAXOBJECTS) return -1;

	if(Prev!=-1)
		Objects[Prev]->Sibling = LastObj;
	else if(Group!=-1)
		Objects[Group]->Group=LastObj;
	Objects[LastObj++]=Obj;

	return LastObj-1;

}

Object::Object()
{

	Ia1.r=1;	Ia1.g=1;	Ia1.b=1;
	Ia2.r=0;	Ia2.g=0;	Ia2.b=0;
	ka=(double).7;	kd=(double).6;		
	ks=(double).3;	kt=0;	NO=4;

	Sibling=-1;
	Group=-1;
	MaxT0=0;
	DeterminePattern=NoPattern;

}

BoundingBoxObj::BoundingBoxObj(double XMin,double XMax,double YMin,double YMax,double ZMin,double ZMax):Object()
{

	Shape=BOUNDINGBOX;
	MinX=XMin;	MaxX=XMax;	MinY=YMin;	MaxY=YMax;	MinZ=ZMin;	MaxZ=ZMax;

}

RectangleObj::RectangleObj(VECTOR *LocV,VECTOR *Vect1,VECTOR *Vect2):Object()
{

	Shape=RECTANGLE;
	V1.x=Vect1->x; V2.x=Vect2->x;	Loc.x=LocV->x;
	V1.y=Vect1->y; V2.y=Vect2->y;	Loc.y=LocV->y;
	V1.z=Vect1->z; V2.z=Vect2->z;	Loc.z=LocV->z;

	Normal=Cross(&V1,&V2);
	Normalize(&Normal);
	A=Normal.x;
	B=Normal.y;
	C=Normal.z;
	D=-(Loc.x*Normal.x + Loc.y*Normal.y + Loc.z*Normal.z);
	MinX=MinOf(Loc.x,Loc.x+V1.x,Loc.x+V2.x,Loc.x+V1.x+V2.x);
	MaxX=MaxOf(Loc.x,Loc.x+V1.x,Loc.x+V2.x,Loc.x+V1.x+V2.x);
	MinY=MinOf(Loc.y,Loc.y+V1.y,Loc.y+V2.y,Loc.y+V1.y+V2.y);
	MaxY=MaxOf(Loc.y,Loc.y+V1.y,Loc.y+V2.y,Loc.y+V1.y+V2.y);
	MinZ=MinOf(Loc.z,Loc.z+V1.z,Loc.z+V2.z,Loc.z+V1.z+V2.z);
	MaxZ=MaxOf(Loc.z,Loc.z+V1.z,Loc.z+V2.z,Loc.z+V1.z+V2.z);

	Len1=Dot(&V1,&V1);
	Len2=Dot(&V2,&V2);

}

SphereObj::SphereObj(double X,double Y,double Z,double R):Object()
{

	Shape=SPHERE;
	RSquared=R*R;
	Loc.x=X;	Loc.y=Y;	Loc.z=Z; 

}

EllipsoidObj::EllipsoidObj(double X,double Y,double Z,double AVal,double BVal,double CVal,double DVal):SphereObj(X,Y,Z,0)
{

	//(V1 & V2 denotes the dividee in each coordinate (V1.x/X2 +V1.Y/Y2 + V1.z/Z2 + V2.x)
	Shape=ELLIPSOID;
	A=AVal;		B=BVal;		C=CVal;	D=DVal;
	Loc.x=X;	Loc.y=Y;	Loc.z=Z;
	V2.x=0;		V2.y=0;		//For cloud texture
}	

CylinderObj::CylinderObj(VECTOR LocV,VECTOR Vect1,double R):SphereObj(LocV.x,LocV.y,LocV.z,R)
{

	VECTOR xAxis={1,0,0};
	Shape=CYLINDER;
	Radius=R;
	V1=Vect1;
	NV1=V1;
	Normalize(&NV1);
	Height=Dot(&V1,&V1);
	NHeight=sqrt(Height);
	NZ=Cross(&xAxis,&NV1);
	Normalize(&NZ);
	NX=Cross(&NV1,&NZ);
	Ref=Trans(&xAxis,&NX,&NV1,&NZ);


}

TriangleObj::TriangleObj(VECTOR *LocV,VECTOR *Vect1,VECTOR *Vect2):RectangleObj(LocV,Vect1,Vect2)
{

	Shape=TRIANGLE;

}

double BoundingBoxObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	double TMinX=-3.4e37,	TMaxX=3.4e38;
	double TMinY=-3e37,		TMaxY=3e38;
	double TMinZ=-3e37,		TMaxZ=3e38;
	double T1,T2;

	if(fabs(Dir->x)<TOL)
	{
	
	//The Ray is parallel to the planes.Therefore if the base of the Ray is 
	//outside of the two x planes, it must miss the bounding volume]
	//Return a failure flag.
		if(Base->x<MinX || Base->x>MaxX)
			return 0;
	
	}
	else
	{
	
		T1=(MinX-Base->x)/Dir->x;
		T2=(MaxX-Base->x)/Dir->x;
		TMinX=(T1<T2) ? T1:T2;
		TMaxX=(T1<T2) ? T2:T1;
		if(TMaxX<0) return 0;
	
	}

	if(fabs(Dir->y)<TOL)
	{
	
		if(Base->y<MinY || Base->y>MaxY)
			return 0;
	
	}
	else
	{
	
		T1=(MinY-Base->y)/Dir->y;
		T2=(MaxY-Base->y)/Dir->y;

		TMinY=(T1<T2) ? T1:T2;
		TMaxY=(T1<T2) ? T2:T1;

		if(TMaxY<0) return 0;
	
	}

	if(fabs(Dir->z)<TOL)
	{
	
		if(Base->z<MinZ||Base->z>MaxZ)
			return 0;
		
	}
	else
	{
	
		T1=(MinZ-Base->z)/Dir->z;
		T2=(MaxZ-Base->z)/Dir->z;

		TMinZ=(T1<T2)? T1:T2;
		TMaxZ=(T1<T2)? T2:T1;
		if(TMaxZ<0) return 0;
	
	}

	if(TMinX>TMinY)
	{
	
		if(TMinX>TMinZ)
			T1=TMinX;
		else
			T1=TMinZ;
	
	}
	else
	{
	
		if(TMinY>TMinZ)
			T1=TMinY;
		else
			T1=TMinZ;
	
	}

	if(TMaxX<TMaxY)
	{
	
		if(TMaxX<TMaxZ)
			T2=TMaxX;
		else
			T2=TMaxZ;
	
	}
	else
	{
	
		if(TMaxY<TMaxZ)
			T2=TMaxY;
		else
			T2=TMaxZ;
	
	}
	if(T1>T2)
		return 0;
	return 1;
	
}

double RectangleObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	double Denom,t;
	VECTOR Pt;

	//Denom=A*Dir->x+B*Dir->y+C*Dir->z;
	Denom=Dot(&Normal,Dir);
	if(Denom==0)
		return -1;
	t=Dot(&Normal,Base);
	t+=D;
	t/=Denom;
	t=-t;
	//t=-(A*Base->x+B*Base->y+C*Base->z+D)/Denom;
	if(t<0) return -1;	//Intersection behind the viewer.

	//Determine if the ray intersects within the bounds of the
	//rectangle.Project the rectangle onto the axis that causes
	//the largest projections,i.e the axis with the largest 
	//coefficient. Then test whether the projected intersection
	//point lies within the projected rectangle;

	Pt.x=Base->x+Dir->x*t;
	Pt.y=Base->y+Dir->y*t;
	Pt.z=Base->z+Dir->z*t;
	

	if((fabs(A)>fabs(B))&&(fabs(A)>fabs(C)))
	{
	
		//X axis plane equation is greatest. Project the rectangle
		//on to the X axis
		if((MinZ<Pt.z&&Pt.z<MaxZ)&&(MinY<Pt.y&&Pt.y<MaxY))
			return t;
	
	}
	else if(fabs(B)>fabs(C))
	{
		//Y axis projection is the largest
		if((MinX<Pt.x && Pt.x<MaxX)&&(MinZ<Pt.z&&Pt.z<MaxZ))
			return t;

	
	}
	else
	{
		//Z axis projection is the largest
		if((MinX<Pt.x&&Pt.x<MaxX)&&(MinY<Pt.y&&Pt.y<MaxY))
			return t;
	
	}
	return -1;

}

double TriangleObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	VECTOR Pt,Delta;
	double U[3],V[3];
	int i,j,CrossCount=0;
	double t,Denom;
	double A=Normal.x; B=Normal.y; C=Normal.z;

	Denom=A*Dir->x+B*Dir->y+C*Dir->z;
	if(Denom==0)
		return -1;
	t=-(A*Base->x+B*Base->y+C*Base->z+D)/Denom;
	if(t<0) return -1;
	
	Pt.x=Base->x+Dir->x*t;
	Pt.y=Base->y+Dir->y*t;
	Pt.z=Base->z+Dir->z*t;

	//Project the the triangle to the axis that causes the
	//largest projection,the axis with largest normal coefficient
	Delta=Substract(&Pt,&Loc);
	if(fabs(A)>fabs(B) && fabs(A) > fabs(C))
	{// Project onto X axis
	
		U[0]=-Delta.y;
		V[0]=-Delta.z;
		U[1]=V1.y-Delta.y;
		V[1]=V1.z-Delta.z;
		U[2]=V2.y-Delta.y;
		V[2]=V2.z-Delta.z;
	
	}

	else if(fabs(B)>fabs(C))
	{//Project onto Y axis
	
		U[0]=-Delta.x;
		V[0]=-Delta.z;
		U[1]=V1.x-Delta.x;
		V[1]=V1.z-Delta.z;
		U[2]=V2.x-Delta.x;
		V[2]=V2.z-Delta.z;
	
	}

	else
	{//Project onto the z axis
	
		U[0]=-Delta.x;
		V[0]=-Delta.y;
		U[1]=V1.x-Delta.x;
		V[1]=V1.y-Delta.y;
		U[2]=V2.x-Delta.x;
		V[2]=V2.y-Delta.y;
	
	}
	//Compute number of intersection
	for(i=0,j=1;i<3;i++,j=(j+1)%3)
	{
	
		
		if((V[i]<0 && V[j]>=0)||
		   (V[i]>=0&& V[j]<0 ))
		{
		
			if(U[i]>=0 && U[j]>=0)
				CrossCount++;

			else if(U[i]>=0 || U[j]>=0)
				if(U[i]-V[i]*(U[j]-U[i])/(V[j]-V[i])>0)
					CrossCount++;
		
		}
		
	
	}
	if(CrossCount%2==0)
		return -1;
	return t;

}

double SphereObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	double Det,Aq,Bq,Cq,First,Second,Int1,Int2;
	VECTOR Base2;

	Base2=Substract(Base,&Loc);
	Aq=Dot(Dir,Dir);
	if(Aq==0) return -1;
	Bq=2*Dot(Dir,&Base2);
	Cq=Dot(&Base2,&Base2)-RSquared;

	Det=Bq*Bq-4*Aq*Cq;
	if(Det>=0)
	{
	
		Int1=sqrt(Det);
		Int2=2*Aq;
		First=(-Bq+Int1)/Int2;
		if(First>TOL)
		{
		
			Second=(-Bq-Int1)/Int2;
			if(First<Second)
				return First;
			if(Second<TOL)
				return First;
			return Second;	
		
		}
		Second=(-Bq-Int1)/Int2;
		if(Second>TOL)
			return Second;
	
	}
	return -1;
}

double EllipsoidObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	double Aq,Bq,Cq,Det,First,Second,Int1,Int2;
	VECTOR Base2;

	Base2=Substract(Base,&Loc);
	Aq=A*Dir->x*Dir->x+ B*Dir->y*Dir->y + C*Dir->z*Dir->z;

	Bq=2*(A*Base2.x*Dir->x+B*Base2.y*Dir->y+C*Base2.z*Dir->z);

	Cq=-D+ A*Base2.x*Base2.x + B*Base2.y*Base2.y + C*Base2.z*Base2.z;

	Det=Bq*Bq - 4*Aq*Cq;

	if(Det>=0)
	{
	
		Int1=sqrt(Det);
		Int2=2*Aq;
		First=(-Bq+Int1)/Int2;
		if(First>TOL)
		{
		
			Second=(-Bq-Int1)/Int2;
			if(First<Second)
				return First;
			if(Second<TOL)
				return First;
			return Second;
		
		}
		Second=(-Bq-Int1)/Int2;
		if(Second>TOL)
			return Second;
	
	}
	return -1;

}

double CylinderObj::ComputeT(VECTOR *Base,VECTOR *Dir)
{

	double Aq,Bq,Cq,Det,FirstT,SecondT,Int1,Int2;
	VECTOR K,D;
			
	double T1,T2,T3;
	double SmallerT,LargerT;

	//Calculate the ray's base location in the object
	//coordinate system

	T2=Dot(Base,&V1) - Dot(&Loc,&V1);
	T1=T2/Height;
	
	K.x=Base->x-Loc.x-T1*V1.x;
	K.y=Base->y-Loc.y-T1*V1.y;
	K.z=Base->z-Loc.z-T1*V1.z;		

	//Calculate the ray's dir in object coordinate system
	
	T3=Dot(Dir,&V1);
	T1=T3/Height;
	D.x=Dir->x-T1*V1.x;
	D.y=Dir->y-T1*V1.y;
	D.z=Dir->z-T1*V1.z;

	Aq=Dot(&D,&D);
	Bq=2*Dot(&D,&K);
	Cq=Dot(&K,&K)-RSquared;

	Det=Bq*Bq-4*Aq*Cq;

	if(Det>=0)
	{
	
		Int1=sqrt(Det);
		Int2=2*Aq;
		FirstT=(-Bq+Int1)/Int2;
		if(FirstT<TOL)
		{
		
			SecondT=(-Bq-Int1)/Int2;
			if(SecondT<TOL)
				return -1;
			SmallerT=SecondT;
			LargerT=-1;
		
		}
		else
		{
		
			SecondT=(-Bq-Int1)/Int2;
			if(FirstT<SecondT)
			{
			
				SmallerT=FirstT;
				LargerT=SecondT;
			
			}
			else if(SecondT<TOL)
			{
			
				SmallerT=FirstT;
				LargerT=-1;
			
			}
			else
			{
			
				SmallerT=SecondT;
				LargerT=FirstT;
			
			}
		
		}
	
		T1=(T2+SmallerT*T3)/Height;
		if(T1>=0 && T1<=1)
			return SmallerT;
		if(LargerT>=0)
		{
		
			T1=(T2+LargerT*T3)/Height;
			if(T1>=0 && T1<=1)
				return LargerT;

		}
	}

	return -1;
}

void RectangleObj::ComputeNormal(VECTOR *,VECTOR *N)
{

	N->x=Normal.x;
	N->y=Normal.x;
	N->z=Normal.z;

}

void SphereObj::ComputeNormal(VECTOR *Q,VECTOR *N)
{

	*N=Substract(Q,&Loc);
	Normalize(N);

}

void EllipsoidObj::ComputeNormal(VECTOR *Q,VECTOR *N)
{

	VECTOR Pos=Substract(Q,&Loc);

	N->x=A*Pos.x;
	N->y=B*Pos.y;
	N->z=C*Pos.z;

	Normalize(N);

}

void CylinderObj::ComputeNormal(VECTOR *Q,VECTOR *N)
{

	double t;
	VECTOR C;
	VECTOR Pt;

	C=V1;
	Normalize(&C);

	VECTOR Tmp=Substract(Q,&Loc);
	t=Dot(&Tmp,&C)/Dot(&C,&V1);

	Pt.x=Loc.x+t*V1.x;
	Pt.y=Loc.y+t*V1.y;
	Pt.z=Loc.z+t*V1.z;

	*N=Substract(Q,&Pt);
	Normalize(N);

}


//Start Of all Jokes
#define NUMWAVECENTERS 16

void WaterObj::ComputeNormal(VECTOR *Q,VECTOR *N)
{

	VECTOR Dist;
	VECTOR WaveCenters[NUMWAVECENTERS]=
	{
	
		{-4,-4, 0},	{ 0, 2,-0},	{ 3, 4, 0},	{-4, 3, 0},
		{10, 2, 0},	{ 4, 1,-0},	{ 2,-4, 0}, {-1, 3, 0},
		{-2,-2, 0}, { 0,-2, 0}, { 3,-1, 0},	{-2, .4, 0},
		{-6,-.4,-0}, { 1, 2, 0}, { 3,-4,-0}, { 2, 1, 0}
	
	};

	double fnum[NUMWAVECENTERS]=
	{10,44,18,24,36,35,54,22,19,22,19,22,10,24,36,35};
	double M,f,fr,PhaseInc;
	int I;
	//Dist=Substract(&From,Q);
	fr=0;
	N->x=Normal.x;
	N->y=Normal.y;
	N->z=Normal.z;

	for(I=0;I<NumWaveCenters;I++)
	{
	
		f=fnum[I]*Rough+fr;
		PhaseInc=pow(f,0.25);
		Dist=Substract(Q,&WaveCenters[I]);
		Dist.x=Dist.x*f;
		Dist.y=Dist.y*f;
		Dist.z=Dist.z*f;

		M=Mag(&Dist);

		N->x+=(Ht*sin(M+FrameNum*PhaseInc));
		N->y+=(Ht*sin(M+FrameNum*PhaseInc));
		N->z+=(Ht*cos(M+FrameNum*PhaseInc));
		
	}
	Normalize(N);

}

COLOR NoPattern(Object *Obj,VECTOR *)
{

	return Obj->Ia1;

}

#define PATTERNWD 2
#define PATTERNHT 2

//Paints a checker pattern on a rectangle
COLOR PatternOnSurface(Object *Obj,VECTOR *Q)
{

//	VECTOR Offset;
	int X,Y;
	double Pos1,Pos2;
	int PatternWd=PATTERNWD,PatternHt=PATTERNHT;
	int MultX,MultY;

	unsigned char Pattern[PATTERNHT][PATTERNWD]={{0,1},{1,0}};
	//The MultX and MultY variables specify how many times the
	//pattern is repeated on the rectangle's surface. These 
	//multipliers are packed in the ImageNdx variable

	MultX=Obj->ImageNdx&0x0ff;
	MultY=(Obj->ImageNdx>>8)&0x0ff;

	Obj->MapToUV(Q,Pos1,Pos2);
	X=Pos1*PatternWd*MultX;
	Y=Pos2*PatternHt*MultY;

	X%=(PatternWd);
	Y%=(PatternHt);

	if(Pattern[Y][X])
		return Obj->Ia2;
	else 
		return Obj->Ia1;

}

//Generate the texture for a cloud


void RectangleObj::MapToUV(VECTOR *Q,double& U,double& V)
{

	VECTOR Offset;
	Offset=Substract(Q,&Loc);

	U=Dot(&Offset,&V1);
	V=Dot(&Offset,&V2);
	U/=Len1;
	V/=Len2;

}

void SphereObj::MapToUV(VECTOR *Q,double& U,double& V)
{

	double Lat,Long,Temp;
	VECTOR N;
	VECTOR PCrossE;
	VECTOR Pole={0,0,1};
	VECTOR Equator={1,0,0};
	*Q=Substract(Q,&Loc);
	ComputeNormal(Q,&N);
	Lat=acos(-Dot(&N,&Pole));
	V=Lat*0.318;

	Temp=Dot(&Equator,&N)/sin(Lat);
	if(Temp>1) Temp=1;
	else if(Temp<-1) Temp=-1;
	Long=acos(Temp)*0.1592;
	PCrossE=Cross(&Pole,&Equator);
	if(Dot(&PCrossE,&N)>0) U=Long;
	else 
		U=1-Long;

}

void CylinderObj::MapToUV(VECTOR *Q,double& U,double& V)
{

	
	VECTOR Tmp=Substract(Q,&Loc);
	V=Dot(&Tmp,&NV1)/NHeight;
	
	Tmp.x=Tmp.x-V*V1.x;
	Tmp.y=Tmp.y-V*V1.y;
	Tmp.z=Tmp.z-V*V1.z;
	Normalize(&Tmp);
	U=Dot(&Ref,&Tmp);		
	U=acos(U)*0.15916;
	if(U>1.0)U=1.0;
	else if(U<0.0)U=0.0;
	if(Tmp.y<0) U=1-U;
	if(V>1.0) V=1.0;
	else if(V<0.0)V=0.0;
	V=1-V;
	

}



double Noise(VECTOR *Q,double TS1,double TS2,double *MaxT0,unsigned char TwoD)
{

	int i,n;
	float FXi,FYi,Ci,PXi,PYi;	
	float T0,T1,T2,T;

	FYi=1.0;		
	FXi=TS1*FYi;	

	Ci=1.0;			
	n=7;

	T0=TS2;

	T1=0.0;
	T2=0.0;
	
	PXi=1.5708 * sin(0.5 * FYi * Q->y);	//PX0
	PYi=1.5708 * sin(0.5 * FXi * Q->x);	//PY0

	if(!TwoD)
	{
	
		PXi+=sin(FXi * Q->z/2)*3.1415927;
		PYi+=sin(FXi * Q->z/2)*3.1415927;
	
	}

	T2+=Ci*sin(FYi*Q->y+PYi)+T0;	

	FXi*=2.0;					//FX1
	FYi*=2.0;					//FY1
	Ci*=0.707;					//C1

	for(i=2;i<n;i++)
	{
		//This one is even func
		T1+=Ci*sin(FXi*Q->x+PXi)+T0;
		T2+=Ci*sin(FYi*Q->y+PYi)+T0;

		PYi=1.5708*sin(FXi*Q->x);
		PXi=1.5708*sin(FYi*Q->y);

		if(!TwoD)
		{
		
			PXi+=sin(FXi*Q->z/2)*3.1415927;
			PYi+=sin(FXi*Q->z/2)*3.1415927;
		
		}

		FXi*=2.0;
		FYi*=2.0;
		Ci*=0.707;
	
	}
	if(fabs(T1*T2)>*MaxT0)
	{
	
		*MaxT0=fabs(T1*T2);
		T=fabs(T1*T2)/ *MaxT0/0.75;
	
	}
	else
		T=fabs(T1*T2)/ *MaxT0;

	if(T>1.0) T=1.0;
	else if(T<0) T=0;

	return T;
}
double Turb(VECTOR *Q,double TS1,double TS2,double *MaxT0,unsigned char TwoD)
{

	int i,n;
	float FXi,FYi,Ci,PXi,PYi;
	float T0,T1,T2,T;

	FYi=1.0;		
	FXi=TS1*FYi;	

	Ci=1.0;			
	n=7;

	T0=TS2;

	T1=0.0;
	T2=0.0;
	
	PXi=1.5708 * sin(0.5 * FYi * Q->y);	//PX0
	PYi=1.5708 * sin(0.5 * FXi * Q->x);	//PY0
	PXi+=sin(FXi * Q->z/2)*3.1415927;
	PYi+=sin(FXi * Q->z/2)*3.1415927;
	T2+=Ci*sin(FYi*Q->y+PYi)+T0;	
	FXi*=2.0;					//FX1
	FYi*=2.0;					//FY1
	Ci*=0.707;					//C1

	for(i=2;i<n;i++)
	{
		//This one is even func
		T1+=Ci*sin(FXi*Q->x+PXi)+T0;
		T2+=Ci*sin(FYi*Q->y+PYi)+T0;

		PYi=1.5708*sin(FXi*Q->x);
		PXi=1.5708*sin(FYi*Q->y);
		PXi+=sin(FXi*Q->z/2)*3.1415927;
		PYi+=sin(FXi*Q->z/2)*3.1415927;
		FXi*=2.0;
		FYi*=2.0;
		Ci*=0.707;
	
	}
	
	T=fabs(T1*T2);

	if(T>1.0) T=1.0;
	else if(T<0) T=0.0;
	return T;

}

COLOR TwoDCloudPattern(Object *ObjPtr,VECTOR *Q)
{

	COLOR Color;
	double T;

	RectangleObj *Obj=(RectangleObj *)ObjPtr;
	T=Noise(Q,Obj->T0,Obj->T1,&Obj->MaxT0,TRUE);


	Color.r=Obj->Ia1.r+(Obj->Ia2.r-Obj->Ia1.r)*T;
	Color.g=Obj->Ia1.g+(Obj->Ia2.g-Obj->Ia1.g)*T;
	Color.b=Obj->Ia1.b+(Obj->Ia2.b-Obj->Ia1.b)*T;

	return Color;

}
COLOR FractalCloudPattern(Object *ObjPtr,VECTOR *Q)
{
	
	COLOR Color;
	double T;
	EllipsoidObj *Obj=(EllipsoidObj *)ObjPtr;
	if(Obj->MaxT0==0)
	{
	
		WorldToScreen(Obj->Loc,Obj->V1.x,Obj->V1.y);
	
	}

	T=Noise(Q,Obj->V1.z,Obj->V2.z,&Obj->MaxT0,TRUE);
	T = acos(T);
	T = pow(T,((EllipsoidObj *)Obj)->Dim);
	T = sin(T);		

	T=fabs(T);
	
	Obj->kt=T;
	if(T>.8 || T<.1)
	{
		Obj->kt=1;
	
	}
	Color.r=Obj->Ia1.r;
	Color.g=Obj->Ia1.g;
	Color.b=Obj->Ia1.b;
	return Color;
}
COLOR ThreeDCloudPattern(Object *ObjPtr,VECTOR *Q)
{

	COLOR Color;
	double k,kx,ky,T;
	EllipsoidObj *Obj=(EllipsoidObj *)ObjPtr;
	if(Obj->MaxT0==0)
	{
	
		WorldToScreen(Obj->Loc,Obj->V1.x,Obj->V1.y);
		//V1.x , V1.y= In Screen Coordinate
	
	}

	T=Noise(Q,Obj->V1.z,Obj->V2.z,&Obj->MaxT0,TRUE);

	kx=(x-Obj->V1.x)*(x-Obj->V1.x);
	if(kx>Obj->V2.x)
		Obj->V2.x=kx;
	ky=(y-Obj->V1.y)*(y-Obj->V1.y);
	if(ky>Obj->V2.y)
		Obj->V2.y=ky*.96;

	kx/=Obj->V2.x;
	ky/=Obj->V2.y;

	k=(kx+ky);

	if(k<0) k=0;
	else if (k>1) k=1;
	Obj->kt=k;	//Modulate the transparency
	T/=2;
	Color.r=1-T;
	Color.g=1-T;
	Color.b=1-T;

	if(T<.5 && Obj->kt<T*2) Obj->kt=T*2;

	return Color;

}

COLOR MapImage(Object *Obj,VECTOR *Q)
{

	double U,V;
	long X,Y;
	int PatternWd,PatternHt;
	COLOR C;

	Obj->MapToUV(Q,U,V);
	PatternWd=Images[Obj->ImageNdx].ImWd;
	PatternHt=Images[Obj->ImageNdx].ImHt;

	X=U*PatternWd;
	Y=V*PatternHt;
	Y*=PatternWd;
	Y+=X;

	if(Images[Obj->ImageNdx].ImType==RGBIMAGE)
	{
	
		C.r=(char)Images[Obj->ImageNdx].Image[Y*3L]/255.0;
		C.g=(char)Images[Obj->ImageNdx].Image[Y*3L+1]/255.0;
		C.b=(char)Images[Obj->ImageNdx].Image[Y*3L+2]/255.0;
	
	}
	else//GrayScale
	{
	
		C.r=Images[Obj->ImageNdx].Image[Y]/255.0;
		C.g=Images[Obj->ImageNdx].Image[Y]/255.0;
		C.b=Images[Obj->ImageNdx].Image[Y]/255.0;
	
	}
	return C;
}

COLOR Blend(Object *Obj,VECTOR *Q)
{

	double U,V;
	COLOR C;
	Obj->MapToUV(Q,U,V);
	V=V-.5*HorizonOffset;

	if(V>1) V=1;
	else if(V<0) V=0;

	C.r=Obj->Ia1.r+(Obj->Ia2.r-Obj->Ia1.r)*V;
	C.g=Obj->Ia1.g+(Obj->Ia2.g-Obj->Ia1.g)*V;
	C.b=Obj->Ia1.b+(Obj->Ia2.b-Obj->Ia1.b)*V;

	return C;

}
void SetObjectTexture(int ObjId,int PatternType,int Ndx)
{

	Object* Obj;
	Obj=Objects[ObjId];
	Obj->ImageNdx=Ndx;
	switch(PatternType)
	{
	
		case IMAGEPATTERN:
			Obj->DeterminePattern=MapImage;
			break;
		case CHECKERPATTERN:
			Obj->DeterminePattern=PatternOnSurface;
			break;
		case CLOUDPATTERN:
			if(Obj->Shape==ELLIPSOID)
				Obj->DeterminePattern=ThreeDCloudPattern;
			else if(Obj->Shape==RECTANGLE)
				Obj->DeterminePattern=TwoDCloudPattern;
			break;
		case FRACTALCLOUD:
			Obj->DeterminePattern = FractalCloudPattern;
			break;
		case MARBLEPATTERN:
			if(Obj->Shape==RECTANGLE)
				Obj->DeterminePattern=TwoDCloudPattern;
			break;
		case BLENDPATTERN:
			Obj->DeterminePattern=Blend;
			break;
	
	}

}

double MaxOf(double V1,double V2,double V3,double V4)
{

	if(V1>V2)
	{
	
		if(V1>V3)
		{
		
			if(V1>V4)
				return V1;
			else 
				return V4;
		
		}
		else
		{
			
			if(V3>V4)
				return V3;
			else
				return V4;

		}
	
	}
	else
	{
	
		if(V2>V3)
		{
		
			if(V2>V4)
				return V2;
			else
				return V4;
		
		}
		else
		{
		
			if(V3>V4)
				return V3;
			else
				return V4;
		
		}
	
	}

}

double MinOf(double V1,double V2,double V3,double V4)
{

	if(V1<V2)
	{
	
		if(V1<V3)
		{
		
			if(V1<V4)
				return V1;
			else
				return V4;
		
		}
		else
		{
		
			if(V3<V4)
				return V3;
			else 
				return V4;
		
		}
	
	}
	else
	{
	
		if(V2<V3)
		{
		
			if(V2<V4)
				return V2;
			else
				return V4;
		
		}
		else
		{
		
			if(V3<V4)
				return V3;
			else return V4;
		
		}
	
	}

}