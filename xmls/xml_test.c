#include <stdio.h>
#include <string.h>
#include "yxml.h"
#include "xmls.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


#define bool unsigned int
#define true 1
#define false 0
#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"

char *gup_profile_multi_res = "<!-- IMSI1 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val value=\"520011893780810\"></val>\
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

	char *gup_profile_multi_multi= "<!-- IMSI1 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val value=\"520011893780810\"></val>\
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
	</entry>\
<!-- IMSI3 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val value=\"520010991813333\"></val>\
		</attr>\
		<attr name=\"msisdn\">\
			<val value=\"66818010400\"></val>\
		</attr>\
		<attr name=\"mscAddress\">\
			<val value=\"66818010400\"></val>\
		</attr>\
	</entry>\
<!-- IMSI4 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"imsi\">\
			<val jounds=\"520010991813333\"></val>\
		</attr>\
		<attr name=\"msisdn\">\
			<val value=\"66818010400\"></val>\
		</attr>\
		<attr name=\"mscAddress\">\
			<val value=\"66818010400\"></val>\
		</attr>\
	</entry>\
<!-- IMSI5 -->\
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
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
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CDPA/GTITLE", "addr", buff, 1024);
  assertion = assertion && (!strcmp(buff, "66818110001"));
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CGPA/GTITLE", "addr", buff, 1024);
  assertion = assertion && (!strcmp(buff, "66818110002"));
  assertion = assertion && (strcmp(buff, "66818110001"));
  memset(buff, 0, sizeof(buff));
  findet = xml_find_attribute(srism_ok, "SS7AP/TCAP/COMPONENT/DATA", "value", buff, 10);
  assertion = assertion && (!strcmp(buff, "3020040825"));


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
	memset (&x, 0, sizeof(x));
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

int process_attrib_imsi(char *xml_string) {
  char buff[100] = {0};
  if (xml_find_attribute(xml_string, "attr", "name", buff, sizeof(buff)) && !strcmp(buff, "imsi")) {
    buff[0] = 0;
    if (xml_find_content(xml_string, "attr/val", buff, sizeof(buff))) {
      fprintf(stdout, "imsi value = %s\n", buff);
      return 1;
    }
    else {
      fprintf(stdout, "imsi empty\n");
      return 0;
    }
  }
  return 1;
}
int process_attrib(char *xml_string) {
  int num_found = xml_foreach_element(xml_string, "entry/attr", process_attrib_imsi);
  fprintf(stdout, "process_attrib num_found attribs = %d \n", num_found);
  print_assertion(num_found == 3);
  return 1;
}

bool process_multi_elem(char *xml_string) {
  int num_found ;
  num_found = xml_foreach_element(xml_string, "entry", process_attrib);
  fprintf(stdout,  "num_found entries =%d \n", num_found);
  print_assertion(num_found == 5);
  return 1;
}



int open_file_xml(char *fname);
int main (int argc, char **argv) {
    fprintf(stdout, "test_xml_attrib  ");
    print_assertion(test_xml_attrib());
    fprintf(stdout, "test_xml_elem  ");
    print_assertion(test_xml_elem());
    fprintf(stdout, "test_xml_iterator ");
    print_assertion(test_xml_iterator());
    fprintf(stdout, "process_multi_elem\n");
    process_multi_elem(gup_profile_multi_multi);
    open_file_xml("test.xml");
    return 0;
}


#include <errno.h>

int file_size(FILE *fin) {
  int nPos,nFSize;
  nPos=ftell(fin);
  fseek(fin,0L,SEEK_END);
  nFSize=ftell(fin);
  fseek(fin,nPos,SEEK_SET);
  return nFSize;
}

int process_elem_ussd_spam(char *xml_string) {
  char buff[100] = {0};
  xml_find_content(xml_string, "ussdSpamBlacklist", buff, sizeof(buff));
  fprintf(stdout, "%s,", buff);
  return 1;
}

int process_elem_exceptional(char *xml_string) {
  char buff[100] = {0};
  xml_find_content(xml_string, "exceptionalSmscMoList", buff, sizeof(buff));
  fprintf(stdout, "%s,", buff);
  return 1;
}

int process_elem_bypassroam(char *xml_string) {
  char buff[100] = {0};
  xml_find_content(xml_string, "smsByPassRoaming", buff, sizeof(buff));
  fprintf(stdout, "%s,", buff);
  return 1;
}

typedef struct {
	int curr_length;
	int max_size;
  int count;
	char *accums;
} collected_content;

int collect_content(char *xml_string, collected_content *collected) {
  char output[1000] = {0};
  int nlen = 0;
  if (xml_find_content(xml_string, "ussdSpamBlacklist", output, sizeof(output))) {
    if (((nlen = strlen(output)+1) + collected->curr_length ) > collected->max_size){
      
      fprintf(stdout, ">>>>>>\nnum elems = %d\n%s\n>>>>>\n", collected->count, collected->accums);
      return 0;
    }
    else if (collected->curr_length) {
        strcat(collected->accums, ",");
        strcat(collected->accums, output);
        collected->curr_length += nlen+1;
        collected->count++;
    }else {
        strcat(collected->accums, output);
        collected->curr_length += nlen;
        collected->count++;
    }
  }
  return 1;
}

int open_file_xml(char *fname) {
  FILE *fp = fopen(fname, "r");
  ssize_t n_read = 0;
  ssize_t n_buff = 0;
  collected_content collector;
  char string_out [100] = {0};
  memset(&collector, 0, sizeof(collector));
  collector.max_size = sizeof(string_out);
  collector.accums = string_out;
  
  if (!fp) {
    fprintf(stderr, "error opening file %s\n error = %d", fname, errno);
    perror("error: ");
    return 0;
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
    //fprintf(stdout, "holder = %s\n", holder);
    fclose(fp);
    fprintf(stdout, "ussdSpamBlacklist\n");
    xml_foreach_element(holder, "configuration/warm/ussdSpamBlacklist", process_elem_ussd_spam);
    fprintf(stdout, "\n");
    fprintf(stdout, "exceptionalSmscMoList\n");
    xml_foreach_element(holder, "configuration/warm/exceptionalSmscMoList", process_elem_exceptional);
    fprintf(stdout, "\n");
    fprintf(stdout, "smsByPassRoaming\n");
    xml_foreach_element(holder, "configuration/warm/smsByPassRoaming", process_elem_bypassroam);
    fprintf(stdout, "\n");
    xml_collect_elements(holder, "configuration/warm/ussdSpamBlacklist", (int (*) (char *, void *))collect_content, (void *)&collector);
    fprintf(stdout, "\n");
    free(holder);
    return 1;
  }
}
