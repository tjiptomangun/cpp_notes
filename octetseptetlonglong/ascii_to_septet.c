/**
 * gcc -Wall -ggdb3 septet_to_octet.c -o septet_to_octet
 */
#include <stdio.h>
#include <string.h>

int septet_to_octet (unsigned char in[], int in_length, unsigned char out[], int out_length) {
  int i;
  int j;
  int d;
  int max_length = in_length * 8 / 7;

  for (j = 0; j < max_length; j ++ ){
    i = j * 7 / 8;
    d = j & 7;
    if (!d) {
      out[j] = in[i] & 0x7F;
    }
    else {
      /*
      unsigned char a = in[i] >> (8 - d);
      unsigned char b = in[i + 1] << d;
      fprintf (stdout, "with i = %d in[i + 1] = %02X in [i] = %02X\n", i, in[i + 1], in[i]);
      fprintf (stdout, "j = %d l = %02X r = %02X\n",j,  b, a);
      */
      out[j] = ((in[i] >> (8 - d))  | (in[i + 1] << d)) & 0x7F;
    }
  }
  return max_length;
}

unsigned char char_hex_to_dec (char *sBil)
{
	int 	nI;

	for (nI = 0; nI < 2; nI++)
	{
		if ((sBil[nI] >= 0x30 && sBil[nI] <= 0x39) ||
             (sBil[nI] >= 0x41 && sBil[nI] <= 0x46) ||
		     (sBil[nI] >= 0x61 && sBil[nI] <= 0x66))
		{
			if ((sBil[nI] >= 0x41 && sBil[nI] <= 0x46) ||
				 (sBil[nI] >= 0x61 && sBil[nI] <= 0x66))
			{
				sBil[nI] = sBil[nI] + 9;
			}
		}
		else
		{
			return (0);
		}

	}

	return ((sBil[1] & 0x0f) + (16 * (sBil[0] & 0x0f)));

}
int octet_to_septet_bytes(char *sBuff, unsigned char *sText, int nLenText){
	unsigned char nShift = 0, nShiftOld = 0;
	unsigned char nBil;
	int 	nIndex = 1;  // index untuk textnya
	int 	nIndx = 0;   // index untuk buffernya
	int 	nI;
	
// 		int idx; // input of signed char index
// 		int odx; // output of unsigned char index
// 		unsigned char carry;
// 		int shlness;
// 		int shrness; //number of bits that will be carry
// 		for (idx = 0, odx = 0; idx< nLenText; idx ++, odx++){
// 			shrness = 7 - (idx % 8);
// 			
// 		}

	while (/*nIndx != (nBuffLen /2)*/ nIndex < nLenText)
	{

		if ((nIndex % 8 != 0) || nIndex == 0)
		{
			// ambil buffer per 2 byte dan convert ke decimal
// 			memcpy (szBil, sBuff + (nIndx * 2), 2);
// 			nBil = char_hex_to_dec(szBil);
     
      nBil = (unsigned long)sBuff[nIndx];

			nShiftOld = nShift;

			nShift = nBil;

// 			geser ke kanan untuk dapatkan shiftnya
			for (nI = 7; nI >= (nIndex % 8); nI--)
			{
				nShift = nShift >> 1;
			}

// 			geser ke kiri per bit (sampai 8 bit balik lagi ke 1)
			for (nI = 1; nI < (nIndex % 8); nI++)
			{
				nBil = nBil << 1;
			}


// 			tambahkan dengan shift yang sebelumnya
			nBil = nBil | nShiftOld;
			nIndx++;
		}
		else
		{
			nBil = nShift;
			nShift = 0;

		}

// 		kosongkan bit ke 7
		nBil = nBil & 0x7F;

		sText[nIndex-1] = nBil;

		nIndex++;
	}

	sText[nIndex-1] = '\0';

	return (0);

}
int octet_to_septet_ascii (char *sBuff, char *sText, int nLenText)
{
	unsigned char nShift = 0, nShiftOld = 0;
	unsigned char nBil;
	int 	nIndex = 1;  // index untuk textnya
	int 	nIndx = 0;   // index untuk buffernya
	int 	nI;
	char 	szBil[3] = {0};

	while (/*nIndx != (nBuffLen /2)*/ nIndex != nLenText+1)
	{

		if ((nIndex % 8 != 0) || nIndex == 0)
		{
			// ambil buffer per 2 byte dan convert ke decimal
			memcpy (szBil, sBuff + (nIndx * 2), 2);
			nBil = char_hex_to_dec(szBil);

			nShiftOld = nShift;

			nShift = nBil;

			// geser ke kanan untuk dapatkan shiftnya
			for (nI = 7; nI >= (nIndex % 8); nI--)
			{
				nShift = nShift >> 1;
			}

			//geser ke kiri per bit (sampai 8 bit balik lagi ke 1)
			for (nI = 1; nI < (nIndex % 8); nI++)
			{
				nBil = nBil << 1;
			}


			// tambahkan dengan shift yang sebelumnya
			nBil = nBil | nShiftOld;
			nIndx++;
		}
		else
		{
			nBil = nShift;
			nShift = 0;

		}

		// kosongkan bit ke 7
		nBil = nBil & 0x7F;

		sText[nIndex-1] = nBil;

		nIndex++;
	}

	sText[nIndex-1] = '\0';

	return (0);

}
int main (int argc, char ** argv) {
/*
  char sept_in[] ={0x89, 0x85, 0x42, 0x0C, 0x06, 0x03, 0x60, 0x00};
  char asserter[] = {0x09, 0x0B, 0x0A, 0x62, 0x60, 0x60, 0x00, 0x30};
  char oct_out[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
*/
  char sept_in[] ={0x89, 0x85, 0x42, 0x0C, 0x06, 0x03, 0x60, 0x01, 0x89, 0x85, 0x42, 0x0C, 0x06, 0x03};
  char asserter[] = {0x09, 0x0B, 0x0A, 0x62, 0x60, 0x60, 0x00, 0x30, 0x01, 0x12, 0x16, 0x14, 0x44, 0x41, 0x41, 0x01};
  char oct_out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  int i = 0; 
  int out_length;
 
  out_length = septet_to_octet((unsigned char *)sept_in, 14, (unsigned char *)oct_out, 16);
  for (i = 0; i < out_length; i ++) {
    if (oct_out[i] != asserter[i]){
      fprintf(stderr, "err in idx %d out %02X expect %2X \n", i, oct_out[i], asserter[i]);
    }
  } 
  fprintf(stdout, "done\n");

  fprintf(stdout, "test ussd\n");
  unsigned char sept_in2[] = {0x2A, 0x18, 0x4C, 0xA6, 0xA2, 0x8D, 0x1A, 0x00};
  fprintf(stdout, "\n=======the septet in hex========\n");
  for(i = 0; i< 7 ; i++){
	fprintf(stdout, " %02X ", sept_in2[i]);
  }
  memset (oct_out, 0, 16);
  out_length = septet_to_octet((unsigned char *)sept_in2, 7, (unsigned char *)oct_out, 16);
  fprintf(stdout, "\n=======the hex========\n");
  for(i = 0; i< out_length; i++){
	fprintf(stdout, " %02X ", oct_out[i]);
  }

  fprintf(stdout, "\n======the ascii=======\n");
  for(int i = 0; i< out_length; i++){
	fprintf(stdout, "%c", oct_out[i]);
  } 
  unsigned char oct_out2[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  fprintf(stdout, "\n=======inverse========\n");
  octet_to_septet_bytes (oct_out, oct_out2, out_length);
  for(int i = 0; oct_out2[i]!= 0; i++){
	fprintf(stdout, " %02X ", oct_out2[i]);
  } 

  fprintf(stdout, "\n======done====\n");

  fprintf(stdout, "\n=======the septet in hex========\n");
  for(i = 0; i< 6 ; i++){
	fprintf(stdout, " %02X ", sept_in2[i]);
  }
  memset (oct_out, 0, 16);
  out_length = septet_to_octet((unsigned char *)sept_in2, 6, (unsigned char *)oct_out, 16);
  fprintf(stdout, "\n=======the hex========\n");
  for(i = 0; i< out_length; i++){
	fprintf(stdout, " %02X ", oct_out[i]);
  }

  fprintf(stdout, "\n======the ascii=======\n");
  for(int i = 0; i< out_length; i++){
	fprintf(stdout, "%c", oct_out[i]);
  }
  fprintf(stdout, "\n======done====\n");

}
