#define TPMM
#include "stdafx.h"
#include "CParseSTEP.h"
#include <SPAIAcisDocument.h>

#include <kernapi.hxx>
#include <transf.hxx>
#include "lists.hxx"
#include "lib3d\Base3d.h"
#include "EntityList_ACIS.h"
#include "Entity_EDGE_ACIS.h"
#include "intrapi.hxx"
#include "Nurbs3D.h"

#include "Lib3D/Base3d.h"
#include "lib3d/Array3d.h"
#include <vector>
#include <GL/gl.h>

//#include "acis/gme/faceter/gme_idx_mesh_utils.hxx"
#include "acistype.hxx"
#include "edge.hxx"
#include "face.hxx"
#include "fct_utl.hxx"
#include "getowner.hxx"
#include "kernapi.hxx"
#include "point.hxx"
#include "rnd_api.hxx"
#include "transf.hxx"
#include "vertex.hxx"
#include <SVNMesh.h>
#include <Entity_FACE_ACIS.h>
#include <STEPControl_Reader.hxx>
#include <TShape_OCC.h>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRepBuilderAPI.hxx>
#include "BRepBndLib.hxx"
#include <GeometricEngineSelect.h>
#include <Poly_Triangulation.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include<TopoDS_Face.hxx>
#include<gp_Pnt.hxx>
#include <NCollection_Array1.hxx>
#include<Poly_Polygon3D.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <TopoDS_Edge.hxx>
#include <vector>
#include <Shape_FACE_OCC.h>
#include "BRep_Builder.hxx"
#include <BRepTools.hxx>
#include <SPAISystemError.h>
#include <Shape_EDGE_OCC.h>
#include "Shape_VERTEX_OCC.h"
#include "BRepBuilderAPI_Sewing.hxx"
#include <mt_stitch_apis.hxx>
#include "Entity_VERTEX_ACIS.h"
#include "TopExp.hxx"
#include "Interface_Static.hxx"
#include "SPAISystemError.h"

void facet_entitylist(ENTITY_LIST& entitylist);


