#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "radixtree.h"
#include "yxml.h"
#include "af.h"
#include "xmlfunc.h"
#include <unistd.h>

int copy_string_10(void *dst, char *in) {
	strncpy(dst, in, 10);
	return 1;
}

int copy_string_20(void *dst, char *in) {
	strncpy(dst, in, 20);
	return 1;
}

int copy_string_64(void *dst, char *in) {
	strncpy(dst, in, 64);
	return 1;
}
int copy_string_128(void *dst, char *in) {
	strncpy(dst, in, 128);
	return 1;
}

int copy_string_256(void *dst, char *in) {
	strncpy(dst, in, 256);
	return 1;
}

int radixtree_insert(void *dst, char *in) {
	PRADIXTREE_ITEM rdx  = (PRADIXTREE_ITEM) dst;
	radixtreeitem_insertkey(rdx, in);
	return 1;
	
}

int copy_af_tidstart(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_20(af_conf->tidStart , in);
	}
	return 1;
}

int copy_af_tidend(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_20(af_conf->tidEnd , in);
	}
	return 1;
}

int copy_af_defaultUSSDMapVersion(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->defaultUSSDMapVersion, in);
	}
	return 1;
}

int add_af_ussedSpamBlackTree(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		radixtree_insert(af_conf->ussdSpamBlackTree, in);
	}
	return 1;
}

int copy_af_ussdSpamEnable(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ussdSpamEnable, in);
	}
	return 1;
}

int copy_af_defaultCgPa(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->defaultCgPa, in);
	}
	return 1;
}

int copy_af_ussdAddress(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->ussdAddress, in);
	}
	return 1;
}

int copy_af_ussdInbound(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ussdInbound, in);
	}
	return 1;
}

int copy_af_ivrInbound(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ivrInbound, in);
	}
	return 1;
}

int copy_af_e01Timeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->e01Timeout, in);
	}
	return 1;
}

int copy_af_d02Timeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->d02Timeout, in);
	}
	return 1;
}

int copy_af_ldapRetryRet(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ldapRetryRet, in);
	}
	return 1;
}

int copy_af_ldapSearchRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ldapSearchRetry, in);
	}
	return 1;
}

int copy_af_ldapModifyRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ldapModifyRetry, in);
	}
	return 1;
}

int copy_af_ussdGWTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ussdGWTimeout, in);
	}
	return 1;
}

int copy_af_ussdGWMaxRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->ussdGWMaxRetry, in);
	}
	return 1;
}

int copy_af_hlrTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->hlrTimeout, in);
	}
	return 1;
}

int copy_af_essRelayMaxRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->essRelayMaxRetry, in);
	}
	return 1;
}

int copy_af_mscTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->mscTimeout, in);
	}
	return 1;
}

int copy_af_smscTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->smscTimeout, in);
	}
	return 1;
}

int copy_af_smscMaxRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->smscMaxRetry, in);
	}
	return 1;
}

int copy_af_fdap2pTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->fdap2pTimeout, in);
	}
	return 1;
}

int copy_af_fdap2pMaxRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->fda2pMaxRetry, in);
	}
	return 1;
}

int copy_af_regionAuthority(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->regionAuthority, in);
	}
	return 1;
}

int copy_af_authoritySelector(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->authoritySelector, in);
	}
	return 1;
}

int copy_af_defaultSAIMAPVersion(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->defaultSAIMAPVersion, in);
	}
	return 1;
}

int copy_af_cloningKiller(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->cloningKiller, in);
	}
	return 1;
}

int copy_af_updateMultisim(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->updateMultisim, in);
	}
	return 1;
}

int copy_af_serviceEnable(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->serviceEnable, in);
	}
	return 1;
}

int copy_af_deleteSDEnable(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->deleteSDEnable, in);
	}
	return 1;
}

int copy_af_delayTime(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->delayTime, in);
	}
	return 1;
}

int add_af_isdCOSTree(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		radixtree_insert(af_conf->isdCOSTree, in);
	}
	return 1;
}

int copy_af_isdRequestTimeout(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->isdRequestTimeout, in);
	}
	return 1;
}

int copy_af_isdRequestMaxRetry(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->isdRequestMaxRetry, in);
	}
	return 1;
}

