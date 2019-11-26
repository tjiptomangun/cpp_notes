#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned short rvsShort (unsigned short v)
{
	unsigned short result = v;
	result = (v << 8) & 0xFF00;
	result |= (v >> 8) & 0x00FF;
	return result;
}
#ifdef _BE_
#define htons rvsShort
#else
#define htons(x) (x)
#endif



void self_htons (unsigned short *inout, int numArray)
{
	unsigned short ret;
	int i = 0;

	for (i = 0; i < numArray; i++)
	{
		ret = inout [i];
		inout[i] = htons (ret);
	}
}
#define MAX_TEMP  4
#pragma pack(1)
typedef struct lstruct
{
#ifdef _LE_ 
	unsigned short a16;
	unsigned short b16;
	unsigned short a8 : 8;
	unsigned short b8 : 8;
	unsigned short a4 : 4;
	unsigned short b4 : 4;
	unsigned short c4 : 4;
	unsigned short d4 : 4; 
	unsigned short  e4 : 4; 
	unsigned short  u12 : 12; //unused
	unsigned k[MAX_TEMP]; 
	unsigned short  g4 : 4; //unused
	unsigned short  h4 : 4; //unused
	unsigned short  i4 : 4; //unused
	unsigned short  j4 : 4; //unused
#else
	unsigned short a16;
	unsigned short b16;
	unsigned short a8 : 8;
	unsigned short b8 : 8;
	unsigned short b4 : 4;
	unsigned short a4 : 4;
	unsigned short d4 : 4; 
	unsigned short c4 : 4;
	unsigned short  u4 : 4; //unused
	unsigned short  e4 : 4; 
	unsigned short  u8_2 : 8;
	unsigned k[MAX_TEMP]; 
	unsigned short  h4 : 4; //unused
	unsigned short  g4 : 4; //unused
	unsigned short  j4 : 4; //unused
	unsigned short  i4 : 4; //unused

#endif
	


}LSTRUCT;

typedef struct shortstruct
{
#ifdef _LE_
	unsigned short a4:4;
	unsigned short b4:4;
	unsigned short c4:4;
	unsigned short d4:4;
#else
	unsigned short b4:4;
	unsigned short a4:4;
	unsigned short d4:4;
	unsigned short c4:4;
#endif
}SSTRUCT;

typedef struct bytestruct
{
#ifdef _LE_ 
	unsigned short a4:4;
	unsigned short b4:4;
#else 
	unsigned short b4:4;
	unsigned short a4:4;
#endif
}BSTRUCT;

#pragma pack()

typedef struct kstruct
{
#ifdef _LE_
	unsigned int a4 : 4;
	unsigned int b4 : 4;
	unsigned int c4 : 4;
	unsigned int d4 : 4; 
	unsigned int e4 : 4;
	unsigned int f8 : 8;
#else 
	unsigned int b4 : 4;
	unsigned int a4 : 4;
	unsigned int d4 : 4; 
	unsigned int c4 : 4;
	unsigned int e4 : 4;
	unsigned int f8 : 8;
#endif
}KSTRUCT;
void print_SSTRUCT (SSTRUCT *s_l)
{
	printf ("s_l->a4= %d\n", s_l->a4);
	printf ("s_l->b4= %d\n", s_l->b4);
	printf ("s_l->c4= %d\n", s_l->c4);
	printf ("s_l->d4= %d\n", s_l->d4);
	printf ("\n===========================\n"); 
}

void print_BSTRUCT (BSTRUCT *b_l)
{
	printf ("b_l->a4= %d\n", b_l->a4);
	printf ("b_l->b4= %d\n", b_l->b4); 
	printf ("\n===========================\n"); 
}

