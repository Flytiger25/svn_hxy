#include "stdafx.h"
#include "Action_ReverseEngineering.h"
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

IMPLEMENT_DYNAMIC(Action_ReverseEngineering, CPropertyPage)
Action_ReverseEngineering::Action_ReverseEngineering()
{

}

Action_ReverseEngineering::~Action_ReverseEngineering()
{

}


void Action_ReverseEngineering::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_ReverseEngineering, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_FIXGAP, &Action_ReverseEngineering::OnBnClickedFixGap)
	ON_BN_CLICKED(IDC_BUTTON_ACIS_TWOSURFACE, &Action_ReverseEngineering::OnBnClickedLoadTwoSurfaces)
	ON_BN_CLICKED(IDC_BUTTON_GenerateSmallerMesh, &Action_ReverseEngineering::OnBnClickedButtonGeneratesmallermesh)
	ON_BN_CLICKED(IDC_BUTTON_DetectPlane, &Action_ReverseEngineering::OnBnClickedButtonDetectplane)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &Action_ReverseEngineering::OnNMThemeChangedScrollbar2)
END_MESSAGE_MAP()


/*
int test_gaps()
{
	unlock_license();

	//// 启动ACIS模型处理器
	api_start_modeller(0);

	// 初始化操作结果为成功
	SPAIResult result = SPAI_S_OK;

	outcome api_result;
	{

		// 指向ACIS实体列表的指针
		ENTITY_LIST* pAcisEntities = new ENTITY_LIST();
		ENTITY_LIST* pAcisEntities1 = new ENTITY_LIST();
		{
			// 目标ACIS文档对象
			SPAIAcisDocument dst;
			// 创建源文档对象，并设置输入格式
			SPAIDocument src("D:\\Desktop\\step\\untighten.step");
			src.SetType("step");

			// 创建转换器对象，开始转换过程
			SPAIConverter converter;
			//将源文件文档转换为目标Acis文档对象
			result &= converter.Convert(src, dst);

			// 获取转换后的实体列表
			dst.GetEntities(pAcisEntities);

			// 检查实体列表是否有效，进行后续操作
			if ((pAcisEntities == nullptr) && (pAcisEntities->count() == 0))
			{
				std::cerr << "No entities available for tightening gaps." << std::endl;
				result = SPAI_E_NO_ENTITIES;
			}
			else
			{
				std::cout << "获取实体成功。" << std::endl;
				std::cout << "获得实体的数量为：" << pAcisEntities->iteration_count() << std::endl;
				int count1 = pAcisEntities->count();
				int count2 = pAcisEntities->iteration_count();
				const char* name1 = pAcisEntities->first()->type_name();
				const char* name2 = pAcisEntities->next()->type_name();
				BODY* pBody = (BODY*)pAcisEntities->first();

				const char* name3 = pAcisEntities->next()->type_name();

				AcisOptions* aopts = NULL;
				stitch_input_handle* sih = NULL;
				stitch_output_handle* soh = NULL;
				mt_stitch_options* sop = new mt_stitch_options();
				sop->set_max_stitch_tol(0.1); // set max stitch tol
				sop->set_heal_type(heal_option::HEAL_ALL); // set heal option
				sop->set_validation_type(validation_option::SHEETS_AND_SOLIDS); // set validation option
				api_stitch_make_input(*pAcisEntities, sih, sop, aopts);
				api_result = api_stitch(sih, soh, *pAcisEntities1, sop, aopts);
				if (api_result.ok() == 0)
				{
					std::cerr << "输入stich的实体数量：" << pAcisEntities->iteration_count() << std::endl;
					std::cerr << "输出stich的实体数量：" << pAcisEntities1->iteration_count() << std::endl;
				}
				check_outcome(api_result);
				ACIS_DELETE sih; sih = NULL;
				ACIS_DELETE soh; soh = NULL;

				//清理实体列表资源
				ACIS_DELETE(pAcisEntities), pAcisEntities = NULL;
				//设置并执行紧密缝隙操作
				tighten_gaps_options* tgopts = nullptr; // 指向 tighten_gaps_options 的指针
				AcisOptions* ao = nullptr;
				tgopts = ACIS_NEW tighten_gaps_options();
				if (tgopts)
				{
					double gap_tightness_val = 0.0001;
					tgopts->set_desired_tightness_value(gap_tightness_val);
					API_BEGIN
						api_result = api_tighten_gaps(*pAcisEntities1, tgopts, ao);

					if (!api_result.ok())
					{
						std::cerr << "Tighten Gaps operation failed." << std::endl;
						check_outcome(api_result);
					}
					else
					{
						std::cerr << "Tighten Gaps operation successed." << std::endl;
						std::cerr << "Tighten Gaps后的实体数量为：" << pAcisEntities1->iteration_count() << std::endl;
					}
					API_END
				}
			}
			SPAIAcisDocument dst1(pAcisEntities1);
			SPAIDocument dstnew("D:\\Desktop\\tighten.step");
			dstnew.SetType("step");
			result &= converter.Convert(dst1, dstnew);

			//清理实体列表资源
			ACIS_DELETE(pAcisEntities1), pAcisEntities1 = NULL;
		}
	}
	// 停止ACIS模型处理器
	api_stop_modeller();
	return result;
}
*/
void Action_ReverseEngineering::OnBnClickedLoadTwoSurfaces()
{
	CSceneGraph3d m_SceneGraph;
	CParseSTEP CParseStep;
	CParseStep.Run_STEP("D:\\Desktop\\step\\untighten.step", &m_SceneGraph);
	//// 检查 Run_STEP 是否成功
	//if (result == 1)
	//{
	//	// 成功加载后绘制场景图
	//	m_SceneGraph.glDraw();
	//	AfxMessageBox("Run_Step Succeed");
	//}
	//else
	//{
	//	// 错误处理
	//	AfxMessageBox("Run_Step Failed");
	//}
}
void Action_ReverseEngineering::OnBnClickedFixGap()
{
	// 函数声明，用于检查操作结果是否成功，并处理错误    
	//int gaps = test_gaps();
	//if (gaps == SPAI_S_OK)
	//	AfxMessageBox("Gaps Succeed");
	//else  AfxMessageBox("Gaps Failed");

}
extern CSDIViewSwitchDoc* pDoc;
void Action_ReverseEngineering::OnBnClickedButtonGeneratesmallermesh()
{
	// TODO: 在此添加控件通知处理程序代码 
	int num = pDoc->m_SceneGraph.NbObject();

	if (pDoc->m_CurrentObjectIndex < 0 || pDoc->m_CurrentObjectIndex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	Point3D lower, upper, lower1, upper1;
	pMesh->ComputeBoundingBox(lower, upper);

	int ratio = 3;
	lower1.setValue(lower.x, lower.y + (upper.y - lower.z) * (10 - ratio) / 10.0, lower.z);
	upper1.setValue(lower.x + (upper.x - lower.x) * (ratio) / 10.0, upper.y, upper.z);

	CMesh3d* pNewMesh = pMesh->SubtructSmallerMesh(lower1, upper1);
	pDoc->m_SceneGraph.Add(pNewMesh);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_ReverseEngineering::OnBnClickedButtonDetectplane()
{
	// TODO: 在此添加控件通知处理程序代码
	int num = pDoc->m_SceneGraph.NbObject();

	if (pDoc->m_CurrentObjectIndex < 0 || pDoc->m_CurrentObjectIndex >= num)
		return;

	CMesh3d* pMesh = (CMesh3d*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->GetWindowText(str);
	double angletolerance = atof(str);
	((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->GetWindowText(str);
	int  groupnumber = atoi(str);

	pMesh->DetectTriangleType(angletolerance, groupnumber);

	pDoc->UpdateAllViews(NULL);
}


void Action_ReverseEngineering::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL Action_ReverseEngineering::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
