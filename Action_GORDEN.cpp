#include<iostream>
#include<string>
#include <fstream>

//自己添加的头文件
#include "stdafx.h"
#include "Action_GORDEN.h"
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
#include "AcisCheckInformation.h"
#include "Entity_FACE_ACIS.h"
#include "Entity_EDGE_ACIS.h"
#include "Shape_FACE_OCC.h"
#include "Shape_EDGE_OCC.h"
#include "TShape_OCC.h"
#include "Interpolate.h"
#include "Compatible.h"
#include "GuideGordon.h"

// acis
#include <body.hxx>
#include <mt_stitch_opts.hxx>
#include <mt_stitch_hndl.hxx>
#include <mt_stitch_apis.hxx>
#include <heal_api.hxx>
#include <kernapi.hxx>
#include <skinapi.hxx>
#include <cstrapi.hxx>
#include <skin_opts.hxx>
#include <coverapi.hxx>
#include <getbox.hxx>
#include <box.hxx>
#include <af_api.hxx>
#include <af_api.hxx>
#include <skin.hxx>
#include <law_util.hxx>
#include <insanity_tbl.hxx>
#include <intrapi.hxx>
#include <insanity_list.hxx>
#include <SPAIAcisDocument.h>
#include <getowner.hxx>

// occ
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <GC_MakeCircle.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Writer.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BRep_Tool.hxx>
#include <GeomConvert.hxx>
#include <Math_matrix.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <STEPControl_Reader.hxx>

// Analysis Situs
#include <asiAlgo_BuildGordonSurf.h>

// Mobius
#include <mobius/bspl_UnifyKnots.h>
#include <mobius/cascade.h>
#include <mobius/geom_InterpolateCurve.h>
#include <mobius/geom_InterpolateMultiCurve.h>
#include <mobius/geom_SkinSurface.h>
#include <mobius/geom_UnifyBCurves.h>
#include <mobius/core_Ptr.h>

using namespace mobius;

IMPLEMENT_DYNAMIC(Action_GORDEN, CPropertyPage)
Action_GORDEN::Action_GORDEN()
{

}

Action_GORDEN::~Action_GORDEN()
{

}

void Action_GORDEN::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_GORDEN, CPropertyPage)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR2, &Action_GORDEN::OnNMThemeChangedScrollbar2)
	ON_BN_CLICKED(IDC_BUTTON_GORDEN, &Action_GORDEN::OnBnClickedButtonGorden)
	ON_BN_CLICKED(IDC_BUTTON_MYGORDON, &Action_GORDEN::OnBnClickedButtonMygordon)
	ON_BN_CLICKED(IDC_BUTTON_Analysis, &Action_GORDEN::OnBnClickedButtonAnalysis)
END_MESSAGE_MAP()

extern CSDIViewSwitchDoc* pDoc;

void Action_GORDEN::OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL Action_GORDEN::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//((CEdit*)GetDlgItem(IDC_EDIT_ANGLE_TOLERANCE))->SetWindowTextA("0.6");
	//((CEdit*)GetDlgItem(IDC_EDIT_TRIANGLE_GROUP_NUMBER))->SetWindowTextA("60");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// OCC 创建样条曲线并返回 EDGE 对象
TopoDS_Edge createSplineEdge(const std::vector<gp_Pnt>& points) {
	TColgp_Array1OfPnt arrayOfPoints(1, static_cast<int>(points.size()));
	for (size_t i = 0; i < points.size(); ++i) {
		arrayOfPoints.SetValue(static_cast<int>(i + 1), points[i]);
	}

	// 节点向量和重数设置（适用于三次B样条曲线）
	TColStd_Array1OfReal knots(1, 5);  // 9 = 控制点数量 + Degree + 1
	knots.SetValue(1, 0);
	knots.SetValue(2, 0.25);
	knots.SetValue(3, 0.5);
	knots.SetValue(4, 0.75);
	knots.SetValue(5, 1.0);

	TColStd_Array1OfInteger multiplicities(1, 5);
	multiplicities.Init(1);


	// 设置B样条曲线的次数
	Standard_Integer degree = 1;

	// 创建B样条曲线
	Handle(Geom_BSplineCurve) splineCurve = new Geom_BSplineCurve(arrayOfPoints, knots, multiplicities, degree);

	return BRepBuilderAPI_MakeEdge(splineCurve);
}

// ACIS 创建样条曲线并返回 EDGE 对象
EDGE* Action_GORDEN::create_spline_edge(const std::vector<SPAposition>& points) {
	EDGE* edge = nullptr;
	Standard_Integer num_points = points.size();

	// 调用 api_curve_spline 创建样条曲线
	outcome res = api_curve_spline(num_points, points.data(), nullptr, nullptr, edge);
	if (!res.ok()) {
		std::cerr << "样条曲线创建失败" << std::endl;
		return nullptr;
	}

	return edge;
}

// ACIS 将 EDGE 对象转换为 BODY 对象
BODY* Action_GORDEN::create_body_from_edges(EDGE* edges[], int num_edges) {
	BODY* body = nullptr;

	// 调用 api_make_ewire 将边缘转换为 BODY 对象
	outcome res = api_make_ewire(num_edges, edges, body);
	if (!res.ok()) {
		std::cerr << "将边转换为 BODY 失败" << std::endl;
		return nullptr;
	}

	return body;
}

// ACIS 使用api_net_wires接口生成Gordon曲面
BODY* Action_GORDEN::create_gordon_surface(BODY* v_wires[], Standard_Integer num_v_wires, BODY* u_wires[], int num_u_wires) {
	int n = sizeof(v_wires) / sizeof(v_wires[0]);
	BODY* sheet_body = nullptr;
	skin_options* opts = new skin_options(); // 使用默认选项
	outcome res = api_net_wires(num_v_wires, v_wires, num_u_wires, u_wires, sheet_body, opts);
	if (!res.ok()) {
		std::cerr << "Gordon曲面创建失败" << std::endl;
		return nullptr;
	}
	return sheet_body;
}

// ACIS 创建Loft_Connected_Coedge_List
Loft_Connected_Coedge_List** create_loft_connected_coedge_list(Standard_Integer num_sections, std::vector<std::vector<COEDGE*>> coedges)
{
	Loft_Connected_Coedge_List** sections = new Loft_Connected_Coedge_List * [num_sections];
	for (Standard_Integer i = 0; i < num_sections; ++i) {
		sections[i] = new Loft_Connected_Coedge_List();
		sections[i]->n_list = coedges[i].size();
		sections[i]->coedge_list = new COEDGE * [sections[i]->n_list];
		for (Standard_Integer j = 0; j < sections[i]->n_list; ++j) {
			sections[i]->coedge_list[j] = coedges[i][j];
		}
		sections[i]->coedge_orient = 0; // 假设方向不反转
		sections[i]->cross_tg_attr = 1.0; // 假设缩放因子为1.0
		sections[i]->law_list = NULL;
		sections[i]->knot_value = 0.0; // 假设参数值为0.0
		sections[i]->knot_value_set = FALSE; // 假设标志值为FALSE
	}
	return sections;
}

void get_edgepoints_from_faceted_edge(EDGE* edge, std::vector<SPAposition>& edgepoints)
{
	Standard_Integer numpoints;
	SPAposition* polyline;
	api_get_facet_edge_points(edge, polyline, numpoints);
	for (Standard_Integer i = 0; i < numpoints; i++)
	{
		edgepoints.push_back(polyline[i]);
	}
}

