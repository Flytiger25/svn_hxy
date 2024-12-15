#include "stdafx.h"
#include "Action_SurfaceHealing.h"
#include "Tshape_occ.h"
//自己添加的头文件
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
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
#include <Lib3D/Base3d.h>
#include <insanity_tbl.hxx>
#include <intrapi.hxx>
#include <insanity_list.hxx>
#include "AcisCheckInformation.h"
#include <kernapi.hxx>
#include <iostream>
#include "StlAPI_Writer.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"
#include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <STEPControl_Writer.hxx>
#include <Standard_Type.hxx>
#include <Geom_BSplineCurve.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Quantity_Color.hxx>
#include <STEPControl_Reader.hxx>
#include <BRepTools.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <ShapeProcess_OperLibrary.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Quantity_Color.hxx>
#include <Prs3d_LineAspect.hxx>

#include <BRepBuilderAPI_Sewing.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <TColStd_Array1OfBoolean.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <set> 
#include "BRepExtrema_DistShapeShape.hxx"

#include <Poly_Triangulation.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include<TopoDS_Face.hxx>
#include<gp_Pnt.hxx>

#include <IGESControl_Reader.hxx>
#include"BRepSewing.h"
#include "EdgePair.h"
#include <freeglut_std.h>
#include "BRepBuilderAPI.hxx"
#include <BRep_TEdge.hxx>
#include <BRep_Curve3D.hxx>
#include <BRep_CurveOnSurface.hxx>
#include <BRep_CurveOn2Surfaces.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include "mysql.h"
#include <cstrapi.hxx>
#include "EDGE.hxx"
#include "vertex.hxx"
#include "point.hxx"
#include "edge.hxx"
#include "curve.hxx"
#include <Entity_FACE_ACIS.h>
#include <boolapi.hxx>
#include <SPAIDocument.h>
#include <SPAIAcisDocument.h>
#include <af_api.hxx>

#include "surface.hxx"
#include "surdef.hxx"
#include "af_api.hxx"
#include <Shape_EDGE_OCC.h>
#include <Shape_VERTEX_OCC.h>
#include <Entity_VERTEX_ACIS.h>
#include "point.hxx"
#include "vertex.hxx"
#include <exception>

#include <kernapi.hxx>
using namespace Eigen;

IMPLEMENT_DYNAMIC(Action_SurfaceHealing, CPropertyPage)
Action_SurfaceHealing::Action_SurfaceHealing()
{
	conn = NULL;
	theCurrentModelId = -1;
	theCurrentRowIndex = -1;
}

Action_SurfaceHealing::~Action_SurfaceHealing()
{

}


void Action_SurfaceHealing::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_SurfaceHealing, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ACIS_CHECK_ENTITY, &Action_SurfaceHealing::OnBnClickedCheckEntity)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &Action_SurfaceHealing::OnNMThemeChangedScrollbar2)
	ON_BN_CLICKED(IDC_BUTTON_HAUSDORFFDISTANCECHECK, &Action_SurfaceHealing::OnBnClickedButtonHausdorffdistancecheck)
	ON_BN_CLICKED(IDC_BUTTON_ACIS_CHECK_FILES, &Action_SurfaceHealing::OnBnClickedButtonAcisCheckFiles)
	ON_BN_CLICKED(IDC_BUTTON_FACELACKCHECK, &Action_SurfaceHealing::OnBnClickedButtonFacelackcheck)
	ON_BN_CLICKED(IDC_BUTTON_MYSQL, &Action_SurfaceHealing::OnBnClickedButtonMysql)
	ON_BN_CLICKED(IDC_BUTTON_FACELACKCHECK_AREA, &Action_SurfaceHealing::OnBnClickedButtonFacelackcheckArea)
	ON_BN_CLICKED(IDC_BUTTON_HAUSDORFFDISTANCE_ACIS, &Action_SurfaceHealing::OnBnClickedButtonHausdorffdistanceAcis)
	ON_BN_CLICKED(IDC_BUTTON_VERIFICATION_HAUSDORFFDISTANCE_ACIS, &Action_SurfaceHealing::OnBnClickedButtonVerificationHausdorffdistanceAcis)
	ON_BN_CLICKED(IDC_BUTTON_LOADFIRSTMODEL, &Action_SurfaceHealing::OnBnClickedButtonLoadfirstmodel)
	ON_BN_CLICKED(IDC_BUTTON_LOADNEXTMODEL, &Action_SurfaceHealing::OnBnClickedButtonLoadnextmodel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFYMODELDESCRIPTION, &Action_SurfaceHealing::OnBnClickedButtonModifymodeldescription)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOHUAWEIGAPDATASET, &Action_SurfaceHealing::OnBnClickedButtonAddtohuaweigapdataset)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOHUAWEILackDATASET, &Action_SurfaceHealing::OnBnClickedButtonAddtohuaweilackdataset)
	ON_BN_CLICKED(IDC_BUTTON_LOADTHEMODEL, &Action_SurfaceHealing::OnBnClickedButtonLoadthemodel)
	ON_BN_CLICKED(IDC_BUTTON_LOADTHEACISMODEL, &Action_SurfaceHealing::OnBnClickedButtonLoadtheacismodel)
	ON_BN_CLICKED(IDC_BUTTON_ADDTOFACELACKSET, &Action_SurfaceHealing::OnBnClickedButtonAddtofacelackset)
	ON_BN_CLICKED(IDC_BUTTON_LOADTHEMODELWITHID, &Action_SurfaceHealing::OnBnClickedButtonLoadthemodelwithid)
	ON_BN_CLICKED(IDC_BUTTON_FACELACKAUTOMATIC, &Action_SurfaceHealing::OnBnClickedButtonFacelackautomatic)
	ON_BN_CLICKED(IDC_BUTTON_GAPDETECTIONAUTOMATIC, &Action_SurfaceHealing::OnBnClickedButtonGapdetectionautomatic)
END_MESSAGE_MAP()

extern CSDIViewSwitchDoc* pDoc;

void PrintEdgeStartPointsAndEndPoint(const TopoDS_Edge& edge) {
	// 获取边的起始点和终点
	TopoDS_Vertex v1, v2;
	TopExp::Vertices(edge, v1, v2);

	//获取起始点坐标
	gp_Pnt p1 = BRep_Tool::Pnt(v1);
	//获取终止点坐标
	gp_Pnt p2 = BRep_Tool::Pnt(v2);

	// 输出起始点和终止点坐标
	std::cout << "Edge start point: ("
		<< p1.X() << ","
		<< p1.Y() << ","
		<< p1.Z() << ")" << "-->";
	std::cout << "Edge end point: ("
		<< p2.X() << ","
		<< p2.Y() << ","
		<< p2.Z() << ")" << std::endl;
}

void addEdgePairToContainer(const EdgePair& edgePair, std::vector<TopoDS_Edge>& findEdges) {
	if (!edgePair.edge1.IsNull() && !edgePair.edge2.IsNull()) {

		findEdges.push_back(edgePair.edge1);
		findEdges.push_back(edgePair.edge2);
	}
}

void SewingModelAndGetEdgePairs(Shape_OCC* pocc, double tolerance, double h_min, double h_max, int& edgePairCount) {

	//STEPControl_Reader reader;
	//reader.TransferRoots();
	//TopoDS_Shape shape;
	//this->processShape(shape); 
	//processShape(shape);

	//const TopoDS_Shape& shape = healer.m_shape;

	// 调用自己改写的sewing类
	BRepSewing sewing(tolerance);
	sewing.Add(pocc->shape);
	sewing.Perform();
	TopoDS_Shape sewedShape = sewing.SewedShape();

	// 两个旧边的pair,计算并保存了两个边的HausdorffDistance
	std::vector<EdgePair>& edgePairs = sewing.myEdgePairs;

	// 可以传递出来newEdge与oldEdges的map，oldEdge与newEdge的map,以便后续查找和更改
	TopTools_DataMapOfShapeListOfShape theNewEdgeOldEdgesMap = sewing.myNewEdgeOldEdges;
	TopTools_IndexedDataMapOfShapeShape theOldEdgeNewEdgeMap = sewing.myOldEdgeNewEdge;

	double maxDistance = std::numeric_limits<double>::lowest();
	double minDistance = std::numeric_limits<double>::max();
	int Haus = 0;
	for (const auto& edgePair : edgePairs) {
		if (!edgePair.edge1.IsNull() && !edgePair.edge2.IsNull()) {
			//对每一对hausdorff距离边起始点进行输出
			/*cout << "edge1: ";
			PrintEdgeStartPointsAndEndPoint(edgePair.edge1);
			cout << "edge2: ";
			PrintEdgeStartPointsAndEndPoint(edgePair.edge2);
			cout << "Hausdorff Distance: " << edgePair.myHausdorffDistances << endl;
			cout << "================================================" << endl;*/

			if (edgePair.myHausdorffDistances > maxDistance && edgePair.myHausdorffDistances >= 0) {
				maxDistance = edgePair.myHausdorffDistances;
			}
			if (edgePair.myHausdorffDistances < minDistance && edgePair.myHausdorffDistances >= 0) {
				minDistance = edgePair.myHausdorffDistances;
			}

			// 如果Hausdorff距离大于tolerance，则将边对保存到gapEdges,保存数值到gapEdgesHausdorffDistance
			if (edgePair.myHausdorffDistances > h_min && edgePair.myHausdorffDistances < h_max) {
				addEdgePairToContainer(edgePair, pocc->gapEdges);
				pocc->gapEdgesHausdorffDistance.push_back(edgePair.myHausdorffDistances);
				edgePairCount++;
			}
		}
	}

	/*cout << "Maximum Hausdorff Distance: " << maxDistance << endl;
	cout << "Minimum Hausdorff Distance: " << minDistance << endl;
	cout << "Number of edge pairs added to the container: " << edgePairCount << endl;*/
}

void Action_SurfaceHealing::OnBnClickedCheckEntity()
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
void Action_SurfaceHealing::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL Action_SurfaceHealing::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//Set hausdorff initial values
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_HAUSDORFF_MIN))->SetWindowTextA("0.001");
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_HAUSDORFF_MAX))->SetWindowTextA("10");

	//
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MIN))->SetWindowTextA("100");
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MAX))->SetWindowTextA("1000000");

	//((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	//((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
double Action_SurfaceHealing::ComputeHausdorffDistanceBetweenTwoPointSets(TColgp_Array1OfPnt& pts1, TColgp_Array1OfPnt& pts2)
{
	Standard_Integer npt = pts1.Size();

	// 初始化最小距离数组
	std::vector<Standard_Real> minDistances;
	Standard_Real minDist, dist;
	// 对于edge1上的每一个点，找到与edge2上点的最小距离
	for (Standard_Integer i = 1; i <= npt; ++i) {
		minDist = Precision::Infinite();
		for (Standard_Integer j = 1; j <= npt; ++j) {
			dist = pts1(i).Distance(pts2(j));
			minDist = std::min(minDist, dist);
		}
		minDistances.push_back(minDist); // 存储最小距离
		//// 输出每个点的最小距离
		//std::cout << "Minimum distance for point " << i << ": " << minDist << std::endl;
	}

	// 对于edge2上的每一个点，找到与edge1上点的最小距离
	for (Standard_Integer j = 1; j <= npt; ++j) {
		minDist = Precision::Infinite();
		for (Standard_Integer i = 1; i <= npt; ++i) {
			dist = pts2(j).Distance(pts1(i));
			minDist = std::min(minDist, dist);
		}
		minDistances.push_back(minDist); // 存储最小距离
		// 输出每个点的最小距离
		//std::cout << "Minimum distance for point " << j << " from edge2 to edge1: " << minDist << std::endl;
	}

	// 从最小距离数组中找到最大的最小距离和最小的最小距离
	double maxOfMinDistances = *std::max_element(minDistances.begin(), minDistances.end());
	return maxOfMinDistances;
}

//计算两个点集之间的hausdorff距离
double Action_SurfaceHealing::EvaluateDistanceBetween2CurveOnSurfaces_PointToPoint(BRep_CurveOnSurface* GC1, BRep_CurveOnSurface* GC2)
{
	const Standard_Integer npt = 100; // 点的离散化数量
	TColgp_Array1OfPnt pts1(1, npt), pts2(1, npt);

	Standard_Real first1, last1, first2, last2;

	// 获取两条边的3D曲线及其参数范围
	GC1->Range(first1, last1);
	GC2->Range(first2, last2);

	// 将曲线离散化为点
	Standard_Real deltaT1 = (last1 - first1) / (npt - 1);
	Standard_Real deltaT2 = (last2 - first2) / (npt - 1);
	for (Standard_Integer i = 1; i <= npt; ++i)
	{
		Standard_Real T1 = first1 + (i - 1) * deltaT1;
		Standard_Real T2 = first2 + (i - 1) * deltaT2;
		GC1->D0(T1, pts1(i));
		GC2->D0(T2, pts2(i));
	}

	return ComputeHausdorffDistanceBetweenTwoPointSets(pts1, pts2);

}

