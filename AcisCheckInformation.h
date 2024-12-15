#pragma once
#include "afxdialogex.h"


// AcisCheckInformation 对话框

class AcisCheckInformation : public CDialog
{
	DECLARE_DYNAMIC(AcisCheckInformation)

public:
	CString m_surfaceinformation;
	AcisCheckInformation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AcisCheckInformation();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG11 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	virtual BOOL OnInitDialog();
};