int copy_af_smsProxyService(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->smsProxyService, in);
	}
	return 1;
}

int copy_af_defaultSMSMAPVersion(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->defaultSMSMAPVersion, in);
	}
	return 1;
}

int copy_af_moReverseEnable(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->moReverseEnable, in);
	}
	return 1;
}

int add_af_exceptionalSmMOTree(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		radixtree_insert(af_conf->exceptionalSmMOTree, in);
	}
	return 1;
}

int copy_af_smscAddress(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->smscAddress, in);
	}
	return 1;
}

int copy_af_moRoamingEnable(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->moRoamingEnable, in);
	}
	return 1;
}

int add_af_smsByPassRoamTree(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		radixtree_insert(af_conf->smsByPassRoamTree, in);
	}
	return 1;
}

int copy_af_sgsnByPass(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->sgsnByPass, in);
	}
	return 1;
}

int copy_af_droamCdrException(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_256(af_conf->droamCdrException, in);
	}
	return 1;
}

int copy_af_defaultLOCUPMAPVersion(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_128(af_conf->defaultLOCUPMAPVersion, in);
	}
	return 1;
}

int copy_af_defCamelServKey(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->defCamelServKey, in);
	}
	return 1;
}

int copy_af_defCamelSCFAddr(AF_CONFIG *af_conf, char *in) {
	if (af_conf) {
		copy_string_64(af_conf->defCamelSCFAddr, in);
	}
	return 1;
}
int check_enable_flag(char *param) {
	return (strcmp(param, "0") && strcmp(param, "1"));
}

