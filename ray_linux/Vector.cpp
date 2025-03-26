#include <math.h>
#include <stdlib.h>
#include "Vector.h"

double Mag(VECTOR *V)
{

        return (double)sqrt(V->x*V->x+V->y*V->y+V->z*V->z);

}

VECTOR Substract(VECTOR *V1,VECTOR *V2)
{

        VECTOR D;
        D.x=V1->x-V2->x;
        D.y=V1->y-V2->y;
        D.z=V1->z-V2->z;
        return D;

}

VECTOR Add(VECTOR *V1,VECTOR *V2)
{

        VECTOR D;
        D.x=V1->x+V2->x;
        D.y=V1->y+V2->y;
        D.z=V1->z+V2->z;
        return D;
        
}

VECTOR Cross(VECTOR *V1,VECTOR *V2)
{

        VECTOR C;
        C.x=V1->y*V2->z-V2->y*V1->z;
        C.y=V1->z*V2->x-V2->z*V1->x;
        C.z=V1->x*V2->y-V2->x*V1->y;
        return C;
        
}

VECTOR Divide(VECTOR *V,double Num)
{

        VECTOR Result;
        if(Num!=0)
        {

                Result.x=V->x/Num;
                Result.y=V->y/Num;
                Result.z=V->z/Num;

        }
        return Result;
}

void Normalize(VECTOR *V)
{

        double D=(double)sqrt(V->x*V->x+V->y*V->y+V->z*V->z);
        if(D!=0)
        {

                V->x=V->x/D;
                V->y=V->y/D;
                V->z=V->z/D;

        }

}

double Dot(VECTOR *V1,VECTOR *V2)
{

        return V1->x*V2->x + V1->y*V2->y + V1->z*V2->z;

}

VECTOR Trans(VECTOR *V1,VECTOR *VX,VECTOR *VY,VECTOR *VZ)
{

	VECTOR Res;
	Res.x=V1->x*VX->x+V1->y*VY->x+V1->z*VZ->x;
	Res.y=V1->x*VX->y+V1->y*VY->y+V1->z*VZ->y;
	Res.z=V1->x*VX->z+V1->y*VY->z+V1->z*VZ->z;

	/*Res.x=Dot(V1,VX);
	Res.y=Dot(V1,VY);
	Res.z=Dot(V1,VZ);*/
	
	return Res;

}
