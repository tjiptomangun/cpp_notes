#ifndef _CTLV_H_
#define _CTLV_H_

#define CTLV_NOMORETAG		0
#define CTLV_NOVALIDTAG		-1
#define CTLV_OUTOFMEMORY	-2
#define CTLV_NULLROOT		-3
#define CTLV_TAGNOTFOUND	-4
#define CTLV_NOPRIMITIVETAG	-5
#define CTLV_UNSUFFICIENTBUFFSIZE -6
#define CTLV_INVALIDTLVSTREAMLENGTH	-7
#define CTLV_INVALIDSTREAMLENGTH -8
#define CTLV_OFFSETEXCEEDVALLENGTH -9
#define CTLV_INVALIDGETSETOFFSET	-10
#define CTLV_INVALIDGETSETLENGTH	-11 //used for length passed in get and set value

#define MAX_TAG_STACK 20
#define MAX_SINGLE_TAGLEN 10 //max tlvtreeitem single tag length 
#define MAX_TAGSTRING_LEN MAX_TAG_STACK*MAX_SINGLE_TAGLEN //max number of bytes in an array of tag

#define TLVSTREAM_MAX_LENGTHOFLENGTH  sizeof (int64_t)+1 //max size of tlv length
#define _MAX_Y_DATA 40

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Name		: tlvtreecomponent
 * Description	: interface
 **/
typedef struct treecomponent
{ 
	int (*add) (struct treecomponent *, unsigned char *, int, int); /*add to child tree*/
	int (*del) (struct treecomponent *);/*delete current item*/
	int (*deletetag) (struct treecomponent *, unsigned char *, int);/*delete from child tree*/
	int (*updatevalue) (struct treecomponent *, unsigned char *, int64_t, int64_t);/*update current item value*/
	int (*updatetagvalue) (struct treecomponent *, 
		unsigned char *, int, unsigned char *, int64_t, int64_t);/*update child value*/
	int64_t (*countitemlength) (struct treecomponent *);/*count all child length plus current item*/
	int (*writetlv) (struct treecomponent *);/*extract and write to stdout tlv*/
	int (*drywritetlv) (struct treecomponent *); /*perform write preparation only*/
	int (*serializetlv) (struct treecomponent *, unsigned char *, int);/*tree to tlv stream*/
	int (*getvalue)(struct treecomponent *, unsigned char*, int64_t, int64_t, int64_t*);/*get value of current item*/
	int (*gettagvalue)(struct treecomponent *, unsigned char *,int ,
		unsigned char *, int64_t, int64_t, int64_t*);/*update child value*/ 
	int (*initialize)(struct treecomponent *, struct treecomponent*, unsigned char *);/*initialize members*/ 
}TREECOMPONENT, *PTREECOMPONENT; 

typedef struct tagStackItem
{
	unsigned char *tag;
	int	tagLength;
}TAGSTACKITEM, *PTAGSTACKITEM;
typedef struct tagStack
{
	int top;
	TAGSTACKITEM c[MAX_TAG_STACK];
	int (* init)(struct tagStack*);
	int (* push)(struct tagStack*, unsigned char *, int);
	int accumtaglen;
	PTAGSTACKITEM (* pop) (struct tagStack*); 
}TAGSTACK, *PTAGSTACK;

typedef struct CXTlv
{
	PTREECOMPONENT root;
	TAGSTACK stack; 

/**
 * Name		: Deserialize
 * Description	: transform stream into tree structure
 * Input
 *	p	: object that will store this structure
 *	stream	: tlv stream
 *	inlength: length of this stream
 **/
	int (*Deserialize)(struct CXTlv *, unsigned char *, int); 

/**
 * Name		: Serialize
 * Description	: serialize tree into tlv stream
 * Input
 * 	p	: object that holds tlv tree
 * 	stream	: output stream
 * 	maxbuffer : buffer size
 */
	int (*Serialize)(struct CXTlv *, unsigned char *, int); 

/**
 * Name		: Cleanup
 * Description	: cleanup tlv tree 
 */
	int (*Cleanup)(struct CXTlv *);

/**
 * Name		: Delete
 * Description	: delete specified tag/tag string from tlv tree 
 **/
	int (*Delete)(struct CXTlv *, unsigned char *);

/**
 * Name		: GetDataFixedL
 * Description	: get data from tree , start from offset zero, up to len. 
 * Input
 *	p	: tlvparser
 *	tag	: tag string to get value from
 *	data	: buffer to store retrieved value
 *	length	: length of value to retrieve 
 * Returns 
 *	CTLV_NOPRIMITIVETAG		: no primitive tag in this tag string
 *	CTLV_OFFSETEXCEEDVALLENGTH 	: offset exceeds tree value length
 *	CTLV_TAGNOTFOUND		: some of the tag passed does not exists
 *	0				: success
 *
 **/
	int (*GetDataFixedL)(struct CXTlv *, unsigned char *, unsigned char *, int);

/**
 * Name		: GetDataVarL
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
 *	CTLV_TAGNOTFOUND		: some of the tag passed does not exists
 *	0				: success
 *
 **/
	int (*GetDataVarL)(struct CXTlv *, unsigned char *, unsigned char *, int, int, int *);

/**
 * Name		: SetDataFixedL
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
	int (*SetDataFixedL)(struct CXTlv *, unsigned char *, unsigned char *, int);	

/**
 * Name		: SetDataVarL
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
	int (*SetDataVarL)(struct CXTlv *, unsigned char *, unsigned char *, int, int);	
	
}CXTLV, *PCXTLV;


/**
 * Name		: BtoH
 * Description	: convert array of bytes into their hexadecimal string
 **/
extern void BtoH(char* pDstHex, unsigned char* pSrc, int nSrcLen);


/**
 * Name		: HtoB
 * Description	: convert hexadecimal string into an array of byte array
 **/
extern void HtoB(unsigned char * pDst, char* pSrcHex, int nSrcLenHex); 

/**
 * Name		: NewDefaultCXTlv
 * Description	: create new cxtlv that uses hierachical linked list structure to store tlv
 **/
extern PCXTLV NewDefaultCXTlv();


/**
 * Name		: InitCXTlvParser
 * Description	: Cleanup p's stack, and assign function pointers to appropriate function.
 **/
extern int InitCXTlvParser (PCXTLV p);

#ifdef __cplusplus
}
#endif
#endif
