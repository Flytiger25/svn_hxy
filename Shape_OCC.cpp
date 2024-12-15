#include "stdafx.h"
#include "TShape_OCC.h"
#include "FormView3.h"
#include "EntityList_ACIS.h"
#include <lib3d/Mesh3d.h>
#include <lib3d/Array3d.h>
#include <vector>
#include <GL/gl.h>
#include <lib3d/Base3d.h>
#include <TopExp_Explorer.hxx>
#include <STEPControl_Reader.hxx>
#include <TShape_OCC.h>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRepBuilderAPI.hxx>
#include "BRepBndLib.hxx"
#include <GeometricEngineSelect.h>
#include <Poly_Triangulation.hxx>
#include <BRep_Tool.hxx>
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
#include <BRepBuilderAPI_MakePolygon.hxx>
#include "BRepAdaptor_Curve.hxx"

#include <GCPnts_UniformAbscissa.hxx>
#include "TopExp.hxx"
#include "TopoDS_Compound.hxx"
#include <TopoDS_Shell.hxx>
#include "BRepTools.hxx"
#include "BRep_TFace.hxx"

extern CFormView3* p_FormView3;
Shape_OCC::Shape_OCC()
{
	type = TYPE_BREP_OCC;
	m_showControlPointsNetwork = 0;
	m_showIsoParameterCurves = 0;
	m_showSurface = 1;
	color.Set(128, 128, 128);
	m_showEdge = 1;
	m_showGapEdge = 1;
	m_showFaceLackBoundary = 1;
	m_showFaceIndex = 0;
	m_showEdgeIndex = 0;
	m_showVertexIndex = 0;
	m_showSelectedEdge = 0;
	m_showVertex = 0;
	m_ShowCoordinateAxis = 0;
	m_showFaceLackTessellationPoints = 0;
	m_showFaceLackFittingPlane = 0;
	m_showFaceLackProjectedPoints = 0;
}
Shape_OCC::~Shape_OCC()

{

}
//********************************************
// glDraw
//********************************************

void  getTessellationPoints(const TopoDS_Edge& edge, std::vector<gp_Pnt>& tessellationPoints)
{
	//std::vector<gp_Pnt> tessellationPoints;

	// 适配边的曲线
	BRepAdaptor_Curve curve(edge);

	// 定义生成的镶嵌点的数量
	const Standard_Integer numPoints = 50;

	// 创建均匀分布点计算器
	GCPnts_UniformAbscissa uniformAbscissa(curve, numPoints);

	if (uniformAbscissa.IsDone())
	{
		// 获取生成的镶嵌点并添加到结果向量中
		for (Standard_Integer i = 1; i <= uniformAbscissa.NbPoints(); ++i)
		{
			gp_Pnt pnt = curve.Value(uniformAbscissa.Parameter(i));
			tessellationPoints.push_back(pnt);
		}
	}
	else
	{
		std::cerr << "Error in tessellation point generation." << std::endl;
	}
}
BOOL Shape_OCC::ChechWhetherExistTopology()
{
	TopAbs_ShapeEnum type = shape.ShapeType();
	BOOL hasFace = 0;
	BOOL hasTopologyAboveFace = 0;
	int facenum = 0;
	int shellnum = 0;
	if (type == TopAbs_COMPOUND)
	{
		TopoDS_Compound comp = TopoDS::Compound(shape);

		TopExp_Explorer explorerF(comp, TopAbs_FACE);
		if (explorerF.More())
			hasFace = 1;
		for (; explorerF.More(); explorerF.Next())
			facenum++;

		TopExp_Explorer explorerSOLID(comp, TopAbs_SOLID);
		if (explorerSOLID.More())
			hasTopologyAboveFace = 1;

		TopExp_Explorer explorerSHELL(comp, TopAbs_SHELL);
		////if (explorerSHELL.More())
		//	hasTopologyAboveFace = 1;
		for (; explorerSHELL.More(); explorerSHELL.Next())
		{
			shellnum++;
		}

		if (shellnum < facenum && shellnum > 0)
			hasTopologyAboveFace = 1;

		TopExp_Explorer explorerCOMPSOLID(comp, TopAbs_COMPSOLID);
		if (explorerCOMPSOLID.More())
			hasTopologyAboveFace = 1;

		if (hasFace && !hasTopologyAboveFace)
			return FALSE;
	}

	return TRUE;

}

