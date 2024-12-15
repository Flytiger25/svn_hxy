#pragma once
#include <TopoDS_Shape.hxx>
#include "Point3D.h"
#include "lib3d/Object3d.h"
#include "Lib3D/Color.h"
class Shape_EDGE_OCC :
	public CObject3d
{
public:
	//color
	CColor color;
	Shape_EDGE_OCC();
	~Shape_EDGE_OCC();

	//store the corresponding occ edge
	TopoDS_Shape  shape;

	//edge index
	int m_EdgeIndex;

	int			   m_showControlPointsNetwork;

	//tessellation polygon for visualization
	std::vector<gp_Pnt> edgepoints;

	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	//void BuildTriangulationList();

	//void glRenderTriangulation();

	//void glShowControlPointNetworks();

	//void glShowIsoParameterCurves();

	void glRenderEdges();

	//
	void glRenderControlPoints();

	virtual int glDraw();

	//virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//pick test
	virtual int PickTest(PickStruct& pickstruct);
};

