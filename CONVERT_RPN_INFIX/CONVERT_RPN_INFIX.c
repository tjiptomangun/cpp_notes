#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "Parse.h"

PNODE CreateExpTreePostfix(char *);
PNODE CreateExpTreePrefix(char *);

void  preorder(PNODE sr);
void  inorder(PNODE sr);
void  postorder(PNODE sr);
char *Evaluate(PNODE sr);
void  push(PNODESTACK, PNODE);
PNODE pop(PNODESTACK);
int	  isNumber(char *);
void  Delete_Tree(PNODE );
//int   top=0;

char Buffer[128];
NODESTACK stack;

PNODE CreateExpTreePostfix(char * str)
{
	int nToken;
	PNODE nleft,nright,nodeptr;	
	char *toParse=str;
	while(nToken=GetToken(&toParse))
	{
		nodeptr=(PNODE)malloc (sizeof(NODE));
		strcpy(nodeptr->m_szStringValue,Buffer);
		switch(nToken)
		{
			case STR_COMPARE:
				//COMPARE (STR1,STR2) which come in postfix format STR1 STR2 , COMPARE
				//So the left child would be , and the right child would be NULL
				//but also notice that the comma part has already been processed since it comes before 
				//the COMPARE function
			case STR_CONCAT:
			case STR_CONTAIN:
			case STR_LENGTH:
			case STR_SUBSTRING:
			case STR_TOLOWERCASE:
			case STR_TOUPPERCASE:
			case STR_PARSEFLOAT:
			case STR_PARSEINT:
			case STR_FORMAT:
				nleft=pop(&stack);
				nright=NULL;
				nodeptr->left_child=nleft;
				nodeptr->right_child=nright;
				break;
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
				nright=pop(&stack);
				nleft=pop(&stack);
				nodeptr->left_child=nleft;
				nodeptr->right_child=nright;
				break;
			default:
				nodeptr->left_child=NULL;
				nodeptr->right_child=NULL;
				break;

		}
		push(&stack,nodeptr);
	}
	return(pop(&stack));
}

PNODE CreateExpTreePrefix(char *str)
{
	int nToken;
	PNODE nleft,nright,nodeptr;	
	char *toParse=str;
	strrev(str);
	while(nToken=GetToken(&toParse))
	{
		nodeptr=(PNODE)malloc (sizeof(NODE));
		strcpy(nodeptr->m_szStringValue,Buffer);
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
			case COMMA:
			case PLUS:
			case MINUS:
			case ASTERIX:
			case DIVIDE:
			case MODULUS:
			case EXPONENT:
				nleft=pop(&stack);
				nright=pop(&stack);
				nodeptr->left_child=nleft;
				nodeptr->right_child=nright;
				break;
			default:
				nodeptr->left_child=NULL;
				nodeptr->right_child=NULL;
				break;

		}
		push(&stack,nodeptr);
	}
	return(pop(&stack));
}

void inorder(PNODE sr)
{
	if(sr!=NULL)
	{
		inorder(sr->left_child);
		//print the data of the node whose left child is NULL or the patch has already been traversed
		printf("%s ",sr->m_szStringValue);
		inorder(sr->right_child);
	}
}

void preorder(PNODE sr)
{
	if(sr!=NULL)
	{
        /* print the data of a node */
        printf("%s ", sr ->m_szStringValue) ;
        /* traverse till left child is NULL */
        preorder(sr -> left_child) ;
        /* traverse till right child is NULL */
        preorder(sr -> right_child) ;	
	}
}

void postorder(PNODE sr)
{
    if (sr != NULL)
    {
		/* traverse till left child is NULL */
        postorder(sr -> left_child) ;
		//postorder(sr -> right_child) ;
		/* traverse till left child is NULL */
        postorder(sr -> right_child) ;
		//postorder(sr -> left_child) ;
        /* print the data of a node */
        printf("%s ", sr -> m_szStringValue) ;
    }
}

void push(PNODESTACK _stack,PNODE ptr)
{
	if(_stack->m_nTop==MAX-1)
		printf("\nStack is full.\n") ;
	else
	{
		_stack->m_nTop++ ;
		_stack->m_stack[_stack->m_nTop] = ptr ;
		//_stack->m_stack[_stack->m_nTop++] = ptr ;
	}
}


PNODE pop(PNODESTACK _stack)
{
    if (_stack->m_nTop == -1)
    {
        printf("Stack is empty\n") ;
        return -1 ;
    }
    else
    {
        PNODE ptr = _stack->m_stack[_stack->m_nTop--] ;
        //top-- ;
        return ptr ;
    }
}

