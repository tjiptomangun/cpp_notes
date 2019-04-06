/**
 * $CC -Wall -ggdb3 mempool.c -o mempool
 **/


#include <stdio.h>
#include <stdlib.h>

struct node;
typedef struct node *ptrtonode;
typedef ptrtonode memory;
typedef struct node nodetype;
int pro_id, pro_size;
memory create();

struct node
{
	int id;
	int start;
	int hole;
	int end;
	ptrtonode next;
};

memory create()
{
	memory mem;
	mem = (ptrtonode)malloc(sizeof(nodetype));
	mem->next = NULL;
	return mem;
}

int main (int argc, char **argv)
{

}

