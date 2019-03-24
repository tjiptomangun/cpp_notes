#ifndef __ARRAYTLV_H__
#define __ARRAYTLV_H__
#define 	MAX_ARRAY	10000
#define 	ERR_TAG		-102
#define 	MAX_DATA	1000000

extern int maxShm;
extern unsigned char *pIpcPoolBitmap;



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
extern MYXIM * shm;

int init_shm (unsigned char **dest, char *shm_name, int shm_size); 
int xim_update (int ximidx, char * tag, int len, unsigned char *data); 
int xim_get (int ximidx, char *tag, int maxlen, unsigned char *dst); 
int xim_delete (int ximidx, char *tag); 
int pack_tlv (int ximidx, unsigned char * tlv); 
int initblock ();
int deinitblock ();
#endif
