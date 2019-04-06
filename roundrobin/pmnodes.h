#ifndef _PMNODES_H_
#define _PMNODES_H_
#include "parserclass.h"
#include "filecfgparser.h"

typedef struct procmgr
{
	char remote_host[256];
	

}PROCMGR, *PPROCMGR;

typedef struct nodeinfo
{
	char node_name[256];
	
}NODEFINFO, *PNODEINFO;



#endif


