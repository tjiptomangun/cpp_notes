#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tiff.h"

UCHAR IntelOrdr;
extern FILE *Efp;

int ReadFxHdr(FILE *fp,ULONG *FileOffs)
{

	
	TIFFHEADER TIFFHdr;
	fseek(fp,0L,SEEK_SET);
	fread(&TIFFHdr,sizeof(TIFFHEADER),1,fp);
	IntelOrdr=(TIFFHdr.ByteOrder==INTELBYTES) ? 1:0;
	if(!IntelOrdr)
		SwapWordOrder(&TIFFHdr.Version);
	if(TIFFHdr.Version!=TIFFVERSION)
		return TIFFFORMATNOTSUPPORTED;

	if(!IntelOrdr) SwapLongOrder(FileOffs);
	*FileOffs=TIFFHdr.Offset;
	fseek(fp,*FileOffs,SEEK_SET);
	return 1;

}

int ReadTags(FILE *fp,IMAGE_INFO *ImInfo)
{

	USHORT i,j;
	USHORT Val;
	USHORT NumTags;
	TAGENTRY Entry;
	ULONG FilePos;

	ReadWord(fp,&NumTags);
	for(i=0;i<NumTags;i++)
	{
	
		ReadTag(fp,&Entry);
		switch(Entry.Tag)
		{
		
			case TGBITSPERSAMPLE:
				if(Entry.DataType!=TIFFSHORT)
					return TIFFFORMATNOTSUPPORTED;
				if(Entry.Length==1)
					ImInfo->BitsPerSample=(SHORT)Entry.Offset;
				else if(Entry.Length==3)
					ImInfo->BitsPerSample=8;
				break;

			case TGCOMPRESSION:
				if(Entry.DataType!=TIFFSHORT ||Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				ImInfo->Compression=(SHORT)Entry.Offset;
				break;

			case TGSAMPLESPERPIXEL:
				if(Entry.DataType!=TIFFSHORT || Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				ImInfo->SamplesPerPixel=(SHORT)Entry.Offset;
				break;

			case TGROWSPERSTRIP:
				if((Entry.DataType!=TIFFSHORT && Entry.DataType!=TIFFLONG)||Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				break;
				
			case TGSTRIPOFFSETS:
				if(Entry.DataType!=TIFFSHORT && Entry.DataType!=TIFFLONG)
				{
				
					return TIFFFORMATNOTSUPPORTED;
				
				}
				ImInfo->NumStrips=(SHORT)Entry.Length;
				ImInfo->StripOffsets=(ULONG *)malloc(sizeof(ULONG)*ImInfo->NumStrips);
				if(ImInfo->StripOffsets==NULL)
				{
				
					return OUTOFMEMORY;
				
				}

				if(ImInfo->NumStrips==1)
				{
				
					ImInfo->StripOffsets[0]=Entry.Offset;
				
				}
				else
				{
				
					FilePos=ftell(fp);
					fseek(fp,Entry.Offset,SEEK_SET);
					if(Entry.DataType==TIFFLONG)
						for(j=0;j<ImInfo->NumStrips;j++)
							ReadULong(fp,&ImInfo->StripOffsets[j]);
					else
					{
					
						for(j=0;j<ImInfo->NumStrips;j++)
						{
						
							ReadWord(fp,&Val);
							ImInfo->StripOffsets[j]=Val;
						
						}
					
					}
					fseek(fp,FilePos,SEEK_SET);
				
				}
				break;

			case TGSTRIPBYTECOUNTS:
				if(Entry.DataType!=TIFFSHORT && Entry.DataType!=TIFFLONG)
					return TIFFFORMATNOTSUPPORTED;
				ImInfo->NumStrips=(SHORT)Entry.Length;
				ImInfo->StripLengths=(ULONG *)malloc(sizeof(ULONG)*ImInfo->NumStrips);
				if(ImInfo->StripLengths==NULL)
				{
				
					free(ImInfo->StripOffsets);
					return OUTOFMEMORY;
				
				}
				if(ImInfo->NumStrips==1)
					ImInfo->StripLengths[0]=Entry.Offset;
				else
				{
				
					FilePos=ftell(fp);
					fseek(fp,Entry.Offset,SEEK_SET);
					if(Entry.DataType==TIFFLONG)
						for(j=0;j<ImInfo->NumStrips;j++)
							ReadULong(fp,&ImInfo->StripLengths[j]);
					else
					{
						
						for(j=0;j<ImInfo->NumStrips;j++)
						{
						
							ReadWord(fp,&Val);
							ImInfo->StripLengths[j]=Val;
						
						}
						
					}
					fseek(fp,FilePos,SEEK_SET);
				}
				break;

			default:
				break;
		}
	
	}
	return 1;
}

int ReadStrips(FILE *fp,IMAGE_INFO *ImInfo,unsigned char *Im)
{

	USHORT i,j,k,NumPieces,MaxStripLen=8192;

	ULONG p;
	UCHAR *Buffer;
	Buffer=(UCHAR *)malloc(MaxStripLen);
	if(Buffer==NULL) return OUTOFMEMORY;	
	p=0;
	for(j=0;j<ImInfo->NumStrips;j++)
	{
	
		fseek(fp,ImInfo->StripOffsets[j],SEEK_SET);
		NumPieces=(USHORT)((double)ImInfo->StripLengths[j]/MaxStripLen);
		for(i=0;i<NumPieces;i++)
		{
		
			if(fread(Buffer,1,MaxStripLen,fp)!=MaxStripLen)
				break;
			
			for(k=0;k<MaxStripLen;k++,p++)
				Im[p]=Buffer[k];										
		}
		NumPieces=(USHORT)(ImInfo->StripLengths[j]%MaxStripLen);
		if(NumPieces!=0)
		{
		
			if(fread(Buffer,1,(UINT)NumPieces,fp)!=NumPieces)break;
			for(i=0;i<NumPieces;i++)
			{
				Im[p++]=Buffer[i];
				
			}
		
		}

	
	}
	free(Buffer);
	return 1;
}

int ReadGrayTIFFImage(FILE *fp,UCHAR *Im)
{

	ULONG FileOffs;
	IMAGE_INFO ImInfo;
	int Err;

	if((Err=ReadFxHdr(fp,&FileOffs))!=1)
		return Err;

	if((Err=ReadTags(fp,&ImInfo))!=1)
		return Err;

	if(ImInfo.BitsPerSample!=8 || ImInfo.Compression!=NOCOMPRESSION ||ImInfo.SamplesPerPixel!=1)
	{
		Err=TIFFFORMATNOTSUPPORTED;
	}
	else		
		Err=ReadStrips(fp,&ImInfo,Im);

	free(ImInfo.StripOffsets);
	free(ImInfo.StripLengths);
	return Err;

}

int ReadRGBTIFFImage(FILE *fp,UCHAR *Im)
{

	ULONG FileOffs;
	IMAGE_INFO ImInfo;
	int Err;

	if((Err=ReadFxHdr(fp,&FileOffs))!=1)
		return Err;

	if((Err=ReadTags(fp,&ImInfo))!=1)
		return Err;

	if(ImInfo.BitsPerSample!=8 || ImInfo.Compression!=NOCOMPRESSION || ImInfo.SamplesPerPixel!=3)
	{
		Err=TIFFFORMATNOTSUPPORTED;
	}
	else
		Err=ReadStrips(fp,&ImInfo,Im);
	free(ImInfo.StripOffsets);
	free(ImInfo.StripLengths);
	return Err;
}

int GetTIFFImageInfo(FILE *fp,SHORT *ImWd,SHORT *ImHt,SHORT *ImType)
{

	int Err;
	USHORT i;
	USHORT NumTags;
	TAGENTRY Entry;
	ULONG FileOffs;

	if((Err=ReadFxHdr(fp,&FileOffs))!=1)
		return Err;
	*ImWd=-1; *ImHt=-1; *ImType=-1;

	fseek(fp,FileOffs,SEEK_SET);

	ReadWord(fp,&NumTags);
	for(i=0;i<NumTags;i++)
	{
	
		ReadTag(fp,&Entry);
		switch(Entry.Tag)
		{
		
			case TGIMAGELENGTH:
				if((Entry.DataType!=TIFFSHORT && Entry.DataType!=TIFFLONG) ||Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				*ImHt=(SHORT)Entry.Offset;
				break;
			case TGIMAGEWIDTH:
				if((Entry.DataType!=TIFFSHORT && Entry.DataType!=TIFFLONG) || Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				*ImWd=(SHORT)Entry.Offset;

			case TGPHOTOMETRICINTERPRETATION:
				if(Entry.DataType!=TIFFSHORT || Entry.Length!=1)
					return TIFFFORMATNOTSUPPORTED;
				*ImType=(SHORT)Entry.Offset;
				break;
			default:
				break;


		
		}
	
	}
	if(*ImWd==-1 || *ImHt==-1 || *ImType==-1)
		return TIFFFORMATNOTSUPPORTED;
	return 1;

}

int GetImageInfo(char *Path,SHORT *ImWd,SHORT *ImHt,SHORT *ImType)
{

	FILE *fp;
	if((fp=fopen(Path,"rb"))==NULL)
		return 0;
	if(GetTIFFImageInfo(fp,ImWd,ImHt,ImType)!=1)
	{
	
		fclose(fp);
		return 0;
	
	}
	fclose(fp);
	return 1;

}

int ReadImage(char *Path,UCHAR *Im)
{

	FILE *fp;
	SHORT ImWd,ImHt,ImType,RetVal=1;
	
	if((fp=fopen(Path,"rb"))==NULL)
		return 0;

	if(GetTIFFImageInfo(fp,&ImWd,&ImHt,&ImType)<0)
	{
	
		fclose(fp);
		return 0;
	
	}

	if(ImType==1)
	{
	
		if(ReadGrayTIFFImage(fp,Im)<0)
			RetVal=0;
		
	
	}
	else if(ImType==2) 
	{
	
		if(ReadRGBTIFFImage(fp,Im)<=0)
			RetVal=0;
		return RetVal;
		
	
	}
	else
		RetVal=0;
	fclose(fp);
	return RetVal;

}

int WriteTag(FILE *fp,USHORT Tag,USHORT DataType,ULONG Len,ULONG Offs)
{

	TAGENTRY Entry;
	Entry.Tag=Tag;
	Entry.DataType=DataType;
	Entry.Length=Len;
	Entry.Offset=Offs;
	fwrite(&Entry,sizeof(TAGENTRY),1,fp);
	return 1;

}

int WriteFxHdr(FILE *fp)
{

	TIFFHEADER TIFFHdr;
	TIFFHdr.ByteOrder=INTELBYTES;
	TIFFHdr.Version=TIFFVERSION;
	//Start IFD after TIFF header
	TIFFHdr.Offset=sizeof(TIFFHEADER);
	fwrite(&TIFFHdr,sizeof(TIFFHEADER),1,fp);
	return 1;

}

int WriteRGBTIFFHeader(FILE *fp,ULONG ImWd,ULONG ImHt,ULONG StripSize)
{

	ULONG FileOffs;
	ULONG NextIFD;
	ULONG StartOfStrip;
	ULONG Numerator=75L;
	ULONG Denominator=1L;
	SHORT NumTagEntries=14;
	USHORT StripsPerImage,i;
	ULONG LastStripLen;
	USHORT RowsPerStrip;
	USHORT Val;
	ULONG Size;
	char temp='$';

	RowsPerStrip=(USHORT)(StripSize/(ImWd*3));
	if(RowsPerStrip==0)
		return 0;
	Size=ImWd*ImHt*3L;
	StripsPerImage=(USHORT)(Size/StripSize + Size%StripSize);

	WriteFxHdr(fp);

	FileOffs=sizeof(TIFFHEADER);
	FileOffs+=sizeof(USHORT) + sizeof(LONG) + 
		NumTagEntries*sizeof(TAGENTRY);

	fwrite(&NumTagEntries,sizeof(SHORT),1,fp);

	WriteTag(fp,TGNEWSUBFILETYPE,TIFFLONG,1L,0L);

	WriteTag(fp,TGIMAGEWIDTH,TIFFSHORT,1L,ImWd);

	WriteTag(fp,TGIMAGELENGTH,TIFFSHORT,1L,ImHt);

	WriteTag(fp,TGBITSPERSAMPLE,TIFFSHORT,3L,FileOffs);

	FileOffs+=3L*sizeof(SHORT);

	WriteTag(fp,TGCOMPRESSION,TIFFSHORT,1L,1L);

	WriteTag(fp,TGPHOTOMETRICINTERPRETATION,TIFFSHORT,1L,2L);

	WriteTag(fp,TGSTRIPOFFSETS,TIFFLONG,StripsPerImage,
		FileOffs+4L*sizeof(ULONG)+StripsPerImage*sizeof(ULONG));

	WriteTag(fp,TGSAMPLESPERPIXEL,TIFFSHORT,1L,3L);

	WriteTag(fp,TGROWSPERSTRIP,TIFFLONG,1L,RowsPerStrip);

	WriteTag(fp,TGSTRIPBYTECOUNTS,TIFFLONG,StripsPerImage,
		FileOffs+4L*sizeof(ULONG));
	FileOffs+=sizeof(ULONG)*2L;

	WriteTag(fp,TGXRESOLUTION,TIFFRATIONAL,1L,FileOffs);
	FileOffs+=sizeof(ULONG)*2L;

	WriteTag(fp,TGYRESOLUTION,TIFFRATIONAL,1L,FileOffs);
	FileOffs+=sizeof(ULONG)*2L;

	WriteTag(fp,TGPLANARCONFIGURATION,TIFFSHORT,1L,1L);

	WriteTag(fp,TGRESOLUTIONUNIT,TIFFSHORT,1L,2L);

	NextIFD=0L;
	fwrite(&NextIFD,sizeof(ULONG),1,fp);

	Val=8;
	fwrite(&Val,sizeof(SHORT),1,fp);
	fwrite(&Val,sizeof(SHORT),1,fp);
	fwrite(&Val,sizeof(SHORT),1,fp);

	//X Resolution
	fwrite(&Numerator,sizeof(ULONG),1,fp);
	fwrite(&Denominator,sizeof(ULONG),1,fp);

	fwrite(&Numerator,sizeof(ULONG),1,fp);
	fwrite(&Denominator,sizeof(ULONG),1,fp);
	//Y Resolution
	LastStripLen=((ULONG)ImWd*(ULONG)ImHt*3L)-
		((ULONG)StripSize*(StripsPerImage-1));
	if(LastStripLen==0)
	{
	
		LastStripLen=StripSize;
	
	}

	for(i=0;i<(StripsPerImage-1);i++)
		fwrite(&StripSize,sizeof(ULONG),1,fp);
	fwrite(&LastStripLen,sizeof(ULONG),1,fp);
	FileOffs+=sizeof(ULONG)*StripsPerImage;
	StartOfStrip=FileOffs+sizeof(ULONG)*StripsPerImage;

	for(i=0;i<StripsPerImage;i++)
	{
	
		fwrite(&StartOfStrip,sizeof(ULONG),1,fp);
		StartOfStrip+=StripSize;
	
	}
	fwrite(&temp,sizeof(UCHAR),2,fp);
	return 1;

}

int WriteGrayTIFFHeader(FILE *fp,ULONG ImWd,ULONG ImHt,ULONG StripSize)
{

	ULONG FileOffs;
	ULONG NextIFD;
	ULONG StartOfStrip;
	ULONG Numerator=75L;
	ULONG Denominator=1L;
	SHORT NumTagEntries=14;
	USHORT StripsPerImage,i;
	ULONG LastStripLen;
	USHORT RowsPerStrip;
	ULONG Size;

	RowsPerStrip=(USHORT)(StripSize/ImWd*3);

	if(RowsPerStrip==0)
		return 0;
	Size=ImWd*ImHt;
	StripsPerImage=(USHORT)(Size/StripSize + Size%StripSize);
	WriteFxHdr(fp);
	FileOffs=sizeof(TIFFHEADER);

	FileOffs+=sizeof(SHORT) + sizeof(LONG) + NumTagEntries*sizeof(TAGENTRY);

	fwrite(&NumTagEntries,sizeof(SHORT),1,fp);

	WriteTag(fp,TGNEWSUBFILETYPE,TIFFLONG,1L,0L);

	WriteTag(fp,TGIMAGEWIDTH,TIFFSHORT,1L,ImWd);

	WriteTag(fp,TGIMAGELENGTH,TIFFSHORT,1L,ImHt);
	
	WriteTag(fp,TGBITSPERSAMPLE,TIFFSHORT,1L,8L);

	WriteTag(fp,TGCOMPRESSION,TIFFSHORT,1L,1L);

	WriteTag(fp,TGPHOTOMETRICINTERPRETATION,TIFFSHORT,1L,1L);

	WriteTag(fp,TGSTRIPOFFSETS,TIFFLONG,StripsPerImage,FileOffs+
		4L*sizeof(ULONG)+StripsPerImage*sizeof(ULONG));

	WriteTag(fp,TGSAMPLESPERPIXEL,TIFFSHORT,1L,1L);

	WriteTag(fp,TGROWSPERSTRIP,TIFFLONG,1L,RowsPerStrip);

	WriteTag(fp,TGSTRIPBYTECOUNTS,TIFFLONG,StripsPerImage,FileOffs+4L*sizeof(ULONG));

	WriteTag(fp,TGXRESOLUTION,TIFFRATIONAL,1L,FileOffs);

	FileOffs+=sizeof(ULONG)*2L;

	WriteTag(fp,TGYRESOLUTION,TIFFRATIONAL,1L,FileOffs);

	FileOffs+=sizeof(ULONG)*2L;

	WriteTag(fp,TGPLANARCONFIGURATION,TIFFSHORT,1L,1L);

	WriteTag(fp,TGRESOLUTIONUNIT,TIFFSHORT,1L,2L);

	NextIFD=0L;

	fwrite(&NextIFD,sizeof(ULONG),1,fp);

	//X Resolution
	fwrite(&Numerator,sizeof(ULONG),1,fp);
	fwrite(&Denominator,sizeof(ULONG),1,fp);

	//Y Resolution
	fwrite(&Numerator,sizeof(ULONG),1,fp);
	fwrite(&Denominator,sizeof(ULONG),1,fp);

	LastStripLen=((ULONG)ImWd*(ULONG)ImHt)-
		((ULONG)StripSize*(StripsPerImage-1));

	if(LastStripLen==0)
		LastStripLen=StripSize;

	for(i=0;i<StripsPerImage-1;i++)
		fwrite(&StripSize,sizeof(ULONG),1,fp);
	fwrite(&LastStripLen,sizeof(ULONG),1,fp);

	FileOffs+=sizeof(ULONG)*StripsPerImage;

	StartOfStrip=FileOffs+sizeof(ULONG)*StripsPerImage;

	for(i=0;i<StripsPerImage;i++)
	{
	
		fwrite(&StartOfStrip,sizeof(ULONG),1,fp);
		StartOfStrip+=StripSize;
	
	}
	return 1;

}

void SwapWordOrder(USHORT *Word)
{

	UCHAR *Byte=(UCHAR *)Word;
	UCHAR Tmp;

	Tmp=Byte[1];
	Byte[1]=Byte[0];
	Byte[0]=Tmp;


}

void SwapLongOrder(ULONG *Long)
{

	UCHAR *Byte=(UCHAR *)Long;
	UCHAR Tmp;
	Tmp=Byte[3];
	Byte[3]=Byte[0];
	Byte[0]=Tmp;
	Tmp=Byte[2];
	Byte[2]=Byte[1];
	Byte[1]=Tmp;

}

int ReadTag(FILE *fp,TAGENTRY *Entry)
{

	int t=fread(Entry,sizeof(TAGENTRY),1,fp);
	if(t==1)
	{
	
		if(!IntelOrdr)
		{
		
			SwapWordOrder(&Entry->Tag);
			SwapWordOrder(&Entry->DataType);
			SwapLongOrder(&Entry->Length);
			SwapLongOrder(&Entry->Offset);
			if(Entry->DataType==TIFFSHORT && Entry->Length==1)
				Entry->Offset=Entry->Offset>>16;
		
		}
		return 1;
	
	}
	return 0;

}

int ReadWord(FILE *fp,USHORT *Val)
{

	int t=fread(Val,sizeof(USHORT),1,fp);
	if(t==1)
	{
	
		if(!IntelOrdr)
			SwapWordOrder(Val);
		return 1;
	
	}
	return 0;

}

int ReadULong(FILE *fp,ULONG *Val)
{

	int t=fread(Val,sizeof(ULONG),1,fp);
	if(t==1)
	{
	
		if(!IntelOrdr)
			SwapLongOrder(Val);
		return 1;
	
	}
	return 0;

}