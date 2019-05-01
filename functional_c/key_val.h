#ifndef _KEY_VAL_H_
#define _KEY_VAL_H_
#include <charstr.h>
/*key value structure*/
typedef struct key_val{
  struct key_val *this;
  void (*delete) (struct key_val *);
  CHARSTR *key;
  CHARSTR *value; 
  /*create charstr object. caller should free returned object*/
  CHARSTR *(*to_charstr) (struct key_val *); 
}KEY_VAL;
void _delete_key_val(KEY_VAL*in);
KEY_VAL *new_key_val(char *key, char *val);
#endif