double Action_SurfaceHealing::EvaluateDistanceBetween2CurveOnSurfaces_PointDistanceToCurve(BRep_CurveOnSurface* GC1, BRep_CurveOnSurface* GC2)
{
	const Standard_Integer npt1 = 100; // 点的离散化数量
	const Standard_Integer npt2 = 100; // 点的离散化数量

	//根据曲线的长度和曲率计算对应的采样率，当前设定为常数

	TColgp_Array1OfPnt pts1(1, npt1), pts2(1, npt2);
	std::vector<double> pts1_para(npt1 + 1);
	std::vector<double> pts2_para(npt2 + 1);

	Standard_Real first1, last1, first2, last2;

	// 获取两条边的3D曲线及其参数范围
	GC1->Range(first1, last1);
	GC2->Range(first2, last2);

	// 将曲线离散化为点
	Standard_Real deltaT1 = (last1 - first1) / (npt1 - 1);

	//对第一条线均匀采样，后面可以修改为根据长度和曲率进行自适应调整
	for (Standard_Integer i = 1; i <= npt1; ++i)
	{
		Standard_Real T1 = first1 + (i - 1) * deltaT1;
		GC1->D0(T1, pts1(i));
		pts1_para[i] = T1;
	}

	//对第二条线均匀采样，后面可以修改为根据长度和曲率进行自适应调整
	Standard_Real deltaT2 = (last2 - first2) / (npt2 - 1);
	for (Standard_Integer i = 1; i <= npt2; ++i)
	{
		Standard_Real T2 = first2 + (i - 1) * deltaT2;
		GC2->D0(T2, pts2(i));
		pts2_para[i] = T2;
	}

	double closestparaonCurve2;

	double MaxMindistance1 = -1;
	double currentminDis;
	//计算curve1到curv2单向的hausdorff距离
	for (Standard_Integer i = 1; i <= npt1; ++i)
	{
		gp_Pnt P = pts1(i);
		gp_Pnt PClosetPointOnCurve2;
		//对Curv1其中的一个点P,计算其在Curv2上的最近点
		closestparaonCurve2 = GetClosestPointOnCurve(P, GC2, pts2, pts2_para);

		GC2->D0(closestparaonCurve2, PClosetPointOnCurve2);

		currentminDis = P.Distance(PClosetPointOnCurve2);
		if (currentminDis > MaxMindistance1)
			MaxMindistance1 = currentminDis;
	}

	double MaxMindistance2 = -1;
	double closestparaonCurve1;
	//计算curve2到curv1单向的hausdorff距离
	for (Standard_Integer i = 1; i <= npt2; ++i)
	{
		gp_Pnt P = pts2(i);
		gp_Pnt PClosetPointOnCurve1;
		//对Curv2其中的一个点P,计算其在Curv1上的最近点
		closestparaonCurve1 = GetClosestPointOnCurve(P, GC1, pts1, pts1_para);

		GC1->D0(closestparaonCurve1, PClosetPointOnCurve1);

		currentminDis = P.Distance(PClosetPointOnCurve1);
		if (currentminDis > MaxMindistance2)
			MaxMindistance2 = currentminDis;
	}

	double theHasudorffDis = Max(MaxMindistance1, MaxMindistance2);
	return theHasudorffDis;
}
double Action_SurfaceHealing::GetClosestPointOnCurve(const gp_Pnt& P, const BRep_CurveOnSurface* GC, TColgp_Array1OfPnt& pts, std::vector<double>& pts_para)
{
	//采样点个数
	int npt = pts.Size();

	// 初始化最小距离数组
	std::vector<Standard_Real> minDistances;
	Standard_Real minDist, dist;
	// 对于edge1上的每一个点，找到与edge2上点的最小距离

	// 获取曲线的参数范围
	Standard_Real first, last;
	GC->Range(first, last);

	minDist = Precision::Infinite();
	int closestpointindex;
	for (Standard_Integer j = 1; j <= npt; ++j)
	{
		dist = P.Distance(pts(j));
		if (dist < minDist)
		{
			minDist = dist;
			closestpointindex = j;
		}
	}

	//deltt = (P-Q)*V/(V*V)
	gp_Pnt Q; //当前点
	gp_Pnt2d p2d;
	gp_Vec2d V2d; //二维参数化向量

	//获取曲线的pcurve
	Handle(Geom2d_Curve) myPCurve = GC->PCurve();
	Handle(Geom_Surface) mySurface = GC->Surface();

	gp_Pnt2d p1;
	gp_Vec2d v1;
	gp_Pnt point3d;
	gp_Vec D1u, D1v;
	gp_Vec V;
	double deltaT;
	double currentPara = pts_para[closestpointindex];
	double nextPara;
	double ratio = 0.01;
	gp_Pnt currentParaPoint;
	gp_Pnt nextParaPoint;
	double currentdis, nextdis;
	//const double myDisToler = 1e-6;
	const double myDisToler = 1e-8;
	//const double myRatioToler = 1e-6;
	const double myRatioToler = 1e-8;
	const double myParaToler = 1e-8;
	const int maxIterations = 1000;
	int currentIterationNumber = 0;
	while (currentIterationNumber < maxIterations)
	{
		//计算参数区域pcurve中的点及向量
		myPCurve->D1(currentPara, p2d, V2d);

		//计算曲面的两个偏导
		mySurface->D1(p2d.X(), p2d.Y(), point3d, D1u, D1v);

		Q = point3d;

		//计算三维切平面中的曲线切向
		V = D1u * V2d.X() + D1v * V2d.Y();

		//用直线局部逼近曲线（一阶逼近），计算对应的参数变换
		deltaT = (P.XYZ() - Q.XYZ()) * V.XYZ() / (V.XYZ() * V.XYZ());

		//deltaT = deltaT / V2d.Magnitude();
		ratio = 1;
		//减少步长
		//while (fabs(deltaT * ratio) > myParaToler)
		while (1)
		{
			nextPara = currentPara + deltaT * ratio;

			if (nextPara < first || nextPara > last)
			{
				ratio = ratio * 0.1;

				if (ratio < myRatioToler)
					return currentPara;

				continue;
			}

			GC->D0(currentPara, currentParaPoint);
			GC->D0(nextPara, nextParaPoint);

			currentdis = P.Distance(currentParaPoint);
			nextdis = P.Distance(nextParaPoint);

			if (fabs(currentdis - nextdis) < myDisToler)
				return currentPara;

			if (nextdis < currentdis)
				break;

			ratio = ratio * 0.1;

			//if (ratio < myRatioToler)
			//	return currentPara;
		}

		//if (fabs(deltaT * ratio) < myParaToler)
		//{
		//	return currentPara;
		//}

		currentPara = nextPara;
		currentIterationNumber++;
	}

	return currentPara;
}
double Action_SurfaceHealing::EvaluateDistanceBetween2CurveOnSurfaces(BRep_Curve3D* geomCurve, BRep_CurveOnSurface* GC, BRep_CurveOn2Surfaces* CurveOn2Surfaces)
{
	const Standard_Integer npt = 100; // 点的离散化数量
	TColgp_Array1OfPnt pts1(1, npt), pts2(1, npt);

	Standard_Real first1, last1, first2, last2;

	// 获取两条边的3D曲线及其参数范围
	geomCurve->Range(first1, last1);
	GC->Range(first2, last2);

	// 将曲线离散化为点
	Standard_Real deltaT1 = (last1 - first1) / (npt - 1);
	Standard_Real deltaT2 = (last2 - first2) / (npt - 1);
	double maxdis = -1;
	double dis;
	for (Standard_Integer i = 1; i <= npt; ++i) {
		Standard_Real T1 = first1 + (i - 1) * deltaT1;
		Standard_Real T2 = first2 + (i - 1) * deltaT2;
		geomCurve->D0(T1, pts1(i));
		GC->D0(T2, pts2(i));

		//project pts1(i) on another surface
		Handle(Geom_Surface) surface = CurveOn2Surfaces->Surface();
		Handle(Geom_Surface) surface2 = CurveOn2Surfaces->Surface2();
		Handle(Geom_Surface) GCsurface = GC->Surface();
		Handle(Geom_Surface) anothersurface;
		if (surface == GCsurface)
			anothersurface = surface2;
		else
			anothersurface = surface;

		GeomAPI_ProjectPointOnSurf pointprosurface(pts1(i), anothersurface);
		if (pointprosurface.NbPoints() > 1)
		{

			int ij = 0;

		}
		else
			if (pointprosurface.NbPoints() == 1)
			{
				gp_Pnt projectpoint = pointprosurface.Point(1);
				dis = projectpoint.Distance(pts1(i));
				if (dis > maxdis)
					maxdis = dis;
			}
	}

	return ComputeHausdorffDistanceBetweenTwoPointSets(pts1, pts2);

}

double Action_SurfaceHealing::EvaluateDistanceBetweenCurveson2PlanarSurfaces(BRep_Curve3D* geomCurve, BRep_CurveOn2Surfaces* CurveOn2Surfaces)
{
	const Standard_Integer npt = 100; // 点的离散化数量
	TColgp_Array1OfPnt pts1(1, npt), pts2(1, npt), pts3(1, npt);

	Standard_Real first1, last1;

	// 获取两条边的3D曲线及其参数范围
	geomCurve->Range(first1, last1);

	// 将曲线离散化为点
	Standard_Real deltaT1 = (last1 - first1) / (npt - 1);
	double maxdis = -1;
	double dis;
	for (Standard_Integer i = 1; i <= npt; ++i) {
		Standard_Real T1 = first1 + (i - 1) * deltaT1;
		geomCurve->D0(T1, pts1(i));

		pts2(i) = pts3(i) = pts1(i);
		//project pts1(i) on another surface
		Handle(Geom_Surface) surface1 = CurveOn2Surfaces->Surface();
		Handle(Geom_Surface) surface2 = CurveOn2Surfaces->Surface2();

		GeomAPI_ProjectPointOnSurf pointprosurface1(pts1(i), surface1);
		if (pointprosurface1.NbPoints() > 1)
		{
			int ij = 0;
		}
		else
			if (pointprosurface1.NbPoints() == 1)
			{
				pts2(i) = pointprosurface1.Point(1);
			}

		GeomAPI_ProjectPointOnSurf pointprosurface2(pts1(i), surface2);
		if (pointprosurface2.NbPoints() > 1)
		{
			int ij = 0;
		}
		else
			if (pointprosurface2.NbPoints() == 1)
			{
				pts3(i) = pointprosurface2.Point(1);

			}
	}

	return ComputeHausdorffDistanceBetweenTwoPointSets(pts2, pts3);

}

int Action_SurfaceHealing::CheckModelFaceLack_OCC_AREA(Shape_OCC* pocc, double h_min, double h_max)
{
	pocc->lackFaceBoundaries.clear();
	pocc->theFaceLackBelowMinArea = 0;
	pocc->theFaceLackBetweenMinAreaAndMaxArea = 0;
	pocc->theFaceLackAboveMaxArea = 0;

	//check whether to sew it to build the topology
	double tolerance = 0.5;

	TopAbs_ShapeEnum type = pocc->shape.ShapeType();

	//if has face and has no shell and above topology, sew it. 
	//if (!pocc->ChechWhetherExistTopology())
	//{
	//	BRepBuilderAPI_Sewing sewing(tolerance);
	//	sewing.Add(pocc->shape);
	//	sewing.Perform();
	//	TopoDS_Shape sewedShape = sewing.SewedShape();
	//	pocc->shape = sewedShape;
	//}

	std::vector<std::pair<TopoDS_Edge, TopoDS_Face>> edgeArray;
	//std::vector<TopoDS_Face> edgeArray_locatedFace;
	TopExp_Explorer faceexplorer(pocc->shape, TopAbs_FACE);
	int edgeindex;
	int faceindex;
	for (; faceexplorer.More(); faceexplorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(faceexplorer.Current());
		faceindex = pocc->getOCCFaceIndex(face);

		TopExp_Explorer edgeexplorer(face, TopAbs_EDGE);

		for (; edgeexplorer.More(); edgeexplorer.Next())
		{
			TopoDS_Edge edge = TopoDS::Edge(edgeexplorer.Current());

			Handle(TopoDS_TShape) tshape = edge.TShape();

			edgeindex = pocc->getOCCEdgeIndex(edge);

			TopAbs_ShapeEnum type = tshape->ShapeType();
			if (type == TopAbs_EDGE)
			{
				// find the representation
				const BRep_TEdge* TE = static_cast<const BRep_TEdge*>(edge.TShape().get());
				BRep_ListIteratorOfListOfCurveRepresentation itcr(TE->Curves());
				TopLoc_Location L;
				std::vector<BRep_CurveOnSurface*> arrayCurveOnSurfaces;
				BRep_Curve3D* geomCurve = NULL;
				BRep_CurveOn2Surfaces* CurveOn2Surfaces = NULL;
				while (itcr.More()) {
					const Handle(BRep_CurveRepresentation)& cr = itcr.Value();
					if (cr->IsCurve3D())
					{
						geomCurve = static_cast<BRep_Curve3D*>(cr.get());
					}
					else
						if (cr->IsCurveOnSurface())
						{
							BRep_CurveOnSurface* GC = static_cast<BRep_CurveOnSurface*>(cr.get());
							arrayCurveOnSurfaces.push_back(GC);
							//L = edge.Location() * GC->Location();
							//GC->Range(first, last);
							//geomCurve = GC->Curve3D();
						}
						else
							if (cr->IsRegularity())
							{
								CurveOn2Surfaces = static_cast<BRep_CurveOn2Surfaces*>(cr.get());
							}
					itcr.Next();
				}
				//
				//if (!CurveOn2Surfaces && arrayCurveOnSurfaces.size() == 1)
				if (!CurveOn2Surfaces && arrayCurveOnSurfaces.size() == 0 || !CurveOn2Surfaces && arrayCurveOnSurfaces.size() == 1)
				{
					edgeindex = pocc->getOCCEdgeIndex(edge);
					faceindex = pocc->getOCCFaceIndex(face);
					edgeArray.push_back(std::make_pair(edge, face));

					//edgeArray_locatedFace.push_back(face);
				}

			}
		}
	}

	std::vector<std::pair<TopoDS_Edge, TopoDS_Face>> edgeArrayonCurrentBoundary;
	char message[2000];
	CString messstr;
	//std::vector<TopoDS_Face> edgeArrayonCurrentBoundary_locatedFace;
	while (edgeArray.size() > 0)
	{
		edgeArrayonCurrentBoundary.clear();

		std::pair<TopoDS_Edge, TopoDS_Face> edge = edgeArray[0];
		edgeindex = pocc->getOCCEdgeIndex(edge.first);
		//TopoDS_Edge edge = edgeArray[0];
		//TopoDS_Face face = edgeArray_locatedFace[0];
		//TopAbs_Orientation edgeori = BRepTools::OriEdgeInFace(edge, face);

		edgeArrayonCurrentBoundary.push_back(edge);

		edgeArray.erase(edgeArray.begin());

		//
		int flag;
		int currrentsize;

		while (1)
		{
			currrentsize = edgeArrayonCurrentBoundary.size();
			for (int i = 0; i < edgeArray.size(); i++)
			{
				edgeindex = pocc->getOCCEdgeIndex(edgeArray[i].first);
				flag = CheckWhetherEdgeBelongtoCurrentBoundary(pocc, edgeArray[i], edgeArrayonCurrentBoundary);

				if (flag)
				{
					edgeArrayonCurrentBoundary.push_back(edgeArray[i]);
					edgeArray.erase(edgeArray.begin() + i);
					i--;
				}
			}
			if (edgeArrayonCurrentBoundary.size() == currrentsize)
				break;
		}
		int size1 = edgeArrayonCurrentBoundary.size();

		//
		double area = ComputeAreaSurroundedbyCurrentBoundary(pocc, edgeArrayonCurrentBoundary);
		if (area < h_min)
		{
			pocc->theFaceLackBelowMinArea++;
			pocc->lackFaceBoundaries.push_back(edgeArrayonCurrentBoundary);
			pocc->lackFaceBoundariesArea.push_back(area);
		}
		else
			if (area >= h_min && area <= h_max)
			{
				pocc->lackFaceBoundaries.push_back(edgeArrayonCurrentBoundary);
				pocc->lackFaceBoundariesArea.push_back(area);
				pocc->theFaceLackBetweenMinAreaAndMaxArea++;
				sprintf(message, "Area: %f", area);

				messstr = messstr + message;
				messstr = messstr + "\r\n";
			}
			else
				if (area > h_max)
				{
					pocc->theFaceLackAboveMaxArea++;
					pocc->lackFaceBoundaries.push_back(edgeArrayonCurrentBoundary);
					pocc->lackFaceBoundariesArea.push_back(area);
				}

	}
	sprintf(message, "Total Lack Face Number%d", pocc->lackFaceBoundaries.size());
	messstr = messstr + message;
	messstr = messstr + "\r\n";

	GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION2)->SetWindowTextA(messstr.GetBuffer());
	return pocc->lackFaceBoundaries.size();
}