static void get_triangles_from_faceted_face_occ(TopoDS_Shape shape, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords, std::vector<std::vector<Point3D>>& edgepoints)
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
			if (location.IsIdentity())
			{
				coords.push_back(p.X());
				coords.push_back(p.Y());
				coords.push_back(p.Z());
			}
			else
			{
				p.Transformed(location);
			}

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
static void get_triangles_from_faceted_face(FACE* face, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords, std::vector<std::vector<SPAposition>>& edgepoints) {
	af_serializable_mesh* sm = GetSerializableMesh(face);
	if (nullptr == sm) {
		// Application decision: do we throw for unfaceted faces?
		return;
	}
	SPAtransf tr = get_owner_transf(face);

	const int nv = sm->number_of_vertices();
	int ntri = sm->number_of_polygons();

	coords.resize(3 * nv);
	sm->serialize_positions(coords.data());  // if std::vector::data is not available, &(coords[0]) will also work.
	if (!tr.identity()) {
		for (int ii = 0; ii < nv; ii++) {
			int jj = 3 * ii;
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
	int ntri_actual = sm->serialize_triangles(triangles.data());
	while (ntri_actual < ntri) {
		triangles.pop_back();
		ntri_actual = static_cast<int>(triangles.size());
	}

	//

	ENTITY_LIST edgelist;
	api_get_edges(face, edgelist);
	edgepoints.resize(edgelist.count());
	int i = 0;
	int numpoints;
	SPAposition* polyline;
	for (auto edge : edgelist)
	{
		api_get_facet_edge_points((EDGE*)edge, polyline, numpoints);
		for (int j = 0; j < numpoints; j++)
			edgepoints[i].push_back(polyline[j]);
		i++;
	}

}

static void get_triangles_from_faceted_faces(ENTITY_LIST& faces, std::vector<SVNMesh::FaceMesh>& faceData, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords) {
	//static void get_triangles_from_faceted_faces(ENTITY_LIST& faces, std::vector<float>& coords, std::vector<int>& triangles, std::vector<float>& normalCoords) {

	int nF = 0;
	int nV = 0;
	int nI = 0;
	int numFaces = faces.iteration_count();
	//assert(numFaces == faceData.size());
	for (ENTITY* ent = faces.first(); ent; ent = faces.next()) {
		//assert(nF < numFaces);
		//assert(is_FACE(ent));
		if (!is_FACE(ent)) {
			continue;
		}

		FACE* face = (FACE*)ent;
		std::vector<float> temp_coords;
		std::vector<int> temp_triangles;
		std::vector<float> temp_normalCoords;
		std::vector<std::vector<SPAposition>> edgepoints;

		get_triangles_from_faceted_face(face, temp_coords, temp_triangles, temp_normalCoords, edgepoints);
		{
			int nCoordsStart = (int)coords.size() / 3;
			int nCoords = (int)temp_coords.size();
			for (int ii = 0; ii < nCoords; ii++) {
				coords.push_back(temp_coords[ii]);
				normalCoords.push_back(temp_normalCoords[ii]);
			}
			int nTri = (int)temp_triangles.size();
			for (int jj = 0; jj < nTri; jj++) {
				triangles.push_back(temp_triangles[jj] + nCoordsStart);
			}
		}
		logical found = FALSE;
		outcome out = api_rh_get_entity_rgb(ent, faceData[nF].color, TRUE, found);

		if (!out.ok() || !found) {
			faceData[nF].color = rgb_color(1., 1., 1.);
		}
		faceData[nF].numIndices = (unsigned int)temp_triangles.size();
		faceData[nF].baseIndex = (unsigned int)nI;
		faceData[nF].baseVertex = (unsigned int)nV;
		faceData[nF].ptrFace = face;

		nI += (unsigned int)temp_triangles.size();
		nV += (unsigned int)temp_coords.size();
		nF++;
	}
}

int CParseSTEP::get_FaceList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pFaceEntities)
{
	ENTITY_LIST facelist;
	for (ENTITY* ent = pAcisEntities->first(); ent; ent = pAcisEntities->next()) {
		//assert(nF < numFaces);
		//if (is_BODY(ent))
		facelist.clear();
		api_get_faces(ent, facelist);
		pFaceEntities->add(facelist);
	}
	return 1;
}

void CParseSTEP::SaveObjFile(char* filename, EntityList_ACIS* pEntityList)

{
	std::ofstream file(filename);

	//file << NbVertex() << "  " << NbFace() << "\n";
	for (int i = 0; i < pEntityList->coords.size(); i++)
	{

	}
	/* //Vertices
	 for (MyMesh::VertexIter v_it = pmesh->vertices_begin(); v_it != pmesh->vertices_end(); ++v_it)
	 {
		 int vindex = v_it.handle().idx();
		 OpenMesh::Vec3d pt;
		 pt = pmesh->point(v_it.handle());
		 file << "v " << pt[0] << " " << pt[1] << " " << pt[2] << " \n";
	 }

	 //Faces
	 std::vector<int> vhandles;
	 for (MyMesh::FaceIter f_it = pmesh->faces_begin(); f_it != pmesh->faces_end(); ++f_it)
	 {
		 vhandles.clear();
		 for (MyMesh::FaceVertexIter fv_it = pmesh->fv_iter(f_it.handle()); fv_it; fv_it++)
		 {
			 vhandles.push_back(fv_it.handle().idx());
		 }
		 file << "f " << vhandles[0] + 1 << " " << vhandles[1] + 1 << " " << vhandles[2] + 1 << " \n";
	 }

	 file.close();*/
}

//运行读取step文件进行几何引擎

int CParseSTEP::Run_STEP(char* filename, CSceneGraph3d* pSceneGraph)
{
	GeometricEngineSelect dlg;
	int IsAcisEngine = 1;
	int flag;
	if (dlg.DoModal())
	{
		IsAcisEngine = dlg.m_acis;
	}
	IsAcisEngine = dlg.m_acis;
	if (IsAcisEngine)
	{
		flag = Run_Acis(filename, pSceneGraph, "STEP");
	}
	else
	{
		flag = Run_OCC_STEP(filename, pSceneGraph);
	}
	//读入失败
	return flag;
}

int CParseSTEP::get_EdgeList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pEdgeEntities)
{
	for (ENTITY* ent = pAcisEntities->first(); ent; ent = pAcisEntities->next())
	{
		ENTITY_LIST edgeList;
		api_get_edges(ent, edgeList);
		//ENTITY* TEST = edgeList.first();
		pEdgeEntities->add(edgeList);
	}
	return 1;
}
int CParseSTEP::get_VertexList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pVertexEntities)
{
	for (ENTITY* ent = pAcisEntities->first(); ent; ent = pAcisEntities->next())
	{
		ENTITY_LIST vertexList;
		api_get_vertices(ent, vertexList);
		//ENTITY* TEST = edgeList.first();
		pVertexEntities->add(vertexList);
	}
	return 1;
}

