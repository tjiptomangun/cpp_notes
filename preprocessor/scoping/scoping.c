//http://www.mybitbox.com/tag/robust-code/

#include <stdio.h>

#define fn_unscoped(x, y, z) x+=1; y+=1; z+=1;
#define fn_do_whiled(x, y, z) do{x+=1; y+=1; z+=1;}while(0)
#define fn_simple_scoped(x, y, z) {x+=1; y+=1; z+=1;}

int main(int argc, char **argv) {
	int a = 10;
	int b = 20;
	int c = 30;

	if(a == 10)
		fn_unscoped(a, b, c);
/*	else
		b--;*/

	if(b == 21) 
		fn_unscoped(a, b, c);
/*	else
		c--;*/

	fprintf(stdout, "fn_unscoped a = %d, b = %d, c = %d\n", a, b, c);

	a = 10;
	b = 20;
	c = 30;

	if(a == 10)
		fn_do_whiled(a, b, c);
	else
		b--;

	if(b == 21) 
		fn_do_whiled(a, b, c);
	else
		c--;

	fprintf(stdout, "fn_do_whiled a = %d, b = %d, c = %d\n", a, b, c);


	a = 10;
	b = 20;
	c = 30;

	if(a == 10)
		fn_simple_scoped(a, b, c);
/*	else
		b--;*/

	if(b == 21) 
		fn_simple_scoped(a, b, c);
/*	else
		c--;*/

	fprintf(stdout, "fn_simple_scoped a = %d, b = %d, c = %d\n", a, b, c);

}

