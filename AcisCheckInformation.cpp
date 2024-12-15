// AcisCheckInformation.cpp: 实现文件
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "afxdialogex.h"
#include "AcisCheckInformation.h"
#include "resource.h"

// AcisCheckInformation 对话框

IMPLEMENT_DYNAMIC(AcisCheckInformation, CDialog)

AcisCheckInformation::AcisCheckInformation(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG11, pParent)
{

}

AcisCheckInformation::~AcisCheckInformation()
{
}

void AcisCheckInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AcisCheckInformation, CDialog)
END_MESSAGE_MAP()

void AcisCheckInformation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	//UpdateData(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SURFACEINFORMATION))->SetWindowText(m_surfaceinformation);
	//AfxGetDlgItem(IDC_EDIT_SURFACEINFORMATION)


}
// AcisCheckInformation 消息处理程序


BOOL AcisCheckInformation::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CEdit*)GetDlgItem(IDC_EDIT_SURFACEINFORMATION))->SetWindowText(m_surfaceinformation);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
