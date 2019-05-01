#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <charstr.h>
#include <list.h>
#include <any.h>
#include <key_val.h>
/*xml element*/
typedef struct element {
  struct element *this;
  void (*delete) (struct element *);
  /*name of this element*/
  CHARSTR *name;
  LIST *attributes;
  /*child nodes*/
  LIST *children;
  /*Add attribute, key and val will be copied , so you can delete it*/
  KEY_VAL *add_attribute(struct element *, char *key, char *val);
  /*add child, child won't be copied, so do not free it*/ 
  LIST *add_child(struct element *, ANY *child);
} ELEMENT; 
void _delete_element(ELEMENT *in);
ELEMENT *new_element(char *name);
#endif
