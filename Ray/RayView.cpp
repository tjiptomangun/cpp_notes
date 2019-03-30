// RayView.cpp : implementation of the CRayView class
//

#include "stdafx.h"
#include "Ray.h"
#include "Trace.h"
#include "RayDoc.h"
#include "RayView.h"
#include "RenderWindow.h"
#include "Play.h"
#include "ParserEll.h"
#include "ParserTri.h"
#include "SweepParser.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
char Separator[3] = {0x0D, 0x0A, 0x00};
/////////////////////////////////////////////////////////////////////////////
// CRayView

IMPLEMENT_DYNCREATE(CRayView, CEditView)

BEGIN_MESSAGE_MAP(CRayView, CEditView)
	//{{AFX_MSG_MAP(CRayView)
	ON_COMMAND(ID_SCENE, OnScene)
	ON_COMMAND(ID_SPHERE, OnSphere)
	ON_COMMAND(ID_TRIANGLE, OnTriangle)
	ON_COMMAND(ID_RENDER, OnRender)
	ON_COMMAND(ID_RECTANGLE, OnRectangle)
	ON_COMMAND(ID_CONELIGHT, OnConelight)
	ON_COMMAND(ID_CYLINDER, OnCylinder)
	ON_COMMAND(ID_DIRECTEDLIGHT, OnDirectedlight)
	ON_COMMAND(ID_ELLIPSOID, OnEllipsoid)
	ON_COMMAND(ID_POINTLIGHT, OnPointlight)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_COMMAND(ID_WATER, OnWater)
	ON_COMMAND(ID_CLOUDS, OnClouds)
	ON_COMMAND(ID_HORIZON, OnHorizon)
	ON_COMMAND(ID_MARBLE, OnMarble)
	ON_COMMAND(IDM_CLOUDMACRO, OnCloudmacro)
	ON_COMMAND(IDM_TERRAINMACRO, OnTerrainmacro)
	ON_COMMAND(IDM_MAKROPUTAR, OnMakroBendaPutar)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayView construction/destruction

CRayView::CRayView()
{
	// TODO: add construction code here

}

CRayView::~CRayView()
{
}

BOOL CRayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CRayView drawing

void CRayView::OnDraw(CDC* pDC)
{	

	CRayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRayView printing

BOOL CRayView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CRayView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CRayView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CRayView diagnostics

#ifdef _DEBUG
void CRayView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRayView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CRayDoc* CRayView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRayDoc)));
	return (CRayDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayView message handlers

void CRayView::OnScene() 
{
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		strcat(pChar,"Antialiasing()");
		strcat(pChar,Separator);
		strcat(pChar,"Resolution(400,400)");
		strcat(pChar,Separator);
		strcat(pChar,"From(0,0,0)");
		strcat(pChar,Separator);
		strcat(pChar,"At(10,10,10)");
		strcat(pChar,Separator);
		strcat(pChar,"Up(0,0,1)");
		strcat(pChar,Separator);
		strcat(pChar,"Background(0,0,0)");
		strcat(pChar,Separator);
		strcat(pChar,"MaxLevel(2)");
		strcat(pChar,Separator);

		strcat(pChar,"DMin(0)");
		strcat(pChar,Separator);
		strcat(pChar,"DMax(1)");
		strcat(pChar,Separator);
		strcat(pChar,"LMin(0)");
		strcat(pChar,Separator);
		strcat(pChar,"LMax(1)");
		strcat(pChar,Separator);
		strcat(pChar,"NumFrames(1)");
		strcat(pChar,Separator);
		strcat(pChar,"SaveTo(Un.Tif) /Don't set to empty/");
		strcat(pChar,Separator);
		
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);
	
}

void CRayView::OnSphere() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Sphere(Loc(0,0,0), Radius(4), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(0,0,0), Color2(1,1,1), /*Map(?.tif)*/, Checker(1,1), Blend(.25))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);

}

void CRayView::OnTriangle() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Triangle(Loc(0,0,0),V1(0,0,15),V2(15,0,0), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(0,0,0), Color2(1,1,1), Map(?.tif)), Chacker(1,1), Marble(), Blend(.25))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);
	
}

void CRayView::OnRender() 
{
	// TODO: Add your command handler code here
		
	RenderWindow *pRenderWin ;
	
	CString CurrPathName=GetDocument()->GetPathName();
	char *SetUpChar;

	if(CurrPathName.IsEmpty() || GetDocument()->IsModified())
	{
	
		MessageBox("Save your work first","Save Work",MB_ICONSTOP|MB_OK);
		return ;
	
	}
	SetUpChar=CurrPathName.GetBuffer(80);
	Setup(SetUpChar);
	pRenderWin=new RenderWindow;
	
	pRenderWin->DoModal();

}

void CRayView::OnRectangle() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Rectangle(Loc(0,0,0),V1(0,0,15),V2(15,0,0), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(1,1,1), Color2(0,0,0), /Map(?.tif)/, Checker(1,1), Blend(.25))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
}

void CRayView::OnConelight() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);		
		strcat(pChar,"Light(Loc(100,100,100), Color(1,1,1), Cone(), SpreadPower(2), SpreadAngle(.25),LightDir(50,50,50))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
	
}

void CRayView::OnCylinder() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Cylinder(Loc(0,0,0), Radius(4), V1(0,0,1),Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(0,0,0), Color2(1,1,1),/* Map(?.tif)*/, Checker(1,1),Blend(.25))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
}

