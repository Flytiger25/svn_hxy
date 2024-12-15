#pragma once
#include <afxdlgs.h>
class Action_OCCFilling :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_OCCFilling)

public:
	Action_OCCFilling();
	virtual ~Action_OCCFilling();

	// Dialog Data
	enum { IDD = IDD_DIALOG_OCCFILLING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckEntity();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOccfilling();
};

