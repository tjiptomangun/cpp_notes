#ifndef _MYTRIE_H_
#define _MYTRIE_H_
typedef struct mytrie_item{
char name[16];
int name_len;
struct mytrie_item *parent;
struct mytrie_item *head;
struct mytrie_item *tail;
struct mytrie_item *next; 
int is_word;
}MYTRIE_ITEM, *PMYTRIE_ITEM;
#endif
