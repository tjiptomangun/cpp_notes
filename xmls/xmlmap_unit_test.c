#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include "parserclass.h"
#include "xmlmap.h"

#define bool unsigned int
#define true 1
#define false 0
#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define KNRM  "\x1B[0m"

char *gup_profile_single = "<!-- IMSI1 -->\
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
	</entry>";

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

char *gup_profile_multi_multi_nc= "<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
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
	<entry dn=\"subdata=profile,ds=gup,subdata=services,msisdn=66818010400,dc=MSISDN,dc=C-NTDB\">\
		<attr name=\"msisdn\">\
			<val value=\"66818010400\"></val>\
		</attr>\
		<attr name=\"mscAddress\">\
			<val value=\"66818010400\"></val>\
		</attr>\
	</entry>";
char * gup_profile_multi_multi_simp = "<entry></entry><entry></entry>";
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

bool test_xml_elem(char *instring) {
		PPRIMTREE_ITEM curr = newprimtreeitem();
		xmls_map_unmarshall(instring, curr);
		
		bool assertion = true;
		PPRIMTREE_ITEM findet ;
		findet = xmlmap_find_element(curr, "SS7AP");
		assertion = assertion && findet != NULL;
		findet = xmlmap_find_element(curr, "SS7AX");    
		assertion = assertion && findet == NULL;
		findet = xmlmap_find_element(curr, "SS7AP/SCCX");
		assertion = assertion && findet == NULL;    
		findet = xmlmap_find_element(curr, "SS7AP/SCCP/CDPA");
		assertion = assertion && findet != NULL;
		findet = xmlmap_find_element(curr, "SS7AP/SCCP/CDPA/GTX");
		assertion = assertion && findet == NULL;
		findet = xmlmap_find_element(curr, "SS7AP/SCCP/CDPA/GTITLE");
		assertion = assertion && findet != NULL;
		findet = xmlmap_find_element(curr, " SS7AP/SCCP/CDPA/GTITLE");
		assertion = assertion && findet != NULL;
		findet = xmlmap_find_element(curr, " SS7AP/SCCP/CDPA/GTIT");
		assertion = assertion && findet == NULL;
		findet = xmlmap_find_element(curr, " SS7AP/SCCP/CDPA/ GTITLE ");
		assertion = assertion && findet != NULL;
		findet = xmlmap_find_element(curr, " SS7AP/SCCP/ CDPA / GTITLE ");
		assertion = assertion && findet != NULL;
		curr->delete(curr);
		
		return assertion;
}