void CParseSTEP::get_tessellationpoints_from_Edges(EDGE* edge, std::vector<SPAposition>& edgepoints)
{
	int numpoints;
	SPAposition* polyline;
	api_get_facet_edge_points(edge, polyline, numpoints);
	for (int i = 0; i < numpoints; i++)
	{
		edgepoints.push_back(polyline[i]);
	}
}
// SKY 6/1/06 - Licensing project
extern void unlock_license();
int CParseSTEP::Run_Acis(char* filename, CSceneGraph3d* pSceneGraph, const char* type)
{
	ENTITY_LIST* pAcisEntities = new ENTITY_LIST();

	ENTITY_LIST* pFaceEntities = new ENTITY_LIST();

	ENTITY_LIST* pEdgeEntities = new ENTITY_LIST();

	ENTITY_LIST* pVertexEntities = new ENTITY_LIST();

	//try
	//{
		// 目标ACIS文档对象

	SPAIAcisDocument dst;
	// 创建源文档对象，并设置输入格式
	SPAIDocument src(filename);
	src.SetType(type);
	// 创建转换器对象，开始转换过程
	SPAIConverter converter;
	SPAIResult result;

	//将源文件文档转换为目标Acis文档对象
	result &= converter.Convert(src, dst);

	// 获取转换后的实体列表
	dst.GetEntities(pAcisEntities);

	//}
	//catch (SPAISystemError& err)
	//{
	//	//std::cerr << err.GetMessage() << std::endl;
	//	//const char* messge = err.GetMessage();
	//	const SPAIValue er = err.GetMessageAsValue();
	//	const char* m1 = er;
	//	int ij = 0;
	//}

	//if (result.IsFailure())
	//	return 0;

	EntityList_ACIS* pEntityList = new EntityList_ACIS();

	pEntityList->pAcisEntities = pAcisEntities;

	//离散化面
	facet_entitylist(*pAcisEntities);

	//获取面
	get_FaceList_from_EntityList(pAcisEntities, pFaceEntities);

	//获取边
	get_EdgeList_from_EntityList(pAcisEntities, pEdgeEntities);

	//获取顶点
	get_VertexList_from_EntityList(pAcisEntities, pVertexEntities);

	pEntityList->faceData.resize(pFaceEntities->count());
	// 调用处理多个面的函数
	//static void get_triangles_from_faceted_faces(ENTITY_LIST & faces, std::vector<SVNMesh::FaceMesh>&faceData, std::vector<float>&coords, std::vector<int>&triangles, std::vector<float>&normalCoords) {
	get_triangles_from_faceted_faces(*pFaceEntities, pEntityList->faceData, pEntityList->coords, pEntityList->triangles, pEntityList->normalCoords);


	// SaveObjFile("D:\\1.obj",pEntityList);

	if (pSceneGraph->NbObject() >= 1)
	{

		CObject3d* pSurface = (CObject3d*)pSceneGraph->GetAt(0);
		pEntityList->SetTransform(*pSurface->GetTransform());

	}
	else
	{
		Vector3D m_lower, m_upper;

		pEntityList->ComputeBoundingBox(m_lower, m_upper);
		//get_EntityList_box(pEntityList, min_pt, max_pt);
		//设置包围盒
		CTransform transform;
		transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

		pEntityList->SetTransform(transform);
	}

	pEntityList->filename = filename;
	pEntityList->fullfilepath = filename;

	pSceneGraph->Add(pEntityList);

	//add childs of faces
	AddFaceChildren_ACIS(pFaceEntities, pEntityList, pSceneGraph);

	//add childs of edges
	AddEdgeChildren_ACIS(pEdgeEntities, pEntityList, pSceneGraph);

	//add childs of vertexs
	AddVertexChildren_ACIS(pVertexEntities, pEntityList, pSceneGraph);

	//debug
	/*stitch_input_handle* sih;
	mt_stitch_preview_container* cntnr;
	api_stitch_make_input(*pFaceEntities, sih);
	api_stitch_preview(sih, cntnr);
	pEntityList->cntnr = cntnr;

	int size = cntnr->size();
	double dis;
	EDGE* edge0, * edge1;
	double maxdis = -1;
	int maxindex;
	for (int i = 0; i < size; i++)
	{
		cntnr->get_edge_pair(i, edge0, edge1);
		dis = cntnr->get_hausdorff_distance(i);

		if (dis > maxdis)
		{
			maxdis = dis;
			maxindex = i;
		}
	}*/

	return 1;
}


