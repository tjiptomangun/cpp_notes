#include <stdio.h>
#include <string.h>
#include "yxml.h"

/**
 * NAME         : tools_find_xml_attribute
 * DESCRIPTION  : get an attribute's value of xml path from an xml string.
 * INPUT
 *  xml_string  : xml string to find element from
 *  path to find: the path to search for.
 *                example  "SS7AP / SCCP / CDPA / GTITLE" will look
 *                for GTITLE within CDPA within SCCP within SS7AP
 *  attribute   : name of the attributes get value from within path_to_find.
 *                if null then will return pointer to element and no value copied.
 * 
 *  out_buf_size: size of output buffer to store attribute value
 * OUTPUT
 *  outbuf      : value to copy the attribute value to
 * 
 * RETURNS
 *   NULL    : path/attribute does not is_exists
 *   others  : pointer to the attribute
 * 
 */
char* xml_find_attribute(char *xml_string, char *path, char *attr_name, char *out_buffer, int outbuffer_max) { 
  char *p_path = path;
  char *start_path, *end_path ; 
  int done_path = 0;

  int xml_depth = 0;
  int path_depth = 0;

  char *doc_p = xml_string; 
  
  yxml_t xml_elem;
  yxml_ret_t xml_ret;
  char buf[2048];
  yxml_init(&xml_elem, buf, 2048);
  int match = 0;
  int i = 0;
  int found = 0;
  int ret_len = -1;
  char *out_ptr = NULL;
  char *elem_ptr = NULL;
  char *attrib_ptr = NULL;

  while (*p_path != 0 && !done_path) { 

    while(*p_path == ' '){
      p_path ++;
    }

    start_path = p_path;
    while (*p_path != 0 && *p_path != '/' && *p_path != ' '){
      p_path ++;
    }
  
    /* empty path or end of string */
    if (start_path == p_path)
      done_path = 1; 
   
    end_path = p_path; 
  
    while (*p_path == ' ')
      p_path ++;
    
    if (*p_path == '\0')
      done_path = 1;

    if (*p_path == '/')
      p_path ++;

    path_depth ++; 
    
    // find the elem    
    found = 0;
    out_ptr = NULL;
    for(; !found && *doc_p; doc_p++) {
        
        xml_ret = yxml_parse(&xml_elem, *doc_p);
        switch (xml_ret) {
          case YXML_ELEMSTART:
            xml_depth ++;
            int elem_len = strlen(xml_elem.elem);
            int path_len = end_path - start_path;
            match = elem_len == path_len;
            for(i = 0; i < path_len && match; i++)
                match = match && (start_path[i] == xml_elem.elem[i]);
                
            if (match && xml_depth == path_depth){
              found = 1;
              doc_p -= path_len;
              for(;*doc_p != '<'; doc_p--){
                      ;
              }
              elem_ptr = doc_p;
            }
            break;
          case YXML_ELEMEND:
            xml_depth --;
            break;
            
          default:
            break;
        }
    }
    if (found && done_path){
      if (attr_name != NULL){
        int attr_found = 0;
        for(; ret_len < 0 && *doc_p != '\0'; doc_p++){
            xml_ret = yxml_parse(&xml_elem, *doc_p);
            switch (xml_ret) {
            case YXML_ATTRSTART:
                if (!strcmp(attr_name, xml_elem.attr)) {
                    attr_found = 1;
                    attrib_ptr = doc_p - strlen(xml_elem.attr);
                }
                else{
                    attr_found = 0;
                }
                break;
            case YXML_ATTRVAL:
                if (attr_found) {
                  strncpy(out_buffer, xml_elem.data, outbuffer_max);
                  ret_len = strlen(out_buffer);
                  out_ptr = attrib_ptr;
                }
                break;
                
            default:
                break;
            }
                
        }//for
      }//if
      else {
       out_ptr = elem_ptr;
       for (;out_ptr != NULL && *out_ptr != 0 && *out_ptr != '<'; out_ptr --)
            ;
      }
    }//if
  }//!done_path
  return out_ptr;
}