double Action_SurfaceHealing::ComputeAreaSurroundedbyCurrentBoundary(Shape_OCC* pocc, const std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary)
{
	std::vector<gp_Pnt> boundary_Sampling;
	std::vector<gp_Pnt2d> boundary_Sampling2D;
	double area1, area2, area3;

	//在当前边界上进行采点
	SamplingOpenBoundary(pocc, edgeArrayonCurrentBoundary, boundary_Sampling);

	pocc->curveSectionArray1_Sampling = boundary_Sampling;
	//return 0;
	//计算最小二乘拟合平面 z = a * x + b * y + c
	gp_Pnt point;
	gp_XYZ N, X, Y;
	ComputeLeastSquarePlaneof3DPolygon_SDM(boundary_Sampling, point, N, X, Y);
	pocc->theFittingPlanePoint = point;
	pocc->theFittingPlaneN = N;
	pocc->theFittingPlaneX = X;
	pocc->theFittingPlaneY = Y;

	//将三维多边形向拟合平面进行投影
	Project3DPolygontoPlane(boundary_Sampling, point, N, X, Y, boundary_Sampling2D);
	pocc->theFittingplaneProjectedPoints1 = boundary_Sampling2D;

	//用格林公式计算二维多边形的面积
	area1 = ComputeAreabyGreenTheorem(boundary_Sampling2D);

	boundary_Sampling2D.clear();
	Project3DPolygontoPlane(boundary_Sampling, point, X, Y, N, boundary_Sampling2D);
	pocc->theFittingplaneProjectedPoints2 = boundary_Sampling2D;

	//用格林公式计算二维多边形的面积
	area2 = ComputeAreabyGreenTheorem(boundary_Sampling2D);

	boundary_Sampling2D.clear();
	Project3DPolygontoPlane(boundary_Sampling, point, Y, N, X, boundary_Sampling2D);
	pocc->theFittingplaneProjectedPoints3 = boundary_Sampling2D;

	//用格林公式计算二维多边形的面积
	area3 = ComputeAreabyGreenTheorem(boundary_Sampling2D);

	double totalarea = fabs(area1) + fabs(area2) + fabs(area3);
	return totalarea;
}
double Action_SurfaceHealing::ComputeAreabyGreenTheorem(const std::vector<gp_Pnt2d>& boundary_Sampling2D)
{
	double area = 0;
	gp_Pnt2d point2d1, point2d2;
	int pointNum = boundary_Sampling2D.size();
	for (int i = 0; i < pointNum; i++)
	{
		point2d1 = boundary_Sampling2D[i];
		point2d2 = boundary_Sampling2D[(i + 1) % pointNum];
		area += point2d1.X() * point2d2.Y() - point2d2.X() * point2d1.Y();
	}
	area /= 2;
	return area;
}
int Action_SurfaceHealing::Project3DPolygontoPlane(const std::vector<gp_Pnt>& boundary_Sampling, gp_Pnt O, gp_XYZ N, gp_XYZ X, gp_XYZ Y, std::vector<gp_Pnt2d>& boundary_Sampling2D)
{
	//首先计算平面上的一个点
	int pointNum = boundary_Sampling.size();
	gp_Pnt point;
	gp_Pnt2d point2d;

	//P - (OP * N) * N
	for (int i = 0; i < boundary_Sampling.size(); i++)
	{
		point = boundary_Sampling[i];
		double value = (point.XYZ() - O.XYZ()) * N;
		point.SetXYZ(point.XYZ() - N * value);

		point2d.SetCoord((point.XYZ() - O.XYZ()) * X, (point.XYZ() - O.XYZ()) * Y);
		boundary_Sampling2D.push_back(point2d);
	}
	return 1;
}