int CParseSTEP::Run_Acis(ENTITY_LIST* pAcisEntities, CSceneGraph3d* pSceneGraph)
{
	ENTITY_LIST* pFaceEntities = new ENTITY_LIST();

	ENTITY_LIST* pEdgeEntities = new ENTITY_LIST();

	ENTITY_LIST* pVertexEntities = new ENTITY_LIST();

	EntityList_ACIS* pEntityList = new EntityList_ACIS();

	pEntityList->pAcisEntities = pAcisEntities;

	//离散化面
	facet_entitylist(*pAcisEntities);

	//获取面
	get_FaceList_from_EntityList(pAcisEntities, pFaceEntities);

	//获取边
	get_EdgeList_from_EntityList(pAcisEntities, pEdgeEntities);

	//获取顶点
	get_VertexList_from_EntityList(pAcisEntities, pVertexEntities);

	pEntityList->faceData.resize(pFaceEntities->count());
	// 调用处理多个面的函数
	//static void get_triangles_from_faceted_faces(ENTITY_LIST & faces, std::vector<SVNMesh::FaceMesh>&faceData, std::vector<float>&coords, std::vector<int>&triangles, std::vector<float>&normalCoords) {
	get_triangles_from_faceted_faces(*pFaceEntities, pEntityList->faceData, pEntityList->coords, pEntityList->triangles, pEntityList->normalCoords);


	// SaveObjFile("D:\\1.obj",pEntityList);

	if (pSceneGraph->NbObject() >= 1)
	{

		CObject3d* pSurface = (CObject3d*)pSceneGraph->GetAt(0);
		pEntityList->SetTransform(*pSurface->GetTransform());

	}
	else
	{
		Vector3D m_lower, m_upper;

		pEntityList->ComputeBoundingBox(m_lower, m_upper);
		//get_EntityList_box(pEntityList, min_pt, max_pt);
		//设置包围盒
		CTransform transform;
		transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

		pEntityList->SetTransform(transform);
	}

	pEntityList->filename = "Acis Model";
	pSceneGraph->Add(pEntityList);

	//add childs of faces
	AddFaceChildren_ACIS(pFaceEntities, pEntityList, pSceneGraph);

	//add childs of edges
	AddEdgeChildren_ACIS(pEdgeEntities, pEntityList, pSceneGraph);

	//add childs of vertices
	AddVertexChildren_ACIS(pVertexEntities, pEntityList, pSceneGraph);

	////debug
	//stitch_input_handle* sih;
	//mt_stitch_preview_container* cntnr;
	//api_stitch_make_input(*pAcisEntities, sih);
	//api_stitch_preview(sih, cntnr);
	//pEntityList->cntnr = cntnr;

	//int size = cntnr->size();
	//double dis;
	//EDGE* edge0, * edge1;
	//double maxdis = -1;
	//int maxindex;
	//for (int i = 0; i < size; i++)
	//{
	//	cntnr->get_edge_pair(i, edge0, edge1);
	//	dis = cntnr->get_hausdorff_distance(i);

	//	if (dis > maxdis)
	//	{
	//		maxdis = dis;
	//		maxindex = i;
	//	}
	//}

	return 1;
}

