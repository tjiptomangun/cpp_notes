#include <charstr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void delete_charstr(CHARSTR *in){
  free(in->data);
  in->len = 0;
  in->delete = 0;
  free(in);
}

CHARSTR *to_charstr(CHARSTR *in){
  return new_charstr(in->data);
}

CHARSTR *new_charstr(char *in){
  int len = strlen(in);
  char *data = calloc(1, len + 1);
  CHARSTR *ret;
  if (!data)
    return NULL;
  strncpy(data, in, len);
  ret = calloc(1, sizeof(CHARSTR));
  if(!ret){
    free(data);
    return NULL;
  }
  ret->this = ret;
  ret->len = len;
  ret->delete = delete_charstr;
  ret->data = data;
  ret->to_charstr = to_charstr;
  ret->copy = to_charstr;
  return ret; 
} 


#ifdef _CHARSTR_TEST_
int main (int argc, char **argv){
  CHARSTR *astr = new_charstr("Hello World");
  CHARSTR *astr2;
  assert(astr != NULL);
  astr2 = astr->to_charstr(astr);
  assert(astr2 != NULL);
  assert(astr->len == astr2->len);
  assert(!strcmp(astr->data, astr2->data)); 
  fprintf(stdout, "%s : %s\n", astr->data, astr2->data);
  astr->delete(astr);  
  astr2->delete(astr2); 
}
#endif