int Action_SurfaceHealing::ComputeLeastSquarePlaneof3DPolygon_SDM(const std::vector<gp_Pnt>& boundary_Sampling, gp_Pnt& P, gp_XYZ& N, gp_XYZ& X, gp_XYZ& Y)
{
	//P平面上的一个点，N平面的法向
	P.SetCoord(0, 0, 0);
	gp_Pnt point;
	int num = boundary_Sampling.size();
	for (int i = 0; i < num; i++)
	{
		point = boundary_Sampling[i];
		P.SetCoord(P.X() + point.X(), P.Y() + point.Y(), P.Z() + point.Z());
	}

	P.SetCoord(P.X() / num, P.Y() / num, P.Z() / num);


	int m = 3;
	Eigen::MatrixXd A1 = Eigen::MatrixXd::Zero(m, m);

	double c00 = 0, c01 = 0, c02 = 0, c11 = 0, c12 = 0, c22 = 0;

	for (int i = 0; i < num; i++)
	{
		point = boundary_Sampling[i];


		c00 += (point.X() - P.X()) * (point.X() - P.X());
		c01 += (point.X() - P.X()) * (point.Y() - P.Y());
		c02 += (point.X() - P.X()) * (point.Z() - P.Z());;

		c11 += (point.Y() - P.Y()) * (point.Y() - P.Y());
		c12 += (point.Y() - P.Y()) * (point.Z() - P.Z());

		c22 += (point.Z() - P.Z()) * (point.Z() - P.Z());
	}

	A1(0, 0) = c00;
	A1(0, 1) = c01;
	A1(0, 2) = c02;

	A1(1, 0) = c01;
	A1(1, 1) = c11;
	A1(1, 2) = c12;

	A1(2, 0) = c02;
	A1(2, 1) = c12;
	A1(2, 2) = c22;

	EigenSolver<MatrixXd> eigensolver(A1);
	Eigen::VectorXcd E1 = eigensolver.eigenvalues();
	auto  E2 = eigensolver.eigenvectors();

	auto eigen1 = E1.col(0)[0];
	auto eigen2 = E1.col(0)[1];
	auto eigen3 = E1.col(0)[2];

	double eigenvalue1 = E1.col(0)[0].real();
	double eigenvalue2 = E1.col(0)[1].real();
	double eigenvalue3 = E1.col(0)[2].real();

	if (eigenvalue1 < eigenvalue2 && eigenvalue1 < eigenvalue3)
	{
		N.SetCoord(E2.col(0)[0].real(), E2.col(1)[0].real(), E2.col(2)[0].real());
		X.SetCoord(E2.col(0)[1].real(), E2.col(1)[1].real(), E2.col(2)[1].real());
		Y.SetCoord(E2.col(0)[2].real(), E2.col(1)[2].real(), E2.col(2)[2].real());
	}
	else if (eigenvalue2 < eigenvalue1 && eigenvalue2 < eigenvalue3)
	{
		N.SetCoord(E2.col(0)[1].real(), E2.col(1)[1].real(), E2.col(2)[1].real());
		Y.SetCoord(E2.col(0)[0].real(), E2.col(1)[0].real(), E2.col(2)[0].real());
		X.SetCoord(E2.col(0)[2].real(), E2.col(1)[2].real(), E2.col(2)[2].real());
	}
	else
	{
		N.SetCoord(E2.col(0)[2].real(), E2.col(1)[2].real(), E2.col(2)[2].real());
		Y.SetCoord(E2.col(0)[1].real(), E2.col(1)[1].real(), E2.col(2)[1].real());
		X.SetCoord(E2.col(0)[0].real(), E2.col(1)[0].real(), E2.col(2)[0].real());
	}

	return 1;
}
int Action_SurfaceHealing::ComputeLeastSquarePlaneof3DPolygon(const std::vector<gp_Pnt>& boundary_Sampling, double& a, double& b, double& c)
{
	//最小二乘能量方程
	// f = min\sum(a * xi + b * yi + c -zi)^2
	//计算f对a,b,c的偏导，使之为0，获得三个方程的系数
	// Equation 1: \partial f over \partial a = 0   
	// (\sum xi^2) a + (\sum xi * yi) * b + (\sum xi) * c - \sum xi * zi = 0
	// Equation 2: \partial f over \partial b = 0  
	// (\sum xi * yi) a + (\sum yi * yi) * b + (\sum yi) * c - \sum yi * zi = 0
	// Equation 3: \partial f over \partial c = 0
	// (\sum xi) a + (\sum yi) * b + m * c - \sum zi = 0
	//对称矩阵，因此只需要6个系数
	double c00 = 0, c01 = 0, c02 = 0, c11 = 0, c12 = 0, c22 = 0;
	double b0, b1, b2;

	int num = boundary_Sampling.size();
	gp_Pnt point;
	for (int i = 0; i < num; i++)
	{
		point = boundary_Sampling[i];

		c00 += point.X() * point.X();
		c01 += point.X() * point.Y();
		c02 += point.X();

		c11 += point.Y() * point.Y();
		c12 += point.Y();

		c22 += 1;

		b0 += point.X() * point.Z();
		b1 += point.Y() * point.Z();
		b2 += point.Z();
	}

	int m = 3;
	Eigen::MatrixXd A1 = Eigen::MatrixXd::Zero(m, m);

	Eigen::VectorXd b_ = Eigen::VectorXd::Zero(m);
	Eigen::VectorXd X;

	A1(0, 0) = c00;
	A1(0, 1) = c01;
	A1(0, 2) = c02;

	A1(1, 0) = c01;
	A1(1, 1) = c11;
	A1(1, 2) = c12;

	A1(2, 0) = c02;
	A1(2, 1) = c12;
	A1(2, 2) = c22;

	b_[0] = b0;
	b_[0] = b1;
	b_[0] = b2;

	X = A1.colPivHouseholderQr().solve(b_);

	a = X[0];
	b = X[1];
	c = X[2];

	return 1;
}
void Action_SurfaceHealing::SamplingOpenBoundary(Shape_OCC* pOCC, const std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary, std::vector<gp_Pnt>& boundary_Sampling)
{
	std::vector<gp_Pnt> tessellationpoints;
	for (int i = 0; i < edgeArrayonCurrentBoundary.size(); i++)
	{
		TopoDS_Edge edge = edgeArrayonCurrentBoundary[i].first;
		pOCC->get_tessellationpoints_from_edges_occ(edge, tessellationpoints);
		if (boundary_Sampling.size() == 0)
			boundary_Sampling.insert(boundary_Sampling.end(), tessellationpoints.cbegin(), tessellationpoints.cend());
		else
		{
			gp_Pnt point1 = boundary_Sampling[boundary_Sampling.size() - 1];
			gp_Pnt point2 = boundary_Sampling[0];
			gp_Pnt p1 = tessellationpoints[0];
			gp_Pnt p2 = tessellationpoints[tessellationpoints.size() - 1];
			double dis1 = point1.Distance(p1);
			double dis2 = point1.Distance(p2);

			double dis3 = point2.Distance(p1);
			double dis4 = point2.Distance(p2);

			if (dis1 <= dis2 + TOLER && dis1 <= dis3 + TOLER && dis1 <= dis4 + TOLER)
			{
				int size = tessellationpoints.size();
				for (int j = 1; j < size; j++)
				{
					boundary_Sampling.push_back(tessellationpoints[j]);
				}
			}
			else
				if (dis2 <= dis1 + TOLER && dis2 <= dis3 + TOLER && dis2 <= dis4 + TOLER)
				{
					int size = tessellationpoints.size();
					for (int j = size - 2; j >= 0; j--)
					{
						boundary_Sampling.push_back(tessellationpoints[j]);
					}
				}
				else if (dis3 <= dis1 + TOLER && dis3 <= dis2 + TOLER && dis3 <= dis4 + TOLER)
				{
					int size = tessellationpoints.size();
					for (int j = 1; j < size; j++)
					{
						boundary_Sampling.insert(boundary_Sampling.begin(), tessellationpoints[j]);
					}
				}
				else if (dis4 <= dis1 + TOLER && dis4 <= dis2 + TOLER && dis4 <= dis3 + TOLER)
				{
					int size = tessellationpoints.size();
					for (int j = size - 2; j >= 0; j--)
					{
						boundary_Sampling.insert(boundary_Sampling.begin(), tessellationpoints[j]);
					}
				}
		}
	}
	//for (int i)
	//	()
	//	double totallength = 0;
	//for (int i = 0; i < curveSectionArray.size(); i++)
	//{
	//	TopoDS_Edge edge = pOCC->arrayOCCEdges[i];
	//	GProp_GProps props;
	//	// 计算边的属性
	//	BRepGProp::LinearProperties(edge, props);
	//	// 获取边的长度
	//	totallength += props.Mass();
	//}

	//for (int i = 0; i <= SamplingNum; i++)
	//{
	//	double len = (double)i * totallength / SamplingNum;


	//}
	//delete the last one
	if (boundary_Sampling.size() > 0)
		boundary_Sampling.erase(boundary_Sampling.end() - 1);
}
int Action_SurfaceHealing::CheckModelFaceLack_OCC(Shape_OCC* pocc, int h_min, int h_max)
{
	pocc->lackFaceBoundaries.clear();

	//check whether to sew it to build the topology
	double tolerance = 0.5;

	TopAbs_ShapeEnum type = pocc->shape.ShapeType();

	//if has face and has no shell and above topology, sew it. 
	/*if (!pocc->ChechWhetherExistTopology())
	{
		BRepBuilderAPI_Sewing sewing(tolerance);
		sewing.Add(pocc->shape);
		sewing.Perform();
		TopoDS_Shape sewedShape = sewing.SewedShape();
		pocc->shape = sewedShape;
	}*/

	std::vector<std::pair<TopoDS_Edge, TopoDS_Face>> edgeArray;
	//std::vector<TopoDS_Face> edgeArray_locatedFace;
	TopExp_Explorer faceexplorer(pocc->shape, TopAbs_FACE);

	for (; faceexplorer.More(); faceexplorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(faceexplorer.Current());
		TopExp_Explorer edgeexplorer(face, TopAbs_EDGE);

		for (; edgeexplorer.More(); edgeexplorer.Next())
		{
			TopoDS_Edge edge = TopoDS::Edge(edgeexplorer.Current());

			Handle(TopoDS_TShape) tshape = edge.TShape();

			TopAbs_ShapeEnum type = tshape->ShapeType();
			if (type == TopAbs_EDGE)
			{
				// find the representation
				const BRep_TEdge* TE = static_cast<const BRep_TEdge*>(edge.TShape().get());
				BRep_ListIteratorOfListOfCurveRepresentation itcr(TE->Curves());
				TopLoc_Location L;
				std::vector<BRep_CurveOnSurface*> arrayCurveOnSurfaces;
				BRep_Curve3D* geomCurve = NULL;
				BRep_CurveOn2Surfaces* CurveOn2Surfaces = NULL;
				while (itcr.More()) {
					const Handle(BRep_CurveRepresentation)& cr = itcr.Value();
					if (cr->IsCurve3D())
					{
						geomCurve = static_cast<BRep_Curve3D*>(cr.get());
					}
					else
						if (cr->IsCurveOnSurface())
						{
							BRep_CurveOnSurface* GC = static_cast<BRep_CurveOnSurface*>(cr.get());
							arrayCurveOnSurfaces.push_back(GC);
							//L = edge.Location() * GC->Location();
							//GC->Range(first, last);
							//geomCurve = GC->Curve3D();
						}
						else
							if (cr->IsRegularity())
							{
								CurveOn2Surfaces = static_cast<BRep_CurveOn2Surfaces*>(cr.get());
							}
					itcr.Next();
				}
				//
				if (!CurveOn2Surfaces && arrayCurveOnSurfaces.size() == 1)
				{
					edgeArray.push_back(std::make_pair(edge, face));
					//edgeArray_locatedFace.push_back(face);
				}

			}
		}
	}

	std::vector<std::pair<TopoDS_Edge, TopoDS_Face>> edgeArrayonCurrentBoundary;
	//std::vector<TopoDS_Face> edgeArrayonCurrentBoundary_locatedFace;
	int edgeindex;
	while (edgeArray.size() > 0)
	{
		edgeArrayonCurrentBoundary.clear();

		std::pair<TopoDS_Edge, TopoDS_Face> edge = edgeArray[0];
		edgeindex = pocc->getOCCEdgeIndex(edge.first);
		//TopoDS_Edge edge = edgeArray[0];
		//TopoDS_Face face = edgeArray_locatedFace[0];
		//TopAbs_Orientation edgeori = BRepTools::OriEdgeInFace(edge, face);

		edgeArrayonCurrentBoundary.push_back(edge);

		edgeArray.erase(edgeArray.begin());

		//
		int flag;
		int currrentsize;

		while (1)
		{
			currrentsize = edgeArrayonCurrentBoundary.size();
			for (int i = 0; i < edgeArray.size(); i++)
			{
				edgeindex = pocc->getOCCEdgeIndex(edgeArray[i].first);
				flag = CheckWhetherEdgeBelongtoCurrentBoundary(pocc, edgeArray[i], edgeArrayonCurrentBoundary);

				if (flag)
				{
					edgeArrayonCurrentBoundary.push_back(edgeArray[i]);
					edgeArray.erase(edgeArray.begin() + i);
					i--;
				}
			}
			if (edgeArrayonCurrentBoundary.size() == currrentsize)
				break;
		}
		int size1 = edgeArrayonCurrentBoundary.size();
		if (size1 >= h_min && size1 <= h_max)
			pocc->lackFaceBoundaries.push_back(edgeArrayonCurrentBoundary);
	}

	return pocc->lackFaceBoundaries.size();
}
BOOL Action_SurfaceHealing::CheckWhetherEdgeBelongtoCurrentBoundary(Shape_OCC* pocc, std::pair<TopoDS_Edge, TopoDS_Face>& edge, std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>& edgeArrayonCurrentBoundary)
{

	//TopoDS_Edge edge = edge.first;
	//TopoDS_Face face = edge.second;
	TopAbs_Orientation edgeori = BRepTools::OriEdgeInFace(edge.first, edge.second);

	TopoDS_Vertex v1, v2, v3, v4;
	TopoDS_Vertex edgeEndV, nextEdgeStartV;
	TopExp::Vertices(edge.first, v1, v2);
	int v1index = pocc->getOCCVertexIndex(v1);
	int v2index = pocc->getOCCVertexIndex(v2);

	if (edgeori == TopAbs_FORWARD)
		edgeEndV = v1;
	else
		edgeEndV = v2;

	for (int i = 0; i < edgeArrayonCurrentBoundary.size(); i++)
	{
		std::pair<TopoDS_Edge, TopoDS_Face> e1 = edgeArrayonCurrentBoundary[i];

		TopExp::Vertices(e1.first, v3, v4);
		int v3index = pocc->getOCCVertexIndex(v3);
		int v4index = pocc->getOCCVertexIndex(v4);

		TopAbs_Orientation edgeori1 = BRepTools::OriEdgeInFace(e1.first, e1.second);

		if (edgeori1 == TopAbs_FORWARD)
			nextEdgeStartV = v4;
		else
			nextEdgeStartV = v3;

		//if (edgeEndV.IsSame(nextEdgeStartV) && !edge.second.IsSame(e1.second))
		//if (edgeEndV.IsSame(nextEdgeStartV))
		//if (edgeEndV.IsSame(v3) || edgeEndV.IsSame(v4))
		if (v1.IsSame(v3) || v1.IsSame(v4) || v2.IsSame(v3) || v2.IsSame(v4))
		{
			return TRUE;
		}
	}
	return FALSE;
}
int Action_SurfaceHealing::AddEndPointsandMiddlePoints(BRep_Curve3D* geomCurve, BRep_CurveOn2Surfaces* CurveOn2Surfaces, std::vector<gp_Pnt>& gapEdgePoints)
{

	Standard_Real first, last;

	// 获取两条边的3D曲线及其参数范围
	geomCurve->Range(first, last);

	gp_Pnt startpoint, middelpoint, endpoint;
	gp_Pnt propoint;

	//project pts1(i) on another surface
	Handle(Geom_Surface) surface1 = CurveOn2Surfaces->Surface();
	Handle(Geom_Surface) surface2 = CurveOn2Surfaces->Surface2();

	geomCurve->D0(first, startpoint);
	geomCurve->D0((first + last) / 2, middelpoint);
	geomCurve->D0(last, endpoint);

	GeomAPI_ProjectPointOnSurf pointprosurface1(startpoint, surface1);
	if (pointprosurface1.NbPoints() > 1)
	{
		//选择一个
		int ij = 0;
	}
	else
		if (pointprosurface1.NbPoints() == 1)
		{
			propoint = pointprosurface1.Point(1);
			gapEdgePoints.push_back(propoint);
		}

	GeomAPI_ProjectPointOnSurf pointprosurface2(middelpoint, surface1);
	if (pointprosurface2.NbPoints() > 1)
	{
		//选择一个
		int ij = 0;
	}
	else
		if (pointprosurface2.NbPoints() == 1)
		{
			propoint = pointprosurface2.Point(1);
			gapEdgePoints.push_back(propoint);
		}


	GeomAPI_ProjectPointOnSurf pointprosurface3(endpoint, surface1);
	if (pointprosurface3.NbPoints() > 1)
	{
		int ij = 0;
	}
	else
		if (pointprosurface3.NbPoints() == 1)
		{
			propoint = pointprosurface3.Point(1);
			gapEdgePoints.push_back(propoint);
		}

	GeomAPI_ProjectPointOnSurf pointprosurface4(startpoint, surface2);
	if (pointprosurface4.NbPoints() > 1)
	{
		//选择一个
		int ij = 0;
	}
	else
		if (pointprosurface4.NbPoints() == 1)
		{
			propoint = pointprosurface4.Point(1);
			gapEdgePoints.push_back(propoint);
		}

	GeomAPI_ProjectPointOnSurf pointprosurface5(middelpoint, surface2);
	if (pointprosurface5.NbPoints() > 1)
	{
		//选择一个
		int ij = 0;
	}
	else
		if (pointprosurface5.NbPoints() == 1)
		{
			propoint = pointprosurface5.Point(1);
			gapEdgePoints.push_back(propoint);
		}


	GeomAPI_ProjectPointOnSurf pointprosurface6(endpoint, surface2);
	if (pointprosurface6.NbPoints() > 1)
	{
		int ij = 0;
	}
	else
		if (pointprosurface6.NbPoints() == 1)
		{
			propoint = pointprosurface6.Point(1);
			gapEdgePoints.push_back(propoint);
		}

	return 1;
}
int Action_SurfaceHealing::AddEndPointsandMiddlePoints(BRep_Curve3D* curve, std::vector<gp_Pnt>& gapEdgePoints)
{
	double first, last;
	curve->Range(first, last);

	gp_Pnt point;
	curve->D0(first, point);
	gapEdgePoints.push_back(point);

	curve->D0((first + last) / 2, point);
	gapEdgePoints.push_back(point);

	curve->D0(last, point);
	gapEdgePoints.push_back(point);
	return 1;
}
int Action_SurfaceHealing::AddEndPointsandMiddlePoints(BRep_CurveOnSurface* GC, std::vector<gp_Pnt>& gapEdgePoints)
{
	double first, last;
	GC->Range(first, last);

	gp_Pnt point;
	GC->D0(first, point);
	gapEdgePoints.push_back(point);

	GC->D0((first + last) / 2, point);
	gapEdgePoints.push_back(point);

	GC->D0(last, point);
	gapEdgePoints.push_back(point);
	return 1;
}
int Action_SurfaceHealing::CheckModelGap_OCC(Shape_OCC* pocc, double h_min, double h_max, double& maxhausdorff)
{
	pocc->gapEdges.clear();
	pocc->gapEdgesHausdorffDistance.clear();

	//check whether to sew it to build the topology
	double tolerance = 0.5;

	TopAbs_ShapeEnum type = pocc->shape.ShapeType();

	//if has face and has no shell and above topology, sew it. 
	/*if (!pocc->ChechWhetherExistTopology())
	{
		BRepBuilderAPI_Sewing sewing(tolerance);
		sewing.Add(pocc->shape);
		sewing.Perform();
		TopoDS_Shape sewedShape = sewing.SewedShape();
		pocc->shape = sewedShape;
	}*/


	maxhausdorff = -1;
	TopExp_Explorer explorer(pocc->shape, TopAbs_EDGE);
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Handle(TopoDS_TShape) tshape = edge.TShape();

		TopAbs_ShapeEnum type = tshape->ShapeType();
		if (type == TopAbs_EDGE)
		{
			// find the representation
			const BRep_TEdge* TE = static_cast<const BRep_TEdge*>(edge.TShape().get());
			BRep_ListIteratorOfListOfCurveRepresentation itcr(TE->Curves());
			TopLoc_Location L;
			std::vector<BRep_CurveOnSurface*> arrayCurveOnSurfaces;
			BRep_Curve3D* geomCurve = NULL;
			BRep_CurveOn2Surfaces* CurveOn2Surfaces = NULL;
			while (itcr.More()) {
				const Handle(BRep_CurveRepresentation)& cr = itcr.Value();
				if (cr->IsCurve3D())
				{
					geomCurve = static_cast<BRep_Curve3D*>(cr.get());
				}
				else
					if (cr->IsCurveOnSurface())
					{
						BRep_CurveOnSurface* GC = static_cast<BRep_CurveOnSurface*>(cr.get());
						arrayCurveOnSurfaces.push_back(GC);
						//L = edge.Location() * GC->Location();
						//GC->Range(first, last);
						//geomCurve = GC->Curve3D();
					}
					else
						if (cr->IsRegularity())
						{
							CurveOn2Surfaces = static_cast<BRep_CurveOn2Surfaces*>(cr.get());
						}
				itcr.Next();
			}
			double hausdorffdis, mindistance;
			if (arrayCurveOnSurfaces.size() == 2)
			{
				//两个邻面都是非平面
				BRep_CurveOnSurface* GC1 = arrayCurveOnSurfaces[0];
				BRep_CurveOnSurface* GC2 = arrayCurveOnSurfaces[1];
				Handle(Geom_Surface) theSurface1 = GC1->Surface();
				Handle(Geom_Surface) theSurface2 = GC2->Surface();
				auto theSurface1Type = theSurface1->DynamicType()->SystemName();
				auto theSurface2Type = theSurface2->DynamicType()->SystemName();

				//hausdorffdis = EvaluateDistanceBetween2CurveOnSurfaces_PointDistanceToPoint(GC1, GC2);
				hausdorffdis = EvaluateDistanceBetween2CurveOnSurfaces_PointDistanceToCurve(GC1, GC2);
				if (hausdorffdis > maxhausdorff)
					maxhausdorff = hausdorffdis;
				if (hausdorffdis >= h_min && hausdorffdis < h_max)
				{
					std::vector<gp_Pnt> gapEdgePoints;
					AddEndPointsandMiddlePoints(GC1, gapEdgePoints);
					AddEndPointsandMiddlePoints(GC2, gapEdgePoints);

					pocc->gapEdgeEndandMiddlePoints.push_back(gapEdgePoints);
					pocc->gapEdges.push_back(edge);
					pocc->gapEdgesHausdorffDistance.push_back(hausdorffdis);
				}
			}
			else
				if (CurveOn2Surfaces && arrayCurveOnSurfaces.size() > 0)
				{
					//一个为平面，一个为非平面
					hausdorffdis = EvaluateDistanceBetween2CurveOnSurfaces(geomCurve, arrayCurveOnSurfaces[0], CurveOn2Surfaces);
					if (hausdorffdis > maxhausdorff)
						maxhausdorff = hausdorffdis;
					if (hausdorffdis >= h_min && hausdorffdis < h_max)
					{
						std::vector<gp_Pnt> gapEdgePoints;
						AddEndPointsandMiddlePoints(arrayCurveOnSurfaces[0], gapEdgePoints);
						AddEndPointsandMiddlePoints(geomCurve, gapEdgePoints);

						pocc->gapEdgeEndandMiddlePoints.push_back(gapEdgePoints);
						pocc->gapEdges.push_back(edge);
						pocc->gapEdgesHausdorffDistance.push_back(hausdorffdis);
					}
				}
				else if (CurveOn2Surfaces && arrayCurveOnSurfaces.size() == 0)
				{
					//两个平面的交线
					hausdorffdis = EvaluateDistanceBetweenCurveson2PlanarSurfaces(geomCurve, CurveOn2Surfaces);
					if (hausdorffdis > maxhausdorff)
						maxhausdorff = hausdorffdis;
					if (hausdorffdis >= h_min && hausdorffdis < h_max)
					{
						std::vector<gp_Pnt> gapEdgePoints;
						AddEndPointsandMiddlePoints(geomCurve, CurveOn2Surfaces, gapEdgePoints);

						pocc->gapEdgeEndandMiddlePoints.push_back(gapEdgePoints);
						pocc->gapEdges.push_back(edge);
						pocc->gapEdgesHausdorffDistance.push_back(hausdorffdis);
					}

				}
			//else
			//	if ()
		}
	}
	if (pocc->gapEdges.size() > 0)
		return 2;
	else
		return 1;
}
void Action_SurfaceHealing::OnBnClickedButtonHausdorffdistancecheck()
{
	// TODO: 在此添加控件通知处理程序代码

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

	CString hausdorff_min, hausdorff_max;
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_HAUSDORFF_MIN))->GetWindowTextA(hausdorff_min);
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_HAUSDORFF_MAX))->GetWindowTextA(hausdorff_max);


	double h_min, h_max;
	h_min = atof(hausdorff_min);
	h_max = atof(hausdorff_max);
	double maxhausdorff;
	int resultflag;

	//
	resultflag = CheckModelGap_OCC(pocc, h_min, h_max, maxhausdorff);

	char message[200];
	sprintf(message, "%.2f", maxhausdorff);
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_HAUSDORFF_MAX))->SetWindowTextA(message);
	pDoc->UpdateAllViews(NULL);
	//AfxMessageBox("sd");
}
void Action_SurfaceHealing::FindStepFilesRecursive(CString dir, std::vector<CString>& arrayFiles)
{
	CString filepath = _T(dir);
	CString filename = _T("");
	CString fullname = _T("");

	CFileFind find;//+|/*.STP|/*.STEP|/*.step"
	BOOL IsFind = find.FindFile(filepath + _T("/*.*"));

	while (IsFind)
	{
		IsFind = find.FindNextFile();
		if (find.IsDots())
		{
			continue;
		}
		else
		{
			if (find.IsDirectory())
			{
				FindStepFilesRecursive(find.GetFilePath(), arrayFiles);
			}
			else //if (find.IsNormal())
			{
				filename = find.GetFileName();
				CString ext = filename.Right(filename.GetLength() - filename.ReverseFind('.'));
				if (ext.CompareNoCase(".STP") == 0 || ext.CompareNoCase(".STEP") == 0 || ext.CompareNoCase(".igs") == 0)
				{
					arrayFiles.push_back(find.GetFilePath());
				}

			}

		}
	}
}
void Action_SurfaceHealing::OnBnClickedButtonAcisCheckFiles()
{
	// TODO: 在此添加控件通知处理程序代码

	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	conn = mysql_init(NULL);

	if (conn == NULL) {
		fprintf(stderr, "mysql_init failed\n");
		return;
	}

	if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
		fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
		mysql_close(conn);
		conn = NULL;
		return;
	}

	CString dir;
	CFolderPickerDialog dlg_folder;
	dlg_folder.m_ofn.lpstrInitialDir = "D:\\workinxjtu\\modelstep";// 设置起始目录
	dlg_folder.m_ofn.lpstrTitle = "选择文件夹";// 设置标题
	if (dlg_folder.DoModal() == IDOK)
	{
		dir = dlg_folder.GetPathName();
	}
	else
	{
		dir = "";
	}

	std::vector<CString> arrayFiles;
	//遍历所有的文件
	FindStepFilesRecursive(dir, arrayFiles);

	CParseSTEP stepParser;
	double h_min = 0.001, h_max = MAX_DOUBLE;
	int l_min = 3, l_max = 8;
	double maxhausdorff;
	int resultflag;
	int openflag;

	//std::ofstream outfile(dir + _T("\\ModelGapCheckResult.txt"));
	//outfile << "Total File Number:" << arrayFiles.size() << "\n";
	//outfile << "Hausdorff Distance Range:" << "(" << h_min << " , " << h_max << ")" << "\n";
	//outfile << "Face Lack Range:" << "(" << l_min << " , " << l_max << ")" << "\n";

	int sucessopenfilenumbers = 0;
	int modelwithgaps = 0;
	int modelwithoutgaps = 0;
	int modellackface = 0;
	int flag;
	char sqlmessage[1000];
	const char* message;
	int id;
	for (int i = 0; i < arrayFiles.size(); i++)
	{
		//first check whether this file is checked or not
		flag = CheckWhetherFileisChecked(conn, arrayFiles[i].GetBuffer());
		if (flag)
			continue;

		CString cs_filename = arrayFiles[i].GetBuffer();
		cs_filename.Replace(".stp", ".brep");
		Standard_CString filename = cs_filename;
		//outfile << "\n";
		flag = InserttoTableModelFile(conn, filename, 0, id);
		if (!flag)
			continue;

		openflag = stepParser.Run_OCC_STEP(arrayFiles[i].GetBuffer(), &pDoc->m_SceneGraph);
		sucessopenfilenumbers += openflag;

		//outfile << arrayFiles[i].GetBuffer() << "\n";
		//outfile << "Open Sucess Flag:" << openflag;

		if (openflag && pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_BREP_OCC)
		{
			//set occread ok
			UpdateTableModelFile_OCCOpenOK(conn, id);

			Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

			//save the file in the brep file format
			CString cs_filename = arrayFiles[i].GetBuffer();
			cs_filename.Replace(".stp", ".brep");
			Standard_CString filename = cs_filename;
			//BRepTools::Write(pocc->shape, filename);

			//gap check
			resultflag = CheckModelGap_OCC(pocc, h_min, h_max, maxhausdorff);

			if (resultflag == 1)
				modelwithoutgaps++;
			else if (resultflag == 2)
				modelwithgaps++;

			//outfile << "Gap Flag:" << resultflag << " Number of Gap Edges: " << pocc->gapEdges.size() << "Maximal Hausdorff Distance:" << maxhausdorff << "\n";

			resultflag = CheckModelFaceLack_OCC(pocc, l_min, l_max);
			//outfile << "Face Lack Number:" << resultflag << "\n";
			if (resultflag > 0)
				modellackface++;

			//flag = InserttoTableModelFile(conn, filename, 1, id);
			//if (!flag)
			//	continue;

			//insert to the gap table	
			sprintf(sqlmessage, "%s%d%s%f%s%d%s", "INSERT INTO modelgap(id, modelfileid, maxhausdorffdistance, gapedgenumber, gapsettingid)VALUES(0,", id, ",", maxhausdorff, ",", pocc->gapEdges.size(), ",1); ");
			if (mysql_query(conn, sqlmessage))
			{
				//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
				message = mysql_error(conn);
				mysql_close(conn);
				conn = NULL;
				return;
			}

			//insert to the facelack table
			sprintf(sqlmessage, "%s%d%s%d%s", "INSERT INTO modelfacelack(id, modelfileid, facelacknumber, facelacksettingid)VALUES(0,", id, ",", resultflag, ",1);");
			if (mysql_query(conn, sqlmessage))
			{
				//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
				message = mysql_error(conn);
				mysql_close(conn);
				conn = NULL;
				return;
			}
		}
		else
		{
			CString cs_filename = arrayFiles[i].GetBuffer();
			//cs_filename.Replace(".stp", ".brep");
			//Standard_CString filename = cs_filename;
			//outfile << "\n";
			//flag = InserttoTableModelFile(conn, filename, 0, id);
		}
		pDoc->m_SceneGraph.Free();
		//outfile.flush();
	}
	//outfile << "-------------------------------------\n";
	//outfile << "Successfully Open File Number:" << sucessopenfilenumbers << "\n";
	//outfile << "Models with gaps:" << modelwithgaps << "\n";
	//outfile << "Models without gaps:" << modelwithoutgaps << "\n";
	//outfile << "Models lack faces:" << modellackface << "\n";
	//outfile.close();
	mysql_close(conn);
	conn = NULL;
}


