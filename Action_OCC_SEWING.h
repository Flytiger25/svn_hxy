#pragma once
#include <afxdlgs.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "ap.h"
class Action_occ_sewing :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_occ_sewing)

public:
	Action_occ_sewing();
	virtual ~Action_occ_sewing();

	// Dialog Data
	enum { IDD = IDD_DIALOG_ACIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedFixGap();
};

