// GeometricEngineSelect.cpp: 实现文件
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "afxdialogex.h"
#include "GeometricEngineSelect.h"


// GeometricEngineSelect 对话框

IMPLEMENT_DYNAMIC(GeometricEngineSelect, CDialog)

GeometricEngineSelect::GeometricEngineSelect(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG10, pParent)
{
	m_acis = 1;
}

GeometricEngineSelect::~GeometricEngineSelect()
{
}

void GeometricEngineSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENGINE_ACIS, m_Acis_Button);
}


BEGIN_MESSAGE_MAP(GeometricEngineSelect, CDialog)
	ON_BN_CLICKED(IDOK, &GeometricEngineSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// GeometricEngineSelect 消息处理程序


void GeometricEngineSelect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_acis = m_Acis_Button.GetCheck();
	CDialog::OnOK();
}


BOOL GeometricEngineSelect::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Acis_Button.SetCheck(TRUE);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
