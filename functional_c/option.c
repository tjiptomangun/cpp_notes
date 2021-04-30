#include <option.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <any.h>

bool __some_is_some(){
	return true;
}

bool __none_is_some(){
	return false;
}

extern NONE_OPTION *none_instance;

ANY *get_some_data(SOME *indata){
	return (ANY *) indata->wrapped_data;
}

OPTION* none_object() {
	NONE_OPTION* t = NULL;
	if (none_instance == NULL){
		t = (NONE_OPTION *)calloc(1, sizeof(NONE_OPTION));
		if(t) {
			none_instance = t;
			none_instance->this = (OPTION *)none_instance;;
			none_instance->is_some = __none_is_some; 
			none_instance->delete = (void (*) (NONE_OPTION *))free_o_option;
		}
	}
	return (OPTION*)none_instance;
}

void free_o_option(OPTION *opt) {
	if (opt->is_some()) {
		SOME*x = (SOME *)opt;
		x->wrapped_data->delete(x->wrapped_data);
		x->this = NULL;
		x->delete = NULL;
		x->is_some = NULL;
		x->get = NULL;
		free(x);
	}
}

SOME *some_object(ANY *obj) {
	SOME *x = (SOME *)calloc(1, sizeof(SOME)); 
	if(x) {
		ANY *dup = obj->copy(obj);
		x->this = (OPTION *)x;
		x->delete = (void (*) (SOME*)) free_o_option;
		x->is_some = __some_is_some;
		x->get = get_some_data; 
		x->wrapped_data = dup;
	}
	return x;
	
}