/**
 * NAME         : tools_find_xml_element
 * DESCRIPTION  : find element of a an xml string. Returns to pointer of this element
 * INPUT
 *  xml_string  : xml string to find element from
 *  path to find: the path to search for.
 *                example  "SS7AP / SCCP / CDPA / GTITLE" will look
 *                for GTITLE within CDPA within SCCP within SS7AP
 * RETURNS
 *      NULL    : path does not is_exists
 *      others  : pointer to element
 * 
 */
char *xml_find_element(char *xml_string, char *path) {
  return xml_find_attribute(xml_string, path, NULL, NULL, 0);
}

/**
 * collection of pointers to xml element of same name
 */
typedef struct xml_elem_iterator {
  int num; // number of pointer
  char *ptrs[20]; //the pointer collection of start of element
  char *src_str; // source xml string
}XML_ELEM_ITERATOR;

/**
 * NAME          : to_xml_iterator
 * DESCRIPTION   : create element iterator from an xml string
 * INPUT
 * 	xml_str      : string to find iterator from
 *  elem_name    : name of element to iterate 
 *  iter         : iterator buffer
 *
 * RETURNS
 *  NULL		 : none exists
 *  iter		 : found some
 */
XML_ELEM_ITERATOR *to_xml_iterator(char *xml_str, char *elem_name, XML_ELEM_ITERATOR *iter) {
	char *tmp;
	char *xptr;
	int len = strlen(elem_name);	
	XML_ELEM_ITERATOR *ret = NULL;
	if (iter == NULL)
		return NULL;
	else if (xml_str == NULL)
		return NULL;
	else if (elem_name == NULL)
		return NULL; 
	else if ((tmp = xml_find_element(xml_str, elem_name)) == NULL)
		return NULL;
	else {
		xptr = &elem_name[len];
		do {
			xptr --;
		}while (*xptr != 0 && *xptr != '/');
		xptr ++;
		iter->src_str = xml_str;
		char *doc_p;
		yxml_t xml_elem;
		yxml_ret_t xml_ret;
		char buf[2048];
		int xml_depth = 0;

		do{	
			yxml_init(&xml_elem, buf, 2048);
			iter->num += 1;
			iter->ptrs[iter->num - 1] = tmp;
			doc_p = tmp; 
			for (; xml_depth == 0; doc_p ++){
				xml_ret = yxml_parse(&xml_elem, *doc_p);
				if (xml_ret == YXML_ELEMSTART)
					xml_depth ++; 
			}
			do {
				xml_ret = yxml_parse(&xml_elem, *doc_p);
				switch(xml_ret) {
					case YXML_ELEMSTART:
						xml_depth ++;
						break;
					case YXML_ELEMEND:
						xml_depth --;
						break;
					default:
						break;
				}
				doc_p ++;
			}while (xml_depth > 0);
		}while ((tmp = xml_find_element(doc_p, xptr)) != NULL);

		ret =  iter; 
	}

	return ret;
	 
}

#define bool unsigned int
#define true 1
#define false 0
#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"

char *gup_profile_multi_res = "<!-- IMSI1 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val value=\"520011893780810|\"></val>\
		</attr>\
		<attr name=\"msisdn\">\
			<val value=\"66818010400\"></val>\
		</attr>\
		<attr name=\"mscAddress\">\
			<val value=\"66818010400\"></val>\
		</attr>\
	</entry>\
<!-- IMSI2 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val value=\"520010991818108\"></val>\
		</attr>\
		<attr name=\"msisdn\">\
			<val value=\"66818010400\"></val>\
		</attr>\
		<attr name=\"mscAddress\">\
			<val value=\"66818010400\"></val>\
		</attr>\
	</entry>";

