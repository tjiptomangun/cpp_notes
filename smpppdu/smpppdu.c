#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "tcp.h"
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t
#define USSDGW_PORT (3601)
int debuglevel = 2;
char clientip[50]="";
int total_connection = 0;
#define PURGE_TIME (60)
unsigned int math_array[]={1,256,65536,16777216};
pthread_mutex_t lock_connection=PTHREAD_MUTEX_INITIALIZER;

int Stdin=0;
int Stdout=1;
char USSDGW_IP[20][50];
int TIMEOUT=60;
unsigned int request_timeout=0;
unsigned int enable_log=1;
unsigned int response_timeout=0;
u16 seqno=1;
int no_of_connection=0;
time_t starttime=0;
pthread_mutex_t lock_seqno=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_request=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_response=PTHREAD_MUTEX_INITIALIZER;



int firstrequest=1;

time_t requesttime=0;
time_t responsetime=0;

u8 *WriteString(u8 *out,char *in);
typedef struct SMPPPDUTag {
	u32 length;
	u32 id;
	u32 status;
	u32 sequence;
	u8 body[2048];
} SMPPPDU;

typedef struct SMTag{
        char service_type[7];
        u8 source_addr_ton;
        u8 source_addr_npi;
        char source_addr[22];
        u8 dest_addr_ton;
        u8 dest_addr_npi;
        char destination_addr[22];
        u8 esm_class;
        u8 protocol_id;
        u8 priority_flag;
        char schedule_delivery_time[18];
        char validity_period[18];
        u8 registered_delivery;
        u8 replace_if_present_flag;
        u8 data_coding;
        u8 sm_default_msg_id;
        u8 sm_length;
        char short_message[255];
	u8 ussd_service_op;
        char source_subaddr[22];
} SM;
int Gsock[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int sock_switch=0;


int signin_flag=0;
char Gsystem_id[20];
char Gpassword[20];
char Gsystem_type[20];
u8 Ginterface_version;
u8 Gaddr_ton;
u8 Gaddr_npi;
u32 Gid;
u32 Gsequence;
char Gaddress_range[20];

int GetSock() {
	int ret,i;
	if(Gsock[sock_switch]==-1) {
		for(i=0;i<no_of_connection-1;i++)
		{
			sock_switch++;
			if(sock_switch==no_of_connection){
				sock_switch=0;
			} 
			if(Gsock[sock_switch]==-1) {
				continue;
			}
			else
			{
				ret=Gsock[sock_switch];
			}
		}
		if(i==no_of_connection-1) exit(1);
	}
	else {
		ret=Gsock[sock_switch];
		sock_switch++;
		if(sock_switch==no_of_connection) sock_switch=0;
	}
	return(ret);
}

int getIdx(char *msisdn) {
	int len;
	int idx;
	len=strlen(msisdn);
	if(len<10) {
		return(0);
	}
	idx=((msisdn[8]-'0')*10)+(msisdn[9]-'0');
	return(idx);
}

u16 GetSeqNo() {
	u16 l_seqno;	
	pthread_mutex_lock(&lock_seqno);
	if(seqno>9999) seqno=1;
	else seqno++;
	l_seqno=seqno;
	pthread_mutex_unlock(&lock_seqno);
	return(l_seqno);
}

void CopySM2PDU(SMPPPDU *pdu,SM *sm) {
	u8 *ptr;

	ptr=pdu->body;
	ptr=WriteString(ptr,sm->service_type);
	*ptr++=sm->source_addr_ton;
	*ptr++=sm->source_addr_npi;
	ptr=WriteString(ptr,sm->source_addr);
	*ptr++=sm->dest_addr_ton;
	*ptr++=sm->dest_addr_npi;
	ptr=WriteString(ptr,sm->destination_addr);
	*ptr++=sm->esm_class;
	*ptr++=sm->protocol_id;
	*ptr++=sm->priority_flag;
	ptr=WriteString(ptr,sm->schedule_delivery_time);
	ptr=WriteString(ptr,sm->validity_period);
	*ptr++=sm->registered_delivery;
	*ptr++=sm->replace_if_present_flag;
	*ptr++=sm->data_coding;
	*ptr++=sm->sm_default_msg_id;
	*ptr++=sm->sm_length;
	memcpy(ptr,sm->short_message,sm->sm_length);
	ptr+=sm->sm_length;
	if(sm->ussd_service_op != 0x00) {
		*ptr++=0x05;
		*ptr++=0x01;
		*ptr++=0x00;
		*ptr++=0x01;
		*ptr++=sm->ussd_service_op;
	}
	if(sm->source_subaddr[0]!=0x00) { //IMSI
		*ptr++=0x02;
		*ptr++=0x03;
		*ptr++=0x00;
		*ptr++=(u8)(strlen(sm->source_subaddr));
		ptr=WriteString(ptr,sm->source_subaddr);
		ptr--;
	}
	pdu->length=ptr-(pdu->body)+16;
}

int WriteSMPPPDU(SMPPPDU *pdu) {
	u8 buff[2048];
	u8 raw[2048];
	int i,j;
	int divider;

	memset(buff,0,2048);
	for(i=0;i<4;i++) {
		divider=math_array[3-i];
		buff[i]=pdu->length/divider;
		buff[4+i]=pdu->id/divider;
		buff[8+i]=pdu->status/divider;
		buff[12+i]=pdu->sequence/divider;
	}
	memcpy(&buff[16],pdu->body,pdu->length-16);
	if(debuglevel==2)
    {
        sprintf((char *)raw,"%02x",buff[0]);
        for(j=1;j<pdu->length-1;j++) {
			sprintf((char *)raw,"%s %02x",raw,buff[j]);
		}
             
    }
	if(send(1,buff,pdu->length,0) <=0)
	{
		logger("SMPPD->APP SEND FAIL MSG[%s]",raw);
		return -1;
	} else {
		logger("SMPPD->APP RAW MSG[%s]",raw);
		return(1); 
	}
        //WriteLog(log,2);
}

int ReadSMPPPDU(SMPPPDU *pdu) {
	u32 len=0;
	u8 temp;
	u8 *ptr;
	u8 buff[2048];
	int ret;
	int i,j;
	u8 temp_len[4]="";
	 u8 raw[1024]="";

	memset(pdu,0,sizeof(SMPPPDU));
	for(i=0;i<4;i++) {
		ret=Recv(Stdin, (char *) &temp,1,0,TIMEOUT);
		if(ret!=1) return(ret);
		len+=(u32)(temp*math_array[3-i]);
	}
	pdu->length=len;
	if(len<16) return(-1);
	len-=4;
	ptr=buff;
	while(len>0) {
		ret=Recv(Stdin, (char *) ptr,len,0,TIMEOUT);
		if(ret<=0) return(-1);
		len-=ret;
		ptr+=ret;
	}
	if( debuglevel==2)
        {
                sprintf((char *) raw,"%02x %02x %02x %02x",temp_len[0],temp_len[1],temp_len[2],temp_len[3]);
                for(j=0;j<pdu->length-4;j++)
                        sprintf((char *) raw,"%s %02x",raw,buff[j]);
                logger("APP->SMPPD RAW MSG[%s]",raw);
        }

	ptr=buff;
	for(i=0;i<4;i++) {
		pdu->id+=(*ptr)*math_array[3-i];
		ptr++;
	}
	for(i=0;i<4;i++) {
		pdu->status+=(*ptr)*math_array[3-i];
		ptr++;
	}
	for(i=0;i<4;i++) {
		pdu->sequence+=(*ptr)*math_array[3-i];
		ptr++;
	}
	memcpy(pdu->body,ptr,pdu->length-16);
	return(1);
}

char *ReadString(char *out,char *in) {
	while(*in != 0x00) {
		*out++=*in++;
	}
	*out=0x00;
	in++;
	return(in);
}
u8 *WriteString(u8 *out,char *in) {
	while(*in!=0x00) {
		*out++=*in++;
	}
	*out++=0x00;
	return(out);
}


void WriteSMPPACK(unsigned int id,unsigned int status,unsigned int sequence) {
        SMPPPDU pdu;
        pdu.id=id;
        pdu.status=status;
        pdu.sequence=sequence;
        pdu.length=16;
        WriteSMPPPDU(&pdu);
}
typedef struct LISTTag {
	time_t insert_time;
	char msisdn[30];
	int ussd_service_op;
	u32 inst_id;
	u32 dlg_id;
	u32 smpp_id;
	int sock;
	int idx;
	struct LISTTag *before;
	struct LISTTag *next;
} LIST;
#define MAXSEGMENT (100)
LIST *start_list[MAXSEGMENT]={NULL};
LIST *end_list[MAXSEGMENT]={NULL};
pthread_mutex_t list_lock[MAXSEGMENT]=PTHREAD_MUTEX_INITIALIZER;

LIST *insert_list(int idx) {
        LIST *ptr;
	if(idx>MAXSEGMENT) return(NULL);
        ptr=(LIST *)malloc(sizeof(LIST));
        memset(ptr,0x00,sizeof(LIST));
	pthread_mutex_lock(&list_lock[idx]);
        if(ptr != NULL) {
                time(&(ptr->insert_time));
                if(start_list[idx] == NULL) {
                        start_list[idx]=ptr;
                        ptr->before=NULL;
                }
                if(end_list[idx] != NULL) {
                        ptr->before=end_list[idx];
                        end_list[idx]->next=ptr;
                }
                end_list[idx]=ptr;
                ptr->next=NULL;
        }
	pthread_mutex_unlock(&list_lock[idx]);
        return(ptr);
}

void delete_list(LIST *ptr) {
	int idx;
	if(ptr==NULL) return;
	idx=ptr->idx;
	if(idx>MAXSEGMENT) return;
       pthread_mutex_lock(&list_lock[idx]);
       if(ptr!=NULL) {
                if(ptr==start_list[idx]) {
                        start_list[idx]=ptr->next;
                } else {
                        if(ptr->before != NULL) (ptr->before)->next=ptr->next;
                }
                if(ptr==end_list[idx]) {
                        end_list[idx]=ptr->before;
                }  else {
                        if(ptr->next != NULL) (ptr->next)->before=ptr->before;
                }
                free(ptr);
        }
       pthread_mutex_unlock(&list_lock[idx]);
}

LIST *search_list_smppid(char *msisdn,u32 smpp_id) {
	int idx;
	
	LIST *ptr;
	idx=getIdx(msisdn);
	ptr=start_list[idx];
        pthread_mutex_lock(&list_lock[idx]);
	while(ptr!=NULL) {
		if(!strcmp(msisdn,ptr->msisdn) && smpp_id==ptr->smpp_id) break;
		ptr=ptr->next;
	}
        pthread_mutex_unlock(&list_lock[idx]);
	return(ptr);
}

LIST *search_list_tcapid(char *msisdn,u32 inst_id, u32 dlg_id) {
	int idx;
	LIST *ptr;
	idx=getIdx(msisdn);
	ptr=start_list[idx];
        pthread_mutex_lock(&list_lock[idx]);
	while(ptr!=NULL) {
		if(!strcmp(msisdn,ptr->msisdn) && inst_id==ptr->inst_id && dlg_id==ptr->dlg_id) break;
		ptr=ptr->next;
	}
        pthread_mutex_unlock(&list_lock[idx]);
	return(ptr);
}

LIST *search_list_service(char *msisdn,int ussd_service_op) {
	int idx;
	LIST *ptr;
	idx=getIdx(msisdn);
	ptr=start_list[idx];
       pthread_mutex_lock(&list_lock[idx]);
	while(ptr!=NULL) {
		if(!strcmp(msisdn,ptr->msisdn) && ussd_service_op==ptr->ussd_service_op) break;
		ptr=ptr->next;
	}
       pthread_mutex_unlock(&list_lock[idx]);
	return(ptr);
}

int ProcessSMPPPDU(SMPPPDU *pdu) {
	SMPPPDU pduout;
	char service_type[7];
	u8 source_addr_ton;
	u8 source_addr_npi;
	char source_addr[22];
	u8 dest_addr_ton;
	u8 dest_addr_npi;
	char destination_addr[22];
	u8 esm_class;
	u8 protocol_id;
	u8 priority_flag;
	char schedule_delivery_time[18];
	char validity_period[18];
	u8 registered_delivery;
	u8 replace_if_present_flag;
	u8 data_coding;
	u8 sm_default_msg_id;
	u8 sm_length;
	char short_message[255];
	u8 ussd_service_op=0;
	LIST *lptr = NULL;
	u8 status;
	int sock;

	u8 *ptr;
	char *start;
	char buff[2000]="";
	char log[1024]="";
	int msglen,len;
	int idx;

	ptr= pdu->body;
	
	if(pdu->id==0x00000004) {
		ptr=(u8 *)ReadString(service_type,(char *)ptr);
		source_addr_ton=*ptr++;
		source_addr_npi=*ptr++;
		ptr=(u8 *)ReadString(source_addr,(char *)ptr);
		dest_addr_ton=*ptr++;
		dest_addr_npi=*ptr++;
		ptr=(u8 *)ReadString(destination_addr,(char *)ptr);
		esm_class=*ptr++;
		protocol_id=*ptr++;
		priority_flag=*ptr++;
		ptr=(u8 *)ReadString(schedule_delivery_time,(char *)ptr);
		ptr=(u8 *)ReadString(validity_period,(char *)ptr);
		registered_delivery=*ptr++;
		replace_if_present_flag=*ptr++;
		data_coding=*ptr++;
		sm_default_msg_id=*ptr++;
		sm_length=*ptr++;
		memcpy(short_message,ptr,sm_length);
		ptr+=sm_length;
		if(*ptr==0x05) {
			if(*(ptr+1)==0x01)
			ussd_service_op=*(ptr+4);
		} else {
			ussd_service_op=3;
		}

		short_message[sm_length]=0x00;
		if(data_coding==0x00||data_coding==0x0f)
				logger("APP->SMPPD : ID=0x%08x STATUS=0x%02x SEQUENCE=%d DEST=%s CODING=%d OPCODE=%d STING_LEN=%d STRING=%s",pdu->id,
						pdu->status,pdu->sequence,destination_addr,data_coding,ussd_service_op,sm_length,short_message);
		else
				logger("APP->SMPPD : ID=0x%08x STATUS=0x%02x SEQUENCE=%d DEST=%s CODING=%d OPCODE=%d STING_LEN=%d STRING=ASCII",pdu->id,
						pdu->status,pdu->sequence,destination_addr,data_coding,ussd_service_op,sm_length);
		
		if(data_coding==0x00) data_coding=15;
		if(ussd_service_op==3) {
			lptr=search_list_service(destination_addr,18);
			if(lptr!=NULL) {
				time(&(lptr->insert_time));
				lptr->ussd_service_op=ussd_service_op;
				lptr->smpp_id=pdu->sequence;
	 			sprintf(buff,"SRNF %u %u %u %d %u %d",(unsigned int)lptr->smpp_id,(unsigned int)lptr->inst_id,(unsigned int)lptr->dlg_id,data_coding,pdu->status,sm_length);
				sock=lptr->sock;
			} else {
	 			sprintf(buff,"SNFY %u %s %d %u %d",(unsigned int)pdu->sequence,destination_addr,data_coding,pdu->status,sm_length);
				sock=GetSock();
			}
		} else if(ussd_service_op==2) {
			lptr=search_list_service(destination_addr,18);
			if(lptr!=NULL) {
				time(&(lptr->insert_time));
				lptr->ussd_service_op=ussd_service_op;
				lptr->smpp_id=pdu->sequence;
	 			sprintf(buff,"SRQC %u %u %u %d %u %d",(unsigned int)lptr->smpp_id,(unsigned int)lptr->inst_id,(unsigned int)lptr->dlg_id,data_coding,pdu->status,sm_length);
				sock=lptr->sock;
			} else {
				idx=getIdx(destination_addr);
				lptr=insert_list(idx);
				if(lptr!=NULL) {
					sprintf(lptr->msisdn,"%s",destination_addr);
					lptr->smpp_id=pdu->sequence;
					lptr->ussd_service_op=ussd_service_op;
	 				sprintf(buff,"SRQT %u %s %d %u %d",(unsigned int)pdu->sequence,destination_addr,data_coding,pdu->status,sm_length);
					sock=GetSock();
					lptr->sock=sock;
					lptr->idx=idx;
				}
			}
		} else if(ussd_service_op==17) {
			pthread_mutex_lock(&lock_response);
			time(&responsetime);
			pthread_mutex_unlock(&lock_response);
			status=0x50;
			lptr=search_list_service(destination_addr,1);
			if(lptr==NULL) lptr=search_list_service(destination_addr,0);
			if(lptr!=NULL) {
				time(&(lptr->insert_time));
				sock=lptr->sock;
	 			sprintf(buff,"SPRP %u %u %d %u %d",(unsigned int)lptr->inst_id,lptr->dlg_id,data_coding,pdu->status,sm_length);
				delete_list(lptr);
				status=0;
			}
			pduout.id=pdu->id|=0x80000000;
			pduout.sequence=pdu->sequence;
			pduout.status=status;
			ptr=pduout.body;
			if(status==0) {
				ptr=WriteString(ptr,"000000");
				logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=000000",pduout.id,pduout.status,pduout.sequence);
			} else {
				ptr=WriteString(ptr,"FFFFFF");
				logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=FFFFFF",pduout.id,pduout.status,pduout.sequence);
			}
			pduout.length=16+((u8 *)ptr-pduout.body);
			WriteSMPPPDU(&pduout);
			//WriteLog(log,1);
		} else if(ussd_service_op==16) {
			pthread_mutex_lock(&lock_response);
			time(&responsetime);
			pthread_mutex_unlock(&lock_response);
			status=0x50;
			lptr=search_list_service(destination_addr,1);
			if(lptr==NULL) lptr=search_list_service(destination_addr,0);
			if(lptr!=NULL) {
				time(&(lptr->insert_time));
	 			sprintf(buff,"SPRC %u %u %u %d %u %d",(unsigned int)pdu->sequence,(unsigned int)lptr->inst_id,lptr->dlg_id,data_coding,pdu->status,sm_length);
				status=0;
				lptr->smpp_id=pdu->sequence;
				sock=lptr->sock;
			}
			pduout.id=pdu->id|=0x80000000;
			pduout.sequence=pdu->sequence;
			pduout.status=status;
			ptr=pduout.body;
			if(status==0) {
				ptr=WriteString(ptr,"000000");
				 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=000000",pduout.id,pduout.status,pduout.sequence);	
			} else {
				ptr=WriteString(ptr,"FFFFFF");
				 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=FFFFFF",pduout.id,pduout.status,pduout.sequence);
			}
			pduout.length=16+((u8 *)ptr-pduout.body);
			//WriteLog(log,1);
		}else if (ussd_service_op==4){
			pthread_mutex_lock(&lock_response);
			time(&responsetime);
			pthread_mutex_unlock(&lock_response);
			status=0x50;
                        lptr=search_list_service(destination_addr,0);
                        if(lptr==NULL)
                        	lptr=search_list_service(destination_addr,1);
			if(lptr==NULL)
				lptr=search_list_service(destination_addr,18);
                        if(lptr!=NULL){
                                time(&(lptr->insert_time));
				sock=lptr->sock;
                                sprintf(buff,"SPRF %u %u %d %u %d",(unsigned int)lptr->inst_id,lptr->dlg_id,data_coding,pdu->status,sm_length);
                                status =0;
                        }
                        pduout.id=pdu->id|=0x80000000;
                        pduout.sequence=pdu->sequence;
                        pduout.status=status;
                        ptr=pduout.body;
                        if(status==0){
                                ptr=WriteString(ptr,"000000");
				 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=000000",pduout.id,pduout.status,pduout.sequence);
                        }else{
                                ptr=WriteString(ptr,"FFFFFF");
				 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=FFFFFF",pduout.id,pduout.status,pduout.sequence);
                        }
                        pduout.length=16+((u8 *)ptr-pduout.body);
                        WriteSMPPPDU(&pduout);
			//WriteLog(log,1);
			logger("%s kok buff=%s",log,buff);
			
		}else if (ussd_service_op==5){
			pthread_mutex_lock(&lock_response);
			time(&responsetime);
			pthread_mutex_unlock(&lock_response);
			status =0x50;
                        lptr=search_list_service(destination_addr,1);
                        if(lptr==NULL)lptr=search_list_service(destination_addr,0);
                        if(lptr==NULL)lptr=search_list_service(destination_addr,2);
                        if(lptr==NULL)lptr=search_list_service(destination_addr,18);
                        if(lptr!=NULL){
                                time(&(lptr->insert_time));
				sock=lptr->sock;
                           	sprintf(buff,"SPRA %u %u %d %u %d",(unsigned int)lptr->inst_id,lptr->dlg_id,data_coding,pdu->status,sm_length);
				delete_list(lptr);
                                status=0;
                        }
                        pduout.id=pdu->id|=0x80000000;
                        pduout.sequence=pdu->sequence;
                        pduout.status=status;
                        ptr=pduout.body;
                        if(status==0){
                                ptr=WriteString(ptr,"000000");
				 				logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=000000",pduout.id,pduout.status,pduout.sequence);
                        }else{
                                ptr=WriteString(ptr,"FFFFFF");
				 				logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=FFFFFF",pduout.id,pduout.status,pduout.sequence);
                        }
                        pduout.length=16+((u8 *)ptr-pduout.body);
                        WriteSMPPPDU(&pduout);
			//WriteLog(log,1);
		}
		len=strlen(buff);
		if(len>0) {
			buff[len]=0x00;
			len+=1;
			memcpy(&buff[len],short_message,sm_length);
			msglen=len+sm_length;
			TCPSendLV(sock, (u8*) buff,msglen,0);
			if(data_coding==0x00||data_coding==0x0f){
                logger("SMPPD->USSDGW SOCK=%d: %s %s",sock,buff,short_message);
			} else{
                logger("SMPPD->USSDGW SOCK=%d: %s NOT ASCII",sock,buff,short_message);
			}
		}
	} else if((pdu->id&0x80000000) == 0) {
		logger("APP->SMPPD : ID=0x%08x STATUS=0x%02x SEQUENCE=%d ",pdu->id,pdu->status,pdu->sequence);
                //WriteLog(log,1);
		pduout.id=pdu->id|=0x80000000;
		pduout.sequence=pdu->sequence;
		pduout.status=0;
		pduout.length=16;
		WriteSMPPPDU(&pduout);
		logger("APP->SMPPD : ID=0x%08x STATUS=0x%02x SEQUENCE=%d ",pduout.id,pduout.status,pduout.sequence);
                //WriteLog(log,1);
	}
	else
        {
                logger("APP->SMPPD : ID=0x%08x STATUS=0x%02x SEQUENCE=%d ERROR=PROTOCAL ERROR",pdu->id,pdu->status,pdu->sequence);
                //WriteLog(log,1);
        }

	return(1);
}


void handle_processrequest_request(int sock,char *msg,int ussd_service_op) {
	SMPPPDU pdu;
	char msisdn[30];
	char vlraddress[30];
	char imsi[30]="";
	u32 dlg_id;
	u32 inst_id;
	int ussd_len;
	int ussd_version;
	int ussd_coding;
	int status;
	char ussdstring[200];
	u8 *start;
	u8 *ptr;
	int msglen;
	u16 sequence_no;
	LIST *lptr;
	LIST *dptr;
	time_t now;
	char log[1024]="";
	int idx;

	SM sm;

	memset(&sm,0,sizeof(SM));

	sequence_no=GetSeqNo();

	if(ussd_service_op==1) {
		sscanf(msg,"%*s %u %u %s %s %d %d %d %s",&inst_id,&dlg_id,msisdn,vlraddress,&ussd_version,&ussd_coding,&ussd_len,imsi);
		sm.dest_addr_ton=1;
		sm.dest_addr_npi=1;
		sprintf(sm.destination_addr,"%s",vlraddress);
		sprintf(sm.service_type,"P%d",ussd_version);

		time(&now);
		idx=getIdx(msisdn);
		lptr=start_list[idx];
        	pthread_mutex_lock(&list_lock[idx]);
		while(lptr!=NULL) {
			dptr=NULL;
			if(!strcmp(msisdn,lptr->msisdn) || now-lptr->insert_time > PURGE_TIME) {
				dptr=lptr;
			}
			lptr=lptr->next;
			if(dptr!=NULL) {
        			pthread_mutex_unlock(&list_lock[idx]);
				delete_list(dptr);
				if(dptr->ussd_service_op == 2 || dptr->ussd_service_op == 3 || dptr->ussd_service_op == 16 || dptr->ussd_service_op == 17) 
					WriteSMPPACK(0x80000004,0x50,dptr->smpp_id);
        			pthread_mutex_lock(&list_lock[idx]);
			}
		}
        	pthread_mutex_unlock(&list_lock[idx]);

		lptr=insert_list(idx);
		if(lptr!=NULL) {
			sprintf(lptr->msisdn,"%s",msisdn);
			lptr->ussd_service_op=ussd_service_op;
			lptr->inst_id=inst_id;
			lptr->dlg_id=dlg_id;
			lptr->smpp_id=sequence_no;
			lptr->sock=sock;
			lptr->idx=idx;
		} else {
			return;
		}

	} else if(ussd_service_op==0) {
		sscanf(msg,"%*s %u %u %d %s %d %d",&inst_id,&dlg_id,&status,msisdn,&ussd_coding,&ussd_len);
		lptr=search_list_tcapid(msisdn,inst_id,dlg_id);
		if(lptr!=NULL) {
			time(&(lptr->insert_time));
			lptr->ussd_service_op=ussd_service_op;
			memset(&pdu,0,sizeof(SMPPPDU));
			pdu.id=0x80000004;
			pdu.sequence=lptr->smpp_id;
			pdu.status=status;
			ptr=pdu.body;
			if(status==0) {
				ptr=WriteString(ptr,"000000");
				pdu.length=16+(ptr-pdu.body);
				WriteSMPPPDU(&pdu);
				 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=000000",pdu.id, pdu.status,pdu.sequence);
			} else {
				ptr=WriteString(ptr,"FFFFFF");
				pdu.length=16+(ptr-pdu.body);
				delete_list(lptr);
				WriteSMPPPDU(&pdu);
				logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d BODY=FFFFFF",pdu.id, pdu.status,pdu.sequence);
				return;
			}
			lptr->smpp_id=sequence_no;
			lptr->sock=sock;
		}  else {
			return;
		}
	}
	msglen=strlen(msg);
	msglen+=1;

	sm.sm_length=ussd_len;
	memcpy(sm.short_message,&msg[msglen],sm.sm_length);
	sm.source_addr_ton=1;
	sm.source_addr_npi=1;
	sprintf(sm.source_addr,"%s",msisdn);
	sprintf(sm.source_subaddr,"%s",imsi);
	sm.data_coding=ussd_coding;
	sm.ussd_service_op=ussd_service_op;
	
	sprintf(sm.validity_period,"%d",dlg_id);
	memset(&pdu,0,sizeof(SMPPPDU));
	CopySM2PDU(&pdu,&sm);

	pdu.id=0x05;
	pdu.status=0;
	pdu.sequence=sequence_no;
	WriteSMPPPDU(&pdu);
	logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d SOURCE=%s DEST=%s CODING=%d OPCODE=%d STRING_LEN=%d STING=%s",pdu.id,
                pdu.status,pdu.sequence,msisdn,vlraddress,ussd_coding,sm.ussd_service_op,ussd_len,sm.short_message);
}