void CParseSTEP::AddFaceChildren_ACIS(ENTITY_LIST* pFaceEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph)
{
	char name[200];
	int faceindex = 0;
	pEntityList->arrayACISFaces.clear();
	pEntityList->m_FaceIndexMaps.clear();

	for (auto entity : *pFaceEntities)
	{
		Entity_FACE_ACIS* pEntity = new Entity_FACE_ACIS();
		pEntity->pParentObject = pEntityList;
		pEntity->pAcisEntity = (FACE*)entity;
		pSceneGraph->Add(pEntity); //先加入面
		pEntity->m_FaceIndex = faceindex;

		//add faces
		pEntityList->arrayACISFaces.push_back((FACE*)entity);
		pEntityList->m_FaceIndexMaps.insert(std::make_pair((FACE*)entity, faceindex));

		get_triangles_from_faceted_face((FACE*)entity, pEntity->coords, pEntity->triangles, pEntity->normalCoords, pEntity->edgepoints);
		pEntity->SetTransform(*pEntityList->GetTransform());
		sprintf(name, "Face%d", faceindex++);
		pEntity->filename = name;
		//pSceneGraph->Add(pEntity);
	}
}
void CParseSTEP::AddVertexChildren_ACIS(ENTITY_LIST* pVertexEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph)
{
	char name[200];
	int vertexindex = 0;
	pEntityList->arrayACISVertices.clear();
	pEntityList->m_VertexIndexMaps.clear();
	for (auto vertexEntity : *pVertexEntities)
	{
		Entity_VERTEX_ACIS* pVertexEntity = new Entity_VERTEX_ACIS();

		//get_tessellationpoints_from_Edges((EDGE*)edgeEntity, pEdgeEntity->edgepoints);
		pVertexEntity->pAcisEntity = (VERTEX*)vertexEntity;
		pVertexEntity->pParentObject = pEntityList;
		pVertexEntity->m_VertexIndex = vertexindex;

		//add edges
		pEntityList->arrayACISVertices.push_back((VERTEX*)vertexEntity);
		pEntityList->m_VertexIndexMaps.insert(std::make_pair((VERTEX*)vertexEntity, vertexindex));

		sprintf(name, "Vertex%d", vertexindex++);
		pVertexEntity->filename = name;
		pVertexEntity->SetTransform(*pEntityList->GetTransform());
		pSceneGraph->Add(pVertexEntity);
	}
}
void CParseSTEP::AddEdgeChildren_ACIS(ENTITY_LIST* pEdgeEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph)
{
	char name[200];
	int edgeindex = 0;
	pEntityList->arrayACISEdges.clear();
	pEntityList->m_EdgeIndexMaps.clear();
	for (auto edgeEntity : *pEdgeEntities)
	{
		Entity_EDGE_ACIS* pEdgeEntity = new Entity_EDGE_ACIS();

		get_tessellationpoints_from_Edges((EDGE*)edgeEntity, pEdgeEntity->edgepoints);
		pEdgeEntity->pAcisEntity = (EDGE*)edgeEntity;
		pEdgeEntity->pParentObject = pEntityList;
		pEdgeEntity->m_EdgeIndex = edgeindex;

		//add edges
		pEntityList->arrayACISEdges.push_back((EDGE*)edgeEntity);
		pEntityList->m_EdgeIndexMaps.insert(std::make_pair((EDGE*)edgeEntity, edgeindex));

		sprintf(name, "Edge%d", edgeindex++);
		pEdgeEntity->filename = name;
		pEdgeEntity->SetTransform(*pEntityList->GetTransform());
		pSceneGraph->Add(pEdgeEntity);
	}
}

