#ifndef _INTEGER_H_
#define _INTEGER_H_
typedef struct integer{
	struct integer *this;
	void (*delete) (struct integer *);
	struct integer *(*copy) (struct integer *);
	int value;
}Integer;
void delete_integer(Integer *);
Integer* new_integer(int i);
#endif