void CRayView::OnDirectedlight() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);		
		strcat(pChar,"Light(Loc(100,100,100), Color(1,1,1), Directed(), SpreadPower(2), LightDir(50,50,50))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
	
}

void CRayView::OnEllipsoid() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Ellipsoid(Loc(0,0,0),V1(1,1,1), V2(5,5,5), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(0,0,0), Color2(1,1,1), /*Map(?.tif)*/, Checker(1,1), Blend(.25))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
}

void CRayView::OnPointlight() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);		
		strcat(pChar,"Light(Loc(100,100,100),Point(),Color(1,1,1))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
	
}

void CRayView::OnPlay() 
{
	// TODO: Add your command handler code here
	CString m_Path;
	Play m_PlayBack;
	char tChar[16];
	DWORD NumFrames;
	FILE *fp;

	CFileDialog m_File(TRUE,"",m_Path,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"");
	if(m_File.DoModal()==IDCANCEL)
		return ;
	m_Path=m_File.GetPathName();
	m_PlayBack.m_PathName=m_Path.GetBuffer(10);
	fp=fopen(m_Path.GetBuffer(10),"r");
	fread(tChar,sizeof(DWORD),1,fp);
	NumFrames=(DWORD)atoi(tChar);
	m_PlayBack.m_NumFrames=NumFrames;

	m_PlayBack.DoModal();

	
}

void CRayView::OnWater() 
{
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);		
		strcat(pChar,"Rectangle(Loc(0,0,0),V1(0,0,15),V2(15,0,0), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(1,1,1),Water(.1,.1,12)");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
	
}

void CRayView::OnClouds() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);
		
		strcat(pChar,"Ellipsoid(Loc(0,0,0),V1(1,1,1), V2(5,5,5), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(0,0,0), Color2(1,1,1), Cloud(1,2))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);
	
}

void CRayView::OnHorizon() 
{
	// TODO: Add your command handler code here
		UINT BuffLen=GetBufferLength();
		TCHAR *pChar=new TCHAR[BuffLen+2048];
		::GetWindowText(m_hWnd,pChar,BuffLen+1);		
		strcat(pChar,"Rectangle(Loc(0,0,0),V1(0,0,15),V2(15,0,0), Ambient(.5), Diffuse(.5), Specular(.6), SpecSpread(3), Transparency(0), Color(1,1,1),Color2(0,0,0),Cloud(.2,3))");
		strcat(pChar,Separator);
		SetWindowText(NULL);
		::SetWindowText(m_hWnd,pChar); 	
		delete[] pChar;
		GetDocument()->SetModifiedFlag(true);	
	
}

void CRayView::OnMarble() 
{
	// TODO: Add your command handler code here
	
}

void CRayView::OnCloudmacro() 
{

	CString InFile,OutFile;
	static char BASED_CODE szFilter[] = "Ellipse Files (*.ell)|*.ell|All Files (*.*)|*.*||";
	static char BASED_CODE szFilter1[] = "Ray File (*.ray)|*.ray||";
	CFileDialog m_ldFile(TRUE, ".ell", NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	CFileDialog m_svFile(FALSE, ".ray", NULL,
        OFN_CREATEPROMPT, szFilter1);
	if(m_ldFile.DoModal() == IDOK)
	{
	
		InFile = m_ldFile.GetPathName();
		if(m_svFile.DoModal() == IDOK)
		{
			OutFile = m_svFile.GetPathName();
			ellParserMain(InFile.GetBuffer(20),OutFile.GetBuffer(20));
		}
	
	}
	
	
}

void CRayView::OnTerrainmacro() 
{
	// TODO: Add your command handler code here
		
	CString InFile,OutFile;
	static char BASED_CODE szFilter[] = "Traingles Files (*.tri)|*.tri|All Files (*.*)|*.*||";
	static char BASED_CODE szFilter1[] = "Ray File (*.ray)|*.ray||";
	CFileDialog m_ldFile(TRUE, ".tri", NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	CFileDialog m_svFile(FALSE, ".ray", NULL,
        OFN_CREATEPROMPT, szFilter1);
	if(m_ldFile.DoModal() == IDOK)
	{
	
		InFile = m_ldFile.GetPathName();
		if(m_svFile.DoModal() == IDOK)
		{
			OutFile = m_svFile.GetPathName();
			triParserMain(InFile.GetBuffer(20),OutFile.GetBuffer(20));
		}
	
	}
	
}

void CRayView::OnMakroBendaPutar() 
{
	CString InFile,OutFile;
	static char BASED_CODE szFilter[] = "Object Files (*.odf)|*.odf|All Files (*.*)|*.*||";
	static char BASED_CODE szFilter1[] = "Ray File (*.tri)|*.tri|All Files (*.*)|*.*||";
	CFileDialog m_ldFile(TRUE, ".odf", NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	CFileDialog m_svFile(FALSE, ".tri", NULL,
        OFN_CREATEPROMPT, szFilter1);
	if(m_ldFile.DoModal() == IDOK)
	{
	
		InFile = m_ldFile.GetPathName();
		if(m_svFile.DoModal() == IDOK)
		{
			OutFile = m_svFile.GetPathName();
			swpParserMain(InFile.GetBuffer(20),OutFile.GetBuffer(20));
		}
	
	}
	
	
}
