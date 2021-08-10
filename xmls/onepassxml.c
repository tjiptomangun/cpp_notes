#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h"
#include "radixtree.h"
#include "yxml.h"

#define EQX_BUFF_SIZE_10 10
#define EQX_BUFF_SIZE_64 64
#define EQX_BUFF_SIZE_128 128
#define EQX_BUFF_SIZE_256 256
#define EQX_BUFF_SIZE_10 10

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
/**
 * name 						:is name of this element
 * update_content		:function to update config_ptr. 
 *										1st parameter of this function will be configPtr,
 *										2nd  parameter of this function will be content of xml
 * extract_content 	:flag whether to store this xml content to configPtr
 * dst_ptr				:where to store the result, this is destination (1st param)
 * 									 of update_content
 */
typedef struct {
	char name [80];
	int (*update_content) (void*, char*);
	int extract_content; //if 1 mark the last path of element, 0 have next element
	void *dst_ptr; //destination to store the result
}cfg_element_data;


/**
 * name 						:is name of this attribute
 * update_value			:function to update config_ptr. 
 *										1st parameter of this function will be configPtr,
 *										2nd  parameter of this function will be content of xml
 * dst_ptr				:where to store the result, this is destination (1st param)
 * 									 of update_content
 */
typedef struct {
	char name [80];
	int (*update_value) (void*, char*);
	void *dst_ptr; //destination to store the result
}cfg_attribute_data;

cfg_attribute_data * attribute_data_ctor(cfg_attribute_data * in, char *name, int (*update_value) (void*, char*), void *dest) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_value = update_value;
		in->dst_ptr = dest;
	}
	return in;
}

cfg_attribute_data * new_attribute_data(char *name, int (*update_value) (void *, char*), void *dest) {
	cfg_attribute_data * ret = (cfg_attribute_data *) calloc(1, sizeof(cfg_attribute_data));
	return attribute_data_ctor(ret, name, update_value, dest);
}

cfg_element_data * element_data_ctor(cfg_element_data *in, char *name, int (*update_value) (void*, char*), int extract_content, void *dest) {
	if (in) {
		strncpy(in->name, name, sizeof(in->name));
		in->update_content = update_value;
		in->extract_content = extract_content;
		in->dst_ptr = dest;
	}
	return in;
}

cfg_element_data * new_element_data(char *name, int (*update_value) (void *, char*), int extract_content, void *dest) {
	cfg_element_data * ret = (cfg_element_data *) calloc(1, sizeof(cfg_element_data));
	return element_data_ctor(ret, name, update_value, extract_content, dest);
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


int radixtree_add(void *dst, char *in) {
	PRADIXTREE_ITEM rdx  = * ((PRADIXTREE_ITEM *) dst);
	radixtreeitem_insertkey(rdx, in);
	return 1;
	
}

int element_cmp(cfg_element_data * c1, cfg_element_data *c2) {
	return strcmp(c1->name, c2->name);
}

int attribute_cmp(cfg_attribute_data * c1, cfg_attribute_data * c2) {
	return strcmp(c1->name, c2->name);
}

int do_nothing(void *dest, char *in){
	return 1;
}

/**
 * NAME 				: xmlf_add_element
 * DESCRIPTION	: add one element relative to node with path path_to_find. 
 * 								If element already exists will not add or update
 */
PPRIMTREE_ITEM xmlf_add_element(PPRIMTREE_ITEM node, char *path_to_find, int (*update_value) (void *, char*), void *dest){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;
	cfg_element_data * elem = NULL;
	int have_next = 0;

	char elem_name[100] = {0};
	PPRIMTREE_ITEM next_tree;

	while(*p_path == ' '){
		p_path ++;
	}

	start_path = p_path;
	while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
		p_path ++;
	}

	/* empty path or end of string */
	if (start_path == p_path){
		return node;
	}

	end_path = p_path;

	while (*p_path == ' ')
		p_path ++;

	if (*p_path == '/') {
		p_path ++;
		have_next = 1;
	}

	strncpy(elem_name, start_path, end_path - start_path);
	if (have_next) {
		elem = new_element_data(elem_name, do_nothing, 0, NULL);
	}
	else {
		elem = new_element_data(elem_name, update_value, 1, dest);
	}
	if (! (next_tree = node->find_one(node, elem, (int (*) (void *, void *))element_cmp))) {
		
		next_tree = newprimtreeitem();
		next_tree->set_data(next_tree, elem);
		node->add_one(node, next_tree, (int (*) (void *, void *))element_cmp);
	}
	else {
		free(elem);
	}
	return xmlf_add_element(next_tree, p_path, update_value, dest);
	
}

