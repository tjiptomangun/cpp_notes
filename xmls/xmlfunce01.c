#include "xmlfunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "af.h"
#include "parserclass_test.h"

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

XMLFUNC funcsE01[11] = {
		{"message", "command", (int (*) (void *, char *)) copy_e01_command, 0, 0, NULL},
		{"message", "resultcode", (int (*) (void *, char *)) copy_e01_resultcode, 0, 0, NULL},
		{"message", "description", (int (*) (void *, char *)) copy_e01_description, 0, 0, NULL},
		{"message", "objecttype", (int (*) (void *, char *)) copy_e01_objecttype, 0, 0, NULL},
		{"message", "key0", (int (*) (void *, char *)) copy_e01_key0, 0, 0, NULL},
		{"message", "key1", (int (*) (void *, char *)) copy_e01_key1, 0, 0, NULL},
		{"message", "key2", (int (*) (void *, char *)) copy_e01_key2, 0, 0, NULL},
		{"message", "key3", (int (*) (void *, char *)) copy_e01_key3, 0, 0, NULL},
		{"message", "key4", (int (*) (void *, char *)) copy_e01_key4, 0, 0, NULL},
		{"message/data", "value", (int (*) (void *, char *)) store_ptr_e01_value, 1, 0, NULL},
		
};

int setupE01Funcs () {
	int i = 0;
	int num = sizeof(funcsE01)/sizeof(XMLFUNC);
	for (i = 0; i < num; i++) {
		if (funcsE01[i].ptr_only) {
			funcsE01[i].ptr_passed = 0;
		}
	}
	return 1;
}
int main(int argc, char **argv) {
	EQX_MSG_E01 *msg_e01 = NULL;
	msg_e01 = (EQX_MSG_E01 * )calloc(1, sizeof(EQX_MSG_E01));
	char * holder ;
	int num = sizeof(funcsE01)/sizeof(XMLFUNC);
	do {
		holder = read_file("e01small.xml");
		setupE01Funcs();
		xmlfunc_parsedata(holder, funcsE01, num, (void *) msg_e01);
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
		setupE01Funcs();
		xmlfunc_parsedata(holder, funcsE01, num, (void *) msg_e01);
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
	
	return 0;
}