void get_triangles_from_faceted_face(FACE* face, std::vector<float>& coords, std::vector<Standard_Integer>& triangles, std::vector<float>& normalCoords, std::vector<std::vector<SPAposition>>& edgepoints) {
	af_serializable_mesh* sm = GetSerializableMesh(face);
	if (nullptr == sm) {
		// Application decision: do we throw for unfaceted faces?
		return;
	}
	SPAtransf tr = get_owner_transf(face);

	const Standard_Integer nv = sm->number_of_vertices();
	Standard_Integer ntri = sm->number_of_polygons();

	coords.resize(3 * nv);
	sm->serialize_positions(coords.data());  // if std::vector::data is not available, &(coords[0]) will also work.
	if (!tr.identity()) {
		for (Standard_Integer ii = 0; ii < nv; ii++) {
			Standard_Integer jj = 3 * ii;
			SPAposition pos(coords[jj], coords[jj + 1], coords[jj + 2]);
			pos *= tr;
			coords[jj] = (float)pos.x();
			coords[jj + 1] = (float)pos.y();
			coords[jj + 2] = (float)pos.z();
		}
	}

	bool const has_normals = sm->has_normals() == TRUE;
	if (has_normals) {
		normalCoords.resize(3 * nv);
		sm->serialize_normals(normalCoords.data());
	}

	triangles.resize(3 * ntri);
	Standard_Integer ntri_actual = sm->serialize_triangles(triangles.data());
	while (ntri_actual < ntri) {
		triangles.pop_back();
		ntri_actual = static_cast<Standard_Integer>(triangles.size());
	}

	//

	ENTITY_LIST edgelist;
	api_get_edges(face, edgelist);
	edgepoints.resize(edgelist.count());
	Standard_Integer i = 0;
	Standard_Integer numpoints;
	SPAposition* polyline;
	for (auto edge : edgelist)
	{
		api_get_facet_edge_points((EDGE*)edge, polyline, numpoints);
		for (Standard_Integer j = 0; j < numpoints; j++)
			edgepoints[i].push_back(polyline[j]);
		i++;
	}

}

static void get_triangles_from_faceted_face_occ(TopoDS_Shape shape, std::vector<float>& coords, std::vector<Standard_Integer>& triangles, std::vector<float>& normalCoords, std::vector<std::vector<Point3D>>& edgepoints)
{
	TopLoc_Location location;
	TopoDS_Face face = TopoDS::Face(shape);
	Handle_Poly_Triangulation triangulation = BRep_Tool::Triangulation(face, location);
	//triangulation->AddNormals();
	if (!triangulation.IsNull())
	{

		// 获取三角形的数量
		Standard_Integer numTriangles = triangulation->NbTriangles();
		Standard_Integer numNodes = triangulation->NbNodes();
		//int nodenum = pshape->coords.size() / 3;

		for (Standard_Integer i = 1; i <= numNodes; ++i)
		{
			const gp_Pnt& p = triangulation->Node(i);
			coords.push_back(p.X());
			coords.push_back(p.Y());
			coords.push_back(p.Z());

		}

		//normal
		if (triangulation->HasNormals())
			for (Standard_Integer i = 1; i <= numNodes; ++i)
			{
				const gp_Dir& p = triangulation->Normal(i);
				normalCoords.push_back(p.X());
				normalCoords.push_back(p.Y());
				normalCoords.push_back(p.Z());
			}


		// 迭代所有三角形并提取节点
		for (Standard_Integer i = 1; i <= numTriangles; ++i) {
			const Poly_Triangle& triangle = triangulation->Triangle(i);
			Standard_Integer n1, n2, n3;
			triangle.Get(n1, n2, n3);  // 获取三角形的三个顶点索引
			triangles.push_back(n1 - 1);
			triangles.push_back(n2 - 1);
			triangles.push_back(n3 - 1);
		}
	}
	else {
		std::cerr << "Face does not have triangulation data." << std::endl;
	}
}

static void get_tessellationpoints_from_edges_occ(Shape_OCC* pocc, const TopoDS_Shape& edge, std::vector<gp_Pnt>& tessellationPoints)
{
	std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
	auto iterator = pocc->m_EdgeTessellationMaps.find(edge.TShape());
	if (iterator != pocc->m_EdgeTessellationMaps.end())
	{
		ptessellationPoints = &iterator->second;

	}
	else
	{
		getTessellationPoints(TopoDS::Edge(edge), tessellationPoints);
		pocc->m_EdgeTessellationMaps.insert(std::make_pair(edge.TShape(), tessellationPoints));
	}
}

void TriangulateShape(Shape_OCC* pshape)
{

	TopLoc_Location location;
	TopExp_Explorer explorer(pshape->shape, TopAbs_FACE);
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(explorer.Current());
		BRepMesh_IncrementalMesh mesh(face, 1);
		Handle_Poly_Triangulation triangulation = BRep_Tool::Triangulation(face, location);
		triangulation->AddNormals();
		triangulation->ComputeNormals();
		if (!triangulation.IsNull())
		{

			// 获取三角形的数量
			Standard_Integer numTriangles = triangulation->NbTriangles();
			Standard_Integer numNodes = triangulation->NbNodes();
			Standard_Integer nodenum = pshape->coords.size() / 3;

			for (Standard_Integer i = 1; i <= numNodes; ++i)
			{
				const gp_Pnt& p = triangulation->Node(i);
				pshape->coords.push_back(p.X());
				pshape->coords.push_back(p.Y());
				pshape->coords.push_back(p.Z());

			}

			//normal
			if (triangulation->HasNormals())
				for (Standard_Integer i = 1; i <= numNodes; ++i)
				{
					const gp_Dir& p = triangulation->Normal(i);
					pshape->normalCoords.push_back(p.X());
					pshape->normalCoords.push_back(p.Y());
					pshape->normalCoords.push_back(p.Z());
				}


			// 迭代所有三角形并提取节点
			for (Standard_Integer i = 1; i <= numTriangles; ++i) {
				const Poly_Triangle& triangle = triangulation->Triangle(i);
				Standard_Integer n1, n2, n3;
				triangle.Get(n1, n2, n3);  // 获取三角形的三个顶点索引
				pshape->triangles.push_back(n1 - 1 + nodenum);
				pshape->triangles.push_back(n2 - 1 + nodenum);
				pshape->triangles.push_back(n3 - 1 + nodenum);
			}
		}
		else {
			std::cerr << "Face does not have triangulation data." << std::endl;
		}
	}


}

void AddFaceChildren(Shape_OCC* pshape, CSceneGraph3d pSceneGraph)
{
	TopExp_Explorer explorer(pshape->shape, TopAbs_FACE);
	Standard_Integer index = 0;
	char message[200];
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(explorer.Current());
		Shape_FACE_OCC* pFaceOCC = new Shape_FACE_OCC();
		pFaceOCC->m_FaceIndex = index++;
		pFaceOCC->shape = face;
		pFaceOCC->pParentObject = pshape;

		get_triangles_from_faceted_face_occ(face, pFaceOCC->coords, pFaceOCC->triangles, pFaceOCC->normalCoords, pFaceOCC->edgepoints);
		pFaceOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Face%d", pFaceOCC->m_FaceIndex);
		pFaceOCC->filename = message;
		pSceneGraph.Add(pFaceOCC);
	}
}

void AddEdgeChildren(Shape_OCC* pshape, CSceneGraph3d pSceneGraph)
{
	char message[200];
	TopExp_Explorer explorer1(pshape->shape, TopAbs_EDGE);
	Standard_Integer index = 0;
	for (; explorer1.More(); explorer1.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer1.Current());
		Shape_EDGE_OCC* pEdgeOCC = new Shape_EDGE_OCC();
		pEdgeOCC->m_EdgeIndex = index++;
		pEdgeOCC->shape = edge;
		pEdgeOCC->pParentObject = pshape;

		get_tessellationpoints_from_edges_occ(pshape, edge, pEdgeOCC->edgepoints);
		pEdgeOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Edge%d", pEdgeOCC->m_EdgeIndex);
		pEdgeOCC->filename = message;
		pSceneGraph.Add(pEdgeOCC);
	}
}