void *synchronize(void *ssock);

void handle_ussd_error(int sock,char *msg) {
	SMPPPDU pdu;
	char msisdn[30];
	char vlraddress[30];
	u32 dlg_id;
	u32 inst_id;
	u32 smpp_id;
	int ussd_len;
	int ussd_status;
	int ussd_coding;
	char ussdstring[200];
	u8 *start;
	u8 *ptr;
	int msglen;
	u16 sequence_no;
	LIST *lptr;
	LIST *dptr;
	char log[1024]="";
	int idx;

	SM sm;

	memset(&sm,0,sizeof(SM));

	sscanf(msg,"%*s %s",msisdn);
	
	idx=getIdx(msisdn);	
	lptr=start_list[idx];
       	pthread_mutex_lock(&list_lock[idx]);
	while(lptr!=NULL) {
		dptr=NULL;
		if(!strcmp(msisdn,lptr->msisdn)) {
			dptr=lptr;
		}
		lptr=lptr->next;
		if(dptr!=NULL) {
        		pthread_mutex_unlock(&list_lock[idx]);
			delete_list(dptr);
        		pthread_mutex_lock(&list_lock[idx]);
		}
	}
       	pthread_mutex_unlock(&list_lock[idx]);

	sprintf(sm.short_message,"ABORT from mobile phone");
	sm.sm_length=strlen(sm.short_message);
	sm.source_addr_ton=1;
	sm.source_addr_npi=1;
	sprintf(sm.source_addr,"%s",msisdn);
	sm.data_coding=0;
	sm.esm_class=0x08;

	memset(&pdu,0,sizeof(SMPPPDU));
	pdu.id=0x00000005;
	pdu.sequence=GetSeqNo();
	ptr=pdu.body;
	pdu.status=0;
	CopySM2PDU(&pdu,&sm);
	WriteSMPPPDU(&pdu);
	logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d SOURCE=%s CODING=%d OPCODE=%d STRING_LEN=%d STING=%s",pdu.id,
                pdu.status,pdu.sequence,msisdn,ussd_coding,sm.ussd_service_op,sm.sm_length,sm.short_message);

}


