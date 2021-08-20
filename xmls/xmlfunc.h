#ifndef _XML_FUNC_
#define _XML_FUNC_
typedef struct xmlfunc{
	char path[500];
	char attribute[100];
	int (*update_value) (void *, char*);
	int ptr_only;
	int ptr_passed;
	int (*update_content) (void *, char*);
	
}XMLFUNC, *PXMLFUNC;
int xmlfunc_parsedata(char *xml_string, PXMLFUNC fn_data, int xmlfunc_max, void * dest);
#endif
