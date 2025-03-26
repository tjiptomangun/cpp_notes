#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>
#include "Parse.h"

char output[MAX];
char input[MAX];
char *s, *t ;			/*pointers to input and output strings*/
char ch;				/*choice*/
int  l ;				/*length of infix string*/

struct tag_stack output_queue;

void PushOnStack(PSTACK_ELMT element,PSTACK dest_stack);
PSTACK_ELMT PopFromStack(PSTACK  src_stack);
int priority (char *);
void InitStack(PSTACK opr_stack);
void InitStack();
void ConvertToPrefix (PSTACK opr_stack);
void ConvertToPostfix (PSTACK opr_stack);
PSTACK_ELMT MakeStackElementOfToken(int nToken);
PSTACK_ELMT MakeStackElementOfString(char *szToCreate);
void InitStack(PSTACK opr_stack)
{
	opr_stack->m_nTop=-1;
	strcpy ( output, "" ) ;
	memset(opr_stack->m_Stack,0,sizeof(opr_stack->m_Stack));
	l = 0 ;
}

void SetExpression ( char *str )
{
	s = str ;
	l = strlen ( s ) ;
	*( output + l ) = '\0' ;
	t = output;
}

void PushOnStack(PSTACK_ELMT element,PSTACK dest_stack)
{
	if(dest_stack->m_nTop== MAX-1)
	{
		printf ( "\nStack is full.\n" ) ;	
	}
	else
	{
		dest_stack->m_nTop++;
		strcpy(dest_stack->m_Stack[dest_stack->m_nTop].m_szStringValue,element->m_szStringValue);
	}
}

PSTACK_ELMT PopFromStack(PSTACK src_stack)
{
	PSTACK_ELMT pToReturn;
	if(src_stack->m_nTop==-1)
	{
		return NULL;
	}
	else
	{
		pToReturn=(PSTACK_ELMT)malloc(sizeof(STACK_ELMT));
		strcpy(pToReturn->m_szStringValue,src_stack->m_Stack[src_stack->m_nTop].m_szStringValue);
		src_stack->m_nTop--;
		return pToReturn;
	}
}