void Shape_OCC::glDrawrFaceLackBoundaries()
{
	glDisable(GL_LIGHTING);
	coords.clear(); // 清空 coords 数组
	//显示edge
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, -2.0);

	std::vector<float> coords;
	std::vector<gp_Pnt> tessellationPoints;
	glColor3f(0, 0, 1);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10);
	for (int i = 0; i < lackFaceBoundaries.size(); i++)
	{
		for (int j = 0; j < lackFaceBoundaries[i].size(); j++)
		{
			TopLoc_Location location;

			// 如果没有现有的镶嵌数据，创建一个新的多边形表示

			// 函数getTessellationPoints(edge)来获取边的镶嵌点
			std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
			auto iterator = m_EdgeTessellationMaps.find(lackFaceBoundaries[i][j].first.TShape());
			if (iterator != m_EdgeTessellationMaps.end())
			{
				ptessellationPoints = &iterator->second;
			}
			else
			{
				getTessellationPoints(lackFaceBoundaries[i][j].first, tessellationPoints);
				m_EdgeTessellationMaps.insert(std::make_pair(lackFaceBoundaries[i][j].first.TShape(), tessellationPoints));
			}
			//std::vector<gp_Pnt> tessellationPoints = getTessellationPoints(gapEdges[i]);

			//Handle_Poly_Polygon3D polygon = BRep_Tool::Polygon3D(gapEdges[i], location);

			//add the tessellation of edges
			//获取gapEdges中的当前边的多边形（tessellation）

			// 添加边的镶嵌（tessellation）功能的代码
			//if (polygon.IsNull()) {




			//coords.clear();

			//if (!polygon.IsNull())
			{

				// 获取顶点的数量
				//Standard_Integer numNodes = polygon->NbNodes();
				//int nodenum = pshape->coords.size() / 3;
				//const TColgp_Array1OfPnt& nodes = polygon->Nodes();
				if ((*ptessellationPoints).size() > 0)
				{
					::glBegin(GL_LINES);
					// 循环绘制边的线段
					for (Standard_Integer j = 0; j < (*ptessellationPoints).size() - 1; ++j)
					{
						const gp_Pnt& p1 = (*ptessellationPoints)[j];
						const gp_Pnt& p2 = (*ptessellationPoints)[j + 1];
						::glVertex3f(p1.X(), p1.Y(), p1.Z());
						::glVertex3f(p2.X(), p2.Y(), p2.Z());
					}

					::glEnd();

				}

				//显示顶点
				TopoDS_Vertex v1, v2;
				TopExp::Vertices(lackFaceBoundaries[i][j].first, v1, v2);

				gp_Pnt p1 = BRep_Tool::Pnt(v1);
				gp_Pnt p2 = BRep_Tool::Pnt(v2);
				::glPointSize(10);
				::glBegin(GL_POINTS);
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
				::glEnd();
			}
		}

	}
}

