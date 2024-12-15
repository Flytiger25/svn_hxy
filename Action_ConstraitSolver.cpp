#include "stdafx.h"
#include "Action_ConstraitSolver.h"
//自己添加的头文件
#include<iostream>
#include<string>
#include <fstream>
#include <heal_api.hxx>
#include "mt_stitch_opts.hxx"
#include "mt_stitch_hndl.hxx"
#include <mt_stitch_apis.hxx>
#include <body.hxx>
//#include "Lib3D/SceneGraph3d.h"
#include "CParseSTEP.h"
#include "Lib3D/SceneGraph3d.h"
#include "SDIViewSwitch.h"
#include "MyTreeView.h"
#include "FormView3.h"
#include "MainFrm.h"
#include "Lib3D/Mesh3d.h"
#include "Lib3D/Vertex3d.h"
#include "SDIViewSwitchDoc.h"
#include "Lib3D/Base3d.h"
#include <insanity_tbl.hxx>
#include <intrapi.hxx>
#include <insanity_list.hxx>
#include "AcisCheckInformation.h"
#include <kernapi.hxx>


IMPLEMENT_DYNAMIC(Action_ConstraitSolver, CPropertyPage)
Action_ConstraitSolver::Action_ConstraitSolver()
{

}

Action_ConstraitSolver::~Action_ConstraitSolver()
{

}


void Action_ConstraitSolver::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_ConstraitSolver, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ADDCONSTRAINT, &Action_ConstraitSolver::OnBnClickedButtonAddconstraint)
END_MESSAGE_MAP()

extern CSDIViewSwitchDoc* pDoc;

BOOL Action_ConstraitSolver::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	//((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}




extern HWND g_hwndMain;
extern char Post_Message[1000];
void Action_ConstraitSolver::OnBnClickedButtonAddconstraint()
{
	// TODO: 在此添加控件通知处理程序代码
	int size = pDoc->m_ArraySelectObjects.size();
	sprintf(Post_Message, "Select %d faces", size);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	//AfxMessageBox("constraint");
}
