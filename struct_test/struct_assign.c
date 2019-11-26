#include <stdio.h>
#include <stdlib.h>



typedef struct 
{
	int	int_value1;
	int	int_value2;
	char 	char_value;
	double	double_value;
	char	char_array[20];
} ourstruct;
int printstruct (ourstruct *in)
{
	fprintf (stdout, "int_value1 = %d\n", in->int_value1);
	fprintf (stdout, "int_value2 = %d\n", in->int_value2);
	fprintf (stdout, "char_value= %c\n", in->char_value);
	fprintf (stdout, "double_value= %f\n", in->double_value);
	fprintf (stdout, "char_array= %s\n", in->char_array);
	return 0;
}

void struct_test_assign(ourstruct *in)
{
	ourstruct copied = *in;	
	printstruct (&copied); 
	
}

int main (int argc, char **argv)
{
	ourstruct orig, assign ;
	
	orig.int_value1 = -1;
	orig.int_value2 = -7;
	orig.char_value = 'p';
	orig.double_value = 2.71828247254;
	sprintf (orig.char_array, "%s", "hello world!");
	fprintf (stdout, "original\n==============\n");
	printstruct (&orig);
	assign = orig;
	fprintf (stdout, "assigned local\n==============\n");
	printstruct (&assign);
	fprintf (stdout, "assigned pointer\n==============\n");
	struct_test_assign(&orig);
	
	exit (0);
}