void Shape_OCC::glDrawEdges()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, -2.0);
	coords.clear(); // 清空 coords 数组

	TopExp_Explorer explorer(shape, TopAbs_EDGE);
	std::vector<float> coords;
	glColor3f(0, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2);
	std::vector<gp_Pnt> tessellationPoints;
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		TopLoc_Location location;

		tessellationPoints.clear();

		std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
		auto iterator = m_EdgeTessellationMaps.find(edge.TShape());
		if (iterator != m_EdgeTessellationMaps.end())
		{
			ptessellationPoints = &iterator->second;
		}
		else
		{
			getTessellationPoints(edge, tessellationPoints);
			m_EdgeTessellationMaps.insert(std::make_pair(edge.TShape(), tessellationPoints));
		}

		if ((*ptessellationPoints).size() > 0)
		{
			::glBegin(GL_LINES);
			for (Standard_Integer i = 0; i < ptessellationPoints->size() - 1; ++i)
			{
				const gp_Pnt& p1 = (*ptessellationPoints)[i];
				const gp_Pnt& p2 = (*ptessellationPoints)[i + 1];
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
			}
			::glEnd();
		}

	}
}
void Shape_OCC::glDrawFaceIndex()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayOCCFaces.size();
	gp_Pnt p1;
	for (int i = 0; i < size; i++)
	{
		TopoDS_Face face = TopoDS::Face(arrayOCCFaces[i]);
		double umin, umax, vmin, vmax;
		BRepTools::UVBounds(face, umin, umax, vmin, vmax);

		const BRep_TFace* TE = static_cast<const BRep_TFace*>(face.TShape().get());
		Handle(Geom_Surface) surf = TE->Surface();

		surf->D0((umin + umax) / 2, (vmin + vmax) / 2, p1);

		sprintf(message, "%d", i + 1);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}
	glEnable(GL_DEPTH_TEST);
}
void Shape_OCC::glDrawEdgeIndex()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayOCCEdges.size();
	gp_Pnt p1;
	TopLoc_Location loc;
	Standard_Real first, last;
	for (int i = 0; i < size; i++)
	{
		TopoDS_Edge edge = TopoDS::Edge(arrayOCCEdges[i]);

		Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);

		gcurve->D0((first + last) / 2, p1);

		sprintf(message, "%d", i + 1);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
void Shape_OCC::glDrawVertexIndex()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayOCCVertices.size();
	gp_Pnt p1;
	TopLoc_Location loc;
	Standard_Real first, last;
	for (int i = 0; i < size; i++)
	{
		TopoDS_Vertex theVertex = TopoDS::Vertex(arrayOCCVertices[i]);
		p1 = BRep_Tool::Pnt(theVertex);

		sprintf(message, "%d", i + 1);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Shape_OCC::glDrawProjectedPoints()
{
	char message[100];
	glDisable(GL_LIGHTING);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glPointSize(10);

	int size = theFittingplaneProjectedPoints1.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt2d p1 = theFittingplaneProjectedPoints1[i];
		gp_Pnt point = theFittingPlanePoint.XYZ() + theFittingPlaneX * p1.X() + theFittingPlaneY * p1.Y();
		::glBegin(GL_POINTS);
		::glVertex3f(point.X(), point.Y(), point.Z());
		::glEnd();
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(point.X(), point.Y(), point.Z(), message);
	}

	size = theFittingplaneProjectedPoints2.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt2d p1 = theFittingplaneProjectedPoints2[i];
		gp_Pnt point = theFittingPlanePoint.XYZ() + theFittingPlaneX * p1.X() + theFittingPlaneY * p1.Y();
		::glBegin(GL_POINTS);
		::glVertex3f(point.X(), point.Y(), point.Z());
		::glEnd();
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(point.X(), point.Y(), point.Z(), message);
	}

	size = theFittingplaneProjectedPoints3.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt2d p1 = theFittingplaneProjectedPoints3[i];
		gp_Pnt point = theFittingPlanePoint.XYZ() + theFittingPlaneX * p1.X() + theFittingPlaneY * p1.Y();
		::glBegin(GL_POINTS);
		::glVertex3f(point.X(), point.Y(), point.Z());
		::glEnd();
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(point.X(), point.Y(), point.Z(), message);
	}
}
void Shape_OCC::glDrawFittingPlane()
{
	char message[100];
	glDisable(GL_LIGHTING);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glPointSize(10);

	int size = theFittingplaneProjectedPoints1.size();
	double minx = MAX_DOUBLE;
	double miny = MAX_DOUBLE;
	double maxx = -MAX_DOUBLE;
	double maxy = -MAX_DOUBLE;

	for (int i = 0; i < size; i++)
	{
		gp_Pnt2d p1 = theFittingplaneProjectedPoints1[i];

		minx = min(minx, p1.X());
		miny = min(miny, p1.Y());

		maxx = max(maxx, p1.X());
		maxy = max(maxy, p1.Y());
	}
	gp_Pnt point;
	double centerx = (minx + maxx) / 2;
	double centery = (miny + maxy) / 2;

	minx = minx * 2 - centerx;
	maxx = maxx * 2 - centerx;
	miny = miny * 2 - centery;
	maxy = maxy * 2 - centery;

	::glBegin(GL_LINES);
	point = theFittingPlanePoint.XYZ() + theFittingPlaneX * minx + theFittingPlaneY * miny;
	::glVertex3f(point.X(), point.Y(), point.Z());
	point = theFittingPlanePoint.XYZ() + theFittingPlaneX * maxx + theFittingPlaneY * miny;
	::glVertex3f(point.X(), point.Y(), point.Z());
	::glVertex3f(point.X(), point.Y(), point.Z());
	point = theFittingPlanePoint.XYZ() + theFittingPlaneX * maxx + theFittingPlaneY * maxy;
	::glVertex3f(point.X(), point.Y(), point.Z());
	::glVertex3f(point.X(), point.Y(), point.Z());
	point = theFittingPlanePoint.XYZ() + theFittingPlaneX * minx + theFittingPlaneY * maxy;
	::glVertex3f(point.X(), point.Y(), point.Z());
	::glVertex3f(point.X(), point.Y(), point.Z());
	point = theFittingPlanePoint.XYZ() + theFittingPlaneX * minx + theFittingPlaneY * miny;
	::glVertex3f(point.X(), point.Y(), point.Z());
	::glEnd();
}

