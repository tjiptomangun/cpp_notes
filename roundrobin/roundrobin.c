/*
 * This file may be not user in xparsecfg
 * Use parserclass* instead
 *
/
#include <stdio.h>

typedef struct class
{
	struct class *this;
	void (*delete) (struct class *);
} CLASS, *PCLASS;

typedef struct circularitem
{	
	CLASS  class;
	struct circularitem *next;
	struct circularitem *prev;
} CIRCULARITEM, *PCIRCULARITEM;

typedef struct circularlist
{
	PCIRCULARITEM current;
	int count;
	void (*add) (PCIRCULARLIST, PCIRCULARITEM);
	int (*remove) (PCIRCULARLIST, PCIRCULARITEM); 
} CIRCULARLIST, *PCIRCULARLIST;

void __class_delete (PCLASS p)
{
	free (p->this);
}

void __circularitem_delete(PCIRCULARITEM item)
{
	item->next = item->prev = NULL;
	free (item);
}

void __cirularlist_add (PCIRCULARLIST clist, PCIRCULARITEM citem)
{
	if (!clist->current)
	{
		clist->current = citem;
		citem->prev = citem->next = citem; 
	}
	else
	{
		if (clist->current->pprev)
		{
			clist->current->pprev->next = citem;
			citem->next = clist->current;
			citem->prev = clist->current->prev;
			clist->current->prev = citem;
		}
	}
	count ++;
}

/*
 * Name		: __circularlist_remove
 * Description	: remove particular item from the list.
 * Returns	  NULL if item is of this list
 *		  Others if item is not of this list
 */
PCIRCULARITEM  __circularlist_remove (PCIRCULARLIST clist, PCIRCULARITEM citem)
{
	PCIRCULARITEM temp;
	temp = clist->current;
	if (!temp)
		return citem;
	do
	{
		if (citem == temp)
		{
			citem->prev->next  = citem->next;
			citem->next->prev  = citem->prev;
			if (citem == clist->current)
			{
				clist->current = NULL;
			}
			clist->count --;
			citem->class.delete (citem);	
			return NULL;
		}
		temp = temp->next;
	}while (temp != clist->current);
	return citem;
}

void __circularlist_delete (PCIRCULARLIST clist)
{
	while (clist->current)
	{
		__circularlist_remove (clist, clist->current);
	}
	free (clist); 
}