bool test_xml_attrib(char *instring) {
	bool assertion = true;
	map_struct* findet ;
	char buff[1024];
	PPRIMTREE_ITEM curr = newprimtreeitem();
	xmls_map_unmarshall(instring, curr);
	findet = xmlmap_get_attribute(curr, "SS7AP/SCCP/CDPA/GTITLE", "ret");
	assertion = assertion && (findet == NULL);
	findet = xmlmap_get_attribute(curr, "SS7AP/SCCP/CDPA/GTITLE", "nai");
	assertion = assertion && (findet != NULL);
	assertion = assertion && (!strcmp(findet->value, "4"));
	findet = xmlmap_get_attribute(curr, "SS7AP/SCCP/CDPA/GTITLE", "addr");
	assertion = assertion && (!strcmp(findet->value, "66818110001"));
	findet = xmlmap_get_attribute(curr, "SS7AP/SCCP/CGPA/GTITLE", "addr");
	assertion = assertion && (!strcmp(findet->value, "66818110002"));
	assertion = assertion && (strcmp(findet->value, "66818110001"));
	memset(buff, 0, sizeof(buff));
	findet = xmlmap_get_attribute(curr, "SS7AP/TCAP/COMPONENT/DATA", "value");
	assertion = assertion && (strstr(findet->value, "3020040825") == findet->value);

	curr->delete(curr);
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

/*
bool process_multi_elem_modified(char *instring, char *path, int numExpected) {
	bool assertion = true;
	char *modstring = (char *) calloc(1, strlen(instring) + 20);
	char *modpath = (char *) calloc(1, strlen(path) + 10);
	sprintf(modstring, "<main>%s</main>", instring);
	sprintf(modpath, "main/%s", path);
	XML_MAP_ELEM_ITERATOR iter;
	memset(&iter, 0, sizeof(XML_MAP_ELEM_ITERATOR ));
	PPRIMTREE_ITEM curr = newprimtreeitem();

	xmls_map_unmarshall(modstring, curr);
	free(modstring);

	assertion = assertion && (to_xml_map_iterator(curr, modpath, &iter) != NULL);
	assertion = assertion && iter.num == numExpected;
	free(modpath);
	curr->delete(curr);
	return assertion;
}
*/

bool process_multi_elem(char *instring, char *path, int numExpected) {
	bool assertion = true;
	XML_MAP_ELEM_ITERATOR iter;
	memset(&iter, 0, sizeof(XML_MAP_ELEM_ITERATOR ));
	PPRIMTREE_ITEM curr = newprimtreeitem();
	xmls_map_unmarshal_multiroot (instring, curr);  
	assertion = assertion && (to_xml_map_iterator(curr, path, &iter) != NULL);
	assertion = assertion && iter.num == numExpected;
	curr->delete(curr);
	return assertion;
}

bool process_multi_elem_inside() {
	bool assertion = true;
	char *instring = gup_profile_multi_multi;
	XML_MAP_ELEM_ITERATOR iter;
	XML_MAP_ELEM_ITERATOR innerIter;
	char name[100] = {0};
	char imsi[100] = {0};
	char msisdn[100] = {0};
	char mscAddress[100] = {0};
	typedef struct {
		int imsi;
		int msisdn;
		int mscAddress;
	} gupReq;
	gupReq gupReqs[20];
	PPRIMTREE_ITEM findet ;
	memset (gupReqs, 0, sizeof(gupReqs));
	memset(&iter, 0, sizeof(XML_MAP_ELEM_ITERATOR));
	PPRIMTREE_ITEM root = newprimtreeitem();
	xmls_map_unmarshal_multiroot(instring, root);
	assertion = assertion && (to_xml_map_iterator(root, "entry", &iter) != NULL);
	for(int i = 0; i < iter.num; i++) {
		imsi[0] = 0;
		msisdn[0] = 0;
		mscAddress[0] = 0;
		memset(&innerIter, 0, sizeof(XML_MAP_ELEM_ITERATOR));
		assertion = assertion && (to_xml_map_iterator(iter.ptrs[i], "attr", &innerIter) != NULL);
		for (int j = 0; j < innerIter.num; j++) {
			findet = innerIter.ptrs[j];
			if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "imsi")) {
				if (xmlmap_get_attribute_string(findet, "val", "value", imsi, 100)) {
					gupReqs[i].imsi = 1;
				}
			}
			else if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "msisdn")) {
				if (xmlmap_get_attribute_string(findet, "val", "value", msisdn, 100)) {
					gupReqs[i].msisdn = 1;
				}
			}
			else if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "mscAddress")) {
				if (xmlmap_get_attribute_string(findet, "val", "value", mscAddress, 100)) {
					gupReqs[i].mscAddress = 1;
				}
			}
		}
		
	}

	assertion = assertion && gupReqs[0].imsi == 1;
	assertion = assertion && gupReqs[0].msisdn == 1;
	assertion = assertion && gupReqs[0].mscAddress == 1;
	assertion = assertion && gupReqs[3].imsi == 0;
	assertion = assertion && gupReqs[3].msisdn == 1;
	assertion = assertion && gupReqs[3].mscAddress == 1;
	assertion = assertion && gupReqs[4].imsi == 0;
	assertion = assertion && gupReqs[4].msisdn == 1;
	assertion = assertion && gupReqs[4].mscAddress == 1;

	root->delete(root);
	return assertion;
}

