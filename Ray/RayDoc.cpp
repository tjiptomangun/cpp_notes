// RayDoc.cpp : implementation of the CRayDoc class
//

#include "stdafx.h"
#include "Ray.h"

#include "RayDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayDoc

IMPLEMENT_DYNCREATE(CRayDoc, CDocument)

BEGIN_MESSAGE_MAP(CRayDoc, CDocument)
	//{{AFX_MSG_MAP(CRayDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayDoc construction/destruction

CRayDoc::CRayDoc()
{
	// TODO: add one-time construction code here

}

CRayDoc::~CRayDoc()
{
}

BOOL CRayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRayDoc serialization

void CRayDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CRayDoc diagnostics

#ifdef _DEBUG
void CRayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayDoc commands
