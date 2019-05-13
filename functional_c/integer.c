#include <integer.h>
#include <stdlib.h>

static Integer *copy(Integer *in){
	return new_integer(in->value);
}

void delete_integer(Integer *in){
	if (in) {
		in->value = 0;
		in->delete = NULL;
		in->copy =  NULL;
		free(in);
	}	
} 

Integer* new_integer(int i){
	Integer * ret = (Integer *) calloc(1, sizeof(Integer));
	if (ret){
		ret->this = ret;
		ret->value = i;
		ret->delete = delete_integer;	
		ret->copy = copy;
	}
	return ret;
}

