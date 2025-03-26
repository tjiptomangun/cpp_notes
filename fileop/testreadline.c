#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
char **msisdn_list;
#define ANSWER_MAX_STRING 10
#define MAX_PROMPTS 20
typedef struct prompt_answers { 
	int maxPromptAnswer; 
	int currentAnswer;
	char answers[MAX_PROMPTS][ANSWER_MAX_STRING]; 
} PROMPT_ANSWERS;

PROMPT_ANSWERS *list_prompt_answers;

static char*
trim(const char *str, char *buf, int buf_len)
{
  char *p = NULL;

  memset (buf, 0, buf_len);

  // trim head
  while (*str) {
    if (isspace(*str)) str++;
    else break;
  }
  int nlen = strlen(str);
  memcpy(buf, str, buf_len< nlen ? buf_len : nlen);
  p = buf;
  p += strlen(buf) - 1;

  // trim tail
  while (p && *p) {
    if (isspace(*p)) {
      *p = 0;
       p--;
    } else
       break;
  }

  return buf;
}

PROMPT_ANSWERS* addPromptAnswers(PROMPT_ANSWERS *ans, char *toAdd) {
	char buffer[1024] = {0};
	char *pBuff;
	if ((*(pBuff = trim(toAdd, buffer, 1024))) !=0) {
		if (ans->maxPromptAnswer <MAX_PROMPTS)
			strncpy(ans->answers[ans->maxPromptAnswer ++], pBuff, ANSWER_MAX_STRING);
	}
	return ans;
}

int num_msisdn = 0;
void printMsisdnPrompt() {
	char *msisdn;
	PROMPT_ANSWERS *prompt_answers = list_prompt_answers;
	for (int i = 0; i < num_msisdn; i++) {
		msisdn = msisdn_list[i];
		printf("msisdn : [%s] answers", msisdn);
		for (int j = 0; j < prompt_answers->maxPromptAnswer; j++) {
			printf("[%d: %s]",j, prompt_answers->answers[j]);
		}
		printf("\n");
		prompt_answers ++;

	}
}
char * answer(char *msisdn, char *out, size_t outsize) {
	char *ret = NULL;
	char found = 0;
	int i = 0;
	PROMPT_ANSWERS *prompt_ans = NULL;
	for (i = 0; i < num_msisdn; i++) {
		if (!strcmp(msisdn_list[i], msisdn)) {
			found = 1;
			break;
		}
	}
	if (found) {
		prompt_ans = list_prompt_answers + i;
		if (prompt_ans->maxPromptAnswer) {
			if (prompt_ans->answers[prompt_ans->currentAnswer][0] != 0) {
				strcpy(out, prompt_ans->answers[prompt_ans->currentAnswer]);
				prompt_ans->currentAnswer++;
				return out;
			}
		}
	}
	return ret;
}
void printAnswer(char *in)  {
	char x[1024] = {0};
	printf("%s %s\n", in, answer(in, x, 1024));
}
int main (int argc, char **argv) {
	int ch; 
	int count=0;
	int index;

	FILE *fp=fopen("msisdn_list.txt","rt");
	if(fp==NULL) {
		printf("Can not open file\n"); 
		return(-1); 
	}
	do { 
		ch = fgetc(fp); 
		if( ch== '\n') 
			count++;   
	}while( ch != EOF );   
	num_msisdn = count;
	fclose(fp);
	msisdn_list = malloc(sizeof(char *)*num_msisdn);
	list_prompt_answers = calloc(num_msisdn, sizeof(PROMPT_ANSWERS));
	PROMPT_ANSWERS *prompt_answers = list_prompt_answers;

	for(index = 0;index < num_msisdn;index++) { 
		msisdn_list[index] = calloc(1, 20); 
	}
	fp=fopen("msisdn_list.txt","rt");
	index = 0;
	char buffer[1024] = "";
	char tmpBuffer[1024];
	char tokBuffer[1024];
	char *pbuff;
	char *pTmpBuff;
	char *pTokBuff;
	char *tok;
	int tokIndex;
	char *eol;
	while(fgets(buffer,1024,fp)) 
	{
		eol = NULL;
		if(index >= num_msisdn) { 
			break; 
		} 
		else if ((*(pbuff = trim(buffer, tmpBuffer, 1024)) == 0)) {
			continue;
		} else if ((eol = strstr(pbuff, "\n"))) {
			*eol = 0;
		}
		tokIndex = 0;
		tok = strtok(pbuff, ",");
		while(tok != NULL) {
			if (tokIndex == 0) {
				pTokBuff = trim(tok, tokBuffer, 1024);
				sprintf(msisdn_list[index],"%s", pTokBuff);
			} else {
				addPromptAnswers(prompt_answers, tok);
			}
			tokIndex ++;
			tok = strtok (NULL, ",");
		}
		prompt_answers++;
		index ++;
	}
	fclose(fp);
	printMsisdnPrompt();
	char *k ;
	printAnswer("124233241");
	printAnswer("68632320");
	printAnswer("68632320");
	printAnswer("54334353");
	printAnswer("5433435");
	printAnswer("124233241");
	printAnswer("124233241");
	printAnswer("54334353");
	printAnswer("68632320");
	printAnswer("68632320");
	printAnswer("54334353");
	

}








