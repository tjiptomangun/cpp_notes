#include "Parse.h"
#include "String.h"


CNode::CNode()
{

}
CNode::CNode(int nIsXimField,char *szStringValue)
{
	m_nIsXIMField=nIsXimField;
	strcpy(m_szStringValue,szStringValue);
}
CNode::CNode(int nIsXimField,char *szStringValue,CNode *left,CNode *right)
{
	m_nIsXIMField=nIsXimField;
	strcpy(m_szStringValue,szStringValue);
	left_child=left;
	right_child=right;
}
CNode::~CNode()
{

}
CNodeStack::CNodeStack()
{

}
CNodeStack::~CNodeStack()
{

}
int CNodeStack::Push(CNode* NewNode)
{
	if(m_nTop==MAX-1)
		return 0;
	else
	{
		m_nTop++;
		m_Stack[m_nTop].m_nIsXIMField=NewNode->m_nIsXIMField;
		strcpy(m_Stack[m_nTop].m_szStringValue,NewNode->m_szStringValue);
		m_Stack[m_nTop].left_child=NewNode->left_child;
		m_Stack[m_nTop].right_child=NewNode->right_child;
		return 1;
	}
}

CNode* CNodeStack::Pop()
{
	
	if(m_nTop==-1)
		return NULL;
	return &this->m_Stack[m_nTop--];
}

int CXCriteria::Priority(char *c)
{
	if (  !strcmp(c,"COMPARE")||!strcmp(c,"CONCAT")||!strcmp(c,"CONTAIN")\
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
	else if ( !strcmp(c , "+" )|| !strcmp(c , "-") ) 
		return 3 ;
	else if ( !strcmp(c ,"==" )|| !strcmp(c , ">=") || !strcmp(c , "<=")|| !strcmp(c , ">")|| !strcmp(c , "<")   ) 
		return 2 ;
	else if ( !strcmp(c ,"," )) 
		return 1 ;
	else 
		return 0 ;
}


CParser::CParser()
{
	strcpy(this->KeyWords[ 0],"");
	strcpy(this->KeyWords[ 1],"COMPARE");
	strcpy(this->KeyWords[ 2],"CONCAT");
	strcpy(this->KeyWords[ 3],"CONTAIN");
	strcpy(this->KeyWords[ 4],"LENGTH");
	strcpy(this->KeyWords[ 5],"SUBSTRING");
	strcpy(this->KeyWords[ 6],"TOLOWERCASE");
	strcpy(this->KeyWords[ 7],"TOUPPERCASE");
	strcpy(this->KeyWords[ 8],"PARSEFLOAT");
	strcpy(this->KeyWords[ 9],"PARSEINT");
	strcpy(this->KeyWords[10],"FORMAT");

}
CParser::~CParser()
{

}