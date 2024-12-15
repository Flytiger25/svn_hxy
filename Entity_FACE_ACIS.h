#pragma once
#include "lib3d/Object3d.h"
#include "SVNMesh.h"
#include <position.hxx>
#include "Lib3D/Color.h"
class FACE;
class EntityList_ACIS;
class Entity_FACE_ACIS :public CObject3d
{
public:
	//color
	CColor color;
	Entity_FACE_ACIS();
	~Entity_FACE_ACIS();
	//
	int m_FaceIndex;

	FACE* pAcisEntity;

	int			   m_showControlPointsNetwork;
	int            m_showIsoParameterCurves;
	int            m_showSurface;

	std::vector<float> coords;
	std::vector<int> triangles;
	std::vector<float> normalCoords;
	std::vector<std::vector<SPAposition>> edgepoints;

	std::vector<SVNMesh::FaceMesh> faceData;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	void BuildTriangulationList();

	void glRenderTriangulation();

	//
	void glShowControlPointNetworks();

	//
	void glShowIsoParameterCurves();

	void glRenderEdges();

	virtual int glDraw();

	virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//pick test
	virtual int PickTest(PickStruct& pickstruct);
};

