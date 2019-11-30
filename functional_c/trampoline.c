//https://stackoverflow.com/questions/189725/what-is-a-trampoline-function
#include <stdio.h>

typedef struct trampoline_data {
	void (*callback)(struct trampoline_data*);
	void *parameters;
}trampoline_data;

void trampoline(trampoline_data* data) {
	while(data->callback != NULL)
		data->callback(data);
}

typedef struct factorial_parameters{
	int n;
	int product;
}factorial_parameters;

void factorial(trampoline_data *data){
	factorial_parameters *parameters = (factorial_parameters *)data->parameters;

	if (parameters->n <=1){
		data->callback = NULL;
	}
	else{
		parameters->product *= parameters->n;
		parameters->n --;
	}
}

int main () {
	factorial_parameters params = {5, 1};
	trampoline_data t = {*factorial, &params};	

	trampoline(&t);
	printf("\n%d\n", params.product);

	return 0;
}
