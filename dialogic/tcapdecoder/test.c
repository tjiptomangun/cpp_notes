#include <stdio.h>
#include <inttypes.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "msg.h"
#include "sysgct.h"
#include "ss7_inc.h"
#include "tcp_inc.h"
//U06SSS-TCAP-PM.pdf



/**
 * Name     : getTlvStreamDataLength
 * Description  : get length of this data
 * Input 
 *  in  : stream to evaluate
 *  maxbyte : maximum byte to evaluate
 *  result  : result length
 * Returns
 *  length of length  in bytes
 */
int getTlvStreamDataLength (unsigned char * in, int maxlength,   int32_t* result)
{
    int64_t res = 0;
    int i = 0;
    res |= (in[i]&0x7F) ;
    i++;
    while ( (in[i-1] & 0x80))
    {
        res <<=7;
        res |= (in[i]&0x7F);
        i++;
    }
    *result = res;
    return i;

}

/**
 * Name     : HtoB
 * Description  : convert hexadecimal string into an array of byte array
 **/
int HtoB(unsigned char * pDst, char* pSrcHex, int nSrcLenHex)
{  
   char c;
   int nNibble1;
   int nNibble2;
   int i; 
   int j = 0;
   for (i = 0; i < nSrcLenHex; i += 2)
   {  
      c = pSrcHex[i]; 
      if (c >= '0' && c <= '9')
         nNibble1 = c - '0'; 
      else if (c >= 'A' && c <= 'F')
         nNibble1 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble1 = c - 'a' + 10;
      else
         nNibble1 = 0;
      c = pSrcHex[i+1]; 
      if (c >= '0' && c <= '9')
         nNibble2 = c - '0'; 
      else if (c >= 'A' && c <= 'F')
         nNibble2 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble2 = c - 'a' + 10;
      else
         nNibble2 = 0;
      *pDst++ = (nNibble1 << 4) + nNibble2;
	  j++;
   }
   return j;

}

void HtoB2 (unsigned char * pDst, char *pSrcHex, int srcLen)
{
    int i = 0;
    unsigned char temp;
    char *currSrc = pSrcHex;
    unsigned char *currDst = pDst;
    do
    {
        *currDst = 0;
        for (i = 0; i < 2; i++)
        {
            *currDst<<=4;
        	temp = *currSrc;
            temp &= 0x4F;
            *currDst += (temp >> 6)*10 + (temp & 0xF) - (temp >> 6);
            currSrc ++;
            srcLen --;
        }
        currDst ++;
    } while (srcLen);
}
unsigned char getTagClass(unsigned char tag) {
	unsigned char ret = tag & 0xc0 ;
	return ret >> 6;
}

unsigned char getTagForm(unsigned char tag) {
	unsigned char ret = tag & 0x20 ;
	return ret >> 5;
}

unsigned char getTagNumber(unsigned char tag) {
	printf(" %d (%02x", tag, tag);
	unsigned char ret = tag & 0x1F ;
	return ret;
}

char *tagClasses[] = {
	"Universal",
	"Application",
	"Context-Specific",
	"PRIVATE",
};

char *tagForms [] = {
	"Primitive", 
	"Construct"
};

