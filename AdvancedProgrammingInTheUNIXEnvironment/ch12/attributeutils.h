#include <pthread.h>

#ifndef _ATTRIBUTE_UTILS_H_ 
#define _ATTRIBUTE_UTILS_H_ 
int makethread(void *(*fn)(void *), void *arg, int detached) ;
#endif