bool process_single_elem_inside() {
	bool assertion = true;
	char *instring = gup_profile_single;
	XML_MAP_ELEM_ITERATOR iter;
	char name[100] = {0};
	char imsi[100] = {0};
	char msisdn[100] = {0};
	char mscAddress[100] = {0};
	typedef struct {
		int imsi;
		int msisdn;
		int mscAddress;
	} gupReq;
	gupReq xGupReq;
	PPRIMTREE_ITEM findet ;
	memset (&xGupReq, 0, sizeof(xGupReq));
	memset(&iter, 0, sizeof(XML_MAP_ELEM_ITERATOR));
	PPRIMTREE_ITEM root = newprimtreeitem();
	xmls_map_unmarshal_multiroot(instring, root);
	assertion = assertion && (to_xml_map_iterator(root, "entry/attr", &iter) != NULL);
	for(int i = 0; i < iter.num; i++) {
		imsi[0] = 0;
		msisdn[0] = 0;
		mscAddress[0] = 0;
		findet = iter.ptrs[i];
		if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "imsi")) {
			if (xmlmap_get_attribute_string(findet, "val", "value", imsi, 100)) {
				xGupReq.imsi = 1;
			}
		}
		else if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "msisdn")) {
			if (xmlmap_get_attribute_string(findet, "val", "value", msisdn, 100)) {
				xGupReq.msisdn = 1;
			}
		}
		else if (xmlmap_get_attribute_string(findet, "", "name", name, 100) && !strcmp(name, "mscAddress")) {
			if (xmlmap_get_attribute_string(findet, "val", "value", mscAddress, 100)) {
				xGupReq.mscAddress = 1;
			}
		}
		
	}

	assertion = assertion && xGupReq.imsi == 1;
	assertion = assertion && xGupReq.msisdn == 1;
	assertion = assertion && xGupReq.mscAddress == 1;

	root->delete(root);
	return assertion;
}

void usage(char *app) {
	fprintf(stdout, "%s [-(a)ll case| -(h)elp -(m)emleak test]\n", app);
}
int main (int argc, char **argv) {
		int option_index = 0;
		int opt;
		int optlong;
		static struct option long_options[]	 = {
		{"all", no_argument, 0, 'a'},
		{"help", no_argument, 0, 'h'},
		{"memleak", optional_argument, 0, 'm'},
		{0, 0, 0, 0}
	};
		while ((opt = getopt_long(argc, argv, "ahm", long_options,&option_index))!= -1) {
		switch (opt) {
			case 'h':
				usage(argv[0]);
				exit(0);
				break;
			case 'a':
        optlong = opt;
        break;
			case 'm':
				optlong = opt;
				break;
			
		}
		
	}
	do {
		fprintf(stdout, "test_xml_attrib  ");
		print_assertion(test_xml_attrib(srism_ok));
		fprintf(stdout, "test_xml_elem  ");
		print_assertion(test_xml_elem(srism_ok));
		fprintf(stdout, "process_multi_elem (5) ");
		print_assertion(process_multi_elem(gup_profile_multi_multi, "entry", 5));
		fprintf(stdout, "process_multi_elem (2) ");
		print_assertion(process_multi_elem(gup_profile_multi_res, "entry", 2));
		fprintf(stdout, "process_single_elem ");
		print_assertion(process_multi_elem(gup_profile_single, "entry", 1));
		fprintf(stdout, "process_multi (simple) (2) ");
		print_assertion(process_multi_elem(gup_profile_multi_multi_simp, "entry", 2));
		fprintf(stdout, "process_multi_elem_inside");
		print_assertion(process_multi_elem_inside());
		fprintf(stdout, "process_single_elem_inside");
		print_assertion(process_single_elem_inside());
		
	} while (optlong == 'm');

}