void Action_SurfaceHealing::OnBnClickedButtonFacelackcheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

	CString facelack_min, facelack_max;
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MIN))->GetWindowTextA(facelack_min);
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MAX))->GetWindowTextA(facelack_max);

	//
	int h_min, h_max;
	h_min = atoi(facelack_min);
	h_max = atoi(facelack_max);

	//
	CheckModelFaceLack_OCC(pocc, h_min, h_max);

	pDoc->UpdateAllViews(NULL);
}
int Action_SurfaceHealing::CheckWhetherFileisChecked(MYSQL*& conn, char* filepath)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where filepath = '";
	strstr << filepath;
	strstr << "';";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return 0;
	}
	else
		return 1;

	mysql_free_result(res);
}
int Action_SurfaceHealing::UpdateTableModelFile_OCCOpenOK(MYSQL*& conn, int id)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET occopen = 1 where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}
int Action_SurfaceHealing::UpdateTableModelFile_HuaweiGapDataSet(MYSQL*& conn, int id)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET HuaweiGap = 1 where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}

int Action_SurfaceHealing::UpdateTableModelFile_HuaweiLackDataSet(MYSQL*& conn, int id, int flag = 1)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET HuaweiLack = 1 where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	std::ostringstream strstr1;
	strstr1 << "SELECT id FROM modelfile where HuaweiLack =";
	strstr1 << flag << ";";
	sql = strstr1.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	if (res)
	{
		return res->row_count;
	}
	return 0;
}
int Action_SurfaceHealing::UpdateTableModelFile_ModelInfo(MYSQL*& conn, int id, int facenum, int edgenum, int vertexnum)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET FaceNum = ";
	strstr << facenum;
	strstr << ", EdgeNum = ";
	strstr << edgenum;
	strstr << ", VertexNum = ";
	strstr << vertexnum;
	strstr << " where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}
int Action_SurfaceHealing::UpdateTableModelFile_BoundingBox(MYSQL*& conn, int id, double MinX, double MinY, double MinZ, double MaxX, double MaxY, double MaxZ)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET MinX = ";
	strstr << MinX;
	strstr << ", MinY = ";
	strstr << MinY;
	strstr << ", MinZ = ";
	strstr << MinZ;
	strstr << ", MaxX = ";
	strstr << MaxX;
	strstr << ", MaxY = ";
	strstr << MaxY;
	strstr << ", MaxZ = ";
	strstr << MaxZ;
	strstr << " where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}
int Action_SurfaceHealing::GetHuaweiModelGapNumber(MYSQL*& conn)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;

	strstr << "SELECT * FROM modelfile where HuaweiGap = 1;";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	if (res)
		return res->row_count;
	return 0;
}

int Action_SurfaceHealing::GetHuaweiModelFaceLackNumber(MYSQL*& conn)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;

	strstr << "SELECT * FROM modelfile where HuaweiLack = 1;";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	if (res)
		return res->row_count;
	return 0;
}

int Action_SurfaceHealing::UpdateTableModelFile_DebugTime(MYSQL*& conn, int id, int debugreadtime)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET ReadTimeDebug = ";
	strstr << debugreadtime;
	strstr << " where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}

int Action_SurfaceHealing::UpdateTableModelFile_ReleaseTime(MYSQL*& conn, int id, int Releasereadtime)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET ReadTimeRelease = ";
	strstr << Releasereadtime;
	strstr << " where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}
int Action_SurfaceHealing::UpdateTableModelFile_Description(MYSQL*& conn, int id, char* des)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	std::ostringstream strstr;
	strstr << "UPDATE modelfile SET description = '";
	strstr << des;
	strstr << "' where id = ";
	strstr << id;
	strstr << ";";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);
	return 1;
}
int Action_SurfaceHealing::InserttoTableModelFile(MYSQL*& conn, const std::string& line, int occopenflag, int& id)
{
	std::string brepfilename;
	std::string brepmessage = ".brep";
	char sqlmessage[1000];
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	CString str = line.c_str();
	str.Replace("\\", "\\\\");
	brepfilename = str.GetBuffer();

	int pos = brepfilename.find(".step");
	if (pos > -1)
		brepfilename.replace(pos, 5, brepmessage);
	pos = brepfilename.find(".stp");
	if (pos > -1)
		brepfilename.replace(pos, 4, brepmessage);

	std::ostringstream strstr;
	strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where filepath = '";
	strstr << str.GetBuffer();
	strstr << "' ;";
	//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return 0;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		//add to the database
		if (std::filesystem::exists(brepfilename))
		{
			sprintf(sqlmessage, "%s%s%s%s%s", "INSERT INTO modelfile(id, filepath, brepfilepath, description, occopen, checked)VALUES(0, '", str.GetBuffer(), "', '", brepfilename.c_str(), "', '', 1, 1); ");
		}
		else
		{
			sprintf(sqlmessage, "%s%s%s", "INSERT INTO modelfile(id, filepath, brepfilepath, description, occopen, checked)VALUES(0, '", str.GetBuffer(), "', '', '', 1, 1); ");
		}

		if (mysql_query(conn, sqlmessage))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return 0;
		}

		sprintf(sqlmessage, "%s%s%s", "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where filepath = '", str.GetBuffer(), "'; ");

		if (mysql_query(conn, sqlmessage))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return 0;
		}
		res = mysql_store_result(conn);
		if ((row = mysql_fetch_row(res)) != NULL)
		{
			id = std::stoi(row[0]);
		}
		return 1;
	}
	else
	{
		if ((row = mysql_fetch_row(res)) != NULL)
		{
			id = std::stoi(row[0]);
		}
	}
	return 0;
}
void Action_SurfaceHealing::OnBnClickedButtonMysql()
{
	// TODO: 在此添加控件通知处理程序代码
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	conn = mysql_init(NULL);

	if (conn == NULL) {
		fprintf(stderr, "mysql_init failed\n");
		return;
	}

	if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
		fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
		mysql_close(conn);
		conn = NULL;
		return;
	}

	const char* message;

	//if (mysql_query(conn, "DELETE  FROM modelfile;"))
	//{
	//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
	//	message = mysql_error(conn);
	//	mysql_close(conn);
	//	return;
	//}

	//if (mysql_query(conn, "DELETE  FROM modelgap;"))
	//{
	//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
	//	message = mysql_error(conn);
	//	mysql_close(conn);
	//	return;
	//}

	//if (mysql_query(conn, "DELETE  FROM modelfacelack;"))
	//{
	//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
	//	message = mysql_error(conn);
	//	mysql_close(conn);
	//	return;
	//}

	////添加facelack
	//if (mysql_query(conn, "	INSERT INTO facelacksetting (id, minedgenum, maxedgenum) VALUES(0, 3, 8)")) {
	//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
	//	message = mysql_error(conn);
	//	mysql_close(conn);
	//	return;
	//}
	////添加gapcheck
	//if (mysql_query(conn, "	INSERT INTO gapsetting (id,  mindistance, maxdistance) VALUES(0, 0.001, 1.7e308)")) {
	//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
	//	message = mysql_error(conn);
	//	mysql_close(conn);
	//	return;
	//}

	//res = mysql_store_result(conn);


	//while ((row = mysql_fetch_row(res)) != NULL) {
	//	printf("%s %s\n", row[0], row[1]);
	//}
	std::ifstream infile(_T("D:\\workinxjtu\\modelstep\\ModelGapCheckResult1.txt"));
	std::string line;
	//first read three lines
	getline(infile, line);
	getline(infile, line);
	getline(infile, line);
	char* lineinfo;
	int opensuccessflag;
	int gapflag;
	int numberofgapedges;
	double maxhausdorffdis;
	int facelacknumber;
	char sqlmessage[1000];
	int id;
	int flag;
	std::string filename;

	namespace fs = std::filesystem;
	while (getline(infile, line))
	{
		//infile.read(lineinfo, 17);
		infile >> opensuccessflag;
		if (opensuccessflag)
		{
			filename = line;
			//infile.read(lineinfo, 9);
			infile >> gapflag;
			//infile.read(lineinfo, 20);
			infile >> numberofgapedges;
			//infile.read(lineinfo, 27);
			infile >> maxhausdorffdis;
			getline(infile, line);
			//infile.read(lineinfo, 17);
			infile >> facelacknumber;
			getline(infile, line);

			flag = InserttoTableModelFile(conn, filename, 1, id);
			if (!flag)
				continue;

			//insert to the gap table	
			sprintf(sqlmessage, "%s%d%s%f%s%d%s", "INSERT INTO modelgap(id, modelfileid, maxhausdorffdistance, gapedgenumber, gapsettingid)VALUES(0,", id, ",", maxhausdorffdis, ",", numberofgapedges, ",1); ");
			if (mysql_query(conn, sqlmessage))
			{
				//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
				message = mysql_error(conn);
				mysql_close(conn);
				conn = NULL;
				return;
			}

			//insert to the facelack table
			sprintf(sqlmessage, "%s%d%s%d%s", "INSERT INTO modelfacelack(id, modelfileid, facelacknumber, facelacksettingid)VALUES(0,", id, ",", facelacknumber, ",1);");
			if (mysql_query(conn, sqlmessage))
			{
				//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
				message = mysql_error(conn);
				mysql_close(conn);
				conn = NULL;
				return;
			}
		}
		else
		{
			//first check whether it exists in in the database
			//sprintf(sqlmessage, "%s", "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile; ");
			////sprintf(sqlmessage, "%s%s%s", "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where filepath = '", line.c_str(), "'; ");
			//if (mysql_query(conn, sqlmessage))
			//{
			//	//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			//	message = mysql_error(conn);
			//	mysql_close(conn);
			//	return;
			//}
			//res = mysql_store_result(conn);
			//if (res->row_count == 0)
			//{
			//	//add to the database
			//	//line.replace('\',0);
			//	//	REPLACE(line, '\\', char(0));
			//	CString linestr = line.c_str();

			//	//linestr.Replace('\\', char(1));
			//	//linestr.Replace("\\\\", "\\");

			//	sprintf(sqlmessage, "%s%s%s", "INSERT INTO modelfile(id, filepath, brepfilepath, description, occopen, checked)VALUES(0, '", linestr.GetBuffer(), "', '', '', 0, 1); ");
			//	if (mysql_query(conn, sqlmessage))
			//	{
			//		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			//		message = mysql_error(conn);
			//		mysql_close(conn);
			//		return;
			//	}
			//}
			//else
			//{
			//	std::string ss1;
			//	if ((row = mysql_fetch_row(res)) != NULL)
			//		ss1 = row[1];
			//}

			getline(infile, line);

		}

		//line >> 
		//int ii = 0;
	}

	//mysql_free_result(res);
	mysql_close(conn);
	conn = NULL;
}


void Action_SurfaceHealing::OnBnClickedButtonFacelackcheckArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

	CString facelack_min, facelack_max;
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MIN))->GetWindowTextA(facelack_min);
	((CEdit*)GetDlgItem(IDC_EDIT_OCC_FACELACK_MAX))->GetWindowTextA(facelack_max);

	//
	int h_min, h_max;
	h_min = atof(facelack_min);
	h_max = atof(facelack_max);

	//
	CheckModelFaceLack_OCC_AREA(pocc, h_min, h_max);

	pDoc->UpdateAllViews(NULL);
}


