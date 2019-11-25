#ifndef PARSECFG_H
#define PARSECFG_H
#define CFG_BUF_LEN     256
#include <parserclass.h>
extern int proc_cfg_file (FILE *fp, PLIST *plist); 
/*
 * Name         : get_cfg_val
 * Returns      
 *      0       : success
 *      -1      : no such item
 */
extern int get_cfg_val (PLIST plist, char *cfg_name, char *buf, int buflen);
#endif
