#include "tuple.h"
#include <stdlib.h>

void delete_tuple2(TUPLE_2 *in){
	in->e0->delete(in->e0);
	in->e0 = NULL;
	in->e1->delete(in->e1);
	in->e1 = NULL;
	in->copy = NULL;
	in->delete = NULL;	
}

TUPLE_2 * copy_tuple2(TUPLE_2 *in) {
	TUPLE_2  *tin = calloc(1, sizeof (TUPLE_2));
	if (tin) {
		tin->e0 = in->e0->copy(in->e0);
		tin->e1 = in->e1->copy(in->e1);
		tin->delete = in->delete;
		tin->copy = in->copy;	
	}
	return tin;
	
}
TUPLE_2 *new_tuple2(ANY *e0, ANY *e1){ 
	TUPLE_2  *tin = calloc(1, sizeof (TUPLE_2));
	if(tin){
		tin->e0 = e0->copy(e0);
		tin->e1 = e1->copy(e1);
		tin->delete = delete_tuple2;
		tin->copy = copy_tuple2;
	}
	return tin;
}
