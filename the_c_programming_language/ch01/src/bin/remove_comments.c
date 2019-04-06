#include <stdio.h>

/**
 * Test against yourself
 * With c comments
 */
#define STATE_INCOMMENT 0
#define STATE_OUTCOMMENT 1
int main (int argc, char **argv) {
	int curr_char;
	int prev_char = 0;
	int i = 0;
	int state = STATE_OUTCOMMENT;
	int prev_state; /*state of previous character*/
	
	while ((curr_char = getchar()) != EOF){
		if (prev_char == '/' && curr_char == '*'){
			prev_state = state;
			state = STATE_INCOMMENT;
		}
		else if (prev_char == '*' && curr_char == '/'){
			prev_state = state;
			state = STATE_OUTCOMMENT;
		} 
		else if (state == STATE_OUTCOMMENT && i > 0){
			if (prev_state == STATE_OUTCOMMENT)
				putchar(prev_char);
			prev_state = state;
			state = STATE_OUTCOMMENT;
		}
		prev_char = curr_char;
		i ++;
	}
	/*doubt here*/
	putchar(prev_char); /*should i need this ?*/ 
} /*main*/
