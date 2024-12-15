#pragma once
#include "lib3d/Object3d.h"
#include "SVNMesh.h"
#include <position.hxx>
#include "Lib3D/Color.h"
class EDGE;
class EntityList_ACIS;
class Entity_EDGE_ACIS : public CObject3d
{
public:
	//color
	CColor color;
	Entity_EDGE_ACIS();
	~Entity_EDGE_ACIS();

	//edge index
	int m_EdgeIndex;

	//store the corresponding occ edge
	EDGE* pAcisEntity;

	int m_showControlPointsNetwork;

	//tessellation polygon for visualization
	std::vector<SPAposition> edgepoints;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	void glRenderControlPoints();

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	//void BuildTriangulationList();

	//void glRenderTriangulation();

	//void glShowControlPointNetworks();

	//void glShowIsoParameterCurves();

	void glRenderEdges();

	virtual int glDraw();

	//virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//pick test
	virtual int PickTest(PickStruct& pickstruct);
};

