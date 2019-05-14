#ifndef _TUPLE_H_
#define _TUPLE_H_
#include "any.h"
typedef struct tuple_2 {
	struct tuple_2 *this;
	void (*delete) (struct tuple_2*);
	struct tuple_2* (*copy) (struct tuple_2*); 
	ANY	*e0;
	ANY *e1;
}TUPLE_2;

TUPLE_2 *new_tuple2(ANY *e0, ANY *e1);
#endif
