/**
 * gcc -ggdb3 -Wall -m32 -fPIC -g -c xuitlv.c
 * gcc -m32  -shared -o libxuitlv.so.1.0 xuitlv.o
**/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <inttypes.h>

#include "ctlv.h"

/**
 * Name		: xuitlvtreenode
 * Description	: implementation of our binary tree node of tlv parser
 *
 **/
typedef struct xuitlvtreenode
{ 
	TREECOMPONENT comp;
	struct xuitlvtreenode *up; /*up sibling*/
	struct xuitlvtreenode *left; /*left sibling*/
	struct xuitlvtreenode *right; /*right sibling*/
	struct xuitlvtreenode *parent; /*parent*/
	struct xuitlvtreenode *head; /*the beginning of child tree*/
	int count; /*number of child*/
	unsigned char tag[MAX_SINGLE_TAGLEN];
	int64_t valuelen;
	unsigned char *value;
	int64_t childlen;
	
} XUITLVTREENODE, *PXUITLVTREENODE;


/**
 * tlvtreeitem
 **/

typedef struct tlvtreeitem
{
	TREECOMPONENT comp;
	struct tlvtreeitem *next; /*next sibling*/
	struct tlvtreeitem *parent; /*pointer to parent*/
	struct tlvtreeitem *head; /*head of child list*/
	struct tlvtreeitem *tail; /*tail of child list*/
	struct tlvtreeitem *curr; /*current child traversed*/ 
	int count; /*number of child*/
	unsigned char tag[MAX_SINGLE_TAGLEN]; /*current implementation only requires 4 bytes*/
	int64_t valuelen; /*may be we would not need it in the future*/ 
	unsigned char *value;
	int64_t childlen; 
} TLVTREEITEM, *PTLVTREEITEM;

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
 * Name		: __xui_getNextTag
 * Description	: __xui_getNextTag in this stream
 * Input
 *	in	: stream to evaluate
 *	maxlen	: maximum length
 * Returns
 *	length of tag in this in, started from index 0.
 *	if it is started with 0x00 then no more tag, returns 0.
 *	if after maxlen there are no unexpanded tag found then 
 *	it returns 0.  
 */
int __xui_getNextTag (unsigned char * in, int maxlen)
{
	int i = 0;
	if (in[i] == 0x00)
		return CTLV_NOMORETAG;
	if (!((in[i++]&0x1F)==0x1F))
		return i;
	while ((in[i] & 0x80)&& (i < maxlen))
	{
		i++;
	}
	if (i == maxlen)
		return CTLV_NOVALIDTAG; 
	return i+1; 
}

/**
 * Name		: __xui_tagCmp
 * Description	: compare to tag. it is an MSB (little endian compare).  
 * Input
 *	tagA	: first tag
 *	tagB	: second tag
 * Returns
 *	>0	: if tagA  > tagB, also if length tagA > length tagB
 *      <0	: if tagA  < tagB
 *	0	: if both equals
 **/
