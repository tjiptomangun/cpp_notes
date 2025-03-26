#include <stdio.h>
#include <string.h>
#include "portable.h"
#include "xuimsg.h"

void printBytes(unsigned char * pData, int nLen)
{
	int i;

	for(i = 0 ; i < nLen; i++)
	{	
		if(i%16 == 0) 
			printf("\n");
		else if(i%2 == 0 && i >0)
			printf("   ");
		printf("%02X ",pData[i]);
		 	
	}	
	printf("\n\n");
}

int main(int argc, char **argv)
{

	XL_ATM_HW_FITNESS XLAtmFitness;
	XL_NDC_HW_FITNESS NdcFitness;
	STRUCT_TEST m;
	unsigned short *temp, *anchor;
	int i;
	unsigned char k = 'a';
	unsigned char *pData;
	unsigned short l = 0x2332;
	printf("test data %c %02x",k,k);
	printBytes(&k,1);
	printf("short data %04x",l);
	printBytes(&l,2);
	m.a = 5;
	m.b = 6;
	m.c = 4;
	m.d = 11;
	printf("trying  :4-%02d :4-%02d :4-%02d :4-%02d",m.a, m.b, m.c, m.d);
	printBytes(&m,2);
	
	int NdcFitSz = sizeof(NdcFitness);
	memset(&NdcFitness, 0 , NdcFitSz);
	XLAtmFitness.nTimeOfDayClock = 1;
	XLAtmFitness.nHighOrderComm = 3;
	XLAtmFitness.nSystemDisk = 0;
	XLAtmFitness.nCardReader = 2;
	XLAtmFitness.nDispenser  = 4;
	XLAtmFitness.nDepository = 0;
	XLAtmFitness.nReceiptPrinter = 0;
	XLAtmFitness.nJournalPrinter = 0;
	XLAtmFitness.nNightSafeDepository = 0;
	XLAtmFitness.nEncryptor = 0;
	XLAtmFitness.nCamera = 0;
	XLAtmFitness.nDoorAccess = 0;
	XLAtmFitness.nFlexDisk = 0;
	XLAtmFitness.nUNUSED = 0;
	for(i = 0; i < XL_ATM_MAX_DENOMINATION_TYPE ; i++)
	{
		XLAtmFitness.nDenomination[i] = 0;
	}
	XLAtmFitness.nDenomination[1] = 1;
	XLAtmFitness.nStatementPrinter = 0;

			
	pData= (unsigned char * ) &NdcFitness;
#ifdef _LE_

	NdcFitness.nTimeOfDayClock		= htons (XLAtmFitness.nTimeOfDayClock);
	NdcFitness.nHighOrderComm 		= htons (XLAtmFitness.nHighOrderComm);
	NdcFitness.nSystemDisk 			= htons (XLAtmFitness.nSystemDisk);
	NdcFitness.nCardReader 			= htons (XLAtmFitness.nCardReader);
	NdcFitness.nDispenser 			= htons (XLAtmFitness.nDispenser);
	NdcFitness.nDepository 			= htons (XLAtmFitness.nDepository);
	NdcFitness.nReceiptPrinter 		= htons (XLAtmFitness.nReceiptPrinter);
	NdcFitness.nJournalPrinter 		= htons (XLAtmFitness.nJournalPrinter);
	NdcFitness.nNightSafeDepository = htons (XLAtmFitness.nNightSafeDepository);
	NdcFitness.nEncryptor 			= htons (XLAtmFitness.nEncryptor);
	NdcFitness.nCamera 				= htons (XLAtmFitness.nCamera);
	NdcFitness.nDoorAccess 			= htons (XLAtmFitness.nDoorAccess);
	NdcFitness.nFlexDisk 			= htons (XLAtmFitness.nFlexDisk);

	for (i =0; i <XL_NDC_MAX_CASSETTE; i ++) {
		NdcFitness.Cassette[i] = htonl (XLAtmFitness.nDenomination[i]);
	}
	
	NdcFitness.nStatementPrinter 	= htons (XLAtmFitness.nStatementPrinter);
	NdcFitness.nUNUSED2		= htons (0); 


#else
	
	printBytes(pData,NdcFitSz);
	NdcFitness.nTimeOfDayClock		= XLAtmFitness.nTimeOfDayClock;
	printf("tod atm %02x\n", XLAtmFitness.nTimeOfDayClock);
	printf("tod ndc %02x\n", NdcFitness.nTimeOfDayClock);

	printBytes(pData,1);
	printBytes(&XLAtmFitness,1);
	NdcFitness.nHighOrderComm 		= XLAtmFitness.nHighOrderComm;
	printBytes(pData,1);

	printf("ndc %x\n", NdcFitness);


	printBytes(pData,NdcFitSz);
	NdcFitness.nSystemDisk 			= XLAtmFitness.nSystemDisk;

	printBytes(pData,NdcFitSz);
	NdcFitness.nCardReader 			= XLAtmFitness.nCardReader;

	
	NdcFitness.nDispenser 			= XLAtmFitness.nDispenser;
	NdcFitness.nDepository 			= XLAtmFitness.nDepository;
	NdcFitness.nReceiptPrinter 		= XLAtmFitness.nReceiptPrinter;
	NdcFitness.nJournalPrinter 		= XLAtmFitness.nJournalPrinter;

	
	NdcFitness.nNightSafeDepository = XLAtmFitness.nNightSafeDepository;
	NdcFitness.nEncryptor 			= XLAtmFitness.nEncryptor;
	NdcFitness.nCamera 				= XLAtmFitness.nCamera;
	NdcFitness.nDoorAccess 			= XLAtmFitness.nDoorAccess;
	NdcFitness.nFlexDisk 			= XLAtmFitness.nFlexDisk;
	

	for (i =0; i <XL_NDC_MAX_CASSETTE; i ++) {
		NdcFitness.Cassette[i] = htonl (XLAtmFitness.nDenomination[i]);
	}
	
	NdcFitness.nStatementPrinter 	= XLAtmFitness.nStatementPrinter;
	NdcFitness.nUNUSED2		= htons (0); 

	i = 0;
	printBytes(pData,NdcFitSz);
	anchor = (unsigned short *)&NdcFitness;

	temp  = &anchor[i++];//TOD
	*temp = htons(*temp);
	printBytes(pData,NdcFitSz);

	temp = &anchor[i++];//DISP
	*temp = htons(*temp);
	printBytes(pData,NdcFitSz);

	temp = &anchor[i++];//NSD
	*temp = htons(*temp);
	printBytes(pData,NdcFitSz);


	temp = &anchor[i];//FlexD
	*temp = htons(*temp);
	i+=8;

	temp = &anchor[i++];
	*temp = htons(*temp);


#endif


        printf("\n\nfinal\n\n");
	printBytes(pData, NdcFitSz);

	return 0;

}

