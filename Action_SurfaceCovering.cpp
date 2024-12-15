#include "stdafx.h"
#include "Action_SurfaceCovering.h"
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
#include "TShape_OCC.h"
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
#include "Geom_BSplineCurve.hxx"
#include "SurfaceModelingTool_OCC.h"
#include "GeomFill_Coons.hxx"
#include "GeomFill_BSplineCurves.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "Geom_BSplineSurface.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"
#include "TopExp.hxx"
#include "Geom2d_Line.hxx"
#include <cstrapi.hxx>
#include <wire_offset_options.hxx>
#include "ofstapi.hxx"
#include <rnd_api.hxx>
#include <kernapi.hxx>
#include "curve.hxx"
#include "curdef.hxx"
#include "intdef.hxx"
#include "sps3crtn.hxx"
#include <SurfaceModelingTool_ACIS.h>
IMPLEMENT_DYNAMIC(Action_SurfaceCovering, CPropertyPage)
Action_SurfaceCovering::Action_SurfaceCovering()
{

}

Action_SurfaceCovering::~Action_SurfaceCovering()
{

}


void Action_SurfaceCovering::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_SurfaceCovering, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SURFACECOVERING, &Action_SurfaceCovering::OnBnClickedButtonSurfacecovering)
	ON_BN_CLICKED(IDC_BUTTON1_Load_Boundary_Example1, &Action_SurfaceCovering::OnBnClickedButton1LoadBoundaryExample1)
	ON_BN_CLICKED(IDC_BUTTON1_G0COONS_GENERATION, &Action_SurfaceCovering::OnBnClickedButton1CoonsGeneration)
	ON_BN_CLICKED(IDC_BUTTON1_Load_G1Coons_Example1, &Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample1)
	ON_BN_CLICKED(IDC_BUTTON1_Load_G1Coons_Example3, &Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample3)
	ON_BN_CLICKED(IDC_BUTTON1_Load_G1Coons_Example2, &Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample2)
	ON_BN_CLICKED(IDC_BUTTON1_G1COONS_GENERATION, &Action_SurfaceCovering::OnBnClickedButton1G1coonsGeneration)
	ON_BN_CLICKED(IDC_BUTTON1_POINTPAIRCALCULATION, &Action_SurfaceCovering::OnBnClickedButton1Pointpaircalculation)
	ON_BN_CLICKED(IDC_BUTTON_BSPLINECURVEOFFSET, &Action_SurfaceCovering::OnBnClickedButtonBsplinecurveoffset)
END_MESSAGE_MAP()



void Action_SurfaceCovering::OnBnClickedLoadTwoSurfaces()
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
void Action_SurfaceCovering::OnBnClickedFixGap()
{
	// 函数声明，用于检查操作结果是否成功，并处理错误    
	//int gaps = test_gaps();
	//if (gaps == SPAI_S_OK)
	//	AfxMessageBox("Gaps Succeed");
	//else  AfxMessageBox("Gaps Failed");

}
extern CSDIViewSwitchDoc* pDoc;

void Action_SurfaceCovering::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


BOOL Action_SurfaceCovering::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	//((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Action_SurfaceCovering::OnBnClickedButtonSurfacecovering()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox("covering");
}


void Action_SurfaceCovering::OnBnClickedButton1LoadBoundaryExample1()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\Loops\\loopRes5.brep");

	pDoc->UpdateAllViews(NULL);
}


