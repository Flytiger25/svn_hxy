#pragma once
#include "lib3d/Object3d.h"
#include <SVNMesh.h>
#include <TopoDS_Shape.hxx>
#include <vector.hxx>
#include <vector>
#include "lib3d/Array3d.h"
#include <Point3D.h>
#include <CParseSTEP.h>
#include "TopoDS_Edge.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Vertex.hxx"
#include "Geom_BSplineCurve.hxx"
#include "gp_Pnt.hxx"
#include <map>
class TopoDS_Shape;
class CSceneGraph3d;

class Shape_OCC : public CObject3d
{
private:
	//color
	CColor color;
	CArray3d<CObject3d> m_ArrayObject3d;
public:


	Shape_OCC();
	~Shape_OCC();

	int		  m_showControlPointsNetwork;
	int       m_showIsoParameterCurves;
	int       m_showSurface;
	int		  m_showEdge;
	int		  m_showVertex;
	int       m_showGapEdge;
	int       m_showFaceLackBoundary;
	int       m_showFaceIndex;
	int       m_showEdgeIndex;
	int       m_showVertexIndex;
	int		  m_showSelectedEdge;
	int		  m_showFaceLackTessellationPoints;
	int       m_showFaceLackFittingPlane;
	int       m_showFaceLackProjectedPoints;

	int       theFaceLackBelowMinArea;
	int       theFaceLackBetweenMinAreaAndMaxArea;
	int       theFaceLackAboveMaxArea;

	TopoDS_Shape  shape;
	std::vector<TopoDS_Edge> arrayOCCEdges;
	std::vector<TopoDS_Face> arrayOCCFaces;
	std::vector<TopoDS_Vertex> arrayOCCVertices;
	std::map<Handle(TopoDS_TShape), int> m_EdgeIndexMaps;
	std::map<Handle(TopoDS_TShape), int> m_FaceIndexMaps;
	std::map<Handle(TopoDS_TShape), int> m_VertexIndexMaps;

	//
	std::map<int, int> m_EdgeIndexMaps_OCCTOACIS;

	//for debug
	std::vector<int> arraySelectedGapIndexArray;

	std::vector<float> coords;
	std::vector<int> triangles;
	std::vector<float> normalCoords;

	std::vector<std::vector<gp_Pnt>> edgepoints;

	std::vector<TopoDS_Edge> gapEdges;
	std::vector<double> gapEdgesHausdorffDistance;
	std::vector<std::vector<gp_Pnt>> gapEdgeEndandMiddlePoints;

	std::vector<std::vector<std::pair<TopoDS_Edge, TopoDS_Face>>> lackFaceBoundaries;
	std::vector<double> lackFaceBoundariesArea;

	//可以考虑 Brep_tool::polygon函数
	std::map<Handle(TopoDS_TShape), std::vector<gp_Pnt>> m_EdgeTessellationMaps;

	//for coons debug
	std::vector<Handle(Geom_BSplineCurve)> curveArray;
	std::vector<Handle(Geom_BSplineCurve)> curvederiveArray;

	std::vector<gp_Pnt> curveSectionArray1_Sampling;
	std::vector<gp_Pnt> curveSectionArray2_Sampling;

	//fitting plane
	gp_Pnt  theFittingPlanePoint;
	gp_XYZ	theFittingPlaneN, theFittingPlaneX, theFittingPlaneY;
	std::vector<gp_Pnt2d> theFittingplaneProjectedPoints1;
	std::vector<gp_Pnt2d> theFittingplaneProjectedPoints2;
	std::vector<gp_Pnt2d> theFittingplaneProjectedPoints3;
	//
	void SetTransform(CTransform& transform) { m_Transform.Copy(transform); }

	//
	CTransform* GetTransform(void) { return &m_Transform; }

	void BuildTriangulationList();

	//
	void glDrawEdges();

	//
	void glDrawProjectedPoints();

	//
	void glDrawFittingPlane();

	//
	void glDrawCurveSampling();

	//
	void glRenderTriangulation();

	virtual int glDraw();

	virtual void ComputeBoundingBox(Vector3D& lower, Vector3D& upper);

	virtual int PickTest(PickStruct& pickstruct);

	int NbObject(void) { return m_ArrayObject3d.GetSize(); }

	//
	void glDrawrGapEdges();

	//
	void glDrawrSelectedEdges();

	//
	void glDrawrFaceLackBoundaries();

	//
	void DrawCoordinateAxis();

	//
	BOOL ChechWhetherExistTopology();

	//
	void glDrawCurveandDerivatives();

	//
	int getOCCEdgeIndex(TopoDS_Edge edge);

	//
	int getOCCFaceIndex(TopoDS_Face face);

	//
	int getOCCVertexIndex(TopoDS_Vertex vertex);

	//
	void glDrawFaceIndex();

	//
	void glDrawEdgeIndex();

	//
	void glDrawVertexIndex();

	//
	void get_tessellationpoints_from_edges_occ(const TopoDS_Shape& edge, std::vector<gp_Pnt>& tessellationPoints);
};

void  getTessellationPoints(const TopoDS_Edge& edge, std::vector<gp_Pnt>& tessellationPoints);