char *srism_ok = "\
		<SS7AP ret=\"0\" type=\"response\" session=\"66818110001:12345678:OSS7.ESS.0.0\">\
			<SCCP>\
				<CDPA routing=\"gt\">\
					<GTITLE addr=\"66818110001\" es=\"1\" np=\"1\" tt=\"0\" nai=\"4\" ssn=\"8\" spc=\"1\" />\
				</CDPA>\
				<CGPA routing=\"gt\">\
					<GTITLE addr=\"66818110002\" es=\"1\" np=\"1\" tt=\"0\" nai=\"4\" ssn=\"6\" spc=\"0\" />\
				</CGPA>\
			</SCCP>\
			<TCAP dialogueid=\"1\" dtid=\"12345678\" type=\"end\">\
				<DIALOGUE oid=\"0.0.17.773.1.1.1\">\
					<AARE result=\"accepted\" acn=\"0.4.0.0.1.0.20.3\" resultsourcediagnostic=\"null\" source=\"user\">\
					</AARE>\
				</DIALOGUE>\
				<COMPONENT>\
					<DATA opcode=\"45\" invokeid=\"0\"\
						value=\"3020040825000302848341F7A0148107916629031121F1A6098107916629031122F1\"\
						type=\"rresult\" />\
				</COMPONENT>\
			</TCAP>\
		</SS7AP>\
	";
	

bool test_xml_elem() {
    
    bool assertion = true;
    char *findet ;
    findet = xml_find_element( srism_ok, "SS7AP");
    assertion = assertion && findet != NULL;
    findet = xml_find_element(srism_ok, "SS7AX");    
    assertion = assertion && findet == NULL;
    findet = xml_find_element(srism_ok, "SS7AP/SCCX");
    assertion = assertion && findet == NULL;    
    findet = xml_find_element(srism_ok, "SS7AP/SCCP/CDPA");
    assertion = assertion && findet != NULL;
    findet = xml_find_element(srism_ok, "SS7AP/SCCP/CDPA/GTX");
    assertion = assertion && findet == NULL;
    findet = xml_find_element(srism_ok, "SS7AP/SCCP/CDPA/GTITLE");
    assertion = assertion && findet != NULL;
    findet = xml_find_element(srism_ok, " SS7AP/SCCP/CDPA/GTITLE");
    assertion = assertion && findet != NULL;
    findet = xml_find_element(srism_ok, " SS7AP/SCCP/CDPA/GTIT");
    assertion = assertion && findet == NULL;
    findet = xml_find_element(srism_ok, " SS7AP/SCCP/CDPA/ GTITLE ");
    assertion = assertion && findet != NULL;
    findet = xml_find_element(srism_ok, " SS7AP/SCCP/ CDPA / GTITLE ");
    assertion = assertion && findet != NULL;
    
    return assertion;
}

bool test_xml_attrib() {
  bool assertion = true;
  char *findet ;
  char buff[1024];  
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CDPA/GTITLE", "ret", buff, 1024);
  assertion = assertion && (findet == NULL);
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CDPA/GTITLE", "nai", buff, 1024);
  assertion = assertion && (findet != NULL);
  assertion = assertion && (!strcmp(buff, "4"));

  return assertion; 
  
}

bool test_xml_iterator() {
  	bool assertion = true;
	XML_ELEM_ITERATOR t, p, x ;
	memset (&t, 0, sizeof(t));
	to_xml_iterator(gup_profile_multi_res, "entry", &t);
	assertion = assertion && t.num == 2;
	memset (&p, 0, sizeof(p));
	to_xml_iterator(t.ptrs[0], "entry/attr", &p);
	assertion = assertion && p.num == 3;
	to_xml_iterator(t.ptrs[1], "entry/attr", &x);
	assertion = assertion && p.num == 3;
	return assertion;
}


void print_assertion(bool assertion) {
  if (assertion){
    printf("%sOk", KBLU);
  }
  else{
    printf("%sfailed", KRED);
  }
    printf("%s\n", KNRM);
}

int main (int argc, char **argv) {
    fprintf(stdout, "test_xml_attrib  ");
    print_assertion(test_xml_attrib());
    fprintf(stdout, "test_xml_elem  ");
    print_assertion(test_xml_elem());
    fprintf(stdout, "test_xml_iterator ");
    print_assertion(test_xml_iterator());
    return 0;
}