//OCC三角化形状
void CParseSTEP::TriangulateShape(Shape_OCC* pshape)
{
	//BRepMesh_IncrementalMesh mesh(shape, 0.1); 
	//TopLoc_Location location;

	//	// 使用 TopExp_Explorer 遍历形状中的面
	//	TopExp_Explorer explorer(shape, TopAbs_FACE);
	//int count = 0;
	//for (; explorer.More(); explorer.Next()) {
	//	TopoDS_Face face = TopoDS::Face(explorer.Current());
	//	//sewer.Add(face);
	//	//if (++count == 2) break; // 仅添加前两个面进行示例
	//	Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);
	//	const TColgp_Array1OfPnt& nodes = triangulation->Nodes();
	//	const Poly_Array1OfTriangle& triangles = triangulation->Triangles();
	//	// Iterate over the triangles and their nodes.
	//	for (int i = triangles.Lower(); i <= triangles.Upper(); i++)
	//	{
	//		const Poly_Triangle& triangle = triangles(i);
	//		const gp_Pnt& p1 = nodes(triangle(1));
	//		const gp_Pnt& p2 = nodes(triangle(2));
	//		const gp_Pnt& p3 = nodes(triangle(3));
	//		// Do something with the current triangle's nodes
	//	}
	//}

	TopLoc_Location location;
	TopExp_Explorer explorer(pshape->shape, TopAbs_FACE);
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(explorer.Current());
		BRepMesh_IncrementalMesh mesh(face, 1);
		Handle_Poly_Triangulation triangulation = BRep_Tool::Triangulation(face, location);

		if (!triangulation.IsNull())
		{
			triangulation->AddNormals();
			triangulation->ComputeNormals();
			// 获取三角形的数量
			Standard_Integer numTriangles = triangulation->NbTriangles();
			Standard_Integer numNodes = triangulation->NbNodes();
			int nodenum = pshape->coords.size() / 3;

			for (Standard_Integer i = 1; i <= numNodes; ++i)
			{
				const gp_Pnt& p = triangulation->Node(i);
				if (location.IsIdentity())
				{
					pshape->coords.push_back(p.X());
					pshape->coords.push_back(p.Y());
					pshape->coords.push_back(p.Z());
				}
				else
				{
					p.Transformed(location);
					pshape->coords.push_back(p.X());
					pshape->coords.push_back(p.Y());
					pshape->coords.push_back(p.Z());
				}


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
		else
		{
			std::cerr << "Face does not have triangulation data." << std::endl;
		}
	}


}
void CParseSTEP::AddFaceChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	//这种遍历方式没有重复的元素
	TopTools_IndexedMapOfShape aFaceMap;
	TopExp::MapShapes(pshape->shape, TopAbs_FACE, aFaceMap);
	char message[200];
	pshape->arrayOCCFaces.clear();
	for (int i = 1; i <= aFaceMap.Extent(); ++i)
	{
		const TopoDS_Face& face = TopoDS::Face(aFaceMap(i));
		//BRepAdaptor_Surface adaptorSurface(curFace);
		//GeomAbs_SurfaceType surfaceType = adaptorSurface.GetType();

		Shape_FACE_OCC* pFaceOCC = new Shape_FACE_OCC();
		pFaceOCC->m_FaceIndex = i;
		pFaceOCC->shape = face;
		pFaceOCC->pParentObject = pshape;

		//add the face
		pshape->arrayOCCFaces.push_back(face);
		pshape->m_FaceIndexMaps.insert(std::make_pair(face.TShape(), i));

		get_triangles_from_faceted_face_occ(face, pFaceOCC->coords, pFaceOCC->triangles, pFaceOCC->normalCoords, pFaceOCC->edgepoints);
		pFaceOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Face%d", pFaceOCC->m_FaceIndex);
		pFaceOCC->filename = message;
		pSceneGraph->Add(pFaceOCC);
	}
}
void CParseSTEP::AddFaceChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	//这种遍历方式可能会出现重复的元素
	TopExp_Explorer explorer(pshape->shape, TopAbs_FACE);
	int index = 0;
	char message[200];
	pshape->arrayOCCFaces.clear();
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Face face = TopoDS::Face(explorer.Current());
		Shape_FACE_OCC* pFaceOCC = new Shape_FACE_OCC();
		pFaceOCC->m_FaceIndex = index;
		pFaceOCC->shape = face;
		pFaceOCC->pParentObject = pshape;

		//add the face
		pshape->arrayOCCFaces.push_back(face);
		pshape->m_FaceIndexMaps.insert(std::make_pair(face.TShape(), index));

		get_triangles_from_faceted_face_occ(face, pFaceOCC->coords, pFaceOCC->triangles, pFaceOCC->normalCoords, pFaceOCC->edgepoints);
		pFaceOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Face%d", pFaceOCC->m_FaceIndex);
		pFaceOCC->filename = message;
		pSceneGraph->Add(pFaceOCC);
		index++;
	}
}
void CParseSTEP::AddEdgeChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	TopTools_IndexedMapOfShape aEdgeMap;
	TopExp::MapShapes(pshape->shape, TopAbs_EDGE, aEdgeMap);
	char message[200];
	for (int i = 1; i <= aEdgeMap.Extent(); ++i)
	{
		const TopoDS_Edge& aEdge = TopoDS::Edge(aEdgeMap(i));

		Shape_EDGE_OCC* pEdgeOCC = new Shape_EDGE_OCC();
		pEdgeOCC->m_EdgeIndex = i;
		pEdgeOCC->shape = aEdge;
		pEdgeOCC->pParentObject = pshape;

		//add the edge
		pshape->arrayOCCEdges.push_back(aEdge);
		pshape->m_EdgeIndexMaps.insert(std::make_pair(aEdge.TShape(), i));

		pshape->get_tessellationpoints_from_edges_occ(aEdge, pEdgeOCC->edgepoints);
		pEdgeOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Edge%d", pEdgeOCC->m_EdgeIndex);
		pEdgeOCC->filename = message;
		pSceneGraph->Add(pEdgeOCC);
	}
}
void CParseSTEP::AddEdgeChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	char message[200];
	TopExp_Explorer explorer1(pshape->shape, TopAbs_EDGE);
	int index = 0;
	int i;
	for (; explorer1.More(); explorer1.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer1.Current());

		i = 0;
		for (i = 0; i < pshape->arrayOCCEdges.size(); i++)
		{
			if (edge.IsSame(pshape->arrayOCCEdges[i]))
				break;
		}

		if (i < pshape->arrayOCCEdges.size())
			continue;

		Shape_EDGE_OCC* pEdgeOCC = new Shape_EDGE_OCC();
		pEdgeOCC->m_EdgeIndex = index++;
		pEdgeOCC->shape = edge;
		pEdgeOCC->pParentObject = pshape;

		//add the edge
		pshape->arrayOCCEdges.push_back(edge);

		pshape->get_tessellationpoints_from_edges_occ(edge, pEdgeOCC->edgepoints);
		pEdgeOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Edge%d", pEdgeOCC->m_EdgeIndex);
		pEdgeOCC->filename = message;
		pSceneGraph->Add(pEdgeOCC);
	}

	//add the map
	for (int i = 0; i < pshape->arrayOCCEdges.size(); i++)
	{
		pshape->m_EdgeIndexMaps.insert(std::make_pair(pshape->arrayOCCEdges[i].TShape(), i));
	}
}