//https://en.m.wikipedia.org/wiki/X.690#BER_encoding
char *tagNumbers [] = {
	"End Of Content (EOC)", //0
	"BOOLEAN",
	"INTEGER",
	"BIT STRING",
	"OCTET STRING",
	"NULL", //5
	"OBJECT IDENTIFIER",
	"Object Descriptor",
	"EXTERNAL",
	"REAL(float)",
	"ENUMERATED",//10
	"EMBEDDED PDV",
	"UTF8String",
	"RELATIVE-OID",
	"TIME",
	"Reserved",//15
	"SEQUENCE AND SEQUENCE OF",
	"SET AND SET OF",
	"NumericString",
	"PrintableString",
	"T61String",//20
	"VideotexString",
	"IA5String",
	"UTCTime",
	"GeneralizedTime",
	"GraphicString",//25
	"VisibleString",
	"GeneralString",
	"UniversalString",
	"CHARACTER STRING",
	"BMPString",//30
	"DATE",
	"TIME-OF-DAY",
	"DATE-TIME",
	"DURATION",
	"OID-IRI",//35
	"RELATIVE-OID-IRI"
};
void printTag(unsigned char tag, int identation) {
	unsigned char tagNum = getTagNumber(tag);
	unsigned char tagClass = getTagClass(tag);
	unsigned char tagForm = getTagForm(tag);
	if (tagNum < 31) {
		printf("%*c", identation, ' ');
		fprintf(stdout, "class = %s | form = %s | type = %s\n", tagClasses[tagClass], tagForms[tagForm], tagNumbers[tagNum]);
	}
}
void parseUpTo(unsigned char *stream, int max_len, int identation, int type, int context) {
	int currIdx = 0;
	int nextLen = max_len;
	unsigned char *pptr  = stream;
	int paramparamlen = 0;
	int lenoflen = 0;
	int i = 0;
	while (currIdx <= max_len) {
			unsigned char isConstruct = getTagForm(*pptr);
			printTag(*pptr, identation);
			pptr++; currIdx++;
			lenoflen = getTlvStreamDataLength (pptr, 100,  &paramparamlen);
			currIdx+= lenoflen;
			pptr+=lenoflen;
			nextLen -= lenoflen;
			if(isConstruct) {
				parseUpTo(pptr, paramparamlen, identation + 1, type, context);
				pptr+=paramparamlen;
				currIdx+=paramparamlen;
			}
			else {
				for(i=0;i< paramparamlen;i++) {
					printf("%02x",(unsigned char)pptr[i]);
				
				}
				printf("\n");
				pptr+=paramparamlen;
				currIdx+=paramparamlen;
				
			}
	}
	switch(type) {
		case TCP_MSG_CPT_REQ: //0xc781
		case TCP_MSG_CPT_IND: //0x8782
			break;
		case TCP_MSG_DLG_IND: //0x8784
		case TCP_MSG_DLG_REQ: //0xc783

	}

}


void parseUpToOld(unsigned char *stream, int max_len, int identation) {
	int currIdx = 0;
	int nextLen = max_len;
	unsigned char *pptr  = stream;
	int paramparamlen = 0;
	int lenoflen = 0;
	int i = 0;
	if (currIdx < max_len) {
			
			printTag(*pptr, identation);
			printf("\n");
		if (*pptr >= 0x80)  {
			if (*pptr >= 0xa0) {//tagged or constructed choice
				printf("%*c", identation, ' ');
				printf("tagged / constructed choice %d\n", *pptr - 0xa0);
				pptr++;
				lenoflen = getTlvStreamDataLength (pptr, 100,  &paramparamlen);
				pptr+=lenoflen;
				nextLen -= lenoflen;
				parseUpTo(pptr, paramparamlen, identation + 1);

			} else {//primitive choice
				printf("primitive choice %d\n", *pptr - 0x80);
				pptr++;
				lenoflen = getTlvStreamDataLength (pptr, 100,  &paramparamlen);
				pptr+=lenoflen;
				nextLen -= lenoflen;
				for(i=0;i< paramparamlen;i++) {
					printf("%02x",(unsigned char)pptr[i]);
				
				}
			}
		}
		else if (*pptr == 0x30)  {//sequence
		}
		else if (*pptr == 0x01)  {//boolean
		}
		else if (*pptr == 0x05)  {//null
		}
		else if (*pptr == 0x02)  {//integer

		}
		else if (*pptr == 0x04)  {//octet string
			printf("octet string");
		}
		else if (*pptr == 0x03)  {//bit string
		}
		else if (*pptr == 0x23)  {//constructed bit string
		}
	}

}

