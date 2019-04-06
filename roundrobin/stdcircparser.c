#include <string.h>
#include <stdlib.h>
#include "parserclass.h"
#include "stduserparser.h"
 
int killprocname (char *procname )
{
	char killerpatt [256] = {0};
	sprintf (killerpatt,"pids=`ps ax|sed -e '/ %s /!d;/grep/d;"
		"s/^ *\\([0-9]*\\) *.*/\\1/g'`; for pid in $pids ;"
		"do kill -9 $pid; done;", procname);
	system(killerpatt);
	return 0;	
	
	
}

int help ()
{ 
	fprintf (stdout ,"commands:\n" 
		"  q      : quit\n"
		"  print  : [|detached|all], \n"
		"           zero param will do operation to root's current\n"
		"  delete : [|detached|all], \n"
		"           zero param will do operation to root's current\n"
		"  get    : [.*], detach and item from root circular list\n"
		"           zero param will do operation to root's current\n"
		"           otherwise will search specified item\n"
		"  next   : change root's current to current's next\n"
		"  prev   : change root's curremt to curremt's prev\n"
		"  add    : [.*] , add an item to root\n" 
		"  help   :, this help\n" 
		);
	return 0; 
}

#define STATE_GET 1
#define STATE_PRINT 2
#define STATE_REMOVE 3
#define STATE_ADD 4
#define STATE_OTHER 0
int circparse (FILE *fp, char *tokenlist, PLIST *proot)

	char buffer [256];

	int token = -1;
	int prevtoken = -1;
	int length = 0;
	int state = 0; 
	help ();
	PCIRCULARLIST root = newcircularlist ("circparse");
	PCIRCULARITEM p_icurr  = root->current;
	PCIRCULARITEM p_detached  = 0;
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
			else if (!(strcmp (buffer, "q") && strcmp (buffer, "quit")))
			{
				fprintf (fp, "danke schon...");
				return 0; 
			}
			else if (!strncmp (buffer, "print", 5))
			{
				state = STATE_PRINT;
				p_icurr = root->current;
				if (p_icurr)
				{
					p_icurr->class.printattributes
						(&p_icurr->class, 0); 
				} 
				else
				{
					fprintf (stdout ,"empty\n"); 
				}
				state = STATE_OTHER; 
			}
			else if (!strncmp (buffer, "delete", 6))/*delete current*/
			{
				state = STATE_OTHER;
				p_icurr = root->current;
				if (!p_icurr)
				{
					fprintf (stdout ,"empty\n");
				}
				else
				{
					root->remove(root, p_icurr);
				}
			} 
			else if (!strncmp (buffer, "get", 3))/*get and detach current*/
			{
				state = STATE_OTHER;
				p_detached = root->take (root);

			}
			else if (!strncmp (buffer, "next", 4))/*next item*/
			{
				state = STATE_OTHER; 
					root->current = root->current->next; 
			}
			else if (!(strcmp (buffer, "help")&& strcmp (buffer,"h")))/*next item*/
			{
				help (); 
			}
			else if (!strncmp (buffer, "prev", 4))/*next item*/
			{
				state = STATE_OTHER;
				if (root->current)
					root->current = root->current->prev; 
			} 
			else
			{
				switch (state)
				{
				case STATE_GET:
					trimstring (buffer);
					if (!strlen(buffer))
						break;
					if (!root)
					{
						fprintf (stdout ,"empty\n"); 
					}
					p_detached = root->takename (root, buffer); 
					state = STATE_OTHER;
					break;
				case STATE_REMOVE:
					trimstring (buffer);
					if (!strlen (buffer))
						break;
					if (!strncmp (buffer, "detached", 8))
					{
						if (p_detached)
						{ 
							p_detached->class.delete (&p_detached->class);
							p_detached = NULL;
						}
						else
						{
							fprintf (stderr, "none get yet\n");
						} 
					}
					else if (!strncmp (buffer, "all", 3))
					{
						root->class.delete (&root->class); 
					}
					else
					{
						fprintf (stderr,"not a valid option, delete [|all|detached]");
					}
					break;
				case STATE_PRINT:
					//print detached
					trimstring (buffer);
					if (!strncmp (buffer, "detached", 8))
					{
						if (p_detached)
						{	
							p_detached->class.printattributes
								(&p_detached->class, 0); 
						}
						else 
						{
							fprintf (stderr, "no item get yet\n");
						}
						state = STATE_OTHER;
					}
					else if (!strncmp (buffer, "all", 3))
					{
						if (root)
						{
							root->class.printattributes 
								(&root->class, 0);

						}
						else
						{ 
							fprintf (stderr, "no  root\n");
						}
						state = STATE_OTHER;
					}
					else
					{
						fprintf (stderr,"not a valid option, print [|all|detached]");
					}
					//print all
					break;
				case STATE_ADD:
					trimstring (buffer); 
					root->add (root, newcircularitem(buffer, 0, 0));
					break;
				default :
					break;
				}
			}
			prevtoken = token; 
			token = stream_gettoken (fp, tokenlist, buffer, 256,&length);
			break;
		case ' ':
			if (token >= 0)
			{ 
				if (!strncmp (buffer, "get", 3))
				{ 
					state = STATE_GET; 
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, 256,&length); 
					break;
				}
				else if (!strncmp (buffer, "delete", 6))
				{ 
					state = STATE_REMOVE; 
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, 256,&length); 
					break;
				}
				else if (!strncmp (buffer, "add", 3))
				{
					state = STATE_ADD;
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, 256, &length);
				}
				else if (!strncmp (buffer, "print", 5))
				{
					state = STATE_PRINT;
					prevtoken = token;
					token = stream_gettoken (fp, tokenlist, buffer, 256, &length);
				}
				else
				{ 
					state = STATE_OTHER;
					fprintf (stderr,"[ER] unknow option\n");
					token = stream_gettoken (fp, tokenlist, 
						buffer, 256,&length); 
					//cleanup();
					break;
				}


			} 
			break;	
		} 
	}
	return 0;
	
}


int stdin_circparse(PMINIPARSER parser, PLIST *plist)
{
	circparse (parser->input, parser->tokenlist, plist);
	return 0;
}

int stdin_circparse2(PMINIPARSER parser, PLIST  *plist)
{
	
	circparse (parser->input, " \n", plist);
	return 0;
}
