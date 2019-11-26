#ifndef _SLIST_H_
#include <any.h>
/**
 * This is an attempt to mimic functional programming list.
 * But without copying all the structure for sublist operations.
 * Example of sublist operations : filter, take, takewhile etc
 * May won't be working.
 */
typedef struct slist {
	struct list* this;
	void (*delete) (struct list *);
	ANY* _head_;
	struct list* _tail_;
} SLIST;

SLIST *cons(ANY *hd, SLIST *tl);
SLIST *append(SLIST *list, ANY* elem);
SLIST *prepend(SLIST *list, ANY *elem);


#define _SLIST_H_

