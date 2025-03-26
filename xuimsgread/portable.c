#include "xuimsg.h"
#ifdef _RHEL_AS4_
#include <stdint.h>
#endif
#ifdef _aix_51_
#include <stddef.h>
#endif


USINT32 rvsInt (USINT32 v) 
{
	uint32_t result = v;
	result = (v << 24) & 0xFF000000;
  	result |= (v << 8) & 0x00FF0000;
  	result |= (v >> 8) & 0x0000FF00;
  	result |= (v >> 24) & 0x000000FF;  	
	return result;
} /* End of rvsInt */


USINT16 rvsShort (USINT16 v) 
{
	uint16_t result = v;
	result = (v << 8) & 0xFF00;
  	result |= (v >> 8) & 0x00FF;

	return result;
} /* End of rvsInt */
