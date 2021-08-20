#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "radixtree.h"
#include "yxml.h"
#include "af.h"
#include "xmlf.h"

/**
 * NAME					: addConfigElemContent
 * DESCRIPTION	: add tree relative to root to handle case if element content found.
 * 								Element content and element value attribute handled using same function
 */
int addConfigElemContent(PPRIMTREE_ITEM root, char *path, int (*update_value) (void *, char*), void *dest) {
	PPRIMTREE_ITEM tmp = xmlf_add_element(root, path, update_value, dest);
	xfn_attribute_data *udata = new_xfn_attribute_data("value", update_value, dest);
	PPRIML_ITEM new_item = newpriml_item();
	new_item->set_data(new_item, udata);
	new_item->set_data_remove_fn(new_item, (int (*) (void *)) free);
	tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))xfn_attribute_cmp);
	return 1;
	
}


int radixtree_add(void *dst, char *in) {
	PRADIXTREE_ITEM rdx  = * ((PRADIXTREE_ITEM *) dst);
	radixtreeitem_insertkey(rdx, in);
	return 1;
	
}

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


PRIMTREE_ITEM configFunctionRoot;
int setup_functions() {
	static int loaded =  0;
	if(!loaded) {
		primtreeitem_ctor(&configFunctionRoot);
		PPRIMTREE_ITEM root = &configFunctionRoot;
		addConfigElemContent(root, "configuration/warm/TCAP-DialogID-Rang-Start", copy_string_20, af_config.tidStart);
		addConfigElemContent(root, "configuration/warm/TCAP-DialogID-Rang-End", copy_string_20, af_config.tidEnd);
		addConfigElemContent(root, "configuration/warm/Default-USSD-MAPVersion", copy_string_128, af_config.defaultUSSDMapVersion);
		addConfigElemContent(root, "configuration/warm/ussdSpamBlacklist", radixtree_add, &af_config.ussdSpamBlackTree);
		addConfigElemContent(root, "configuration/warm/ussdSpamEnable", copy_string_64, af_config.ussdSpamEnable);
		addConfigElemContent(root, "configuration/warm/Default-CgPA", copy_string_128, af_config.defaultCgPa);
		addConfigElemContent(root, "configuration/warm/ussdAddress", copy_string_128, af_config.ussdAddress);
		addConfigElemContent(root, "configuration/warm/ussdInbound", copy_string_64, af_config.ussdInbound);
		addConfigElemContent(root, "configuration/warm/ivrInbound", copy_string_64, af_config.ivrInbound);
		addConfigElemContent(root, "configuration/warm/E01Timeout", copy_string_64, af_config.e01Timeout);
		addConfigElemContent(root, "configuration/warm/D02-Timeout", copy_string_64, af_config.d02Timeout);
		addConfigElemContent(root, "configuration/warm/LDAPRetryRet", copy_string_64, af_config.ldapRetryRet);
		addConfigElemContent(root, "configuration/warm/LDAPSearchRetry", copy_string_64, af_config.ldapSearchRetry);
		addConfigElemContent(root, "configuration/warm/LDAPModifyRetry", copy_string_64, af_config.ldapModifyRetry);
		addConfigElemContent(root, "configuration/warm/USSDGW-Timeout", copy_string_64, af_config.ussdGWTimeout);
		addConfigElemContent(root, "configuration/warm/USSDGW-Max-Retry", copy_string_64, af_config.ussdGWMaxRetry);
		addConfigElemContent(root, "configuration/warm/HLR-Timeout", copy_string_64, af_config.hlrTimeout);
		addConfigElemContent(root, "configuration/warm/ESS-Relay-Max-Retry", copy_string_64, af_config.essRelayMaxRetry);
		addConfigElemContent(root, "configuration/warm/MSC-Timeout", copy_string_64, af_config.mscTimeout);
		addConfigElemContent(root, "configuration/warm/SMSC-Timeout", copy_string_64, af_config.smscTimeout);
		addConfigElemContent(root, "configuration/warm/SMSC-Max-Retry", copy_string_64, af_config.smscMaxRetry);
		addConfigElemContent(root, "configuration/warm/FDAP2P-Timeout", copy_string_64, af_config.fdap2pTimeout);
		addConfigElemContent(root, "configuration/warm/FDAP2P-Max-Retry", copy_string_64, af_config.fda2pMaxRetry);
		addConfigElemContent(root, "configuration/warm/regionAuthority", copy_string_64, af_config.regionAuthority);
		addConfigElemContent(root, "configuration/warm/authoritySelector", copy_string_64, af_config.authoritySelector);
		addConfigElemContent(root, "configuration/warm/Default-SAI-MAPVersion", copy_string_128, af_config.defaultSAIMAPVersion);
		addConfigElemContent(root, "configuration/warm/cloningKiller", copy_string_64, af_config.cloningKiller);
		addConfigElemContent(root, "configuration/warm/updateMultisim", copy_string_64, af_config.updateMultisim);
		addConfigElemContent(root, "configuration/warm/serviceEnable", copy_string_64, af_config.serviceEnable);
		addConfigElemContent(root, "configuration/warm/deleteSDEnable", copy_string_64, af_config.deleteSDEnable);
		addConfigElemContent(root, "configuration/warm/DelayTime", copy_string_128, af_config.delayTime);
		addConfigElemContent(root, "configuration/warm/isdClassOfService", radixtree_add, &af_config.isdCOSTree);
		addConfigElemContent(root, "configuration/warm/ISDRequest-Timeout", copy_string_64, af_config.isdRequestTimeout);
		addConfigElemContent(root, "configuration/warm/ISDRequest-MaxRetry", copy_string_64, af_config.isdRequestMaxRetry);
		addConfigElemContent(root, "configuration/warm/smsProxyService", copy_string_64, af_config.smsProxyService);
		addConfigElemContent(root, "configuration/warm/Default-SMS-MAPVersion", copy_string_128, af_config.defaultSMSMAPVersion);
		addConfigElemContent(root, "configuration/warm/moReverseEnable", copy_string_64, af_config.moReverseEnable);
		addConfigElemContent(root, "configuration/warm/exceptionalSmscMoList", radixtree_add, &af_config.exceptionalSmMOTree);
		addConfigElemContent(root, "configuration/warm/smscAddress", copy_string_128, af_config.smscAddress);
		addConfigElemContent(root, "configuration/warm/moRoamingEnable", copy_string_64, af_config.moRoamingEnable);
		addConfigElemContent(root, "configuration/warm/smsByPassRoaming", radixtree_add, &af_config.smsByPassRoamTree);
		addConfigElemContent(root, "configuration/warm/sgsnByPass", copy_string_64, af_config.sgsnByPass);
		addConfigElemContent(root, "configuration/warm/droamCdrException", copy_string_256, af_config.droamCdrException);
		addConfigElemContent(root, "configuration/warm/Default-LOCUP-MAPVersion", copy_string_128, af_config.defaultLOCUPMAPVersion);
		addConfigElemContent(root, "configuration/warm/Default-CamelServKey", copy_string_64, af_config.defCamelServKey);
		addConfigElemContent(root, "configuration/warm/Default-CamelSCFAddr", copy_string_64, af_config.defCamelSCFAddr);
		loaded = 1;
	}
	
	return 1;
}

