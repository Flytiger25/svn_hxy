// Bspline.h : main header file for the BSPLINE application
//

#if !defined(AFX_BSPLINE_H__64736BB9_C093_4306_9645_D8CF903684A8__INCLUDED_)
#define AFX_BSPLINE_H__64736BB9_C093_4306_9645_D8CF903684A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBsplineApp:
// See Bspline.cpp for the implementation of this class
//

class CBsplineApp : public CWinApp
{
public:
	CBsplineApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBsplineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBsplineApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSPLINE_H__64736BB9_C093_4306_9645_D8CF903684A8__INCLUDED_)