char *Evaluate(PNODE sr)
{
	char sz_X[128],sz_Y[128],sz_Z[128];
	int n_X,n_Y,n_Z;
	int nToken;
	char * sz_temp=sr->m_szStringValue;
	if(sr!=NULL)
	{
		nToken=GetToken(&sz_temp);
		switch(nToken)
		{
			case PLUS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X+n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s + %s)",sz_X,sz_Y);
				}
				break;
			case MINUS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X-n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s - %s)",sz_X,sz_Y);				
				}
				break;
			case ASTERIX:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X*n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s * %s)",sz_X,sz_Y);				
				}
				break;
			case DIVIDE:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X/n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s / %s)",sz_X,sz_Y);				
				}
				break;
			case MODULUS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X%n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s %% %s)",sz_X,sz_Y);				
				}
				break;
			case EXPONENT:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=pow(n_X,n_Y);
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s ^ %s)",sz_X,sz_Y);
				}
				break;
			case AND:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X&&n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s && %s)",sz_X,sz_Y);
				}
				break;
			case OR:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X||n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s || %s)",sz_X,sz_Y);
				}
				break;
			case BITWISE_AND:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X&n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s & %s)",sz_X,sz_Y);
				}
				break;
			case BITWISE_OR:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X|n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s | %s)",sz_X,sz_Y);
				}
				break;
			case BITWISE_XOR:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X^n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s : %s)",sz_X,sz_Y);
				}
				break;
			case EQUALS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=(n_X==n_Y);
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s == %s)",sz_X,sz_Y);
				}
				break;
			case GREATEROREQUALS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X>=n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s >= %s)",sz_X,sz_Y);
				}
				break;
			case LESSOREQUALS:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X<=n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s <= %s)",sz_X,sz_Y);
				}
				break;
			case GREATERTHAN:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X>n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s > %s)",sz_X,sz_Y);
				}
				break;
			case LESSTHAN:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X<n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s < %s)",sz_X,sz_Y);
				}
				break;
			case ASSIGN:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				if(isNumber(sz_X)&&isNumber(sz_Y))
				{
					n_X=atoi(sz_X);
					n_Y=atoi(sz_Y);
					n_Z=n_X=n_Y;
					itoa(n_Z,sz_Z,10);
				}
				else
				{
					sprintf(sz_Z,"(%s = %s)",sz_X,sz_Y);
				}
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
				sprintf(sz_X,"%s",Buffer);
				sprintf(sz_Y,"%s",Evaluate(sr->left_child));//Get The Comma
				sprintf(sz_Z,"%s(%s)",sz_X,sz_Y);
				break;
			case COMMA:
				sprintf(sz_X,"%s",Evaluate(sr->left_child));
				sprintf(sz_Y,"%s",Evaluate(sr->right_child));
				sprintf(sz_Z,"%s , %s",sz_X,sz_Y);
				break;				
			default:
				//sprintf(str_Z,"%s ^ %s",str_X,strY);
				sprintf(sz_Z,"%s",Buffer);
				break;

		}
	}
	return sz_Z;
}

void Delete_Tree(PNODE root)
{
    if(root!=NULL)
    {
        Delete_Tree(root->left_child);
        Delete_Tree(root->right_child);
        free(root);
    }
}

void InitStack()
{
	memset(&stack,0,sizeof(stack));
	stack.m_nTop=-1;
}

int main()
{
    PNODE root;
    char str[50];
    char * z;
    char ch;
    //clrscr();
	InitStack();
  //  printf("Input expression is:\n1)Prefix\n2)Postfix ");
  //  ch=getche();
	/*
    if(ch=='1')
	{
		printf("\nEnter Prefix Expression:");
		gets(str);
		root = CreateExpTreePrefix(str);
	}
    else
	*/
	{
		printf("\nEnter Postfix Expression:");
		gets(str);
		root =  CreateExpTreePostfix(str);
	}
    printf("\nPrefix Exp.  :");
    preorder(root);
    printf("\nInfix Exp.   :");
    inorder(root);
    printf("\nPostfix Exp. :");
    postorder(root);
    z=Evaluate(root);
    printf("\nExpression Evaluated to: %s", z);
    Delete_Tree(root);
    getch();
}
int isNumber(char * strIn)
{
	int toReturn;
	int nLen=strlen(strIn);	
	int i;
	toReturn=1;
	for(i=0;i<nLen;i++)
	{
		if(isdigit(strIn[i]))
		{
			toReturn&=1;
		}
		else
		{
			toReturn&=0;
		}
	}
	return toReturn;
}