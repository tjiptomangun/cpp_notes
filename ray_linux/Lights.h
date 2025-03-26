#ifndef LIGHTSH
#define LIGHTSH

#include "Objects.h"
#include "Vector.h"
#include "Trace.h"
#include <math.h>


class PointLight
{

public:
	VECTOR LFW;
	COLOR Color;
	PointLight(VECTOR *Loc,COLOR *I)
	{
	
		LFW=*Loc;
		Color=*I;
	
	}

	double W(double CosTheta,double K);

	virtual double LightFactor(VECTOR *)
	{
		return 1.0;
	}

};

class DirectedLight:public PointLight
{
	
public:
	int DirectPow;
	VECTOR Direction;

	DirectedLight(VECTOR *Loc,COLOR *I):PointLight(Loc,I)
	{
	
		DirectPow=4;
		Direction=Substract(&At,Loc);
		Normalize(&Direction);
	
	}

	void SetLightDirection(VECTOR *D);
	virtual double LightFactor(VECTOR *L)
	{
	
		double CosBeta=-Dot(L,&Direction);
		if(CosBeta>0)
			return (double)pow(CosBeta,DirectPow);
		return 0.0;
	
	}

};

class ConeLight:public DirectedLight
{

public:
	double CosGamma;
	ConeLight(VECTOR *Loc,COLOR *I):DirectedLight(Loc,I)
	{
	
		CosGamma=(double)0.9397;
	
	}

	virtual double LightFactor(VECTOR *L)
	{
	
		double CosBeta=-Dot(L,&Direction);
		if(CosBeta>0 && CosBeta<=CosGamma)
			return (double)pow(CosBeta,DirectPow);
		return 0.0;
	
	}

	void SetConeLightAngle(double Angle);

};

extern PointLight* Lights[MAXLIGHTS];
extern int NumLights;
extern double LMin,LMax;
extern double DMin;
extern double DMax;

double ScaleLight(double PointToLight);
void InitializeLights(PointLight * Lights[]);
int AddLight(PointLight *L);
void RemoveLights();

#endif
