#include <stdio.h>
#include <stdlib.h>
typedef struct ctest
{
	int i;
	void (*print) (struct ctest *); 
	void (*delete) (struct ctest *);
} CTEST, *PCTEST;

typedef CTEST dtest;
typedef dtest DTEST, *PDTEST;

void __ctest_print (PCTEST p)
{
	fprintf (stdout, "%d\n", p->i);
}

void __ctest_delete (PCTEST p)
{
	free (p);
}
void __dtest_print (PDTEST p)
{
	fprintf (stdout, "[[%d]]\n", p->i);
}

PCTEST newctest (int k)
{
	PCTEST pc = (PCTEST) calloc (1, sizeof (CTEST));
	if (pc)
	{
		pc->i = k;
		pc->print = (void (*) (PCTEST)) (__ctest_print);
		pc->delete = (void (*) (PCTEST)) (__ctest_delete);
	}
	return pc;
}

PDTEST newdtest (int k)
{
	PDTEST pc = (PDTEST) newctest (k);	
	if (pc)
	{ 
		pc->print = (void (*) (PDTEST)) (__dtest_print);
	}
	return pc;
}
int main (int argc, char **argv)
{
	PCTEST pc = newctest (10);
	PDTEST pd = newdtest (20);

	pc->print(pc);
	pd->print(pd);

	pc->delete(pc);
	pd->delete(pd);	
	return 0;
}
