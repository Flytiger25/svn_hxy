#pragma once
#include "lib3d/Object3d.h"
#include "SVNMesh.h"
#include "Lib3D/Color.h"
#include <map>
class ENTITY_LIST;
class mt_stitch_preview_container;
class EDGE;
class FACE;
class VERTEX;
class EntityList_ACIS : public CObject3d
{
public:

	CColor color;
	EntityList_ACIS();
	~EntityList_ACIS();

	ENTITY_LIST* pAcisEntities;

	int m_showSurface;

	//
	mt_stitch_preview_container* cntnr;

	std::vector<float> coords;
	std::vector<int> triangles;
	std::vector<float> normalCoords;

	//show options
	int       m_showFaceIndex;
	int       m_showEdgeIndex;
	int       m_showVertexIndex;
	int		  m_showSelectedEdge;
	int       m_showGapEdge;
	int       m_showEdge;
	int       m_showVertex;

	//
	std::vector<EDGE*> arrayACISEdges;
	std::vector<FACE*> arrayACISFaces;
	std::vector<VERTEX*> arrayACISVertices;
	std::map<EDGE*, int> m_EdgeIndexMaps;
	std::map<FACE*, int> m_FaceIndexMaps;
	std::map<VERTEX*, int> m_VertexIndexMaps;

	//for debug
	std::vector<int> arraySeletedEdgePairIndexAarray;

	std::vector<SVNMesh::FaceMesh> faceData;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	void BuildTriangulationList();

	void glRenderTriangulation();

	virtual int glDraw();

	virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//
	void glShowSelectedEdges();

	//
	void glShowHausdorffDistance();

	virtual int PickTest(PickStruct& pickstruct);

	//
	void glDrawFaceIndex();

	//
	void glDrawEdgeIndex();

	//
	void glDrawVertexIndex();
};