/*
 * @note 比较两个给定的edge实体是否在几何与拓扑意义上相同
 * @return API执行的结果
 * @param edge1
 * 给定的第一个edge实体
 * @param edge2
 * 给定的第二个edge实体
 * @param check_topology
 * 是否检查拓扑合法性，不合法直接返回false，不进行比较
 * @param check_direction
 * 比较的时候是否考虑edge的方向
 * @param ill_entity_list
 * 若不为空且拓扑检查打开，则将不合法的拓扑存储在list中返回给调用者
 * @param failed_entity_list
 * 若不为空则返回entity1中导致匹配失败的entity，可能为entity1自身或者entity1直接下属的某个entity
 */
bool same_edge(const EDGE* edge1, const EDGE* edge2)
{
	bool check_direction = TRUE;
	if (edge1 == nullptr || edge2 == nullptr) {
		if (edge1 == nullptr && edge2 == nullptr)
			return true;
		else
			return false;
	}

	// 检查端点情况
	bool vertex_check = false;
	int closed1 = edge1->closed();
	int closed2 = edge2->closed();
	if (closed1 != closed2) {
		return false;
	}
	if (!closed1 && !closed2) {
		SPAposition edge1_start_pos = { 0, 0, 0 };
		if (edge1->start())
			edge1_start_pos = edge1->start_pos();

		SPAposition edge2_start_pos = { 0, 0, 0 };
		if (edge2->start())
			edge2_start_pos = edge2->start_pos();

		SPAposition edge1_end_pos = { 0, 0, 0 };
		if (edge1->end())
			edge1_end_pos = edge1->end_pos();

		SPAposition edge2_end_pos = { 0, 0, 0 };
		if (edge2->end())
			edge2_end_pos = edge2->end_pos();

		SPAposition edge1_middle_pos = edge1->mid_pos();
		SPAposition edge2_middle_pos = edge2->mid_pos();

		double point_tol = SPAresabs;
		// 如果是intcurve，考虑拟合容差
		if ((edge1->geometry() != nullptr && strcmp(edge1->geometry()->type_name(), "intcurve") == 0) || (edge2->geometry() != nullptr && strcmp(edge2->geometry()->type_name(), "intcurve") == 0)) {
			/*const intcurve* int1 = (const intcurve*)&edge1->geometry()->equation();
			const intcurve* int2 = (const intcurve*)&edge2->geometry()->equation();
			double length1 = edge1->length();
			double length2 = edge2->length();
			double tol1 = std::max(int1->fitol(), int1->fitol() / length1);
			double tol2 = std::max(int2->fitol(), int2->fitol() / length2);
			if(tol1 > 0) {
				point_tol += tol1;
			}
			if(tol2 > 0) {
				point_tol += tol2;
			}*/
			point_tol = SPAresfit;
		}
		if (same_point(edge1_start_pos, edge2_start_pos, point_tol) && same_point(edge1_end_pos, edge2_end_pos, point_tol) && same_point(edge1_middle_pos, edge2_middle_pos))
		{
			vertex_check = true;
		}
		if (!check_direction) {
			if (same_point(edge1_start_pos, edge2_end_pos, point_tol) && same_point(edge1_end_pos, edge2_start_pos, point_tol) && same_point(edge1_middle_pos, edge2_middle_pos))
			{
				vertex_check = true;
			}
		}
	}
	else if (closed1 && closed2) {
		vertex_check = true;
	}
	if (vertex_check)
	{
		return true;
	}
	return false;
}
void Action_SurfaceHealing::CheckAcisHausdorffdistanceAcis(EntityList_ACIS* pACIS)
{
	ENTITY_LIST* pFaceEntities = new ENTITY_LIST();

	CParseSTEP	parser;

	ENTITY_LIST* pFaceBodies = new ENTITY_LIST();
	//ENTITY_LIST* pEdgeList = new ENTITY_LIST();
	//获取面
	parser.get_FaceList_from_EntityList(pACIS->pAcisEntities, pFaceEntities);

	//parser.get_EdgeList_from_EntityList(pACIS->pAcisEntities, pEdgeList);

	BODY* pBody;
	FACE* facearray[1];
	for (auto pFace : *pFaceEntities)
	{
		facearray[0] = (FACE*)pFace;
		api_sheet_from_ff(1, facearray, pBody);

		pFaceBodies->add(pBody);
	}

	stitch_input_handle* sih;
	mt_stitch_preview_container* cntnr;
	api_stitch_make_input(*pFaceBodies, sih);
	api_stitch_preview(sih, cntnr);
	pACIS->cntnr = cntnr;
}
void Action_SurfaceHealing::OnBnClickedButtonHausdorffdistanceAcis()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	if (pDoc->m_SceneGraph.GetAt(0)->GetType() != TYPE_BREP_ACIS)
		return;

	EntityList_ACIS* pACIS = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(0);


	CheckAcisHausdorffdistanceAcis(pACIS);
	//int size = cntnr->size();
	//double dis;
	//EDGE* edge0, * edge1;
	//double maxdis = -1;
	//int maxindex;
	//int flag1 = 0;
	/*for (int i = 0; i < size; i++)
	{
		cntnr->get_edge_pair(i, edge0, edge1);

		ENTITY* edgeentity = (ENTITY*)edge0;
		for (ENTITY* ent = pEdgeList->first(); ent; ent = pEdgeList->next())
		{
			if (same_edge(edge0, (EDGE*)ent))
			{
				flag1 = 1;
				break;
			}

		}

		dis = cntnr->get_hausdorff_distance(i);

		if (dis > maxdis)
		{
			maxdis = dis;
			maxindex = i;
		}
	}*/
	pDoc->UpdateAllViews(NULL);
}

void Action_SurfaceHealing::CheckEdgeCorrespondenceACIS_OCC(EntityList_ACIS* pACIS, Shape_OCC* pOCC)
{
	int size = pOCC->arrayOCCEdges.size();
	gp_Pnt p1, p2, p3;
	TopLoc_Location loc;
	Standard_Real first, last;
	int theAcisEdgeIndex;
	for (int i = 0; i < size; i++)
	{
		TopoDS_Edge edge = TopoDS::Edge(pOCC->arrayOCCEdges[i]);

		Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);

		gcurve->D0(first, p1);
		gcurve->D0((first + last) / 2, p2);
		gcurve->D0(last, p3);

		theAcisEdgeIndex = FindCorrespondingAcisEdgeIndex(pACIS, p1, p2, p3);

		if (theAcisEdgeIndex > -1)
		{
			pOCC->m_EdgeIndexMaps_OCCTOACIS.insert(std::make_pair(i, theAcisEdgeIndex));
		}
	}
}


int Action_SurfaceHealing::FindCorrespondingAcisEdgeIndex(EntityList_ACIS* pACIS, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3)
{
	int size = pACIS->arrayACISEdges.size();
	TopLoc_Location loc;
	Standard_Real first, last;
	SPAposition pos1, pos2, pos3;
	for (int i = 0; i < size; i++)
	{
		EDGE* pEdge = pACIS->arrayACISEdges[i];

		pos1 = pEdge->start_pos();
		pos2 = pEdge->mid_pos();
		pos3 = pEdge->end_pos();

		int val = SQR(p1.X() - pos1.x()) + SQR(p1.Y() - pos1.y()) + SQR(p1.Z() - pos1.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos3.x()) + SQR(p3.Y() - pos3.y()) + SQR(p3.Z() - pos3.z());

		if (val < TOLER)
			return i;

		val = SQR(p1.X() - pos3.x()) + SQR(p1.Y() - pos3.y()) + SQR(p1.Z() - pos3.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos1.x()) + SQR(p3.Y() - pos1.y()) + SQR(p3.Z() - pos1.z());

		if (val < TOLER)
			return i;
	}
	return -1;
}

void Action_SurfaceHealing::OnBnClickedButtonVerificationHausdorffdistanceAcis()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);

	CString dir;
	MYSQL_ROW row;
	BOOL theOpenFileFlag = 0;
	if (conn && res)
	{
		dir = theCurrentFilePath;
	}
	else
	{
		theOpenFileFlag = 1;
		if (dlg.DoModal() == IDOK)
		{
			dir = dlg.GetPathName();
		}
		else
		{
			return;
		}

		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}
		std::string filename = dir;
		int flag = InserttoTableModelFile(conn, filename, 1, theCurrentModelId);

		UpdateTableModelFile_HuaweiGapDataSet(conn, theCurrentModelId);
	}


	CParseSTEP parser;

	//delete
	pDoc->m_SceneGraph.Free();

	int occflag = parser.Run_OCC_STEP(dir.GetBuffer(), &pDoc->m_SceneGraph);

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);
	int index = pDoc->m_SceneGraph.NbObject();
	UpdateModelInformation(theCurrentModelId);

	int acisflag = parser.Run_Acis(dir.GetBuffer(), &pDoc->m_SceneGraph, "STEP");

	EntityList_ACIS* pACIS = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(index);


	//compute the hausdorff of acis
	//OnBnClickedButtonHausdorffdistanceAcis();
	CheckAcisHausdorffdistanceAcis(pACIS);

	double maxhausdorff;
	int resultflag;

	double h_min = -1;
	double h_max = MAX_DOUBLE;

	//compute the corresnpondence
	//CheckEdgeCorrespondenceACIS_OCC(pACIS, pocc);

	//
	resultflag = CheckModelGap_OCC(pocc, h_min, h_max, maxhausdorff);

	//
	int flag1 = ChecktheHausdorffEquality(pocc, pACIS);

	if (theOpenFileFlag)
	{
		mysql_close(conn);
		conn = NULL;
	}
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
int Action_SurfaceHealing::ChecktheHausdorffEquality(Shape_OCC* pOCC, EntityList_ACIS* pACIS)
{
	int size = pOCC->arrayOCCEdges.size();
	gp_Pnt p1, p2, p3;
	TopLoc_Location loc;
	Standard_Real first, last;
	double acishausdorff;
	int flag;
	//const double TOLERHAUSDORFF = 1e-5;
	const double TOLERHAUSDORFF = 3e-5;
	//const double TOLERHAUSDORFF = 2e-3;
	double dis;
	double maxdis = -1;
	int maxindex;
	int edgePairIndex;
	int maxEdgePairIndex;
	int successnumber = 0;
	int failurenumber = 0;
	for (int i = 0; i < size; i++)
	{
		TopoDS_Edge edge = TopoDS::Edge(pOCC->gapEdges[i]);

		Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);

		if (!gcurve)
			continue;

		//gcurve->D0(first, p1);
		//gcurve->D0((first + last) / 2, p2);
		//gcurve->D0(last, p3);

		//flag = FindCorrespondingAcisEdgeHausdorffDistance(pACIS, p1, p2, p3, acishausdorff, edgePairIndex);
		flag = FindCorrespondingAcisEdgeHausdorffDistance(pACIS, pOCC->gapEdgeEndandMiddlePoints[i], acishausdorff, edgePairIndex);

		if (flag)
		{
			dis = fabs(acishausdorff - pOCC->gapEdgesHausdorffDistance[i]);
			if (dis > maxdis)
			{

				maxdis = dis;
				maxindex = i;
				maxEdgePairIndex = edgePairIndex;
			}

			if (dis > TOLERHAUSDORFF)
			{
				failurenumber++;
			}
			else
			{
				successnumber++;
			}
		}
		else
		{
			//AfxMessageBox("not found the corresponding edge in acis");
		}
	}

	char message[1000];
	sprintf(message, "OCC total Gap Edges:%d, Maximal Hausdorff Differences between occ and acis: %f, (occ: %d,acis: %d)", pOCC->gapEdges.size(), maxdis, maxindex, maxEdgePairIndex);
	//AfxMessageBox(message);
	CString messstr;
	//messstr = messstr + "\r\n";
	messstr = messstr + message;
	sprintf(message, "Success %d, failure %d", successnumber, failurenumber);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION2)->SetWindowTextA(messstr.GetBuffer());

	pOCC->arraySelectedGapIndexArray.push_back(maxindex);
	pACIS->arraySeletedEdgePairIndexAarray.push_back(maxEdgePairIndex);

	//InserttoTableModelGap(theCurrentModelId, maxdis, pOCC->gapEdges.size());
	InserttoTableModelGap_Detail(theCurrentModelId, maxdis, pOCC->gapEdges.size(), successnumber, failurenumber);

	//UpdateModelInformation(id);

	return 1;
}

int Action_SurfaceHealing::CheckWhetherHasSameEndandMiddlePoints(EDGE* edge, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3)
{
	SPAposition pos1, pos2, pos3;
	pos1 = edge->start_pos();
	pos2 = edge->mid_pos();
	pos3 = edge->end_pos();

	double val = SQR(p1.X() - pos1.x()) + SQR(p1.Y() - pos1.y()) + SQR(p1.Z() - pos1.z());
	val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
	val += SQR(p3.X() - pos3.x()) + SQR(p3.Y() - pos3.y()) + SQR(p3.Z() - pos3.z());

	if (val < TOLER)
	{
		return 1;
	}

	val = SQR(p1.X() - pos3.x()) + SQR(p1.Y() - pos3.y()) + SQR(p1.Z() - pos3.z());
	val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
	val += SQR(p3.X() - pos1.x()) + SQR(p3.Y() - pos1.y()) + SQR(p3.Z() - pos1.z());

	if (val < TOLER)
	{
		return 1;
	}
	return 0;
}
int Action_SurfaceHealing::FindCorrespondingAcisEdgeHausdorffDistance(EntityList_ACIS* pACIS, std::vector<gp_Pnt>& edgepoints, double& acishausdorff, int& edgePairIndex)
{

	int size = pACIS->cntnr->size();
	TopLoc_Location loc;
	Standard_Real first, last;
	SPAposition pos1, pos2, pos3;
	EDGE* edge0, * edge1;
	double dis;
	if (edgepoints.size() != 6)
		return 0;

	int flag = 0;
	for (int i = 0; i < size; i++)
	{
		pACIS->cntnr->get_edge_pair(i, edge0, edge1);
		dis = pACIS->cntnr->get_hausdorff_distance(i);

		flag = 0;
		flag += CheckWhetherHasSameEndandMiddlePoints(edge0, edgepoints[0], edgepoints[1], edgepoints[2]);
		flag += CheckWhetherHasSameEndandMiddlePoints(edge1, edgepoints[3], edgepoints[4], edgepoints[5]);

		if (flag == 2)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}

		flag = 0;
		flag += CheckWhetherHasSameEndandMiddlePoints(edge1, edgepoints[0], edgepoints[1], edgepoints[2]);
		flag += CheckWhetherHasSameEndandMiddlePoints(edge0, edgepoints[3], edgepoints[4], edgepoints[5]);

		if (flag == 2)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}
	}
	return 0;
}

