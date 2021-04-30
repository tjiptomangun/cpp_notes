#include <any.h>
#include <stdlib.h>
void delete_any(struct any* in){
	free(in);
}
static ANY* copy(ANY *a) {
	return new_any();
}

ANY* new_any(){
	ANY *ret = (ANY*)calloc(1, sizeof(ANY));
	if(ret) {
		ret->this = ret;
		ret->delete = (void (*) ()) delete_any;
		ret->copy= copy;
	}
	return ret;
}

