#pragma once
#include <afxdlgs.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "ap.h"
#include "resource.h"
#include <afxdlgs.h> 
#include <afx.h>
class Action_acis_sewing :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_acis_sewing)

public:
	Action_acis_sewing();
	virtual ~Action_acis_sewing();

	// Dialog Data
	enum { IDD = IDD_DIALOG_ACIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadTwoSurfaces();
	afx_msg void OnBnClickedFixGap();
public:
};