void Shape_OCC::glDrawCurveSampling()
{
	char message[100];
	glDisable(GL_LIGHTING);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glPointSize(10);

	int size = curveSectionArray1_Sampling.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt p1 = curveSectionArray1_Sampling[i];
		::glBegin(GL_POINTS);
		::glVertex3f(p1.X(), p1.Y(), p1.Z());
		::glEnd();
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}

	size = curveSectionArray2_Sampling.size();
	for (int i = 0; i < size; i++)
	{
		gp_Pnt p1 = curveSectionArray2_Sampling[i];
		::glBegin(GL_POINTS);
		::glVertex3f(p1.X(), p1.Y(), p1.Z());
		::glEnd();

		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}


}
void Shape_OCC::glDrawCurveandDerivatives()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, -2.0);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10);
	std::vector<gp_Pnt> tessellationPoints;
	int size = curveArray.size();
	::glBegin(GL_LINES);
	int SEGMENTS = 10;
	for (int i = 0; i < size; i++)
	{
		double t0 = curveArray[i]->FirstParameter();
		double t1 = curveArray[i]->LastParameter();
		double t;
		gp_Pnt p1, p2;
		for (int j = 0; j <= SEGMENTS; j++)
		{
			t = t0 + (t1 - t0) * j / SEGMENTS;
			curveArray[i]->D0(t, p1);
			curvederiveArray[i]->D0(t, p2);

			p2.SetXYZ(p1.XYZ() + p2.XYZ());
			::glVertex3f(p1.X(), p1.Y(), p1.Z());
			::glVertex3f(p2.X(), p2.Y(), p2.Z());
		}
	}
	::glEnd();

	for (int i = 0; i < size; i++)
	{
		double t0 = curveArray[i]->FirstParameter();
		double t1 = curveArray[i]->LastParameter();
		gp_Pnt p1;
		curveArray[i]->D0((t0 + t1) / 2, p1);

		char message[100];
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
	}
}
void Shape_OCC::glDrawrSelectedEdges()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, -2.0);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10);
	std::vector<gp_Pnt> tessellationPoints;
	int edgeindex;
	for (int i = 0; i < arraySelectedGapIndexArray.size(); i++)
	{

		edgeindex = arraySelectedGapIndexArray[i];
		TopLoc_Location location;

		// 如果没有现有的镶嵌数据，创建一个新的多边形表示

		// 函数getTessellationPoints(edge)来获取边的镶嵌点
		tessellationPoints.clear();

		std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
		auto iterator = m_EdgeTessellationMaps.find(gapEdges[edgeindex].TShape());
		if (iterator != m_EdgeTessellationMaps.end())
		{
			ptessellationPoints = &iterator->second;
		}
		else
		{
			getTessellationPoints(gapEdges[edgeindex], tessellationPoints);
			m_EdgeTessellationMaps.insert(std::make_pair(gapEdges[edgeindex].TShape(), tessellationPoints));
		}

		//draw gap edge
		if ((*ptessellationPoints).size() > 0)
		{
			::glBegin(GL_LINES);
			// 循环绘制边的线段
			for (Standard_Integer j = 0; j < (*ptessellationPoints).size() - 1; ++j)
			{
				const gp_Pnt& p1 = (*ptessellationPoints)[j];
				const gp_Pnt& p2 = (*ptessellationPoints)[j + 1];
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
			}

			::glEnd();
		}

		if (gapEdgesHausdorffDistance[edgeindex] < 1e-8)
			continue;

		//draw hausdorff

		Standard_Real first, last;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(gapEdges[edgeindex], first, last);

		gp_Pnt middle;

		curve->D0((first + last) / 2, middle);


		char message[100];
		sprintf(message, "%f", gapEdgesHausdorffDistance[edgeindex]);
		p_FormView3->drawOpenglString(middle.X(), middle.Y(), middle.Z(), message);
	}
}
void Shape_OCC::glDrawrGapEdges()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0, -2.0);

	glColor3f(1, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10);
	std::vector<gp_Pnt> tessellationPoints;
	for (int i = 0; i < gapEdges.size(); i++)
	{
		TopLoc_Location location;

		// 如果没有现有的镶嵌数据，创建一个新的多边形表示

		// 函数getTessellationPoints(edge)来获取边的镶嵌点
		tessellationPoints.clear();

		std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
		auto iterator = m_EdgeTessellationMaps.find(gapEdges[i].TShape());
		if (iterator != m_EdgeTessellationMaps.end())
		{
			ptessellationPoints = &iterator->second;
		}
		else
		{
			getTessellationPoints(gapEdges[i], tessellationPoints);
			m_EdgeTessellationMaps.insert(std::make_pair(gapEdges[i].TShape(), tessellationPoints));
		}

		//draw gap edge
		if ((*ptessellationPoints).size() > 0)
		{
			::glBegin(GL_LINES);
			// 循环绘制边的线段
			for (Standard_Integer j = 0; j < (*ptessellationPoints).size() - 1; ++j)
			{
				const gp_Pnt& p1 = (*ptessellationPoints)[j];
				const gp_Pnt& p2 = (*ptessellationPoints)[j + 1];
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
			}

			::glEnd();
		}

		if (gapEdgesHausdorffDistance[i] < 1e-8)
			continue;

		//draw hausdorff
		// 获取边的起始点和终点
		TopoDS_Vertex v1, v2;
		TopExp::Vertices(gapEdges[i], v1, v2);

		//获取起始点坐标
		gp_Pnt p1 = BRep_Tool::Pnt(v1);
		//获取终止点坐标
		gp_Pnt p2 = BRep_Tool::Pnt(v2);

		double x = (p1.X() + p2.X()) / 2;
		double y = (p1.Y() + p2.Y()) / 2;
		double z = (p1.Z() + p2.Z()) / 2;
		char message[100];
		sprintf(message, "%f", gapEdgesHausdorffDistance[i]);
		p_FormView3->drawOpenglString(x, y, z, message);

	}

}

