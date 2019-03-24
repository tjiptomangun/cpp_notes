#ifndef __MYXIM_H__

#define __MYXIM_H__

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


extern int init_shm (unsigned char **dest, char *shm_name, int shm_size); 
extern int initblock (); 
extern int deinitblock (); 
#endif
