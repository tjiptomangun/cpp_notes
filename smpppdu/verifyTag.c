#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t
int isInValidTag(u16 tag) {
    return 
        tag < 5 || 
        (tag > 0x8  && tag < 0x0D) ||
        (tag > 0x10  && tag < 0x17) ||
        (tag == 0x18) ||
        (tag > 0x19  && tag < 0x1D) ||
        (tag > 0x1E  && tag < 0x30) ||
        (tag > 0x30  && tag < 0x201) ||
        (tag > 0x205  && tag < 0x20A) ||
        (tag > 0x210  && tag < 0x302) ||
        (tag > 0x304 && tag < 0x381) ||
        (tag > 0x381 && tag < 0x420) ||
        (tag > 0x428 && tag < 0x501) ||
        (tag > 0x501 && tag < 0x600) ||
        (tag > 0x613 && tag < 0x1201) ||
        tag == 0x1202 ||
        (tag > 0x1204  && tag < 0x130C) ||
        (tag > 0x130C  && tag < 0x1380) ||
        tag == 0x1381 ||  tag == 0x1382 ||
        tag > 0x1383 
    ;

}

// int isInValidTag(u16 tag) {
//     return 
//         tag < 5 || 
//         (tag > 0x8  && tag < 0x0D) ||
//         (tag > 0x10  && tag < 0x17) ||
//         (tag == 0x18) ||
//         (tag > 0x19  && tag < 0x1D) ||
//         (tag > 0x1E  && tag < 0x30) ||
//         (tag > 0x30  && tag < 0x201) ||
//         (tag > 0x205  && tag < 0x20A) ||
//         (tag > 0x210  && tag < 0x302) ||
//         (tag > 0x304 && tag < 0x381) ||
//         (tag > 0x381 && tag < 0x420) ||
//         (tag > 0x428 && tag < 0x501) ||
//         (tag > 0x501 && tag < 0x600) ||
//         (tag > 0x613 && tag < 0x1201) ||
//         tag == 0x1202 ||
//         (tag > 0x1204  && tag < 0x130C) ||
//         (tag > 0x130C  && tag < 0x1380) ||
//         tag == 0x1381 ||  tag == 0x1382 ||
//         tag > 0x1383 
//     ;

// }

typedef struct _TLV{ 
    u16 tag;
    u16 length;
    u8 data[1024];
} TLV;

typedef struct _SMPPPDU {
    u32 length;                 	
    u32 id;                     	
    u32 status;                 	
    u32 sequence;               
    u8 body[67584];             	// 2048 + 65536 
} SMPPPDU;

int isValidTLV(u8 *data,u32 data_length){
    long len;
    u8 *ptr;
    TLV tlv;

    if(data_length<=0) return(0);
    ptr=data;
    len=data_length;
    while(len>0){
        tlv.tag=*ptr++;
        tlv.tag<<=8;
        tlv.tag|=*ptr++;
        tlv.length=*ptr++;
        tlv.length<<=8;
        tlv.length|=*ptr++;
        if (isInValidTag(tlv.tag)) {
            return -1;
        }
        ptr+=tlv.length;
        len=len-(tlv.length+4);
    }
    return(1);
}

int hex_to_int(char c){
        if (c>=97) {
            c-=32;
        }
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

unsigned char hex_to_uchar(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return (unsigned char) high+low;
}
static u32 math_array[]={1,256,65536,16777216}; // {2^0,2^8,2^16,2^24}	//Phase 2	moved from smpp.h
int main (int argc, char * argv[]) {
        SMPPPDU pdu;
		int smpplength = 0;
        u8 *ptr;
        if(argc<2)
        {
           printf("usage : %s pdustring to check",argv[0]);
        }
        const char* st = argv[1];
        int length = strlen(st);
        if (!length || length %2) {
            fprintf(stderr, "invalid input\n");
            exit(1);
        }
        unsigned char *res = (unsigned char *) calloc(length/2 + 2, sizeof(char));
        int i;
        char buf = 0;
        int j = 0;
        for(i = 0; i < length; i++){
                if(i % 2 != 0){
                        res[j++] =  hex_to_uchar(buf, st[i]);
                }else{
                        buf = st[i];
                }
        }
        memset(&pdu, 0, sizeof(pdu));
		for (i = 0; i < 4; i++) {
			smpplength+=(u32)(res[i]*math_array[3-i]);	
		}
        pdu.length=smpplength;
        if(smpplength<16) {
            fprintf(stderr, "invalid pdu length %d\n", smpplength);
        } else {
            fprintf(stdout, "valid pdu length %d\n", pdu.length);
        }
        ptr = &res[4];
        for(i=0;i<4;i++){
	        pdu->id+=(*ptr)*math_array[3-i];
	        ptr++;
        }
		fprintf(stdout, "pdu id %d\n", pdu.id);
        for(i=0;i<4;i++){
	        pdu->status+=(*ptr)*math_array[3-i];
	        ptr++;
        }
        fprintf(stdout, "pdu status %d\n", pdu.status);
                for(i=0;i<4;i++){
	        pdu->sequence+=(*ptr)*math_array[3-i];
	        ptr++;
        }
        fprintf(stdout, "pdu sequence %d\n", pdu.sequence);
}

