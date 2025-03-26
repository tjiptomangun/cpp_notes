/**
 * gcc -ggdb3 -Wall -m32 -fPIC -g -c cber.c
 * gcc -m32  -shared -o libcber.so.1.0 cber.o
 * cber  : implementation of ber to xui messaging
 * created  : 20140625
 * modified : 20140815 , allow multiple tag to coexists in a tree.
 *            This is an adaptation to tlv class set.
 * treeitem is list item that can have children of type  treeitem
 * treeitem
 *      |
 *       --treecomponent
 *      |
 *       --treecomponent
 *      |
 *       --treecomponent
 *               |
 *                --treecomponent
 *
 * treenode is binary tree node that can have right and left sibling and
 * a pointer to its up sibling
 *        treenode
 *   /\    /\    /\
 *   /    /  \    \
 *  /    /    \    \
 * /up  \/    \/    up 
 *    left   right  
**/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <inttypes.h>

#include "cber.h"

/**
 * Name		: cbertreenode
 * Description	: implementation of our binary tree node of ber parser
 *
 **/
typedef struct cbertreenode
{ 
	TREECOMPONENT comp;
	struct cbertreenode *up; /*up sibling*/
	struct cbertreenode *left; /*left sibling*/
	struct cbertreenode *right; /*right sibling*/
	struct cbertreenode *parent; /*parent*/
	struct cbertreenode *childhead; /*the beginning of child tree*/
	int count; /*number of child*/
	unsigned char tag[MAX_SINGLE_TAGLEN];
	int64_t valuelen; 
	unsigned char *value; 
	int64_t childlen;
	
} CBERTREENODE, *PCBERTREENODE;


/**
 * bertreeitem
 **/

typedef struct bertreeitem
{
	TREECOMPONENT comp;
	struct bertreeitem *next; /*next sibling*/
	struct bertreeitem *parent; /*pointer to parent*/
	struct bertreeitem *childhead; /*childhead of child list*/
	struct bertreeitem *tail; /*tail of child list*/
	struct bertreeitem *curr; /*current child traversed*/ 
	int count; /*number of child*/
	unsigned char tag[MAX_SINGLE_TAGLEN]; /*current implementation only requires 4 bytes*/
	int64_t valuelen; /*may be we would not need it in the future*/ 
	unsigned char *value;
	int64_t childlen; 
} BERTREEITEM, *PBERTREEITEM;

/**
 * Name		: BtoH
 * Description	: convert array of bytes into their hexadecimal string
 **/
void BtoH(char* pDstHex, unsigned char* pSrc, int nSrcLen)
{
   int i;
   for (i = 0; i < nSrcLen; ++i)
   {
      *pDstHex = ((pSrc[i] >> 4) & 0x0F);
      if (*pDstHex < 10)
         *pDstHex += '0';
      else
         *pDstHex += 'A' - 10;
      ++pDstHex;
      *pDstHex = (pSrc[i] & 0x0F);
      if (*pDstHex < 10)
         *pDstHex += '0';
      else
         *pDstHex += 'A' - 10;
      ++pDstHex;
   }
}
/**
 * Name		: HtoB
 * Description	: convert hexadecimal string into an array of byte array
 **/
void HtoB(unsigned char * pDst, char* pSrcHex, int nSrcLenHex)
{
   char c;
   int nNibble1;
   int nNibble2;
   int i;
   for (i = 0; i < nSrcLenHex; i += 2)
   {
      c = pSrcHex[i];
      if (c >= '0' && c <= '9')
         nNibble1 = c - '0';
      else if (c >= 'A' && c <= 'F')
         nNibble1 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble1 = c - 'a' + 10;
      else
         nNibble1 = 0;
      c = pSrcHex[i+1];
      if (c >= '0' && c <= '9')
         nNibble2 = c - '0';
      else if (c >= 'A' && c <= 'F')
         nNibble2 = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
         nNibble2 = c - 'a' + 10;
      else
         nNibble2 = 0;
      *pDst++ = (nNibble1 << 4) + nNibble2;
   }

} 

/**
 * Name		: getNextTag
 * Description	: getNextTag in this stream
 * Input
 *	in	: stream to evaluate
 *	maxlen	: maximum length
 * Returns
 *	length of tag in this in, started from index 0.
 *	if it is started with 0x00 then no more tag, returns 0.
 *	if after maxlen there are no unexpanded tag found then 
 *	it returns 0.  
 */
int getNextTag (unsigned char * in, int maxlen)
{
	int i = 0;
	if (in[i] == 0x00)
		return CBER_E_NOMORETAG;
	if (!((in[i++]&0x1F)==0x1F))
		return i;
	while ((in[i] & 0x80)&& (i < maxlen))
	{
		i++;
	}
	if (i == maxlen)
		return CBER_E_NOVALIDTAG; 
	return i+1; 
}

/**
 * Name		: cmpTag
 * Description	: compare to tag. it is an MSB (little endian compare).  
 * Input
 *	tagA	: first tag
 *	tagB	: second tag
 * Returns
 *	>0	: if tagA  > tagB, also if length tagA > length tagB
 *      <0	: if tagA  < tagB
 *	0	: if both equals
 **/
int cmpTag (unsigned char *tagA, unsigned char *tagB)
{
	int lenA = 0;
	int lenB = 0;
	lenA = getNextTag (tagA,16);
	lenB = getNextTag (tagB,16);
	if (lenA != lenB)
	{
		return (lenA - lenB);
	}
	return memcmp (tagA, tagB, lenA);	
}

/**
 * Name		: getTlvStreamDataLength
 * Description	: get length of this data
 * Input 
 *	in	: stream to evaluate
 *	maxbyte	: maximum byte to evaluate
 *	result	: result length
 * Returns
 *	length of length  in bytes
 */
int getTlvStreamDataLength (unsigned char * in, int maxlength,   int64_t* result)
{ 
	int64_t res = 0;
	int i = 0; 
	res |= (in[i]&0x7F) ;
	i++;
	while ( (in[i-1] & 0x80))
	{
		res <<=7; 
		res |= (in[i]&0x7F);
		i++;
	}
	*result = res;
	return i;

}
/**
 * Name		: setTlvStreamDataLength
 * Description	: set tlv length, base 128
 * Input
 *		in 	: values to set in tlv
 *		maxout 	: maxoutput buffer size
 * Output
 *		out 	: array to put the tlv length
 * Returns	: number of bytes written to out
 **/

int setTlvStreamDataLength (int64_t in, unsigned char *out, int maxout)
{ 
	int i = 0;
	int maxbytes = 0;
	int halfbyte = 0; 
	unsigned char ctemp = 0x00;
	out[i] = (in & 0x7F);
	in>>=7;	
	i++;
	while ((in>0) && (i<=maxout))
	{
		out[i] = (in & 0x7F);
		out[i]|=0x80;	
		in >>= 7; 
		i++;
	} 
	maxbytes = i; 
	halfbyte = maxbytes>>1;
	for (i=0; i<halfbyte; i++)
	{
		ctemp = out[i];
		out[i] = out[maxbytes-1-i];
		out[maxbytes-1-i] = ctemp;
	} 

	return maxbytes;
}
/**
 * Name		: calculateBerLengthUsed
 * Description	: count number of bytes will be occupied if length is =val
 * Returns	: return the counted value. max value is 9 bytes
 **/
int calculateBerLengthUsed (int64_t val)
{ 
	int ret = 1;
	if (val >=0x80)
		ret+=1; 
	if (val >=0x4000)
		ret+=1; 
	if (val >=0x200000)
		ret+=1; 
	if (val >=0x10000000)
		ret+=1; 
	if (val >= 34359738368LL)
		ret+=1; 
	if (val >= 4398046511104LL)
		ret+=1; 
	if (val >= 562949953421312LL)
		ret+=1;
	if (val >= 72057594037927936LL)
		ret+=1;
	return ret;
}

/**
 * Name		: assignTag
 * Description	: assign a new tag to destination tag
 **/
int assignTag (unsigned char *dest, unsigned char *src)
{
	int srcLen = getNextTag(src, MAX_SINGLE_TAGLEN); 
	memcpy (dest, src, srcLen);
	return srcLen;


}