int __xui_tagCmp (unsigned char *tagA, unsigned char *tagB)
{
	int lenA = 0;
	int lenB = 0;
	lenA = __xui_getNextTag (tagA,16);
	lenB = __xui_getNextTag (tagB,16);
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
 * Name		: calculateTlvLengthUsed
 * Description	: count number of bytes will be occupied if length is =val
 * Returns	: return the counted value. max value is 9 bytes
 **/
int calculateTlvLengthUsed (int64_t val)
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
	int srcLen = __xui_getNextTag(src, MAX_SINGLE_TAGLEN); 
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


extern PXUITLVTREENODE newxuitlvtreenode (PXUITLVTREENODE parent, unsigned char *tag);

/**
 * Name		: insert
 * Description	: insert to head (pto). If such item doet no exist
 * Input
 *   pto	: root
 *   in		: node insert
 * Returns	: if pto is not NULL then returns pto, otherwise
 *		  returns in.
 **/
PXUITLVTREENODE insert (PXUITLVTREENODE pto, PXUITLVTREENODE in)
{
	int __xui_tagCmpRes = 0;
	if (pto == NULL)
		return in;
	__xui_tagCmpRes = __xui_tagCmp (in->tag, pto->tag);
	if (!__xui_tagCmpRes)
		return pto;
	if (__xui_tagCmpRes >0)
	{
		if (pto->right)
			return insert (pto->right, in);
		pto->right = in;
		in->up = pto;
	}
	else if (__xui_tagCmpRes <0)
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
PXUITLVTREENODE inserttag (PXUITLVTREENODE parent, PXUITLVTREENODE pto, unsigned char *tag)
{
	int __xui_tagCmpRes = 0;
	PXUITLVTREENODE ret = 0;
	if (pto == NULL)
		return newxuitlvtreenode (parent, tag);
	__xui_tagCmpRes = __xui_tagCmp (tag, pto->tag);
	if (!__xui_tagCmpRes)
		return pto;
	if ((__xui_tagCmpRes > 0) && pto->right) 
		return inserttag (parent, pto->right, tag); 
	else if ((__xui_tagCmpRes <0) && pto->left)
		return inserttag (parent, pto->left, tag);
	ret = newxuitlvtreenode (parent, tag);
	if (!ret) 
		return ret;
	if (__xui_tagCmpRes>0)
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
PXUITLVTREENODE postorder (PXUITLVTREENODE p)
{
	if (!p->right)
		return p;
	return postorder (p->right);
}

/**
 * Name		: preorder
 * Description	: get the leftmost sibling
 **/
PXUITLVTREENODE preorder (PXUITLVTREENODE p)
{
	if (!p->left)
		return p;
	return (preorder (p->left));
}

/**
 * Name		: inorderpre
 * Description	: get the rightmost member or left child
 **/
PXUITLVTREENODE inorderpre (PXUITLVTREENODE p)
{
	if (!p->left)
		return NULL;
	return postorder (p->left);
}
/**
 * Name		: inorderpost
 * Description	: get the leftmost member of right child
 **/
PXUITLVTREENODE inorderpost (PXUITLVTREENODE p)
{
	if (!p->right)
		return NULL;
	return preorder (p->right);
}

/**
 * Name		: searchtag
 * Description	: search for specified tag
 **/
PXUITLVTREENODE searchnodetag (PXUITLVTREENODE p, unsigned char *tag)
{
	int __xui_tagCmpRes = 0;
	if (!p)
		return NULL;
	__xui_tagCmpRes = __xui_tagCmp (tag, p->tag);
	if (!__xui_tagCmpRes)
		return p;
	else if (__xui_tagCmpRes < 0)
		return searchnodetag (p->left, tag);
	return searchnodetag (p->right, tag);
}

/**
 * Name		: siblingdelete
 * Description	: delete left, delete right, delete current
 **/
int siblingdelete (PXUITLVTREENODE p)
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
	if (p->head)
	{
		siblingdelete(p->head);
		p->head = NULL;
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
int cleanupchild(PXUITLVTREENODE p)
{
	if (p)
		siblingdelete(p->head); 
	return 0;

}
/**
 * Name		: __xuitlvtreenode_move
 * Description	: move some values of source to dest. this is not include
 *		  siblings pointers (left, right, up)
 *		  this will also delete src's value and head.
 **/
static int __xuitlvtreenode_move (PXUITLVTREENODE dst, PXUITLVTREENODE src)
{
	int num = 0;
	assignTag (dst->tag, src->tag);
	if (dst->value)
		free(dst->value);
	num  = cleanupchild (dst);
	dst->value = src->value;
	dst->valuelen  = src->valuelen;
	dst->childlen = src->childlen;
	dst->head = src->head;
	src->valuelen = 0;
	src->value = NULL;
	src->head = NULL;
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
static int  __xuitlvtreenode_delete(PXUITLVTREENODE p)
{
	PXUITLVTREENODE cand = NULL;
	
	if (!p)
		return 0;	
	
	if (!(p->left || p->right))
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
		__xuitlvtreenode_move (p, cand);
		return __xuitlvtreenode_delete(cand);
	} 
	return 0;
}

extern PXUITLVTREENODE newxuitlvtreenode (PXUITLVTREENODE parent, unsigned char *tag);
/**
 * Name		: __xuitlvtreenode_add
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
static int __xuitlvtreenode_add(PXUITLVTREENODE root, unsigned char *in, int maxtaglen, int valsize)
{
	int taglen; 
	PXUITLVTREENODE newItem;
	unsigned char *k;
	if (!root)
		return CTLV_NULLROOT;
	taglen = __xui_getNextTag (in, maxtaglen);
	if (taglen <= 0)
		return taglen;/*No more tag in supplied in param*/
	k = &in[taglen]; 
	newItem = inserttag (root, root->head, in);
	if(!newItem)
		return CTLV_OUTOFMEMORY;
	if (!(in[0] &0x20))
	{
		if (valsize> 0)
		{
			newItem->value = (unsigned char *) calloc(1, valsize);
			if (!newItem->value)
			{
				free (newItem);
				return CTLV_OUTOFMEMORY;
			}
			newItem->valuelen = valsize;
		}
	}
	if (!root->head) 
		root->head = newItem;
	return __xuitlvtreenode_add (newItem, k, maxtaglen - taglen, valsize); 
}
/**
 * Name		: __xuitlvtreenode_deletetag
 * Description	: delete tree item of this tag, deleted tag is the last tag
 *		  in this hierarchy. matching tag string done to child list.
 *		  if this is the last tag, then call deletenode to match one.
 *		  if it is node call deletetag
 **/
static int __xuitlvtreenode_deletetag(PXUITLVTREENODE root, unsigned char *tag, int maxtaglen)
{
	int taglen1 = 0;
	int taglen2 = 0;
	unsigned char *k;
	PXUITLVTREENODE match = NULL;
	taglen1 = __xui_getNextTag (tag, maxtaglen);
	if (taglen1 <= 0)
		return CTLV_NOVALIDTAG;
	taglen2 = __xui_getNextTag (&tag[taglen1], maxtaglen - taglen1);
	if (taglen2 < 0)
		return CTLV_NOVALIDTAG;
	k = &tag[taglen1];
	match = searchnodetag (root->head, tag);
	if (!match)
		return CTLV_TAGNOTFOUND;
	if (!taglen2)
		return __xuitlvtreenode_delete(match);		
	return __xuitlvtreenode_deletetag (match, k, maxtaglen - taglen1);	
}
/**
 * Name		: __xuitlvtreenode_updatevalue
 * Description	: update tlv value of current item. it is a copy instead of pointer assignment
 * Input
 *	ptr	: current item
 *	data	: new data
 *	offset	: offset to start fill this data to in ptr's value
 *	length	: new length
 * Returns	:
 *	CTLV_OUTOFMEMORY 		: out of memory
 *	CTLV_INVALID_GETSET_OFFSET	: negative offset
 *	CTLV_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __xuitlvtreenode_updatevalue (PXUITLVTREENODE ptr, unsigned char *data, int64_t offset, int64_t length)
{
	
//	printTlvRoutine ("updatevalue", ptr->tag);
	int64_t totallength = offset+length; 
	if (ptr->value == NULL)
	{
		ptr->value = (unsigned char *) calloc (1, totallength);
		if (!ptr->value)
			return CTLV_OUTOFMEMORY; 
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	}
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (length < 0)
		return CTLV_INVALIDGETSETLENGTH;
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
		//ptr->valuelen = totallength; 
	}
	else
	{
		ptr->valuelen = totallength; 
		ptr->value = (unsigned char *) realloc (ptr->value, totallength);
		if (!ptr->value)
			return CTLV_OUTOFMEMORY; 
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	}
	return 0;
}
/**
 * Name		: __xuitlvtreenode_updatetagvalue
 * Description	: update tag value of string of tag. the value is assign to last tag
 *		  of string tag. match taglist with child, if not found add one.  
 * Input
 *	root	: root to search for the tag
 *	in	: tag to search
 *	maxtaglen : max tag length to search
 *	data	: new data to assign
 *	offset	: offset of value to start copy this new data to
 *	length	: length of data to copy
 * Returns	:
 *	CTLV_OUTOFMEMORY 		: out of memory
 *	CTLV_NOPRIMITIVETAG		: no primitive tag here
 *	CTLV_INVALID_GETSET_OFFSET	: negative offset
 *	CTLV_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __xuitlvtreenode_updatetagvalue (PXUITLVTREENODE root, unsigned char *in, int maxtaglen, unsigned char *data,
	int64_t offset, int64_t length)
{
	int	taglen = 0; 
	PXUITLVTREENODE newItem = NULL;
	unsigned char *k;
	if (!root)
		return CTLV_NULLROOT;
	taglen = __xui_getNextTag (in, maxtaglen);
	if (taglen <=0)
	{
		if (!(root->tag[0]&0x20))
			return __xuitlvtreenode_updatevalue (root, data, offset, length);
		return CTLV_NOPRIMITIVETAG;
	}
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (length < 0)
		return CTLV_INVALIDGETSETLENGTH;
	k = &in[taglen];
	newItem = inserttag (root, root->head, in);
	if (!newItem)
		return CTLV_OUTOFMEMORY;
	if (!root->head)
		root->head = newItem;
	return __xuitlvtreenode_updatetagvalue(newItem, k, maxtaglen - taglen, data, offset, length); 
}
/**
 * Name		: __xuitlvtreenode_countitemlength
 * Description	: count all child length and save it in childlen
 * Input
 *	p	: pointer to item to count
 * Returns	: childlen + taglen + valuelen 
 **/
static int64_t __xuitlvtreenode_countitemlength (PXUITLVTREENODE p)
{
	int64_t ret = 0;
	int taglen = __xui_getNextTag (p->tag, MAX_SINGLE_TAGLEN); 
	if (p->left) 
		ret+=__xuitlvtreenode_countitemlength(p->left);
	if (p->head)
	{
		p->childlen=__xuitlvtreenode_countitemlength(p->head);
		ret+=p->childlen;
	}
	if (p->right)
		ret+=__xuitlvtreenode_countitemlength(p->right); 
	
	ret = ret + taglen + p->valuelen + calculateTlvLengthUsed (p->childlen+ p->valuelen);	
	//printTlvRoutine ("__xuitlvtreenode_countitemlength",p->tag);
	//fprintf (stdout,"childlen =%lld valuelen =%lld ret = %lld\n", p->childlen, p->valuelen , ret);
	return ret;
}
/**
 * Name		: __xuitlvtreenode_writetlv
 * Description	: write to stdout tree data and child data. write left in right
 * Input
 *	p	: pointer to item to write data for
 **/
static int __xuitlvtreenode_writetlv (PXUITLVTREENODE p)
{
	unsigned char lenTlv[32];
	int lenTLVLen = 0;
	int taglen  = 0;
	int i = 0; 
	
	if (!p)
		return 0;
	if (p->left)
		__xuitlvtreenode_writetlv (p->left);
		
	if (p->parent)
	{	
		taglen = __xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
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
	if (p->head)
		__xuitlvtreenode_writetlv (p->head);

	if (p->right)
		__xuitlvtreenode_writetlv (p->right);
	return 0; 
}

/**
 * Name		: __xuitlvtreenode_serializetlv 
 * Description	: serializing tree into stream. if this is the root, then  it will
 * 		  first call countitemlength.
 *
 * Input
 *	p	: pointer to tlvitem to serialize
 *	buffer	: where to put serialize data
 *	maxbuffer : maximum buffer size
 * Returns	: actual bytes wrtitten to buffer if success
 *		  if buffer size insufficient it will return CTLV_INSUFFICIENTBUFFSZ
 **/
static int __xuitlvtreenode_serializetlv (PXUITLVTREENODE p, unsigned char *buffer, int maxbuffer)
{
	
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	int ctr = 0;
	//fprintf (stdout, "__xuitlvtreenode_serializetlv\n");	
	if (p->left)
		i= __xuitlvtreenode_serializetlv (p->left, &buffer[ctr], maxbuffer-ctr);
	if (i<0)
		return i;
	if (!p->parent)//root
		__xuitlvtreenode_countitemlength(p);
	if (p->childlen > (maxbuffer - i))
		return CTLV_UNSUFFICIENTBUFFSIZE;
	ctr+=i;
	
	if (p->parent)
	{	
		taglen = __xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
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
	else if (p->head)
	{
		i = __xuitlvtreenode_serializetlv (p->head, &buffer[ctr], maxbuffer-ctr);
		if (i < 0)
			return i;
		ctr+=i; 
	}
	i = 0;	
	if (p->right)
		i= __xuitlvtreenode_serializetlv (p->right, &buffer[ctr], maxbuffer-ctr);
	if (i<0)
		return i;
	ctr+=i;
	return ctr;
}
/**
 * Name		: __xuitlvtreenode_drywritetlv
 * Description	: do step needed to peform writting data, but do not printf
 * Input
 *	p	: pointer to item for data to write
 **/
static int __xuitlvtreenode_drywritetlv (PXUITLVTREENODE p)
{ 
	unsigned char lenTlv[32];

	if (p->left)
		__xuitlvtreenode_drywritetlv (p->left);
	
	if (p->parent)
	{	
		__xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memset (lenTlv, 0, 32); 
		setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
	}
	if (p->head)
	{
		__xuitlvtreenode_drywritetlv (p->head);	
	}
	
	if (p->right)
		__xuitlvtreenode_drywritetlv (p->right);
	return 0;
}

/**
 * Name		: __xuitlvtreenode_getvalue
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
 *	CTLV_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CTLV_INVALID_GETSET_OFFSET	: negativ offset
 *	CTLV_INVALID_GETSET_LENGTH	: negativ length
 *	0				: success		
 **/
static int __xuitlvtreenode_getvalue (PXUITLVTREENODE p, unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	int64_t maxsize = 0; 
	if (p->tag[0] & 0x20)
		return CTLV_NOPRIMITIVETAG;
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (len < 0)
		return CTLV_INVALIDGETSETLENGTH;
	if (len == 0)
	{
		memcpy (buffer, &p->value[offset], p->valuelen - offset);
		*copied = p->valuelen - offset;
		return 0;
	}
	maxsize = (p->valuelen - offset) > len?len: (p->valuelen -offset);
	if (maxsize <= 0) 
		return CTLV_OFFSETEXCEEDVALLENGTH;
	memcpy (buffer, &p->value[offset], maxsize);
	*copied = maxsize;
	return 0;
}

/**
 * Name		: __xuitlvtreenode_gettagvalue
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
 *	CTLV_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CTLV_INVALID_GETSET_OFFSET	: negative offset
 *	CTLV_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 **/
static int __xuitlvtreenode_gettagvalue (PXUITLVTREENODE root, unsigned char *tag, int maxtaglen,
	unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	
	unsigned char *k = tag;
	int taglen = 0; 
	int xmaxtaglen = maxtaglen - taglen;
	PXUITLVTREENODE child; 
	do
	{
		if (! (k[0]&0x20))
			break;
		k = &k[taglen];
		xmaxtaglen -= taglen;
	}while ((taglen = __xui_getNextTag(k, xmaxtaglen)) >0);
	if ((tag[0]&0x20) && (!taglen))
		return CTLV_NOPRIMITIVETAG;
	child = searchnodetag (root->head, tag);
	taglen = __xui_getNextTag(tag, maxtaglen);
	if (!child)
		return CTLV_TAGNOTFOUND;
	if ((tag[0] &0x20))
		return __xuitlvtreenode_gettagvalue(child, &tag[taglen], maxtaglen -taglen,
			buffer, offset, len, copied);
	return __xuitlvtreenode_getvalue (child, buffer, offset, len, copied);
}
/**
 * Name		: __xuitlvtreenode_initialize
 * Description	: reset variables
 **/
static int __xuitlvtreenode_initialize(PXUITLVTREENODE node, PXUITLVTREENODE parent, unsigned char *tag)
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
	node->head  = NULL;
	return 0;

}
/**
 * Name		: initializexuitlvtreenode
 * Description	: initialize this tree node
 *
 **/
int initializexuitlvtreenode (PXUITLVTREENODE node, PXUITLVTREENODE parent, unsigned char *tag)
{ 
	__xuitlvtreenode_initialize( node,  parent, tag);
	node->comp.add = (int(*)(PTREECOMPONENT, unsigned char *, int, int))__xuitlvtreenode_add;
	node->comp.del = (int(*)(PTREECOMPONENT))__xuitlvtreenode_delete;
	node->comp.deletetag = 
		(int(*)(PTREECOMPONENT, unsigned char *, int))__xuitlvtreenode_deletetag;
	node->comp.updatevalue = 
		(int(*)(PTREECOMPONENT, unsigned char *,int64_t, int64_t))__xuitlvtreenode_updatevalue;
	node->comp.updatetagvalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int, unsigned char *, int64_t, int64_t))__xuitlvtreenode_updatetagvalue;
	node->comp.countitemlength = (int64_t(*)(PTREECOMPONENT))__xuitlvtreenode_countitemlength;
	node->comp.writetlv = (int(*)(PTREECOMPONENT))__xuitlvtreenode_writetlv;
	node->comp.drywritetlv = (int(*)(PTREECOMPONENT))__xuitlvtreenode_drywritetlv;
	node->comp.serializetlv = (int(*)(PTREECOMPONENT, unsigned char*, int))__xuitlvtreenode_serializetlv;
	node->comp.getvalue =(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t, int64_t*)) __xuitlvtreenode_getvalue;
	node->comp.gettagvalue = 
		(int(*)(PTREECOMPONENT,unsigned char *, int, unsigned char *, int64_t, int64_t, int64_t*))__xuitlvtreenode_gettagvalue;
	node->comp.initialize = (int(*)(PTREECOMPONENT, PTREECOMPONENT, unsigned char *))__xuitlvtreenode_initialize;
	return 0;
}

/**
 * Name		: newxuitlvtreenode
 * Description	: create a new xuitlvtreenode
 * Input
 *	parent	: parent of this item
 *	tag	: tag of this item
 **/
PXUITLVTREENODE newxuitlvtreenode (PXUITLVTREENODE parent, unsigned char *tag)
{
	PXUITLVTREENODE newnode = NULL; 
	newnode = (PXUITLVTREENODE) calloc (1, sizeof (XUITLVTREENODE));
	if (!newnode)
		return newnode;
	__xui_getNextTag (tag, MAX_SINGLE_TAGLEN);
	initializexuitlvtreenode (newnode, parent, tag);
	return newnode;
}

/**
 * Name		: newrootxuitlvtreenode
 * Description	: alloc memory for new item, set all necessary functions and variables 
 **/
PXUITLVTREENODE newrootxuitlvtreenode ()
{
	PXUITLVTREENODE newItem = NULL;
	unsigned char tag[] = {0,0}; 
	newItem = (PXUITLVTREENODE) calloc (1, sizeof (XUITLVTREENODE));
	if (!newItem)
		return newItem; 
	initializexuitlvtreenode (newItem, NULL, tag);
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
		BtoH (tagStr, tag, __xui_getNextTag(tag, MAX_SINGLE_TAGLEN));
	}
	fprintf (stdout, "<%s>", tagStr);
	fprintf (stdout, "\n");	
	return 0;
}
extern PTLVTREEITEM newtlvtreeitem (PTLVTREEITEM parent, unsigned char * tag);
/**
 * Name		: __tlvtreeitem_add
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
static int __tlvtreeitem_add(PTLVTREEITEM root, unsigned char *in, int maxtaglen, int valsize)
{
	int taglen = 0;
	int nres = 0;
	PTLVTREEITEM newItem;
	PTLVTREEITEM ptrCurr = NULL, ptrPrev = NULL; 
	unsigned char *k;
	if (!root)
		return CTLV_NULLROOT;
	taglen = __xui_getNextTag (in, maxtaglen);
	if (taglen <= 0)
		return taglen;/*No more tag in supplied "in" param*/

	k = &in[taglen];
	if (root->head == NULL)
	{
		newItem = newtlvtreeitem (root, in);
		if (newItem == NULL)
			return CTLV_OUTOFMEMORY;
		root->head = root->tail = root->curr= newItem; 
		root->count ++;
		return __tlvtreeitem_add (newItem, k, maxtaglen - taglen, valsize); 
	}
	//find item , check if exists.
	//if it is then recurse to this item
	//otherwise allocate new item 
	ptrPrev = NULL;
	ptrCurr = root->head;
	while ( ptrCurr && ((nres = __xui_tagCmp (ptrCurr->tag, in)) <0))
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (!ptrCurr)
	{ 
		newItem = newtlvtreeitem (root, in);
		if (newItem == NULL)
			return CTLV_OUTOFMEMORY;
		if (!root->head) 
			root->head = newItem;
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
					return CTLV_OUTOFMEMORY;
				} 
				newItem->valuelen = valsize; 
			} 
		}
		return __tlvtreeitem_add (newItem, k, maxtaglen - taglen, valsize);
	}
	if (nres ==  0) 
		return __tlvtreeitem_add (ptrCurr, k, maxtaglen - taglen, valsize);

	newItem = newtlvtreeitem (root, in);
	if (newItem == NULL)
		return CTLV_OUTOFMEMORY;
	if (ptrPrev)
	{
		ptrPrev->next = newItem;
		newItem->next = ptrCurr; 
	}
	else
	{//we re preceding head
		newItem->next = ptrCurr;
		root->head = newItem;
	}
	root->count ++;
	return __tlvtreeitem_add (newItem, k, maxtaglen - taglen, valsize); 
	//found or in the end of list 

}
/**
 * Name		: __tlvtreeitem_delete
 * Description	: delete current treeitem, includes data and its child
 * Input	
 *	toDelete : item to delete
 *
 **/
