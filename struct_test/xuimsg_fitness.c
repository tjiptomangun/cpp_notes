#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "xuimsg.h"

int usage ()
{
	fprintf (stderr, "usage : xuimsg_fitness [r/w] [|file\n");
	fprintf (stderr, "     r read from file, w write to file\n");
	fprintf (stderr, "     if file is empty and option is write\n");
	fprintf (stderr, "     then this program will write the value to stdout\n");
	exit (EXIT_FAILURE);
}

int printNDCFitness (XL_NDC_HW_FITNESS *pHWFitness)
{ 
	int nDx = 0;
	printf("\nHWFitness\n------------------\n");
	printf("nTimeOfDayClock         = %d\n",pHWFitness->nTimeOfDayClock);
	printf("nHighOrderComm          = %d\n",pHWFitness->nHighOrderComm);
	printf("nSystemDisk             = %d\n",pHWFitness->nSystemDisk);
	printf("nCardReader             = %d\n",pHWFitness->nCardReader);
	printf("nDispenser              = %d\n",pHWFitness->nDispenser);
	printf("nDepository             = %d\n",pHWFitness->nDepository);
	printf("nReceiptPrinter         = %d\n",pHWFitness->nReceiptPrinter);
	printf("nJournalPrinter         = %d\n",pHWFitness->nJournalPrinter);
	printf("nNightSafeDepository    = %d\n",pHWFitness->nNightSafeDepository);
	printf("nEncryptor              = %d\n",pHWFitness->nEncryptor);
	printf("nCamera                 = %d\n",pHWFitness->nCamera);
	printf("nDoorAccess             = %d\n",pHWFitness->nDoorAccess);
	printf("nFlexDisk               = %d\n",pHWFitness->nFlexDisk);

	for(nDx = 0; nDx<XL_NDC_MAX_CASSETTE; nDx++)
	{
		printf("Cassette[%d] = %d\n",nDx,pHWFitness->Cassette[nDx]);
	}
	printf("nStatementPrinter       = %d\n",pHWFitness->nStatementPrinter);
	return 0; 
}
#define MODE_WRITE 1
#define MODE_READ 2
int main (int argc, char **argv)
{
	int mode = 0;
	FILE *fp;
	XL_NDC_HW_FITNESS hwFitness;
	if (argc < 2)
	{
		usage();	
	}
	switch (argv[1][0])
	{
	case 'r':
		mode = MODE_READ;
		break;
	case 'w':
		mode = MODE_WRITE;
		break; 
	}
	if (!mode)
		usage();
	if (mode == MODE_WRITE)
	{ 
		memset (&hwFitness, 0, sizeof(XL_NDC_HW_FITNESS));
		hwFitness.nDispenser = 4; 

		if (argc < 3)
		{
			printNDCFitness (&hwFitness);
			exit (0);
		}
		fp = fopen (argv[2], "wb");
		if (!fp)
		{
			fprintf (stderr, "failed openning %s\n", argv[2]);
			exit (EXIT_FAILURE);
		}
		fwrite (&hwFitness, 1, sizeof (XL_NDC_HW_FITNESS), fp);
		fclose (fp);

	}
	else
	{ 
		memset (&hwFitness, 0, sizeof(XL_NDC_HW_FITNESS));
		fp = fopen (argv[2], "rb");
		fread (&hwFitness, 1, sizeof (XL_NDC_HW_FITNESS), fp);
		fclose (fp);
		printNDCFitness (&hwFitness);

	}
	exit (EXIT_SUCCESS);
}