void decode(int type, unsigned char *param, int paramlen) {
	unsigned char  *pptr = param;
	unsigned char primitive_type = *pptr;
	unsigned char param_name;
	int lenoflen ;
	int paramparamlen;
	int i;
	char *emptyStr = "";
	char *dialog_primitive_types [] = {
		"",
		"TCPPT_TC_UNI",
		"TCPPT_TC_BEGIN",
		"TCPPT_TC_CONTINUE",
		"TCPPT_TC_END",
		"TCPPT_TC_U_ABORT",
		"TCPPT_TC_P_ABORT",
		"TCPPT_TC_NOTICE",
		
	};
	char *dialog_param_names [] = {
		"","","","","","",
		"TCPPN_QOS",//6
		"TCPPN_DEST_ADDR",//7
		"TCPPN_ORIG_ADDR",//8
		"TCPPN_TERMINATION",//9
		"TCPPN_ABORT_REASON",//10
		"TCPPN_REPORT_CAUSE",//11
		"TCPPN_CPT_PRESENT",//12
		"TCPPN_APPL_CONTEXT",//13
		"TCPPN_USER_INFO",//14
		"TCPPN_P_ABORT",//15
		"TCPPN_UABORT_INFO",//16
		"TCPPN_SECURITY",//17
		"TCPPN_CONFIDENTIALITY",//18
		"TCPPN_PERMISSION",//19
		"TCPPN_VERSION",//20
		"TCPPN_NC",//21
		"TCPPN_DLG_IDLE_TIMEOUT",//22
	};
	char *component_primitive_types [] = {
		"TCCPT_TC_NULL",
		"","","","","","","",
		"TCCPT_TC_INVOKE",
		"TCCPT_TC_RESULT_L",
		"TCCPT_TC_RESULT_NL",
		"TCCPT_TC_U_ERROR",
		"TCCPT_TC_L_CANCEL",
		"TCCPT_TC_U_CANCEL",
		"TCCPT_TC_L_REJECT",
		"TCCPT_TC_R_REJECT",
		"TCCPT_TC_U_REJECT",
		"TCCPT_TC_INVOKE_NL",
	};
	typedef struct tag_components {
		char * name;
		int isConstruct;
	} TAG_COMPONENTS;

	TAG_COMPONENTS component_params[] ={
		{
			"", 0
		}, {
			"TCPPN_COMPONENT", 1
		},
		{
			"TCPPN_LAST_CPT", 0
		},{
			"TCPPN_CLASS", 0
		},{
			"TCPPN_TIMEOUT", 0
		},{
			"TCPPN_INVOKE_ID", 0
		},{
			"TCPPN_NC", 0
		},{
			"TCPPN_CODESHIFT", 0
		}
	} ;
	char *component_param_names [256] = {
		"",
		"TCPPN_COMPONENT",
		"TCPPN_LAST_CPT",
		"TCPPN_CLASS",
		"TCPPN_TIMEOUT",
		"TCPPN_INVOKE_ID",
	};
	for (i = 6; i< 256; i++) {
		component_param_names[i] = emptyStr;
	}
	component_param_names[21] = "TCPPN_NC";
	component_param_names[255] = " TCPPN_CODE_SHIFT";


	switch(type) {
		case TCP_MSG_DLG_IND: //0x8784
		case TCP_MSG_DLG_REQ: //0xc783
			if (type == TCP_MSG_DLG_IND ) {
				printf("TCP_MSG_DLG_IND : 0x%04x  === ", type);
			} else {
				printf("TCP_MSG_DLG_REQ : 0x%04x  === ", type);
			}
			
			for(i=0;i< paramlen;i++) {
				printf("%02x",(unsigned char)pptr[i]);
				
			}
			printf (" ===");
			printf("\nDialogue Primitive Type %02x ", primitive_type);
			printf("%s \n", dialog_primitive_types[primitive_type]);
			pptr++;
			param_name = *pptr;
			while(!(*pptr == 0x00 && *(pptr+1) == 0x00)) {
				printf("\nDialogue Parameter Name %02x ", param_name);
				printf("%s ", dialog_param_names[param_name]);
				pptr++;
				lenoflen = getTlvStreamDataLength (pptr, 100,  &paramparamlen);
				printf("length %d = ", paramparamlen);
				pptr+=lenoflen;
				for(i=0;i< paramparamlen;i++) {
					printf("%02x",(unsigned char)pptr[i]);
					
				}
				

				pptr+=paramparamlen;
				param_name = *pptr;			
				printf("\n");

			}
			break;

		case TCP_MSG_CPT_REQ: //0xc781
		case TCP_MSG_CPT_IND: //0x8782
			if (type == TCP_MSG_CPT_REQ ) {
				printf("TCP_MSG_CPT_REQ : 0x%04x  === ", type);
			} else {
				printf("TCP_MSG_CPT_IND : 0x%04x  === ", type);
			}
			
			for(i=0;i< paramlen;i++) {
				printf("%02x",(unsigned char)pptr[i]);
				
			}
			printf (" ===");

			printf("\nComponent Primitive Type %02x ", primitive_type);
			printf("%s \n", component_primitive_types[primitive_type]);
			pptr++;
			param_name = *pptr;
			while(!(*pptr == 0x00 && *(pptr+1) == 0x00)) {
				printf("\nComponent Parameter Name %02x ", param_name);
				printf("%s ", component_params[param_name].name);
				pptr++;
				lenoflen = getTlvStreamDataLength (pptr, 100,  &paramparamlen);
				printf("length %d = ", paramparamlen);
				pptr+=lenoflen;
				for(i=0;i< paramparamlen;i++) {
					printf("%02x",(unsigned char)pptr[i]);
					
				}
				printf("\n");
				if (component_params[param_name].isConstruct) {
					parseUpTo(pptr, paramparamlen, 1, type, 0);
				}

				pptr+=paramparamlen;
				param_name = *pptr;
				printf("\n");

			}
			break;
		case TCP_MSG_MAINT_IND:
			printf("TCP_MSG_MAINT_IND");
			printf("\n");
			break;
		case TCP_MSG_ERROR_IND:
			printf("TCP_MSG_ERROR_IND");
			printf("\n");
			break;
		case SYS_MSG_CONGESTION:
			printf("SYS_MSG_CONGESTION");
			printf("\n");
			break;
	}
}
void usage(char *x) {
	printf("%s d|g|c|t|m hexstring\n", x);
	printf("d: dialog_request, g: dialog_indication, c: component_request, t: component_indicator, m: maintenance\n");
}

