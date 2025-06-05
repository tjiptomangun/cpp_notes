#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

typedef struct _SUBMIT_SM {
    char service_type[7];               // Var. max 6 size octets
    u8 source_addr_ton;
    u8 source_addr_npi;
    char source_addr[22];               // Var. max 21 size octets
    u8 dest_addr_ton;
    u8 dest_addr_npi;
    char dest_addr[22];          // Var. max 21 size octets
    u8 esm_class;
    u8 protocol_id;
    u8 priority_flag;
    char schedule_delivery_time[18];    // Var. max 17 size octets
    char validity_period[18];           // Var. max 17 size octets
    u8 registered_delivery;
    u8 replace_if_present_flag;
    u8 data_coding;
    u8 sm_default_msg_id;               // If not using an SMSC canned message, set to NULL
    u8 sm_length;
    u8 short_message[255];            	// Octet string - max 254 size octets
    u16 payload_length;			// message_payload TLV length
    u16 src_port;			// TLV value	//Phase 2 support MMS parameters
    u16 dst_port;			// TLV value	//Phase 2 support MMS parameters
    u16 sar_msg_ref_num;		// TLV value
    u8 sar_total_segments;		// TLV value
    u8 sar_segment_seqnum;		// TLV value
    u8 concat_num;			// Proprietary: number of receiving concat message segments - use to tell core module how many concat recved
    char concat_msg_length[1536];	// Proprietary: list of msg length and udhi (ex. 0:140,0:140,1:82)
    u8 udhi;				// Proprietary: 1 if data contain additional UDH
    u8 *message_payload;		// TLV value - Octet string upto 64K octets
} SUBMIT_SM;
typedef enum {
    SMPP_GENERIC_NACK				= 0x80000000,
    SMPP_BIND_RECEIVER				= 0x00000001,
    SMPP_BIND_RECEIVER_RESP			= 0x80000001,
    SMPP_BIND_TRANSMITTER			= 0x00000002,
    SMPP_BIND_TRANSMITTER_RESP			= 0x80000002,
    SMPP_QUERY_SM				= 0x00000003,
    SMPP_QUERY_SM_RESP				= 0x80000003,
    SMPP_SUBMIT_SM				= 0x00000004,
    SMPP_SUBMIT_SM_RESP				= 0x80000004,
    SMPP_DELIVER_SM				= 0x00000005,
    SMPP_DELIVER_SM_RESP			= 0x80000005,
    SMPP_UNBIND					= 0x00000006,
    SMPP_UNBIND_RESP				= 0x80000006,
    SMPP_REPLACE_SM				= 0x00000007,
    SMPP_REPLACE_SM_RESP			= 0x80000007,
    SMPP_CANCEL_SM				= 0x00000008,
    SMPP_CANCEL_SM_RESP				= 0x80000008,
    SMPP_BIND_TRANSCEIVER			= 0x00000009,
    SMPP_BIND_TRANSCEIVER_RESP			= 0x80000009,
    SMPP_OUTBIND				= 0x0000000B,
    SMPP_ENQUIRE_LINK				= 0x00000015,
    SMPP_ENQUIRE_LINK_RESP			= 0x80000015,
    SMPP_SUBMIT_MULTI				= 0x00000021,
    SMPP_SUBMIT_MULTI_RESP			= 0x80000021,
    SMPP_ALERT_NOTIFICATION			= 0x00000102,
    SMPP_DATA_SM				= 0x00000103,
    SMPP_DATA_SM_RESP				= 0x80000103,
    SMPP_BROADCAST_SM				= 0x00000111,	// SMPP v5.0
    SMPP_BROADCAST_SM_RESP			= 0x80000111,	// SMPP v5.0
    SMPP_QUERY_BROADCAST_SM			= 0x00000112,	// SMPP v5.0
    SMPP_QUERY_BROADCAST_SM_RESP		= 0x80000112,	// SMPP v5.0
    SMPP_CANCEL_BROADCAST_SM			= 0x00000113,	// SMPP v5.0
    SMPP_CANCEL_BROADCAST_SM_RESP		= 0x80000113,	// SMPP v5.0
/* Reserved for MC Vendor:
    0x00010200 - 0x000102FF
    0x80010200 - 0x800102FF
*/
// TELOGIC Proprietary //
    SMPP_SUBMIT_MO				= 0x00000016,
    SMPP_SUBMIT_MO_RESP				= 0x80000016,
    SMPP_DELIVER_MT				= 0x00000017,
    SMPP_DELIVER_MT_RESP			= 0x80000017,
    SMPP_QUERY_IMSI				= 0x00000018,
    SMPP_QUERY_IMSI_RESP			= 0x80000018,
    SMPP_ALERT_SC				= 0x00000019,
    SMPP_ALERT_SC_RESP				= 0x80000019,
    SMPP_DELIVER_RP				= 0x00000020,
    SMPP_DELIVER_RP_RESP			= 0x80000020,
    SMPP_QUERY_APPID                            = 0x00010222,   // BOY: 20110606 to sync active appid among smsrouters
    SMPP_QUERY_APPID_RESP                       = 0x80010222,   // BOY: 20110606 to sync active appid among smsrouters
/////////////////////////
} SMPP_COMMAND_ID;

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
typedef struct _UDH{
    u8 tag;
    u8 length;
    u8 data[20];
} UDH;

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
int isInValidTLV(u8 *data,u32 data_length){
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
            fprintf(stderr, "invalid tag %04x\n", tlv.tag);
            return -1;
        } else {
            fprintf(stdout, "valid tag %04x\n", tlv.tag);
        }
        ptr+=tlv.length;
        len=len-(tlv.length+4);
    }
    return(0);
}

