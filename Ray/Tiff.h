#ifndef TIFFH
#define TIFFH
#include <stdio.h>

typedef unsigned short USHORT;
typedef signed short SHORT;

typedef signed int INT;
typedef unsigned int UINT;

typedef unsigned long ULONG;
typedef signed long LONG;

typedef unsigned char UCHAR;

#define TIFFBYTE  1
#define TIFFASCII 2
#define TIFFSHORT 3
#define TIFFLONG  4
#define TIFFRATIONAL 5

#define TGNEWSUBFILETYPE	254
#define TGOLDSUBFILETYPE	255
#define TGIMAGEWIDTH		256
#define TGIMAGELENGTH		257
#define TGBITSPERSAMPLE		258
#define TGCOMPRESSION		259

#define TGPHOTOMETRICINTERPRETATION	262
#define TGTHRESHOLDING		263
#define TGCELLWIDTH			264
#define TGCELLLENGTH		265
#define TGFILLORDER			266

#define TGDOCUMENTNAME		269
#define TGIMAGEDESCRIPTION	270
#define TGMAKE				271
#define TGMODEL				272
#define TGSTRIPOFFSETS		273
#define TGORIENTATION		274

#define TGSAMPLESPERPIXEL	277
#define TGROWSPERSTRIP		278
#define TGSTRIPBYTECOUNTS	279
#define TGMINSAMPLEVALUE	280
#define TGMAXSAMPLEVALUE	281
#define TGXRESOLUTION		282
#define TGYRESOLUTION		283
#define TGPLANARCONFIGURATION	284
#define TGPAGENAME			285
#define TGXPOSITION			286
#define TGYPOSITION			287
#define TGFREEOFFSETS		288
#define TGFREEBYTECOUNTS	289
#define TGGRAYUNIT			290
#define TGGRAYCURVE			291
#define TGRESOLUTIONUNIT	296
#define TGPAGENUMBER		297

#define TGCOLORRESPONSECURVES	301

#define TGSOFTWARE			305
#define TGDATETIME			306

#define TGARTIST			315
#define TGHOSTCOMPUTER		316

#define TGPREDICTOR			317
#define TGWHITEPOINT		318
#define TGPRIMARYCHROMATICITIES	319
#define TGCOLORMAP			320

#define INTELBYTES			0x4949
#define TIFFVERSION			0x002A
#define NOCOMPRESSION		1

#define TIFFFORMATNOTSUPPORTED	-2
#define IMAGEFILETOOBIG		-3
#define OUTOFMEMORY			-4

#define STRIPSIZE 8192

typedef struct
{

	USHORT ByteOrder;
	USHORT Version;
	ULONG Offset;

}TIFFHEADER;

typedef struct
{

	USHORT Tag;
	USHORT DataType;
	ULONG Length;
	ULONG Offset;

}TAGENTRY;

typedef struct
{

	SHORT BitsPerSample;
	SHORT Compression;
	SHORT SamplesPerPixel;
	SHORT NumStrips;
	ULONG *StripLengths;
	ULONG *StripOffsets;

}IMAGE_INFO;

//functions in TIFF.C

int ReadStrips(FILE *fp,IMAGE_INFO *ImInfo,UCHAR *Im);
int ReadGrayTIFFImage(FILE *fp,UCHAR *Im);
int ReadRGBTIFFImage(FILE *fp,UCHAR *Im);
int GetTIFFImageInfo(FILE *fp,SHORT *ImWd,SHORT *ImHt,SHORT *ImType);
int GetImageInfo(char *Path,SHORT *ImWd,SHORT *ImHt,SHORT *ImType);
int ReadImage(char *Path,UCHAR *Im);
int WriteTag(FILE *fp,USHORT Tag,USHORT DataType,ULONG Len,ULONG Offs);
int WriteRGBTIFFHeader(FILE *fp,ULONG ImWd,ULONG ImHt,ULONG StripSize);
int WriteGrayTIFFHeader(FILE *fp,ULONG ImWd,ULONG ImHt,ULONG StripSize);
int WriteFxHdr(FILE *fp);
void SwapWordOrder(USHORT *Word);
void SwapLongOrder(ULONG *Long);
int ReadTag(FILE *fp,TAGENTRY *Entry);
int ReadWord(FILE *fp,USHORT *Val);
int ReadULong(FILE *fp,ULONG *Val);
int ReadTags(FILE *fp,IMAGE_INFO *ImInfo);
int ReadFxHdr(FILE *fp,ULONG *FileOfs);

#endif