// ACIS 显示曲面
void face_visualization_ACIS(BODY* sheet_body)
{
	// todo：显示戈登曲面

	ENTITY_LIST* pAcisEntities = new ENTITY_LIST();
	EntityList_ACIS* pEntityList = new EntityList_ACIS();
	Entity_FACE_ACIS* pFaceEntity = new Entity_FACE_ACIS(); // child
	//ENTITY_LIST* pEdgeEntities = new ENTITY_LIST();
	FACE* gordon = sheet_body->lump()->shell()->face();

	pFaceEntity->pAcisEntity = gordon;
	api_facet_entity(gordon);
	get_triangles_from_faceted_face(gordon, pFaceEntity->coords,
		pFaceEntity->triangles, pFaceEntity->normalCoords, pFaceEntity->edgepoints);

	//if (pDoc->m_SceneGraph.NbObject() == 0)
	if (pDoc->m_SceneGraph.NbObject() == 0 || pDoc->m_SceneGraph.GetAt(0)->filename != "Gordon")
	{
		pAcisEntities->add(gordon);
		pEntityList->pAcisEntities = pAcisEntities;
		//pEntityList->faceData.resize(1);

		//
		//transform
		Vector3D m_lower, m_upper;
		pFaceEntity->ComputeBoundingBox(m_lower, m_upper);
		CTransform transform;
		Vector3D m_center;
		m_center = m_lower + m_upper;
		m_center /= -2;
		float length = m_upper.x - m_lower.x;
		if (length < m_upper.y - m_lower.y)
			length = m_upper.y - m_lower.y;
		if (length < m_upper.z - m_lower.z)
			length = m_upper.z - m_lower.z;
		length = length / 2;
		//translation
		CVector3d translation(m_center[0], m_center[1], m_center[2]);
		transform.SetTranslation(&translation);
		//scale
		CVector3d scale(1 / length, 1 / length, 1 / length);
		transform.SetScale(&scale);

		pEntityList->SetTransform(transform);
		//
		pEntityList->filename = "Gordon";
		pDoc->m_SceneGraph.Add(pEntityList);
	}

	pFaceEntity->pParentObject = pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1); // 加入最后一个
	pFaceEntity->SetTransform(pDoc->m_SceneGraph.GetAt(0)->m_Transform); // transform设置为与第一个相同
	pFaceEntity->filename = "Surface";
	pDoc->m_SceneGraph.Add(pFaceEntity);
	//pDoc->UpdateTreeControl();
}

// OCC 显示曲面
void face_visualization_OCC(TopoDS_Shape shape)
{
	Shape_OCC* pshape = new Shape_OCC();
	pshape->shape = shape;

	// 三角化形状
	TriangulateShape(pshape);

	//设置transform
	Vector3D m_lower, m_upper;
	pshape->ComputeBoundingBox(m_lower, m_upper);

	CTransform transform;

	transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

	if (pDoc->m_SceneGraph.NbObject() >= 1)
	{
		CObject3d* pSurface = (CObject3d*)pDoc->m_SceneGraph.GetAt(0);
		pshape->SetTransform(*pSurface->GetTransform());
	}
	else
	{
		pshape->SetTransform(transform);
	}

	pshape->filename = "Gordon";

	pDoc->m_SceneGraph.Add(pshape);

	//add face children
	//AddFaceChildren(pshape, pDoc->m_SceneGraph);
	TopExp_Explorer explorer(pshape->shape, TopAbs_FACE);
	Standard_Integer index = 0;
	char message[200];
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(explorer.Current());
		Shape_FACE_OCC* pFaceOCC = new Shape_FACE_OCC();
		pFaceOCC->m_FaceIndex = index++;
		pFaceOCC->shape = face;
		pFaceOCC->pParentObject = pshape;

		get_triangles_from_faceted_face_occ(face, pFaceOCC->coords, pFaceOCC->triangles, pFaceOCC->normalCoords, pFaceOCC->edgepoints);
		pFaceOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Face%d", pFaceOCC->m_FaceIndex);
		pFaceOCC->filename = message;
		pDoc->m_SceneGraph.Add(pFaceOCC);
	}

	char message2[200];
	TopExp_Explorer explorer1(pshape->shape, TopAbs_EDGE);
	Standard_Integer index2 = 0;
	for (; explorer1.More(); explorer1.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer1.Current());
		Shape_EDGE_OCC* pEdgeOCC = new Shape_EDGE_OCC();
		pEdgeOCC->m_EdgeIndex = index++;
		pEdgeOCC->shape = edge;
		pEdgeOCC->pParentObject = pshape;

		get_tessellationpoints_from_edges_occ(pshape, edge, pEdgeOCC->edgepoints);
		pEdgeOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Edge%d", pEdgeOCC->m_EdgeIndex);
		pEdgeOCC->filename = message;
		pDoc->m_SceneGraph.Add(pEdgeOCC);
	}

	//add edge children
	//AddEdgeChildren(pshape, pDoc->m_SceneGraph);
}

// 显示边
void edge_visualization_ACIS(EDGE* u_edges[], EDGE* v_edges[], int usize, int vsize)
{
	ENTITY_LIST* pEdgeEntities = new ENTITY_LIST();

	// 添加线
	for (int i = 0; i < usize; ++i)
	{
		pEdgeEntities->add(u_edges[i]);
	}
	for (int i = 0; i < vsize; ++i)
	{
		pEdgeEntities->add(v_edges[i]);
	}

	char name[200];
	int edgeindex = 0;
	for (auto edgeEntity : *pEdgeEntities)
	{
		api_facet_entity(edgeEntity);
		Entity_EDGE_ACIS* pEdgeEntity = new Entity_EDGE_ACIS();

		EDGE* edge = (EDGE*)edgeEntity;
		get_edgepoints_from_faceted_edge(edge, pEdgeEntity->edgepoints);
		if (pEdgeEntity->edgepoints.size() == 0)
		{
			pEdgeEntity->edgepoints.push_back(edge->start_pos());
			pEdgeEntity->edgepoints.push_back(edge->mid_pos());
			pEdgeEntity->edgepoints.push_back(edge->end_pos());
		}


		pEdgeEntity->pAcisEntity = (EDGE*)edgeEntity;
		//pEdgeEntity->pParentObject = pFaceEntity;
		pEdgeEntity->pParentObject = pDoc->m_SceneGraph.GetAt(0);
		pEdgeEntity->SetTransform(pDoc->m_SceneGraph.GetAt(0)->m_Transform);
		sprintf(name, "Edge%d", edgeindex++);
		pEdgeEntity->filename = name;
		pDoc->m_SceneGraph.Add(pEdgeEntity);
	}

}

