// DialogInputInformation.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SDIViewSwitch.h"
#include "DialogInputInformation.h"


// DialogInputInformation dialog

IMPLEMENT_DYNAMIC(DialogInputInformation, CDialog)

DialogInputInformation::DialogInputInformation(CWnd* pParent /*=NULL*/)
	: CDialog(DialogInputInformation::IDD, pParent)
{

}

DialogInputInformation::~DialogInputInformation()
{
}

void DialogInputInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogInputInformation, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDVERTEX, &DialogInputInformation::OnBnClickedButtonAddvertex)
	ON_BN_CLICKED(IDC_BUTTON_ADDEDGE, &DialogInputInformation::OnBnClickedButtonAddedge)
END_MESSAGE_MAP()


// DialogInputInformation message handlers

void DialogInputInformation::OnBnClickedButtonAddvertex()
{
	// TODO: Add your control notification handler code here
	CString str;
	((CEdit* )GetDlgItem(IDC_EDITINPUTINFORMATION))->GetWindowText(str);
	int index = atoi(str);
	m_ArraySelectedVertex.push_back(index);
}

void DialogInputInformation::OnBnClickedButtonAddedge()
{
	// TODO: Add your control notification handler code here
	CString str;
	((CEdit* )GetDlgItem(IDC_EDITINPUTINFORMATION))->GetWindowText(str);
	int index = atoi(str);
	m_ArraySelectedEdge.push_back(index);
}
