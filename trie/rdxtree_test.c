#include <stdio.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "parserclass.h"

#include "rdxtree.h"


int option ()
{
	fprintf (stdout, "type:\n"
			"add key - add a key\n"
			"print - print properties of active item\n"
			"del - delete a key\n"
			"gpref - check if string has prefix in list\n"
			"gkey - check if string has key in list\n"
			"q - quit application\n"); 
	return 0;
}

void rdxtree_print_reverse(PRDXTREE_FIND_CONTEXT ctx) {
	if (ctx) {
		ctx->reverse_once(ctx);
	}
	PRDXTREE_CONTEXT_ITEM curr = ctx->head;
	while (curr) {
		fprintf(stdout, " %s ", curr->item->name);
		fprintf(stdout, " <- ");
		curr = curr->next;
	}
	
}

void rdxtree_find_child_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding name %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findkey(root, to_find, ctx);
	if(curr){
		rdxtree_print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
}

void rdxtree_find_prefix_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding prefix %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findprefix(root, to_find, ctx);
	if(curr){
		rdxtree_print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
}

#define MAX_PROCESSED_DATA 3000

int stdparse (FILE *fp, PRDXTREE_ITEM pTree)
{
	char tokenlist[] = {" \n"};
	char buffer[MAX_PROCESSED_DATA] = {0};
	int token = -1;
	int length = 0;

	int state = 0;
	unsigned char * newvar;
	PSTACK_PTR stack = newstackptr(); 
	int numvar = 0;
#define STATE_OTHER	0x00
#define STATE_ADD 	0x01
#define STATE_DELETE	0x02
#define STATE_GET_PREFIX	0x03
#define STATE_GET_KEY	0x04
	//option();
	token = stream_gettoken (fp, tokenlist, buffer, MAX_PROCESSED_DATA, &length);
	state = STATE_OTHER;
	while (token != -1)
	{
		switch (tokenlist[token])
		{
		case ' ':
		case '\n':
			trimstring (buffer);
			if (!strcmp (buffer, "q"))
			{
				rdxtreeitem_deletenode(pTree);
				fprintf (stdout, "bye...\n");
				if(stack) {
					free(stack);
					stack = NULL;
				}
				return 0;
			}
			else if (!strncmp (buffer, "help", 4))
			{
				option();
				state = STATE_OTHER; 
				rdxtreeitem_deletenode(pTree);
				return 0;
			}
			else if (!strncmp (buffer, "add", 3))
			{
				state = STATE_ADD; 
			}
			else if (!strncmp (buffer, "delete", 6))
			{
				state = STATE_DELETE; 
			}
			else if (!strncmp (buffer, "del", 3))
			{
				state = STATE_DELETE; 
			}
			else if (!strncmp (buffer, "gpref", 3))
			{
				state = STATE_GET_PREFIX; 
			}
			else if (!strncmp (buffer, "gkey", 3))
			{
				state = STATE_GET_KEY; 
			}
			else if (!strncmp (buffer, "kwprint", 3))
			{
				char buff[1000] = {0};
				int count = 0;
				rdxtreeitem_getkeywords(pTree, buff, sizeof(buff), &count, ",");
				fprintf(stdout, "%s\n", buff);
				state = STATE_OTHER; 
			}
			else if (!strncmp (buffer, "print", 5))
			{
				print_rdx_tree(pTree, 0);
				state = STATE_OTHER; 
				break;
			}
			else if (strlen(buffer))
			{
				if ((numvar ++)	> MAX_STACKPTR)
				{
					fprintf (stderr, "stack overflow\n");
					continue;
				}
				newvar = (unsigned char *)  calloc (length +1, 1);
				if (!newvar)
				{
					fprintf (stderr, "out of memory\n");
					continue;
				}
				numvar ++;
				memcpy (newvar, buffer, length);
				stack->push (stack, newvar); 
			}
			if (tokenlist[token] == ' ')
				break;
			char * a;
			switch (state)
			{
			case STATE_ADD:
				if (stack->top < 0)
					break;
				do
				{
					a = stack->pop(stack);
					rdxtreeitem_insertkey(pTree, a);
					free (a);

				} while (stack->top>=0);
				break;
			case STATE_DELETE:
				if (stack->top < 0)
					break;
				do
				{
					a = stack->pop(stack);
					rdxtreeitem_deletekey(pTree, a);
					free (a);

				} while (stack->top>=0);
				break;
			case STATE_GET_PREFIX:
				if (stack->top < 0)
					break;
				do
				{
					a = stack->pop(stack);
					rdxtree_find_prefix_and_print(pTree, a);
					free (a);

				} while (stack->top>=0);
				break;
			case STATE_GET_KEY:
				if (stack->top < 0)
					break;
				do
				{
					a = stack->pop(stack);
					rdxtree_find_child_and_print(pTree, a);
					free (a);
				} while (stack->top>=0);
				break;
			case STATE_OTHER:
				while (stack->top>=0)
				{
					a = stack->pop(stack);
					free (a);

				}
				break;
			default:
				break;
			}
			numvar = 0;
			state = STATE_OTHER;
		break;
		default:
			break;
		}

		token = stream_gettoken (fp, tokenlist, buffer, MAX_PROCESSED_DATA, &length);
	}
	if(stack) {
		free(stack);
		stack = NULL;
	}

	return 0;

}

int main (int argc, char **argv)
{
	PRDXTREE_ITEM pTree = new_rdxtreeitem(NULL, 0);
	stdparse (stdin, pTree);
	exit (EXIT_SUCCESS);
}
