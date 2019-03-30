// RayView.h : interface of the CRayView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAYVIEW_H__331ABD86_67AA_11D6_B72E_E77FF821D736__INCLUDED_)
#define AFX_RAYVIEW_H__331ABD86_67AA_11D6_B72E_E77FF821D736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRayView : public CEditView
{
protected: // create from serialization only
	CRayView();
	DECLARE_DYNCREATE(CRayView)

// Attributes
public:
	CRayDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRayView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRayView)
	afx_msg void OnScene();
	afx_msg void OnSphere();
	afx_msg void OnTriangle();
	afx_msg void OnRender();
	afx_msg void OnRectangle();
	afx_msg void OnConelight();
	afx_msg void OnCylinder();
	afx_msg void OnDirectedlight();
	afx_msg void OnEllipsoid();
	afx_msg void OnPointlight();
	afx_msg void OnPlay();
	afx_msg void OnWater();
	afx_msg void OnClouds();
	afx_msg void OnHorizon();
	afx_msg void OnMarble();
	afx_msg void OnCloudmacro();
	afx_msg void OnTerrainmacro();
	afx_msg void OnMakroBendaPutar();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RayView.cpp
inline CRayDoc* CRayView::GetDocument()
   { return (CRayDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAYVIEW_H__331ABD86_67AA_11D6_B72E_E77FF821D736__INCLUDED_)