int priority ( char *c )
{
	if ( !strcmp(c,"COMPARE")||!strcmp(c,"CONCAT")||!strcmp(c,"CONTAIN")\
		||!strcmp(c,"LENGTH")||!strcmp(c,"SUBSTRING")||!strcmp(c,"TOLOWERCASE")\
		||!strcmp(c,"TOUPPERCASE")\
		||!strcmp(c,"PARSEFLOAT")||!strcmp(c,"PARSEINT")||!strcmp(c,"FORMAT"))
		return 11;
	else if ( !strcmp(c , "&"))
		return 10;
	else if ( !strcmp(c , "|"))
		return 9;
	else if ( !strcmp(c , ":"))
		return 8;
	else if ( !strcmp(c , "&&"))
		return 7;
	else if ( !strcmp(c , "||"))
		return 6;
	else if ( !strcmp(c , "^") ) 
		return 5 ;/*Exponential operator*/
	else if ( !strcmp(c , "*") || !strcmp(c , "/") || !strcmp(c ,"%") ) 
		return 4 ;
	else if ( !strcmp(c ,"+" )|| !strcmp(c , "-") ) 
		return 3 ;
	else if ( !strcmp(c ,"==" )|| !strcmp(c , ">=") || !strcmp(c , "<=")|| !strcmp(c , ">")|| !strcmp(c , "<")   ) 
		return 2 ;
	else if ( !strcmp(c ,"," )) 
		return 1 ;
	else 
		return 0 ;
}
void ConvertToPostfix (PSTACK opr_stack)
{
	int nToken;
	char *toParse=s;

	PSTACK_ELMT new_elmt;
	PSTACK_ELMT opr;
	while(nToken=GetToken(&toParse))
	{
		switch(nToken)
		{
			case NUMBER:
				strcpy(t,Buffer);
				t+=strlen(Buffer);
				opr=MakeStackElementOfString(Buffer);
				PushOnStack(opr,&output_queue);
				free(opr);
				break;
			case STRING:
				strcpy(t,Buffer);
				t+=strlen(Buffer);
				opr=MakeStackElementOfString(Buffer);
				PushOnStack(opr,&output_queue);
				free(opr);
				break;
			case LEFTPAREN:
				new_elmt=MakeStackElementOfString(Buffer);
				PushOnStack(new_elmt,opr_stack);
				free(new_elmt);
				break;
			case STR_COMPARE:
			case STR_CONCAT:
			case STR_CONTAIN:
			case STR_LENGTH:
			case STR_SUBSTRING:
			case STR_TOLOWERCASE:
			case STR_TOUPPERCASE:
			case STR_PARSEFLOAT:
			case STR_PARSEINT:
			case STR_FORMAT:
			case COMMA:
			case EQUALS:
			case GREATEROREQUALS:
			case LESSOREQUALS:
			case GREATERTHAN:
			case LESSTHAN:
			case PLUS:
			case MINUS:
			case ASTERIX:
			case DIVIDE:
			case MODULUS:
			case EXPONENT:
			case AND:
			case OR:
			case BITWISE_AND:
			case BITWISE_OR:
			case BITWISE_XOR:
			//case DOUBLE_QUOTES:
				if(opr_stack->m_nTop!=-1)
				{
					opr=PopFromStack(opr_stack);
					//while(priority(opr->m_szStringValue)>=priority(Buffer))
					while(opr && (priority(opr->m_szStringValue)>=priority(Buffer)))
					{
						*(t)=opr->m_szStringValue[0];
						t++;
						//Send to output
						opr=MakeStackElementOfString(opr->m_szStringValue);
						PushOnStack(opr,&output_queue);
						//Send to output
						opr=PopFromStack(opr_stack);
					}
					if(opr)
					{
						PushOnStack(opr,opr_stack);
					}
					new_elmt=MakeStackElementOfToken(nToken);
					PushOnStack(new_elmt,opr_stack);	
					free(new_elmt);
				}
				else
				{//donothing
					new_elmt=MakeStackElementOfToken(nToken);
					PushOnStack(new_elmt,opr_stack);
					free(new_elmt);
				}
				break;
			case RIGHTPAREN:
				{
					opr=PopFromStack(opr_stack);
					while(opr->m_szStringValue[0]!='(')
					{
						*(t)=opr->m_szStringValue[0];
						t++;
						//Send to output
						opr=MakeStackElementOfString(opr->m_szStringValue);
						PushOnStack(opr,&output_queue);
						//Send to output
						opr=PopFromStack(opr_stack);
						//free(opr);
					}
				}
				break;
			default:
				break;
		}
	}
	while(opr_stack->m_nTop!=-1)
	{
		opr=PopFromStack(opr_stack);
		*(t)=opr->m_szStringValue[0];
		t++;
		//Send to output
		opr=MakeStackElementOfString(opr->m_szStringValue);
		PushOnStack(opr,&output_queue);
		free(opr);
		//Send to output
	}
	t++;
}

