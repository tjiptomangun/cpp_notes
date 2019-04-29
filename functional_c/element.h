#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <charstr.h>
#include <list.h>
#include <any.h>
#include <key_val.h>
typedef struct element {
  struct element *this;
  void (*delete) (struct element *);
  CHARSTR *name;
  LIST *attributes;
  LIST *child;
}


#endif
