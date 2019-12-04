#include <stdio.h>

#define ONE_SHL( X ) ( 1 << X )

#define ASSGNBY_UNDEFINED 0
#define ASSGNBY_ORIGIFACE 0
#define ASSGNBY_REGULATOR 1
#define ASSGNBY_XAC       2
#define ASSGNBY_CAM       3
#define ASSGNBY_RECEIVER  4


int main ( int argc, char **argv )
{
	int assgnby_regulator =	ONE_SHL( ASSGNBY_REGULATOR ) ;
	int assgnby_xac =	ONE_SHL( ASSGNBY_XAC ) ;
	int assgnby_cam = ONE_SHL ( ASSGNBY_CAM );
	int assgnby_cam_xac = ONE_SHL ( ASSGNBY_CAM ) | ONE_SHL ( ASSGNBY_RECEIVER ); 
	int assgnby_cam_xac2 = ONE_SHL ( (ASSGNBY_CAM + ASSGNBY_RECEIVER ); 
	


	printf ("assgnby_regulator %d\n",assgnby_regulator);
	printf ("assgnby_xac %d\n",assgnby_xac);
	printf ("assgnby_cam %d\n",assgnby_cam);
	printf ("assgnby_cam_xac %d\n",assgnby_cam_xac);
	printf ("assgnby_cam_xac2 %d\n",assgnby_cam_xac2);

}

