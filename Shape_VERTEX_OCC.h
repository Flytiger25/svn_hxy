#pragma once
#include <TopoDS_Shape.hxx>
#include "Point3D.h"
#include "lib3d/Object3d.h"
#include "Lib3D/Color.h"
class Shape_VERTEX_OCC :
	public CObject3d
{
public:
	//color
	CColor color;
	Shape_VERTEX_OCC();
	~Shape_VERTEX_OCC();

	//store the corresponding occ edge
	TopoDS_Shape  shape;

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

