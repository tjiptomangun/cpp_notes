#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_
typedef struct radixtree_item{
char name[16];
int name_len;
struct radixtree_item *parent;
struct radixtree_item *head;
struct radixtree_item *tail;
struct radixtree_item *next; 
int is_word;
}RADIXTREE_ITEM, *PRADIXTREE_ITEM;
#endif
