#include <any.h>
#include <stdlib.h>
void delete_any(struct any* in){
	free(in);
}

ANY* new_any(){
	ANY *ret = (ANY*)calloc(1, sizeof(ANY));
	ret->this = ret;
	ret->delete = (void (*) ()) delete_any;
	return ret;
}
