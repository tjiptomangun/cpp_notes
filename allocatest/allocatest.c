#include <stdio.h>
#include <alloca.h>

unsigned char *allocatest(){
	unsigned char *allocat;
	allocat = alloca(200); 
	return allocatest();
}

int main (int argc, char **argv) {
	allocatest(10);	
}