void Shape_OCC::glRenderTriangulation()
{
	if (!m_ListDone || m_Modified)
		BuildTriangulationList();

	// Search for a new list
	if (::glIsList(m_ListOpenGL) == GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
		return;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n", m_ListOpenGL);
		return;
	}
}

void Shape_OCC::BuildTriangulationList()

{
	if (!m_Modified && m_ListDone)
		return;


	// Erase last list
	::glDeleteLists(m_ListOpenGL, 1);

	m_ListOpenGL = ::glGenLists(1);
	if (m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return;
	}
	::glNewList(m_ListOpenGL, GL_COMPILE_AND_EXECUTE);

	glDisable(GL_POLYGON_OFFSET_FILL);
	::glColor3ub(color.r(), color.g(), color.b());
	int nTriangles = triangles.size() / 3;
	int n1, n2, n3; //vertex indices
	::glBegin(GL_TRIANGLES);
	for (int i = 0; i < nTriangles; i++)
	{
		n1 = triangles[i * 3];
		n2 = triangles[i * 3 + 1];
		n3 = triangles[i * 3 + 2];

		if (normalCoords.size() > 0)
			::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
		::glVertex3f(coords[n1 * 3], coords[n1 * 3 + 1], coords[n1 * 3 + 2]);

		if (normalCoords.size() > 0)
			::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
		::glVertex3f(coords[n2 * 3], coords[n2 * 3 + 1], coords[n2 * 3 + 2]);

		if (normalCoords.size() > 0)
			::glNormal3f(normalCoords[n3 * 3], normalCoords[n3 * 3 + 1], normalCoords[n3 * 3 + 2]);
		::glVertex3f(coords[n3 * 3], coords[n3 * 3 + 1], coords[n3 * 3 + 2]);
	}
	::glEnd();
	::glEndList();

	m_ListDone = 1;
	m_Modified = 0;
}
int Shape_OCC::PickTest(PickStruct& pickstruct)
{
	return -1;
}
void Shape_OCC::ComputeBoundingBox(Vector3D& lower, Vector3D& upper)
{
	if (m_setBoundingBox)
	{
		lower = this->lower - ZeroP3D;
		upper = this->upper - ZeroP3D;
		return;
	}
	m_setBoundingBox = 1;

	Bnd_Box boundingBox;
	BRepBndLib::Add(shape, boundingBox);
	boundingBox.Get(lower.x, lower.y, lower.z, upper.x, upper.y, upper.z);
	this->lower = lower;
	this->upper = upper;
	/*int n = coords.size() / 3;
	if (n == 0)
		return;
	lower[0] = upper[0] = coords[0];
	lower[1] = upper[1] = coords[1];
	lower[2] = upper[2] = coords[2];

	for (int i = 1; i < n; i++)
	{
		if (lower[0] > coords[i * 3])
			lower[0] = coords[i * 3];
		if (lower[1] > coords[i * 3 + 1])
			lower[1] = coords[i * 3 + 1];
		if (lower[2] > coords[i * 3 + 2])
			lower[2] = coords[i * 3 + 2];

		if (upper[0] < coords[i * 3])
			upper[0] = coords[i * 3];
		if (upper[1] < coords[i * 3 + 1])
			upper[1] = coords[i * 3 + 1];
		if (upper[2] < coords[i * 3 + 2])
			upper[2] = coords[i * 3 + 2];
	}*/
}

