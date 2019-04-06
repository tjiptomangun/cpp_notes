/**
 * heap
 * parent must have lesser value than both children
 * $CC -Wall -ggdb3 heap.c -o heap
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct heapnode
{
	int idx;
	int value;
} HEAPNODE, *PHEAPNODE;

typedef struct heap
{
	PHEAPNODE *nodes;
	int	max;
	int	idx;
} HEAP, *PHEAP;
#define HEAP_OUTOFMEMORY	-1
#define HEAP_OVERFLOW		-2
#define HEAP_UNDERFLOW		-3
/**
 * Name		: newnode
 * Description	: create a new node
 **/
PHEAPNODE newnode (int value)
{
	PHEAPNODE ret = NULL;
	ret = (PHEAPNODE) calloc (1, sizeof (HEAPNODE));
	if (!ret)
		return ret;
	ret->value = value;
	return ret;
}
/**
 * Name		: newheap
 * Description	: allocate new heap. Assign its values;
 * 
 **/
PHEAP newheap (int max_member)
{
	PHEAP ret = NULL;
	ret = (PHEAP) calloc (1, sizeof (HEAP));
	if (!ret)
		return NULL;
	ret->nodes = (PHEAPNODE *) calloc (max_member, sizeof (PHEAPNODE));
	if (!ret->nodes)
	{
		free (ret);
		return NULL;
	}
	ret->max = max_member;
	ret->idx = -1;
	return ret;

}
PHEAPNODE heapnode_getparent (PHEAPNODE x, PHEAP pheap)
{
	int parent_idx  = -1;
	if (!x)
		return NULL;
	if (!x->idx)
		return NULL;
	parent_idx = (x->idx - 1) >> 1;
	return pheap->nodes[parent_idx];
}

PHEAPNODE heapnode_getleft (PHEAPNODE x, PHEAP pheap)
{
	int left_idx = -1;
	if (!x)
		return NULL;
	left_idx = (x->idx<<1) + 1;	
	return pheap->nodes[left_idx];
}

PHEAPNODE heapnode_getright (PHEAPNODE x, PHEAP pheap)
{
	int right_idx = -1;
	if (!x)
		return NULL;
	right_idx = (x->idx<<1) + 2;
	return pheap->nodes[right_idx];
}
/**
 * Name		: heapnode_swap
 * Description	: swap values among two nodes
 **/
void heapnode_swap (PHEAPNODE x, PHEAPNODE y, PHEAP pheap)
{
	int temp = 0;
	temp = x->value;
	x->value = y->value;
	y->value = temp; 
	return;
}
/**
 * Name		: upheap
 * Description	: compares a heap to its parent
 * 		  if it is of lesser value then switch value
 **/
void heapnode_upheap (PHEAPNODE x, PHEAP pheap)
{ 
	PHEAPNODE p = NULL;
	if (!(p = heapnode_getparent (x, pheap)))
		return;
	if (p->value <= x->value)
		return;
	heapnode_swap (p, x, pheap); 
	return heapnode_upheap (p, pheap);
}
/**
 * Name		: downheap
 * Description	: compares a heap against both child,
 * 		  if it is of any greater value to one of them
 * 		  switch their value
 */
void heapnode_downheap (PHEAPNODE x, PHEAP pheap)
{ 
	PHEAPNODE c = NULL;
	if (!(c = heapnode_getleft (x, pheap)))
		return;
	if (c->value < x->value)
	{
		heapnode_swap(c,x,pheap);
		return heapnode_downheap (c, pheap);
	}
	if (!(c = heapnode_getright (x, pheap)))
		return;
	if (c->value < x->value)
	{
		heapnode_swap(c,x,pheap);
		return heapnode_downheap (c, pheap);
	} 
	return ;
}

/**
 * Name		: heapnode_add
 * Description	: add a node
 * input
 * 	idx	: idx/number of current bottommost, rightmost item
 * returns
 * 	-2	overflow (idx>=max)
 * 	0	success	
 **/
int heapnode_add (PHEAPNODE x, PHEAP pheap)
{
	if (pheap->idx >= pheap->max)
		return HEAP_OVERFLOW;
	x->idx = ++pheap->idx;
	pheap->nodes [pheap->idx]  = x;
	heapnode_upheap (x, pheap);
	return 0;
}

/**
 * Name		: heapnode_delete
 * Description	: delete a node
 * input	
 * 	idx	: idx/number of current bottommost, rightmost item
 * returns
 * 		  idx/number of bottommost, rightmost item after deletion
 **/
int heapnode_del (PHEAPNODE x, PHEAP pheap)
{
	if (x->idx<0)
		return x->idx;
	heapnode_swap (pheap->nodes[pheap->idx], x, pheap); 
	pheap->nodes[pheap->idx--] = NULL;
	heapnode_downheap (x, pheap); 
	return 0; 
}

int heap_print (PHEAP x)
{
	int idx;
	for (idx = 0; idx <= x->idx; idx++)
	{
		fprintf (stdout , "value of %d : %d\n", idx, x->nodes[idx]->value);
	}
	return 0;
}
int main (int argc, char **argv)
{
	PHEAP x = newheap (1000); 
	PHEAPNODE y = NULL;
	if (!x)
		exit (EXIT_FAILURE);
	y = newnode (100);
	heapnode_add (y, x);
	y = newnode (57);
	heapnode_add (y, x);
	y = newnode (37);
	heapnode_add (y, x);
	y = newnode (27);
	heapnode_add (y, x); 
	y = newnode (21);
	heapnode_add (y, x);
	y = newnode (87);
	heapnode_add (y, x);
	y = newnode (23);
	heapnode_add (y, x);
	y = newnode (20);
	heapnode_add (y, x);
	heap_print(x);
	y = x->nodes[0];
	fprintf (stdout, "del idx %d of val %d\n", 0, y->value);
	heapnode_del (y, x);
	heap_print(x);
	y = x->nodes[2];
	fprintf (stdout, "del idx %d of val %d\n", 2, y->value);
	heapnode_del (y, x);
	heap_print(x);
	y = x->nodes[0];
	fprintf (stdout, "del idx %d of val %d\n", 0, y->value);
	heapnode_del (y, x);
	heap_print(x);
	exit (EXIT_SUCCESS);
}
