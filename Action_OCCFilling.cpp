#include "stdafx.h"
#include "Action_OCCFilling.h"
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


IMPLEMENT_DYNAMIC(Action_OCCFilling, CPropertyPage)
Action_OCCFilling::Action_OCCFilling()
{

}

Action_OCCFilling::~Action_OCCFilling()
{

}


void Action_OCCFilling::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_OCCFilling, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ACIS_CHECK_ENTITY, &Action_OCCFilling::OnBnClickedCheckEntity)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &Action_OCCFilling::OnNMThemeChangedScrollbar2)
	ON_BN_CLICKED(IDC_BUTTON_OCCFILLING, &Action_OCCFilling::OnBnClickedButtonOccfilling)
END_MESSAGE_MAP()

extern CSDIViewSwitchDoc* pDoc;
void Action_OCCFilling::OnBnClickedCheckEntity()
{
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_BREP_ACIS)
		return;

	EntityList_ACIS* pEntityList = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(0);

	insanity_list* issues = NULL;        // 初始化问题列表
	AcisOptions* options = NULL;         // 可以设置选项，如果有特定需求

	// 检查实体
	ENTITY* my_entity;
	const char* name;
	int sizebeforestitch = pEntityList->pAcisEntities->count();
	for (auto ent = pEntityList->pAcisEntities->first(); ent; ent = pEntityList->pAcisEntities->next())
	{
		name = ent->type_name();
	}
	int sizeafterstitch = sizebeforestitch;
	if (sizebeforestitch > 1)
	{
		//如果是多个实体，则首先进行缝合操作
		AcisOptions* aopts = NULL;
		stitch_input_handle* sih = NULL;
		stitch_output_handle* soh = NULL;
		mt_stitch_options* sop = new mt_stitch_options();
		ENTITY_LIST* pAcisEntities1 = new ENTITY_LIST();
		api_stitch_make_input(*pEntityList->pAcisEntities, sih, sop, aopts);
		outcome api_result = api_stitch(sih, soh, *pAcisEntities1, sop, aopts);
		sizeafterstitch = pAcisEntities1->iteration_count();
		my_entity = pAcisEntities1->first();
	}
	else
	{
		my_entity = pEntityList->pAcisEntities->first();
	}
	AcisOptions opt;
	api_set_int_option("check_level", 70);
	api_set_int_option("check_edge_on_face", 1);
	outcome result = api_check_entity(my_entity, issues);
	if (result.ok())
	{
		printf("api_check_entity调用成功");
	}
	insanity_list* this_list = issues;


	int size = this_list->count();
	insanity_data* data;
	char message[200];
	AcisCheckInformation dlg;

	sprintf(message, "一共%d个模型，缝合后%d个模型\r\n-----错误信息-------\r\n", sizebeforestitch, sizeafterstitch);
	dlg.m_surfaceinformation = message;
	size = this_list->count();
	sprintf(message, "一共%d条错误信息\r\n", size);
	dlg.m_surfaceinformation.Insert(dlg.m_surfaceinformation.GetLength(), message);

	int index = 1;
	for (; this_list; this_list = this_list->next())
	{
		//for (int i = 0; i < size; i++)
		{
			data = this_list->data();
			if (data)
			{
				//dlg.m_surfaceinformation.
				//	AfxMessageBox(data->get_message());
				sprintf(message, "错误信息 %d: 错误类型%d, ID: %d,%s;%s\r\n", index, data->get_type(), data->get_insane_id(), data->get_message(), data->get_aux_msg());
				dlg.m_surfaceinformation.Insert(dlg.m_surfaceinformation.GetLength(), message);
				index++;
			}
		}
	}
	dlg.DoModal();
	//AcisCheckInformation dlg;
	//dlg.m_surfaceinformation =
		//if (data && data->get_type() == ERROR_TYPE)
		//{

		//	//Now pick out the particular errors that we are looking for. 
		//	int insane_id = data->get_insane_id();

		//	if (SURF_NOT_G0 == insane_id) {

		//		//Report that the entity has an irregular surface. 
		//		/*We print a slightly different message depending on
		//		whether the entity with the bad surface is the one
		//		we checked, or one of its children. */
		//		if (data->get_ent() == pEntity)
		//		{
		//			AfxMessageBox("Gaps Succeed");

		//		}
		//		else {
		//			printf("Surface of entity 0x%x (owning entity 0x%x) is irregular.",
		//				data->get_ent(), pEntity);
		//		}

		//		/*Now check whether the irregularity is due to
		//		extreme curvature on the surface. */
		//		if (data->get_sub_category() == HIGH_CURVATURE) {

		//			//It is true. Report it.                    
		//			printf(" Surface has high curvature");

		//			/*Check if the data includes a parameter
		//			position indicating the region of high curvature.
		//			get_par_pos() sets uv and returns TRUE if
		//			a parameter position is stored. */
		//		}
		//	}
		//}
}
void Action_OCCFilling::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL Action_OCCFilling::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	//((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Action_OCCFilling::OnBnClickedButtonOccfilling()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox("n-sided hole filling");
}
