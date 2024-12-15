#pragma once
#include <afxdlgs.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "ap.h"
#include "resource.h"
#include <afxdlgs.h> 
#include <afx.h>
class Action_ReverseEngineering :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_ReverseEngineering)

public:
	Action_ReverseEngineering();
	virtual ~Action_ReverseEngineering();

	// Dialog Data
	enum { IDD = IDD_DIALOG_ReverseEngineering };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadTwoSurfaces();
	afx_msg void OnBnClickedFixGap();
public:
	afx_msg void OnBnClickedButtonGeneratesmallermesh();
	afx_msg void OnBnClickedButtonDetectplane();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
};

