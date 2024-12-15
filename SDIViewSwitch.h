// SDIViewSwitch.h : main header file for the SDIVIEWSWITCH application
//

#if !defined(AFX_SDIVIEWSWITCH_H__CB97F4C1_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
#define AFX_SDIVIEWSWITCH_H__CB97F4C1_1752_11D2_B134_00C04FB9CA2B__INCLUDED_

#include "SDIViewSwitchDoc.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSDIViewSwitchApp:
// See SDIViewSwitch.cpp for the implementation of this class

class CSDIViewSwitchApp : public CWinApp
{
public:
	CSDIViewSwitchApp();
	~CSDIViewSwitchApp();

	CString m_szIniFileSection;
	CString m_szIniFileEntry;

	UINT m_nNumberOfInitialRecords;

	CSDIViewSwitchDoc* m_pDoc;  //pointer to the app's one document

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIViewSwitchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSDIViewSwitchApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIVIEWSWITCH_H__CB97F4C1_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
