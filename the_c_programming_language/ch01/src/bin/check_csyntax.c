#include <stdio.h>

#define PARENTHESE		0
#define BRACKET			1
#define BRACE			2
#define QUOTE			3
#define DOUBLE_QUOTE	4
#define ESCAPE			5
#define COMMENT			6

#define MAX_TOKEN_PAIR  7

#define OPEN_PAREN		'('
#define CLOSE_PAREN		')'
#define OPEN_BRACE		'{'
#define CLOSE_BRACE		'}'
#define OPEN_BRACKET	'['
#define CLOSE_BRACKET	']'



int main (int argc, char **argv){
	int parenthese = 0;
	int bracket = 0;
	int brace = 0;
	int paren_last = -1;
	int bracket_last = -1;
	int brace_last = -1;
	int linenumber = 0;
	int charnumber = 0;
	int ch = 0;
	int instring = 0;
	int incomment = 0;
	int inchar = 0;
	int prevchar = 0;
	int inescape = 0;

	while ((ch = getchar()) != EOF) {
		if (ch == '\n'){
			if (inchar){ 
				inchar --;
				printf("inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
			}
			if (inescape){
				inescape = 0;
			}
			linenumber ++;
			charnumber = 0;
		}	
		else if (ch == '*' && prevchar == '/'){
			if (!instring){
				incomment ++;
				if (incomment > 1){
					printf("starting another comment on active comemnt on line %d:%d\n", linenumber + 1, charnumber + 1);
				}
			}
		}
		else if (ch == '/' && prevchar == '*' && incomment){
			if (!instring){
				incomment  --;
				if (incomment < 0){
					printf("invalid comment on line %d:%d\n", linenumber + 1, charnumber + 1);
				}
			}
		}
		else if (ch == '"'){
			if (!incomment){
				if(!inchar){
					if (!instring)
						instring ++;	
					else if (instring)
						instring  --;
				}
			}
		} 
		else if (!instring && !incomment)
		{
			if (ch == '\''){
				if (prevchar == '\'' && !inescape){
					printf("empty character literal on line %d:%d\n", linenumber + 1, charnumber + 1); 
				}
				else if(prevchar == '\'' && inescape) { /*handle '\'' case but break on '\\' case */
					inescape --;
					inchar --;
					printf("inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
					printf("*inescape -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);
				}
				else if (inchar){
					if (inescape==2 && prevchar == '\\'){ /* break on '\'' */
						inescape = 0; 
						printf("inescape = 0 on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);
						inchar--;
						printf("**inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
					}
					else if(!(inescape == 1 && prevchar == '\\')){
						inchar --;
						printf("***inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
					}
					
				}
				else{
					inchar ++;
					printf("inchar ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
				}
			}
			else if (!inchar)
			{ 
				if (ch == CLOSE_PAREN){
					parenthese --;
					if (parenthese < 0){
						printf("unmatch close parentheses on line %d:%d\n", linenumber + 1, charnumber + 1);
					}
					
				} 
				else if (ch == OPEN_PAREN) {
					parenthese ++;
					paren_last = linenumber;
				}
				else if (ch == CLOSE_BRACE){
					brace --;		
					if (brace < 0){
						printf("unmatch close brace on line %d:%d\n", linenumber + 1, charnumber + 1);
					}
				}
				else if (ch == OPEN_BRACE) {
					brace ++;
					brace_last = linenumber;
				}
				else if (ch == CLOSE_BRACKET){
					bracket --;		
					if (bracket < 0){
						printf("unmatch close bracket on line %d:%d\n", linenumber + 1, charnumber + 1);
					}
				}
				else if (ch == OPEN_BRACE) {
					bracket ++;
					bracket_last = linenumber;
				}
			}
			else if (ch == '\\') {
				inescape ++;
				printf("inescape ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);
				if (inescape > 2){
					printf("invalid escape sequence on line %d:%d\n", linenumber + 1, charnumber + 1);
				}
			}
			else if (ch == ' '){
				if (prevchar != '\'' && prevchar != '\\' ){
					printf("premature character literal on line %d:%d\n", linenumber + 1, charnumber + 1);
				}
			}
/*
			else {
				if (!inescape){
					if (prevchar != '\''){
						printf("invalid string literal on line %d:%d\n", linenumber + 1, charnumber + 1);
					}	
				}
			}
*/

		}/*instring incomment*/
		prevchar = ch; 
		charnumber ++;
	}

	if (parenthese > 0){
		printf("unclosed parentheses on line %d\n", paren_last + 1);
	}
	if (bracket > 0) {
		printf("unclosed bracket on line %d\n", bracket_last + 1);
	}
	if (brace > 0){
		printf("unclosed brace on line %d\n", brace_last + 1);
	}

	return 0;	
}