/**
 * Name		: copyTagStack
 * Description	: copy stack tag values to destination buffer
 * Input	: p -> stack where tag string originate
 * Output	  pdata -> destination where to put stack tag upside down
 * Returns	: length of string copied
 */
int copyTagStack (unsigned char *pdata, PTAGSTACK p)
{ 
	unsigned char *ptr;
	int i=0;
	ptr = pdata;
	for (i =0; i<= p->top;i++)
	{
		memcpy (ptr, p->c[i].tag, p->c[i].tagLength);
		ptr += p->c[i].tagLength;
	} 
	return ptr-pdata;	
}
static int __tagStackPush(PTAGSTACK p, unsigned char *tagstr, int taglen)
{
	if (p->top >= MAX_TAG_STACK)
		return -1;
	p->top ++;
	//fprintf (stdout, "push\n");
	p->c[p->top].tag = tagstr;
	p->c[p->top].tagLength = taglen;
	p->accumtaglen+=taglen;
	return p->top;
}

static PTAGSTACKITEM __tagStackPop(PTAGSTACK p)
{
	PTAGSTACKITEM ret;
	if (p->top < 0)
		return NULL;
	ret =  &p->c[p->top];
	p->accumtaglen-=ret->tagLength;
	p->top -=1; 
	return ret; 
}

static int __tagStackInit(PTAGSTACK p)
{
	memset (p, 0, MAX_TAG_STACK * sizeof (TAGSTACKITEM));
	p->top = -1; 
	p->accumtaglen = 0;
	p->push = __tagStackPush;
	p->pop = __tagStackPop;
	return 0;
}

int initStack(PTAGSTACK p)
{
	if (p!=NULL)
	{ 
		p->top = -1;
		p->init  = __tagStackInit;
		__tagStackInit(p);
	}
	return 0;
}

PTAGSTACK newtagstack()
{
	PTAGSTACK p= (PTAGSTACK) calloc (1, sizeof (TAGSTACK));
	if (!p)
		return NULL;
	initStack (p);
	return p;
}


extern PCBERTREENODE newcbertreenode (PCBERTREENODE parent, unsigned char *tag); 

/**
 * Name		: insert
 * Description	: insert to childhead (pto).
 * Input
 *	pto	: root
 *	in	: node insert
 * Returns	: if pto is not NULL then returns pto, otherwise
 *		  returns in.  
 **/
PCBERTREENODE insert (PCBERTREENODE pto, PCBERTREENODE in)
{
	int cmpTagRes = 0;
	if (pto == NULL)
		return in;
	cmpTagRes = cmpTag (in->tag, pto->tag);
	if (cmpTagRes >0)
	{
		if (pto->right)
			return insert (pto->right, in);
		pto->right = in;
		in->up = pto; 
	}
	else
	{
		if (pto->left)
			return insert (pto->left, in);
		pto->left = in;
		in->up = pto; 
	}
	return pto;
}

/**
 * Name		: inserttag
 * Description	: find current tag, if exists returns, otherwise create and 
 *		  and return created one
 **/
PCBERTREENODE inserttag (PCBERTREENODE parent, PCBERTREENODE pto, unsigned char *tag)
{
	int cmpTagRes = 0;
	PCBERTREENODE ret = 0;
	if (pto == NULL)
		return newcbertreenode (parent, tag);
	cmpTagRes = cmpTag (tag, pto->tag);
	if ((cmpTagRes > 0) && pto->right) 
		return inserttag (parent, pto->right, tag); 
	else if ((cmpTagRes <=0) && pto->left)
		return inserttag (parent, pto->left, tag);
	ret = newcbertreenode (parent, tag);
	if (!ret) 
		return ret;
	if (cmpTagRes>0)
		pto->right = ret;
	else
		pto->left = ret; 
	ret->up = pto; 
	return ret; 
}

/**
 * Name		: postorder
 * Description	: get the rightmost sibling
 *
 **/
PCBERTREENODE postorder (PCBERTREENODE p)
{
	if (!p->right)
		return p;
	return postorder (p->right);
}

/**
 * Name		: preorder
 * Description	: get the leftmost sibling
 **/
PCBERTREENODE preorder (PCBERTREENODE p)
{
	if (!p->left)
		return p;
	return (preorder (p->left));
}

/**
 * Name		: inorderpre
 * Description	: get the rightmost member or left child
 **/
PCBERTREENODE inorderpre (PCBERTREENODE p)
{
	if (!p->left)
		return NULL;
	return postorder (p->left);
}
/**
 * Name		: inorderpost
 * Description	: get the leftmost member of right child
 **/
PCBERTREENODE inorderpost (PCBERTREENODE p)
{
	if (!p->right)
		return NULL;
	return preorder (p->right);
}

/**
 * Name		: searchtag
 * Description	: search for specified tag
 **/
PCBERTREENODE searchnodetag (PCBERTREENODE p, unsigned char *tag)
{
	int cmpTagRes = 0;
	if (!p)
		return NULL;
	cmpTagRes = cmpTag (tag, p->tag);
	if (!cmpTagRes)
		return p;
	else if (cmpTagRes < 0)
		return searchnodetag (p->left, tag);
	return searchnodetag (p->right, tag);
}

/**
 * Name		: siblingdelete
 * Description	: delete left, delete right, delete current
 **/
int siblingdelete (PCBERTREENODE p)
{
	if (!p)
		return 0;
	if (p->left)
	{
		siblingdelete (p->left);
		p->left = NULL;
	}
	if (p->right)
	{
		siblingdelete (p->right);
		p->right = NULL;
	}
	if (p->childhead)
	{
		siblingdelete(p->childhead);
		p->childhead = NULL;
	}
	if (p->value)
	{
		//fprintf (stdout, "free %X\n", (unsigned int)p->value);
		free (p->value);
		p->value = NULL;
	}
	free (p);
	return 0;
	
}
/**
 * Name		: cleanupchild
 * Description	: clean up child of a node
 **/
int cleanupchild(PCBERTREENODE p)
{
	if (p)
		siblingdelete(p->childhead); 
	return 0;

}

/**
 * Name		: __cbertreenode_move
 * Description	: move some values of source to dest. this is not include
 *		  siblings pointers (left, right, up)
 *		  this will also delete src's value and childhead.
 **/
static int __cbertreenode_move (PCBERTREENODE dst, PCBERTREENODE src)
{
	int num = 0;
	assignTag (dst->tag, src->tag);
	if (dst->value)
		free(dst->value);
	num  = cleanupchild (dst);
	dst->value = src->value;
	dst->valuelen  = src->valuelen;
	dst->childlen = src->childlen;
	dst->childhead = src->childhead;
	src->valuelen = 0;
	src->value = NULL;
	src->childhead = NULL;
	return num;
		
} 

/**
 * Name		: delete
 * Description	: delete specified node. If no right-left siblings, the just delete it.
 *		  if there is one child, then redirect up sibling right/left pointer
 *		  to this sibling accordingly, then delete the node.
 *		  If there are 2 siblings (left/right), exchange inorderpre tag/value or inorderpost
 *		  value with current tag/value, delete the inorderpre or the inorderpost choosen.
 *		  If there are neither inorderpre nor inorderpost , then redirect parent left to
 *		  current left, and delete this
 **/
static int  __cbertreenode_delete(PCBERTREENODE p)
{
	PCBERTREENODE cand = NULL;
	
	if (!p)
		return 0;	
	
	if ((!p->left) && (!p->right))
	{
		if (p->up)
		{
			if (p->up->left == p)
				p->up->left = NULL;
			else
				p->up->right = NULL;
		}
		if (p->value)
			free (p->value);
		cleanupchild (p);
		free (p);
		return 0;
	} 
	if ((cand = inorderpre (p)) || (cand = inorderpost(p)))
	{
		__cbertreenode_move (p, cand);
		return __cbertreenode_delete(cand);
	} 
	return 0;
}


/**
 * Name		: detach
 * Description	: detach specified tag. 
 **/
static PCBERTREENODE   __cbertreenode_detach(PCBERTREENODE p, char *tag)
{
	PCBERTREENODE cand = NULL;
	
	if (!p)
		return 0;	
	
	if ((!p->left) && (!p->right))
	{
		if (p->up)
		{
			if (p->up->left == p)
				p->up->left = NULL;
			else
				p->up->right = NULL;
		}
		if (p->value)
			free (p->value);
		cleanupchild (p);
		free (p);
		return 0;
	} 
	if ((cand = inorderpre (p)) || (cand = inorderpost(p)))
	{
		__cbertreenode_move (p, cand);
		return __cbertreenode_delete(cand);
	} 
	return 0;
}


