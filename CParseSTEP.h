#pragma once
#include "EntityList_ACIS.h"
#include <gp_Pnt.hxx>
#include <TopoDS_Face.hxx>
#include <vector>
#include "position.hxx"
class CSceneGraph3d;
class ENTITY_LIST;
class Shape_OCC;
class EDGE;
class SPAIAcisDocument;
class CParseSTEP
{
	//Parse PSLG
public:

	//
	void TriangulateShape(Shape_OCC* pshape);

	int IsAcisEngine;

	//
	//void AddEdgeChildren_ACIS(ENTITY* pEntity, CSceneGraph3d* pSceneGraph);

	//
	int Run_STEP(char* filename, CSceneGraph3d* pSceneGraph);
	//
	int Run_OCC_STEP(char* filename, CSceneGraph3d* pSceneGraph);

	//
	int Run_OCC_BREP(char* filename, CSceneGraph3d* pSceneGraph);

	//
	int Run_Acis(ENTITY_LIST* pAcisEntities, CSceneGraph3d* pSceneGraph);

	//
	int Run_Acis(char* filename, CSceneGraph3d* pSceneGraph, const char* type = "STEP");

	int get_FaceList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pFaceEntities);

	void SaveObjFile(char* filename, EntityList_ACIS* pEntityList);

	//
	int get_EdgeList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pEdgeEntities);

	//
	void get_tessellationpoints_from_Edges(EDGE* edge, std::vector<SPAposition>& edgepoints);

	//
	void AddFaceChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph);

	//
	void AddFaceChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph);

	//
	void AddEdgeChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph);

	//
	void AddVertexChildren_OCC_NoDuplicate(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph);

	//
	void AddEdgeChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGrap);

	//
	void AddVertexChildren_OCC(Shape_OCC* pshape, CSceneGraph3d* pSceneGraph);

	//
	void AddFaceChildren_ACIS(ENTITY_LIST* pFaceEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph);

	//
	void AddEdgeChildren_ACIS(ENTITY_LIST* pEdgeEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph);

	//
	void AddVertexChildren_ACIS(ENTITY_LIST* pVertexEntities, EntityList_ACIS* pEntityList, CSceneGraph3d* pSceneGraph);

	//
	int get_VertexList_from_EntityList(ENTITY_LIST* pAcisEntities, ENTITY_LIST* pVertexEntities);
};