int xmlf_parseconfig(char *xml_string, PRIMTREE_ITEM *fn_data) {
	char *doc_p = xml_string;
	yxml_t xml_elem;
	yxml_ret_t xml_ret;
	char buf[8192] = {0};
	yxml_init(&xml_elem, buf, 8192);
	DLIST stack;
	dlist_ctor(&stack);
	char *item_name = NULL;
	char item_path[200] = {0};
	char attrib_name[200] = {0};
	char attrib_val[200] = {0};
	char content_val[200] = {0};
	PDLIST_ITEM dlitem;
	xfn_element_data *eldat = NULL;
	xfn_attribute_data *attdat = NULL;
	while(*doc_p){
		xml_ret = yxml_parse(&xml_elem, *doc_p);
		switch(xml_ret) {
			case YXML_ELEMSTART:
				dlitem = new_dlist_item();
				item_name = copy_string(&item_name, xml_elem.elem);
				dlitem->set_data(dlitem, item_name);
				stack.push(&stack, dlitem);
				item_name = NULL;
				break;
			case YXML_ELEMEND:
				dlist_serialize(&stack, item_path, "/");
				if ((eldat = xmlf_find_element(fn_data, item_path)) && eldat->extract_content && content_val[0]) {
					eldat->update_content(eldat->dst_ptr, content_val);
				}
				content_val[0] = '\0';
				item_path[0] = '\0';
				dlitem = stack.pop(&stack);
				dlitem->delete(dlitem);
				dlitem = NULL;
				break;

			case YXML_ATTRSTART:
				strcpy(attrib_name, xml_elem.attr);
				break;
			case YXML_ATTRVAL:
				strcat(attrib_val, xml_elem.data);
				break;
			case YXML_ATTREND:
				dlist_serialize(&stack, item_path, "/");
				if ( (attdat = xmlf_find_attribute(fn_data, item_path, attrib_name)) ) {
					attdat->update_value(attdat->dst_ptr, attrib_val);
				}
				attrib_name [0] = '\0';
				attrib_val [0] = '\0';
				item_path[0] = '\0';
				break;
			case YXML_CONTENT:
				dlist_serialize(&stack, item_path, "/");
				if ((eldat = xmlf_find_element(fn_data, item_path)) && eldat->extract_content) {
					strcat(content_val, xml_elem.data);
				}
				item_path[0] = '\0';
				break;
			case YXML_ESYN:
				dlist_cleanup(&stack);
				return -1;
			default: 
				break;
		}
		doc_p ++;
	}
	dlist_cleanup(&stack);
	return 1;
}

