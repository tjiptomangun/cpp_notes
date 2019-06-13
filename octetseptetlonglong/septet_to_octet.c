/**
 * gcc -Wall -ggdb3 septet_to_octet.c -o septet_to_octet
 */
#include <stdio.h>

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
      unsigned char a = in[i] >> (8 - d);
      unsigned char b = in[i + 1] << d;
      fprintf (stdout, "with i = %d in[i + 1] = %02X in [i] = %02X\n", i, in[i + 1], in[i]);
      fprintf (stdout, "j = %d l = %02X r = %02X\n",j,  b, a);
      out[j] = ((in[i] >> (8 - d))  | (in[i + 1] << d)) & 0x7F;
    }
  }
  return max_length;
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
}
