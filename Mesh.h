// Mesh.h : main header file for the MESH application
//

#if !defined(AFX_MESH_H__50ED6A28_C0C1_11D2_B4C7_006067306B48__INCLUDED_)
#define AFX_MESH_H__50ED6A28_C0C1_11D2_B4C7_006067306B48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMeshApp:
// See Mesh.cpp for the implementation of this class
//

class CMeshApp : public CWinApp
{
public:
	CMeshApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMeshApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESH_H__50ED6A28_C0C1_11D2_B4C7_006067306B48__INCLUDED_)