extern PCBERTREENODE newcbertreenode (PCBERTREENODE parent, unsigned char *tag);
/**
 * Name		: __cbertreenode_add
 * Description	: add an item/tag to current tree. matching tag string done to child list.
 * Input
 *	root	: root item, where to put new child
 *	in	: child tag
 * Returns
 *	0	: success
 *	-1	: No more tag in supplied "in" variable. This may indicate that your supplied
 *		  "in" tag has had an already entry in the tree.
 *	-2	: Unable to allocate memory
 *	-3	: Not a valid root
 **/
static int __cbertreenode_add(PCBERTREENODE root, unsigned char *in, int maxtaglen, int valsize)
{
	int taglen; 
	PCBERTREENODE newItem;
	unsigned char *k;
	if (!root)
		return CBER_E_NULLROOT;
	taglen = getNextTag (in, maxtaglen);
	if (taglen <= 0)
		return taglen;/*No more tag in supplied in param*/
	k = &in[taglen]; 
	newItem = inserttag (root, root->childhead, in);
	if(!newItem)
		return CBER_E_OUTOFMEMORY;
	if (!(in[0] &0x20))
	{
		if (valsize> 0)
		{
			newItem->value = (unsigned char *) calloc(1, valsize);
			if (!newItem->value)
			{
				free (newItem);
				return CBER_E_OUTOFMEMORY;
			}
			newItem->valuelen = valsize;
		}
	}
	if (!root->childhead) 
		root->childhead = newItem;
	return __cbertreenode_add (newItem, k, maxtaglen - taglen, valsize); 
}
/**
 * Name		: __cbertreenode_deletetag
 * Description	: delete tree item of this tag, deleted tag is the last tag
 *		  in this hierarchy. matching tag string done to child list.
 *		  if this is the last tag, then call deletenode to match one.
 *		  if it is node call deletetag
 **/
static int __cbertreenode_deletetag(PCBERTREENODE root, unsigned char *tag, int maxtaglen)
{
	int taglen1 = 0;
	int taglen2 = 0;
	unsigned char *k;
	PCBERTREENODE match = NULL;
	taglen1 = getNextTag (tag, maxtaglen);
	if (taglen1 <= 0)
		return CBER_E_NOVALIDTAG;
	taglen2 = getNextTag (&tag[taglen1], maxtaglen - taglen1);
	if (taglen2 < 0)
		return CBER_E_NOVALIDTAG;
	k = &tag[taglen1];
	match = searchnodetag (root->childhead, tag);
	if (!match)
		return CBER_E_TAGNOTFOUND;
	if (!taglen2)
		return __cbertreenode_delete(match);		
	return __cbertreenode_deletetag (match, k, maxtaglen - taglen1);	
}
/**
 * Name		: __cbertreenode_setvalue
 * Description	: set tlv value of current item. it is a copy instead of pointer assignment
 * Input
 *	ptr	: current item
 *	data	: new data
 *	offset	: offset to start fill this data to in ptr's value
 *	length	: new length
 * Returns	:
 *	CBER_E_OUTOFMEMORY 		: out of memory
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __cbertreenode_setvalue (PCBERTREENODE ptr, unsigned char *data, int64_t offset, int64_t length)
{
	
//	printTlvRoutine ("updatevalue", ptr->tag);
	int64_t totallength = offset+length; 
	if (ptr->value == NULL)
	{
		ptr->value = (unsigned char *) calloc (1, totallength);
		if (!ptr->value)
			return CBER_E_OUTOFMEMORY; 
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	}
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (length < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	if (totallength == ptr->valuelen)
	{
		if (! memcmp (data, &ptr->value[offset],length))//do nothing
			return 0; 
		memcpy (&ptr->value[offset], data, length); 
		return 0;
	}	
	else
	{
		ptr->valuelen = totallength; 
		ptr->value = (unsigned char *) realloc (ptr->value, totallength);
		if (!ptr->value)
			return CBER_E_OUTOFMEMORY; 
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	}
	return 0;
}
/**
 * Name		: __cbertreenode_inserttagvalue
 * Description	: insert tag of string of tag. the value is assign to last tag of
 *		  string tag. match taglist against children.
 * Input
 *	root	: root to search for the tag
 *	in	: tag to search
 *	maxtaglen : max tag length to search
 *	data	: new data to assign
 *	offset	: offset of value to start copy this new data to
 *	length	: length of data to copy
 * Returns	:
 *	CBER_E_OUTOFMEMORY 		: out of memory
 *	CBER_E_NOPRIMITIVETAG		: no primitive tag here
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/

static int __cbertreenode_inserttagvalue (PCBERTREENODE root, unsigned char *in, int maxtaglen, unsigned char *data,
	int64_t offset, int64_t length)
{
	int	taglen = 0; 
	PCBERTREENODE newItem = NULL;
	unsigned char *k;
	if (!root)
		return CBER_E_NULLROOT;
	taglen = getNextTag (in, maxtaglen);
	if (taglen <=0)
	{
		if (!(root->tag[0]&0x20))
			return __cbertreenode_setvalue (root, data, offset, length);
		return CBER_E_NOPRIMITIVETAG;
	}
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (length < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	k = &in[taglen];
	newItem = inserttag (root, root->childhead, in);
	if (!newItem)
		return CBER_E_OUTOFMEMORY;
	if (!root->childhead)
		root->childhead = newItem;
	return __cbertreenode_inserttagvalue(newItem, k, maxtaglen - taglen, data, offset, length); 
}
/**
 * Name		: __cbertreenode_updatetagvalue
 * Description	: update tag value of string of tag. the value is assign to last tag
 *		  of string tag. match taglist against children.
 * Input
 *	root	: root to search for the tag
 *	in	: tag to search
 *	maxtaglen : max tag length to search
 *	data	: new data to assign
 *	offset	: offset of value to start copy this new data to
 *	length	: length of data to copy
 * Returns	:
 *	CBER_E_TAGNOTFOUND 		: not in this tag string
 *	CBER_E_NOPRIMITIVETAG		: no primitive tag here
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __cbertreenode_updatetagvalue (PCBERTREENODE root, unsigned char *in, int maxtaglen, unsigned char *data, int64_t offset, int64_t length)
{
	int	taglen = 0; 
	PCBERTREENODE newItem = NULL;
	unsigned char *k;
	if (!root)
		return CBER_E_NULLROOT;
	taglen = getNextTag (in, maxtaglen);
	if (taglen <=0)
	{
		if (!(root->tag[0]&0x20))
			return __cbertreenode_setvalue (root, data, offset, length);
		return CBER_E_NOPRIMITIVETAG;
	}
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (length < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	k = &in[taglen];
	newItem = searchnodetag (root->childhead, in);
	if (!newItem)
		return CBER_E_TAGNOTFOUND;
	if (!root->childhead)
		root->childhead = newItem;
	return __cbertreenode_updatetagvalue(newItem, k, maxtaglen - taglen, data, offset, length); 
}
/**
 * Name		: __cbertreenode_countitemlength
 * Description	: count all child length and save it in childlen
 * Input
 *	p	: pointer to item to count
 * Returns	: childlen + taglen + valuelen 
 **/
static int64_t __cbertreenode_countitemlength (PCBERTREENODE p)
{
	int64_t ret = 0;
	int taglen = getNextTag (p->tag, MAX_SINGLE_TAGLEN); 
	if (p->left) 
		ret+=__cbertreenode_countitemlength(p->left);
	if (p->childhead)
	{
		p->childlen=__cbertreenode_countitemlength(p->childhead);
		ret+=p->childlen;
	}
	if (p->right)
		ret+=__cbertreenode_countitemlength(p->right); 
	
	ret = ret + taglen + p->valuelen + calculateBerLengthUsed (p->childlen+ p->valuelen);	
	//printTlvRoutine ("__cbertreenode_countitemlength",p->tag);
	//fprintf (stdout,"childlen =%lld valuelen =%lld ret = %lld\n", p->childlen, p->valuelen , ret);
	return ret;
}
/**
 * Name		: __cbertreenode_writeber
 * Description	: write to stdout tree data and child data. write order :
                  left in right
 * Input
 *	p	: pointer to item to write data for
 **/
