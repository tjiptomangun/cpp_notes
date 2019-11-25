#include <parsecfg.h>
#include <stdio.h>
#include <string.h>
#define KEY_NL		'\n'
#define	KEY_EQU		'='
#define KEY_HASH	'#'

/*configuration specific items*/
int proc_cfg_file (FILE *fp, PLIST *plist){
	char buffer[CFG_BUF_LEN];
	int token  = -1;
	int  prevtoken = -1; 
	int length = 0;
	PLIST root = newlist ("root"); 
	PLIST lcurr = root;
	PPROPERTY pcurr = NULL; /*current active property */
	PL_ITEM pl_item = NULL;
	char tokenlist [] = "\n=#";
	*plist = root;
	token = stream_gettoken (fp, tokenlist, buffer, 256, &length) ;
	while ( token >= 0)
	{
		prevtoken = token;
		switch ( tokenlist[token] )
		{
		case KEY_NL:
			prevtoken = token;
			token = stream_expectnexttoken (fp, tokenlist, buffer, CFG_BUF_LEN, 
				&length, tokenlist);
			if (tokenlist [token] != KEY_NL) 
				break;
			else if (!length)
				break;
			else if (!strlen(trimstring(buffer)))
				break;
			pl_item = (PL_ITEM) newl_item (buffer);
			lcurr->add (lcurr, pl_item); 
				
			break; 
		case KEY_HASH:/* comment, ignore everything in this line */
			token = stream_expectnexttoken (fp, tokenlist, buffer, CFG_BUF_LEN,
				&length, "\n");
			if (tokenlist[token] != '\n')
			{
				memset (buffer, 0, CFG_BUF_LEN);
				token = prevtoken;
			}
			break; 
		case KEY_EQU:
			trimstring (buffer);
			pcurr = newproperty(buffer);
			token = stream_expectnexttoken (fp, tokenlist, buffer, CFG_BUF_LEN,
				&length,"\n#"); 
			if ((token < 0) || (lcurr == 0))
			{
				pcurr->l_item.class.delete(&pcurr->l_item.class); 
				break;
			} 
			trimstring (buffer);
			pcurr->setvalue(pcurr, buffer);
			lcurr->add (lcurr, &pcurr->l_item); 
			break; 
		default: 
			token = stream_gettoken (fp, tokenlist, buffer, CFG_BUF_LEN, &length) ;
			break;
		}
	}
	return 0;
}
/*
 * Name		: get_cfg_val
 * Returns
 *	0	: success
 *	-1	: no such item
 */
int get_cfg_val (PLIST plist, char *cfg_name, char *buf, int buflen){
	PPROPERTY	pprop = NULL;
	pprop = (PPROPERTY)plist->getname(plist, cfg_name);
	if (!pprop)
		return -1; 
	strncpy (buf , pprop->value, buflen);
	return 0; 
}