int check_enable_flag(char *param) {
	return (strcmp(param, "0") && strcmp(param, "1"));
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
int af_get_configure(char *all_conf) {
	setup_functions();

	cleanup_config();
	
	af_config.ussdSpamBlackTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.smsByPassRoamTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.exceptionalSmMOTree = new_radixtreeitem(NULL, 0, NULL);
	af_config.isdCOSTree = new_radixtreeitem(NULL, 0, NULL);
	
	if(xmlf_parseconfig(all_conf, &configFunctionRoot) == -1) {
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
}
int file_size(FILE *fin) {
  int nPos,nFSize;
  nPos=ftell(fin);
  fseek(fin,0L,SEEK_END);
  nFSize=ftell(fin);
  fseek(fin,nPos,SEEK_SET);
  return nFSize;
}

#include <unistd.h>

#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

void print_assertion(int assertion) {
  if (assertion){
    printf("\n%sOk", KBLU);
  }
  else{
    printf("\n%sfailed", KRED);
  }
  printf("%s\n", KNRM);
}

void test_fun(char *name, int fn){
	fprintf(stdout, "%s+%s\n", KCYN, name); 
	printf("%s", KNRM);  
	print_assertion(fn);
}

int main (int argc, char **argv) {
	
	if (argc < 2) {
		fprintf(stdout , "please %s xml_file.xml\n", argv[0]);
		return 1;
	}
	
	do{
		ssize_t n_read = 0;
		ssize_t n_buff = 0;
// 		PPRIMTREE_ITEM curr;
// 		setup_functions();
// 		af_config.ussdSpamBlackTree = new_radixtreeitem(NULL, 0, NULL);
// 		af_config.smsByPassRoamTree = new_radixtreeitem(NULL, 0, NULL);
// 		af_config.exceptionalSmMOTree = new_radixtreeitem(NULL, 0, NULL);
// 		af_config.isdCOSTree = new_radixtreeitem(NULL, 0, NULL);
		FILE *fp = fopen(argv[1], "r");
		int fsz = file_size(fp);
		char * holder = calloc(fsz + 1, sizeof(char));
		char *ptrh = holder;    
		do {
			ptrh+=n_read;
			n_read = fread(ptrh, 1, 32, fp);
			n_buff += n_read;
			//fprintf(stdout, "n_read = %zd n_buff = %zd\n", n_read, n_buff);
		}while(n_read == 32 && n_buff < fsz);
		fclose(fp);
		
		af_get_configure(holder);
			
		test_fun("TCAP-DialogID-Rang-Start [1440]", !strcmp(af_config.tidStart, "1"));
		test_fun("TCAP-DialogID-Rang-End [100]", !strcmp(af_config.tidEnd, "10000000"));
		test_fun("ussdSpamBlackTree present [856207000048]", radixtreeitem_findkey(af_config.ussdSpamBlackTree, "856207000048") != NULL);
		test_fun("ussdSpamBlackTree absent  [342342323]", !radixtreeitem_findkey(af_config.ussdSpamBlackTree, "342342323"));
		test_fun("smsByPassRoamTree present [66810000015]", radixtreeitem_findkey(af_config.smsByPassRoamTree, "66810000015") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124579"));
		free(holder);
// 		curr = root.detach_head(&root) ;
// 		while(curr) {
// 			curr->delete(curr);
// 			curr = root.detach_head(&root) ;
// 		}
// 		radixtreeitem_deletenode(af_config.ussdSpamBlackTree);
// 		radixtreeitem_deletenode(af_config.smsByPassRoamTree);
// 		radixtreeitem_deletenode(af_config.exceptionalSmMOTree);
// 		radixtreeitem_deletenode(af_config.isdCOSTree);
	}while(argc >= 3 && !strcmp(argv[2], "-m"));
	
	return 0;
}
