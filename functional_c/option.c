#include <option.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool __some_is_some(){
	return true;
}

bool __none_is_some(){
	return false;
}

extern NONE *none_instance;

char *get_str(SOME *indata){
	return (char *) indata->wrapped_data;
}

OPTION* some_string(char *instr) {
	SOME *x = (SOME *)calloc(1, sizeof(SOME));
	int len = strlen(instr);
	char *data = (char *) calloc(1, len  + 1);
	memcpy(data, instr, len); 
	x->this = (OPTION *)x;
	x->is_some = __some_is_some;
	x->get = (void *(*) (SOME*))get_str; 
	x->wrapped_data = (void *) data;
	x->delete = (void (*) (SOME*)) free_option;
	return (OPTION *)x;
}

OPTION* none_object() {
	if (none_instance == NULL){
		none_instance = (NONE *)calloc(1, sizeof(NONE));
		none_instance->this = (OPTION *)none_instance;;
		none_instance->is_some = __none_is_some; 
		none_instance->delete = (void (*) (NONE *))free_option;
	}
	return (OPTION*)none_instance;
}

void free_option(OPTION *opt) {
	if (opt->is_some()) {
		SOME*x = (SOME *)opt;
		free(x->wrapped_data);
		free(x);
	}
}