void handle_request_response(int sock,char *msg) {
	SMPPPDU pdu;
	char msisdn[30];
	char vlraddress[30];
	u32 dlg_id;
	u32 inst_id;
	u32 smpp_id;
	int ussd_len;
	int ussd_status;
	int ussd_coding;
	char ussdstring[200];
	u8 *start;
	u8 *ptr;
	int msglen;
	u16 sequence_no;
	LIST *lptr;
	char log[1024]="";
	int idx;

	SM sm;

	memset(&sm,0,sizeof(SM));

	sscanf(msg,"%*s %u %u %u %d %s %d %d",&smpp_id,&inst_id,&dlg_id,&ussd_status,msisdn,&ussd_coding,&ussd_len);
	lptr=search_list_smppid(msisdn,smpp_id);
	if(lptr!=NULL && lptr->ussd_service_op==2) {
		time(&(lptr->insert_time));
		lptr->ussd_service_op=18;
		lptr->inst_id=inst_id;
		lptr->dlg_id=dlg_id;
		lptr->sock=sock;
	} else {
		return;
	} 
	sprintf(sm.service_type,"P2");
	msglen=strlen(msg);
	msglen+=1;

	sm.sm_length=ussd_len;
	memcpy(sm.short_message,&msg[msglen],sm.sm_length);
	sm.source_addr_ton=1;
	sm.source_addr_npi=1;
	sprintf(sm.source_addr,"%s",msisdn);
	sm.data_coding=ussd_coding;
	sm.ussd_service_op=18;
	
	sprintf(sm.validity_period,"%d",dlg_id);
	memset(&pdu,0,sizeof(SMPPPDU));
	pdu.id=0x80000004;
	pdu.sequence=smpp_id;
	ptr=pdu.body;
	pdu.status=ussd_status;
	if(pdu.status!=0) {
		ptr=WriteString(ptr,"FFFFFF");
		pdu.length=16+ptr-pdu.body;
		WriteSMPPPDU(&pdu);
		delete_list(lptr);
		 logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d STING=FFFFFF",pdu.id, pdu.status,pdu.sequence);
                //WriteLog(log,1);
	} else {
		ptr=WriteString(ptr,"000000");
		pdu.length=16+ptr-pdu.body;
		WriteSMPPPDU(&pdu);
		logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d STING=000000",pdu.id, pdu.status,pdu.sequence);
                //WriteLog(log,1);
		memset(&pdu,0,sizeof(SMPPPDU));
		CopySM2PDU(&pdu,&sm);

		pdu.id=0x05;
		pdu.status=0;
		sequence_no=GetSeqNo();
		lptr->smpp_id=sequence_no;
		pdu.sequence=sequence_no;
		WriteSMPPPDU(&pdu);
		logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d SOURCE=%s CODING=%d OPCODE=%d STRING_LEN=%d STING=%s",pdu.id,
                        pdu.status,pdu.sequence,msisdn,ussd_coding,sm.ussd_service_op,ussd_len,sm.short_message);
                //WriteLog(log,1);

	}
}

