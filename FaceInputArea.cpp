// FaceInputArea.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "FaceInputArea.h"


// FaceInputArea dialog

IMPLEMENT_DYNAMIC(FaceInputArea, CDialog)

FaceInputArea::FaceInputArea(CWnd* pParent /*=NULL*/)
	: CDialog(FaceInputArea::IDD, pParent)
{

}

FaceInputArea::~FaceInputArea()
{
}

void FaceInputArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FaceInputArea, CDialog)
	ON_BN_CLICKED(IDOK, &FaceInputArea::OnBnClickedOk)
END_MESSAGE_MAP()


// FaceInputArea message handlers

void FaceInputArea::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str;
	((CEdit* )GetDlgItem(IDC_EDITFACEAREA))->GetWindowText(str);
	area = atof(str);
	OnOK();
}