static int __tlvtreeitem_delete (PTLVTREEITEM toDelete)
{
	PTLVTREEITEM ptrCurr;
	if (toDelete->value != NULL)
	{
		free(toDelete->value); 
		toDelete->value = NULL;
	}
	toDelete->valuelen = 0;

	ptrCurr = toDelete->head;
	while (ptrCurr)
	{
		toDelete->head = ptrCurr->next;
		__tlvtreeitem_delete (ptrCurr);
		ptrCurr = toDelete->head;
		toDelete->count --; 
	}
	toDelete->head = toDelete->tail = toDelete->curr = NULL;
	toDelete->parent = NULL;
	memset (toDelete->tag, 0, MAX_SINGLE_TAGLEN); 
	free (toDelete);
	return 0;
}

/**
 * Name		: __tlvtreeitem_deletetag
 * Description	: delete tree item of this tag, deleted tag is the last tag
 *		  in the hierarchy. matching tag string done to child list. 
 **/
static int __tlvtreeitem_deletetag (PTLVTREEITEM root, unsigned char *tag, int maxtaglen)
{
	//getnexttlvtag 
	unsigned char *k;
	PTLVTREEITEM ptrPrev, ptrCurr;
	int taglen1 = 0;
	int taglen2 = 0;
	int nres = 0;
	taglen1 = __xui_getNextTag(tag, maxtaglen);
	if (taglen1 <= 0)
		return CTLV_NOVALIDTAG;
	taglen2 = __xui_getNextTag(&tag[taglen1], maxtaglen - taglen1);
	if (taglen2 <0)
		return CTLV_NOVALIDTAG;
	k = &tag[taglen1];
	ptrPrev = NULL;
	ptrCurr = root->head;
	if (!ptrCurr)
		return CTLV_TAGNOTFOUND;
	while (( (nres = __xui_tagCmp (ptrCurr->tag, tag)) <0)
		&& ptrCurr!=NULL)
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (nres != 0)
	{
		return CTLV_TAGNOTFOUND;		
	}
	if (!ptrCurr)
		return CTLV_TAGNOTFOUND;
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
			{//we're deleting both head and tail
				root->head = root->tail = ptrCurr->next;	
			}
		}
		else
		{
		
			if (ptrPrev)
				ptrPrev->next = ptrCurr->next;
			else
			//we're deleting head
				root->head  = ptrCurr->next; 
		}
		__tlvtreeitem_delete(ptrCurr);
		return 0;
	}
	// there delete the child
	return __tlvtreeitem_deletetag (ptrCurr, k, maxtaglen - taglen1);
}
/**
 * Name		: __tlvtreeitem_updatevalue
 * Description	: update tlv value of current item. it is a copy instead of pointer assignment.  
 * Input 
 * 	ptr	: current item
 *	data	: new data
 *	offset	: offset to start fill this data to in ptr's value
 *	length	: new length 
 * Returns	:
 *	CTLV_OUTOFMEMORY 		: out of memory
 *	CTLV_INVALID_GETSET_OFFSET	: negative offset
 *	CTLV_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 *
 **/

