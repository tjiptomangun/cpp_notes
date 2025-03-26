#include <stdio.h>
#include <stdarg.h>
void writeAppLog(int id,char *args,...) {
	char data[1000]="";
	va_list ap;
	printf("id : %d\n", id);
	printf("args  : %s\n", args);
	va_start(ap,args);
	vsprintf(data,args,ap);
	fprintf(stdout, "%d: %s\n", id, data);
	va_end(ap);
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
	writeAppLog(1, "%s: %s: %u", "abc", "def", 100);
	writeDebug("%s: %s: %u", "abc", "def", 100);
}
