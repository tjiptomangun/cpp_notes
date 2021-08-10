#include "xmle01.h"
#include "parserclass.h"
#include "yxml.h"
#include <stdlib.h>
#include <string.h>
#include "af.h"

PRIMTREE_ITEM e0ParseFuncionRoot;
int copy_e01_command(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->command , in, 256);
	}
	return 1;
}
int copy_e01_resultcode(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->resultcode , in, 256);
	}
	return 1;
}
int copy_e01_description(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->description, in, 256);
	}
	return 1;
}
int copy_e01_objecttype(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->objecttype, in, 256);
	}
	return 1;
}
int copy_e01_key0(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->key0, in, 256);
	}
	return 1;
}
int copy_e01_key1(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->key1, in, 256);
	}
	return 1;
}
int copy_e01_key2(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->key2, in, 256);
	}
	return 1;
}
int copy_e01_key3(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->key3, in, 256);
	}
	return 1;
}
int copy_e01_key4(EQX_MSG_E01 *msg_e01, char *in) {
	if (msg_e01) {
		strncpy(msg_e01->key4, in, 256);
	}
	return 1;
}

int store_ptr_e01_value(EQX_MSG_E01 *msg_e01, char *in) {
	msg_e01->value_ptr = in;
	return 1;
}
int setup_e01_functions() {
	static int e01ParseFuncLoaded = 0;
	if (!e01ParseFuncLoaded) {
		primtreeitem_ctor(&e0ParseFuncionRoot);
		e01ParseFuncLoaded = 1;
	}
	PPRIMTREE_ITEM root = &e0ParseFuncionRoot;
	
	xmle01_add_attribute(root, "message", "command", new_xe01_attribute_data("command",(int (*) (void *, char *)) copy_e01_command, 0));
	xmle01_add_attribute(root, "message", "resultcode", new_xe01_attribute_data("resultcode", (int (*) (void *, char *)) copy_e01_resultcode, 0));
	xmle01_add_attribute(root, "message", "description", new_xe01_attribute_data("description", (int (*) (void *, char *)) copy_e01_description, 0));
	xmle01_add_attribute(root, "message", "objecttype", new_xe01_attribute_data("objecttype", (int (*) (void *, char *)) copy_e01_objecttype, 0));
	xmle01_add_attribute(root, "message", "key0", new_xe01_attribute_data("key0", (int (*) (void *, char *)) copy_e01_key0, 0));
	xmle01_add_attribute(root, "message", "key1", new_xe01_attribute_data("key1", (int (*) (void *, char *)) copy_e01_key1, 0));
	xmle01_add_attribute(root, "message", "key2", new_xe01_attribute_data("key2", (int (*) (void *, char *)) copy_e01_key2, 0));
	xmle01_add_attribute(root, "message", "key3", new_xe01_attribute_data("key3", (int (*) (void *, char *)) copy_e01_key3, 0));
	xmle01_add_attribute(root, "message", "key4", new_xe01_attribute_data("key4", (int (*) (void *, char *)) copy_e01_key4, 0));
	xmle01_add_attribute(root, "message/data", "value", new_xe01_attribute_data("value", (int (*) (void *, char *)) store_ptr_e01_value, 1));
	return 1;
}

#include <stdlib.h>
#include <unistd.h>
int file_size(FILE *fin) {
  int nPos,nFSize;
  nPos=ftell(fin);
  fseek(fin,0L,SEEK_END);
  nFSize=ftell(fin);
  fseek(fin,nPos,SEEK_SET);
  return nFSize;
}
char * read_file(char *filename) {
	ssize_t n_read = 0;
	ssize_t n_buff = 0;
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		return NULL;
	}
	else {
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
		return holder;
	}
}
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
void printall(PPRIMTREE_ITEM root) {
	PPRIMTREE_ITEM curr = root;
	PPRIML_ITEM curitem;
	while(curr) {
		curitem = curr->list.get_first_child(&curr->list);
		while(curitem) {
			xe01_attribute_data * ad = (xe01_attribute_data *)curitem ->get_data(curitem);
			fprintf(stdout, "%s\n", ad->name);
			curitem = curr->list.get_next_child(&curr->list);
			
		}
		curr = curr->head;
	}
}

int main (int argc, char **argv) {
	EQX_MSG_E01 *msg_e01 = NULL;
	msg_e01 = (EQX_MSG_E01 * )calloc(1, sizeof(EQX_MSG_E01));
	char * holder ;
	PPRIMTREE_ITEM root = &e0ParseFuncionRoot;
	do {
		holder = read_file("e01small.xml");
		setup_e01_functions();
		xmle01_parsedata(holder, root, (void *) msg_e01);
		printall(root);
		test_fun("command = search", !strcmp(msg_e01->command, "search"));
		test_fun("resultcode = 0", !strcmp(msg_e01->resultcode, "0"));
		test_fun("description = 0", !strcmp(msg_e01->description, "SUCCESS"));
		test_fun("objecttype = 0", !strcmp(msg_e01->objecttype, "regionAuthorization"));
		test_fun("key0 = 520084910204920", !strcmp(msg_e01->key0, "520084910204920"));
		test_fun("key1 = ", !strcmp(msg_e01->key1, ""));
		test_fun("key2 = ", !strcmp(msg_e01->key2, ""));
		test_fun("key3 = ", !strcmp(msg_e01->key3, ""));
		test_fun("key4 = ", !strcmp(msg_e01->key4, ""));
		test_fun("value_ptr = !@#$%&amp;", !strcmp(msg_e01->value_ptr, "!@#$%&amp;"));
		free(holder);
		
		holder = read_file("e01large.xml");
		xmle01_parsedata(holder, root, (void *) msg_e01);
		printall(root);
		test_fun("command = search", !strcmp(msg_e01->command, "search"));
		test_fun("resultcode = 0", !strcmp(msg_e01->resultcode, "0"));
		test_fun("description = 0", !strcmp(msg_e01->description, "SUCCESS"));
		test_fun("objecttype = 0", !strcmp(msg_e01->objecttype, "regionAuthorization"));
		test_fun("key0 = 520084910204920", !strcmp(msg_e01->key0, "520084910204920"));
		test_fun("key1 = ", !strcmp(msg_e01->key1, ""));
		test_fun("key2 = ", !strcmp(msg_e01->key2, ""));
		test_fun("key3 = ", !strcmp(msg_e01->key3, ""));
		test_fun("key4 = ", !strcmp(msg_e01->key4, ""));
		fprintf(stdout, "%s\n", msg_e01->value_ptr);
		free(holder);
	}while(argc >= 2 && !strcmp(argv[1], "-m"));
}

