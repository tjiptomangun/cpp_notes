#ifndef __TRIE_H__
#define __TRIE_H__

typedef struct trie_item
{ 
	char key;
	struct trie_item *parent;
	struct trie_item *head;
	struct trie_item *tail;
	struct trie_item *curr;
	struct trie_item *next;
} TRIE_ITEM, *PTRIE_ITEM;
#endif
