#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//https://github.com/RoyalAliceAcademyOfSciences/PhoneOverIp/blob/master/includes/pdu.c

// Encode PDU message by merging 7 bit ASCII characters into 8 bit octets.

enum {
	BITMASK_7BITS = 0x7F,
	BITMASK_8BITS = 0xFF,
	BITMASK_HIGH_4BITS = 0xF0,
	BITMASK_LOW_4BITS = 0x0F,

	TYPE_OF_ADDRESS_INTERNATIONAL_PHONE = 0x91,
	TYPE_OF_ADDRESS_NATIONAL_SUBSCRIBER = 0xC8,

	SMS_DELIVER_ONE_MESSAGE = 0x04,
	SMS_SUBMIT              = 0x11,

	SMS_MAX_7BIT_TEXT_LENGTH  = 160,
};
static int EncodePDUMessage(const char* sms_text, int sms_text_length, unsigned char* output_buffer, int buffer_size)
{
	// Check if output buffer is big enough.
	if ((sms_text_length * 7 + 7) / 8 > buffer_size)
		return -1;

	int output_buffer_length = 0;
	int carry_on_bits = 1;
	int i = 0;

	for (; i < sms_text_length - 1; ++i) {
		output_buffer[output_buffer_length++] =
			((sms_text[i] & BITMASK_7BITS) >> (carry_on_bits - 1)) |
			((sms_text[i + 1] & BITMASK_7BITS) << (8 - carry_on_bits));
		carry_on_bits++;
		if (carry_on_bits == 8) {
			carry_on_bits = 1;
			++i;
		}
	}


	if (i < sms_text_length)
		output_buffer[output_buffer_length++] =	(sms_text[i] & BITMASK_7BITS) >> (carry_on_bits - 1);

	return output_buffer_length;
}

// Decode PDU message by splitting 8 bit encoded buffer into 7 bit ASCII
// characters.
static int DecodePDUMessage(const unsigned char* buffer, int buffer_length, char* output_sms_text, int sms_text_length)
{
	int output_text_length = 0;
	if (buffer_length > 0)
		output_sms_text[output_text_length++] = BITMASK_7BITS & buffer[0];

	int carry_on_bits = 1;
	int i = 1;
	for (; i < buffer_length; ++i) {

		output_sms_text[output_text_length++] = BITMASK_7BITS &	((buffer[i] << carry_on_bits) | (buffer[i - 1] >> (8 - carry_on_bits)));

		if (output_text_length == sms_text_length) break;

		carry_on_bits++;

		if (carry_on_bits == 8) {
			carry_on_bits = 1;
			output_sms_text[output_text_length++] = buffer[i] & BITMASK_7BITS;
			if (output_text_length == sms_text_length) break;
		}

	}
	if (output_text_length < sms_text_length)  // Add last remainder.
		output_sms_text[output_text_length++] =	buffer[i - 1] >> (8 - carry_on_bits);

	return output_text_length;
}


void encode_decode_pdu_to_ascii_test(unsigned char *in_pdu_message, int in_length) {
	char out_ascii[40] = {0};
	unsigned char out_pdu[40] = {0};
  int i;

	int ascii_length = DecodePDUMessage(in_pdu_message, in_length, out_ascii, 40);
	fprintf(stdout, "input pdu in hex = ");
	for (int i = 0; i < in_length; i++){
		fprintf(stdout, " %02X ", in_pdu_message[i]);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "output in hex = ");
	for (i = 0; i < strlen(out_ascii); i++) { 
		fprintf(stdout, " %02X ", out_ascii[i]);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "output in ascii = "); 
	for (i = 0; i < ascii_length; i++) { 
		fprintf(stdout, " %c ", out_ascii[i]);
	}
	
	fprintf(stdout, "\n");

	int pdu_length = EncodePDUMessage(out_ascii, ascii_length, out_pdu, 40);
	fprintf(stdout, "decoded back = "); 
	for (i = 0; i < pdu_length; i++) { 
		fprintf(stdout, " %02X ", out_pdu[i]);
	}
	
	fprintf(stdout, "\n");

}

void encode_decode_ascii_to_pdu_test(char *in_ascii, int ascii_length) {
	int i;
	unsigned char out_pdu[40] = {0};
	char out_ascii[40] = {0};
	int pdu_length = EncodePDUMessage(in_ascii, ascii_length, out_pdu, 40);
	fprintf(stdout, "input in ascii = "); 
	for (i = 0; i < ascii_length; i++) { 
		fprintf(stdout, " %c ", in_ascii[i]);
	}
	
	fprintf(stdout, "\n");
  
	fprintf(stdout, "input in hex = ");
	for (i = 0; i < ascii_length; i++) { 
		fprintf(stdout, " %02X ", in_ascii[i]);
	}
	
	fprintf(stdout, "\n");
  
	fprintf(stdout, "encoded pdu = "); 
	for (i = 0; i < pdu_length; i++) { 
		fprintf(stdout, " %02X ", out_pdu[i]);
	}
	
	fprintf(stdout, "\n");
  
	ascii_length = DecodePDUMessage(out_pdu, pdu_length, out_ascii, 40);
  
	fprintf(stdout, "decoded back to ascii = "); 
	for (i = 0; i < ascii_length; i++) { 
		fprintf(stdout, " %02X ", out_ascii[i]);
	}
	fprintf(stdout, "\n");
  
}

int main(int argc, char **argv) {
	unsigned char in_pdu_message[] = {0x2A, 0x18, 0x4C, 0xA6, 0xA2, 0x8D, 0x1A};
  char in_ascii[] = {0x2A, 0x30, 0x30, 0x32, 0x2A, 0x34, 0x23};
	
	char *in_ascii2 = "hei";

	encode_decode_pdu_to_ascii_test(in_pdu_message, 7);
	encode_decode_ascii_to_pdu_test(in_ascii, 7);
	encode_decode_pdu_to_ascii_test(in_pdu_message, 6);
	
	encode_decode_ascii_to_pdu_test(in_ascii2, 3);

}