static int __cbertreenode_writeber (PCBERTREENODE p)
{
	unsigned char lenTlv[32];
	int lenTLVLen = 0;
	int taglen  = 0;
	int i = 0; 
	
	if (!p)
		return 0;
	if (p->left)
		__cbertreenode_writeber (p->left);
		
	if (p->parent)
	{	
		taglen = getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		for (i=0; i<taglen; i++) 
			fprintf (stdout, "%02X", p->tag[i]); 
		memset (lenTlv, 0, 32); 
		lenTLVLen = setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
		for (i = 0; i < lenTLVLen; i++)
			fprintf (stdout, "%02X", lenTlv[i]); 
	}
	if (p->value)
	{
		for (i = 0; i< p->valuelen; i++)
		{
			fprintf (stdout, "%02X", p->value[i]);
		}
		return 0;
	}
	if (p->childhead)
		__cbertreenode_writeber (p->childhead);

	if (p->right)
		__cbertreenode_writeber (p->right);
	return 0; 
}

/**
 * Name		: __cbertreenode_serializeber 
 * Description	: serializing tree into stream. if this is the root, then  it will
 * 		  first call countitemlength.
 *
 * Input
 *	p	: pointer to tlvitem to serialize
 *	buffer	: where to put serialize data
 *	maxbuffer : maximum buffer size
 * Returns	: actual bytes wrtitten to buffer if success
 *		  if buffer size insufficient it will return CBER_E_INSUFFICIENTBUFFSZ
 **/
static int __cbertreenode_serializeber (PCBERTREENODE p, unsigned char *buffer, int maxbuffer)
{
	
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	int ctr = 0;
	//fprintf (stdout, "__cbertreenode_serializeber\n");	
	if (p->left)
		i= __cbertreenode_serializeber (p->left, &buffer[ctr], maxbuffer-ctr);
	if (i<0)
		return i;
	if (!p->parent)//root
		__cbertreenode_countitemlength(p);
	if (p->childlen > (maxbuffer - i))
		return CBER_E_UNSUFFICIENTBUFFSIZE;
	ctr+=i;
	
	if (p->parent)
	{	
		taglen = getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memcpy (&buffer[ctr], p->tag, taglen);
		ctr+=taglen;
		memset (lenTlv, 0, 32);
		//fprintf (stdout, "childlen = %lld valuelen = %lld\n",p->childlen, p->valuelen);	
		lenTLVLen = setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
		memcpy (&buffer[ctr], lenTlv, lenTLVLen);
		ctr+=lenTLVLen; 
	}
	if (p->value)
	{
		memcpy (&buffer[ctr], p->value, p->valuelen);
		ctr+=p->valuelen; 
	}
	else if (p->childhead)
	{
		i = __cbertreenode_serializeber (p->childhead, &buffer[ctr], maxbuffer-ctr);
		if (i < 0)
			return i;
		ctr+=i; 
	}
	i = 0;	
	if (p->right)
		i= __cbertreenode_serializeber (p->right, &buffer[ctr], maxbuffer-ctr);
	if (i<0)
		return i;
	ctr+=i;
	return ctr;
}
/**
 * Name		: __cbertreenode_drywriteber
 * Description	: do step needed to peform writting data, but do not printf
 * Input
 *	p	: pointer to item for data to write
 **/
static int __cbertreenode_drywriteber (PCBERTREENODE p)
{ 
	unsigned char lenTlv[32]; 
	if (p->left)
		__cbertreenode_drywriteber (p->left); 
	if (p->parent)
	{	
		getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memset (lenTlv, 0, 32); 
		setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
	}
	if (p->childhead)
	{
		__cbertreenode_drywriteber (p->childhead);	
	} 
	if (p->right)
		__cbertreenode_drywriteber (p->right);
	return 0;
}

/**
 * Name		: __cbertreenode_getvalue
 * Description	: get value of current item, copy to specified output buffer
 * Input
 *	p	: node of this value
 *	buffer	: data to copy from
 *	offset	: destination offset in node value
 *	len	: length of data to copy. if len is zero then data from offset to valuelen will be copied
 *	
 * Output
 *	copied	: number of data actually copied
 * Returns	:
 *	CBER_E_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __cbertreenode_getvalue (PCBERTREENODE p, unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	int64_t maxsize = 0; 
	if (p->tag[0] & 0x20)
		return CBER_E_NOPRIMITIVETAG;
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (len < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	if (len == 0)
	{
		memcpy (buffer, &p->value[offset], p->valuelen - offset);
		*copied = p->valuelen - offset;
		return 0;
	}
	maxsize = (p->valuelen - offset) > len?len: (p->valuelen -offset);
	if (maxsize <= 0) 
		return CBER_E_OFFSETEXCEEDVALLENGTH;
	memcpy (buffer, &p->value[offset], maxsize);
	*copied = maxsize;
	return 0;
}

/**
 * Name		: __cbertreenode_gettagvalue
 * Description	: get data of specified string tag
 * Input
 *	tag	: string tag to search 
 *	maxtaglen : maximum tag string length
 *	buffer	: buffer to store retrieved value
 *	offset	: offset of node's value to copy from
 *	len	: length of data to copy 
 * Output
 *	copied	: number of data actually copied
 * Returns	:
 *	CBER_E_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __cbertreenode_gettagvalue (PCBERTREENODE root, unsigned char *tag, int maxtaglen,
	unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	
	unsigned char *k = tag;
	int taglen = 0; 
	int xmaxtaglen = maxtaglen - taglen;
	PCBERTREENODE child; 
	do
	{
		if (! (k[0]&0x20))
			break;
		k = &k[taglen];
		xmaxtaglen -= taglen;
	}while ((taglen = getNextTag(k, xmaxtaglen)) >0);
	if ((tag[0]&0x20) && (!taglen))
		return CBER_E_NOPRIMITIVETAG;
	child = searchnodetag (root->childhead, tag);
	taglen = getNextTag(tag, maxtaglen);
	if (!child)
		return CBER_E_TAGNOTFOUND;
	if ((tag[0] &0x20))
		return __cbertreenode_gettagvalue(child, &tag[taglen], maxtaglen -taglen,
			buffer, offset, len, copied);
	return __cbertreenode_getvalue (child, buffer, offset, len, copied);
}
/**
 * Name		: __cbertreenode_initialize
 * Description	: reset variables
 **/
static int __cbertreenode_initialize(PCBERTREENODE node, PCBERTREENODE parent, unsigned char *tag)
{
	cleanupchild (node);
	if (tag)
	{
		assignTag (node->tag, tag);	
	}
	if (parent)
		node->parent = parent;
	
	node->value = NULL;
	node->valuelen = 0;
	node->count = 0;
	node->childlen = 0;
	node->childhead  = NULL;
	return 0;

}
/**
 * Name		: initializecbertreenode
 * Description	: initialize this tree node
 *
 **/
int initializecbertreenode (PCBERTREENODE node, PCBERTREENODE parent, unsigned char *tag)
{ 
	__cbertreenode_initialize( node,  parent, tag);
	node->comp.add = (int(*)(PTREECOMPONENT, unsigned char *, int, int))__cbertreenode_add;
	node->comp.del = (int(*)(PTREECOMPONENT))__cbertreenode_delete;
	node->comp.deletetag = 
		(int(*)(PTREECOMPONENT, unsigned char *, int))__cbertreenode_deletetag;
	node->comp.updatevalue = 
		(int(*)(PTREECOMPONENT, unsigned char *,int64_t, int64_t))__cbertreenode_setvalue;
	node->comp.updatetagvalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int, unsigned char *, int64_t, int64_t))__cbertreenode_updatetagvalue;
	node->comp.countitemlength = (int64_t(*)(PTREECOMPONENT))__cbertreenode_countitemlength;
	node->comp.writeber = (int(*)(PTREECOMPONENT))__cbertreenode_writeber;
	node->comp.drywriteber = (int(*)(PTREECOMPONENT))__cbertreenode_drywriteber;
	node->comp.serializeber = (int(*)(PTREECOMPONENT, unsigned char*, int))__cbertreenode_serializeber;
	node->comp.getvalue =(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t, int64_t*)) __cbertreenode_getvalue;
	node->comp.gettagvalue = 
		(int(*)(PTREECOMPONENT,unsigned char *, int, unsigned char *, int64_t, int64_t, int64_t*))__cbertreenode_gettagvalue;
	node->comp.initialize = (int(*)(PTREECOMPONENT, PTREECOMPONENT, unsigned char *))__cbertreenode_initialize;
	return 0;
}