void Action_SurfaceCovering::OnBnClickedButton1CoonsGeneration()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_SceneGraph.NbObject() == 0 || pDoc->m_CurrentObjectIndex >= pDoc->m_SceneGraph.NbObject())
		return;

	if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	std::vector<Handle(Geom_BSplineCurve)> curveArray;
	BRep_Curve3D* geomCurve;
	TopExp_Explorer explorer(pOCC->shape, TopAbs_EDGE);
	int edgenum = 0;
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());

		edgenum++;

		Handle(TopoDS_TShape) tshape = edge.TShape();

		TopAbs_ShapeEnum type = tshape->ShapeType();
		if (type == TopAbs_EDGE)
		{
			// find the representation
			TopLoc_Location loc;
			Standard_Real first, last;
			Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);
			gcurve = Handle(Geom_Curve)::DownCast(gcurve->Copy());
			Handle(Geom_BSplineCurve) bCurve1 = Handle(Geom_BSplineCurve)::DownCast(gcurve);

			bCurve1->Segment(first, last);


			if (!bCurve1.IsNull() && bCurve1->IsKind(STANDARD_TYPE(Geom_BSplineCurve)))
			{
				curveArray.push_back(bCurve1);
			}

			//

		}
	}

	if (curveArray.size() != 4)
		return;

	Handle(Geom_BSplineCurve) bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4;

	//Standard_Real Tol = Precision::Confusion();

	SurfaceModelingTool_OCC::Arrange_Coons_G0(curveArray, bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4, 2.0, TRUE);

	GeomFill_BSplineCurves gBSpline(bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4, GeomFill_CoonsStyle);
	Handle(Geom_BSplineSurface) surface = gBSpline.Surface();
	//GeomFill_Coons coons();

	Handle(Geom_Surface) basesurfacce = Handle(Geom_Surface)::DownCast(surface);
	double tol = 0.001;
	//
	BRepBuilderAPI_MakeFace MF(basesurfacce, tol);

	CParseSTEP parser;
	Shape_OCC* pshape = new Shape_OCC();
	pshape->shape = MF.Shape();

	// 三角化形状
	parser.TriangulateShape(pshape);
	// 

	CObject3d* pSurface = (CObject3d*)pDoc->m_SceneGraph.GetAt(0);
	pshape->SetTransform(*pSurface->GetTransform());

	pshape->filename = "OCC_Model";

	pDoc->m_SceneGraph.Add(pshape);

	parser.AddFaceChildren_OCC(pshape, &pDoc->m_SceneGraph);

	Handle(Geom_BSplineSurface) surfacecoons;
	SurfaceModelingTool_OCC::Coons_G0(bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4, surfacecoons);

	BRepBuilderAPI_MakeFace MF1(surfacecoons, tol);

	Shape_OCC* pshape1 = new Shape_OCC();
	pshape1->shape = MF1.Shape();

	// 三角化形状
	parser.TriangulateShape(pshape1);
	// 

	pshape1->SetTransform(*pSurface->GetTransform());

	pshape1->filename = "OCC_Model";

	pDoc->m_SceneGraph.Add(pshape1);

	parser.AddFaceChildren_OCC(pshape1, &pDoc->m_SceneGraph);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

void Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample1()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CParseSTEP parser;
	parser.Run_OCC_STEP("D:\\papers\\code\\data\\constrained2.stp", &pDoc->m_SceneGraph);
	//pDoc->OnOpenDocument();
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample3()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CParseSTEP parser;
	parser.Run_OCC_STEP("D:\\papers\\code\\data\\TestG1Coons.stp", &pDoc->m_SceneGraph);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_SurfaceCovering::OnBnClickedButton1LoadG1coonsExample2()
{
	// TODO: 在此添加控件通知处理程序代码
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CParseSTEP parser;
	parser.Run_OCC_STEP("D:\\papers\\code\\data\\testConstrainedFill3.stp", &pDoc->m_SceneGraph);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}


