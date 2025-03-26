#include <stdio.h>
#include "logregx.h"

int main(int argc, char **argv){
	int k = replaceWithOne("(qu|[b-df-hj-np-tv-z]*)([a-z]+)",
		"\\2\\1ay",	
      "the quick brown fox jumps over the lazy dogs.",
      "ethay quick brown fox jumps over the lazy dogs.",
      "ethay ickquay ownbray oxfay umpsjay overay ethay azylay ogsday.",
      9);
	printf("%d\n", k);
}