static int __tlvtreeitem_updatevalue (PTLVTREEITEM ptr, unsigned char *data, int64_t offset, int64_t length)
{
	//printTlvRoutine ("updatevalue", ptr->tag);
	int64_t totallength  = offset+length;
	if (ptr->value == NULL)
	{
		ptr->value = (unsigned char *) calloc (1, totallength);
		if (!ptr->value)
			return CTLV_OUTOFMEMORY;
		memcpy (&ptr->value[offset], data, length);
		ptr->valuelen = totallength;
		return 0; 
	} 
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (length < 0)
		return CTLV_INVALIDGETSETLENGTH;
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
			return CTLV_OUTOFMEMORY;
		ptr->valuelen = totallength;
		memcpy (&ptr->value[offset], data, length); 
		return 0; 
	}
	return 0;
}
/**
 * Name		: __tlvtreeitem_updatetagvalue
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
 *	CTLV_OUTOFMEMORY 		: out of memory
 *	CTLV_NOPRIMITIVETAG		: no primitive tag here
 *	CTLV_INVALID_GETSET_OFFSET	: negative offset
 *	CTLV_INVALID_GETSET_LENGTH	: negative length
 *	0				: success		
 *
 **/


static int __tlvtreeitem_updatetagvalue (PTLVTREEITEM root, unsigned char *in,int maxtaglen ,unsigned char *data, int64_t offset, int64_t length)
{
	int taglen = 0;
	int nres = 0;
	PTLVTREEITEM newItem = NULL;
	PTLVTREEITEM ptrCurr = NULL, ptrPrev = NULL;
	unsigned char *k; /*next tag*/
	if (!root)
		return CTLV_NULLROOT;
	taglen = __xui_getNextTag (in,maxtaglen);
	if (taglen <= 0)
	{
		if (! (root->tag[0]&0x20)) 
		{ 
			return __tlvtreeitem_updatevalue (root, data, offset, length); 

		}
		return CTLV_NOPRIMITIVETAG;
	} 
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (length < 0)
		return CTLV_INVALIDGETSETLENGTH;
	//printTlvRoutine ("updatetagvalue", in);
	k = &in[taglen]; 
	ptrCurr = root->head;
	while (ptrCurr && ( (nres = __xui_tagCmp (ptrCurr->tag, in)) < 0))
	{
		ptrPrev = ptrCurr;
		ptrCurr = ptrCurr->next;
	}
	if (!ptrCurr)
	{
		newItem = newtlvtreeitem (root, in);
		if (newItem == NULL)
			return CTLV_OUTOFMEMORY;
		if (!root->head)
			root->head = newItem;
		else
			root->tail->next = newItem;
		root->tail = newItem;
		root->count ++;
		return __tlvtreeitem_updatetagvalue (newItem, k, maxtaglen - taglen, data, offset, length);
	}
	if (nres == 0)
		return __tlvtreeitem_updatetagvalue (ptrCurr, k, maxtaglen - taglen, data, offset, length);
	newItem = newtlvtreeitem (root, in);
	if (newItem == NULL)
		return CTLV_OUTOFMEMORY;
	if (ptrPrev)
	{
		ptrPrev->next = newItem;
		newItem->next = ptrCurr;
	}
	else
	{
		newItem->next = ptrCurr;
		root->head = newItem;
	}
	root->count ++;
	return __tlvtreeitem_updatetagvalue (newItem, k, maxtaglen - taglen, data, offset, length); 
} 
/**
 * Name		: __tlvtreeitem_countitemlength
 * Description	: count all child length and save it in childlen
 * Input
 *	p	: pointer to item to count
 * Returns	: childlen + taglen + valuelen
 **/
