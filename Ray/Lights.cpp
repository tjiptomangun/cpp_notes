#include "stdafx.h"
#include <math.h>
#include "Lights.h"

double ScaleLight(double PointToLight)
{

	double LightScale=(LMax-LMin)*(PointToLight-DMin)/(DMax-DMin) +LMin;
	if(LightScale<LMin) return LMin;
	else if(LightScale>LMax) return LMax;
	else return LightScale;

}

double PointLight::W(double CosTheta,double K)
{

	//W is magnitude of the light
	return 1-K*CosTheta;

}

void ConeLight::SetConeLightAngle(double Angle)
{

	CosGamma=(double)cos(Angle*0.01745);

}

void DirectedLight::SetLightDirection(VECTOR *D)
{

	Direction=Substract(D,&LFW);
	Normalize(&Direction);

}

void RemoveLights()
{

	int i;
	for(i=0;i<NumLights;i++)
		delete Lights[i];

}

int AddLight(PointLight *L)
{

	if(NumLights>=MAXLIGHTS) return -1;
	Lights[NumLights]=L;
	NumLights++;
	return NumLights-1;

}