// ACIS sample
void Action_GORDEN::OnBnClickedButtonGorden()
{
	// TODO: 在此添加控件通知处理程序代码
	//int num = pDoc->m_SceneGraph.NbObject();
	//if (pDoc->m_CurrentObjectIndex < 0 || pDoc->m_CurrentObjectIndex >= num)
	//	return;
	//if (pDoc->m_SceneGraph.GetAt(0)->GetType() == TYPE_BREP_ACIS)
	//{
	//	EntityList_ACIS* pList = (EntityList_ACIS*)pDoc->m_SceneGraph.GetAt(0);
	//	AfxMessageBox("GORDEN");
	//}

	const int num_v_wires = 4;
	const int num_u_wires = 3;

	// 定义 u 方向和 v 方向的样条曲线
	EDGE* v_edges[num_v_wires];

	//std::vector<SPAposition> v_points1 = { SPAposition(0, 0, 0), SPAposition(0, 1, 0), SPAposition(0, 2, 0) };
	std::vector<SPAposition> v_points1 = { SPAposition(0, 0, 0), SPAposition(5, 10, 0), SPAposition(10, 5, 0), SPAposition(15, 15, 0), SPAposition(20, 0, 0) };
	v_edges[0] = create_spline_edge(v_points1);

	//std::vector<SPAposition> v_points2 = { SPAposition(2, 0, 1), SPAposition(2, 1, 1), SPAposition(2, 2, 1) };
	std::vector<SPAposition> v_points2 = { SPAposition(0, 10, 5), SPAposition(5, 5, 5), SPAposition(10, 15, 5), SPAposition(15, 10, 5), SPAposition(20, 10, 5) };
	v_edges[1] = create_spline_edge(v_points2);

	//std::vector<SPAposition> v_points3 = { SPAposition(4, 0, 0), SPAposition(4, 1, 0), SPAposition(4, 2, 0) };
	std::vector<SPAposition> v_points3 = { SPAposition(0, 20, 10), SPAposition(5, 15, 10), SPAposition(10, 20, 10), SPAposition(15, 5, 10), SPAposition(20, 20, 10) };
	v_edges[2] = create_spline_edge(v_points3);

	std::vector<SPAposition> v_points4 = { SPAposition(0, 15, 15), SPAposition(5, 10, 15), SPAposition(10, 15, 15), SPAposition(15, 0, 15), SPAposition(20, 15, 15) };
	v_edges[3] = create_spline_edge(v_points4);

	BODY* v_wires[num_v_wires];
	for (int i = 0; i < num_v_wires; ++i) {
		v_wires[i] = create_body_from_edges(&v_edges[i], 1); // 每个边转换为一个 BODY 对象
	}

	EDGE* u_edges[num_u_wires];
	//std::vector<SPAposition> u_points1 = { SPAposition(0, 0, 0), SPAposition(2, 0, 1), SPAposition(4, 0, 0) };
	std::vector<SPAposition> u_points1 = { SPAposition(0, 0, 0), SPAposition(0, 10, 5), SPAposition(0, 20, 10), SPAposition(0, 15, 15) };
	u_edges[0] = create_spline_edge(u_points1);

	//std::vector<SPAposition> u_points2 = { SPAposition(0, 1, 0), SPAposition(2, 1, 1), SPAposition(4, 1, 0) };
	std::vector<SPAposition> u_points2 = { SPAposition(10, 5, 0), SPAposition(10, 15, 5), SPAposition(10, 20, 10), SPAposition(10, 15, 15) };
	u_edges[1] = create_spline_edge(u_points2);

	//std::vector<SPAposition> u_points3 = { SPAposition(0, 2, 0), SPAposition(2, 2, 1), SPAposition(4, 2, 0) };
	std::vector<SPAposition> u_points3 = { SPAposition(20, 0, 0), SPAposition(20, 10, 5), SPAposition(20, 20, 10), SPAposition(20, 15, 15) };
	u_edges[2] = create_spline_edge(u_points3);

	BODY* u_wires[num_u_wires];
	for (int i = 0; i < num_u_wires; ++i) {
		u_wires[i] = create_body_from_edges(&u_edges[i], 1); // 每个边转换为一个 BODY 对象
	}

	// 测试api_skin_wires
	//BODY* wires[2]; 
	//EDGE* ueds[2];
	//SPAposition p(0, 0, 0);
	//SPAposition p2(0, 0, 1);
	//EDGE* e1;
	//EDGE* e2;
	//std::vector<SPAposition> ups1 = { SPAposition(0, 0, 0), SPAposition(1, 0, 0), SPAposition(2, 0, 0)};
	//std::vector<SPAposition> ups2 = { SPAposition(0, 1, 0), SPAposition(1, 1, 0), SPAposition(2, 1, 0) };
	//ueds[0] = create_spline_edge(ups1);
	//ueds[1] = create_spline_edge(ups2);
	//outcome res1 = api_curve_arc(p, 4, 0, 6.28, e1);
	//outcome res2 = api_curve_arc(p2, 4, 0, 6.28, e2);
	//wires[0] = create_body_from_edges(&e1, 1); 
	//wires[1] = create_body_from_edges(&e2, 1); 
	//for (int i = 0; i < 2; ++i) {
	//	wires[i] = create_body_from_edges(&ueds[i], 1); // 每个边转换为一个 BODY 对象
	//}

	// 测试net_sections
	//std::vector<std::vector<COEDGE*>> v_coedges(4);
	//std::vector<std::vector<COEDGE*>> u_coedges(3);
	//v_coedges[0].push_back(new COEDGE(v_edges[0], 0, NULL, NULL));
	//v_coedges[1].push_back(new COEDGE(v_edges[1], 0, NULL, NULL));
	//v_coedges[2].push_back(new COEDGE(v_edges[2], 0, NULL, NULL));
	//v_coedges[3].push_back(new COEDGE(v_edges[3], 0, NULL, NULL));
	//u_coedges[0].push_back(new COEDGE(u_edges[0], 0, NULL, NULL));
	//u_coedges[1].push_back(new COEDGE(u_edges[1], 0, NULL, NULL));
	//u_coedges[2].push_back(new COEDGE(u_edges[2], 0, NULL, NULL));
	//Loft_Connected_Coedge_List** v_sects = create_loft_connected_coedge_list(4, v_coedges);
	//Loft_Connected_Coedge_List** u_sects = create_loft_connected_coedge_list(3, u_coedges);

	BODY* sheet_body = nullptr;
	skin_options* opts = new skin_options(); // 使用默认选项
	opts->set_simplify(0);
	double net_tol = SPAresfit;
	//double net_tol = 2.0;
	//outcome res = api_net_sections(4, v_sects, 3, u_sects, sheet_body); // wet_sections 
	outcome res = api_net_wires(num_v_wires, v_wires, num_u_wires, u_wires, sheet_body, opts, net_tol); // net_wires
	//outcome res = api_skin_wires(2, wires, sheet_body, opts); // skin
	if (!res.ok()) {
		std::cerr << "Gordon曲面创建失败" << std::endl;
		AfxMessageBox("Cannot build Gordon surface!");
	}
	else
	{
		export_step_ACIS(sheet_body, "D:\\SurfaceHealing\\data\\step\\ACIS_GordonSample.step");
		face_visualization_ACIS(sheet_body); // 显示面
		edge_visualization_ACIS(u_edges, v_edges, num_u_wires, num_v_wires); // 显示边
		pDoc->UpdateTreeControl();
		AfxMessageBox("Gordon surface sample is created successfully");
	}
}