int64_t __tlvtreeitem_countitemlength (PTLVTREEITEM p)
{
	int64_t ret = 0;
	int taglen = __xui_getNextTag(p->tag,MAX_SINGLE_TAGLEN);
	PTLVTREEITEM child = p->head; 
	while (child)
	{
		ret+= __tlvtreeitem_countitemlength (child);
		child = child->next;
	}
	p->childlen = ret;
	return ret + taglen +p->valuelen+ calculateTlvLengthUsed (p->childlen + p->valuelen);
	
	
}
/**
 * Name		: __tlvtreeitem_writetlv
 * Description	: write to stdout tree data and child data
 * Input
 *	p	: pointer to item to write data
 *
 **/
static int __tlvtreeitem_writetlv (PTLVTREEITEM p)
{ 
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	PTLVTREEITEM child = 0;
	
	if (p->parent)
	{	
		taglen = __xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
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
	child = p->head;
	while (child)
	{
		__tlvtreeitem_writetlv (child);
		child = child->next;
	}
	return 0; 
}
/**
 * Name		: __tlvtreeitem_serializetlv
 * Description	: serializing tree into stream. if this is the root, then it will
 *		  first call countitemlength.
 * Input
 *	p	: pointer to tlvitem to serialize
 *	buffer	: where to put serialize data
 *	maxbuffer : maximum buffer size
 * Returns	: actual bytes wrtitten to buffer if success
 *		  if buffer size insufficient it will return CTLV_INSUFFICIENTBUFFSZ
 *
 **/
static int __tlvtreeitem_serializetlv (PTLVTREEITEM p, unsigned char *buffer, int maxbuffer)
{ 
	unsigned char lenTlv[32]; 
	int lenTLVLen = 0;
	int taglen = 0;
	int i = 0;
	int ctr = 0;
	PTLVTREEITEM child = 0; 
	if (!p->parent)//root
		__tlvtreeitem_countitemlength(p);
	if (p->childlen > maxbuffer)
		return CTLV_UNSUFFICIENTBUFFSIZE;
	if (p->parent)
	{	
		taglen = __xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
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
	child = p->head;
	while (child)
	{
		i = __tlvtreeitem_serializetlv (child, &buffer[ctr], maxbuffer-ctr);
		ctr+=i;
		child = child->next;
	}
	return ctr;
	
}

/**
 * Name		: __tlvtreeitem_drywritetlv
 * Description	: do step need to perform on writting data, but do not printf
 * Input
 *	p	: pointer to item to write data
 *
 **/
static int __tlvtreeitem_drywritetlv (PTLVTREEITEM p)
{ 
	unsigned char lenTlv[32]; 
	
	PTLVTREEITEM child = 0;
	
	if (p->parent)
	{	
		__xui_getNextTag(p->tag, MAX_SINGLE_TAGLEN);
		memset (lenTlv, 0, 32); 
		setTlvStreamDataLength (p->childlen+p->valuelen, lenTlv, 32);
	}
	if (p->value)
	{
		return 0;
	}
	child = p->head;
	while (child)
	{
		__tlvtreeitem_drywritetlv (child);
		child = child->next;
	}
	return 0; 
}

/**
 * Name		: __tlvtreeitem_getvalue
 * Description	: get value of current item, copy to specified output buffer
 * Input 
 *	p	: node of this value
 *	buffer	: data to copy from
 *	offset	: destination offset in node value
 *	len	: length of data to copy. if len is zero then data from offset to valuelen will be copied
 * Output
 *	copied	: number of data actually copied
 * Returns	:
 *	CTLV_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length 
 *	CTLV_INVALID_GETSET_OFFSET	: negativ offset
 *	CTLV_INVALID_GETSET_LENGTH	: negativ length
 *	0				: success		
 *
 **/
static int __tlvtreeitem_getvalue (PTLVTREEITEM p, unsigned char *buffer,int64_t offset,  int64_t len, int64_t *copied)
{
	int64_t maxsize;
	if (p->tag[0] & 0x20)
		return CTLV_NOPRIMITIVETAG;
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (len < 0)
		return CTLV_INVALIDGETSETLENGTH; 
	if (len == 0)
	{
		memcpy (buffer, &p->value[offset], p->valuelen - offset);
		*copied = p->valuelen - offset;
		return 0;
	}
	maxsize = (p->valuelen - offset) > len ?len : (p->valuelen - offset);
	if (maxsize <=0)
		return CTLV_OFFSETEXCEEDVALLENGTH;
	memcpy (buffer, &p->value[offset], maxsize);
	*copied = maxsize;
	return 0;

}
/**
 * Name		: __tlvtreeitem_gettagvalue
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
 *	CTLV_NOPRIMITIVETAG 		: no primitive in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH	: if offset exceeds node value length
 *	CTLV_INVALID_GETSET_OFFSET	: negativ offset
 *	CTLV_INVALID_GETSET_LENGTH	: negativ length
 *	0				: success		
 **/
static int __tlvtreeitem_gettagvalue (PTLVTREEITEM root, unsigned char *tag, int maxtaglen,
	unsigned char *buffer, int64_t offset, int64_t len, int64_t *copied)
{
	unsigned char *k = tag;
	int taglen = 0;
	int res = 1;
	int xmaxtaglen = maxtaglen - taglen;
	PTLVTREEITEM child;

	do
	{
		if (! (k[0]&0x20))
			break;
		k = &k[taglen];
		xmaxtaglen -= taglen;
	}while ((taglen = __xui_getNextTag(k, xmaxtaglen))>0);
	if ((tag[0]&0x20) && (!taglen))
		return CTLV_NOPRIMITIVETAG;
	if (offset < 0)
		return CTLV_INVALIDGETSETOFFSET;
	if (len < 0)
		return CTLV_INVALIDGETSETLENGTH;
	child = root->head;
	while( child && ((res = __xui_tagCmp (child->tag, tag)) <0)) 
	{
		child = child->next;
	} 
	taglen = __xui_getNextTag(tag, maxtaglen);
	if (!child)
		return CTLV_TAGNOTFOUND;
	if (res)
		return CTLV_TAGNOTFOUND;
	if ((tag[0]&0x20))
	{
		return __tlvtreeitem_gettagvalue (child, &tag[taglen], maxtaglen - taglen, 
			buffer,offset, len, copied);
	}/*it is supposed to be our child*/
	return __tlvtreeitem_getvalue (child, buffer, offset, len, copied);
}

static int __tlvtreeitem_initialize(PTLVTREEITEM newItem, PTLVTREEITEM parent, unsigned char *tag)
{

	int len = 0; 
	PTLVTREEITEM child = newItem->head;
	if (tag!=NULL)
	{
		len = __xui_getNextTag(tag, MAX_SINGLE_TAGLEN);
		if (len) 
			memcpy (newItem->tag, tag, len>MAX_SINGLE_TAGLEN?MAX_SINGLE_TAGLEN:len);
	}
	if (parent)
		newItem->parent = parent;
	newItem->next = NULL;
	/*delete all child*/
	while (child)
	{
		newItem->head = child->next;
		__tlvtreeitem_delete (child);
		child = newItem->head;
	}	
	newItem->value = NULL;
	newItem->valuelen = 0;
	newItem->count = 0;
	newItem->childlen = 0;
	newItem->head = newItem->tail = newItem->curr = NULL; 
	return 0;
}
/**
 * Name		: inittlvtreeitem
 * Description	: initialize this item
 **/ 
int initializetlvtreeitem (PTLVTREEITEM newItem, PTLVTREEITEM parent,unsigned char *tag)
{
	__tlvtreeitem_initialize(newItem, parent, tag);
	newItem->comp.add = (int(*)(PTREECOMPONENT, unsigned char *, int, int))__tlvtreeitem_add;
	newItem->comp.del = (int(*)(PTREECOMPONENT))__tlvtreeitem_delete;
	newItem->comp.deletetag = 
		(int(*)(PTREECOMPONENT, unsigned char *, int))__tlvtreeitem_deletetag;
	newItem->comp.updatevalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t))__tlvtreeitem_updatevalue;
	newItem->comp.updatetagvalue = 
		(int(*)(PTREECOMPONENT, unsigned char *, int, unsigned char *, int64_t, int64_t))__tlvtreeitem_updatetagvalue;
	newItem->comp.countitemlength = (int64_t(*)(PTREECOMPONENT))__tlvtreeitem_countitemlength;
	newItem->comp.writetlv = (int(*)(PTREECOMPONENT))__tlvtreeitem_writetlv;
	newItem->comp.drywritetlv = (int(*)(PTREECOMPONENT))__tlvtreeitem_drywritetlv;
	newItem->comp.serializetlv = (int(*)(PTREECOMPONENT, unsigned char*, int))__tlvtreeitem_serializetlv;
	newItem->comp.getvalue =(int(*)(PTREECOMPONENT, unsigned char *, int64_t, int64_t, int64_t*)) __tlvtreeitem_getvalue;
	newItem->comp.gettagvalue = 
		(int(*)(PTREECOMPONENT,unsigned char *, int, unsigned char *, int64_t, int64_t, int64_t*))__tlvtreeitem_gettagvalue;
	newItem->comp.initialize = (int(*)(PTREECOMPONENT, PTREECOMPONENT, unsigned char *))__tlvtreeitem_initialize;
	return 0; 
}