/**
 * _xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: found element
 */
PPRIMTREE_ITEM _xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	char *p_path = path_to_find;
	char *start_path = NULL, *end_path = NULL;

	char elem_name[100] = {0};
	cfg_element_data elem;
	
	PPRIMTREE_ITEM next_tree;

		while(*p_path == ' '){
			p_path ++;
		}

		start_path = p_path;
		while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
			p_path ++;
		}

		/* empty path or end of string */
		if (start_path == p_path){
			return node;
		}

		end_path = p_path;

		while (*p_path == ' ')
			p_path ++;

		if (*p_path == '/')
			p_path ++;

		strncpy(elem_name, start_path, end_path - start_path);
		element_data_ctor(&elem, elem_name, do_nothing, 0, NULL);
		
		if (! (next_tree = node->find_one(node, &elem, (int (*) (void *, void *))element_cmp))) {
			return next_tree;
		}
		else {
			return _xmlf_find_element(next_tree, p_path);
		}
}

/**
 * xmlf_find_element : _xmlf_find_element
 * DESCRIPTION				: find an element that match this path
 * 											relative (but not include) to node
 * RETURNS
 * 		NULL						: not found
 * 		OTHER						: data that stored in this element
 */
void * xmlf_find_element(PPRIMTREE_ITEM node, char *path_to_find){
	PPRIMTREE_ITEM found = _xmlf_find_element(node, path_to_find);
	if(found) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}
/**
 * NAME				 	: xmlf_find_attribute
 * DESCRIPTION 	: find attribute data attached to path_to_find attribute_name
 * 								this data is previously allocated
 */
cfg_attribute_data * xmlf_find_attribute(PPRIMTREE_ITEM node, char *path_to_find, char *attribute_name){
	PPRIMTREE_ITEM elem = NULL;
	cfg_attribute_data attr;
	PPRIML_ITEM found = NULL;
	attribute_data_ctor(&attr, attribute_name, do_nothing, NULL);
	if ( node && path_to_find && attribute_name && (elem = _xmlf_find_element(node, path_to_find)) 
		&& (found = elem->list.find_one(&elem->list, &attr,  (int (*) (void *, void *))attribute_cmp)) ) {
		return found->get_data(found);
	}
	else {
		return NULL;
	}
}
/**
 * NAME					: addConfigElemContent
 * DESCRIPTION	: add tree relative to root to handle case if element content found.
 * 								Element content and element value attribute handled using same function
 */
int addConfigElemContent(PPRIMTREE_ITEM root, char *path, int (*update_value) (void *, char*), void *dest) {
	PPRIMTREE_ITEM tmp = xmlf_add_element(root, path, update_value, dest);
	cfg_attribute_data *udata = new_attribute_data("value", update_value, dest);
	PPRIML_ITEM new_item = newpriml_item();
	new_item->set_data(new_item, udata);
	new_item->set_data_remove_fn(new_item, (int (*) (void *)) free);
	tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	return 1;
	
}


