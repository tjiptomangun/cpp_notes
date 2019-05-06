#ifndef _CHARSTR_H_
#define _CHARSTR_H_
/* string object */
typedef struct charstr{
  struct charstr *this;
  void (*delete)(struct charstr *);
  struct charstr* (*copy)(struct charstr *);
  char *data;
  int len;
  struct charstr* (*to_charstr)(struct charstr *);
}CHARSTR;
void delete_charstr(CHARSTR *);
CHARSTR *new_charstr(char *in); 
#endif
