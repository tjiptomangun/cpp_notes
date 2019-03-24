/**
 * array tlv
 * tlv as static array
 * $CC -Wall -ggdb3 -lrt arraytlv.c -o arraytlv
 * shared memory in unix/linux will be stored in /dev/shm
 **/
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <regex.h>
#include "arraytlv.h"

//init shm
//extract tlv
//build tlv
//structure and data

int maxShm = 1000;
unsigned char *pIpcPoolBitmap;


void HtoB2 (unsigned char * pDst, char *pSrcHex, int srcLen)
{
	int i = 0;
	unsigned char temp;
	char *currSrc = pSrcHex;
	unsigned char *currDst = pDst;
	do 
	{
		*currDst = 0;
		for (i = 0; i < 2; i++)
		{
			*currDst<<=4;
		temp = *currSrc;
			temp &= 0x4F;
			*currDst += (temp >> 6)*10 + (temp & 0xF) - (temp >> 6); 
			currSrc ++;
			srcLen --;
		}
		currDst ++; 
	} while (srcLen);
}

typedef struct treeitem
{
	int idx;
	int len;
	int type; //primitive or mempool
} TREEITEM, *PTREEITEM;
typedef struct xim_header
{
	pid_t currentPID;
	time_t timestamp;
} XIM_HEADER;
/**
 * tree idx 0, 1, 2, 3 ,4 is respectvely reserved for
 * stan, mtype, mclass, ttype, tclass.
 * while in local storage these tags will keep intact.
 * only when the transmitted to another node then they
 * will be assigned to tlv.
 **/
typedef struct myxim
{
	XIM_HEADER Header;
	unsigned int	index;
	unsigned int	stan;
	unsigned int 	mtype;
	unsigned int	mclass;
	unsigned int	ttype;
	unsigned int	tclass; 
	int		dataidx;
	TREEITEM	tree[MAX_ARRAY];
	unsigned char 	data[1];
} MYXIM; 
MYXIM* shm;

/**
 * Name		: __xim_update_data
 * Description	: update data of xim in treeidx.
 *		  if data already exists and old_data_len > length
 *		  copy new data to treeidx.idx, update 
 *		  treeidx.len = length.
 *		  if old_data_len < length go to next free dataidx
 *		  update treeimteidx.idx and treeidx.len
 **/
int __xim_tree_update (MYXIM *pxim, int treeidx, 
		unsigned char *data, int length)
{
	TREEITEM *pximtree ;
	pximtree =  &pxim->tree[treeidx];
	pximtree->len = length;
	if ((pximtree->idx>=0) && (pximtree->len >= length))
	{ 
		memcpy (&pxim->data[pximtree->idx], data, length);
		return 0; 
	}
	pximtree->idx = pxim->dataidx; 
	memcpy (&pxim->data[pximtree->idx], data, length); 
	pxim->dataidx+=length; 
	return 0;
}
/**
 * Name		: __xim_get_data
 * Description	: get data of pxim tree in treeidx
 **/
int __xim_tree_get (MYXIM *pxim, int treeidx, 
		unsigned char *dst, int maxlen, int *copied)
{
	TREEITEM *pximtree = &pxim->tree[treeidx];
	if (pximtree->idx < 0)
	{
		*copied = 0;
		return 0;
	}
	else if (maxlen < pximtree->len)
	{
		*copied = maxlen;
	}
	else
	{
		*copied = pximtree->len;
	}
	memcpy (dst, &pxim->data[pximtree->idx], *copied);
	return 0;
	
}
/**
 * Name		: __xim_delete_data
 * Description	: delete data of pxim tree in treeidx
 **/
int __xim_tree_delete (MYXIM *pxim, int treeidx)
{
	TREEITEM *pximtree = &pxim->tree[treeidx];
	pximtree->idx = -1;
	pximtree->len = 0;
	return 0;
}
/**
 * Name		: update_tag 
 */
int update_tag (int ximidx, char * tag, 
		int len, unsigned char *data, int *calc)
{ 
	MYXIM *pxim ; 
	
	if ((!tag[0]) || (tag[0]<'0') || (tag[0]>'9'))
		return ERR_TAG;
		
	if (tag[1] == 0)	
	{
		pxim = &shm[ximidx]; 
		return __xim_tree_update (pxim, 
			(*calc) + tag[0]-'0', data, len); 

	}
	*calc  = 10 * (*calc) + tag[0] - '0';

	return update_tag (ximidx, tag+1, len, data, calc);
}
int get_tag (int ximidx, char * tag, int *calc, int maxlen, 
		unsigned char *dst, int *copiedlen)
{ 
	MYXIM *pxim;
	if ((!tag[0]) || (tag[0]<'0') || (tag[0]>'9'))
		return ERR_TAG;
		
	if (tag[1] == 0)
	{
		pxim = &shm[ximidx];
		return __xim_tree_get (pxim, (*calc) + tag[0]-'0', 
			dst, maxlen, copiedlen); 
	}
	*calc  = 10 * (*calc) + tag[0] - '0';
	return get_tag (ximidx, tag+1, calc, maxlen, dst,  copiedlen);
}

int delete_tag (int ximidx,  char *tag, int *calc)
{ 
	MYXIM *pxim;
	if ((!tag[0]) || (tag[0]<'0') || (tag[0]>'9'))
		return ERR_TAG;
	if (tag[1] == 0)
	{
		pxim = &shm[ximidx];
		return __xim_tree_delete (pxim, (*calc) + tag[0] - '0');
	}
	*calc  = 10 * (*calc) + tag[0] - '0';
	return delete_tag (ximidx, tag+1, calc);
}

int xim_update (int ximidx, char * tag, int len, unsigned char *data)
{
	int calc = 0;
	return update_tag (ximidx, tag, len, data, &calc);
}

int xim_get (int ximidx, char *tag, int maxlen, unsigned char *dst)
{
	int calc = 0;
	int copiedlen = 0;
	return get_tag (ximidx, tag, &calc, maxlen, dst, &copiedlen);
}

int xim_delete (int ximidx, char *tag)
{
	int calc = 0;
	return delete_tag (ximidx, tag, &calc);
}

int pack_tlv (int ximidx, unsigned char * tlv)
{
	int i = 0;
	MYXIM *pxim = (MYXIM *)shm[idx];
	for (i = 0; i < MAX_ARRAY; i++)
	{
		if (pxim->tree[i].idx<0)
			continue; 
		//serialize here
		if (pxim->tree[i].type)
		{
		}
		else
		{
		}

	}
	return 0;	
}