PSTACK_ELMT MakeStackElementOfToken(int nToken)
{
	PSTACK_ELMT toRet=(PSTACK_ELMT )malloc(sizeof(STACK_ELMT));

	switch(nToken)
	{
		case STR_COMPARE:
		case STR_CONCAT:
		case STR_CONTAIN:
		case STR_LENGTH:
		case STR_SUBSTRING:
		case STR_TOLOWERCASE:
		case STR_TOUPPERCASE:
		case STR_PARSEFLOAT:
		case STR_PARSEINT:
		case STR_FORMAT:
			strcpy(toRet->m_szStringValue,Buffer);
			break;
		case PLUS:
			strcpy(toRet->m_szStringValue,"+");
			break;
		case MINUS:
			strcpy(toRet->m_szStringValue,"-");
			break;
		case ASTERIX:
			strcpy(toRet->m_szStringValue,"*");
			break;
		case DIVIDE:
			strcpy(toRet->m_szStringValue,"/");
			break;
		case MODULUS:
			strcpy(toRet->m_szStringValue,"%");
			break;
		case EXPONENT:
			strcpy(toRet->m_szStringValue,"^");
			break;
		case DOUBLE_QUOTES:
			strcpy(toRet->m_szStringValue,"\"");
			break;
		case AND:
			strcpy(toRet->m_szStringValue,"&&");
			break;
		case OR:
			strcpy(toRet->m_szStringValue,"||");
			break;
		case BITWISE_AND:
			strcpy(toRet->m_szStringValue,"&");
			break;
		case BITWISE_OR:
			strcpy(toRet->m_szStringValue,"|");
			break;
		case BITWISE_XOR:
			strcpy(toRet->m_szStringValue,":");
			break;
		case COMMA:
			strcpy(toRet->m_szStringValue,",");
			break;
		case EQUALS:
			strcpy(toRet->m_szStringValue,"==");
			break;
		case GREATEROREQUALS:
			strcpy(toRet->m_szStringValue,">=");
			break;
		case LESSOREQUALS:
			strcpy(toRet->m_szStringValue,"<=");
			break;
		case GREATERTHAN:
			strcpy(toRet->m_szStringValue,">");
			break;
		case LESSTHAN:
			strcpy(toRet->m_szStringValue,"<");
			break;
		case ASSIGN:
			strcpy(toRet->m_szStringValue,"=");
			break;
		default:
			return 0;
	}
	return toRet;
}

PSTACK_ELMT MakeStackElementOfString(char *szToCreate)
{
	PSTACK_ELMT toRet=(PSTACK_ELMT)malloc(sizeof(STACK_ELMT));
	strcpy(toRet->m_szStringValue,szToCreate);
	return toRet;	
}

void main( )
{
	STACK new_stack;
	char Ch;
	int i;
	FILE * fp=NULL;
	time_t long_time;
	char szXCriteriaName[40];
	char szDescription[256];
	char szModule[40];
	long nLocalTime=time(&long_time);
	char szFileName [512];
	while(1)
	{
		InitStack(&output_queue);
		InitStack(&new_stack);
		printf ( "\nEnter an expression in infix form: " ) ;
		gets ( input ) ;
		//ch=getch();
		SetExpression (input) ;	
		ConvertToPostfix (&new_stack) ;
		printf ( "\nThe Postfix expression is: " );
		for(i=0;i<=output_queue.m_nTop;i++)
		{
			printf("%s ",output_queue.m_Stack[i].m_szStringValue);
		}
		printf("\n");
		printf("\n");
		printf("[s]ave   [c]ontinue    [q]uit   ");
		printf("\n");
		printf("\n");
		Ch=getch( ) ;
		if(Ch=='s')
		{			
			printf("XCriteriaName:\n");
			gets(szXCriteriaName);
			printf("Description:\n");
			gets(szDescription);
			printf("Module:\n");
			gets(szModule);
				strftime(szFileName,512,"CONVERT_INFIX_RPN_%Y%m%d.sql",localtime(&nLocalTime));
				fp=fopen(szFileName,"a");
				fprintf(fp,"INSERT INTO XLINK.XCriteria VALUES ((SELECT NextXID FROM XLINK.XLObjectType WHERE ObjectType=60010),'%s','%s','%s','%s');commit;\n",szXCriteriaName,szDescription,input,szModule);

				for(i=0;i<output_queue.m_nTop;i++)
				{
					fprintf(fp,"INSERT INTO XLINK.XCriteriaStacks VALUES ((SELECT NextXID FROM XLINK.XLObjectType WHERE ObjectType=60010),%d,'%s');commit;\n",i,output_queue.m_Stack[i].m_szStringValue);
				}

				fprintf(fp,"UPDATE XLINK.XLObjectType SET NextXID = (SELECT NextXID+1 FROM XLINK.XLObjectType WHERE ObjectType=60010) WHERE ObjectType=60010;commit;\n",input);
				fclose(fp);
		}
		else if(Ch=='c')
		{
			continue;
		}
		else if(Ch=='q')
		{
			exit(0);
		}
	}
}

