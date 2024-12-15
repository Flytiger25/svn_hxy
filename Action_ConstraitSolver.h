#pragma once
#include <afxdlgs.h>
class Action_ConstraitSolver :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_ConstraitSolver)

public:
	Action_ConstraitSolver();
	virtual ~Action_ConstraitSolver();

	// Dialog Data
	enum { IDD = IDD_DIALOG_CONSTRAINTSOLVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonAddconstraint();
};

