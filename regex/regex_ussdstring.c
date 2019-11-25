#include <regex.h>                                                              
#include <locale.h>                                                             
#include <stdio.h>                                                              
#include <stdlib.h>

int regcheck(char *in) {
  //char *pattern = "^(\\*){0,1}([0-9]+\\*)*(#){0,1}$";
  char *pattern = "^(\\*){0,1}([0-9]+\\**)*([0-9])+#{0,1}$";
  //char *pattern = "#$";
  
  regex_t    preg; 
  if (regcomp(&preg, pattern, REG_EXTENDED) != 0){
    fprintf(stderr, "error compile regex %s\n", pattern);
    return -1;
  }
  fprintf(stdout, "in %s : ", in);
  if(regexec(&preg, in, 0, NULL, 0) == REG_NOMATCH){
    fprintf(stdout, "No Match \n");
  }
  else 
    fprintf(stdout, "Match \n");
  return 0;
}

int main (int argc, char **argv) {
  char *string1 = "*888#";
  char *string2 = "*123*24222323*3#";
  char *string3 = "*123*#";
  char *string4 = "*123**";
  char *string5 = "*123";
  char *string6 = "*AAA*123432#";

  regcheck(string1);
  regcheck(string2);
  regcheck(string3);
  regcheck(string4);
  regcheck(string5);
  regcheck(string6);
}
                          
