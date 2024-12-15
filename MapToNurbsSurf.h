#if !defined(AFX_MAPTONURBSSURF_H__407A5354_4D2D_4ADF_BF0A_D5DEDCC815F3__INCLUDED_)
#define AFX_MAPTONURBSSURF_H__407A5354_4D2D_4ADF_BF0A_D5DEDCC815F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapToNurbsSurf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MapToNurbsSurf dialog

class MapToNurbsSurf : public CDialog
{
// Construction
public:
	MapToNurbsSurf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MapToNurbsSurf)
	enum { IDD = IDD_DIALOG2 };
	CString	m_surf;
	CString	m_parameter;
	CString	m_mapresult;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapToNurbsSurf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MapToNurbsSurf)
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPTONURBSSURF_H__407A5354_4D2D_4ADF_BF0A_D5DEDCC815F3__INCLUDED_)
