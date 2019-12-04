#include <stdio.h>

#define message_for(a, b) \
  printf(#a " and " #b ": We love you!\n");

int main (int argc, char **argv) {
  message_for (Carole, Debrah);
  return ;
}
