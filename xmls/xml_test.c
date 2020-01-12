#include <stdio.h>
#include <string.h>
#include "yxml.h"
#include "xmls.h"


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
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CDPA/GTITLE", "addr", buff, 1024);
  assertion = assertion && (!strcmp(buff, "66818110001"));
  findet = xml_find_attribute(srism_ok, "SS7AP/SCCP/CGPA/GTITLE", "addr", buff, 1024);
  assertion = assertion && (!strcmp(buff, "66818110002"));
  assertion = assertion && (strcmp(buff, "66818110001"));


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

int main (int argc, char **argv) {
    fprintf(stdout, "test_xml_attrib  ");
    print_assertion(test_xml_attrib());
    fprintf(stdout, "test_xml_elem  ");
    print_assertion(test_xml_elem());
    fprintf(stdout, "test_xml_iterator ");
    print_assertion(test_xml_iterator());
    return 0;
}