void handle_ussd_info(int sock,char *msg) {
	SMPPPDU pdu;
	char msisdn[30];
	char vlraddress[30];
	u32 dlg_id;
	u32 inst_id;
	u32 smpp_id;
	int ussd_len;
	int ussd_status;
	int ussd_coding;
	char ussdstring[200];
	u8 *start;
	u8 *ptr;
	int msglen;
	u16 sequence_no;
	LIST *lptr;
	char log[1024]="";
	int idx;

	SM sm;

	memset(&sm,0,sizeof(SM));

	sscanf(msg,"%*s %u %u %u %d %s",&smpp_id,&inst_id,&dlg_id,&ussd_status,msisdn);
	lptr=search_list_smppid(msisdn,smpp_id);
	if(lptr!=NULL && (lptr->ussd_service_op==2 || lptr->ussd_service_op==3)) {
		time(&(lptr->insert_time));
		lptr->inst_id=inst_id;
		lptr->dlg_id=dlg_id;
		lptr->sock=sock;
	} else {
		return;
	} 
}
char* get_viewer_ipaddress()
{
        //struct sockaddr s2[2];
        struct sockaddr s2;
        //struct sockaddr_in *s1;
        unsigned char *array;
        socklen_t namelen;
        char *client_ip = NULL;
        int len, ret_val,i;
        char str[128];
        char test[20];


        if(client_ip == NULL)
        {

                //s2.sin_family = AF_INET6;
                //s1=malloc(sizeof(struct sockaddr_in)*2);
                namelen=sizeof(s2);
                //printf("namelen=%d\n",namelen);
                ret_val = getpeername(1,(struct sockaddr*)&s2,&namelen);
                //printf("ret_val=%d\n",ret_val);
                if(ret_val==1){

                        //printf("Fail to get peer name\n");
                }
                else
                {
                        //array = (unsigned char*)malloc((namelen)*2);


                        //memcpy(array,&s2,namelen);
                        //for(i=0;i<namelen;i++)
                        //printf("%02x ",array[i]);
                        //printf("\n");
                        //printf("IP is |%s|port=%d namelen=%d\n\r",inet_ntoa(((struct sockaddr_in *)&s2[1])->sin_addr),
					//ntohs(((struct sockaddr_in *)&s2)->sin_port),namelen);
                        client_ip=(char *)inet_ntoa(((struct sockaddr_in *)&s2)->sin_addr);
                }

        }
        return client_ip;
}
void *synchronize(void *ssock) {
	int sock;
	int rc;
	char log[1000] = "";
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	sock=*(int *)ssock;
	while(1) {
		sleep(60);
		logger("SMPPDP->USSDGW : SYNC");
                //WriteLog(log,1);
		rc=TCPSendLV(sock,(u8* ) "SYNC",4,0);
		if(rc<=0) break;
	}
	shutdown(sock,SHUT_RDWR);
	pthread_exit(0);
}