/**
 * Name		: newtlvtreeitem
 * Description	: alloc memory for new item, set all necessary functions and variables
 * Input
 *	parent	: parent to this new item
 *	tag	: tag for this new item
 *		  it will cut the tag until unexpanded tag is found
 **/
PTLVTREEITEM newtlvtreeitem (PTLVTREEITEM parent, unsigned char *tag)
{
	PTLVTREEITEM newItem = NULL; 
	newItem = (PTLVTREEITEM) calloc (1, sizeof (TLVTREEITEM));
	if (!newItem)
		return newItem;
	__xui_getNextTag(tag, MAX_SINGLE_TAGLEN);
	initializetlvtreeitem (newItem, parent, tag);
	return newItem;	
}
/**
 * Name		: newroottlvtreeitem
 * Description	: alloc memory for new item, set all necessary functions and variables 
 **/
PTLVTREEITEM newroottlvtreeitem ()
{
	PTLVTREEITEM newItem = NULL;
	unsigned char tag[] = {0,0}; 
	newItem = (PTLVTREEITEM) calloc (1, sizeof (TLVTREEITEM));
	if (!newItem)
		return newItem; 
	initializetlvtreeitem (newItem, NULL, tag);
	return newItem;	
} 


/**
 * Name 	: __tlvparser_setdata
 * Description	: set data of specified tag sequence
 * Input
 *	p	: pointer to parser to set data to
 *	data	: data to set
 *	datalen : length of data
 **/