// ACIS 输入u、v方向的曲线，生成gordon并显示
void Action_GORDEN::OnBnClickedButtonCreategordon()
{
	// TODO: 在此添加控件通知处理程序代码
	//const int num = pDoc->m_SceneGraph.NbObject();

	if (pDoc->m_SceneGraph.NbObject() == 0)
	{
		AfxMessageBox("Input wires first!");
		return;
	}

	const int num_v_wires = 3;
	const int num_u_wires = 3;

	EDGE* edges[num_v_wires + num_u_wires];
	EDGE* v_edges[num_v_wires];
	EDGE* u_edges[num_u_wires];

	for (int i = 0; i < num_v_wires + num_u_wires; i++)
	{
		Entity_EDGE_ACIS* edgeEntity = (Entity_EDGE_ACIS*)pDoc->m_SceneGraph.GetAt(2 * i + 1);
		EDGE* edge = (EDGE*)edgeEntity->pAcisEntity;
		edges[i] = edge;
	}

	for (int i = 0; i < num_v_wires; i++)
	{
		v_edges[i] = edges[i];
		u_edges[i] = edges[i + num_v_wires];
	}

	BODY* v_wires[num_v_wires];
	for (int i = 0; i < num_v_wires; ++i) {
		v_wires[i] = create_body_from_edges(&v_edges[i], 1); // 每个边转换为一个 BODY 对象
	}

	BODY* u_wires[num_u_wires];
	for (int i = 0; i < num_u_wires; ++i) {
		u_wires[i] = create_body_from_edges(&u_edges[i], 1); // 每个边转换为一个 BODY 对象
	}

	BODY* sheet_body = nullptr;
	skin_options* opts = new skin_options(); // 使用默认选项
	double net_tol = SPAresfit;
	outcome res = api_net_wires(num_v_wires, v_wires, num_u_wires, u_wires, sheet_body, opts, net_tol);
	if (!res.ok()) {
		std::cerr << "Gordon曲面创建失败" << std::endl;
		AfxMessageBox("Cannot build Gordon surface!");
	}
	else
	{
		export_step_ACIS(sheet_body, "D:\\SurfaceHealing\\data\\step\\ACIS_Gordon.step");
		face_visualization_ACIS(sheet_body); // 显示面
		//edge_visualization_ACIS(u_edges, v_edges, num_u_wires, num_v_wires); // 显示边
		pDoc->UpdateTreeControl();
		AfxMessageBox("Gordon surface sample is created successfully");
	}
}

// ASitus 输入u、v方向的曲线，生成gordon并显示
void Action_GORDEN::OnBnClickedButtonAnalysis()
{
	/*
	// TODO: 在此添加控件通知处理程序代码
	if (pDoc->m_SceneGraph.NbObject() == 0)
	{
		AfxMessageBox("Input wires first!");
		return;
	}

	std::vector<TopoDS_Edge> profiles;
	std::vector<TopoDS_Edge> guides;
	Handle(Geom_BSplineSurface) resSurf;
	TopoDS_Face                 resFace;

	const int num_v_wires = 3;
	const int num_u_wires = 3;

	TopoDS_Edge edges[num_v_wires + num_u_wires];
	int n = pDoc->m_SceneGraph.NbObject();

	for (int i = 0; i < num_v_wires + num_u_wires; i++)
	{
		Shape_EDGE_OCC* pEdge = (Shape_EDGE_OCC*)pDoc->m_SceneGraph.GetAt(2 * i + 1);
		TopoDS_Shape edge = pEdge->shape;
		edges[i] = TopoDS::Edge(edge);
	}

	for (int i = 0; i < num_v_wires; i++)
	{
		guides.push_back(edges[i]);
		profiles.push_back(edges[i + num_v_wires]);
	}

	// Build Gordon surface.
	asiAlgo_BuildGordonSurf buildGordon(nullptr, nullptr);

	if (!buildGordon.Build(profiles, guides, resSurf, resFace))
	{
		AfxMessageBox("Cannot build Gordon surface!");
	}
	else
	{
		export_step_OCC(resFace, "D:\\SurfaceHealing\\data\\step\\OCC_Gordon.step");
		export_brep_OCC(resFace, "D:\\SurfaceHealing\\data\\brep\\OCC_Gordon.brep");
		face_visualization_OCC(resFace);
		pDoc->UpdateTreeControl();
		AfxMessageBox("Gordon surface sample is created successfully");
	}
	*/

	std::vector<TopoDS_Edge> profiles;
	std::vector<TopoDS_Edge> guides;
	Handle(Geom_BSplineSurface) resSurf;
	TopoDS_Face                 resFace;

	TopoDS_Shape shape;
	BRep_Builder b;
	std::ifstream is;
	is.open("D:\\work\\data\\input\\djwCases\\10_UResult.brep");
	BRepTools::Read(shape, is, b);
	is.close();

	for (TopExp_Explorer explorer(shape, TopAbs_EDGE); explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Standard_Real f, l;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
		Handle(Geom_BSplineCurve) bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
		if (!bsplineCurve.IsNull())
		{
			profiles.push_back(edge);
		}
	}

	TopoDS_Shape shape2;
	BRep_Builder b2;
	std::ifstream is2;
	is2.open("D:\\work\\data\\input\\djwCases\\10_VResult.brep");
	BRepTools::Read(shape2, is2, b2);
	is2.close();

	for (TopExp_Explorer explorer(shape2, TopAbs_EDGE); explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Standard_Real f, l;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
		Handle(Geom_BSplineCurve) bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
		if (!bsplineCurve.IsNull())
		{
			guides.push_back(edge);
		}
	}

	// Build Gordon surface.
	asiAlgo_BuildGordonSurf buildGordon(nullptr, nullptr);

	if (!buildGordon.Build(profiles, guides, resSurf, resFace))
	{
		AfxMessageBox("Cannot build Gordon surface!");
	}
	else
	{
		export_step_OCC(resFace, "D:\\SurfaceHealing\\data\\step\\OCC_Gordon.step");
		export_brep_OCC(resFace, "D:\\SurfaceHealing\\data\\brep\\OCC_Gordon.brep");
		face_visualization_OCC(resFace);
		pDoc->UpdateTreeControl();
		AfxMessageBox("Gordon surface sample is created successfully");
	}

}

void Action_GORDEN::export_step_OCC(TopoDS_Shape shape, std::string filePath)
{
	STEPControl_Writer stepWriter;
	stepWriter.Transfer(shape, STEPControl_AsIs);
	IFSelect_ReturnStatus status = stepWriter.Write(filePath.c_str());
}

void Action_GORDEN::export_brep_OCC(TopoDS_Shape shape, std::string filePath)
{
	BRep_Builder builder;
	BRepTools::Write(shape, filePath.c_str());
}

void Action_GORDEN::export_step_ACIS(ENTITY* ent, std::string filePath)
{
	ENTITY_LIST* saveList = new ENTITY_LIST();
	saveList->add(ent);
	SPAIAcisDocument dst(saveList);
	SPAIDocument dstnew(filePath.c_str());
	dstnew.SetType("step");
	SPAIConverter converter;
	SPAIResult result;
	result &= converter.Convert(dst, dstnew);

}

void Action_GORDEN::GeomLib_ChangeUBounds(Handle(Geom_BSplineSurface)& aSurface,
	const Standard_Real newU1,
	const Standard_Real newU2)
{
	TColStd_Array1OfReal  knots(1, aSurface->NbUKnots());
	aSurface->UKnots(knots);
	BSplCLib::Reparametrize(newU1, newU2, knots);
	aSurface->SetUKnots(knots);
}

void Action_GORDEN::GeomLib_ChangeVBounds(Handle(Geom_BSplineSurface)& aSurface,
	const Standard_Real newV1,
	const Standard_Real newV2)
{
	TColStd_Array1OfReal  knots(1, aSurface->NbVKnots());
	aSurface->VKnots(knots);
	BSplCLib::Reparametrize(newV1, newV2, knots);
	aSurface->SetVKnots(knots);
}

void Action_GORDEN::GeomLib_ChangeCurveBounds(Handle(Geom_BSplineCurve)& aCurve,
	const Standard_Real newU1,
	const Standard_Real newU2)
{
	TColStd_Array1OfReal  knots(1, aCurve->NbKnots());
	aCurve->Knots(knots);
	BSplCLib::Reparametrize(newU1, newU2, knots);
	aCurve->SetKnots(knots);
}