int Action_SurfaceHealing::FindCorrespondingAcisEdgeHausdorffDistance(EntityList_ACIS* pACIS, gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, double& acishausdorff, int& edgePairIndex)
{

	int size = pACIS->cntnr->size();
	TopLoc_Location loc;
	Standard_Real first, last;
	SPAposition pos1, pos2, pos3;
	EDGE* edge0, * edge1;
	double dis;
	for (int i = 0; i < size; i++)
	{
		pACIS->cntnr->get_edge_pair(i, edge0, edge1);
		dis = pACIS->cntnr->get_hausdorff_distance(i);

		pos1 = edge0->start_pos();
		pos2 = edge0->mid_pos();
		pos3 = edge0->end_pos();

		double val = SQR(p1.X() - pos1.x()) + SQR(p1.Y() - pos1.y()) + SQR(p1.Z() - pos1.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos3.x()) + SQR(p3.Y() - pos3.y()) + SQR(p3.Z() - pos3.z());

		if (val < TOLER)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}

		val = SQR(p1.X() - pos3.x()) + SQR(p1.Y() - pos3.y()) + SQR(p1.Z() - pos3.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos1.x()) + SQR(p3.Y() - pos1.y()) + SQR(p3.Z() - pos1.z());

		if (val < TOLER)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}

		pos1 = edge1->start_pos();
		pos2 = edge1->mid_pos();
		pos3 = edge1->end_pos();

		val = SQR(p1.X() - pos1.x()) + SQR(p1.Y() - pos1.y()) + SQR(p1.Z() - pos1.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos3.x()) + SQR(p3.Y() - pos3.y()) + SQR(p3.Z() - pos3.z());

		if (val < TOLER)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}

		val = SQR(p1.X() - pos3.x()) + SQR(p1.Y() - pos3.y()) + SQR(p1.Z() - pos3.z());
		val += SQR(p2.X() - pos2.x()) + SQR(p2.Y() - pos2.y()) + SQR(p2.Z() - pos2.z());
		val += SQR(p3.X() - pos1.x()) + SQR(p3.Y() - pos1.y()) + SQR(p3.Z() - pos1.z());

		if (val < TOLER)
		{
			acishausdorff = dis;
			edgePairIndex = i;
			return 1;
		}
	}
	return 0;
}

void Action_SurfaceHealing::OnBnClickedButtonLoadfirstmodel()
{
	// TODO: 在此添加控件通知处理程序代码

	MYSQL_ROW row;

	//if (!conn)
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}

		const char* message;
		std::ostringstream strstr;
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfacelack.facelacknumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap, modelfacelack WHERE modelgap.modelfileid = modelfile.id AND modelfacelack.modelfileid = modelfile.id AND modelfacelack.facelacknumber > 0;";
		strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap WHERE modelgap.modelfileid = modelfile.id;";
		std::string sql = strstr.str();
		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
		res = mysql_store_result(conn);

		if (res->row_count == 0)
		{
			return;
		}
		else
		{

			if ((row = mysql_fetch_row(res)) != NULL)
			{
				theCurrentRowIndex = 1;

				UpdateModelInformation(row);

				pDoc->UpdateTreeControl();
				pDoc->UpdateAllViews(NULL);
			}
		}
	}
}

void Action_SurfaceHealing::UpdateModelInformation(const MYSQL_ROW& row)
{
	char* filepath;
	char message[200];
	CString messstr;
	filepath = row[1];

	theCurrentModelId = atoi(row[0]);

	DWORD star_time = GetTickCount();
	CParseSTEP parser;
	parser.Run_OCC_STEP(filepath, &pDoc->m_SceneGraph);
	DWORD end_time = GetTickCount();

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);


	sprintf(message, "Model Id: %s\r\n", row[0]);
	messstr = messstr + message;

	sprintf(message, "%d / %d", theCurrentRowIndex, res->row_count);
	messstr = messstr + message;
	GetDlgItem(IDC_SURFACEHEALING_MODELINFO)->SetWindowTextA(message);
	GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELPATH)->SetWindowTextA(filepath);
	theCurrentFilePath = filepath;

	messstr = messstr + "\r\n";
	messstr = messstr + filepath;
	GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION)->SetWindowTextA(row[2]);
	//sprintf(message, "Model Face Lack Number:%s", row[7]);
	//GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELFACELACK)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;
	sprintf(message, "Model Face Gap Number:%s", row[6]);
	GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELGAPNUMBER)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;
	sprintf(message, "Maximal Hausdorff Distance:%s", row[5]);
	GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELMAXHAUSDORFF)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	int facenum = atoi(row[7]);
	int edgenum = atoi(row[8]);
	int vertexnum = atoi(row[9]);

	if (facenum == -1)
	{
		sprintf(message, "Face: %d, Edge: %d, Vertex: %d", pOCC->arrayOCCFaces.size(), pOCC->arrayOCCEdges.size(), pOCC->arrayOCCVertices.size());
		GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELINFO)->SetWindowTextA(message);
		messstr = messstr + "\r\n";
		messstr = messstr + message;
		UpdateTableModelFile_ModelInfo(conn, theCurrentModelId, pOCC->arrayOCCFaces.size(), pOCC->arrayOCCEdges.size(), pOCC->arrayOCCVertices.size());
	}
	else
	{
		sprintf(message, "Face: %d, Edge: %d, Vertex: %d", facenum, edgenum, vertexnum);
		GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELINFO)->SetWindowTextA(message);
		messstr = messstr + "\r\n";
		messstr = messstr + message;
	}


	Vector3D m_lower, m_upper;
	pOCC->ComputeBoundingBox(m_lower, m_upper);

	sprintf(message, "LeftBottom: %f,%f,%f", m_lower.x, m_lower.y, m_lower.z);
	GetDlgItem(IDC_SURFACEHEALING_LEFTBOTTOM)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;
	sprintf(message, "RightTop: %f,%f,%f", m_upper.x, m_upper.y, m_upper.z);
	GetDlgItem(IDC_SURFACEHEALING_RIGHTTOP)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	UpdateTableModelFile_BoundingBox(conn, theCurrentModelId, m_lower.x, m_lower.y, m_lower.z, m_upper.x, m_upper.y, m_upper.z);

	sprintf(message, "Debug Read Time:%d ms", end_time - star_time);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	GetDlgItem(IDC_SURFACEHEALING_READTIMEDEBUG)->SetWindowTextA(message);

#ifdef DEBUG
	UpdateTableModelFile_DebugTime(conn, theCurrentModelId, end_time - star_time);
#else
	UpdateTableModelFile_ReleaseTime(conn, theCurrentModelId, end_time - star_time);
#endif // DEBUG

	int huaweigapmodelnumber = GetHuaweiModelGapNumber(conn);
	sprintf(message, "Huawei Gap Model Number:%d", huaweigapmodelnumber);
	GetDlgItem(IDC_SURFACEHEALING_HUAWEIGAPMODELNUM)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	int huaweiLackmodelnumber = GetHuaweiModelFaceLackNumber(conn);
	sprintf(message, "Huawei Lack Model Number:%d", huaweiLackmodelnumber);
	GetDlgItem(IDC_SURFACEHEALING_HUAWEILACKMODELNUM)->SetWindowTextA(message);
	messstr = messstr + "\r\n";
	messstr = messstr + message;

	GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION2)->SetWindowTextA(messstr.GetBuffer());
}


void Action_SurfaceHealing::UpdateModelInformation(int id)
{
	char* filepath;
	CString messstr;
	MYSQL_ROW row;

	char message[1000];
	const char* message1;
	std::ostringstream strstr;
	strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile WHERE modelfile.id = ";
	strstr << id;
	strstr << "; ";

	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message1 = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return;
	}
	else
	{

		if ((row = mysql_fetch_row(res)) != NULL)
		{
			filepath = row[1];

			theCurrentModelId = id;
			theCurrentRowIndex = 1;
			Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

			sprintf(message, "%d / %d", theCurrentRowIndex, res->row_count);
			messstr = messstr + message;
			GetDlgItem(IDC_SURFACEHEALING_MODELINFO)->SetWindowTextA(message);
			GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELPATH)->SetWindowTextA(filepath);
			theCurrentFilePath = filepath;

			messstr = messstr + "\r\n";
			messstr = messstr + filepath;
			GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION)->SetWindowTextA(row[2]);

			int facenum = atoi(row[5]);
			int edgenum = atoi(row[6]);
			int vertexnum = atoi(row[7]);

			if (facenum == -1)
			{
				sprintf(message, "Face: %d, Edge: %d, Vertex: %d", pOCC->arrayOCCFaces.size(), pOCC->arrayOCCEdges.size(), pOCC->arrayOCCVertices.size());
				GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELINFO)->SetWindowTextA(message);
				messstr = messstr + "\r\n";
				messstr = messstr + message;
				UpdateTableModelFile_ModelInfo(conn, theCurrentModelId, pOCC->arrayOCCFaces.size(), pOCC->arrayOCCEdges.size(), pOCC->arrayOCCVertices.size());
			}
			else
			{
				sprintf(message, "Face: %d, Edge: %d, Vertex: %d", facenum, edgenum, vertexnum);
				GetDlgItem(IDC_SURFACEHEALING_CURRENTMODELINFO)->SetWindowTextA(message);
				messstr = messstr + "\r\n";
				messstr = messstr + message;
			}


			Vector3D m_lower, m_upper;
			pOCC->ComputeBoundingBox(m_lower, m_upper);

			sprintf(message, "LeftBottom: %f,%f,%f", m_lower.x, m_lower.y, m_lower.z);
			GetDlgItem(IDC_SURFACEHEALING_LEFTBOTTOM)->SetWindowTextA(message);
			messstr = messstr + "\r\n";
			messstr = messstr + message;
			sprintf(message, "RightTop: %f,%f,%f", m_upper.x, m_upper.y, m_upper.z);
			GetDlgItem(IDC_SURFACEHEALING_RIGHTTOP)->SetWindowTextA(message);
			messstr = messstr + "\r\n";
			messstr = messstr + message;

			UpdateTableModelFile_BoundingBox(conn, theCurrentModelId, m_lower.x, m_lower.y, m_lower.z, m_upper.x, m_upper.y, m_upper.z);

			GetDlgItem(IDC_SURFACEHEALING_READTIMEDEBUG)->SetWindowTextA(message);

			int huaweiLackmodelnumber = GetHuaweiModelFaceLackNumber(conn);
			sprintf(message, "Huawei Lack Model Number:%d", huaweiLackmodelnumber);
			GetDlgItem(IDC_SURFACEHEALING_HUAWEILACKMODELNUM)->SetWindowTextA(message);
			messstr = messstr + "\r\n";
			messstr = messstr + message;

			GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION2)->SetWindowTextA(messstr.GetBuffer());
		}
	}
}
void Action_SurfaceHealing::OnBnClickedButtonLoadnextmodel()
{
	MYSQL_ROW row;
	pDoc->m_SceneGraph.Free();

	// TODO: 在此添加控件通知处理程序代码
	if ((row = mysql_fetch_row(res)) != NULL)
	{
		theCurrentRowIndex++;

		UpdateModelInformation(row);

		pDoc->UpdateTreeControl();
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		AfxMessageBox("No more models");
	}
}


void Action_SurfaceHealing::OnBnClickedButtonModifymodeldescription()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_MODEL_DESCRIPTION)->GetWindowText(str);
	UpdateTableModelFile_Description(conn, theCurrentModelId, str.GetBuffer());
}


void Action_SurfaceHealing::OnBnClickedButtonAddtohuaweigapdataset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTableModelFile_HuaweiGapDataSet(conn, theCurrentModelId);
}


void Action_SurfaceHealing::OnBnClickedButtonAddtohuaweilackdataset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTableModelFile_HuaweiLackDataSet(conn, theCurrentModelId);
}


void Action_SurfaceHealing::OnBnClickedButtonLoadthemodel()
{
	MYSQL_ROW row;

	//if (!conn)
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}

		const char* message;
		std::ostringstream strstr;
		strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfacelack.facelacknumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap, modelfacelack WHERE modelgap.modelfileid = modelfile.id AND modelfacelack.modelfileid = modelfile.id AND modelfacelack.facelacknumber > 0;";
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfacelack.facelacknumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap, modelfacelack WHERE modelgap.modelfileid = modelfile.id;";
		std::string sql = strstr.str();
		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
		res = mysql_store_result(conn);

		if (res->row_count == 0)
		{
			return;
		}
		else
		{
			CString str;
			GetDlgItem(IDC_HEALING_EDIT_MODELINDEX)->GetWindowTextA(str);
			int modelindex = atoi(str.GetBuffer());
			while (modelindex)
			{
				row = mysql_fetch_row(res);
				modelindex--;
			}
			theCurrentRowIndex = atoi(str.GetBuffer());

			UpdateModelInformation(row);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);

		}
	}
}


void Action_SurfaceHealing::OnBnClickedButtonLoadtheacismodel()
{
	// TODO: 在此添加控件通知处理程序代码
	pDoc->m_SceneGraph.Free();

	CParseSTEP parser;
	parser.Run_Acis(theCurrentFilePath.GetBuffer(), &pDoc->m_SceneGraph, "STEP");

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_SurfaceHealing::OnBnClickedButtonAddtofacelackset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;

	Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

	if (!conn)
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}
	}

	Standard_CString filename = pOCC->fullfilepath;
	//outfile << "\n";
	int id;
	int flag = InserttoTableModelFile(conn, filename, 1, id);

	int theFaceLackModelNum = UpdateTableModelFile_HuaweiLackDataSet(conn, id);

	InserttoTableModelFaceLack(id, pOCC->lackFaceBoundaries.size());

	UpdateModelInformation(id);
	//char message[200];
	//sprintf(message, "Total Huawei Face Lack Model Number: %d", theFaceLackModelNum);
	//AfxMessageBox(message);
}

void Action_SurfaceHealing::InserttoTableModelGap_Detail(int id, double maxhausdorff, int gapedgenum, int successnumber, int failurenumber)
{
	char sqlmessage[200];
	const char* message;
	MYSQL_ROW row;
	sprintf(sqlmessage, "%s%d%s", "SELECT id, modelfileid FROM modelgap WHERE modelfileid=", id, "; ");
	if (mysql_query(conn, sqlmessage))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		sprintf(sqlmessage, "%s%d%s%f%s%d%s%d%s%d%s", "INSERT INTO modelgap(id, modelfileid, maxhausdorffdistance, gapedgenumber, gapsettingid, sucess, failure)VALUES(0,", id, ",", maxhausdorff, ",", gapedgenum, ",1,", successnumber, ",", failurenumber, "); ");
		if (mysql_query(conn, sqlmessage))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
	}
	else
	{
		row = mysql_fetch_row(res);
		std::ostringstream strstr;
		strstr << "UPDATE modelgap SET maxhausdorffdistance=";
		strstr << maxhausdorff;
		strstr << ", gapedgenumber=";
		strstr << gapedgenum << ",gapsettingid = 1, sucess=";
		strstr << successnumber << ", failure=";
		strstr << failurenumber;
		strstr << " where id=";
		strstr << atoi(row[0]);
		strstr << ";";
		//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
		std::string sql = strstr.str();

		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
	}
}
void Action_SurfaceHealing::InserttoTableModelGap(int id, double maxhausdorff, int gapedgenum)
{
	//首先查找

	char sqlmessage[200];
	const char* message;
	MYSQL_ROW row;
	sprintf(sqlmessage, "%s%d%s", "SELECT id, modelfileid FROM modelgap WHERE modelfileid=", id, "; ");
	if (mysql_query(conn, sqlmessage))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		sprintf(sqlmessage, "%s%d%s%f%s%d%s", "INSERT INTO modelgap(id, modelfileid, maxhausdorffdistance, gapedgenumber, gapsettingid)VALUES(0,", id, ",", maxhausdorff, ",", gapedgenum, ",1); ");
		if (mysql_query(conn, sqlmessage))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
	}
	else
	{
		row = mysql_fetch_row(res);
		std::ostringstream strstr;
		strstr << "UPDATE modelgap SET maxhausdorffdistance=";
		strstr << maxhausdorff;
		strstr << "and gapedgenumber=";
		strstr << gapedgenum;
		strstr << "where id=";
		strstr << atoi(row[0]);
		strstr << ";";
		//strstr << "SELECT id, filepath, brepfilepath, description, occopen, checked FROM modelfile where id = '3';";
		std::string sql = strstr.str();

		if (mysql_query(conn, sqlmessage))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
	}



}
void Action_SurfaceHealing::InserttoTableModelFaceLack(int id, int theFaceLackNumber)
{
	char sqlmessage[200];
	const char* message;
	//insert to the facelack table
	sprintf(sqlmessage, "%s%d%s%d%s", "INSERT INTO modelfacelack(id, modelfileid, facelacknumber, facelacksettingid)VALUES(0,", id, ",", theFaceLackNumber, ",1);");
	if (mysql_query(conn, sqlmessage))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
}