int main(int argc, char **argv) {
	unsigned char dialog_stream[600] = {0};
	int dialog_stream_len = 0;
	unsigned char component_stream[600] = {0};
	int component_stream_len = 0;
	unsigned char maint_stream[600] = {0};
	int maint_stream_len = 0;
	int opt;
	int optlong;
	int option_index = 0;
	static struct option long_options[]	 = {
		{"dialog_request", required_argument, 0, 'd'},
		{"dialog_indicator", required_argument, 0, 'g'},
		{"component_request", required_argument, 0, 'c'},
		{"component_indicator", required_argument, 0, 't'},
		{"maint", required_argument, 0, 'm'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};
	unsigned int dialog_present = 0;
	unsigned int component_present = 0;
	unsigned int maint_present = 0;
	while ((opt = getopt_long(argc, argv, "d:c:m:h", long_options,&option_index))!= -1) {
		switch (opt) {
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			case 'd':
			case 'g':
				dialog_stream_len =  HtoB(dialog_stream, optarg, strlen(optarg));
				if (opt == 'd') {
					dialog_present = TCP_MSG_DLG_REQ;
				} else {
					dialog_present = TCP_MSG_DLG_IND;
				}
				break;				
			case 'c':
			case 't':
				component_stream_len = HtoB(component_stream, optarg, strlen(optarg));
				if (opt == 'c') { 
					component_present = TCP_MSG_CPT_REQ;
				} else {
					component_present = TCP_MSG_CPT_IND;
				}
        		break;
			case 'm':
				maint_present = 1;
				maint_stream_len =  HtoB(maint_stream, optarg, strlen(optarg));
				break;
			
		}
	}
	if(dialog_present) {
		decode(dialog_present, dialog_stream, dialog_stream_len);
	} 
	if(component_present) {
		decode(component_present, component_stream, component_stream_len);
	}
}