void Shape_OCC::DrawCoordinateAxis()
{
	glDisable(GL_LIGHTING);
	Vector3D lower, upper;
	ComputeBoundingBox(lower, upper);

	glLineWidth(3.0);
	glDisable(GL_LIGHTING);
	::glColor3f(1, 0, 0);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(upper.x, lower.y, lower.z);
	::glEnd();

	::glColor3f(0, 1, 0);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(lower.x, upper.y, lower.z);
	::glEnd();

	::glColor3f(0, 0, 1);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(lower.x, lower.y, upper.z);
	::glEnd();
	glEnable(GL_LIGHTING);
}

int Shape_OCC::glDraw()
{
	if (!m_Show)
		return 0;

	::glPushMatrix();
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);

	glFrontFace(GL_CW);
	//------------
	// Material

	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 


	//construct texture
	GLubyte* pTextureBits;
	GLint width, height;
	/*if (m_ShowTexture)
	{
		m_pDib = new CDib(m_FileTextureName);
		pTextureBits = (GLubyte*)m_pDib->GetDibBitsPtr();
		width = m_pDib->GetDibWidth();
		height = m_pDib->GetDibHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTextureBits);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}*/
	//---------------
	//This is a for Bezier surface;
	//glMap2f(GL_MAP2_VERTEX_4,
	//	0.0, 1.0, 4, m_udegree + 1, 0.0, 1.0, 800, m_vdegree + 1,
	//	&m_ControlPoint[0][0][0]);

	//
	GLboolean re1 = glIsEnabled(GL_MAP2_VERTEX_4);
	if (re1 == GL_INVALID_ENUM || re1 == GL_INVALID_VALUE)
	{
		int i = 0;
	}

	//----------- Show Texture----------------
	/*if (m_ShowTexture)
	{
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, (const float*)textureCntrlPnt);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
	}*/
	//-----------End Texture--------------

	//::glDisable(GL_LIGHTING);
	glEnable(GL_MAP2_VERTEX_4);
	//glColor3f(0.0,0.0,1.0);

	glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
	::glPushMatrix();

	//Show the Coordinate Axis
	if (m_ShowCoordinateAxis)
	{
		DrawCoordinateAxis();
	}

	if (!m_SetCameraMode)
	{
		// Position / translation / scaling
		glScalef(m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());

		glRotatef(m_Transform.GetValueRotation(),
			m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z());

		glTranslatef(m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());
	}

	//store the model view & projection view.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, m_viewPort);

	//Don't render triangulation here, render triangulation in each faces
	//::glDisable(GL_LIGHTING);
	//glRenderTriangulation();
	//::glEnable(GL_LIGHTING);

	//if (m_showEdge)
	//	glDrawEdges();

	if (m_showGapEdge)
		glDrawrGapEdges();

	if (m_showSelectedEdge)
		glDrawrSelectedEdges();

	if (m_showFaceLackBoundary)
		glDrawrFaceLackBoundaries();

	if (m_showFaceIndex)
		glDrawFaceIndex();

	if (m_showEdgeIndex)
		glDrawEdgeIndex();

	if (m_showVertexIndex)
		glDrawVertexIndex();

	glDrawCurveandDerivatives();

	if (m_showFaceLackTessellationPoints)
		glDrawCurveSampling();

	if (m_showFaceLackFittingPlane)
		glDrawFittingPlane();

	if (m_showFaceLackProjectedPoints)
		glDrawProjectedPoints();

	//Draw the Area Preserving Iso-Parameter Curves.
	//Stored in the uarray & varray

	::glPopMatrix();

	::glPopMatrix();
	glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	return 1;
}

