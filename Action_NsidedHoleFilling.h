#pragma once
#include <afxdlgs.h>
class Action_NsidedHoleFilling :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_NsidedHoleFilling)

public:
	Action_NsidedHoleFilling();
	virtual ~Action_NsidedHoleFilling();

	// Dialog Data
	enum { IDD = IDD_DIALOG_NSIDEDHOLEFILLING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckEntity();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNsidedholefilling();
};