void Action_SurfaceCovering::OnBnClickedButton1G1coonsGeneration()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<int> arrayEdgeIndex = { 11, 3, 5,15 };

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_SceneGraph.NbObject() == 0 || pDoc->m_CurrentObjectIndex >= pDoc->m_SceneGraph.NbObject())
		return;

	if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	//TopTools_IndexedMapOfShape aVertexMap;
	//TopExp::MapShapes(aShape, TopAbs_VERTEX, aVertexMap);

	TopTools_IndexedDataMapOfShapeListOfShape theEdgeFaceMap;
	TopExp::MapShapesAndAncestors(pOCC->shape, TopAbs_EDGE, TopAbs_FACE, theEdgeFaceMap);

	//make the curve arrange for G1
	if (arrayEdgeIndex.size() != 4)
		return;

	std::vector<std::pair<TopoDS_Edge, TopoDS_Face>> arrayTopoDSEdges;
	for (int i = 0; i < arrayEdgeIndex.size(); i++)
	{
		TopoDS_Edge edge = pOCC->arrayOCCEdges[arrayEdgeIndex[i]];

		if (!theEdgeFaceMap.Contains(edge))
			return;

		const TopTools_ListOfShape& ListOfShapes = theEdgeFaceMap.FindFromKey(edge);
		if (ListOfShapes.Extent() != 1)
			return;

		TopTools_ListIteratorOfListOfShape it(ListOfShapes);
		TopoDS_Face face;
		if (it.More())
			face = TopoDS::Face(it.Value());

		arrayTopoDSEdges.push_back(std::make_pair(edge, face));
	}

	if (arrayTopoDSEdges.size() != 4)
		return;

	std::vector<Handle(Geom_BSplineCurve)> curveArray;
	std::vector<Handle(Geom_BSplineCurve)> curvederiveArray;
	std::vector<Handle(Geom2d_Curve)> pcurveArray;
	BRep_Curve3D* geomCurve;
	int edgenum = 0;
	for (int i = 0; i < arrayEdgeIndex.size(); i++)
	{
		TopoDS_Edge edge = arrayTopoDSEdges[i].first;
		TopoDS_Face face = arrayTopoDSEdges[i].second;

		edgenum++;

		Handle(TopoDS_TShape) tshape = edge.TShape();

		TopAbs_ShapeEnum type = tshape->ShapeType();
		if (type == TopAbs_EDGE)
		{
			// find the representation
			TopLoc_Location loc;
			Standard_Real first, last;
			Standard_Real first_para, last_para;
			Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);
			gcurve = Handle(Geom_Curve)::DownCast(gcurve->Copy());
			Handle(Geom2d_Curve) pcurve = BRep_Tool::CurveOnSurface(edge, face, first_para, last_para);
			pcurve = Handle(Geom2d_Curve)::DownCast(pcurve->Copy());

			//deal with the boundary curve only now
			if (pcurve.IsNull() || !pcurve->IsKind(STANDARD_TYPE(Geom2d_Line)))
			{
				return;
			}
			Handle(Geom2d_Line) pcurve_Line2D = Handle(Geom2d_Line)::DownCast(pcurve);
			Handle(Geom_BSplineCurve) bCurve1 = Handle(Geom_BSplineCurve)::DownCast(gcurve);
			//copy it before modify it
			bCurve1 = Handle(Geom_BSplineCurve)::DownCast(bCurve1->Copy());

			bCurve1->Segment(first, last);

			if (!bCurve1.IsNull() && bCurve1->IsKind(STANDARD_TYPE(Geom_BSplineCurve)))
			{
				curveArray.push_back(bCurve1);
				pcurveArray.push_back(pcurve_Line2D);
			}

			//

		}
	}

	if (curveArray.size() != 4)
		return;

	Handle(Geom_BSplineCurve) bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4;
	Handle(Geom_BSplineCurve) bslpineCurve1_deriv, bslpineCurve2_deriv, bslpineCurve3_deriv, bslpineCurve4_deriv;
	double Tol = 2;
	BOOL IsModify = TRUE;
	bslpineCurve1 = curveArray[0];
	bslpineCurve2 = curveArray[1];
	bslpineCurve3 = curveArray[2];
	bslpineCurve4 = curveArray[3];

	Standard_Real maxDis = -1;
	Standard_Real dis;
	gp_Pnt curve1startpoint = bslpineCurve1->StartPoint();
	gp_Pnt curve1endpoint = bslpineCurve1->EndPoint();

	curveArray.erase(curveArray.begin());
	gp_Pnt curve2startpoint = bslpineCurve2->StartPoint();
	gp_Pnt curve2endpoint = bslpineCurve2->EndPoint();

	gp_Pnt curve3startpoint = bslpineCurve3->StartPoint();
	gp_Pnt curve3endpoint = bslpineCurve3->EndPoint();

	gp_Pnt curve4startpoint = bslpineCurve4->StartPoint();
	gp_Pnt curve4endpoint = bslpineCurve4->EndPoint();

	gp_Pnt point;
	//fine curve2
	for (int i = 0; i < curveArray.size(); i++)
	{
		bslpineCurve2 = curveArray[i];

		curve2startpoint = bslpineCurve2->StartPoint();
		curve2endpoint = bslpineCurve2->EndPoint();

		if (curve1endpoint.IsEqual(curve2startpoint, Tol))
		{
			dis = curve1endpoint.Distance(curve2startpoint);
			if (dis > maxDis)
				maxDis = dis;

			if (IsModify && dis < Tol)
			{
				curve1endpoint.BaryCenter(0.5, curve2startpoint, 0.5);
				bslpineCurve2->SetPole(1, curve1endpoint);
				bslpineCurve1->SetPole(bslpineCurve1->NbPoles(), curve1endpoint);
			}

			curveArray.erase(curveArray.begin() + i);
			break;
		}
		if (curve1endpoint.IsEqual(curve2endpoint, Tol))
		{
			dis = curve1endpoint.Distance(curve2endpoint);
			if (dis > maxDis)
				maxDis = dis;

			if (IsModify && dis < Tol)
			{
				curve1endpoint.BaryCenter(0.5, curve2endpoint, 0.5);
				bslpineCurve2->SetPole(bslpineCurve2->NbPoles(), curve1endpoint);
				bslpineCurve1->SetPole(bslpineCurve1->NbPoles(), curve1endpoint);
			}

			curveArray.erase(curveArray.begin() + i);
			bslpineCurve2->Reverse();

			curve2startpoint = bslpineCurve2->StartPoint();
			curve2endpoint = bslpineCurve2->EndPoint();
			break;
		}
	}

	//fine curve3
	for (int i = 0; i < curveArray.size(); i++)
	{
		bslpineCurve3 = curveArray[i];

		curve3startpoint = bslpineCurve3->StartPoint();
		curve3endpoint = bslpineCurve3->EndPoint();

		if (curve2endpoint.IsEqual(curve3endpoint, Tol))
		{
			dis = curve2endpoint.Distance(curve3endpoint);
			if (dis > maxDis)
				maxDis = dis;

			if (IsModify && dis < Tol)
			{
				curve2endpoint.BaryCenter(0.5, curve3endpoint, 0.5);
				bslpineCurve2->SetPole(bslpineCurve2->NbPoles(), curve2endpoint);
				bslpineCurve3->SetPole(bslpineCurve3->NbPoles(), curve2endpoint);
			}

			curveArray.erase(curveArray.begin() + i);
			break;
		}
		if (curve2endpoint.IsEqual(curve3startpoint, Tol))
		{
			dis = curve2endpoint.Distance(curve3startpoint);
			if (dis > maxDis)
				maxDis = dis;

			if (IsModify && dis < Tol)
			{
				curve2endpoint.BaryCenter(0.5, curve3startpoint, 0.5);
				bslpineCurve2->SetPole(bslpineCurve2->NbPoles(), curve2endpoint);
				bslpineCurve3->SetPole(1, curve2endpoint);
			}

			curveArray.erase(curveArray.begin() + i);
			bslpineCurve3->Reverse();

			curve3startpoint = bslpineCurve3->StartPoint();
			curve3endpoint = bslpineCurve3->EndPoint();
			break;
		}
	}
	//

	if (curveArray.size() != 1)
		return;

	bslpineCurve4 = curveArray[0];
	curve4startpoint = bslpineCurve4->StartPoint();
	curve4endpoint = bslpineCurve4->EndPoint();
	if (curve3startpoint.IsEqual(curve4endpoint, Tol))
	{
		dis = curve3startpoint.Distance(curve4endpoint);
		if (dis > maxDis)
			maxDis = dis;

		if (IsModify && dis < Tol)
		{
			curve3startpoint.BaryCenter(0.5, curve4endpoint, 0.5);
			bslpineCurve3->SetPole(1, curve3startpoint);
			bslpineCurve4->SetPole(bslpineCurve4->NbPoles(), curve3startpoint);
		}
	}
	else
		if (curve3startpoint.IsEqual(curve4startpoint, Tol))
		{
			dis = curve3startpoint.Distance(curve4startpoint);
			if (dis > maxDis)
				maxDis = dis;

			if (IsModify && dis < Tol)
			{
				curve3startpoint.BaryCenter(0.5, curve4startpoint, 0.5);
				bslpineCurve3->SetPole(1, curve3startpoint);
				bslpineCurve4->SetPole(1, curve3startpoint);
			}

			bslpineCurve4->Reverse();
			curve4startpoint = bslpineCurve4->StartPoint();
			curve4endpoint = bslpineCurve4->EndPoint();
		}
		else
			return;

	//generate the derivative curve
	TColStd_Array1OfReal    Knots(1, 2);
	TColStd_Array1OfInteger Mults(1, 2);

	Mults(1) = 2;
	Mults(2) = 2;

	TColgp_Array1OfPnt Poles(1, 2);
	gp_Pnt P1, P2;
	gp_Vec V1, V2;
	//bslpineCurve1_deriv
	bslpineCurve4->D1(bslpineCurve4->FirstParameter(), P1, V1);
	bslpineCurve2->D1(bslpineCurve2->FirstParameter(), P2, V2);

	Poles(1).SetXYZ(V1.XYZ());
	Poles(2).SetXYZ(V2.XYZ());

	Knots(1) = bslpineCurve1->Knot(1);
	Knots(2) = bslpineCurve1->Knot(bslpineCurve1->NbKnots());

	bslpineCurve1_deriv = new Geom_BSplineCurve(Poles, Knots, Mults, 1);

	//bslpineCurve2_deriv
	bslpineCurve1->D1(bslpineCurve1->LastParameter(), P1, V1);
	bslpineCurve3->D1(bslpineCurve3->LastParameter(), P2, V2);
	Poles(1).SetXYZ(V1.XYZ());
	Poles(2).SetXYZ(V2.XYZ());

	Knots(1) = bslpineCurve2->Knot(1);
	Knots(2) = bslpineCurve2->Knot(bslpineCurve2->NbKnots());

	bslpineCurve2_deriv = new Geom_BSplineCurve(Poles, Knots, Mults, 1);

	//bslpineCurve3_deriv
	bslpineCurve4->D1(bslpineCurve4->LastParameter(), P1, V1);
	bslpineCurve2->D1(bslpineCurve2->LastParameter(), P2, V2);
	Poles(1).SetXYZ(V1.XYZ());
	Poles(2).SetXYZ(V2.XYZ());

	Knots(1) = bslpineCurve3->Knot(1);
	Knots(2) = bslpineCurve3->Knot(bslpineCurve3->NbKnots());

	bslpineCurve3_deriv = new Geom_BSplineCurve(Poles, Knots, Mults, 1);

	//bslpineCurve4_deriv
	bslpineCurve1->D1(bslpineCurve1->FirstParameter(), P1, V1);
	bslpineCurve3->D1(bslpineCurve3->FirstParameter(), P2, V2);
	Poles(1).SetXYZ(V1.XYZ());
	Poles(2).SetXYZ(V2.XYZ());

	Knots(1) = bslpineCurve4->Knot(1);
	Knots(2) = bslpineCurve4->Knot(bslpineCurve4->NbKnots());

	bslpineCurve4_deriv = new Geom_BSplineCurve(Poles, Knots, Mults, 1);

	bslpineCurve1_deriv->IncreaseDegree(bslpineCurve1->Degree());
	bslpineCurve2_deriv->IncreaseDegree(bslpineCurve2->Degree());
	bslpineCurve3_deriv->IncreaseDegree(bslpineCurve3->Degree());
	bslpineCurve4_deriv->IncreaseDegree(bslpineCurve4->Degree());

	pOCC->curveArray.push_back(bslpineCurve1);
	pOCC->curveArray.push_back(bslpineCurve2);
	pOCC->curveArray.push_back(bslpineCurve3);
	pOCC->curveArray.push_back(bslpineCurve4);
	pOCC->curvederiveArray.push_back(bslpineCurve1_deriv);
	pOCC->curvederiveArray.push_back(bslpineCurve2_deriv);
	pOCC->curvederiveArray.push_back(bslpineCurve3_deriv);
	pOCC->curvederiveArray.push_back(bslpineCurve4_deriv);

	Handle(Geom_BSplineSurface) mySurface_coons;
	SurfaceModelingTool_OCC::Coons_G1(bslpineCurve1, bslpineCurve2, bslpineCurve3, bslpineCurve4, bslpineCurve1_deriv, bslpineCurve2_deriv, bslpineCurve3_deriv, bslpineCurve4_deriv, mySurface_coons);

	BRepBuilderAPI_MakeFace MF1(mySurface_coons, Tol);

	Shape_OCC* pshape1 = new Shape_OCC();
	pshape1->shape = MF1.Shape();

	CParseSTEP parser;
	// 三角化形状
	parser.TriangulateShape(pshape1);
	// 

	pshape1->SetTransform(*pOCC->GetTransform());

	pshape1->filename = "OCC_Model";

	pDoc->m_SceneGraph.Add(pshape1);

	parser.AddFaceChildren_OCC(pshape1, &pDoc->m_SceneGraph);

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void Action_SurfaceCovering::SamplingSectionCurveByArcLength(double len, Shape_OCC* pOCC, std::vector<int>& curveSectionArray, gp_Pnt& point)
{
	double totallength = 0;
	//
	int edgeIndex;
	double currentEdgeLength;
	for (edgeIndex = 0; edgeIndex < curveSectionArray.size(); edgeIndex++)
	{
		TopoDS_Edge edge = pOCC->arrayOCCEdges[edgeIndex];
		GProp_GProps props;
		// 计算边的属性
		BRepGProp::LinearProperties(edge, props);
		// 获取边的长度
		currentEdgeLength = props.Mass();
		totallength += currentEdgeLength;

		if (len < totallength)
		{
			totallength = totallength - currentEdgeLength;
			len -= totallength;
			break;
		}


	}

	BOOL reverseSampling = FALSE;
	//
	if (edgeIndex > 0)
	{
		TopoDS_Vertex v1, v2, v3, v4;
		TopoDS_Vertex theedgeStartV, preEdgeEndV;
		TopoDS_Edge preEdge, theEdge;
		preEdge = pOCC->arrayOCCEdges[edgeIndex - 1];
		theEdge = pOCC->arrayOCCEdges[edgeIndex];
		TopExp::Vertices(preEdge, v1, v2);

		TopExp::Vertices(theEdge, v3, v4);

		if (theEdge.Orientation() == TopAbs_FORWARD)
		{
			theedgeStartV = v3;
		}
		else
		{
			theedgeStartV = v4;
		}

		if (!theedgeStartV.IsSame(v1) && !theedgeStartV.IsSame(v2))
		{
			reverseSampling = TRUE;
		}
	}

	//find the point
	if (reverseSampling)
		len = currentEdgeLength - len;

	double para;

	while (1)
	{

	}
}
//extern void get_tessellationpoints_from_edges_occ(Shape_OCC* pocc, const TopoDS_Shape& edge, std::vector<gp_Pnt>& tessellationPoints);
void Action_SurfaceCovering::SamplingSectionCurveByArcLength(Shape_OCC* pOCC, std::vector<int>& curveSectionArray, std::vector<gp_Pnt>& curveSectionArray1_Sampling, int SamplingNum)
{
	std::vector<gp_Pnt> tessellationpoints;
	for (int i = 0; i < curveSectionArray.size(); i++)
	{
		TopoDS_Edge edge = pOCC->arrayOCCEdges[curveSectionArray[i]];
		pOCC->get_tessellationpoints_from_edges_occ(edge, tessellationpoints);
		if (curveSectionArray1_Sampling.size() == 0)
			curveSectionArray1_Sampling.insert(curveSectionArray1_Sampling.end(), tessellationpoints.cbegin(), tessellationpoints.cend());
		else
		{
			gp_Pnt point1 = curveSectionArray1_Sampling[curveSectionArray1_Sampling.size() - 1];
			gp_Pnt point2 = curveSectionArray1_Sampling[0];
			gp_Pnt p1 = tessellationpoints[0];
			gp_Pnt p2 = tessellationpoints[tessellationpoints.size() - 1];
			double dis1 = point1.Distance(p1);
			double dis2 = point1.Distance(p2);

			double dis3 = point2.Distance(p1);
			double dis4 = point2.Distance(p2);

			if (dis1 < dis2 && dis1 < dis3 && dis1 < dis4)
			{
				int size = tessellationpoints.size();
				for (int j = 1; j < size; j++)
				{
					curveSectionArray1_Sampling.push_back(tessellationpoints[j]);
				}
			}
			else
				if (dis2 < dis1 && dis2 < dis3 && dis2 < dis4)
				{
					int size = tessellationpoints.size();
					for (int j = size - 2; j >= 0; j--)
					{
						curveSectionArray1_Sampling.push_back(tessellationpoints[j]);
					}
				}
				else if (dis3 < dis1 && dis3 < dis2 && dis3 < dis4)
				{
					int size = tessellationpoints.size();
					for (int j = 1; j < size; j++)
					{
						curveSectionArray1_Sampling.insert(curveSectionArray1_Sampling.begin(), tessellationpoints[j]);
					}
				}
				else if (dis4 < dis1 && dis4 < dis2 && dis4 < dis3)
				{
					int size = tessellationpoints.size();
					for (int j = size - 2; j >= 0; j--)
					{
						curveSectionArray1_Sampling.insert(curveSectionArray1_Sampling.begin(), tessellationpoints[j]);
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
	curveSectionArray1_Sampling.erase(curveSectionArray1_Sampling.end() - 1);
}
void Action_SurfaceCovering::OnBnClickedButton1Pointpaircalculation()
{
	// TODO: 在此添加控件通知处理程序代码

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CParseSTEP parser;
	parser.Run_OCC_STEP("D:\\papers\\code\\data\\Part1.step", &pDoc->m_SceneGraph);
	//pDoc->OnOpenDocument();
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);

	if (pDoc->m_SceneGraph.NbObject() == 0 || pDoc->m_CurrentObjectIndex >= pDoc->m_SceneGraph.NbObject())
		return;

	if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() != TYPE_BREP_OCC)
		return;

	Shape_OCC* pOCC = (Shape_OCC*)pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex);

	std::vector<int> curveSectionArray1 = { 2,6,9,12,14 };
	std::vector<int> curveSectionArray2 = { 0,4,7,10,13 };

	std::vector<gp_Pnt> curveSectionArray1_Sampling;
	std::vector<gp_Pnt> curveSectionArray2_Sampling;

	const int SamplingNum = 100;
	SamplingSectionCurveByArcLength(pOCC, curveSectionArray1, curveSectionArray1_Sampling, SamplingNum);

	SamplingSectionCurveByArcLength(pOCC, curveSectionArray2, curveSectionArray2_Sampling, SamplingNum);
	int vertexIndexonSection2 = 0;
	pOCC->curveSectionArray1_Sampling = curveSectionArray1_Sampling;
	pOCC->curveSectionArray2_Sampling = curveSectionArray2_Sampling;

	int index = 0;
	//for the index point in curveSectionArray1_Sampling find the corresnponding one in curveSectionArray2_Sampling

	double val;
	double minval = MAX_DOUBLE;
	int minindex;
	int i;
	for (i = 0; i < curveSectionArray2_Sampling.size(); i++)
	{
		val = ComputeMaximalDeiviation(curveSectionArray1_Sampling, curveSectionArray2_Sampling, i);

		if (val < minval)
		{
			minval = val;
			minindex = i;
		}
	}
	pDoc->UpdateAllViews(NULL);
}
double Action_SurfaceCovering::ComputeMaximalDeiviation(std::vector<gp_Pnt>& curveSectionArray1_Sampling, std::vector<gp_Pnt>& curveSectionArray2_Sampling, int index)
{
	double maxdeviation = -1;
	int size = curveSectionArray1_Sampling.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt p1 = curveSectionArray1_Sampling[i];
		gp_Pnt p2 = curveSectionArray1_Sampling[(i + 1) % size];
		gp_Pnt p3 = curveSectionArray2_Sampling[(index + i + size) % size];
		gp_Pnt p4 = curveSectionArray2_Sampling[(index + i + size + 1) % size];

		Vector3D v1(p1.X(), p1.Y(), p1.Z());
		Vector3D v2(p2.X(), p2.Y(), p2.Z());
		Vector3D v3(p3.X(), p3.Y(), p3.Z());
		Vector3D v4(p4.X(), p4.Y(), p4.Z());

		double angle1 = fabs(acos((v1 - v3) * (v2 - v1) / (Length(v1 - v3) * Length(v2 - v1))) - PI / 2);
		double angle2 = fabs(acos((v1 - v3) * (v4 - v3) / (Length(v1 - v3) * Length(v4 - v3))) - PI / 2);

		if (angle1 > maxdeviation)
			maxdeviation = angle1;
		if (angle2 > maxdeviation)
			maxdeviation = angle2;
	}
	return maxdeviation;
}

