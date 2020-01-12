#include <stdio.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define maxint(a, b)\
({int _a = (a), _b = (b); _a > _b ? _a : _b;\
})

int main (int argc, char **argv){
	int _a = 1 , _b = 2, c;
	c = max(_a, _b);
	fprintf(stdout, "max (a, b) = %d\n", c);

	c = maxint(_a, _b);
	fprintf(stdout, "max (a, b) = %d\n", c);
}
