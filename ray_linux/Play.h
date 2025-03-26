#if !defined(AFX_PLAY_H__086BFA04_68E2_11D6_B72E_83F9C7DE1541__INCLUDED_)
#define AFX_PLAY_H__086BFA04_68E2_11D6_B72E_83F9C7DE1541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Play.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Play dialog

class Play : public CDialog
{
// Construction
public:
	Play(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Play)
	enum { IDD = IDD_PLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Play)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
// Attributes
public:
	DWORD m_NumFrames;
	char *m_PathName;
// Operations
protected:
	DWORD FrameNumber;
	SHORT ImWd,ImHt;
	HBITMAP hBitmap[100];
	HBITMAP hBmpTmp;
	HDC hDC,MemDC;
	BOOL First;

	RECT rect;
public:
	HWND hWnd;
protected:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	int readImage(char *Filenam,HDC MemDC);
protected:

	// Generated message map functions
	//{{AFX_MSG(Play)
	afx_msg void OnPlayback();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAY_H__086BFA04_68E2_11D6_B72E_83F9C7DE1541__INCLUDED_)
