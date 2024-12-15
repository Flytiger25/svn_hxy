#if !defined(AFX_COLORFORMVIEW_H__B8F746CD_922D_11D2_B166_00C04FB9CA2B__INCLUDED_)
#define AFX_COLORFORMVIEW_H__B8F746CD_922D_11D2_B166_00C04FB9CA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CColorFormView : public CFormView
{
	COLORREF m_clrFVBkgnd;
	COLORREF m_clrEdit;
	COLORREF m_clrListBox;
	COLORREF m_clrText;

	CBrush m_BrushFVBkgnd;
	CBrush m_BrushEdit;
	CBrush m_BrushListBox;

protected:
	CColorFormView(UINT nIDTemplate, COLORREF clr); 
	CColorFormView(LPCTSTR lpszTemplateName, COLORREF clr);

	DECLARE_DYNAMIC(CColorFormView) 

// Form Data
public:
	//{{AFX_DATA(CColorFormView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorFormView)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CColorFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CColorFormView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORFORMVIEW_H__B8F746CD_922D_11D2_B166_00C04FB9CA2B__INCLUDED_)
