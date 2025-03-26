#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

int hex_to_int(char c){
        int first = c / 16 - 3;
        int second = c % 16;
        int result = first*10 + second;
        if(result > 9) result--;
        return result;
}

unsigned char hex_to_uchar(char c, char d){
        int high = hex_to_int(c) * 16;
        int low = hex_to_int(d);
        return (unsigned char) high+low;
}
void usage(char *app){
    fprintf (stdout, "usage : %s a:hg:\n\
                      %s -h\n\
                     -a -- ai  input: siug3 address indicator(hex)\n\
                     -g -- gt  input: siug3 global table(hex)\n"
                     , app, app);
	
}

int main(int argc , char * argv[]){
	int option_index = 0;
    static struct option long_options[]  = {
        {"ai", no_argument, 0, 'a'},
        {"help", no_argument, 0, 'h'},
        {"gt", no_argument, 0, 'g'},
        {0, 0, 0, 0}
    };
	char addr_indicator[20] = {0x00};
	char global_tran[80] = {0x00};
	char gtai_patt[40] = {0x00};
	char gtai_repl[40] = {0x00};
	char ssn[10] = {0x00};
	char pc[10] = {0x00};
	unsigned int nAddrI = 0;
	unsigned long nGt = 0;
	unsigned int nSSN = 0;
	unsigned int nPC = 0;
#define OUTPUT_TYPE_ADDRESS 1
#define OUTPUT_TYPE_PATTERN 2
#define OUTPUT_TYPE_GTT 3
	unsigned int nOT=  OUTPUT_TYPE_ADDRESS;
	unsigned int nID=  0;
	int opt;
	char *ptr;
	char mask[20] = {0};
	char *gtai;
    while ((opt = getopt_long(argc, argv, "jdta:hg:s:p:i:b::", long_options,&option_index))!= -1) {
        switch (opt) {
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case 'a':
				strcpy(addr_indicator, optarg); 
				break; 
            case 's':
				strcpy(ssn, optarg); 
				break; 
            case 'g':
				strcpy(global_tran, optarg); 
                break;
            case 'p':
				strcpy(pc, optarg); 
                break;
            case 'd':
				nOT = OUTPUT_TYPE_ADDRESS;
                break;
            case 't':
				nOT = OUTPUT_TYPE_PATTERN;
                break;
            case 'j':
				nOT = OUTPUT_TYPE_GTT;
                break;
            case 'i':
				nID = (unsigned int)strtoul(optarg, &ptr, 10);
                break;
            case 'b':
				if (nOT == OUTPUT_TYPE_PATTERN) { 
					strcpy(gtai_patt, optarg); 
				} else if (nOT == OUTPUT_TYPE_ADDRESS){
					strcpy(gtai_repl, optarg); 
				}
				break; 

        }

    }
	sscanf(global_tran, "%lx", &nGt);
	if (nOT != OUTPUT_TYPE_GTT) {//Mask
		sscanf(addr_indicator, "%8x", &nAddrI);
		
	} else {
		strcpy(mask, addr_indicator);
	}
    FILE *fp ;
	/*
      not gtt need to store repl/patt in file
      otherwise get gtt repl/patt from file
     */
	if (nOT == OUTPUT_TYPE_ADDRESS)  {
		fp = fopen("gtai_repl", "w");
		gtai = gtai_repl;
	}
	else if (nOT == OUTPUT_TYPE_PATTERN) {
		fp = fopen("gtai_patt", "w");
		gtai = gtai_patt;
	} else {
		fp = fopen("gtai_repl", "r");
		fread(gtai_repl, 1, sizeof(gtai_repl), fp);
		fclose(fp);
		fp = fopen("gtai_patt", "r");
		fread(gtai_patt, 1, sizeof(gtai_patt), fp);
		fclose(fp);
	}
	if (nOT != OUTPUT_TYPE_GTT) {
		fprintf(fp, gtai);
	}
	sscanf(ssn, "%8x", &nSSN);
	sscanf(pc, "%8x", &nPC);
	/*
	fprintf(stdout, "GT: %016lx\n", nGt);
	fprintf(stdout, "ai: %08x\n", nAddrI);
	*/
	unsigned int nib0 = nAddrI & 0x000F;
	unsigned int nib1 = nAddrI & 0x00F0;
	unsigned int PCI = nib0 & 0x01;
	unsigned int SSNI= (nib0 & 0x02) >> 1;
	unsigned int GTI= ((nAddrI & 0x00FF)>>2) & 0x0F;
	unsigned int RI = (nib1>>6) & 0x01;
	unsigned int TT = (nGt & 0x00FF0000) >>16;
	unsigned int NP = (nGt & 0xF000)>>12;
	unsigned int ES = (nGt & 0x0F00)>>8;
	unsigned int NAI= (nGt & 0x7F);
	/*
	fprintf(stdout, "PCI = %x\n", PCI);
	fprintf(stdout, "SSNI = %x\n", SSNI);
	fprintf(stdout, "GTI = %u\n", GTI);
	fprintf(stdout, "RI = %x\n", RI);
	fprintf(stdout, "SSN = %u\n", nSSN);
	fprintf(stdout, "TT= %d\n", TT);
	fprintf(stdout, "NP= %d\n", NP);
	fprintf(stdout, "ES= %d\n", ES);
	fprintf(stdout, "NAI= %d\n", NAI);
	fprintf(stdout, "PC= %d\n", nPC); 
	*/
	if (nOT == OUTPUT_TYPE_ADDRESS) { 
		fprintf(stdout, "SCCP_GTT_ADDRESS:GTAID=%d,NC=NC0,SPC=%d,RI=%d,",nID, nPC, RI);
	} else if(nOT == OUTPUT_TYPE_PATTERN){
		fprintf(stdout, "SCCP_GTT_PATTERN:GTPID=%d,NC=NC0,", nID);
	}
	else {
		fprintf(stdout, "SCCP_GTT:GTTID=0,NC=NC0,");
	}
	if (nOT != OUTPUT_TYPE_GTT) {
		fprintf(stdout,"GTI=%d,SSN=%d,TT=%d,NP=%d,NAI=%d", GTI, nSSN, TT, NP, NAI);
	} else {
		fprintf(stdout,"GTPID=%d,GTT_MASK=%s,PRI_GTAID=%s,SEC_GTAID=%s,DIGITS=%s,REPLACE=%s", nID, mask, pc,nSSN == 0 ? "0": ssn, gtai_patt, gtai_repl);
	}
	fprintf(stdout, "\n");

	
	
}
