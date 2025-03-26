#define NUMKEYWORDS 11


#define MAX	4096

//TOKENS
#define	LEFTPAREN		0x100
#define RIGHTPAREN		0x101
#define COMMA			0x102
#define NUMBER			0x103
#define STRING			0x104

//OPERATORS
#define EQUALS				0x201//==
#define GREATEROREQUALS		0x202//>=
#define LESSOREQUALS		0x203//<=
#define GREATERTHAN			0x204//>
#define LESSTHAN			0x205//<

#define PLUS			0x206//+
#define MINUS			0x207//-
#define ASTERIX			0x208//*
#define DIVIDE			0x209//\/
#define MODULUS			0x20a//%
#define EXPONENT		0x20b//^
#define AND				0x20c//&&
#define OR				0x20d//||
#define BITWISE_AND		0x20e//&
#define BITWISE_OR		0x20f//|
#define BITWISE_XOR		0x210//:
#define ASSIGN			0x211//=

#define STR_COMPARE		0x001
#define STR_CONCAT		0x002
#define STR_CONTAIN		0x003
#define STR_LENGTH		0x004
#define STR_SUBSTRING	0x005
#define STR_TOLOWERCASE	0x006
#define STR_TOUPPERCASE	0x007
#define STR_PARSEFLOAT	0x008
#define STR_PARSEINT	0x009
#define STR_FORMAT		0x00a

//STRING SIGN
#define DOUBLE_QUOTES	0x301

int GetToken(char **strToParse);
char Buffer[128];
typedef struct tag_stack_elmt
{
	char m_szStringValue[256];
}STACK_ELMT,*PSTACK_ELMT;

typedef struct tag_stack
{
	struct tag_stack_elmt m_Stack[1256];
	int            m_nTop;
}STACK,*PSTACK;

typedef struct tag_node
{
	struct tag_node * left_child;
	char m_szStringValue[128];
	struct tag_node * right_child;
}NODE,*PNODE,**P_PNODE;

typedef struct tag_node_stack
{
	int	  m_nTop;
	PNODE m_stack[MAX];
}NODESTACK,*PNODESTACK;

