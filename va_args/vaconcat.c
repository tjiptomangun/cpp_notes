#include <stdio.h>
#include <stdarg.h>
char * vaconcat(char *init, char *args,...) {
	char data[1000]="";
	va_list ap;
	va_start(ap,args);
	vsprintf(data,args,ap);
	strcat(init,data);
	va_end(ap);
	return init;
}
/* not working
void writeDebug(char* fmt,  ...) {
#ifndef __NO_DEBUG__
	va_list args;
	va_start(args, fmt);
	writeAppLog(0, fmt, args);
	va_end(args);
#endif
}
*/
void writeDebug(char *args,...) {
	char data[1000]="";
	va_list ap;
	printf("args  : %s\n", args);
	va_start(ap,args);
	vsprintf(data,args,ap);
	fprintf(stdout, "%d: %s\n", 0, data);
	va_end(ap);
}
int main(int argc, char** argv) {
	char initial[2000] = {0};
	vaconcat(initial, "NoVLR <%s %d>","abc", 10); 
	vaconcat(initial, " VLR <%s %d>","abc", 10); 
	printf("%s\n", initial);
}