void Action_SurfaceHealing::OnBnClickedButtonLoadthemodelwithid()
{
	// TODO: 在此添加控件通知处理程序代码
	MYSQL_ROW row;

	//if (!conn)
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}

		CString str;
		GetDlgItem(IDC_HEALING_EDIT_MODELID)->GetWindowTextA(str);
		int theModelID = atoi(str.GetBuffer());

		const char* message;
		std::ostringstream strstr;
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfacelack.facelacknumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap, modelfacelack WHERE modelgap.modelfileid = modelfile.id AND modelfacelack.modelfileid = modelfile.id AND modelfacelack.facelacknumber > 0;";
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfacelack.facelacknumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap, modelfacelack WHERE modelgap.modelfileid = modelfile.id;";
		strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelgap.maxhausdorffdistance, modelgap.gapedgenumber, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ  FROM modelfile, modelgap WHERE modelgap.modelfileid = modelfile.id AND modelfile.id = ";
		strstr << theModelID;
		strstr << "; ";
		std::string sql = strstr.str();
		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
		res = mysql_store_result(conn);

		if (res->row_count == 0)
		{
			return;
		}
		else
		{
			row = mysql_fetch_row(res);

			theCurrentRowIndex = 1;

			UpdateModelInformation(row);

			pDoc->UpdateTreeControl();
			pDoc->UpdateAllViews(NULL);

		}
	}
}


void Action_SurfaceHealing::OnBnClickedButtonFacelackautomatic()
{
	MYSQL_ROW row;
	MYSQL_RES* res1;
	// TODO: 在此添加控件通知处理程序代码
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}
		const char* message;
		std::ostringstream strstr;
		strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ FROM modelfile WHERE modelfile.HuaweiLack=0;";
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ FROM modelfile WHERE modelfile.id=364;";
		std::string sql = strstr.str();
		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
		res1 = mysql_store_result(conn);

		if (res1->row_count == 0)
		{
			return;
		}
		else
		{
			//for (int i = 0; i < 300; i++)
			//	row = mysql_fetch_row(res);

			while ((row = mysql_fetch_row(res1)) != NULL)
			{
				//random delete one face
				DeleteFaceRandom(row);
			}

		}
	}
}

void Action_SurfaceHealing::DetectHuaweiGap(MYSQL_ROW row)
{
	//首先用acis存一遍
	pDoc->m_SceneGraph.Free();
	char* filepath = row[1];

	theCurrentModelId = atoi(row[0]);

	std::string filename = filepath;

	CParseSTEP parser;
	parser.Run_Acis(filepath, &pDoc->m_SceneGraph);

	EntityList_ACIS* pNurbs = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(0);

	SPAIAcisDocument src(pNurbs->pAcisEntities);

	CString s1 = pNurbs->fullfilepath;
	s1.Replace(".", "ACIS.");

	SPAIDocument dst(s1.GetBuffer());
	dst.SetType("STEP");

	SPAIConverter converter;
	converter.Convert(src, dst);

	//将acis存的
	//int flag = InserttoTableModelFile(conn, s1.GetBuffer(), 1, theCurrentModelId);

	UpdateTableModelFile_HuaweiGapDataSet(conn, theCurrentModelId);

	//delete
	pDoc->m_SceneGraph.Free();

	int occflag = parser.Run_OCC_STEP(s1.GetBuffer(), &pDoc->m_SceneGraph);

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);
	int index = pDoc->m_SceneGraph.NbObject();
	UpdateModelInformation(theCurrentModelId);

	int acisflag = parser.Run_Acis(s1.GetBuffer(), &pDoc->m_SceneGraph, "STEP");

	EntityList_ACIS* pACIS = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(index);

	CString s2 = s1;
	s2.Replace(".", "Sep.");
	CheckAcisHausdorffdistanceAcis_Save(pACIS, s2.GetBuffer());
	//CheckAcisHausdorffdistanceAcis(pACIS);

	double maxhausdorff;
	int resultflag;

	double h_min = -1;
	double h_max = MAX_DOUBLE;
	//if has face and has no shell and above topology, sew it. 
	//DWORD star_time_occ = GetTickCount();
	if (!pocc->ChechWhetherExistTopology())
	{
		BRepBuilderAPI_Sewing sewing(0.5);
		sewing.Add(pocc->shape);
		sewing.Perform();
		TopoDS_Shape sewedShape = sewing.SewedShape();
		pocc->shape = sewedShape;
	}
	//DWORD end_time_occ = GetTickCount();

	DWORD star_time = GetTickCount();
	resultflag = CheckModelGap_OCC(pocc, h_min, h_max, maxhausdorff);
	DWORD end_time = GetTickCount();
	//
	int flag1 = ChecktheHausdorffEquality(pocc, pACIS);


	pDoc->m_SceneGraph.Free();

	occflag = parser.Run_OCC_STEP(s2.GetBuffer(), &pDoc->m_SceneGraph);

	pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);

	DWORD star_time_occ = GetTickCount();
	if (!pocc->ChechWhetherExistTopology())
	{
		BRepBuilderAPI_Sewing sewing(0.5);
		sewing.Add(pocc->shape);
		sewing.Perform();
		TopoDS_Shape sewedShape = sewing.SewedShape();
		pocc->shape = sewedShape;
	}
	DWORD end_time_occ = GetTickCount();

	//
	UpdateGapTime(theCurrentModelId, end_time - star_time, end_time_occ - star_time_occ);
}

void Action_SurfaceHealing::CheckAcisHausdorffdistanceAcis_Save(EntityList_ACIS* pACIS, char* filename)
{
	ENTITY_LIST* pFaceEntities = new ENTITY_LIST();

	CParseSTEP	parser;

	ENTITY_LIST* pFaceBodies = new ENTITY_LIST();
	//ENTITY_LIST* pEdgeList = new ENTITY_LIST();
	//获取面
	parser.get_FaceList_from_EntityList(pACIS->pAcisEntities, pFaceEntities);

	//parser.get_EdgeList_from_EntityList(pACIS->pAcisEntities, pEdgeList);

	BODY* pBody;
	FACE* facearray[1];
	for (auto pFace : *pFaceEntities)
	{
		facearray[0] = (FACE*)pFace;
		api_sheet_from_ff(1, facearray, pBody);

		pFaceBodies->add(pBody);
	}

	stitch_input_handle* sih;
	mt_stitch_preview_container* cntnr;
	api_stitch_make_input(*pFaceBodies, sih);
	api_stitch_preview(sih, cntnr);
	pACIS->cntnr = cntnr;

	SPAIAcisDocument src(pFaceBodies);

	SPAIDocument dst(filename);
	dst.SetType("STEP");

	SPAIConverter converter;
	converter.Convert(src, dst);
}
void Action_SurfaceHealing::UpdateGapTime(int modelid, int timeCost, int occTime)
{
	const char* message;
	MYSQL_RES* res;
	MYSQL_ROW row;
	//get the current time
	CTime t = CTime::GetCurrentTime();//#include <atltime.h>
	char timemessage[200];
	sprintf(timemessage, "%d-%d-%d %d:%d:%d", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	std::ostringstream strstr;
	strstr << "UPDATE modelgap SET timeCost = ";
	strstr << timeCost << ", occTime = ";
	strstr << occTime << ", detectTime ='";
	strstr << timemessage << "' WHERE modelfileid=";
	strstr << modelid << ";";

	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
	res = mysql_store_result(conn);
}
void Action_SurfaceHealing::DeleteFaceRandom(MYSQL_ROW row)
{
	//UpdateModelInformation(row);
	pDoc->m_SceneGraph.Free();
	char* filepath = row[1];

	theCurrentModelId = atoi(row[0]);

	//DWORD star_time = GetTickCount();
	CParseSTEP parser;
	parser.Run_Acis(filepath, &pDoc->m_SceneGraph);

	EntityList_ACIS* pNurbs = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(0);

	//随机生成一个数

	int theFaceSize = pNurbs->arrayACISFaces.size();

	if (theFaceSize == 0)
	{
		UpdateTableModelFile_HuaweiLackDataSet(conn, theCurrentModelId, 2);
		return;
	}

	srand((unsigned)time(NULL));
	int theRandomFaceIndex = std::rand() % theFaceSize;

	Vector3D acisboundinglowerpoint, acisboundingupperpoint;
	pNurbs->ComputeBoundingBox(acisboundinglowerpoint, acisboundingupperpoint);

	CString theDeleteFaceType = pNurbs->arrayACISFaces[theRandomFaceIndex]->geometry()->equation().type_name();

	double theFaceArea;
	api_facet_area(pNurbs->arrayACISFaces[theRandomFaceIndex], theFaceArea);

	api_remove_face(pNurbs->arrayACISFaces[theRandomFaceIndex]);
	CString s1;

	SPAIAcisDocument src(pNurbs->pAcisEntities);

	s1 = pNurbs->fullfilepath;
	s1.Replace(".", "deleteface.");

	SPAIDocument dst(s1.GetBuffer());
	dst.SetType("STEP");

	SPAIConverter converter;
	converter.Convert(src, dst);

	pDoc->m_SceneGraph.Free();

	parser.Run_OCC_STEP(s1.GetBuffer(), &pDoc->m_SceneGraph);

	Shape_OCC* pocc = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(0);
	if (!pocc)
		return;
	Vector3D occboundinglowerpoint, occboundingupperpoint;
	pocc->ComputeBoundingBox(occboundinglowerpoint, occboundingupperpoint);
	//double occdiagonallength = Length(occboundingupperpoint - occboundinglowerpoint);
	//double acisdiagonallength = Length(acisboundingupperpoint - acisboundinglowerpoint);
	//int scale = int(occdiagonallength / acisdiagonallength + 0.5);

	//scale = scale * scale;
	double minarea = theFaceArea * 0.1;
	double maxarea = theFaceArea * 10;
	//
	DWORD star_time = GetTickCount();
	CheckModelFaceLack_OCC_AREA(pocc, minarea, maxarea);
	DWORD end_time = GetTickCount();


	Standard_CString filename = s1;
	//outfile << "\n";
	int modelid;
	int flag = InserttoTableModelFile(conn, filename, 1, modelid);

	int theFaceLackModelNum = UpdateTableModelFile_HuaweiLackDataSet(conn, modelid);

	InserttoTableModelFaceLack(pocc, theCurrentModelId, theRandomFaceIndex, modelid, theDeleteFaceType.GetBuffer(), theFaceArea, minarea, maxarea, pocc->theFaceLackBelowMinArea, pocc->theFaceLackBetweenMinAreaAndMaxArea, pocc->theFaceLackAboveMaxArea, end_time - star_time);

	UpdateModelInformation(modelid);
	//pDoc->UpdateAllViews(NULL);
}

void Action_SurfaceHealing::InserttoTableModelFaceLack(Shape_OCC* pocc, int originalmodelid_beforedelteface, int deleteFaceIndex, int modelid, char* deleteFaceType, double theoriginalFaceArea, double MinArea, double MaxArea, int FaceLackBelowMinArea, int FaceLackBetweenMinAreaAndMaxArea, int FaceLackAboveMaxArea, int computeTime)
{
	char sqlmessage[200];
	const char* message;

	//get the current time
	CTime t = CTime::GetCurrentTime();//#include <atltime.h>
	char timemessage[200];
	sprintf(timemessage, "%d-%d-%d %d:%d:%d", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	std::ostringstream strstr;
	strstr << "INSERT INTO modelfacelack(id, originaModelIDBeforeDeleteFace, deleteFaceIndex, modelfileid, deleteFaceType, originalFaceArea, minArea, maxArea, faceLackBelowMinArea, faceLackBetweenMinAreaAndMaxArea, faceLackAboveMaxArea, releaseCompupteTime, CheckTime)VALUES(";
	strstr << 0 << ",";
	strstr << originalmodelid_beforedelteface << ",";
	strstr << deleteFaceIndex << ",";
	strstr << modelid << ",";
	strstr << "'" << deleteFaceType << "',";
	strstr << theoriginalFaceArea << ",";
	strstr << MinArea << ",";
	strstr << MaxArea << ",";
	strstr << FaceLackBelowMinArea << ",";
	strstr << FaceLackBetweenMinAreaAndMaxArea << ",";
	strstr << FaceLackAboveMaxArea << ",";
	strstr << computeTime << ",'";
	strstr << timemessage << "');";

	std::string sql = strstr.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}

	std::ostringstream strstr1;
	strstr1 << "SELECT modelfacelack.id  FROM  modelfacelack WHERE modelfacelack.modelfileid=";
	strstr1 << modelid << ";";
	sql = strstr1.str();
	if (mysql_query(conn, sql.c_str()))
	{
		//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
		message = mysql_error(conn);
		mysql_close(conn);
		conn = NULL;
		return;
	}
	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return;
	}
	else
	{
		MYSQL_ROW row;
		row = mysql_fetch_row(res);
		int modelfacelackid = atoi(row[0]);
		for (int i = 0; i < pocc->lackFaceBoundariesArea.size(); i++)
		{
			sprintf(sqlmessage, "INSERT INTO modelfacelackdetails(id, modelfacelackid, boundaryedges, lackarea)VALUES(0,%d,%d,%f);", modelfacelackid, pocc->lackFaceBoundaries[i].size(), pocc->lackFaceBoundariesArea[i]);
			if (mysql_query(conn, sqlmessage))
			{
				//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
				message = mysql_error(conn);
				mysql_close(conn);
				conn = NULL;
				return;
			}
		}

	}
}

void Action_SurfaceHealing::OnBnClickedButtonGapdetectionautomatic()
{
	// TODO: 在此添加控件通知处理程序代码
	MYSQL_ROW row;
	MYSQL_RES* res1;
	// TODO: 在此添加控件通知处理程序代码
	{
		conn = mysql_init(NULL);

		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return;
		}

		if (mysql_real_connect(conn, "localhost", "xjtu", "HuaweiXjtu@SurfaceHealing66", "surfacehealing", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = NULL;
			return;
		}
		const char* message;
		std::ostringstream strstr;
		strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ FROM modelfile WHERE modelfile.HuaweiGap=0;";
		//strstr << "SELECT modelfile.id, modelfile.filepath, modelfile.description, modelfile.occopen, modelfile.checked, modelfile.FaceNum,modelfile.EdgeNum, modelfile.VertexNum,modelfile.MinX, modelfile.MinY,modelfile.MinZ, modelfile.MaxX,modelfile.MaxY,modelfile.MaxZ FROM modelfile WHERE modelfile.id=364;";
		std::string sql = strstr.str();
		if (mysql_query(conn, sql.c_str()))
		{
			//fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			message = mysql_error(conn);
			mysql_close(conn);
			conn = NULL;
			return;
		}
		res1 = mysql_store_result(conn);

		if (res1->row_count == 0)
		{
			return;
		}
		else
		{
			//for (int i = 0; i < 300; i++)
			//	row = mysql_fetch_row(res);

			while ((row = mysql_fetch_row(res1)) != NULL)
			{
				//random delete one face
				DetectHuaweiGap(row);
			}

		}
	}
}