XMLFUNC funcs_AF_CONFIG[] = {
	{"configuration/warm/TCAP-DialogID-Rang-Start", "value", (int (*) (void *, char *)) copy_af_tidstart, 0, 0, 
		NULL},
		
	{"configuration/warm/TCAP-DialogID-Rang-Start", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_tidstart},


	{"configuration/warm/TCAP-DialogID-Rang-End", "value", (int (*) (void *, char *)) copy_af_tidend, 0, 0, 
		NULL},
		
	{"configuration/warm/TCAP-DialogID-Rang-End", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_tidend},

	{"configuration/warm/Default-USSD-MAPVersion", "value", (int (*) (void *, char *)) copy_af_defaultUSSDMapVersion, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-USSD-MAPVersion", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defaultUSSDMapVersion},


	{"configuration/warm/ussdSpamBlacklist", "value", (int (*) (void *, char *)) add_af_ussedSpamBlackTree, 0, 0, 
		NULL},
		
	{"configuration/warm/ussdSpamBlacklist", "", NULL, 0, 0, 
		(int (*) (void *, char *)) add_af_ussedSpamBlackTree},


	{"configuration/warm/ussdSpamEnable", "value", (int (*) (void *, char *)) copy_af_ussdSpamEnable, 0, 0, 
		NULL},
		
	{"configuration/warm/ussdSpamEnable", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ussdSpamEnable},


	{"configuration/warm/Default-CgPA", "value", (int (*) (void *, char *)) copy_af_defaultCgPa, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-CgPA", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defaultCgPa},


	{"configuration/warm/ussdAddress", "value", (int (*) (void *, char *)) copy_af_ussdAddress, 0, 0, 
		NULL},
		
	{"configuration/warm/ussdAddress", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ussdAddress},


	{"configuration/warm/ussdInbound", "value", (int (*) (void *, char *)) copy_af_ussdInbound, 0, 0, 
		NULL},
		
	{"configuration/warm/ussdInbound", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ussdInbound},


	{"configuration/warm/ivrInbound", "value", (int (*) (void *, char *)) copy_af_ivrInbound, 0, 0, 
		NULL},
		
	{"configuration/warm/ivrInbound", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ivrInbound},


	{"configuration/warm/E01Timeout", "value", (int (*) (void *, char *)) copy_af_e01Timeout, 0, 0, 
		NULL},
		
	{"configuration/warm/E01Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_e01Timeout},


	{"configuration/warm/D02-Timeout", "value", (int (*) (void *, char *)) copy_af_d02Timeout, 0, 0, 
		NULL},
		
	{"configuration/warm/D02-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_d02Timeout},


	{"configuration/warm/LDAPRetryRet", "value", (int (*) (void *, char *)) copy_af_ldapRetryRet, 0, 0, 
		NULL},
		
	{"configuration/warm/LDAPRetryRet", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ldapRetryRet},

	{"configuration/warm/LDAPSearchRetry", "value", (int (*) (void *, char *)) copy_af_ldapSearchRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/LDAPSearchRetry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ldapSearchRetry},


	{"configuration/warm/LDAPModifyRetry", "value", (int (*) (void *, char *)) copy_af_ldapModifyRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/LDAPModifyRetry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ldapModifyRetry},


	{"configuration/warm/USSDGW-Timeout", "value", (int (*) (void *, char *)) copy_af_ussdGWTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/USSDGW-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ussdGWTimeout},


	{"configuration/warm/USSDGW-Max-Retry", "value", (int (*) (void *, char *)) copy_af_ussdGWMaxRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/USSDGW-Max-Retry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_ussdGWMaxRetry},


	{"configuration/warm/HLR-Timeout", "value", (int (*) (void *, char *)) copy_af_hlrTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/HLR-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_hlrTimeout},


	{"configuration/warm/ESS-Relay-Max-Retry", "value", (int (*) (void *, char *)) copy_af_essRelayMaxRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/ESS-Relay-Max-Retry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_essRelayMaxRetry},
		
	{"configuration/warm/MSC-Timeout", "value", (int (*) (void *, char *)) copy_af_mscTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/MSC-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_mscTimeout},


	{"configuration/warm/SMSC-Timeout", "value", (int (*) (void *, char *)) copy_af_smscTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/SMSC-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_smscTimeout},


	{"configuration/warm/SMSC-Max-Retry", "value", (int (*) (void *, char *)) copy_af_smscMaxRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/SMSC-Max-Retry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_smscMaxRetry},


	{"configuration/warm/FDAP2P-Timeout", "value", (int (*) (void *, char *)) copy_af_fdap2pTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/FDAP2P-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_fdap2pTimeout},


	{"configuration/warm/FDAP2P-Max-Retry", "value", (int (*) (void *, char *)) copy_af_fdap2pMaxRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/FDAP2P-Max-Retry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_fdap2pMaxRetry},


	{"configuration/warm/regionAuthority", "value", (int (*) (void *, char *)) copy_af_regionAuthority, 0, 0, 
		NULL},
		
	{"configuration/warm/regionAuthority", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_regionAuthority},


	{"configuration/warm/authoritySelector", "value", (int (*) (void *, char *)) copy_af_authoritySelector, 0, 0, 
		NULL},
		
	{"configuration/warm/authoritySelector", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_authoritySelector},

	{"configuration/warm/Default-SAI-MAPVersion", "value", (int (*) (void *, char *)) copy_af_defaultSAIMAPVersion, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-SAI-MAPVersion", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defaultSAIMAPVersion},

		
	{"configuration/warm/cloningKiller", "value", (int (*) (void *, char *)) copy_af_cloningKiller, 0, 0,
		NULL},
		
	{"configuration/warm/cloningKiller", "", NULL, 0, 0,
		(int (*) (void *, char *)) copy_af_cloningKiller},


	{"configuration/warm/updateMultisim", "value", (int (*) (void *, char *)) copy_af_updateMultisim, 0, 0, 
		NULL},
		
	{"configuration/warm/updateMultisim", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_updateMultisim},


	{"configuration/warm/serviceEnable", "value", (int (*) (void *, char *)) copy_af_serviceEnable, 0, 0, 
		NULL},
		
	{"configuration/warm/serviceEnable", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_serviceEnable},


	{"configuration/warm/deleteSDEnable", "value", (int (*) (void *, char *)) copy_af_deleteSDEnable, 0, 0, 
		NULL},
		
	{"configuration/warm/deleteSDEnable", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_deleteSDEnable},


	{"configuration/warm/DelayTime", "value", (int (*) (void *, char *)) copy_af_delayTime, 0, 0, 
		NULL},
		
	{"configuration/warm/DelayTime", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_delayTime},


	{"configuration/warm/isdClassOfService", "value", (int (*) (void *, char *)) add_af_isdCOSTree, 0, 0, 
		NULL},
		
	{"configuration/warm/isdClassOfService", "", NULL, 0, 0, 
		(int (*) (void *, char *)) add_af_isdCOSTree},


	{"configuration/warm/ISDRequest-Timeout", "value", (int (*) (void *, char *)) copy_af_isdRequestTimeout, 0, 0, 
		NULL},
		
	{"configuration/warm/ISDRequest-Timeout", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_isdRequestTimeout},


	{"configuration/warm/ISDRequest-MaxRetry", "value", (int (*) (void *, char *)) copy_af_isdRequestMaxRetry, 0, 0, 
		NULL},
		
	{"configuration/warm/ISDRequest-MaxRetry", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_isdRequestMaxRetry},


	{"configuration/warm/smsProxyService", "value", (int (*) (void *, char *)) copy_af_smsProxyService, 0, 0, 
		NULL},
		
	{"configuration/warm/smsProxyService", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_smsProxyService},


	{"configuration/warm/Default-SMS-MAPVersion", "value", (int (*) (void *, char *)) copy_af_defaultSMSMAPVersion, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-SMS-MAPVersion", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defaultSMSMAPVersion},


	{"configuration/warm/moReverseEnable", "value", (int (*) (void *, char *)) copy_af_moReverseEnable, 0, 0, 
		NULL},
		
	{"configuration/warm/moReverseEnable", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_moReverseEnable},


	{"configuration/warm/exceptionalSmscMoList", "value", (int (*) (void *, char *))add_af_exceptionalSmMOTree, 0, 0, 
		NULL},
		
	{"configuration/warm/exceptionalSmscMoList", "", NULL, 0, 0, 
		(int (*) (void *, char *))add_af_exceptionalSmMOTree},


	{"configuration/warm/smscAddress", "value", (int (*) (void *, char *)) copy_af_smscAddress, 0, 0, 
		NULL},
		
	{"configuration/warm/smscAddress", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_smscAddress},


	{"configuration/warm/moRoamingEnable", "value", (int (*) (void *, char *)) copy_af_moRoamingEnable, 0, 0, 
		NULL},
		
	{"configuration/warm/moRoamingEnable", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_moRoamingEnable},


	{"configuration/warm/smsByPassRoaming", "value", (int (*) (void *, char *))add_af_smsByPassRoamTree, 0, 0, 
		},
		
	{"configuration/warm/smsByPassRoaming", "", NULL, 0, 0, 
		(int (*) (void *, char *))add_af_smsByPassRoamTree},


	{"configuration/warm/sgsnByPass", "value", (int (*) (void *, char *)) copy_af_sgsnByPass, 0, 0, 
		NULL},
		
	{"configuration/warm/sgsnByPass", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_sgsnByPass},


	{"configuration/warm/droamCdrException", "value", (int (*) (void *, char *)) copy_af_droamCdrException, 0, 0, 
		NULL},
		
	{"configuration/warm/droamCdrException", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_droamCdrException},


	{"configuration/warm/Default-LOCUP-MAPVersion", "value", (int (*) (void *, char *)) copy_af_defaultLOCUPMAPVersion, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-LOCUP-MAPVersion", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defaultLOCUPMAPVersion},


	{"configuration/warm/Default-CamelServKey", "value", (int (*) (void *, char *)) copy_af_defCamelServKey, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-CamelServKey", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defCamelServKey},


	{"configuration/warm/Default-CamelSCFAddr", "value", (int (*) (void *, char *)) copy_af_defCamelSCFAddr, 0, 0, 
		NULL},
		
	{"configuration/warm/Default-CamelSCFAddr", "", NULL, 0, 0, 
		(int (*) (void *, char *)) copy_af_defCamelSCFAddr},

};

int verifyNoEmptyConfig(char *confVariable, const char *label) {
	if (confVariable[0] == 0){
		SFLOG_DEBUG("%s, Empty", label);
			return NO_SUCCESS;
	}
	else{
		SFLOG_DEBUG("%s [%s]", label, confVariable);
		return SUCCESS;
	}
}

int verifyNoEmptyTree(PRADIXTREE_ITEM root, const char *label) {
	char buff[EQX_BUFF_SIZE_8192 *2] = {0};
	int buff_count = 0;
	if (root->head == NULL) {
		SFLOG_DEBUG("%s, Empty", label);
		return NO_SUCCESS;
	}
	else {
		radixtreeitem_getkeywords(root, buff, sizeof(buff) - 1, &buff_count, ",");
		SFLOG_DEBUG("%s [%s]", label, buff);
		return SUCCESS;
	}
}

int verifyEnableFlag(char *confVariable, const char *label) {
	if (confVariable[0] == 0){
		SFLOG_DEBUG("%s, Empty", label);
			return NO_SUCCESS;
	}
	else if(check_enable_flag(confVariable)) {
			SFLOG_DEBUG("%s invalid [%s]", label, confVariable);
			return NO_SUCCESS;
	}
	else{
		SFLOG_DEBUG("%s [%s]", label, confVariable);
		return SUCCESS;
	}
}

int verifyWithLength(char *confVariable, const char *label, int maxLength) {
	if (confVariable[0] == 0){
		SFLOG_DEBUG("%s, Empty", label);
			return NO_SUCCESS;
	}
	else if(strlen(confVariable)> maxLength) {
			SFLOG_DEBUG("[%s] invalid [%s]", label, confVariable);
			return NO_SUCCESS;
	}
	else{
		SFLOG_DEBUG("%s [%s]", label, confVariable);
		return SUCCESS;
	}
}

int verifyWithCustomFunction(char *confVariable, const char *label, int (*fn) (char *)) {
	if (confVariable[0] == 0){
		SFLOG_DEBUG("%s, Empty", label);
			return NO_SUCCESS;
	}
	else if (fn(confVariable) == NO_SUCCESS) {
			SFLOG_DEBUG("[%s] invalid [%s]", label, confVariable);
			return NO_SUCCESS;
	}
	else {
		SFLOG_DEBUG("%s [%s]", label, confVariable);
		return SUCCESS;
	}
}

int validateUssdMapVersion (char * in)  {
	if (strcmp(in, "0.4.0.0.1.0.19.2") &&  strcmp(in, "0.4.0.0.1.0.19.3")){
		return NO_SUCCESS;
	}
	else {
		return SUCCESS;
	}
}

int validateAuthoritySelector(char * in)  {
	if (strcmp(in, "SAI") &&  strcmp(in, "LU")){
		return NO_SUCCESS;
	}
	else {
		return SUCCESS;
	}
}
void cleanup_config() {
	af_config.tidStart[0] = 0;
	af_config.tidEnd[0] = 0;
	af_config.defaultUSSDMapVersion[0] = 0;
	af_config.ussdSpamEnable[0] = 0;
	af_config.defaultCgPa[0] = 0;
	af_config.ussdAddress[0] = 0;
	af_config.ussdInbound[0] = 0;
	af_config.ivrInbound[0] = 0;
	af_config.e01Timeout[0] = 0;
	af_config.d02Timeout[0] = 0;
	af_config.ldapRetryRet[0] = 0;
	af_config.ldapSearchRetry[0] = 0;
	af_config.ldapModifyRetry[0] = 0;
	af_config.ussdGWTimeout[0] = 0;
	af_config.ussdGWMaxRetry[0] = 0;
	af_config.hlrTimeout[0] = 0;
	af_config.essRelayMaxRetry[0] = 0;
	af_config.mscTimeout[0] = 0;
	af_config.smscTimeout[0] = 0;
	af_config.smscMaxRetry[0] = 0;
	af_config.fdap2pTimeout[0] = 0;
	af_config.fda2pMaxRetry[0] = 0;
	af_config.regionAuthority[0] = 0;
	af_config.authoritySelector[0] = 0;
	af_config.defaultSAIMAPVersion[0] = 0;
	af_config.cloningKiller[0] = 0;
	af_config.droamCdrException[0] = 0;
	af_config.updateMultisim[0] = 0;
	af_config.serviceEnable[0] = 0;
	af_config.deleteSDEnable[0] = 0;
	af_config.delayTime[0] = 0;
	af_config.isdRequestTimeout[0] = 0;
	af_config.isdRequestMaxRetry[0] = 0;
	af_config.defaultSMSMAPVersion[0] = 0;
	af_config.moReverseEnable[0] = 0;
	af_config.smscAddress[0] = 0;
	af_config.moRoamingEnable[0] = 0;
	af_config.sgsnByPass[0] = 0;
	af_config.smsProxyService[0] = 0;
	af_config.smsVerifyRomaing[0] = 0;
	af_config.defaultLOCUPMAPVersion[0] = 0;
	af_config.defCamelServKey[0] = 0;
	af_config.defCamelSCFAddr[0] = 0;

	if (af_config.ussdSpamBlackTree) {
		radixtreeitem_deletenode(af_config.ussdSpamBlackTree);
		af_config.ussdSpamBlackTree = NULL;    
	}

	if (af_config.smsByPassRoamTree) {
		radixtreeitem_deletenode(af_config.smsByPassRoamTree);
		af_config.smsByPassRoamTree = NULL;
	}

	if (af_config.exceptionalSmMOTree) {
		radixtreeitem_deletenode(af_config.exceptionalSmMOTree);
		af_config.exceptionalSmMOTree = NULL;
	}
	if (af_config.isdCOSTree) {
		radixtreeitem_deletenode(af_config.isdCOSTree);
		af_config.isdCOSTree = NULL;
	}
}

int af_get_configure(char *all_conf) {
	cleanup_config();
	af_config.ussdSpamBlackTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.smsByPassRoamTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.exceptionalSmMOTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.isdCOSTree = new_radixtreeitem(NULL, 0, NULL);
	int numitems = sizeof(funcs_AF_CONFIG) /sizeof(XMLFUNC);
	if (xmlfunc_parsedata(all_conf, funcs_AF_CONFIG, numitems, (void *) &af_config) == -1) {
		return NO_SUCCESS;
	}
	else {
		if (verifyNoEmptyConfig(af_config.tidStart, "TCAP-DialogID-Rang-Start") == NO_SUCCESS){
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.tidEnd, "TCAP-DialogID-Rang-End") == NO_SUCCESS){
			return NO_SUCCESS;
		}
		else if (verifyWithCustomFunction(af_config.defaultUSSDMapVersion, "Default-USSD-MAPVersion", validateUssdMapVersion) == NO_SUCCESS) {
			return NO_SUCCESS;
		}		
		else if(verifyEnableFlag(af_config.ussdSpamEnable, "ussdSpamEnable") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyWithLength(af_config.defaultCgPa, "Default-CgPA",15) == NO_SUCCESS)  {
			return NO_SUCCESS;
		}
		else if(verifyWithLength(af_config.ussdAddress, "ussdAddress",15) == NO_SUCCESS)  {
			return NO_SUCCESS;
		}
		else if(verifyEnableFlag(af_config.ussdInbound, "ussdInbound") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyEnableFlag(af_config.ivrInbound, "ivrInbound") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.e01Timeout, "E01Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.d02Timeout, "D02-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.ldapRetryRet, "LDAPRetryRet") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.ldapSearchRetry, "LDAPSearchRetry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.ldapModifyRetry, "LDAPModifyRetry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.ussdGWTimeout, "USSDGW-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.ussdGWMaxRetry, "USSDGW-Max-Retry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.hlrTimeout, "HLR-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.essRelayMaxRetry, "ESS-Relay-Max-Retry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.mscTimeout, "MSC-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.smscTimeout, "SMSC-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.smscMaxRetry, "SMSC-Max-Retry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.fdap2pTimeout, "FDAP2P-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.fda2pMaxRetry, "FDAP2P-Max-Retry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.regionAuthority, "regionAuthority") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.authoritySelector, "authoritySelector") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyWithCustomFunction(af_config.authoritySelector, "authoritySelector", validateAuthoritySelector) == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if(verifyNoEmptyConfig(af_config.defaultSAIMAPVersion, "Default-SAI-MAPVersion") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.cloningKiller, "cloningKiller") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.droamCdrException, "droamCdrException") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.updateMultisim, "updateMultisim") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.serviceEnable, "serviceEnable") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.deleteSDEnable, "deleteSDEnable") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.delayTime, "DelayTime") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.isdRequestTimeout, "ISDRequest-Timeout") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.isdRequestMaxRetry, "ISDRequest-MaxRetry") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.defaultSMSMAPVersion, "Default-SMS-MAPVersion") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.moReverseEnable, "moReverseEnable") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.smscAddress, "smscAddress") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.moRoamingEnable, "moRoamingEnable") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.sgsnByPass, "sgsnByPass") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyEnableFlag(af_config.smsProxyService, "smsProxyService") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.defaultLOCUPMAPVersion, "Default-LOCUP-MAPVersion") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.defCamelServKey, "Default-CamelServKey") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyConfig(af_config.defCamelSCFAddr, "Default-CamelSCFAddr") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyTree(af_config.ussdSpamBlackTree, "ussdSpamBlacklist") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyTree(af_config.smsByPassRoamTree, "smsByPassRoaming") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyTree(af_config.exceptionalSmMOTree, "exceptionalSmscMoList") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else if (verifyNoEmptyTree(af_config.isdCOSTree, "isdClassOfService") == NO_SUCCESS) {
			return NO_SUCCESS;
		}
		else {
			return SUCCESS;
		}
	}
	return 0;
	
}
#include "parserclass_test.h"
int main(int argc, char **argv) {
	char * holder ;
	do {
		int retAF ;
		holder = read_file("test_content.xml");
		retAF = af_get_configure(holder);
		test_fun("AF Get CONFIGURE ", retAF != SUCCESS);
		
		test_fun("smsByPassRoamTree present [66818124578]", radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124578") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124579"));
		free(holder);
		
		holder = read_file("test_string_only.xml");
		retAF = af_get_configure(holder);
		test_fun("AF Get CONFIGURE ", retAF != SUCCESS);
		
		test_fun("TCAP-DialogID-Rang-Start [1]", !strcmp(af_config.tidStart, "1"));
		test_fun("TCAP-DialogID-Rang-End [10000000]", !strcmp(af_config.tidEnd, "10000000"));
		free(holder);
		
		holder = read_file("test_minim.xml");
		retAF = af_get_configure(holder);
		test_fun("AF Get CONFIGURE ", retAF == SUCCESS);
		test_fun("TCAP-DialogID-Rang-Start [1]", !strcmp(af_config.tidStart, "1"));
		test_fun("TCAP-DialogID-Rang-End [10000000]", !strcmp(af_config.tidEnd, "10000000"));
		test_fun("ussdSpamBlackTree present [856207000048]", radixtreeitem_findkey(af_config.ussdSpamBlackTree, "856207000048") != NULL);
		test_fun("ussdSpamBlackTree absent  [342342323]", !radixtreeitem_findkey(af_config.ussdSpamBlackTree, "342342323"));
		test_fun("smsByPassRoamTree present [66810000015]", radixtreeitem_findkey(af_config.smsByPassRoamTree, "66810000015") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124579"));
		test_fun("exceptionalSmscMoList present [491700750000]", radixtreeitem_findkey(af_config.exceptionalSmMOTree, "491700750000") != NULL);
		free(holder);
		
		holder = read_file("test_exceed.xml");
		retAF = af_get_configure(holder);
		test_fun("AF Get CONFIGURE ", retAF == SUCCESS);
		test_fun("TCAP-DialogID-Rang-Start [1]", !strcmp(af_config.tidStart, "1"));
		test_fun("TCAP-DialogID-Rang-End [10000000]", !strcmp(af_config.tidEnd, "10000000"));
		test_fun("ussdSpamBlackTree present [856207000048]", radixtreeitem_findkey(af_config.ussdSpamBlackTree, "856207000048") != NULL);
		test_fun("ussdSpamBlackTree absent  [342342323]", !radixtreeitem_findkey(af_config.ussdSpamBlackTree, "342342323"));
		test_fun("smsByPassRoamTree present [66810000015]", radixtreeitem_findkey(af_config.smsByPassRoamTree, "66810000015") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124579"));
		test_fun("exceptionalSmscMoList present [491700750000]", radixtreeitem_findkey(af_config.exceptionalSmMOTree, "491700750000") != NULL);
		test_fun("exceptionalSmscMoList present [6596845994]", radixtreeitem_findkey(af_config.exceptionalSmMOTree, "6596845994") != NULL);
		free(holder);
		usleep(2000);
	}while(argc >= 2 && !strcmp(argv[1], "-m"));
	
	return 0;
}
