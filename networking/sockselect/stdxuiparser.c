#include <string.h>
#include <stdlib.h>
#include "parserclass.h"
#include "stdxuiparser.h" 


int option ()
{
	fprintf (stdout, "type:\n"
			"send send something - to set active item\n"
			"print - print properties of active item\n"
			"q - quit application\n"); 
	return 0;
}
int killprocname (char *procname )
{
	char killerpatt [256] = {0};
	sprintf (killerpatt,"pids=`ps ax|sed -e '/ %s /!d;/grep/d;"
		"s/^ *\\([0-9]*\\) *.*/\\1/g'`; for pid in $pids ;"
		"do kill -9 $pid; done;", procname);
	system(killerpatt);
	return 0;	
	
	
}
int userparse (FILE *fp, char *tokenlist, PLIST *proot)
{
	char buffer [256];

	int token = -1;
	int prevtoken = -1;
	int length = 0;
	int state = 0;
	PLIST root = *proot;
	


#define STATE_SEND 1
#define STATE_PRINT 2 
#define STATE_OTHER 3 

	option(); 
	token = stream_gettoken (fp, tokenlist, buffer, 256,&length);
	state = STATE_OTHER; 
	while (token !=-1)
	{
		switch (tokenlist[token])
		{
		case '\n':
			trimstring (buffer);
			if (!strlen(buffer))
			{
				prevtoken = token; 
				token = stream_gettoken (fp, tokenlist, buffer, 256,&length); 
				continue;
			}
			else if (!strcmp (buffer, "q"))
			{
				fprintf (stdout, "danke schon...\n");
				return 0; 
			} 
			if(state == STATE_SEND)
			{ 
				trimstring (buffer);
				fprintf (stdout, "sending ...\n");
				root->add (root,&(newproperty_value ("send", buffer))->l_item); 
				state = STATE_OTHER; 
			} 
			prevtoken = token; 
			token = stream_gettoken (fp, tokenlist, buffer, 256,&length); 
			break;
		case ' ':
			if (token < 0)
				break;
			if (strncmp (buffer, "send", 4))
			{ 
				state = STATE_OTHER;
				fprintf (stderr,"[ER] unknow option\n");
				token = stream_gettoken (fp, tokenlist, 
					buffer, 256,&length); 
				//cleanup();
				continue;
			}
			state = STATE_SEND; 
			prevtoken = token;
			token = stream_gettoken (fp, tokenlist, buffer, 256,&length); 
			break; 
		} 
	}
	return 0;
	
}


int stdin_parse (PMINIPARSER parser, PLIST *plist)
{
	userparse (parser->input, parser->tokenlist, plist);
	return 0;
}

