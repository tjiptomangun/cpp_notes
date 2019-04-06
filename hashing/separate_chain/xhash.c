/**
 * separate_chain
 * x/link implementation of hash with separate chain
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DATA_TYPE_PRESENT 0
#define DATA_TYPE_REFERENCE 1

typedef struct xhash_context
{
	int	nXIMHeaderSize;
	int	nStructureSize;
	int	nMaxStructures;
	int	nXIMDataOffset;
	int	nXIMDataSize;
	int	nNumHash;
	int	nMaxNodes;
	int	nMaxChains;	
} XHASH_CONTEXT;
typedef struct xhash_header
{
	int nNumStructures;
	int nXIMDataCurrentOffset;
	int nLockingFlag;
	pid_t currentpid;
	time_t timestamp;
}XHASH_HEADER;
typedef struct xhash_node
{
	int key;
	int data_length;
	int data_type;
	int data_index;
} XHASH_NODE
/**
 * Name		: xkey
 * Description  : convert from array of char to integer key
 *
 **/
int xhash_xkey (unsigned char *key, int nKeyLen)
{
	int result = 0;
	int curr = 0;
	int i = 0;
	for (i = 0; i< nKeyLen; i++)
	{
		result*=10;
		curr = key[i] - '0';
		result+=curr;
	}
	return result;
}
/**
 * Name		: max_nodes
 * Description	: count max_nodes for given number max element and number hash keys
 *
 **/
int xhash_max_nodes (int nMaxElement, int nhash_keys)
{
	double k = ((double) nMaxElement / (double)nhash_keys) * ((double)nhash_keys);
	return (int) ceil (k);
}
/**
 * Name		: xhash
 * Description	: count hash of idx given nmax_nodes and nhash_keys
 **/
int xhash_xhash (int idx, int nmax_nodes , int nhash_keys)
{
	return ((int)ceil (idx*nmax_nodes/nhash_keys)) % nmax_nodes;
}
/**
 * Name		: max_chain
 * Description	: count max chain available for given max_nodes and hash_keys
 **/
int xhash_max_chain (int nmax_nodes, int nhash_keys)
{
	return nmax_nodes/nhash_keys;
}

// basic functionalities
// 
int main (int argc, char **argv)
{
	exit (0);
}
