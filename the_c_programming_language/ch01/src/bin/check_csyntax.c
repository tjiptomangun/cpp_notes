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



int main (int argc, char **argv){ /*1*/
	int parenthese = 0;
	int bracket = 0;
	int brace = 0;
	int paren_last = -1;
	int bracket_last = -1;
	int brace_last = -1;
	int comment_last = -1;
	int string_last = -1;
	int linenumber = 0;
	int charnumber = 0;
	int ch = 0;
	int instring = 0;
	int incomment = 0;
	int inchar = 0;
	int prevchar = 0;
	int inescape = 0;

	while ((ch = getchar()) != EOF) {/*2*/
		if (ch == '\n'){/*3*/
			linenumber ++;
			charnumber = 0;
		}/*2*/
		else if (ch == '*' && prevchar == '/'){/*3*/
			if (!instring){/*4*/
				incomment ++;
				comment_last = linenumber;
				/* printf("incomment ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, incomment); */
				if (incomment > 1){/*5*/
					printf("starting another comment on active comment on line %d:%d\n", linenumber + 1, charnumber + 1);
				}/*4*/
			}/*3*/
		}/*2*/
		/*else if (ch == '/' && prevchar == '*' && incomment){*/
		else if (ch == '/' && prevchar == '*'){/*3*/
			if (!instring){/*4*/
				incomment --;
				/* printf("incomment -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, incomment); */
				if (incomment < 0){/*5*/
					printf("invalid comment on line %d:%d\n", linenumber + 1, charnumber + 1);
				}/*4*/
			}
		}/*2*/
		else if (ch == '"'){/*3*/
			if (!incomment){/*4*/
				if(!inchar){/*5*/
					if (!instring){/*6*/
						instring ++;	
						string_last = linenumber;
					}
					else if (instring){
						instring  --;
					}
				}
			}
		}/*2*/ 
		else if (!instring && !incomment)
		{/*3*/
			if (ch == '\''){/*4*/
				if (prevchar == '\'' && !inescape){/*5*/
					printf("empty character literal on line %d:%d\n", linenumber + 1, charnumber + 1); 
				}
				else if(prevchar == '\'' && inescape) { /*handle '\'' case but break on '\\' case */
					inescape --;
					inchar --;
					/*	
					printf("inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);
					printf("*inescape -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);
					*/
					
				}
				else if (inchar){
					if (inescape==2 && prevchar == '\\'){ /* case '\\' */
						inescape = 0; 
						inchar--;
						/*
						printf("inescape = 0 on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape); 
						printf("**inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar); */
					}
					/*@TODO: simplify these 2 else if below*/
					else if (inescape && prevchar != '\\') { /*--- case '\n' , '\b' etc*/
						inchar --;
						inescape --;
						/* printf("****inescape -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);
						printf("****inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar); */
					} 
					else if(!(inescape == 1 && prevchar == '\\')){ /*--- case '\'' */
						inchar --;
						/*printf("***inchar -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);*/
					} 
					
				}
				else{
					inchar ++;
					/*printf("inchar ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inchar);*/
				}
			}/*3*/
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
					/*printf("brace -- on line %d:%d:%d\n", linenumber + 1, charnumber + 1, brace);*/
					if (brace < 0){
						printf("unmatch close brace on line %d:%d\n", linenumber + 1, charnumber + 1);
					}
				}
				else if (ch == OPEN_BRACE) {
					brace ++;
					/*printf("brace ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, brace);*/
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
				/*printf("inescape ++ on line %d:%d:%d\n", linenumber + 1, charnumber + 1, inescape);*/
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
	if (incomment > 0){
		printf("unclosed comment on line %d\n", comment_last + 1);
	}
	if (instring > 0){
		printf("unclosed string on line %d\n", string_last + 1);
	}

	return 0;	
}