void print_LSTRUCT (LSTRUCT *m_l)
{

	printf ("m_l->a16= %d\n", m_l->a16);
	printf ("m_l->b16= %d\n", m_l->b16);
	printf ("m_l->a8 = %d\n", m_l->a8);
	printf ("m_l->b8 = %d\n", m_l->b8);
	printf ("m_l->a4 = %d\n", m_l->a4);
	printf ("m_l->b4 = %d\n", m_l->b4);
	printf ("m_l->c4 = %d\n", m_l->c4);
	printf ("m_l->d4 = %d\n", m_l->d4);
	printf ("m_l->e4 = %d\n", m_l->e4);
	printf ("m_l->k[0] = %d\n", m_l->k[0]); 
	printf ("m_l->k[1] = %d\n", m_l->k[1]); 
	printf ("m_l->k[2] = %d\n", m_l->k[2]); 
	printf ("m_l->k[3] = %d\n", m_l->k[3]); 
	printf ("m_l->g4 = %d\n", m_l->g4);
	printf ("m_l->h4 = %d\n", m_l->h4);
	printf ("m_l->i4 = %d\n", m_l->i4);
	printf ("m_l->j4 = %d\n", m_l->j4);
	printf ("\n===========================\n"); 
		
}
#define MODE_READ 1
#define MODE_WRITE 2
#define STRUCT_LSTRUCT 0
#define STRUCT_SSTRUCT 1
#define STRUCT_BSTRUCT 2
#define OP_PLAIN 1
#define OP_HTONS 2
int usage ()
{ 
	fprintf (stderr, "usage: struct_test [r|w|p][l|s|b][p|h] filename\n");
	fprintf (stderr, "r,  read a file and print the result\n");
	fprintf (stderr, "w,  write to a file. if filename is null then writting to stdout\n");
	exit (EXIT_FAILURE);
}
int main (int argc, char **argv)
{

	LSTRUCT	m_l;
	SSTRUCT	m_s;
	BSTRUCT	m_b;
	FILE *fp;
	int Mode = 0;
	int cstruct = STRUCT_LSTRUCT;
	int operation = OP_PLAIN;
	//KSTRUCT	m_k;
	unsigned short *usArray;
#ifdef _BE_
	fprintf (stdout, "AIX\n");
#else
	fprintf (stdout, "LINUX\n");
#endif
	if (argc < 2)
		usage();
	if (argv[1][0] == 'r')	
		Mode = MODE_READ;
	if (argv[1][0] == 'w')
		Mode = MODE_WRITE;
	if (strlen (argv[1]) >= 2)
	{
		switch (argv[1][1])
		{
		case 's':
			cstruct = STRUCT_SSTRUCT;
			break;
		case 'b':
			cstruct = STRUCT_BSTRUCT;
			break; 
		}
		if (strlen (argv[1]) == 3)
		{
			switch (argv[1][2])
			{
			case 'p':
				operation = OP_PLAIN;
				break;
			case 'h':
				operation = OP_HTONS;
				break; 
			}

		}	
	}
	
	if (!Mode)
		usage();
	if (Mode == MODE_WRITE)
	{//aix here
		if (cstruct == STRUCT_LSTRUCT)
		{	
			m_l.a16 = 12;
			m_l.b16 = 13;
			m_l.a8 = 21;
			m_l.b8 = 31;
			m_l.a4 = 2;
			m_l.b4 = 3;
			m_l.c4 = 5;
			m_l.d4 = 7;
			m_l.e4 = 11;
			m_l.k[0] = 355;
			m_l.k[1] = 257;
			m_l.k[2] = 491;
			m_l.k[3] = 759;
			m_l.g4 = 1;
			m_l.h4 = 8;
			m_l.i4 = 12;
			m_l.j4 = 13;
			print_LSTRUCT (&m_l);
			if (argc < 3)
			{
				return EXIT_SUCCESS ;
			}
			fp = fopen (argv[2], "wb");
			if (!fp)
			{
				fprintf (stderr, "cannot open %s for writting\n", argv[2]);
				exit (EXIT_FAILURE);
			}
			if (operation == OP_HTONS)
			{
				usArray = (unsigned short *) &m_l;
				self_htons (&usArray[0], 2); }
			fwrite (&m_l, 1, sizeof (LSTRUCT), fp);
			fclose (fp);
		}
		else if (cstruct == STRUCT_SSTRUCT)
		{
			m_s.a4 = 1;
			m_s.b4 = 6;
			m_s.c4 = 13;
			m_s.d4 = 3;
			print_SSTRUCT (&m_s);
			if (argc < 3)
			{
				return EXIT_SUCCESS ;
			}
			fp = fopen (argv[2], "wb");
			if (!fp)
			{
				fprintf (stderr, "cannot open %s for writting\n", argv[3]);
				exit (EXIT_FAILURE);
			}
			if (operation == OP_HTONS)
			{
				usArray = (unsigned short *) &m_s;
			
			}
			fwrite (&m_s, 1, sizeof (SSTRUCT), fp);
		}
		else if (cstruct == STRUCT_BSTRUCT)
		{ 
			m_b.a4 = 1;
			m_b.b4 = 6;
			print_BSTRUCT (&m_b);
			if (argc < 3)
			{
				return EXIT_SUCCESS ;
			}
			fp = fopen (argv[2], "wb");
			if (!fp)
			{
				fprintf (stderr, "cannot open %s for writting\n", argv[2]);
				exit (EXIT_FAILURE);
			}
			fwrite (&m_b, 1, sizeof (BSTRUCT), fp);
		}
		
	}
	else if (Mode == MODE_READ)
	{
		fp = fopen (argv[2], "rb");
		if (!fp)
		{
			fprintf (stderr, "cannot open %s for writting\n", argv[2]);
			exit (EXIT_FAILURE);
		}
		if (cstruct == STRUCT_LSTRUCT)
		{
			fread (&m_l, 1, sizeof (LSTRUCT), fp);
			print_LSTRUCT (&m_l);	
		}
		else if (cstruct == STRUCT_SSTRUCT)
		{ 
			fread (&m_s, 1, sizeof (SSTRUCT), fp);
			print_SSTRUCT (&m_s);
		}
		else if (cstruct == STRUCT_BSTRUCT)
		{ 
			fread (&m_b, 1, sizeof (BSTRUCT), fp);
			print_BSTRUCT (&m_b);
		}
		fclose (fp); 
	}
	
/*
	self_htons (&usArray[5 +
		(MAX_TEMP * (sizeof (unsigned)/ sizeof (unsigned short)))], 1); 
*/

/*
	self_htons (&usArray[0], 2); 
	self_htons (&usArray[5 +
		(MAX_TEMP * (sizeof (unsigned)/ sizeof (unsigned short)))], 1); 
*/	

	return 0;
}
	