/**
 * Name		: newcbertreenode
 * Description	: create a new cbertreenode
 * Input
 *	parent	: parent of this item
 *	tag	: tag of this item
 **/
PCBERTREENODE newcbertreenode (PCBERTREENODE parent, unsigned char *tag)
{
	PCBERTREENODE newnode = NULL; 
	newnode = (PCBERTREENODE) calloc (1, sizeof (CBERTREENODE));
	if (!newnode)
		return newnode;
	getNextTag (tag, MAX_SINGLE_TAGLEN);
	initializecbertreenode (newnode, parent, tag);
	return newnode;
}

/**
 * Name		: newrootcbertreenode
 * Description	: alloc memory for new item, set all necessary functions and variables 
 **/
PCBERTREENODE newrootcbertreenode ()
{
	PCBERTREENODE newItem = NULL;
	unsigned char tag[] = {0,0}; 
	newItem = (PCBERTREENODE) calloc (1, sizeof (CBERTREENODE));
	if (!newItem)
		return newItem; 
	initializecbertreenode (newItem, NULL, tag);
	return newItem;	
}


/**
 * Name		: printTlvRoutine
 *
 **/
int printTlvRoutine(char *routine_name, unsigned char *tag)
{
	char tagStr[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	fprintf (stdout,"%s", routine_name);
	if (tag)
	{
		BtoH (tagStr, tag, getNextTag(tag, MAX_SINGLE_TAGLEN));
	}
	fprintf (stdout, "<%s>", tagStr);
	fprintf (stdout, "\n");	
	return 0;
}
extern PBERTREEITEM newbertreeitem (PBERTREEITEM parent, unsigned char * tag);
/**
 * Name		: __bertreeitem_add
 * Description	: add an item/tag to current tree. matching tag string done to child list.
 * Input	
 *	root	: root item, where to put new child
 *	in	: child tag
 * Returns
 *	0	: success
 *	-1	: No more tag in supplied "in" variable. This may indicate that your supplied 
 *		  "in" tag has had an already entry in the tree.
 *	-2	: Unable to allocate memory
 *	-3	: Not a valid root
 **/
static int __bertreeitem_add(PBERTREEITEM root, unsigned char *in, int maxtaglen, int valsize)
{
	int taglen = 0;
	int nres = 0;
	PBERTREEITEM newItem;
	PBERTREEITEM ptrCurr = NULL, ptrPrev = NULL; 
	unsigned char *k;
	if (!root)
		return CBER_E_NULLROOT;
	taglen = getNextTag (in, maxtaglen);
	if (taglen <= 0)
		return taglen;/*No more tag in supplied "in" param*/ 
	k = &in[taglen];
	if (root->childhead == NULL)
	{
		newItem = newbertreeitem (root, in);
		if (newItem == NULL)
			return CBER_E_OUTOFMEMORY;
		root->childhead = root->tail = root->curr= newItem; 
		root->count ++;
		return __bertreeitem_add (newItem, k, maxtaglen - taglen, valsize); 
	}
	ptrPrev = NULL;
	ptrCurr = root->childhead;
	while ( ptrCurr && ((nres = cmpTag (ptrCurr->tag, in)) <0))
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (!ptrCurr)
	{ 
		newItem = newbertreeitem (root, in);
		if (newItem == NULL)
			return CBER_E_OUTOFMEMORY;
		if (!root->childhead) 
			root->childhead = newItem;
		else 
			root->tail->next = newItem; 

		root->tail = newItem; 
		root->count ++;
		if (! (in[0] & 0x20))
		{
			if (valsize > 0)
			{
				newItem->value = (unsigned char *) calloc (1, valsize);
				if (!newItem->value)
				{ 
					free (newItem); 
					return CBER_E_OUTOFMEMORY;
				} 
				newItem->valuelen = valsize; 
			} 
		}
		return __bertreeitem_add (newItem, k, maxtaglen - taglen, valsize);
	}
	if (nres ==  0) 
		return __bertreeitem_add (ptrCurr, k, maxtaglen - taglen, valsize);

	newItem = newbertreeitem (root, in);
	if (newItem == NULL)
		return CBER_E_OUTOFMEMORY;
	if (ptrPrev)
	{
		ptrPrev->next = newItem;
		newItem->next = ptrCurr; 
	}
	else
	{//we re preceding childhead
		newItem->next = ptrCurr;
		root->childhead = newItem;
	}
	root->count ++;
	return __bertreeitem_add (newItem, k, maxtaglen - taglen, valsize); 
	//found or in the end of list 

}
/**
 * Name		: __bertreeitem_delete
 * Description	: delete current treeitem, includes data and its child
 * Input	
 *	toDelete : item to delete
 *
 **/
static int __bertreeitem_delete (PBERTREEITEM toDelete)
{
	PBERTREEITEM ptrCurr;
	if (toDelete->value != NULL)
	{
		free(toDelete->value); 
		toDelete->value = NULL;
	}
	toDelete->valuelen = 0;

	ptrCurr = toDelete->childhead;
	while (ptrCurr)
	{
		toDelete->childhead = ptrCurr->next;
		__bertreeitem_delete (ptrCurr);
		ptrCurr = toDelete->childhead;
		toDelete->count --; 
	}
	toDelete->childhead = toDelete->tail = toDelete->curr = NULL;
	toDelete->parent = NULL;
	memset (toDelete->tag, 0, MAX_SINGLE_TAGLEN); 
	free (toDelete);
	return 0;
}

/**
 * Name		: __bertreeitem_deletetag
 * Description	: delete tree item of this tag, deleted tag is the last tag
 *		  in the hierarchy. matching tag string done to child list. 
 **/
static int __bertreeitem_deletetag (PBERTREEITEM root, unsigned char *tag, int maxtaglen)
{
	//getnexttlvtag 
	unsigned char *k;
	PBERTREEITEM ptrPrev, ptrCurr;
	int taglen1 = 0;
	int taglen2 = 0;
	int nres = 0;
	taglen1 = getNextTag(tag, maxtaglen);
	if (taglen1 <= 0)
		return CBER_E_NOVALIDTAG;
	taglen2 = getNextTag(&tag[taglen1], maxtaglen - taglen1);
	if (taglen2 <0)
		return CBER_E_NOVALIDTAG;
	k = &tag[taglen1];
	ptrPrev = NULL;
	ptrCurr = root->childhead;
	if (!ptrCurr)
		return CBER_E_TAGNOTFOUND;
	while (( (nres = cmpTag (ptrCurr->tag, tag)) <0)
		&& ptrCurr!=NULL)
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (nres != 0)
	{
		return CBER_E_TAGNOTFOUND;		
	}
	if (!ptrCurr)
		return CBER_E_TAGNOTFOUND;
	if (!taglen2)
	{
		//delete ptrCurr
		if (ptrCurr->next == NULL) // we're deleting tail
		{
			if (ptrPrev)
			{
				root->tail = ptrPrev;
			}
			else
			{//we're deleting both childhead and tail
				root->childhead = root->tail = ptrCurr->next;	
			}
		}
		else
		{ 
			if (ptrPrev)
				ptrPrev->next = ptrCurr->next;
			else
			//we're deleting childhead
				root->childhead  = ptrCurr->next; 
		}
		__bertreeitem_delete(ptrCurr);
		return 0;
	}
	// there delete the child
	return __bertreeitem_deletetag (ptrCurr, k, maxtaglen - taglen1);
}
/**
 * Name		: __bertreeitem_updatevalue
 * Description	: update tlv value of current item. it is a copy instead of pointer assignment.  
 * Input 
 * 	ptr	: current item
 *	data	: new data
 *	offset	: offset to start fill this data to in ptr's value
 *	length	: new length 
 * Returns	:
 *	CBER_E_OUTOFMEMORY 		: out of memory
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 *
 **/

static int __bertreeitem_updatevalue (PBERTREEITEM ptr, unsigned char *data, int64_t offset, int64_t length)
{
	//printTlvRoutine ("updatevalue", ptr->tag);
	int64_t totallength  = offset+length;
	if (ptr->value == NULL)
	{
		ptr->value = (unsigned char *) calloc (1, totallength);
		if (!ptr->value)
			return CBER_E_OUTOFMEMORY;
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	} 
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (length < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	if (totallength == ptr->valuelen)
	{
		if (! memcmp (data, &ptr->value[offset],length))//do nothing
			return 0; 
		memcpy (&ptr->value[offset], data, length); 
		return 0;
	}	
	else if (totallength < ptr->valuelen) //shorter data
	{
		memcpy (&ptr->value[offset], data, length); 
	}
	else
	{// 
	
		ptr->valuelen = 0;
		ptr->value = (unsigned char *) realloc (ptr->value, totallength);
		if (!ptr->value)
			return CBER_E_OUTOFMEMORY;
		ptr->valuelen = totallength;
		memcpy (&ptr->value[offset], data, length); 
		return 0; 
	}
	return 0;
}
/**
 * Name		: __bertreeitem_updatetagvalue
 * Description	: Propagate through tag. last item if no more tag exists.
 *		  if no more item in supplied "tag" then we might consider
 *		  this is the item to update, call updatevalue.
 *		  if the tag we are to update does not match any item 
 *		  we will return value that indicate such case. matching tag string
 *		  done to child list.
 * Input
 *	root	: root to search for the tag
 *	in	: tag to search
 *	maxtaglen : max tag length to search
 *	data	: new data to assign
 *	offset	: offset of value to start to copy this data to
 *	length	: length data to copy 
 * Returns	:
 *	CBER_E_OUTOFMEMORY 		: out of memory
 *	CBER_E_NOPRIMITIVETAG		: no primitive tag here
 *	CBER_E_INVALID_GETSET_OFFSET	: negative offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 *
 **/


static int __bertreeitem_updatetagvalue (PBERTREEITEM root, unsigned char *in,int maxtaglen ,unsigned char *data, int64_t offset, int64_t length)
{
	int taglen = 0;
	int nres = 0;
	PBERTREEITEM newItem = NULL;
	PBERTREEITEM ptrCurr = NULL, ptrPrev = NULL;
	unsigned char *k; /*next tag*/
	if (!root)
		return CBER_E_NULLROOT;
	taglen = getNextTag (in,maxtaglen);
	if (taglen <= 0)
	{
		if (! (root->tag[0]&0x20)) 
		{ 
			return __bertreeitem_updatevalue (root, data, offset, length); 

		}
		return CBER_E_NOPRIMITIVETAG;
	} 
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (length < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	//printTlvRoutine ("updatetagvalue", in);
	k = &in[taglen]; 
	ptrCurr = root->childhead;
	while (ptrCurr && ( (nres = cmpTag (ptrCurr->tag, in)) < 0))
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (!ptrCurr)
	{
		newItem = newbertreeitem (root, in);
		if (newItem == NULL)
			return CBER_E_OUTOFMEMORY;
		if (!root->childhead)
			root->childhead = newItem;
		else
			root->tail->next = newItem;
		root->tail = newItem;
		root->count ++;
		return __bertreeitem_updatetagvalue (newItem, k, maxtaglen - taglen, data, offset, length);
	}
	if (nres == 0)
		return __bertreeitem_updatetagvalue (ptrCurr, k, maxtaglen - taglen, data, offset, length);
	newItem = newbertreeitem (root, in);
	if (newItem == NULL)
		return CBER_E_OUTOFMEMORY;
	if (ptrPrev)
	{
		ptrPrev->next = newItem;
		newItem->next = ptrCurr;
	}
	else
	{
		newItem->next = ptrCurr;
		root->childhead = newItem;
	}
	root->count ++;
	return __bertreeitem_updatetagvalue (newItem, k, maxtaglen - taglen, data, offset, length); 
} 
/**
 * Name		: __bertreeitem_countitemlength
 * Description	: count all child length and save it in childlen
 * Input
 *	p	: pointer to item to count
 * Returns	: childlen + taglen + valuelen
 **/
int64_t __bertreeitem_countitemlength (PBERTREEITEM p)
{
	int64_t ret = 0;
	int taglen = getNextTag(p->tag,MAX_SINGLE_TAGLEN);
	PBERTREEITEM child = p->childhead; 
	while (child)
	{
		ret+= __bertreeitem_countitemlength (child);
		child = child->next;
	}
	p->childlen = ret;
	return ret + taglen +p->valuelen+ calculateBerLengthUsed (p->childlen + p->valuelen); 
}
/**
 * Name		: __bertreeitem_writeber
 * Description	: write to stdout tree data and child data
 * Input
 *	p	: pointer to item to write data
 *
 **/
static int __bertreeitem_writeber (PBERTREEITEM p)
{ 
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	PBERTREEITEM child = 0;
	
	if (p->parent)
	{	
		taglen = getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		for (i=0; i<taglen; i++) 
			fprintf (stdout, "%02X ", p->tag[i]); 
		memset (lenTlv, 0, 32); 
		lenTLVLen = setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
		for (i = 0; i < lenTLVLen; i++)
			fprintf (stdout, "%02X ", lenTlv[i]); 
	}
	else
	{
		fprintf (stdout, "no parent\n");
	}
	if (p->value)
	{
		for (i = 0; i< p->valuelen; i++)
		{
			fprintf (stdout, "%02X", p->value[i]);
		} 
	} 
	fprintf (stdout, "\n");
	child = p->childhead;
	while (child)
	{
		__bertreeitem_writeber (child);
		child = child->next;
	}
	return 0; 
}
/**
 * Name		: __bertreeitem_serializeber
 * Description	: serializing tree into stream. if this is the root, then it will
 *		  first call countitemlength.
 * Input
 *	p	: pointer to tlvitem to serialize
 *	buffer	: where to put serialize data
 *	maxbuffer : maximum buffer size
 * Returns	: actual bytes wrtitten to buffer if success
 *		  if buffer size insufficient it will return CBER_E_INSUFFICIENTBUFFSZ
 *
 **/
static int __bertreeitem_serializeber (PBERTREEITEM p, unsigned char *buffer, int maxbuffer)
{ 
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	int ctr = 0;
	PBERTREEITEM child = 0; 
	if (!p->parent)//root
		__bertreeitem_countitemlength(p);
	if (p->childlen > maxbuffer)
		return CBER_E_UNSUFFICIENTBUFFSIZE;
	if (p->parent)
	{	
		taglen = getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memcpy (&buffer[ctr], p->tag, taglen);
		ctr+=taglen;
		memset (lenTlv, 0, 32);
		lenTLVLen = setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
		memcpy (&buffer[ctr], lenTlv, lenTLVLen);
		ctr+=lenTLVLen;
		
	}
	if (p->value)
	{
		memcpy (&buffer[ctr], p->value, p->valuelen);
		ctr+=p->valuelen;
		return ctr;
	}
	child = p->childhead;
	while (child)
	{
		i = __bertreeitem_serializeber (child, &buffer[ctr], maxbuffer-ctr);
		ctr+=i;
		child = child->next;
	}
	return ctr;
	
}

/**
 * Name		: __bertreeitem_drywriteber
 * Description	: do step need to perform on writting data, but do not printf
 * Input
 *	p	: pointer to item to write data
 *
 **/
static int __bertreeitem_drywriteber (PBERTREEITEM p)
{ 
	unsigned char lenTlv[32]; 
	
	PBERTREEITEM child = 0;
	
	if (p->parent)
	{	
		getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memset (lenTlv, 0, 32); 
		setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
	}
	if (p->value)
	{
		return 0;
	}
	child = p->childhead;
	while (child)
	{
		__bertreeitem_drywriteber (child);
		child = child->next;
	}
	return 0; 
}

/**
 * Name		: __bertreeitem_getvalue
 * Description	: get value of current item, copy to specified output buffer
 * Input 
 *	p	: node of this value
 *	buffer	: data to copy from
 *	offset	: destination offset in node value
 *	len	: length of data to copy. if len is zero then data from offset to valuelen will be copied
 * Output
 *	copied	: number of data actually copied
 * Returns	:
 *	CBER_E_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length 
 *	CBER_E_INVALID_GETSET_OFFSET	: negativ offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negativ length
 *	0				: success		
 *
 **/
static int __bertreeitem_getvalue (PBERTREEITEM p, unsigned char *buffer,int64_t offset,  int64_t len, int64_t *copied)
{
	int64_t maxsize;
	if (p->tag[0] & 0x20)
		return CBER_E_NOPRIMITIVETAG;
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (len < 0)
		return CBER_E_INVALIDGETSETLENGTH; 
	if (len == 0)
	{
		memcpy (buffer, &p->value[offset], p->valuelen - offset);
		*copied = p->valuelen - offset;
		return 0;
	}
	maxsize = (p->valuelen - offset) > len ?len : (p->valuelen - offset);
	if (maxsize <=0)
		return CBER_E_OFFSETEXCEEDVALLENGTH;
	memcpy (buffer, &p->value[offset], maxsize);
	*copied = maxsize;
	return 0;

}
/**
 * Name		: __bertreeitem_gettagvalue
 * Description	: get data of specified string tag. matching tag string done to
 *		  child list.
 * Input
 *	p	: pointer root to search for
 *	tag	: tag string to search for
 *	maxtaglength : maximum length of tag string to search for
 *	buffer	: buffer to store value to
 *	offset	: offset of node's value to copy from
 *	len 	: length of buffer to store data 
 * Output
 *	copied	: number of data actually copied
 * Returns	:
 *	CBER_E_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CBER_E_INVALID_GETSET_OFFSET	: negativ offset
 *	CBER_E_INVALID_GETSET_LENGTH	: negativ length
 *	0				: success		
 **/
static int __bertreeitem_gettagvalue (PBERTREEITEM root, unsigned char *tag, int maxtaglen,
	unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	unsigned char *k = tag;
	int taglen = 0;
	int res = 1;
	int xmaxtaglen = maxtaglen - taglen;
	PBERTREEITEM child;

	do
	{
		if (! (k[0]&0x20))
			break;
		k = &k[taglen];
		xmaxtaglen -= taglen;
	}while ((taglen = getNextTag(k, xmaxtaglen))>0);
	if ((tag[0]&0x20) && (!taglen))
		return CBER_E_NOPRIMITIVETAG;
	if (offset < 0)
		return CBER_E_INVALIDGETSETOFFSET;
	if (len < 0)
		return CBER_E_INVALIDGETSETLENGTH;
	child = root->childhead;
	while( child && ((res = cmpTag (child->tag, tag)) <0)) 
	{
		child = child->next;
	} 
	taglen = getNextTag(tag, maxtaglen);
	if (!child)
		return CBER_E_TAGNOTFOUND;
	if (res)
		return CBER_E_TAGNOTFOUND;
	if ((tag[0]&0x20))
	{
		return __bertreeitem_gettagvalue (child, &tag[taglen], maxtaglen - taglen, 
			buffer,offset, len, copied);
	}/*it is supposed to be our child*/
	return __bertreeitem_getvalue (child, buffer, offset, len, copied);
}

static int __bertreeitem_initialize(PBERTREEITEM newItem, PBERTREEITEM parent, unsigned char *tag)
{

	int len = 0; 
	PBERTREEITEM child = newItem->childhead;
	if (tag!=NULL)
	{
		len = getNextTag(tag, MAX_SINGLE_TAGLEN);
		if (len) 
			memcpy (newItem->tag, tag, len>MAX_SINGLE_TAGLEN?MAX_SINGLE_TAGLEN:len);
	}
	if (parent)
		newItem->parent = parent;
	newItem->next = NULL;
	/*delete all child*/
	while (child)
	{
		newItem->childhead = child->next;
		__bertreeitem_delete (child);
		child = newItem->childhead;
	}	
	newItem->value = NULL;
	newItem->valuelen = 0;
	newItem->count = 0;
	newItem->childlen = 0;
	newItem->childhead = newItem->tail = newItem->curr = NULL; 
	return 0;
}
/**
 * Name		: initbertreeitem
 * Description	: initialize this item
 **/ 
int initializebertreeitem (PBERTREEITEM newItem, PBERTREEITEM parent,unsigned char *tag)
{
	__bertreeitem_initialize(newItem, parent, tag);
	newItem->comp.add = (int(*)(PTREECOMPONENT, unsigned char *, int, int))__bertreeitem_add;
	newItem->comp.del = (int(*)(PTREECOMPONENT))__bertreeitem_delete;
	newItem->comp.deletetag = 
		(int(*)(PTREECOMPONENT, unsigned char *, int))__bertreeitem_deletetag;
	newItem->comp.updatevalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t))__bertreeitem_updatevalue;
	newItem->comp.updatetagvalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int, unsigned char *, int64_t, int64_t))__bertreeitem_updatetagvalue;
	newItem->comp.countitemlength = (int64_t(*)(PTREECOMPONENT))__bertreeitem_countitemlength;
	newItem->comp.writeber = (int(*)(PTREECOMPONENT))__bertreeitem_writeber;
	newItem->comp.drywriteber = (int(*)(PTREECOMPONENT))__bertreeitem_drywriteber;
	newItem->comp.serializeber = (int(*)(PTREECOMPONENT, unsigned char*, int))__bertreeitem_serializeber;
	newItem->comp.getvalue =(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t, int64_t*)) __bertreeitem_getvalue;
	newItem->comp.gettagvalue = 
		(int(*)(PTREECOMPONENT,unsigned char *, int, unsigned char *, int64_t, int64_t, int64_t*))__bertreeitem_gettagvalue;
	newItem->comp.initialize = (int(*)(PTREECOMPONENT, PTREECOMPONENT, unsigned char *))__bertreeitem_initialize;
	return 0; 
}

