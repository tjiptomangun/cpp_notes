#include <string.h>
#include <stdlib.h>
#include "parserclass.h"
#include "filecfgparser.h"
#include "stdfileparser.h" 
#define BUFF_SIZE 256

int option ()
{
	fprintf (stdout, "type:\n"
			"get item_name - to set active item\n"
			"print - print properties of active item\n"
			"q - quit application\n"); 
	return 0;
} 
int userparse (FILE *fp, char *tokenlist, PFILECFGPARSER pfilecfg)
{

	char buffer [BUFF_SIZE];
	char buffer_param [BUFF_SIZE];


	int token = -1;
	int prevtoken = -1;
	int length = 0;
	int state = 0; 
	PSTACK_PTR stack = newstackptr();


#define STATE_TAKE 1
#define STATE_PRINT 2
#define STATE_FIND 3
#define STATE_ADD  4
#define STATE_WRITE  5
#define STATE_LIST 6
#define STATE_OTHER 0

	option(); 
	PLIST lcurr = 0; 
	PCONFIGLIST root = pfilecfg->result;
	PCONFIGLIST plist = NULL;
	PL_ITEM pl_icurr  = &root->l_item;
	PL_ITEM pl_temp;
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
				;	
			}
			else if (!strcmp (buffer, "q"))
			{
				fprintf (fp, "danke schon...");
				return 0; 
			}
			else if (!strncmp (buffer, "print", 5))
			{
				state = STATE_PRINT;
				if (pl_icurr)
				{
					pl_icurr->class.printattributes
						(&pl_icurr->class, 0); 
				} 
				else
				{
					fprintf (stdout ,"empty\n"); 
				}
				state = STATE_OTHER; 
			}
			else if (!strncmp (buffer, "delete", 6))
			{
				state = STATE_OTHER;
				if (!pl_icurr)
				{
					fprintf (stdout ,"empty\n");
				}
				else
				{
					pl_icurr->class.delete
						(&pl_icurr->class); 
					pl_icurr = stack->pop (stack);
				}
			}
			else if (!strncmp (buffer, "up", 2))
			{
				state = STATE_OTHER;
				if (!pl_icurr)
				{
					fprintf (stdout ,"empty\n");
				}
				else
				{
					pl_icurr = stack->pop (stack);
				}
			} 
			else if (!strncmp (buffer, "kill", 6))
			{
				state = STATE_OTHER;
				if (!pl_icurr)
				{
					fprintf (stdout ,"empty\n");
				}
				else
				{
					
				}
			}
			else if (!strncmp (buffer, "firstchild", 10))
			{ 
				if (pl_icurr->class.type == CLASS_LIST)
				{
					lcurr = (PLIST) pl_icurr;
					pl_temp = lcurr->getfirstchild (lcurr);
					if (state == STATE_PRINT)
						if (pl_temp)
							pl_temp->class.printattributes 
								(&pl_temp->class, 1);

					
				}
				state = STATE_OTHER;
			}
			else if (!strncmp (buffer, "next", 4))
			{

				if (pl_icurr->class.type == CLASS_LIST)
				{
					lcurr = (PLIST) pl_icurr;
					pl_temp = lcurr->getnextchild (lcurr);
					if (state == STATE_PRINT)
						if (pl_temp)
							pl_temp->class.printattributes 
								(&pl_temp->class, 1);
				}
				state = STATE_OTHER;
			}
			else if (!strncmp (buffer, "write", 5))
			{
				state = STATE_WRITE;
				pfilecfg->class.writetofile (&pfilecfg->class, NULL);
				state = STATE_OTHER;
			}
			else
			{
				switch (state)
				{
				case STATE_TAKE:
					trimstring (buffer);
					if (!strlen(buffer))
						break;
					if (!pl_icurr)
					{
						fprintf (stdout ,"empty\n"); 
					}
					else if (pl_icurr->class.type == CLASS_LIST)
					{
						lcurr = (PLIST) pl_icurr;
						stack->push (stack, pl_icurr);
						pl_icurr = lcurr->takename 
							(lcurr, buffer);
						if  (!pl_icurr )
						{
							fprintf (stdout, 
								"not found\n");
							pl_icurr = stack->
								pop (stack); 
						}
					}
					state = STATE_OTHER;
					break;
				case STATE_FIND:
					trimstring (buffer);
					if (!strlen(buffer))
						break;
					if (!pl_icurr)
					{
						fprintf (stdout ,"empty\n"); 
					}
					else if (pl_icurr->class.type == CLASS_LIST)
					{
						lcurr = (PLIST) pl_icurr;
						stack->push (stack, pl_icurr);
						pl_icurr = lcurr->getname
							(lcurr, buffer);
						if  (!pl_icurr )
						{
							fprintf (stdout, 
								"not found\n");
							pl_icurr = stack->
								pop (stack); 
						}
					}
					state = STATE_OTHER;
					break;
				case STATE_PRINT:
					trimstring (buffer);
					state = STATE_OTHER;
					break;
				case STATE_ADD: 
					trimstring (buffer); 
					if (!pl_icurr)
					{
						fprintf (stdout ,"empty\n"); 
					}
					else if (pl_icurr->class.type == CLASS_LIST)
					{
						lcurr = (PLIST) pl_icurr;
						if (strlen (buffer_param))
						{
							lcurr->addproperty (lcurr, buffer_param, buffer);
						}
						else
						{

							lcurr->addproperty (lcurr, buffer, NULL);
						}
					}
					state = STATE_OTHER;
					break;
				case STATE_LIST:
					trimstring (buffer); 
					if (!pl_icurr)
					{
						fprintf (stdout ,"empty\n"); 
					}
					else if (pl_icurr->class.type == CLASS_LIST)
					{
						lcurr = (PLIST) pl_icurr;
						plist = newconfiglist (buffer);
						lcurr->add(lcurr, &plist->l_item);
					}
					state = STATE_OTHER;
					break;
				default :
					break;
				}
			}
			prevtoken = token; 
			token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE,&length);
			break;
		case ' ':
			if (token >= 0)
			{ 
				if (state == STATE_ADD)
				{//need an extra var to store var=value 
					strncpy(buffer_param, buffer, BUFF_SIZE);	
					prevtoken = token;	
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE, &length);
				}
				else if (!strncmp (buffer, "take", 4))
				{ 
					state = STATE_TAKE; 
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE,&length); 
					break;
				}
				else if (!strncmp (buffer, "find", 4))
				{ 
					state = STATE_FIND; 
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE,&length); 
					break;
				}
				else if (!strncmp (buffer, "print", 5))
				{
					state = STATE_PRINT;
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE, &length);
				}
				else if (!strncmp (buffer, "add", 3))
				{
					state = STATE_ADD;
					prevtoken = token;
					memset (buffer_param, 0, BUFF_SIZE);	
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE, &length);
				}
				else if (!strncmp (buffer, "list", 4))
				{
					state = STATE_LIST;
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, BUFF_SIZE, &length);
				}
				else 
				{ 
					state = STATE_OTHER;
					fprintf (stderr,"[ER] unknow option\n");
					token = stream_gettoken (fp, tokenlist, 
						buffer, BUFF_SIZE,&length); 
					//cleanup();
					break;
				} 
			} 
			break;	
		} 
	}
	return 0;
}


int stdfile_parse (PMINIPARSER parser, void *inout)
{
	PFILECFGPARSER p = (PFILECFGPARSER) inout;
	userparse (parser->input, parser->tokenlist, p); 
	return 0;
}

