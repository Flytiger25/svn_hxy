#pragma once
#include <afxdlgs.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "ap.h"
#include "resource.h"
#include <afxdlgs.h> 
#include <afx.h>
class Action_Intersection :
	public CPropertyPage
{

	DECLARE_DYNAMIC(Action_Intersection)

public:
	Action_Intersection();
	virtual ~Action_Intersection();


	// Dialog Data
	enum { IDD = IDD_DIALOG_SURFSURFINTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadTwoSurfaces();
	afx_msg void OnBnClickedFixGap();
public:
	afx_msg void OnBnClickedButton1LoadBoundaryExample1();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

};

