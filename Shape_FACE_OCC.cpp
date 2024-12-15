#include "stdafx.h"
#include "Shape_FACE_OCC.h"
#include <GeomAPI_IntCS.hxx>
#include "Lib3D/Base3d.h"
#include <gce_MakeLin.hxx>
#include <Geom_Line.hxx>
#include <BRep_Tool.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "IntTools_EdgeFace.hxx"
#include "TopoDS.hxx"
#include "NCollection_Sequence.hxx"

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
#include "BRepTools.hxx"
#include <BRep_Tool.hxx>
#include "Geom_BSplineSurface.hxx"
#include "FormView3.h"
#include "SDIViewSwitchDoc.h"

Shape_FACE_OCC::Shape_FACE_OCC()
{
	type = TYPE_BREP_OCC_FACE;
	m_showControlPointsNetwork = 0;
	m_showIsoParameterCurves = 0;
	m_showSurface = 1;
	m_showVertexIndex = 0;
	color.Set(128, 128, 128);

}
Shape_FACE_OCC::~Shape_FACE_OCC()
{

}
//********************************************
// glDraw
//********************************************
void Shape_FACE_OCC::glRenderTriangulation()
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
double Shape_FACE_OCC::GetFaceArea()
{
	int nTriangles = triangles.size() / 3;
	int n1, n2, n3; //vertex indices
	double trianglearea;
	double facearea = 0;
	for (int i = 0; i < nTriangles; i++)
	{
		n1 = triangles[i * 3];
		n2 = triangles[i * 3 + 1];
		n3 = triangles[i * 3 + 2];

		Vector3D v1(coords[n1 * 3], coords[n1 * 3 + 1], coords[n1 * 3 + 2]);
		Vector3D v2(coords[n2 * 3], coords[n2 * 3 + 1], coords[n2 * 3 + 2]);
		Vector3D v3(coords[n3 * 3], coords[n3 * 3 + 1], coords[n3 * 3 + 2]);
		trianglearea = CMesh3d::TriangleArea(v1, v2, v3);
		facearea += trianglearea;
	}
	return facearea;
}
void Shape_FACE_OCC::BuildTriangulationList()
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
void Shape_FACE_OCC::glDrawVertexIndex()
{
	if (!pParentObject)
		return;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	TopExp_Explorer vertexexplorer(shape, TopAbs_VERTEX);
	gp_Pnt p1;
	int vindex;
	Shape_OCC* pOcc = (Shape_OCC*)pParentObject;
	for (; vertexexplorer.More(); vertexexplorer.Next())
	{
		TopoDS_Vertex theVertex = TopoDS::Vertex(vertexexplorer.Current());

		auto iterator = pOcc->m_VertexIndexMaps.find(theVertex.TShape());
		if (iterator != pOcc->m_VertexIndexMaps.end())
		{
			vindex = iterator->second;
			p1 = BRep_Tool::Pnt(theVertex);

			sprintf(message, "%d", vindex);
			p_FormView3->drawOpenglString(p1.X(), p1.Y(), p1.Z(), message);
		}


	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
void Shape_FACE_OCC::glShowControlPointNetworks()
{
	glLineWidth(2.0f);
	glColor3f(1, 1, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);


	Handle(Geom_Surface) gsurf = BRep_Tool::Surface(TopoDS::Face(shape));
	Handle(Geom_BSplineSurface) bSurf1 = Handle(Geom_BSplineSurface)::DownCast(gsurf);

	TColgp_Array2OfPnt controlnetwork(1, bSurf1->NbUPoles(), 1, bSurf1->NbVPoles());
	bSurf1->Poles(controlnetwork);
	::glBegin(GL_LINES);
	for (int i = 1; i < bSurf1->NbUPoles(); i++)
		for (int j = 1; j < bSurf1->NbVPoles(); j++)
		{
			::glVertex3f(controlnetwork(i, j).X(), controlnetwork(i, j).Y(), controlnetwork(i, j).Z());
			::glVertex3f(controlnetwork(i, j + 1).X(), controlnetwork(i, j + 1).Y(), controlnetwork(i, j + 1).Z());
		}

	for (int i = 1; i < bSurf1->NbUPoles(); i++)
		for (int j = 1; j < bSurf1->NbVPoles(); j++)
		{
			::glVertex3f(controlnetwork(i, j).X(), controlnetwork(i, j).Y(), controlnetwork(i, j).Z());
			::glVertex3f(controlnetwork(i + 1, j).X(), controlnetwork(i + 1, j).Y(), controlnetwork(i + 1, j).Z());
		}
	::glEnd();
	//CC1->Poles(P1);
	//if ()

	//SURFACE* pSurface = pAcisEntity->geometry();

	//int identity = pSurface->identity();
	//if (identity == PLANE_ID)
	//{

	//}
	//else if (identity == SPLINE_ID)
	//{
	//	SPLINE* surface = (SPLINE*)(pSurface);
	//	int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0, degree_u = 0, num_uknots = 0, degree_v = 0, num_vknots = 0, rational_u = 0, rational_v = 0;
	//	::SPAposition* ctrlpts = nullptr;
	//	double* weights = nullptr, * uknots = nullptr, * vknots = nullptr;
	//	spline* splinesurface = (spline*)&(pAcisEntity->geometry()->equation());
	//	bs3_surface_to_array(splinesurface->sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v, num_u, num_v, ctrlpts, weights, degree_u, num_uknots, uknots, degree_v, num_vknots, vknots, 0);
	//	//pAcisEntity->geometry()->equation().
	//	//bs3_surface_to_array()
	//	//	pAcisEntity->geometry()->equation()
	//	::glBegin(GL_LINES);
	//	for (int i = 0; i < num_u; i++)
	//		for (int j = 0; j < num_v; j++)
	//		{
	//			SPAposition  pt1 = ctrlpts[i * num_v + j];
	//			SPAposition pt2;
	//			SPAposition pt3;
	//			if (j < num_v - 1)
	//			{
	//				pt2 = ctrlpts[i * num_v + j + 1];

	//				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
	//				::glVertex3f(pt1.x(), pt1.y(), pt1.z());

	//				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
	//				::glVertex3f(pt2.x(), pt2.y(), pt2.z());
	//			}
	//			if (i < num_u - 1)
	//			{
	//				pt3 = ctrlpts[(i + 1) * num_v + j];
	//				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
	//				::glVertex3f(pt1.x(), pt1.y(), pt1.z());

	//				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
	//				::glVertex3f(pt3.x(), pt3.y(), pt3.z());
	//			}

	//		}
	//	::glEnd();
	//	//splinesurface
	//	//SPAinterval aui = pAcisEntity->geometry()->equation().param_range_u();
	//	SPAinterval avi = pAcisEntity->geometry()->equation().param_range_v();

	//	//std::vector sample_params{ SPApar_pos(spline_param.u, range_v.start_pt()), SPApar_pos(spline_param.u, range_v.end_pt()), SPApar_pos(range_u.start_pt(), spline_param.v), SPApar_pos(range_v.end_pt(), spline_param.v) };

	//	//bool on_boundary = false;
	//	//for (auto param : sample_params) {
	//	//	SPAposition pos;
	//	//	equ->eval(param, pos);
	//	/*const int U_NUM = 1E1, V_NUM = 1E1;

	//	SPApar_box par = surface->param_range();
	//	//double ua = surface->sur()->get_sur()->knot_start_u();


	//	SPAinterval aui = surface->subset_u_interval();
	//	SPAinterval avi = surface->subset_v_interval();*/
	//	/*int iso_NUM = 1E1;
	//	int dis = 100;
	//	double aua = aui.start_pt(), aub = aui.end_pt(), aud = (aub - aua) / iso_NUM;
	//	double ava = avi.start_pt(), avb = avi.end_pt(), avd = (avb - ava) / dis;
	//	::glBegin(GL_LINES);
	//	for (int i = 0; i <= iso_NUM; ++i)
	//	{
	//		for (int j = 0; j < dis; ++j)
	//		{
	//			SPApar_pos auv1(aua + i * aud, ava + j * avd);
	//			SPApar_pos auv2(aua + i * aud, ava + (j + 1) * avd);
	//			// 求参数处的点
	//			SPAposition pos1, pos2;
	//			pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
	//			pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

	//			//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
	//			::glVertex3f(pos1.x(), pos1.y(), pos1.z());

	//			//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
	//			::glVertex3f(pos2.x(), pos2.y(), pos2.z());

	//		}
	//	}

	//	aud = (aub - aua) / dis;
	//	avd = (avb - ava) / iso_NUM;
	//	for (int i = 0; i <= iso_NUM; ++i)
	//	{
	//		for (int j = 0; j < dis; ++j)
	//		{
	//			SPApar_pos auv1(aua + j * aud, ava + i * avd);
	//			SPApar_pos auv2(aua + (j + 1) * aud, ava + i * avd);
	//			// 求参数处的点
	//			SPAposition pos1, pos2;
	//			pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
	//			pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

	//			//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
	//			::glVertex3f(pos1.x(), pos1.y(), pos1.z());

	//			//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
	//			::glVertex3f(pos2.x(), pos2.y(), pos2.z());

	//		}
	//	}
	//	::glEnd();*/
	//}
}
void Shape_FACE_OCC::glShowIsoParameterCurves()
{
	glLineWidth(5.0f);
	glColor3f(0, 1, 0);
	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	if (shape.ShapeType() == TopAbs_FACE)
	{
		TopoDS_Face face = TopoDS::Face(shape);
		double umin, umax, vmin, vmax;
		BRepTools::UVBounds(face, umin, umax, vmin, vmax);
		const BRep_TFace* TE = static_cast<const BRep_TFace*>(face.TShape().get());
		Handle(Geom_Surface) surf = TE->Surface();
		int iso_NUM = pDoc->m_Disc1;
		int dis = pDoc->m_Disc2;
		double u, v1, v2, v, u1, u2;
		gp_Pnt p1, p2;
		::glBegin(GL_LINES);
		for (int i = 0; i <= iso_NUM; i++)
		{
			u = umin + (umax - umin) * i / iso_NUM;
			for (int j = 0; j < dis; j++)
			{
				v1 = vmin + (vmax - vmin) * j / dis;
				v2 = vmin + (vmax - vmin) * (j + 1) / dis;
				surf->D0(u, v1, p1);
				surf->D0(u, v2, p2);
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
			}
		}

		for (int i = 0; i <= iso_NUM; i++)
		{
			v = vmin + (vmax - vmin) * i / iso_NUM;
			for (int j = 0; j < dis; j++)
			{
				u1 = umin + (umax - umin) * j / dis;
				u2 = umin + (umax - umin) * (j + 1) / dis;
				surf->D0(u1, v, p1);
				surf->D0(u2, v, p2);
				::glVertex3f(p1.X(), p1.Y(), p1.Z());
				::glVertex3f(p2.X(), p2.Y(), p2.Z());
			}
		}

		::glEnd();
	}
}
void Shape_FACE_OCC::glRenderEdges_Selected()
{
	if (m_IsSelected)
		glLineWidth(5.0f);
	else
		glLineWidth(2.0f);
	::glDisable(GL_LIGHTING);
	if (m_IsSelected)
		glColor3f(0, 0, 1);
	else
		glColor3f(0, 1, 0);
	//if (m_IsSelected)
	//	glDisable(GL_DEPTH_TEST);
	//else
	glEnable(GL_DEPTH_TEST);

	TopExp_Explorer explorer(shape, TopAbs_EDGE);
	std::vector<float> coords;
	std::vector<gp_Pnt> tessellationPoints;
	for (; explorer.More(); explorer.Next())
	{
		TopoDS_Edge edge = TopoDS::Edge(explorer.Current());
		TopLoc_Location location;

		tessellationPoints.clear();

		// 函数getTessellationPoints(edge)来获取边的镶嵌点
		std::vector<gp_Pnt>* ptessellationPoints = &tessellationPoints;
		Shape_OCC* pShapeOCC = (Shape_OCC*)pParentObject;
		auto iterator = pShapeOCC->m_EdgeTessellationMaps.find(edge.TShape());
		if (iterator != pShapeOCC->m_EdgeTessellationMaps.end())
		{
			ptessellationPoints = &iterator->second;
		}

		//Handle_Poly_Polygon3D polygon = BRep_Tool::Polygon3D(edge, location);
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

}
void Shape_FACE_OCC::ComputeBoundingBox(Vector3D& lower, Vector3D& upper)
{
	int n = coords.size() / 3;
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
	}
}

int Shape_FACE_OCC::PickTest(PickStruct& pickstruct)
{
	gce_MakeLin makeline(gp_Pnt(pickstruct.point.x, pickstruct.point.y, pickstruct.point.z), gp_Dir(pickstruct.dir.x, pickstruct.dir.y, pickstruct.dir.z));

	BRepBuilderAPI_MakeEdge anEdge(makeline.Value());
	//Handle(Geom_Line) aGeomLine = new Geom_Line(makeline.Value());

	IntTools_EdgeFace edgeFaceInt;
	edgeFaceInt.SetEdge(anEdge);
	edgeFaceInt.SetFace(TopoDS::Face(shape));
	edgeFaceInt.SetRange(0, Length(pickstruct.dir));
	edgeFaceInt.Perform();

	if (edgeFaceInt.IsDone()) // Process errors
	{
		const IntTools_SequenceOfCommonPrts& cp = edgeFaceInt.CommonParts();
		int size = cp.Size();
		if (size > 0)
		{
			for (int i = 1; i <= size; i++)
			{
				const IntTools_CommonPrt& item = cp.Value(i);
				if (item.Type() == TopAbs_VERTEX)
				{
					double depth = item.VertexParameter1();
					if (depth < pickstruct.mindepth)
					{
						pickstruct.mindepth = depth;
						pickstruct.m_SelectIndex = m_ScenegraphIndex;

					}
				}
			}
		}
	}


	//if (anAlgo.IsDone())
	//{
	//	// Get result.
	//	TopoDS_Shape aResultShape = anAlgo.Shape();
	//	myResult << "Result shape was created in red" << std::endl;
	//	Handle(AIS_ColoredShape) anAisShape = new AIS_ColoredShape(aShape);
	//	Handle(AIS_Plane) anAisPlane = new AIS_Plane(new Geom_Plane(aPln));
	//	anAisShape->SetColor(Quantity_Color(Quantity_NOC_YELLOW));
	//	Handle(AIS_ColoredShape) anAisResult = new AIS_ColoredShape(aResultShape);
	//	anAisResult->SetColor(Quantity_Color(Quantity_NOC_RED));
	//	myObject3d.Append(anAisShape);
	//	myObject3d.Append(anAisPlane);
	//	myObject3d.Append(anAisResult);
	//	myContext->SetDisplayMode(anAisShape, 0, Standard_True);
	//}

	//BRepAPI_IntEF Intersector(aGeomLine, pOCCEntity->gEO);

	//Standard_Integer nb = Intersector.NbPoints();
	//gp_Pnt& p = Intersector.Point(Index);

	//Handle(GeomAbs_Line) l1 = new Geom_Line(gp_Pnt(pickstruct.point.x, pickstruct.point.y, pickstruct.point.z),
	//	gp_Pnt(pickstruct.point.x + pickstruct.dir.x, pickstruct.point.y + pickstruct.dir.y, pickstruct.point.z + pickstruct.dir.z));

	//GeomAPI_IntCS Intersector();

	//Standard_Integer nb = Intersector.NbPoints();
	//gp_Pnt& p = Intersector.Point(Index);
	////
	//SPAposition p1(pickstruct.point.x, pickstruct.point.y, pickstruct.point.z);
	//SPAposition p2(pickstruct.point.x + pickstruct.dir.x, pickstruct.point.y + pickstruct.dir.y, pickstruct.point.z + pickstruct.dir.z);
	////SPAposition pos1(0.0, 0.0, 0.0);
	////SPAunit_vector gme_uvec(0.0, 0.0, 1.0);
	////GME::plane gme_pla(gme_pos1, gme_uvec);
	//EDGE* st1 = nullptr;
	////GME::FACE* gme_st2 = nullptr;

	///* 调用API */
	//// __GME_API_BEGIN
	//api_curve_line(p1, p2, st1);
	////GME::check_outcome(GME::api_face_plane(gme_pos1, 5, 5, &gme_uvec, gme_st2));

	//ENTITY_LIST* entity_list = ACIS_NEW ENTITY_LIST();
	//api_edfa_int(st1, pAcisEntity, entity_list);
	//if (entity_list->count() > 0)
	//{
	//	for (auto ent : *entity_list)
	//	{
	//		const char* tn = ent->type_name();

	//		if (strcmp(tn, "vertex") == 0)
	//		{
	//			VERTEX* ptrVertex = (VERTEX*)ent;
	//			SPAposition pos = ptrVertex->geometry()->coords();

	//			Point3D pos1(pos.x(), pos.y(), pos.z());
	//			double depth = (pos1 - pickstruct.point) * pickstruct.dir / (pickstruct.dir * pickstruct.dir);
	//			if (depth < pickstruct.mindepth)
	//			{
	//				pickstruct.mindepth = depth;
	//				pickstruct.m_SelectIndex = m_ScenegraphIndex;
	//			}
	//		}
	//	}
	//	return 1;
	//}
	//else
	return 0;
}
int Shape_FACE_OCC::glDraw()
{
	if (pParentObject && pParentObject->m_Show == 0)
		return 0;

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
	//if (m_ShowCoordinateAxis)
	{
		//	DrawCoordinateAxis();
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

	//Draw the iso parameter curves.

	//if (m_showSurface)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//if (pDoc->m_Light)
		::glEnable(GL_LIGHTING);
		//else
		//	::glDisable(GL_LIGHTING);

		//glColor3f(s_r, s_g, s_b);

		{
			//::glDisable(GL_LIGHTING);
			if (m_showSurface)
				glRenderTriangulation();
			//::glEnable(GL_LIGHTING);
		}
	}

	//画曲面的绿色边缘
	if (m_IsSelected)
		glRenderEdges_Selected();

	if (m_showIsoParameterCurves)
		glShowIsoParameterCurves();

	if (m_showControlPointsNetwork)
		glShowControlPointNetworks();

	if (m_showVertexIndex)
		glDrawVertexIndex();
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