void Action_SurfaceCovering::OnBnClickedButtonBsplinecurveoffset()
{
	// TODO: 在此添加控件通知处理程序代码

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	EDGE* origin_edge = nullptr;
	BODY* new_body = nullptr;
	//api_curve_bezier({ 0, 0, 0 }, { 1, 1, 0 }, { 3, 0, 0 }, { 4, 0, 0 }, origin_edge);

	SPAposition spline_points[5];
	//spline_points[0] = SPAposition(-5.0, -4.0, 0.0);
	//spline_points[1] = SPAposition(-1.0, 3.0, 0.0);
	//spline_points[2] = SPAposition(1.0, 2.5, 0.0);
	//spline_points[3] = SPAposition(2.5, 2.5, 0.0);
	//spline_points[4] = SPAposition(4.0, -6.0, 0.0);

	spline_points[0] = SPAposition(-5.0, 4.0, 0.0);
	spline_points[1] = SPAposition(-1.0, 5.0, 0.0);
	spline_points[2] = SPAposition(1.0, 2.5, 0.0);
	spline_points[3] = SPAposition(2.5, 2.0, 0.0);
	spline_points[4] = SPAposition(4.0, 6.0, 0.0);

	int dim = 3, deg = 3, num_ctrlpts = 5, num_knots = 9;
	logical rat = 0;
	SPAposition* ctrpts = nullptr;
	double* weights = nullptr;
	double knots[9] = { 0,0,0,0,0.75,1,1,1,1 };

	api_mk_ed_int_ctrlpts(deg, rat, false, false, num_ctrlpts, spline_points, weights, 0.1, num_knots, knots, 0.1, origin_edge);

	api_make_ewire(1, &origin_edge, new_body);

	ENTITY_LIST acisList;
	acisList.add(new_body);

	//api_sample_edges

	BODY* res_body = nullptr;
	wire_offset_options opt;
	double offsetdistance = 1.0;
	opt.set_plane_normal({ 0, 0, 1 });
	opt.set_distance(offsetdistance);
	api_offset_planar_wire(new_body, &opt, res_body);
	acisList.add(res_body);
	//api_rh_set_entity_rgb(origin_edge, rgb_color(1.0, 0.0, 0.0));
	//output_ents.add(origin_edge);
	//output_ents.add(res_body);
	EDGE* res_edge = res_body->lump()->shell()->wire()->coedge()->edge();
	//res_edge->geometry()->equation().debug("ACIS");
	//origin_edge->geometry()->equation().debug("GME");
	CParseSTEP parser;
	parser.Run_Acis(&acisList, &pDoc->m_SceneGraph);

	//verify whether they are the same parameterization
	ENTITY_LIST edgeList;
	api_get_edges(res_body, edgeList);
	EDGE* edgeOffset = (EDGE*)edgeList.first();
	const curve* originalcurve = &(origin_edge->geometry()->equation());
	const curve* offsetcurve = &(edgeOffset->geometry()->equation());

	curve* offsetcurve1;
	SurfaceModelingTool_ACIS::OffsetCurve(originalcurve, offsetcurve1, offsetdistance);

	const int SEGMENTNUM = 10000;
	double t, t1;
	SPAposition pos, posoffset;
	SPAvector firstvec, firstvecoffset;

	SPAinterval cintval = originalcurve->param_range();
	double maxangle = -1;
	double angle;
	std::vector<double> firstderiArray1, firstderiArray2;
	for (int i = 0; i <= SEGMENTNUM; i++)
	{
		t = (double)i / SEGMENTNUM;
		t1 = cintval.interpolate(t);
		originalcurve->eval(t1, pos, firstvec);
		offsetcurve->eval(t1, posoffset, firstvecoffset);
		firstderiArray1.push_back(firstvec.len());
		firstderiArray2.push_back(firstvecoffset.len());
		SPAunit_vector ufirstvec = normalise(firstvec);
		SPAvector vec1 = pos - posoffset;
		SPAunit_vector vec1n = normalise(vec1);
		angle = (std::acos(vec1n % ufirstvec) - PI / 2) * 180 / PI;
		if (angle > maxangle)
			maxangle = angle;
	}

	intcurve* intcurve_ = (intcurve*)offsetcurve;

	int dim1 = 0, deg1 = 0, num_ctrlpts1 = 0, num_knots1 = 0;
	logical rat1 = 0;
	SPAposition* ctrlpts1 = nullptr;
	double* weights1 = nullptr, * knots1 = nullptr;

	bs3_curve_to_array(intcurve_->cur(), dim1, deg1, rat1, num_ctrlpts1, ctrlpts1, weights1, num_knots1, knots1);

	std::vector<double> firstderiArray3;
	std::vector<double> firstderiArray4;
	double val1, val2, val3, val4, val5;
	SPAvector c1, c2;
	double ratio11, ratio12;
	for (int i = 4; i < num_knots1 - 4; i = i + 2)
	{
		offsetcurve->eval(knots1[i], pos, firstvec);
		c1 = offsetcurve->eval_curvature(knots1[i]);
		firstderiArray3.push_back(firstvec.len());
		originalcurve->eval(knots1[i], pos, firstvec);
		c2 = originalcurve->eval_curvature(knots1[i]);
		firstderiArray4.push_back(firstvec.len());

		ratio11 = c1.len() / c2.len();
		ratio12 = firstderiArray4[firstderiArray3.size() - 1] / firstderiArray3[firstderiArray3.size() - 1];


		//计算offset曲线的长度
		double curlen1_offset = 0, curlen2_offset = 0;
		double curlen1_original = 0, curlen2_original = 0;
		double low = knots1[i - 1];
		double middle = knots1[i];
		double high = knots1[i + 2];
		SPAposition pos1, pos2;
		for (int j = 0; j < 10; j++)
		{
			double ratio1 = (double)j / 10;
			double ratio2 = (double)(j + 1) / 10;
			double t1 = low * (1 - ratio1) + middle * ratio1;
			double t2 = low * (1 - ratio2) + middle * ratio2;

			offsetcurve->eval(t1, pos1);
			offsetcurve->eval(t2, pos2);

			curlen1_offset += (pos1 - pos2).len();
		}

		for (int j = 0; j < 10; j++)
		{
			double ratio1 = (double)j / 10;
			double ratio2 = (double)(j + 1) / 10;
			double t1 = low * (1 - ratio1) + middle * ratio1;
			double t2 = low * (1 - ratio2) + middle * ratio2;

			originalcurve->eval(t1, pos1);
			originalcurve->eval(t2, pos2);

			curlen1_original += (pos1 - pos2).len();
		}

		val1 = curlen1_offset / (middle - low);
		for (int j = 0; j < 10; j++)
		{
			double ratio1 = (double)j / 10;
			double ratio2 = (double)(j + 1) / 10;
			double t1 = middle * (1 - ratio1) + high * ratio1;
			double t2 = middle * (1 - ratio2) + high * ratio2;

			offsetcurve->eval(t1, pos1);
			offsetcurve->eval(t2, pos2);

			curlen2_offset += (pos1 - pos2).len();
		}

		for (int j = 0; j < 10; j++)
		{
			double ratio1 = (double)j / 10;
			double ratio2 = (double)(j + 1) / 10;
			double t1 = middle * (1 - ratio1) + high * ratio1;
			double t2 = middle * (1 - ratio2) + high * ratio2;

			originalcurve->eval(t1, pos1);
			originalcurve->eval(t2, pos2);

			curlen2_original += (pos1 - pos2).len();
		}

		val2 = curlen2_offset / (high - middle);

		val3 = (val1 * (high - middle) + val2 * (middle - low)) / ((high - low));

		val4 = (curlen1_offset + curlen2_offset) / (high - low);

		val5 = firstderiArray4[firstderiArray4.size() - 1] * (curlen1_offset + curlen2_offset) / (curlen1_original + curlen2_original);


	}

	//ENTITY* TEST = edgeList.first();

	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
