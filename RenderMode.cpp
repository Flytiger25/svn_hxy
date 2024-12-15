// RenderMode.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "RenderMode.h"


// RenderMode dialog

IMPLEMENT_DYNAMIC(RenderMode, CDialog)

RenderMode::RenderMode(CWnd* pParent /*=NULL*/)
	: CDialog(RenderMode::IDD, pParent)
	, m_UsingOpenglNurbsRender(FALSE)
	, m_Tessellation(0)
{

}

RenderMode::~RenderMode()
{
}

void RenderMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_UsingOpenglNurbsRender);
	DDX_Slider(pDX, IDC_SLIDER1, m_Tessellation);
}


BEGIN_MESSAGE_MAP(RenderMode, CDialog)
END_MESSAGE_MAP()

BOOL RenderMode::OnInitDialog()
{
	UpdateData(FALSE);
	CDialog::OnInitDialog();
	return TRUE;
}