void *ProcessTCP(void *ussdgw_ip) {
	char msg[300];
	SMPPPDU pduout;
	int ret;
	u32 status;
	u32 seqno;
	u8 *start;
	u8 *ptr;
	int sock;
	char outmsg[128];
	int sockidx=-1;	
	char ip_temp[50];
	char log[1024]="";
	void *res;
	
	pthread_t tcp_thread; //  --->  nan
	pthread_attr_t attr;  // --> nan
      
  	pthread_attr_init(&attr);// --> nan

	logger("Start Thread connect to USSDGW IP %s",(char *)ussdgw_ip);
	
	while(1) {
		logger("connecting to %s port %d",ussdgw_ip,USSDGW_PORT);
		sock=ConnectWithTimeout(ussdgw_ip,USSDGW_PORT,10);
		if(sock<0) {
			close(sock);
			sleep(5);
			continue;

		}
		logger("connected to %s with sock=%d",ussdgw_ip,sock);

		
		logger("Connect USSDGW IP %s Port=%d SOCKET=%d SUCCESS",(char *)ussdgw_ip,USSDGW_PORT,sock);
		//WriteLog(log,1);
		sprintf(outmsg,"SSGN %s %s %s",Gsystem_id,Gpassword,clientip);
		logger("SMPPDP->USSDGW : %s",outmsg);
		//WriteLog(log,1);
		TCPSendLV(sock,(u8*) outmsg,strlen(outmsg),0);
		ret=TCPRecvLV(sock,(u8*) outmsg,128,0);
		if(ret<=0) {
			close(sock);
			logger("Receive data from USSDGW %s FAIL return = %d",(char *)ussdgw_ip,ret);
			//WriteLog(log,1);
			sleep(10);
			continue;
		} 
		logger("USSDGW->SMPPDP : %s",outmsg);
		//WriteLog(log,1);
		if(strncmp(outmsg,"SSSC",4)) {
			close(sock);
			sleep(10);
			continue;
		}

		logger("sign in to USSDGW IP:%s sock=%d",ussdgw_ip,sock);

		if(signin_flag==0) {
			signin_flag=1;
			pduout.id=Gid|0x80000000;
			pduout.sequence=Gsequence;
			pduout.status=0;
			start=pduout.body;
			ptr=WriteString(pduout.body,Gsystem_id);
			pduout.length=16+(ptr-start);
			WriteSMPPPDU(&pduout);
		}

		if(sockidx==-1) {
			sockidx=no_of_connection++;
		}
		
		Gsock[sockidx]=sock;

		pthread_create(&tcp_thread,&attr,synchronize,(void *)&sock);

		pthread_mutex_lock(&lock_connection);
		total_connection++;
		pthread_mutex_unlock(&lock_connection);

		while(1) {
			ret=TCPRecvLV(sock, (u8* ) msg,300,0);
			if(ret<=0) {
				close(sock);
				Gsock[sockidx]=-1;
				logger("close sock=%d errorcode=%d",sock,ret);
				pthread_cancel(tcp_thread);
				pthread_join(tcp_thread,&res);
				if(res==PTHREAD_CANCELED) {
					logger("close SYNC thread on sock=%d",sock);
				} else {
					logger("problem on close SYNC thread on sock=%d",sock);
				}
				pthread_mutex_lock(&lock_connection);
				total_connection--;
				pthread_mutex_unlock(&lock_connection);
				break;
			}
			msg[ret]=0x00;
			logger("USSDGW->SMPPD SOCK=%d :%s",sock,msg);
                	//WriteLog(log,1);            
			if(!strncmp(msg,"SNRP",4)) {
				sscanf(msg,"%*s %u %u",(unsigned int *)&seqno,(unsigned int *)&status);
				pduout.id=0x80000004;
				pduout.sequence=seqno;
				pduout.status=status;
				start=pduout.body;
				if(status==0) {
					ptr=WriteString(pduout.body,"00000000");
					pduout.length=16+(ptr-start);
				    logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d STING=00000000",pduout.id, pduout.status,pduout.sequence);
				} else {
					ptr=WriteString(pduout.body,"FFFFFFFF");
					pduout.length=16+(ptr-start);
				    logger("SMPPD->APP : ID=0x%08x STATUS=0x%02x SEQUENCE=%d STING=FFFFFFFF",pduout.id, pduout.status,pduout.sequence);
				}
				WriteSMPPPDU(&pduout);
				//WriteLog(log,1);
			} else if(!strncmp(msg,"SPRT",4)) {
				pthread_mutex_lock(&lock_request);
				time(&requesttime);
				if(firstrequest==1) {
					firstrequest=0;
					pthread_mutex_lock(&lock_response);
					time(&responsetime);
					pthread_mutex_unlock(&lock_response);
				}
				pthread_mutex_unlock(&lock_request);
				handle_processrequest_request(sock,msg,1);
			} else if(!strncmp(msg,"SPRC",4)) {
				handle_processrequest_request(sock,msg,0);
			} else if(!strncmp(msg,"SRRP",4)) {
				handle_request_response(sock,msg);
			} else if(!strncmp(msg,"SERR",4)) {
				handle_ussd_error(sock,msg);
			 }else if (!strncmp(msg,"CKIP",4)) {
                        	sscanf(msg,"%*s %s",ip_temp);
                        	if(!strncmp(clientip,ip_temp,strlen(clientip)))
                        	{
                                	close(sock);
					Gsock[sockidx]=-1;
					break;
                        	}
			} else if(!strncmp(msg,"SINF",4)) {
				handle_ussd_info(sock,msg);
			} else if(!strncmp(msg,"SYNC",4)) {
			} else {
			}
		}
	}
	pthread_exit(0);
}

