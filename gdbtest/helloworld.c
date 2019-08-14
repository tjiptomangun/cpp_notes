/*/
 * gcc -Wall -ggdb3  helloworld.c -o helloworld
 */

#include <stdio.h>

int test () {
  int k = 100;
  k *= 2;
  k += 2;
  k /=(1);
  return k;
}
int main (int argc, char **argv) {
  int j = 100;
  j += test(); 
  j ++;
  return j;
}
