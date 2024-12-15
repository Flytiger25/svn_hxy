#pragma once
#include "afxdialogex.h"


// GeometricEngineSelect 对话框

class GeometricEngineSelect : public CDialog
{
	DECLARE_DYNAMIC(GeometricEngineSelect)

public:
	int m_acis;
	GeometricEngineSelect(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~GeometricEngineSelect();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Acis_Button;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