void *monitor(void *dummy) {
	time_t now;
	while(1) {
		sleep(1);
		time(&now);
		if(now-starttime> 10) {
			if(total_connection<=0) {
				logger("%s","SMPPD terminated according to no connection to any USSDGW");
				exit(0);
			}
		}
		if(request_timeout>0) {
			if(now-requesttime>request_timeout) {
				logger("%s","SMPPD terminated according to USSD request timeout");
				exit(0);
			}
		}
		if(response_timeout>0) {
			if(requesttime>responsetime && requesttime-responsetime>response_timeout) {
				logger("%s","SMPPD terminated according to USSD response timeout");
				exit(0);
			}
		}
	}
	pthread_exit(0);
}


int main(int argc, char **argv) {
		int ret;
	SMPPPDU pdu;
	int i,j;
	char *ptr;
	char log[1024]="";
	u16 closefile=60;
	int nserver=0;
	char logpath[300]="";
	char configfile[300]="";
	int rc;

	pthread_t tcp_process_thread; //  --->  nan
	pthread_t monitor_thread; //  --->  nan
	pthread_attr_t attr;  // --> nan
      
  	pthread_attr_init(&attr);// --> nan
  	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //--> nan


	sprintf(USSDGW_IP[0],"127.0.0.1");
	nserver=1;
	logger("App start");

	if(ReadSMPPPDU(&pdu)<0) {
		logger("exit because unable to read");
		exit(1);
	} 
	Gid=pdu.id;
	Gsequence=pdu.sequence;
	ptr=pdu.body;

	ptr=ReadString(Gsystem_id,ptr);
	ptr=ReadString(Gpassword,ptr);
	ptr=ReadString(Gsystem_type,ptr);
	Ginterface_version=*ptr++;
	Gaddr_ton=*ptr++;
	Gaddr_npi=*ptr++;
	ptr=ReadString(Gaddress_range,ptr);

	sprintf(clientip,"%s",get_viewer_ipaddress());

	if(request_timeout==0) response_timeout=0;
	else if(response_timeout>0 && response_timeout<=request_timeout) response_timeout=request_timeout+5; 

	time(&requesttime);
	time(&responsetime);

	logger("APP->SMPP :ID=0x%08x Sequence=%d USERNAME=%s PASSWORD=%s",pdu.id,pdu.sequence,Gsystem_id,Gpassword);
        //WriteLog(log,1);
	if(pdu.id!=0x09 && pdu.id!=0x02) exit(1);
	
	for(j=0;j<nserver;j++)
	{
		pthread_create(&tcp_process_thread,&attr,ProcessTCP,(void *)USSDGW_IP[j]);
	}
	time(&starttime);
	pthread_create(&tcp_process_thread,&attr,monitor,(void *)NULL);
	while(1) {
		ret=ReadSMPPPDU(&pdu);
		if(ret>0) ProcessSMPPPDU(&pdu);
		else if(ret!=ERR_TCP_TIMEOUT) break;
	}
	exit(0);
}