static int __tlvparser_setData (PCXTLV p, unsigned  char *data, int datalen)
{
	unsigned char y[_MAX_Y_DATA];
	unsigned char *pdata; 
	int ret = 0;
	if (p->stack.accumtaglen > _MAX_Y_DATA)
	{
		pdata = (unsigned char *) calloc(1,p->stack.accumtaglen);
		if(!pdata)
			return CTLV_OUTOFMEMORY;
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
 * Name		: __tlvparser_parseTlv
 * Description	: parse supplied streams into tag length and value and fill
 *		  out our tree
 */
int64_t __tlvparser_parseTlv (PCXTLV p, unsigned char *stream, int64_t inlength)
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
		return CTLV_INVALIDSTREAMLENGTH;
	}
	taglen = __xui_getNextTag (pStream, inlength);
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
			return CTLV_INVALIDTLVSTREAMLENGTH;
		}
		p->stack.push (&p->stack, pStream, taglen);
		toProc = &pStream[taglen+ length_of_length];
		accumlength += taglen + length_of_length;

		if(structure)
		{
			nret = __tlvparser_parseTlv (p, toProc, datalength);
			if (nret >= 0) 
				accumlength += nret; 
			else
			{ 
				cont = 0; 
			}
		}
		else
		{
			nret = __tlvparser_setData(p, toProc, datalength);
			if (nret < 0)
				return nret;
			accumlength += datalength;
			if (accumlength > inlength) 
				cont = 0;	

		}
		if (inlength <= accumlength) 
			cont = 0;	

		p->stack.pop(&p->stack);
		if (cont)
		{
			pStream = stream + accumlength;
			taglen = __xui_getNextTag(pStream, inlength - accumlength); 
		}
	}while((taglen > 0) && cont); 
	return accumlength; 
}
/**
 * Name		: __tlvparser_serialize
 * Description	: serialize tree into tlv stream
 * Input
 * 	p	: object that holds tlv tree
 * 	stream	: output stream
 * 	maxbuffer : buffer size
 */
static int __tlvparser_serialize (PCXTLV p, unsigned char *stream, int maxbuffer)
{
	if (!p->root)
		return 0;
	return p->root->serializetlv (p->root, stream, maxbuffer);
}
/**
 * Name		: __tlvparser_cleanup
 * Description	: cleanup tlv tree 
 */
