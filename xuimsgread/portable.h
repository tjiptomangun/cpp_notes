#ifndef _PORTABLE_H_
#define _PORTABLE_H_
#ifdef _RHEL_AS_
#include <stdint.h>
#endif
#ifdef _aix_51_
#include <stddef.h>
#endif
#define USINT16 unsigned short
#define USINT32 unsigned int
extern USINT32 rvsInt (USINT32 v); 
extern USINT16 rvsShort (USINT16 v); 
#endif