int Shape_OCC::getOCCVertexIndex(TopoDS_Vertex vertex)
{
	auto iterator = m_VertexIndexMaps.find(vertex.TShape());
	int vertexIndex = -1;
	if (iterator != m_VertexIndexMaps.end())
	{
		vertexIndex = iterator->second;
	}
	return vertexIndex;
}

int Shape_OCC::getOCCEdgeIndex(TopoDS_Edge edge)
{
	auto iterator = m_EdgeIndexMaps.find(edge.TShape());
	int edgeIndex = -1;
	if (iterator != m_EdgeIndexMaps.end())
	{
		edgeIndex = iterator->second;
	}
	return edgeIndex;
}

int Shape_OCC::getOCCFaceIndex(TopoDS_Face face)
{
	auto iterator = m_FaceIndexMaps.find(face.TShape());
	int faceIndex = -1;
	if (iterator != m_FaceIndexMaps.end())
	{
		faceIndex = iterator->second;
	}
	return faceIndex;
}

void Shape_OCC::get_tessellationpoints_from_edges_occ(const TopoDS_Shape& edge, std::vector<gp_Pnt>& tessellationPoints)
{
	auto iterator = m_EdgeTessellationMaps.find(edge.TShape());
	if (iterator != m_EdgeTessellationMaps.end())
	{
		tessellationPoints = iterator->second;

	}
	else
	{
		getTessellationPoints(TopoDS::Edge(edge), tessellationPoints);
		m_EdgeTessellationMaps.insert(std::make_pair(edge.TShape(), tessellationPoints));
	}
}