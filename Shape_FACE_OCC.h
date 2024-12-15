#pragma once
#include "lib3d/Object3d.h"
#include "Point3D.h"
#include "position.hxx"
#include <TopoDS_Shape.hxx>
#include <vector.hxx>
#include <vector>
#include "lib3d/Array3d.h"
#include <Point3D.h>
#include <CParseSTEP.h>
class Shape_FACE_OCC :
	public CObject3d
{
public:
	Shape_FACE_OCC();
	~Shape_FACE_OCC();

	//color
	CColor color;

	TopoDS_Shape  shape;

	int			   m_showControlPointsNetwork;
	int            m_showIsoParameterCurves;
	int            m_showSurface;
	int			   m_showVertexIndex;

	int           m_FaceIndex;

	std::vector<float> coords;
	std::vector<int> triangles;
	std::vector<float> normalCoords;
	std::vector<std::vector<Point3D>> edgepoints;

	std::vector<SVNMesh::FaceMesh> faceData;

	//
	double GetFaceArea();

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

	//
	void glRenderEdges_Selected();

	virtual int glDraw();

	virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	//pick test
	virtual int PickTest(PickStruct& pickstruct);

	void glDrawVertexIndex();
};

