// RenderWindow.cpp : implementation file
//

#include "stdafx.h"
#include "Ray.h"
#include "RenderWindow.h"
#include "Trace.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RenderWindow dialog


RenderWindow::RenderWindow(CWnd* pParent /*=NULL*/)
	: CDialog(RenderWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(RenderWindow)
	m_Count = 0;
	//}}AFX_DATA_INIT
}


void RenderWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RenderWindow)
	DDX_Control(pDX, IDC_UPZ, m_UpZ);
	DDX_Control(pDX, IDC_UPY, m_UpY);
	DDX_Control(pDX, IDC_UPX, m_UpX);
	DDX_Control(pDX, IDC_FROMZ, m_FromZ);
	DDX_Control(pDX, IDC_FROMY, m_FromY);
	DDX_Control(pDX, IDC_FROMX, m_FromX);
	DDX_Control(pDX, IDC_ATZ, m_AtZ);
	DDX_Control(pDX, IDC_ATY, m_AtY);
	DDX_Control(pDX, IDC_ATX, m_AtX);
	DDX_Control(pDX, ID_TIME, m_Render_Time);
	DDX_Control(pDX, ID_IMAGE, m_Render_Image);
	//}}AFX_DATA_MAP
}

CDC* RenderWindow::AskDC()
{

	return m_Render_Image.GetDC();

}

BEGIN_MESSAGE_MAP(RenderWindow, CDialog)
	//{{AFX_MSG_MAP(RenderWindow)
	ON_BN_CLICKED(ID_TRACE, OnTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RenderWindow message handlers

void RenderWindow::OnTrace() 
{
	// TODO: Add your control notification handler code here
	HDC hDC;	
	char tempChar[16];
	m_FromX.GetWindowText(tempChar,16);
	From.x = atof(tempChar);

	m_FromY.GetWindowText(tempChar,16);
	From.y = atof(tempChar);

	m_FromZ.GetWindowText(tempChar,16);
	From.z = atof(tempChar);

	m_AtX.GetWindowText(tempChar,16);
	At.x = atof(tempChar);
	m_AtY.GetWindowText(tempChar,16);
	At.y = atof(tempChar);
	m_AtZ.GetWindowText(tempChar,16);
	At.z = atof(tempChar);

	m_UpX.GetWindowText(tempChar,16);
	Up.x = atof(tempChar);
	m_UpY.GetWindowText(tempChar,16);
	Up.y = atof(tempChar);
	m_UpZ.GetWindowText(tempChar,16);
	Up.z = atof(tempChar);

	DWORD T=GetTickCount();	
	hDC=m_Render_Image.GetDC()->m_hDC;	
	GenImage(hDC);
	T=GetTickCount()-T;
	itoa((int) T,tempChar,10);	
	strcat(tempChar," mSecs");
	m_Render_Time.SetWindowText(tempChar);
	


}

BOOL RenderWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	char Buffer[20]="0000";
	strcat(Buffer," mSecs");
	m_Render_Time.SetWindowText(Buffer);	
	sprintf(Buffer,"%f",From.x);
	m_FromX.SetWindowText(Buffer);
	sprintf(Buffer,"%f",From.y);
	m_FromY.SetWindowText(Buffer);
	sprintf(Buffer,"%f",From.z);
	m_FromZ.SetWindowText(Buffer);

	sprintf(Buffer,"%f",At.x);
	m_AtX.SetWindowText(Buffer);
	sprintf(Buffer,"%f",At.y);
	m_AtY.SetWindowText(Buffer);
	sprintf(Buffer,"%f",At.z);
	m_AtZ.SetWindowText(Buffer);
		
	sprintf(Buffer,"%f",Up.x);
	m_UpX.SetWindowText(Buffer);
	sprintf(Buffer,"%f",Up.y);
	m_UpY.SetWindowText(Buffer);
	sprintf(Buffer,"%f",Up.z);
	m_UpZ.SetWindowText(Buffer);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