Standard_Integer Action_GORDEN::SetSameDistribution(Handle(Geom_BSplineCurve)& C1,
	Handle(Geom_BSplineCurve)& C2)
{
	Standard_Integer nbp1 = C1->NbPoles();
	Standard_Integer nbk1 = C1->NbKnots();
	TColgp_Array1OfPnt      P1(1, nbp1);
	TColStd_Array1OfReal    W1(1, nbp1);
	W1.Init(1.);
	TColStd_Array1OfReal    K1(1, nbk1);
	TColStd_Array1OfInteger M1(1, nbk1);

	C1->Poles(P1);
	if (C1->IsRational())
		C1->Weights(W1);
	C1->Knots(K1);
	C1->Multiplicities(M1);

	Standard_Integer nbp2 = C2->NbPoles();
	Standard_Integer nbk2 = C2->NbKnots();
	TColgp_Array1OfPnt      P2(1, nbp2);
	TColStd_Array1OfReal    W2(1, nbp2);
	W2.Init(1.);
	TColStd_Array1OfReal    K2(1, nbk2);
	TColStd_Array1OfInteger M2(1, nbk2);

	C2->Poles(P2);
	if (C2->IsRational())
		C2->Weights(W2);
	C2->Knots(K2);
	C2->Multiplicities(M2);

	Standard_Real K11 = K1(1);
	Standard_Real K12 = K1(nbk1);
	Standard_Real K21 = K2(1);
	Standard_Real K22 = K2(nbk2);

	if ((K12 - K11) > (K22 - K21)) {
		BSplCLib::Reparametrize(K11, K12, K2);
		C2->SetKnots(K2);
	}
	else if ((K12 - K11) < (K22 - K21)) {
		BSplCLib::Reparametrize(K21, K22, K1);
		C1->SetKnots(K1);
	}
	else if (Abs(K12 - K11) > 1.e-15) {
		BSplCLib::Reparametrize(K11, K12, K2);
		C2->SetKnots(K2);
	}

	Standard_Integer NP, NK;
	if (BSplCLib::PrepareInsertKnots(C1->Degree(), Standard_False,
		K1, M1, K2, &M2, NP, NK, 1.e-15,
		Standard_False)) {
		TColgp_Array1OfPnt      NewP(1, NP);
		TColStd_Array1OfReal    NewW(1, NP);
		TColStd_Array1OfReal    NewK(1, NK);
		TColStd_Array1OfInteger NewM(1, NK);
		BSplCLib::InsertKnots(C1->Degree(), Standard_False,
			P1, &W1, K1, M1, K2, &M2,
			NewP, &NewW, NewK, NewM, 1.e-15,
			Standard_False);
		if (C1->IsRational()) {
			C1 = new Geom_BSplineCurve(NewP, NewW, NewK, NewM, C1->Degree());
		}
		else {
			C1 = new Geom_BSplineCurve(NewP, NewK, NewM, C1->Degree());
		}
		BSplCLib::InsertKnots(C2->Degree(), Standard_False,
			P2, &W2, K2, M2, K1, &M1,
			NewP, &NewW, NewK, NewM, 1.e-15,
			Standard_False);
		if (C2->IsRational()) {
			C2 = new Geom_BSplineCurve(NewP, NewW, NewK, NewM, C2->Degree());
		}
		else {
			C2 = new Geom_BSplineCurve(NewP, NewK, NewM, C2->Degree());
		}
	}
	else {
		throw Standard_ConstructionError(" ");
	}

	return C1->NbPoles();
}

void Action_GORDEN::OnBnClickedButtonMygordon()
{
	std::vector<Handle(Geom_BSplineCurve)> uCurves;
	std::vector<Handle(Geom_BSplineCurve)> vCurves;
	TopoDS_Face face;

	// ucurves
	TopoDS_Shape shape;
	BRep_Builder b;
	std::ifstream is;
	is.open("D:\\work\\svn_hxy\\data\\input\\djwCases\\2_UResult.brep");
	//is.open("D:\\work\\data\\input\\badCases\\uCreateGordenCurves.brep");
	BRepTools::Read(shape, is, b);
	is.close();

	for (TopExp_Explorer explorer(shape, TopAbs_EDGE); explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Standard_Real f, l;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
		Handle(Geom_BSplineCurve) bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
		if (!bsplineCurve.IsNull())
		{
			uCurves.push_back(bsplineCurve);
		}
	}

	//vcurves
	TopoDS_Shape shape2;
	BRep_Builder b2;
	std::ifstream is2;
	is2.open("D:\\work\\svn_hxy\\data\\input\\djwCases\\2_VResult.brep");
	//is2.open("D:\\work\\data\\input\\badCases\\vCreateGordenCurves.brep");
	BRepTools::Read(shape2, is2, b2);
	is2.close();

	for (TopExp_Explorer explorer(shape2, TopAbs_EDGE); explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Standard_Real f, l;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
		Handle(Geom_BSplineCurve) bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
		if (!bsplineCurve.IsNull())
		{
			vCurves.push_back(bsplineCurve);
		}
	}

	// 计算uv等参线参数，提供后面扩展gordon所需的等参线约束
	std::vector<Standard_Real> uIsoparamParams;
	std::vector<Standard_Real> vIsoparamParams;

	BuildMyGordonSurf(uCurves, vCurves, uIsoparamParams, vIsoparamParams, face);
	if (face.IsNull())
	{
		std::cout << "Gordon曲面生成失败！" << std::endl;
		return;
	}

	
	// 直接输入一个面
	STEPControl_Reader reader;
	char* filename = "D:\\work\\svn_hxy\\data\\input\\11coons.step";
	IFSelect_ReturnStatus status = reader.ReadFile(filename);

	if (status != IFSelect_ReturnStatus::IFSelect_RetDone)
	{
		std::cerr << "Error reading STEP file." << std::endl;
		//AfxMessageBox("Error reading STEP file.");
		return;
	}
	// 传输读取的数据
	reader.TransferRoots();
	TopoDS_Shape shape4 = reader.OneShape();

	TopoDS_Face face4;
	for (TopExp_Explorer explorer(shape4, TopAbs_FACE); explorer.More(); explorer.Next())
	{
		face4 = TopoDS::Face(explorer.Current());
		break;
	}
	
	
	// try guide
	std::vector<Handle(Geom_BSplineCurve)> guideCurves;
	TopoDS_Shape shape3;
	BRep_Builder b3;
	std::ifstream is3;
	//is3.open("D:\\work\\data\\input\\newCases\\test1\\internal.brep");
	is3.open("D:\\work\\svn_hxy\\data\\input\\internal\\11_internal_2.brep");
	BRepTools::Read(shape3, is3, b3);
	is3.close();

	for (TopExp_Explorer explorer(shape3, TopAbs_EDGE); explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		Standard_Real f, l;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
		Handle(Geom_BSplineCurve) bsplineCurve = Handle(Geom_BSplineCurve)::DownCast(curve);
		if (!bsplineCurve.IsNull())
		{
			guideCurves.push_back(bsplineCurve);
		}
	}
	Handle(Geom_BSplineSurface) guidedGordonSurf;
	GuideGordon::GuideGordonSurf(BRep_Tool::Surface(face4), uIsoparamParams, vIsoparamParams, guideCurves, guidedGordonSurf);
	TopoDS_Face guidedFace = BRepBuilderAPI_MakeFace(guidedGordonSurf, Precision::Confusion());
	export_step_OCC(guidedFace, "D:\\work\\svn_hxy\\data\\guidedGordon\\guidedGordonSurf.step");
	

	face_visualization_OCC(guidedFace);
	pDoc->UpdateTreeControl();

	AfxMessageBox("Gordon Created Successfully!");

}

