#if !defined(AFX_RENDERWINDOW_H__EEF31EC0_67F4_11D6_B72E_D2ECB32BEF55__INCLUDED_)
#define AFX_RENDERWINDOW_H__EEF31EC0_67F4_11D6_B72E_D2ECB32BEF55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenderWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RenderWindow dialog

class RenderWindow : public CDialog
{
// Construction
public:
	RenderWindow(CWnd* pParent = NULL);   // standard constructor
	CDC* AskDC();

// Dialog Data
	//{{AFX_DATA(RenderWindow)
	enum { IDD = ID_RENDERSCENE };
	CEdit	m_UpZ;
	CEdit	m_UpY;
	CEdit	m_UpX;
	CEdit	m_FromZ;
	CEdit	m_FromY;
	CEdit	m_FromX;
	CEdit	m_AtZ;
	CEdit	m_AtY;
	CEdit	m_AtX;
	CEdit	m_Render_Time;
	CStatic	m_Render_Image;
	int m_Count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RenderWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RenderWindow)
	afx_msg void OnTrace();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENDERWINDOW_H__EEF31EC0_67F4_11D6_B72E_D2ECB32BEF55__INCLUDED_)