void CParseSTEP::AddVertexChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	TopTools_IndexedMapOfShape aVertexMap;
	TopExp::MapShapes(pshape->shape, TopAbs_VERTEX, aVertexMap);
	char message[200];

	for (int i = 1; i <= aVertexMap.Extent(); ++i)
	{
		const TopoDS_Vertex& aVertex = TopoDS::Vertex(aVertexMap(i));
		Shape_VERTEX_OCC* pVertexOCC = new Shape_VERTEX_OCC();

		pVertexOCC->m_VertexIndex = i;
		pVertexOCC->shape = aVertex;
		pVertexOCC->pParentObject = pshape;

		//add the edge
		pshape->arrayOCCVertices.push_back(aVertex);
		pshape->m_VertexIndexMaps.insert(std::make_pair(aVertex.TShape(), i));

		//get_tessellationpoints_from_edges_occ(pshape, edge, pEdgeOCC->edgepoints);
		pVertexOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Vertex%d", pVertexOCC->m_VertexIndex);
		pVertexOCC->filename = message;
		pSceneGraph->Add(pVertexOCC);
	}
}
void CParseSTEP::AddVertexChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph)
{
	char message[200];
	TopExp_Explorer explorer1(pshape->shape, TopAbs_VERTEX);
	int index = 0;
	int i;
	for (; explorer1.More(); explorer1.Next())
	{
		TopoDS_Vertex vertex = TopoDS::Vertex(explorer1.Current());
		i = 0;
		for (i = 0; i < pshape->arrayOCCVertices.size(); i++)
		{
			if (vertex.IsSame(pshape->arrayOCCVertices[i]))
				break;
		}

		if (i < pshape->arrayOCCVertices.size())
			continue;

		Shape_VERTEX_OCC* pVertexOCC = new Shape_VERTEX_OCC();
		pVertexOCC->m_VertexIndex = index++;
		pVertexOCC->shape = vertex;
		pVertexOCC->pParentObject = pshape;

		//add the edge
		pshape->arrayOCCVertices.push_back(vertex);

		//get_tessellationpoints_from_edges_occ(pshape, edge, pEdgeOCC->edgepoints);
		pVertexOCC->SetTransform(*pshape->GetTransform());
		sprintf(message, "Vertex%d", pVertexOCC->m_VertexIndex);
		pVertexOCC->filename = message;
		pSceneGraph->Add(pVertexOCC);
	}

	//add the map
	for (int i = 0; i < pshape->arrayOCCVertices.size(); i++)
	{
		pshape->m_VertexIndexMaps.insert(std::make_pair(pshape->arrayOCCVertices[i].TShape(), i));
	}
}
//运行选择OCC引擎读取step文件
int CParseSTEP::Run_OCC_STEP(char* filename, CSceneGraph3d* pSceneGraph)
{
	// 创建 STEP 文件读取器
	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(filename);

	if (status != IFSelect_ReturnStatus::IFSelect_RetDone)
	{
		std::cerr << "Error reading STEP file." << std::endl;
		//AfxMessageBox("Error reading STEP file.");
		return 0;
	}
	// 传输读取的数据
	reader.TransferRoots();
	TopoDS_Shape shape = reader.OneShape();

	// 计算包围盒
	/*Bnd_Box boundingBox = ComputeBoundingBox(shape);*/


	Shape_OCC* pshape = new Shape_OCC();
	pshape->shape = shape;

	double tolerance = 0.5;
	//if has face and has no shell and above topology, sew it. 
	//if (!pshape->ChechWhetherExistTopology())
	//{
	//	BRepBuilderAPI_Sewing sewing(tolerance);
	//	sewing.Add(pshape->shape);
	//	sewing.Perform();
	//	TopoDS_Shape sewedShape = sewing.SewedShape();
	//	pshape->shape = sewedShape;
	//}

	// 三角化形状
	TriangulateShape(pshape);
	// 
	// 调用处理多个面的函数


	//设置transform

	Vector3D m_lower, m_upper;
	pshape->ComputeBoundingBox(m_lower, m_upper);
	//get_EntityList_box(pEntityList, min_pt, max_pt);
	//设置包围盒
	CTransform transform;

	transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

	if (pSceneGraph->NbObject() >= 1)
	{
		CObject3d* pObject = (CObject3d*)pSceneGraph->GetAt(0);
		pshape->SetTransform(*pObject->GetTransform());
	}
	else

		pshape->SetTransform(transform);

	pshape->filename = "OCC_Model";
	pshape->fullfilepath = filename;
	pSceneGraph->Add(pshape);

	//add face children
	AddFaceChildren_OCC_NoDuplicate(pshape, pSceneGraph);

	//add edge children
	AddEdgeChildren_OCC_NoDuplicate(pshape, pSceneGraph);

	//add vertex children
	AddVertexChildren_OCC_NoDuplicate(pshape, pSceneGraph);
	return 1;
}

