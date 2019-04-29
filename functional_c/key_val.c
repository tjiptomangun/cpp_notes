#include <key_val.h>
#include <charstr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static CHARSTR *_key_val_to_charstr(KEY_VAL* in){
  char * outstr = (char *) calloc(1, in->key->len + in->value->len + 6);
  CHARSTR *out;
  sprintf(outstr, "%s = \"%s\"", in->key->data, in->value->data);
  out = new_charstr(outstr);
  free(outstr);
  return out; 
}

void _delete_key_val(KEY_VAL *in){
  in->key->delete(in->key);
  in->value->delete(in->value);
  in->key = NULL;
  in->value = NULL;
  in->this = NULL;
  in->delete = NULL;
  in->to_charstr = NULL;
}

KEY_VAL *new_key_val(char *key, char *val) {
  CHARSTR *_key;
  CHARSTR *_val;
  KEY_VAL *kv;
  _key = new_charstr(key);
  if(!_key)
    return NULL;
  _val = new_charstr(val);
  if (!_val){
    free(_key);
    return NULL;
  }
  kv = (KEY_VAL *)calloc(1, sizeof(KEY_VAL));
  if (!kv){
    free(_key);
    free(_val);
    return NULL;
  }
  else {
    kv->key = _key;
    kv->value = _val;
    kv->this = kv;
    kv->delete = _delete_key_val;
    kv->to_charstr = _key_val_to_charstr; 
    return kv;
  }
}

#ifdef _KEY_VAL_TEST_
#include <assert.h>
int main(int argc, char **argv){
  KEY_VAL *kv = new_key_val("name", "rudy");
  CHARSTR *str;
  assert(kv != NULL);
  str = kv->to_charstr(kv);
  assert(!strcmp(str->data, "name = \"rudy\""));
  fprintf(stdout, "%s\n", str->data);
  kv->delete(kv);
  str->delete(str); 
}
#endif
