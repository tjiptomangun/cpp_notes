#ifndef _AF_H_
#define _AF_H_
#define EQX_BUFF_SIZE_8192												8*1024
#define EQX_BUFF_SIZE_10 10
#define EQX_BUFF_SIZE_64 64
#define EQX_BUFF_SIZE_128 128
#define EQX_BUFF_SIZE_256 256
#define EQX_BUFF_SIZE_10 10
#include "radixtree.h"
typedef struct {
	int version;
	char name[128];
	char service[256];
	char tidStart[EQX_BUFF_SIZE_64];
	char tidEnd[EQX_BUFF_SIZE_128];
	char defaultUSSDMapVersion[EQX_BUFF_SIZE_128];
//	char ussdSpamBlacklist[32000];
	char ussdSpamEnable[EQX_BUFF_SIZE_64];
	char defaultCgPa[EQX_BUFF_SIZE_128];
	char ussdAddress[EQX_BUFF_SIZE_128];
	char ussdInbound[EQX_BUFF_SIZE_64];
	char ivrInbound[EQX_BUFF_SIZE_64];
	char e01Timeout[EQX_BUFF_SIZE_64];
	char d02Timeout[EQX_BUFF_SIZE_64];
	char ldapRetryRet[EQX_BUFF_SIZE_64];
	char ldapSearchRetry[EQX_BUFF_SIZE_64];
	char ldapModifyRetry[EQX_BUFF_SIZE_64];
	char ussdGWTimeout[EQX_BUFF_SIZE_64];
	char ussdGWMaxRetry[EQX_BUFF_SIZE_64];
	char hlrTimeout[EQX_BUFF_SIZE_64];
	char essRelayMaxRetry[EQX_BUFF_SIZE_64];
	char mscTimeout[EQX_BUFF_SIZE_64];
	char smscTimeout[EQX_BUFF_SIZE_64];
	char smscMaxRetry[EQX_BUFF_SIZE_64];
	char fdap2pTimeout[EQX_BUFF_SIZE_64];
	char fda2pMaxRetry[EQX_BUFF_SIZE_64];
	char regionAuthority[EQX_BUFF_SIZE_64];
	char authoritySelector[EQX_BUFF_SIZE_64];
	char defaultSAIMAPVersion[EQX_BUFF_SIZE_128];
	char cloningKiller[EQX_BUFF_SIZE_64];
	char droamCdrException[EQX_BUFF_SIZE_256];
	char updateMultisim[EQX_BUFF_SIZE_64];
	char serviceEnable[EQX_BUFF_SIZE_64];
	char deleteSDEnable[EQX_BUFF_SIZE_64];
	char delayTime[EQX_BUFF_SIZE_128];
// 	char isdClassOfService[EQX_BUFF_SIZE_8192 * 2];
	char isdRequestTimeout[EQX_BUFF_SIZE_64];
	char isdRequestMaxRetry[EQX_BUFF_SIZE_64];
	char defaultSMSMAPVersion[EQX_BUFF_SIZE_128];
	char moReverseEnable[EQX_BUFF_SIZE_64];
	//char exceptionalSmscMoList[32000];
	char smscAddress[EQX_BUFF_SIZE_128];
	char moRoamingEnable[EQX_BUFF_SIZE_64];
	//char smsByPassRoaming[32000];
	char sgsnByPass[EQX_BUFF_SIZE_64];
	char smsProxyService[EQX_BUFF_SIZE_64];
	char smsVerifyRomaing[EQX_BUFF_SIZE_64];
	char defaultLOCUPMAPVersion[EQX_BUFF_SIZE_128];
	char instance[EQX_BUFF_SIZE_10];
	char defCamelServKey[EQX_BUFF_SIZE_64];
	char defCamelSCFAddr[EQX_BUFF_SIZE_64];
	PRADIXTREE_ITEM ussdSpamBlackTree;
	PRADIXTREE_ITEM smsByPassRoamTree;
	PRADIXTREE_ITEM exceptionalSmMOTree;
	PRADIXTREE_ITEM isdCOSTree;
} AF_CONFIG;

AF_CONFIG af_config;


#define SUCCESS                 0
#define NO_SUCCESS              -1

#define EC_UTILS_MAX_KEY_SIZE 256
#define EC_UTILS_MAX_DB_SIZE 4096
typedef struct _equinox_message_e01_
{
	char command[EC_UTILS_MAX_KEY_SIZE]; //1
	char resultcode[EC_UTILS_MAX_KEY_SIZE]; //2
	char description[EC_UTILS_MAX_KEY_SIZE];
	char objecttype[EC_UTILS_MAX_KEY_SIZE]; //3
	char value[EC_UTILS_MAX_DB_SIZE]; //4
	char key0[EC_UTILS_MAX_KEY_SIZE]; //5
	char key1[EC_UTILS_MAX_KEY_SIZE]; //6
	char key2[EC_UTILS_MAX_KEY_SIZE]; //7
	char key3[EC_UTILS_MAX_KEY_SIZE]; //8
	char key4[EC_UTILS_MAX_KEY_SIZE]; //9
	char *value_ptr;
} EQX_MSG_E01;
#define SFLOG_DEBUG(...) fprintf(stdout, __VA_ARGS__);fprintf(stdout, "\n");
#endif
