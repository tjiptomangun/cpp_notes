/**
 * gcc -Wall sscanfstringstar.c -o sscanfstringstar
 * "%*s" means interpret capture but ignore it
 */
int main(int argc , char **argv) {
	const char* toEv = "abcdef 10 29 abc";
	int ten = 0;
	int twentynine = 0;
	char abc[20] = {0};
	sscanf(toEv, "%*s %d %d %s", &ten, &twentynine, abc);
	printf("%d, %d, %s\n", ten, twentynine, abc);
}
