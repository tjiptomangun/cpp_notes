// Play.cpp : implementation file
//

#include "stdafx.h"
#include "Ray.h"
#include "Play.h"
#define TimerID 1
#include "tiff.h"
#include <conio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Play dialog


Play::Play(CWnd* pParent /*=NULL*/)
	: CDialog(Play::IDD, pParent)
{
	//{{AFX_DATA_INIT(Play)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	FrameNumber=0;
	First=TRUE;
}


void Play::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Play)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void Play::OnPaint()
{
	hDC=::GetDC(::GetDlgItem(hWnd=this->m_hWnd,ID_IMAGEPLAY));
	char Buffer[80];
	char CurrentFile[180];
	SHORT ImType;
	::GetClientRect(this->m_hWnd,&rect);
	MemDC=CreateCompatibleDC(hDC);
	if(First)
	{
		strcpy(CurrentFile,m_PathName);
		strcat(CurrentFile,".");
		strcat(CurrentFile,"0");
		GetImageInfo(CurrentFile,&ImWd,&ImHt,&ImType);
		for(DWORD i=0;i<m_NumFrames;i++)
		{		
			hBitmap[i]=::CreateCompatibleBitmap(hDC,ImWd,ImHt);
			hBmpTmp=(HBITMAP)SelectObject(MemDC,hBitmap[i]);
			itoa(i,Buffer,10);
			strcpy(CurrentFile,m_PathName);
			strcat(CurrentFile,".");
			strcat(CurrentFile,Buffer);
			if(readImage(CurrentFile,hDC)<0)
			{			
				MessageBox("Failed openning file",NULL,MB_OK);
				return ;			
			}
			::BitBlt(MemDC,0,0,ImWd,ImHt,hDC,0,0,SRCCOPY);
			::SelectObject(MemDC,hBmpTmp);		
		}
		First=FALSE;
	}
	do
	{			

		::SelectObject(MemDC,hBitmap[FrameNumber]);
		::BitBlt(hDC,0,0,ImWd,ImHt,MemDC,0,0,SRCCOPY);
		
		
	}while(FrameNumber++<m_NumFrames);
	::DeleteDC(MemDC);			
	::ReleaseDC(hWnd,hDC);
}

void Play::OnDestroy()
{
	DWORD i;
	for(i=0;i<m_NumFrames;i++)
		::DeleteObject(&hBitmap[i]);
	::PostQuitMessage(0);
}

int Play::readImage(char *Filename,HDC MemDC)
{

	SHORT T;
	long x,y;
	BYTE Red,Green,Blue;
	unsigned char * Image;
	
		if(GetImageInfo(Filename,&ImWd,&ImHt,&T)<=0)
			return -1;
		if(T==2)
			Image=(UCHAR *)malloc((long)ImWd*(long)ImHt*3L);
		else
			return -1;
		if(Image==NULL)
			return -1;
		if(!ReadImage(Filename,Image))
			return -1;

		for(y=0;y<ImHt;y++)
		{
			for(x=0;x<ImWd;x++)
			{
			
				if(T==2)
				{
				
					Red=Image[(y*(long)ImWd + x)*3L];
					Green=Image[(y*(long)ImWd + x)*3L+1];
					Blue=Image[(y*(long)ImWd + x)*3L+2];

					SetPixel(MemDC,x,y,RGB(Red,Green,Blue));
				
				}
			
			}
		}

			free(Image);
			return 1;
}	

BEGIN_MESSAGE_MAP(Play, CDialog)
	//{{AFX_MSG_MAP(Play)
	ON_BN_CLICKED(ID_PLAYBACK, OnPlayback)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Play message handlers

void Play::OnPlayback() 
{
	OnPaint();
	//OnTimer();
	
}