char* ReadString(char *out,char *in){
    while(*in!=0x00){
	*out++=*in++;
    }
    *out=0x00;
    in++;
    return(in);
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

int Unpack_SUBMIT_SM(SUBMIT_SM *sm,SMPPPDU *pdu){

    u8 *ptr;
    u8 *ptr1;
    int i,ret;
    u8 udh_len;		// UDH length
    long tlv_len;	// Phase 2 Change from u32 to long because need negative value
    UDH udh;
    UDH pudh;
    u16 new_payload_length;
    char tmp[256];
	TLV tlv;

    memset(sm,0,sizeof(SUBMIT_SM));
    memset(&udh,0,sizeof(UDH));
    memset(&pudh,0,sizeof(UDH));
    ptr=(u8*)pdu->body;

    ptr=ReadString(sm->service_type,ptr);
    printf("service type is %s\n", sm->service_type[0] == 0 ? "Default(0)": sm->service_type);
    sm->source_addr_ton=*ptr++;
    sm->source_addr_npi=*ptr++;
    ptr=ReadString(tmp,ptr);
    if(strlen(tmp)>21){
        printf("source_addr is longer than 21 characters\n");

    }else{
        sprintf(sm->source_addr,"%s",tmp);
        printf("source_addr is %s\n", sm->source_addr);
    }
    sm->dest_addr_ton=*ptr++;
    sm->dest_addr_npi=*ptr++;
    ptr=ReadString(tmp,ptr);
    if(strlen(tmp)>21){
        printf("invalid dest_addr is longer than 21 characters\n");
        exit(-1);
    }else if(strlen(tmp)==0){
        printf("invalid dest_addr is 0 length\n");
        exit(-1);
    }else{
        sprintf(sm->dest_addr,"%s",tmp);
        printf("dest_addr is %s\n", sm->dest_addr);
    }
    sm->esm_class=*ptr++;
    printf("esm_class = %02x\n", sm->esm_class);
    sm->protocol_id=*ptr++;
    printf("protocol_id = %02x\n", sm->protocol_id);
    sm->priority_flag=*ptr++;
    printf("priority_flag = %02x\n", sm->priority_flag);
    ptr=ReadString(sm->schedule_delivery_time,ptr);
    printf("schedule_delivery_time is %s\n", sm->schedule_delivery_time);
    ptr=ReadString(sm->validity_period,ptr);
    printf("validity_period is %s\n", sm->validity_period);
    sm->registered_delivery=*ptr++;
    printf("registered_delivery = %02x\n", sm->registered_delivery);
    sm->replace_if_present_flag=*ptr++;
    printf("replace_if_present_flag = %02x\n", sm->replace_if_present_flag);
    sm->data_coding=*ptr++;
    printf("data_coding = %02x\n", sm->data_coding);
    sm->sm_default_msg_id=*ptr++;
    printf("sm_default_msg_id = %02x\n", sm->sm_default_msg_id);
    sm->sm_length=*ptr++;
    printf("sm_length = %02x\n", sm->sm_length);
    ptr+=sm->sm_length;
    tlv_len=pdu->length-(ptr-(pdu->body))-16;
    if (isInValidTLV(ptr, tlv_len)) {
        exit(-1);
    }


}
static u32 math_array[]={1,256,65536,16777216}; // {2^0,2^8,2^16,2^24}	//Phase 2	moved from smpp.h
int main (int argc, char * argv[]) {
        SMPPPDU pdu;
        SUBMIT_SM sm;
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
            exit(-1);
        } else {
            fprintf(stdout, "valid pdu length %d\n", pdu.length);
        }
        ptr = &res[4];
        for(i=0;i<4;i++){
	        pdu.id+=(*ptr)*math_array[3-i];
	        ptr++;
        }
		fprintf(stdout, "pdu id %d\n", pdu.id);
        for(i=0;i<4;i++){
	        pdu.status+=(*ptr)*math_array[3-i];
	        ptr++;
        }
        fprintf(stdout, "pdu status %d\n", pdu.status);
                for(i=0;i<4;i++){
	        pdu.sequence+=(*ptr)*math_array[3-i];
	        ptr++;
        }
        fprintf(stdout, "pdu sequence %d\n", pdu.sequence);
        memcpy(pdu.body,ptr,pdu.length-16);
        if (pdu.id == SMPP_SUBMIT_SM) {
            Unpack_SUBMIT_SM(&sm, &pdu);
        }
}