static int __tlvparser_cleanup (PCXTLV p)
{
	if (!p->root)
		return 0;
	return p->root->initialize (p->root, NULL, NULL);
}
/**
 * Name		: __tlvparser_deserialize
 * Description	: transform stream into tree structure
 * Input
 *	p	: object that will store this structure
 *	stream	: tlv stream
 *	inlength: length of this stream
 **/
int __tlvparser_deserialize (PCXTLV p, unsigned char *stream, int inlength)
{
	p->stack.init(&p->stack); 
	return __tlvparser_parseTlv (p, stream, inlength);
}
/**
 * Name		: __tlvparser_delete
 * Description	: delete specified tag/tag string from tlv tree 
 **/
static int __tlvparser_delete (PCXTLV p, unsigned char *tag)
{
	return p->root->deletetag (p->root, tag, MAX_TAGSTRING_LEN);

}
/**
 * Name		: __tlvparser_setdatafixedl
 * Description	: copy data to destination tag, start in offset 0 up to len
 * Input
 * 	p	: tlvparser
 *	tag	: tag string to set value to
 *	data	: data to copy from
 *	len	: length of data to copy
 * Returns
 *	CTLV_NULLROOT	: if there has been no tree component
 *	CTLV_NOPRIMITIVETAG : if no primitiva tag in tag string supplied
 *	CTLV_OUTOFMEMORY : fail to allocate memory
 *	0	: success
 **/
static int __tlvparser_setdatafixedl (PCXTLV p, unsigned char *tag, unsigned char *data, int len)
{
	if (!p)
		return CTLV_NULLROOT;
	return p->root->updatetagvalue (p->root, tag, MAX_TAGSTRING_LEN,data, 0, len);
}
/**
 * Name		: __tlvparser_setdatavarl
 * Description	: copy data to destination tag, start in offset up to len 
 * Input
 * 	p	: tlvparser
 *	tag	: tag string to set value to
 *	data	: data to copy from
 *	offset	: offset in destination where to start copy data to
 *	len	: length to data to copy
 * Returns
 *	CTLV_NULLROOT	: if there has been no tree component
 *	CTLV_NOPRIMITIVETAG : if no primitiva tag in tag string supplied
 *	CTLV_OUTOFMEMORY : fail to allocate memory
 *	0	: success
 **/
static int __tlvparser_setdatavarl (PCXTLV p, unsigned char *tag, unsigned char *data, int offset, int len)
{
	if (!p)
		return CTLV_NULLROOT;
	return p->root->updatetagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, offset, len);
}

/**
 * Name		: __tlvparser_getdatafixedl
 * Description	: get data from tree , start from offset zero, up to len. 
 * Input
 *	p	: tlvparser
 *	tag	: tag string to get value from
 *	data	: buffer to store retrieved value
 *	length	: length of value to retrieve 
 * Returns 
 *	CTLV_NOPRIMITIVETAG		: no primitive tag in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH 	: offset exceeds tree value length
 *	0				: success
 *
 **/
static int __tlvparser_getdatafixedl (PCXTLV p, unsigned char * tag, unsigned char * data, int length)
{
	int64_t outlength;
	if (!p)
		return CTLV_NULLROOT;
	return p->root->gettagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, 0, length, &outlength);
}

/**
 * Name		: __tlvparser_getdatavarl
 * Description	: get data from tree, start from offset, up to len 
 * Input
 *	p	: tlvparser
 *	tag	: tag string to get value from
 *	data	: buffer to store retrieved value
 *	offset	: start offset of value to copy from
 *	length	: length of value to retrieve 
 * Output
 *	
 * Returns 
 *	CTLV_NOPRIMITIVETAG		: no primitive tag in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH 	: offset exceeds tree value length
 *	0				: success
 *
 **/
static int __tlvparser_getdatavarl (PCXTLV p, unsigned char * tag, unsigned char *data, int offset, int length, int *copied)
{
	int64_t x;
	int ret = 0;
	if (!p)
		return CTLV_NULLROOT;
	ret =  p->root->gettagvalue (p->root, tag, MAX_TAGSTRING_LEN, data, offset, length, &x);
	if (!ret)
		*copied  = x;
	return ret; 

}
int initializetlvparser (PCXTLV p)
{ 
	initStack(&p->stack); 
	p->Deserialize = __tlvparser_deserialize;
	p->Serialize = __tlvparser_serialize;
	p->Cleanup = __tlvparser_cleanup;
	p->Delete = __tlvparser_delete;
	p->GetDataFixedL = __tlvparser_getdatafixedl;
	p->GetDataVarL = __tlvparser_getdatavarl;
	p->SetDataFixedL = __tlvparser_setdatafixedl;
	p->SetDataVarL = __tlvparser_setdatavarl;
	return 0;
}
int InitCXTlvParser (PCXTLV p)
{
	return initializetlvparser(p);
}
/**
 * Name		: newcxtlv
 * Description	: new cxtlv with tree is comp
 **/
PCXTLV newcxtlv(PTREECOMPONENT comp)
{
	PCXTLV p = (PCXTLV) calloc (1, sizeof (CXTLV));
	if (!p)
		return p;
	p->root = comp;
	initializetlvparser (p);
	return p;
}

/**
 * Name		: newctlvbtree
 * Description	: create new cxtlv that uses hierachical binary tress structure to store tlv
 **/
PCXTLV newcxtlvbtree()
{
	PXUITLVTREENODE root = newrootxuitlvtreenode ();
	PCXTLV p = (PCXTLV) calloc (1, sizeof (CXTLV));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializetlvparser (p);
	return p;
}


/**
 * Name		: newctlvllist
 * Description	: create new cxtlv that uses hierachical linked list structure to store tlv
 **/
PCXTLV newcxtlvllist()
{
	PTLVTREEITEM root = newroottlvtreeitem();
	PCXTLV p = (PCXTLV) calloc (1, sizeof (CXTLV));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializetlvparser (p);
	return p;
}

/**
 * Name		: newdefaultcxtlv
 * Description	: create new cxtlv that uses hierachical linked list structure to store tlv
 **/
PCXTLV NewDefaultCXTlv()
{
	PTLVTREEITEM root = newroottlvtreeitem();
	PCXTLV p = (PCXTLV) calloc (1, sizeof (CXTLV));
	if (!p)
	{
		free (root);
		return p;
	}
	p->root = (PTREECOMPONENT)root;
	initializetlvparser (p);
	return p;
}