/**
 * Name		: newbertreeitem
 * Description	: alloc memory for new item, set all necessary functions and variables
 * Input
 *	parent	: parent to this new item
 *	tag	: tag for this new item
 *		  it will cut the tag until unexpanded tag is found
 **/
PBERTREEITEM newbertreeitem (PBERTREEITEM parent, unsigned char *tag)
{
	PBERTREEITEM newItem = NULL; 
	newItem = (PBERTREEITEM) calloc (1, sizeof (BERTREEITEM));
	if (!newItem)
		return newItem;
	getNextTag(tag, MAX_SINGLE_TAGLEN);
	initializebertreeitem (newItem, parent, tag);
	return newItem;	
}
/**
 * Name		: newrootbertreeitem
 * Description	: alloc memory for new item, set all necessary functions and variables 
 **/
PBERTREEITEM newrootbertreeitem ()
{
	PBERTREEITEM newItem = NULL;
	unsigned char tag[] = {0,0}; 
	newItem = (PBERTREEITEM) calloc (1, sizeof (BERTREEITEM));
	if (!newItem)
		return newItem; 
	initializebertreeitem (newItem, NULL, tag);
	return newItem;	
} 


/**
 * Name 	: __berparser_setdata
 * Description	: set data of specified tag sequence
 * Input
 *	p	: pointer to parser to set data to
 *	data	: data to set
 *	datalen : length of data
 **/
