#ifndef VECTORH
#define VECTORH

typedef struct
{

	double x,y,z;

}VECTOR;

double Mag(VECTOR *V);
VECTOR Substract(VECTOR *V1,VECTOR *V2);
VECTOR Add(VECTOR *V1,VECTOR *V2);
VECTOR Cross(VECTOR *V1,VECTOR *V2);
VECTOR Divide(VECTOR *V,double Num);
VECTOR Trans(VECTOR *V1,VECTOR *VX,VECTOR *VY,VECTOR *VZ);
void Normalize(VECTOR *V);
double Dot(VECTOR *V1,VECTOR *V2);
#endif
