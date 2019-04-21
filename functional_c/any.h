#ifndef _ANY_H_
#define _ANY_H_
/**
 * ANY base class of other classes that is functional data structure.
 * It is like Any in scala and Unit in rust.
 * Like all object oriented class, it should also have member the 
 * `this` pointer;
 */
typedef struct any {
	struct any *this;
	void (*delete) (struct any*);
}ANY;
void delete_any(ANY *);
ANY *new_any();
#endif