int setup_e01_functions(PPRIMTREE_ITEM root) {
		fprintf(stdout, "setup functions");
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
	return 1;
}
int setup_functions_old(PPRIMTREE_ITEM root) {
	PPRIMTREE_ITEM tmp = NULL;
	xmlf_add_element(root, "configuration/warm/TCAP-DialogID-Rang-Start", copy_string_20, af_config.tidStart);
	xmlf_add_element(root, "configuration/warm/TCAP-DialogID-Rang-End", copy_string_20, af_config.tidEnd);
	xmlf_add_element(root, "configuration/warm/ussdSpamBlacklist", radixtree_add, &af_config.ussdSpamBlackTree);
	xmlf_add_element(root, "configuration/warm/smsByPassRoaming", radixtree_add, &af_config.smsByPassRoamTree);
	if ((tmp = _xmlf_find_element(root, "configuration/warm/TCAP-DialogID-Rang-Start"))) {
		cfg_attribute_data *udata = new_attribute_data("value", copy_string_20, af_config.tidStart);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		new_item->set_data_remove_fn(new_item, (int (*) (void *)) free);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/TCAP-DialogID-Rang-End"))) {
		cfg_attribute_data *udata = new_attribute_data("value", copy_string_20, af_config.tidEnd);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/ussdSpamBlacklist"))) {
		cfg_attribute_data *udata = new_attribute_data("value", radixtree_add, &af_config.ussdSpamBlackTree);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	if ((tmp = _xmlf_find_element(root, "configuration/warm/smsByPassRoaming"))) {
		cfg_attribute_data *udata = new_attribute_data("value", radixtree_add, &af_config.smsByPassRoamTree);
		PPRIML_ITEM new_item = newpriml_item();
		new_item->set_data(new_item, udata);
		tmp->list.add_one(&tmp->list, new_item, (int (*) (void *, void *))attribute_cmp);
	}
	return 1;
}

PPRIMTREE_ITEM xmlf_find_one_child(PPRIMTREE_ITEM item, char *name) {
	PPRIMTREE_ITEM found = NULL;
	cfg_element_data to_find;
	element_data_ctor(&to_find, name, do_nothing, 0, NULL);

	if (item && ( (found = item->find_one(item, &to_find,  (int (*) (void *, void *))element_cmp)) ) ){
		return found;
	}
	return NULL;
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
	cfg_element_data *eldat = NULL;
	cfg_attribute_data *attdat = NULL;
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
				return -1;
			default: 
				break;
		}
		doc_p ++;
	}
	return 1;
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
PRADIXTREE_ITEM radixtreeitem_findprefix(PRADIXTREE_ITEM root, char * to_find);
PRADIXTREE_ITEM radixtreeitem_findkey(PRADIXTREE_ITEM root, char * to_find);
int main (int argc, char **argv) {
	
	if (argc < 2) {
		fprintf(stdout , "please %s xml_file.xml\n", argv[0]);
		return 1;
	}
	
	do{
		ssize_t n_read = 0;
		ssize_t n_buff = 0;
		PRIMTREE_ITEM root;
		PPRIMTREE_ITEM curr;
		primtreeitem_ctor(&root);
		setup_e01_functions(&root);
		af_config.ussdSpamBlackTree = new_radixtreeitem(NULL, 0, NULL);
		af_config.smsByPassRoamTree = new_radixtreeitem(NULL, 0, NULL);
		af_config.exceptionalSmMOTree = new_radixtreeitem(NULL, 0, NULL);
		af_config.isdCOSTree = new_radixtreeitem(NULL, 0, NULL);
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
		
		xmlf_parseconfig(holder, &root);
			
		test_fun("TCAP-DialogID-Rang-Start [1440]", !strcmp(af_config.tidStart, "1"));
		test_fun("TCAP-DialogID-Rang-End [100]", !strcmp(af_config.tidEnd, "10000000"));
		test_fun("ussdSpamBlackTree present [856207000048]", radixtreeitem_findkey(af_config.ussdSpamBlackTree, "856207000048") != NULL);
		test_fun("ussdSpamBlackTree absent  [342342323]", !radixtreeitem_findkey(af_config.ussdSpamBlackTree, "342342323"));
		test_fun("smsByPassRoamTree present [66810000015]", radixtreeitem_findkey(af_config.smsByPassRoamTree, "66810000015") != NULL);
		test_fun("smsByPassRoamTree absent [66818124579]", !radixtreeitem_findkey(af_config.smsByPassRoamTree, "66818124579"));
		fclose(fp);
		free(holder);
		curr = root.detach_head(&root) ;
		while(curr) {
			curr->delete(curr);
			curr = root.detach_head(&root) ;
		}
		radixtreeitem_deletenode(af_config.ussdSpamBlackTree);
		radixtreeitem_deletenode(af_config.smsByPassRoamTree);
		radixtreeitem_deletenode(af_config.exceptionalSmMOTree);
		radixtreeitem_deletenode(af_config.isdCOSTree);
	}while(argc >= 3 && !strcmp(argv[2], "-m"));
}
