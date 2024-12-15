#pragma once
#include "Lib3D/Object3d.h"
class VERTEX;
#include "Lib3D/Color.h"
class Entity_VERTEX_ACIS :
	public CObject3d
{
public:
	//color
	CColor color;
	Entity_VERTEX_ACIS();
	~Entity_VERTEX_ACIS();

	//store the corresponding occ edge
	VERTEX* pAcisEntity;

	//edge index
	int m_VertexIndex;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	//void BuildTriangulationList();

	//void glRenderTriangulation();

	//void glShowControlPointNetworks();

	//void glShowIsoParameterCurves();

	void glRenderVertex();

	//
	void glRenderControlPoints();

	virtual int glDraw();

	//virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//pick test
	virtual int PickTest(PickStruct& pickstruct);
};