void Action_GORDEN::BuildMyGordonSurf(std::vector<Handle(Geom_BSplineCurve)> uCurves,
	std::vector<Handle(Geom_BSplineCurve)> vCurves,
	std::vector<Standard_Real>& uIsoparamParams,
	std::vector<Standard_Real>& vIsoparamParams,
	TopoDS_Face& face)
{
	//--------------- 检查传入曲线是否空 ---------------
	if (uCurves.empty())
	{
		std::cout << "U向曲线为空！" << std::endl;
		return;
	}
	if (uCurves.empty())
	{
		std::cout << "V向曲线为空！" << std::endl;
		return;
	}


	//--------------- 提升曲线次数至相同 ---------------
	Standard_Integer uDegree = 3, vDegree = 3;
	for (auto c : uCurves)
	{
		uDegree = max(uDegree, c->Degree());
	}
	for (auto c : vCurves)
	{
		vDegree = max(uDegree, c->Degree());
	}

	for (auto c : uCurves)
	{
		c->IncreaseDegree(uDegree);
	}
	for (auto c : vCurves)
	{
		c->IncreaseDegree(vDegree);
	}


	//--------------- 将曲线方向调整为一致 ---------------
	Standard_Integer usize = uCurves.size();
	Standard_Integer vsize = vCurves.size();
	Standard_Integer i = 0;

	while (i < usize - 1)
	{
		gp_Pnt p1, p2;
		gp_Vec v1, v2;
		Standard_Real midPara1 = 0.5 * (uCurves[i]->FirstParameter() + uCurves[i]->LastParameter());
		Standard_Real midPara2 = 0.5 * (uCurves[i + 1]->FirstParameter() + uCurves[i + 1]->LastParameter());
		uCurves[i]->D1(midPara1, p1, v1);
		uCurves[i + 1]->D1(midPara2, p2, v2);

		if (v1.Dot(v2) < 0)
		{
			uCurves[i + 1]->Reverse();
		}

		i++;
	}

	i = 0;
	while (i < vsize - 1)
	{
		gp_Pnt p1, p2;
		gp_Vec v1, v2;
		Standard_Real midPara1 = 0.5 * (vCurves[i]->FirstParameter() + vCurves[i]->LastParameter());
		Standard_Real midPara2 = 0.5 * (vCurves[i + 1]->FirstParameter() + vCurves[i + 1]->LastParameter());
		vCurves[i]->D1(midPara1, p1, v1);
		vCurves[i + 1]->D1(midPara2, p2, v2);

		if (v1.Dot(v2) < 0)
		{
			vCurves[i + 1]->Reverse();
		}

		i++;
	}


	//--------------- 调整两个方向的曲线起点一致 ---------------
	std::vector< std::pair<unsigned, unsigned> >
		oris = { {0u, 0u},
				 {0u, 1u},
				 {1u, 0u},
				 {1u, 1u} };

	Standard_Boolean syncStop = false;
	size_t syncAttempt = 0;
	do
	{
		std::vector<Handle(Geom_BSplineCurve)> _uCurves;
		std::vector<Handle(Geom_BSplineCurve)> _vCurves;

		// Reverse if the flag is true.
		if (oris[syncAttempt].first)
		{
			for (const auto& C : uCurves)
			{
				_uCurves.push_back(Handle(Geom_BSplineCurve)::DownCast(C->Reversed()));
			}
		}
		else
		{
			_uCurves = uCurves;
		}

		// Reverse if the flag is true.
		if (oris[syncAttempt].second)
		{
			for (const auto& C : vCurves)
			{
				_vCurves.push_back(Handle(Geom_BSplineCurve)::DownCast(C->Reversed()));
			}
		}
		else
		{
			_vCurves = vCurves;
		}

		const gp_Pnt OP = _uCurves[0]->Value(_uCurves[0]->FirstParameter());
		const gp_Pnt OG = _vCurves[0]->Value(_vCurves[0]->FirstParameter());

		if (OP.Distance(OG) < 1.e-2)
		{
			syncStop = true;
			uCurves = _uCurves;
			vCurves = _vCurves;
		}

		if (++syncAttempt > 3)
		{
			syncStop = true;
		}
	} while (!syncStop);


	//--------------- 将两组曲线分别设置为相容 ---------------

	//for (auto c : uCurves)
	//{
	//	GeomLib_ChangeCurveBounds(c, 0, 1);
	//}
	//for (auto c : vCurves)
	//{
	//	GeomLib_ChangeCurveBounds(c, 0, 1);
	//}
	//for (int i = 0; i < uCurves.size() - 1; i++)
	//{
	//	SetSameDistribution(uCurves[i], uCurves[i + 1]);
	//}
	//for (int i = 0; i < vCurves.size() - 1; i++)
	//{
	//	SetSameDistribution(vCurves[i], vCurves[i + 1]);
	//}
	//for (int i = 0; i < uCurves.size() - 1; i++)
	//{
	//	SetSameDistribution(uCurves[i], uCurves[uCurves.size() - 1]);
	//}
	//for (int i = 0; i < vCurves.size() - 1; i++)
	//{
	//	SetSameDistribution(vCurves[i], vCurves[vCurves.size() - 1]);
	//}


	std::vector<Standard_Real> errors;
	std::vector<Handle(Geom_BSplineCurve)> uCurvesCom;
	std::vector<Handle(Geom_BSplineCurve)> vCurvesCom;
	errors = Compatible::ApproximateCompatible(uCurves, uCurvesCom, 0.1);
	errors = Compatible::ApproximateCompatible(vCurves, vCurvesCom, 0.1);

	uCurves = uCurvesCom;
	vCurves = vCurvesCom;

	//--------------- 计算曲线的交点参数 ---------------
	std::vector<gp_Pnt> Pnts;
	std::vector<gp_Pnt2d> PntParams;

	for (Standard_Integer i = 0; i < usize; i++)
	{
		for (Standard_Integer j = 0; j < vsize; j++)
		{
			// u向退化边
			if (uCurves[i]->StartPoint().IsEqual(uCurves[i]->EndPoint(), 1.e-2))
			{
				Pnts.push_back(uCurves[i]->StartPoint());
				Standard_Real u = j == 0 ? 0 : (j + 1.0) / vsize; // 参数u暂时平均取
				Standard_Real v = i == 0 ? 0 : 1; // 参数v为0或1
				gp_Pnt2d Pnt2d(u, v);
				PntParams.push_back(Pnt2d);
				continue;
			}
			// v向退化边
			else if (vCurves[j]->StartPoint().IsEqual(vCurves[j]->EndPoint(), 1.e-2))
			{
				Pnts.push_back(vCurves[j]->StartPoint());
				Standard_Real u = j == 0 ? 0 : 1; // 参数u为0或1
				Standard_Real v = i == 0 ? 0 : (i + 1.0) / usize; // 参数v暂时平均取
				gp_Pnt2d Pnt2d(u, v);
				PntParams.push_back(Pnt2d);
				continue;
			}

			GeomAPI_ExtremaCurveCurve extrema(uCurves[i], vCurves[j]);
			if (extrema.NbExtrema() != 1)
			{
				std::cout << "最近点对不唯一！" << std::endl;
			}
			Standard_Integer nbEx = extrema.NbExtrema();

			Standard_Real para1, para2;
			extrema.Parameters(1, para1, para2);

			gp_Pnt p1, p2;
			extrema.NearestPoints(p1, p2);
			gp_Pnt interPnt((p1.XYZ() + p2.XYZ()) / 2.0); // 取p1p2的中点	
			Pnts.push_back(interPnt);
			gp_Pnt2d Pnt2d(para1, para2);
			PntParams.push_back(Pnt2d);
		}
	}
	//check 一下最近点对的数量是否是usize*vsize
	if (Pnts.size() != usize * vsize)
	{
		std::cout << "获取的等参线最近点对数量有误！" << std::endl;
	}

	// 求均值得到u等参线参数
	std::vector<Standard_Real> uIsoparamParams1, vIsoparamParams1;
	for (Standard_Integer i = 0; i < vsize; i++)
	{
		Standard_Real sumU = 0;
		for (Standard_Integer j = i; j < PntParams.size(); j += vsize)
		{
			sumU += PntParams[j].X();
		}
		uIsoparamParams1.push_back(sumU / usize);
	}

	// 求均值得到v等参线参数
	for (Standard_Integer i = 0; i < usize; i++)
	{
		Standard_Real sumV = 0;
		for (Standard_Integer j = vsize * i; j < vsize * (i + 1); j++)
		{
			sumV += PntParams[j].Y();
		}
		vIsoparamParams1.push_back(sumV / vsize);
	}

	// test
	//for (Standard_Integer i = 1; i < uIsoparamParams1.size() - 1; i++)
	//{
	//	uIsoparamParams.push_back(uIsoparamParams1[i]);
	//}
	//for (Standard_Integer i = 1; i < vIsoparamParams1.size() - 1; i++)
	//{
	//	vIsoparamParams.push_back(vIsoparamParams1[i]);
	//}


	//--------------- 构造三张曲面 ---------------
	Handle(Geom_BSplineSurface) L1, L2, T;

	Standard_Integer uDegree1 = usize <= 3 ? 1 : 3;
	Standard_Integer vDegree1 = vsize <= 3 ? 1 : 3;

	// chenxin's loft
	L1 = InterPolateTool::Loft(uCurves, uDegree1);
	L2 = InterPolateTool::LoftV(vCurves, vDegree1);

	if (L1.IsNull() || L2.IsNull())
	{
		std::cout << "放样失败！" << std::endl;
		return;
	}
	TopoDS_Face face1 = BRepBuilderAPI_MakeFace(L1, Precision::Confusion());
	export_step_OCC(face1, "D:\\work\\svn_hxy\\data\\loft\\cxuloft.step");
	TopoDS_Face face2 = BRepBuilderAPI_MakeFace(L2, Precision::Confusion());
	export_step_OCC(face2, "D:\\work\\svn_hxy\\data\\loft\\cxvloft.step");

	// cx
	TColStd_Array1OfReal uKnotsTCol = L2->UKnots();
	TColStd_Array1OfInteger uMultsTCol = L2->UMultiplicities();
	TColStd_Array1OfReal vKnotsTCol = L1->VKnots();
	TColStd_Array1OfInteger vMultsTCol = L1->VMultiplicities();
	std::vector<Standard_Real> uKnots;
	std::vector<Standard_Real> vKnots;
	std::vector<Standard_Integer> uMults;
	std::vector<Standard_Integer> vMults;
	for (Standard_Integer i = 1; i <= uKnotsTCol.Size(); i++)
	{
		uKnots.push_back(uKnotsTCol(i));
	}
	for (Standard_Integer i = 1; i <= vKnotsTCol.Size(); i++)
	{
		vKnots.push_back(vKnotsTCol(i));
	}
	for (Standard_Integer i = 1; i <= uMultsTCol.Size(); i++)
	{
		uMults.push_back(uMultsTCol(i));
	}
	for (Standard_Integer i = 1; i <= vMultsTCol.Size(); i++)
	{
		vMults.push_back(vMultsTCol(i));
	}

	T = InterPolateTool::Interpolate(Pnts, PntParams, uKnots, vKnots, uMults, vMults, vDegree1, uDegree1);
	TopoDS_Face face0 = BRepBuilderAPI_MakeFace(T, Precision::Confusion());
	export_step_OCC(face0, "D:\\work\\svn_hxy\\data\\interpolate\\cxInterpolate.step");

	//这个地方的度数待定，不一定是Curve的度数了
	L1->IncreaseDegree(uCurves[0]->Degree(), vCurves[0]->Degree());
	L2->IncreaseDegree(uCurves[0]->Degree(), vCurves[0]->Degree());
	T->IncreaseDegree(uCurves[0]->Degree(), vCurves[0]->Degree());


	const Standard_Integer nbPolesU1 = L1->NbUPoles();
	const Standard_Integer nbPolesV1 = L1->NbVPoles();
	const Standard_Integer nbPolesU21 = L2->NbUPoles();
	const Standard_Integer nbPolesV21 = L2->NbVPoles();
	const Standard_Integer nbPolesU121 = T->NbUPoles();
	const Standard_Integer nbPolesV121 = T->NbVPoles();

	//--------------- 节点细化 ---------------
	//将三张曲面的参数域都scale到[0,1]
	GeomLib_ChangeUBounds(L1, 0, 1);
	GeomLib_ChangeVBounds(L1, 0, 1);
	GeomLib_ChangeUBounds(L2, 0, 1);
	GeomLib_ChangeVBounds(L2, 0, 1);
	GeomLib_ChangeUBounds(T, 0, 1);
	GeomLib_ChangeVBounds(T, 0, 1);

	Compatible::SetSurfaceCompatible(L1, L2, T);

	//--------------- 得到共同节点向量和次数 ---------------
	const TColStd_Array1OfReal knotsU = L1->UKnots();
	const TColStd_Array1OfReal knotsV = L1->VKnots();
	const TColStd_Array1OfReal knotsU2 = L2->UKnots();
	const TColStd_Array1OfReal knotsV2 = L2->VKnots();
	const TColStd_Array1OfReal knotsUT = T->UKnots();
	const TColStd_Array1OfReal knotsVT = T->VKnots();
	const TColStd_Array1OfInteger multsU = L1->UMultiplicities();
	const TColStd_Array1OfInteger multsV = L1->VMultiplicities();
	const TColStd_Array1OfInteger multsU2 = L2->UMultiplicities();
	const TColStd_Array1OfInteger multsV2 = L2->VMultiplicities();
	const TColStd_Array1OfInteger multsU3 = T->UMultiplicities();
	const TColStd_Array1OfInteger multsV3 = T->VMultiplicities();

	const Standard_Integer degreeU = L1->UDegree();
	const Standard_Integer degreeV = L1->VDegree();


	//--------------- 计算控制点 ---------------
	const TColgp_Array2OfPnt poles1 = L1->Poles();
	const TColgp_Array2OfPnt poles2 = L2->Poles();
	const TColgp_Array2OfPnt poles12 = T->Poles();

	const Standard_Integer nbPolesU = L1->NbUPoles();
	const Standard_Integer nbPolesV = L1->NbVPoles();
	const Standard_Integer nbPolesU2 = L2->NbUPoles();
	const Standard_Integer nbPolesV2 = L2->NbVPoles();
	const Standard_Integer nbPolesU12 = T->NbUPoles();
	const Standard_Integer nbPolesV12 = T->NbVPoles();

	TColgp_Array2OfPnt resPole(1, nbPolesU, 1, nbPolesV);

	for (Standard_Integer i = 1; i <= nbPolesU12; i++)
	{
		for (Standard_Integer j = 1; j <= nbPolesV12; j++)
		{
			gp_XYZ coord = poles1(i, j).Coord() + poles2(i, j).Coord() - poles12(i, j).Coord();
			resPole(i, j).SetCoord(coord.X(), coord.Y(), coord.Z());
		}
	}


	//--------------- 构造Gordon曲面 ---------------
	Handle(Geom_Surface) gordon = new Geom_BSplineSurface(resPole, knotsU,
		knotsV, multsU, multsV, degreeU, degreeV);

	face = BRepBuilderAPI_MakeFace(gordon, Precision::Confusion());
	export_step_OCC(face, "D:\\work\\svn_hxy\\data\\gordon\\gordonSurf.step");
}