static int __berparser_setData (PCXBER p, unsigned  char *data, int datalen)
{
	unsigned char y[_MAX_Y_DATA];
	unsigned char *pdata; 
	int ret = 0;
	if (p->stack.accumtaglen > _MAX_Y_DATA)
	{
		pdata = (unsigned char *) calloc(1,p->stack.accumtaglen);
		if(!pdata)
			return CBER_E_OUTOFMEMORY;
		copyTagStack (pdata, &p->stack);
		ret = p->root->updatetagvalue(p->root,
			pdata, p->stack.accumtaglen, data, 0, datalen);
		free (pdata);
		return ret;
	}
	pdata = y; 
	copyTagStack (pdata, &p->stack); 
	return p->root->updatetagvalue(p->root,
		pdata, p->stack.accumtaglen, data, 0, datalen); 
}

/**
 * Name		: __berparser_parseTlv
 * Description	: parse supplied streams into tag length and value and fill
 *		  out our tree
 */
int64_t __berparser_parseTlv (PCXBER p, unsigned char *stream, int64_t inlength)
{ 
	//tag is started from 0
	//so if we find 0x00 after all processed, then we have no more tag left 
	unsigned char *pStream = stream;
	int taglen = 0;
	int length_of_length = 0;
	int64_t accumlength = 0;
	int structure = 0;
	int64_t nret = 0;
	int64_t datalength = 0;
	unsigned char *toProc = NULL;
	int cont = 1; //we use cont to make sure every push has a pop counterpart
	if (inlength < 0)
	{ 
		return CBER_E_INVALIDSTREAMLENGTH;
	}
	taglen = getNextTag (pStream, inlength);
	if (taglen == 0)
	{
		fprintf (stdout, "return taglength  = 0\n");
		return 0;
	} 
	do
	{
		structure = *pStream &0x20;
		length_of_length = getTlvStreamDataLength (&pStream[taglen], 
			TLVSTREAM_MAX_LENGTHOFLENGTH, &datalength);
		if (length_of_length <= 0)
		{ 
			return CBER_E_INVALIDTLVSTREAMLENGTH;
		}
		p->stack.push (&p->stack, pStream, taglen);
		toProc = &pStream[taglen+ length_of_length];
		accumlength += taglen + length_of_length; 
		if(structure)
		{
			nret = __berparser_parseTlv (p, toProc, datalength);
			if (nret >= 0) 
				accumlength += nret; 
			else 
				cont = 0; 
		}
		else
		{
			nret = __berparser_setData(p, toProc, datalength);
			if (nret < 0)
				return nret;
			accumlength += datalength;
		}
		if (accumlength > inlength) 
			cont = 0;
		p->stack.pop(&p->stack);
		if (cont)
		{
			pStream = stream + accumlength;
			taglen = getNextTag(pStream, inlength - accumlength); 
		}
	}while((taglen > 0) && cont); 
	return accumlength; 
}
/**
 * Name		: __berparser_serialize
 * Description	: serialize tree into tlv stream
 * Input
 * 	p	: object that holds tlv tree
 * 	stream	: output stream
 * 	maxbuffer : buffer size
 */