int CParseSTEP::Run_OCC_BREP(char* filename, CSceneGraph3d* pSceneGraph)
{
	TopoDS_Shape shape;
	BRep_Builder b;
	std::ifstream is;
	is.open(filename);
	BRepTools::Read(shape, is, b);
	is.close();

	//// 创建 STEP 文件读取器
	///BREPControl_Reader reader;
	//IFSelect_ReturnStatus status = reader.ReadFile(filename);

	//if (status != IFSelect_ReturnStatus::IFSelect_RetDone)
	//{
	//	std::cerr << "Error reading STEP file." << std::endl;
	//	return 1;
	//}
	//// 传输读取的数据
	//reader.TransferRoots();
	//TopoDS_Shape shape = reader.OneShape();

	// 计算包围盒
	/*Bnd_Box boundingBox = ComputeBoundingBox(shape);*/


	Shape_OCC* pshape = new Shape_OCC();
	pshape->shape = shape;
	double tolerance = 0.5;
	//if has face and has no shell and above topology, sew it. 
	if (!pshape->ChechWhetherExistTopology())
	{
		BRepBuilderAPI_Sewing sewing(tolerance);
		sewing.Add(pshape->shape);
		sewing.Perform();
		TopoDS_Shape sewedShape = sewing.SewedShape();
		pshape->shape = sewedShape;
	}

	// 三角化形状
	TriangulateShape(pshape);
	// 
	// 调用处理多个面的函数


	//设置transform

	Vector3D m_lower, m_upper;
	pshape->ComputeBoundingBox(m_lower, m_upper);
	//get_EntityList_box(pEntityList, min_pt, max_pt);
	//设置包围盒
	CTransform transform;
	transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

	if (pSceneGraph->NbObject() >= 1)
	{
		CObject3d* pSurface = (CObject3d*)pSceneGraph->GetAt(0);
		pshape->SetTransform(*pSurface->GetTransform());
	}
	else
		pshape->SetTransform(transform);

	pshape->filename = "OCC_Model";

	pSceneGraph->Add(pshape);

	//add face children
	AddFaceChildren_OCC(pshape, pSceneGraph);

	//add edge children
	AddEdgeChildren_OCC(pshape, pSceneGraph);

	//add Vertex children
	AddVertexChildren_OCC(pshape, pSceneGraph);

	return 1;
}

void facet_entitylist(ENTITY_LIST& entitylist)
{
	for (auto entity : entitylist)
	{
		api_facet_entity(entity);
	}
}