static int __berparser_serialize (PCXBER p, unsigned char *stream, int maxbuffer)
{
	if (!p->root)
		return 0;
	return p->root->serializeber (p->root, stream, maxbuffer);
}
/**
 * Name		: __berparser_cleanup
 * Description	: cleanup tlv tree 
 */
static int __berparser_cleanup (PCXBER p)
{
	if (!p->root)
		return 0;
	return p->root->initialize (p->root, NULL, NULL);
}
/**
 * Name		: __berparser_deserialize
 * Description	: transform stream into tree structure
 * Input
 *	p	: object that will store this structure
 *	stream	: tlv stream
 *	inlength: length of this stream
 **/
int __berparser_deserialize (PCXBER p, unsigned char *stream, int inlength)
{
	p->stack.init(&p->stack); 
	return __berparser_parseTlv (p, stream, inlength);
}
/**
 * Name		: __berparser_delete
 * Description	: delete specified tag/tag string from tlv tree 
 **/
static int __berparser_delete (PCXBER p, unsigned char *tag)
{
	return p->root->deletetag (p->root, tag, MAX_TAGSTRING_LEN);

}
/**
 * Name		: __berparser_setdatafixedl
 * Description	: copy data to destination tag, start in offset 0 up to len
 * Input
 * 	p	: berparser
 *	tag	: tag string to set value to
 *	data	: data to copy from
 *	len	: length of data to copy
 * Returns
 *	CBER_E_NULLROOT	: if there has been no tree component
 *	CBER_E_NOPRIMITIVETAG : if no primitiva tag in tag string supplied
 *	CBER_E_OUTOFMEMORY : fail to allocate memory
 *	0	: success
 **/
static int __berparser_setdatafixedl (PCXBER p, unsigned char *tag, unsigned char *data, int len)
{
	if (!p)
		return CBER_E_NULLROOT;
	return p->root->updatetagvalue (p->root, tag, MAX_TAGSTRING_LEN,data, 0, len);
}
/**
 * Name		: __berparser_setdatavarl
 * Description	: copy data to destination tag, start in offset up to len 
 * Input
 * 	p	: berparser
 *	tag	: tag string to set value to
 *	data	: data to copy from
 *	offset	: offset in destination where to start copy data to
 *	len	: length to data to copy
 * Returns
 *	CBER_E_NULLROOT	: if there has been no tree component
 *	CBER_E_NOPRIMITIVETAG : if no primitiva tag in tag string supplied
 *	CBER_E_OUTOFMEMORY : fail to allocate memory
 *	0	: success
 **/
static int __berparser_setdatavarl (PCXBER p, unsigned char *tag, unsigned char *data, int offset, int len)
{
	if (!p)
		return CBER_E_NULLROOT;
	return p->root->updatetagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, offset, len);
}

/**
 * Name		: __berparser_getdatafixedl
 * Description	: get data from tree , start from offset zero, up to len. 
 * Input
 *	p	: berparser
 *	tag	: tag string to get value from
 *	data	: buffer to store retrieved value
 *	length	: length of value to retrieve 
 * Returns 
 *	CBER_E_NOPRIMITIVETAG		: no primitive tag in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH 	: offset exceeds tree value length
 *	0				: success
 *
 **/
static int __berparser_getdatafixedl (PCXBER p, unsigned char * tag, unsigned char * data, int length)
{
	int64_t outlength;
	if (!p)
		return CBER_E_NULLROOT;
	return p->root->gettagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, 0, length, &outlength);
}

/**
 * Name		: __berparser_getdatavarl
 * Description	: get data from tree, start from offset, up to len 
 * Input
 *	p	: berparser
 *	tag	: tag string to get value from
 *	data	: buffer to store retrieved value
 *	offset	: start offset of value to copy from
 *	length	: length of value to retrieve 
 * Output
 *	
 * Returns 
 *	CBER_E_NOPRIMITIVETAG		: no primitive tag in this tag string
 *	CBER_E_OFFSETEXCEEDVALLENGTH 	: offset exceeds tree value length
 *	0				: success
 *
 **/
static int __berparser_getdatavarl (PCXBER p, unsigned char * tag, unsigned char *data, int offset, int length, int *copied)
{
	int64_t x;
	int ret = 0;
	if (!p)
		return CBER_E_NULLROOT;
	ret =  p->root->gettagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, offset, length, &x);
	if (!ret)
		*copied  = x;
	return ret; 

}
int initializeberparser (PCXBER p)
{ 
	initStack(&p->stack); 
	p->Deserialize = __berparser_deserialize;
	p->Serialize = __berparser_serialize;
	p->Cleanup = __berparser_cleanup;
	p->Delete = __berparser_delete;
	p->GetDataFixedL = __berparser_getdatafixedl;
	p->GetDataVarL = __berparser_getdatavarl;
	p->SetDataFixedL = __berparser_setdatafixedl;
	p->SetDataVarL = __berparser_setdatavarl;
	return 0;
}
int InitCXTlvParser (PCXBER p)
{
	return initializeberparser(p);
}
/**
 * Name		: newcxber
 * Description	: new cxtlv with tree is comp
 **/
PCXBER newcxber(PTREECOMPONENT comp)
{
	PCXBER p = (PCXBER) calloc (1, sizeof (CXBER));
	if (!p)
		return p;
	p->root = comp;
	initializeberparser (p);
	return p;
}

/**
 * Name		: newctlvbtree
 * Description	: create new cxtlv that uses hierachical binary tress structure to store tlv
 **/
PCXBER newcxberbtree()
{
	PCBERTREENODE root = newrootcbertreenode ();
	PCXBER p = (PCXBER) calloc (1, sizeof (CXBER));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializeberparser (p);
	return p;
}


/**
 * Name		: newctlvllist
 * Description	: create new cxtlv that uses hierachical linked list structure to store tlv
 **/
PCXBER newcxberllist()
{
	PBERTREEITEM root = newrootbertreeitem();
	PCXBER p = (PCXBER) calloc (1, sizeof (CXBER));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializeberparser (p);
	return p;
}

/**
 * Name		: newdefaultcxber
 * Description	: create new cxtlv that uses hierachical linked list structure to store tlv
 **/
PCXBER NewDefaultCXBer()
{
	PBERTREEITEM root = newrootbertreeitem();
	PCXBER p = (PCXBER) calloc (1, sizeof (CXBER));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializeberparser (p);
	return p;
}

