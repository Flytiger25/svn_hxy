// SurfaceInformation.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "SurfaceInformation.h"
#include "Nurbs3D.h"
#include "Lib3D/Base3d.h"
#include "Entity_FACE_ACIS.h"
#include "Entity_EDGE_ACIS.h"
#include "face.hxx"
#include "loop.hxx"
#include "edge.hxx"
#include <sps3srtn.hxx>
#include <cstrapi.hxx>
#include "intdef.hxx"
#include "curve.hxx"
#include "sps3crtn.hxx"
#include "Shape_FACE_OCC.h"
#include "TopoDS.hxx"
#include "BRepTools.hxx"
#include <BRep_Tool.hxx>
#include "TShape_OCC.h"
#include "surface.hxx"
#include "surdef.hxx"
#include "af_api.hxx"
#include <Shape_EDGE_OCC.h>
#include <Shape_VERTEX_OCC.h>
#include <Entity_VERTEX_ACIS.h>
#include "point.hxx"
#include "vertex.hxx"
// SurfaceInformation dialog

IMPLEMENT_DYNAMIC(SurfaceInformation, CDialog)

SurfaceInformation::SurfaceInformation(CWnd* pParent /*=NULL*/)
	: CDialog(SurfaceInformation::IDD, pParent)
	, m_surfaceinformation(_T(""))
{
	pSurface = NULL;
}

SurfaceInformation::~SurfaceInformation()
{
}

void SurfaceInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_surfaceinformation);
}


BEGIN_MESSAGE_MAP(SurfaceInformation, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// SurfaceInformation message handlers

void SurfaceInformation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	if (!pSurface)
		return;
	CString s1;

	if (pSurface->GetType() == TYPE_NURBSSURFACE)
	{
		CNurbsSuface* pNurbsSurf = (CNurbsSuface*)pSurface;
		char message[500];
		sprintf(message, "Control number : %d %d  \r\n", pNurbsSurf->GetCtrlNumberU(), pNurbsSurf->GetCtrlNumberV());
		m_surfaceinformation = message;

		sprintf(message, "Degree: %d %d  \r\n", pNurbsSurf->m_udegree, pNurbsSurf->m_vdegree);
		s1 = message;
		m_surfaceinformation = m_surfaceinformation + s1;

		for (int i = 0; i < pNurbsSurf->m_uknotcount; i++)
		{
			sprintf(message, " %1.2f ", pNurbsSurf->m_uknot[i]);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;
		}
		sprintf(message, "\r\n");
		s1 = message;
		m_surfaceinformation = m_surfaceinformation + s1;
		for (int i = 0; i < pNurbsSurf->m_vknotcount; i++)
		{
			sprintf(message, " %1.2f ", pNurbsSurf->m_vknot[i]);
			s1 = message;
			m_surfaceinformation = m_surfaceinformation + s1;
		}
		sprintf(message, "\r\n");
		s1 = message;
		m_surfaceinformation = m_surfaceinformation + s1;

		for (int i = 0; i < pNurbsSurf->GetCtrlNumberU(); i++)
			for (int j = 0; j < pNurbsSurf->GetCtrlNumberV(); j++)
			{
				Point3D p1 = pNurbsSurf->GetControlPoint(i, j);
				double w = pNurbsSurf->GetWeight(i, j);
				sprintf(message, "(%d  %d):  %1.6f   %1.6f   %1.6f  %1.6f \r\n", i, j, p1.x, p1.y, p1.z, w);
				s1 = message;
				m_surfaceinformation = m_surfaceinformation + s1;
			}

		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_ACIS)
	{
		EntityList_ACIS* pEntity = (EntityList_ACIS*)pSurface;
		char message[500];

		sprintf(message, "ACIS Model\r\n");
		m_surfaceinformation = m_surfaceinformation + message;
		int theFaceSize = pEntity->arrayACISFaces.size();
		int theEdgeSize = pEntity->arrayACISEdges.size();
		int theVertexSize = pEntity->arrayACISVertices.size();
		sprintf(message, "%d Faces, %d Edges, %d vertices \r\n", theFaceSize, theEdgeSize, theVertexSize);
		m_surfaceinformation = m_surfaceinformation + message;

		sprintf(message, "Transformation\r\n Translation:%f,%f,%f \r\n", pEntity->GetTransform()->GetTranslation()->x(), pEntity->GetTransform()->GetTranslation()->y(), pEntity->GetTransform()->GetTranslation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Scale:%f,%f,%f \r\n", pEntity->GetTransform()->GetScale()->x(), pEntity->GetTransform()->GetScale()->y(), pEntity->GetTransform()->GetScale()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Rotation:%f,%f,%f \r\n", pEntity->GetTransform()->GetRotation()->x(), pEntity->GetTransform()->GetRotation()->y(), pEntity->GetTransform()->GetRotation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_ACIS_FACE)
	{
		Entity_FACE_ACIS* pEntity = (Entity_FACE_ACIS*)pSurface;

		int loopnum = 0;
		LOOP* curr_loop = pEntity->pAcisEntity->loop();
		while (curr_loop)
		{
			loopnum++;
			curr_loop = curr_loop->next();
		}

		char message[500];

		const char* typename1 = pEntity->pAcisEntity->geometry()->equation().type_name();
		double area;
		api_facet_area(pEntity->pAcisEntity, area);

		sprintf(message, "Surface Type : %s \r\n", typename1);
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Loop number : %d \r\n", loopnum);
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Area: %f \r\n", area);
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_OCC)
	{
		Shape_OCC* pEntity = (Shape_OCC*)pSurface;
		char message[500];

		sprintf(message, "OCC Model\r\n");
		m_surfaceinformation = m_surfaceinformation + message;
		int theFaceSize = pEntity->arrayOCCFaces.size();
		int theEdgeSize = pEntity->arrayOCCEdges.size();
		int theVertexSize = pEntity->arrayOCCVertices.size();
		sprintf(message, "%d Faces, %d Edges, %d vertices \r\n", theFaceSize, theEdgeSize, theVertexSize);
		m_surfaceinformation = m_surfaceinformation + message;

		sprintf(message, "Transformation\r\n Translation:%f,%f,%f \r\n", pEntity->GetTransform()->GetTranslation()->x(), pEntity->GetTransform()->GetTranslation()->y(), pEntity->GetTransform()->GetTranslation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Scale:%f,%f,%f \r\n", pEntity->GetTransform()->GetScale()->x(), pEntity->GetTransform()->GetScale()->y(), pEntity->GetTransform()->GetScale()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Rotation:%f,%f,%f \r\n", pEntity->GetTransform()->GetRotation()->x(), pEntity->GetTransform()->GetRotation()->y(), pEntity->GetTransform()->GetRotation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}
	else if (pSurface->GetType() == TYPE_BREP_ACIS_EDGE)
	{
		Entity_EDGE_ACIS* pEntity = (Entity_EDGE_ACIS*)pSurface;
		EDGE* pEdge = pEntity->pAcisEntity;
		const curve* curve = &(pEdge->geometry()->equation());

		SPAposition start = pEdge->start_pos();
		SPAposition middle = pEdge->mid_pos();
		SPAposition end = pEdge->end_pos();

		char  message[1000];
		sprintf(message, "start: %f,%f,%f \r\n", start.x(), start.y(), start.z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "middle: %f,%f,%f \r\n", middle.x(), middle.y(), middle.z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "end: %f,%f,%f \r\n", end.x(), end.y(), end.z());
		m_surfaceinformation = m_surfaceinformation + message;

		int curvetype = curve->type();
		bs3_curve* bscurve;
		int dim = 0, deg = 0, num_ctrlpts = 0, num_knots = 0;
		logical rat = 0;
		SPAposition* ctrlpts = nullptr;
		double* weights = nullptr, * knots = nullptr;
		if (curvetype == intcurve_type)
		{
			intcurve* intcurve_ = (intcurve*)curve;

			bs3_curve_to_array(intcurve_->cur(), dim, deg, rat, num_ctrlpts, ctrlpts, weights, num_knots, knots);


			sprintf(message, "bs3curve: Dimension : %d \r\n", dim);
			m_surfaceinformation = m_surfaceinformation + message;
			sprintf(message, "Degree : %d \r\n", deg);
			m_surfaceinformation = m_surfaceinformation + message;
			sprintf(message, "Rationality : %d \r\n", rat);
			m_surfaceinformation = m_surfaceinformation + message;
			sprintf(message, "Number of Control Points : %d \r\n", num_ctrlpts);
			m_surfaceinformation = m_surfaceinformation + message;
			for (int i = 0; i < num_ctrlpts; i++)
			{
				sprintf(message, "Control Point %d: %f, %f, %f \r\n", i, ctrlpts[i].x(), ctrlpts[i].y(), ctrlpts[i].z());
				m_surfaceinformation = m_surfaceinformation + message;
			}
			sprintf(message, "Number of Knots : %d \r\n", num_knots);
			m_surfaceinformation = m_surfaceinformation + message;

			for (int i = 0; i < num_knots; i++)
			{
				sprintf(message, " %f ", knots[i]);
				m_surfaceinformation = m_surfaceinformation + message;
			}

			//m_surfaceinformation = message;

		}
		UpdateData(FALSE);
	}
	else if (pSurface->GetType() == TYPE_BREP_ACIS_VERTEX)
	{
		Entity_VERTEX_ACIS* pEntity = (Entity_VERTEX_ACIS*)pSurface;
		VERTEX* pVertex = pEntity->pAcisEntity;
		const SPAposition point = (pVertex->geometry()->coords());

		char message[1000];
		sprintf(message, "Vertex %f,%f,%f\r\n", point.x(), point.y(), point.z());
		m_surfaceinformation = m_surfaceinformation + message;

		//m_surfaceinformation = message;
		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_OCC_FACE)
	{
		Shape_FACE_OCC* pEntity = (Shape_FACE_OCC*)pSurface;
		TopoDS_Face aF = TopoDS::Face(pEntity->shape);
		Shape_OCC* pOCC = (Shape_OCC*)(pEntity->pParentObject);

		Handle(Geom_Surface) surface = BRep_Tool::Surface(aF);
		char message[500];

		auto type = surface->DynamicType()->SystemName();
		sprintf(message, "Surface Type: %s\r\n", type);
		m_surfaceinformation = m_surfaceinformation + message;

		// 定义变量来存储属性
		//GProp_GProps props;
		// 计算面的属性
		//BRepGProp::SurfaceProperties(aF, props);

		sprintf(message, "Surface Area: %f\r\n", pEntity->GetFaceArea());
		m_surfaceinformation = m_surfaceinformation + message;

		sprintf(message, "Transformation\r\n Translation:%f,%f,%f \r\n", pEntity->GetTransform()->GetTranslation()->x(), pEntity->GetTransform()->GetTranslation()->y(), pEntity->GetTransform()->GetTranslation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Scale:%f,%f,%f \r\n", pEntity->GetTransform()->GetScale()->x(), pEntity->GetTransform()->GetScale()->y(), pEntity->GetTransform()->GetScale()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Rotation:%f,%f,%f \r\n", pEntity->GetTransform()->GetRotation()->x(), pEntity->GetTransform()->GetRotation()->y(), pEntity->GetTransform()->GetRotation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_OCC_EDGE)
	{
		Shape_EDGE_OCC* pEntity = (Shape_EDGE_OCC*)pSurface;
		TopoDS_Edge aF = TopoDS::Edge(pEntity->shape);
		Shape_OCC* pOCC = (Shape_OCC*)(pEntity->pParentObject);

		Standard_Real first, last;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(aF, first, last);

		char message[500];

		gp_Pnt start, middle, end;
		curve->D0(first, start);
		curve->D0((first + last) / 2, middle);
		curve->D0(last, end);

		sprintf(message, "Start: %f,%f,%f \r\n", start.X(), start.Y(), start.Z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "middle: %f,%f,%f \r\n", middle.X(), middle.Y(), middle.Z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "end: %f,%f,%f \r\n", end.X(), end.Y(), end.Z());
		m_surfaceinformation = m_surfaceinformation + message;

		auto type = curve->DynamicType()->SystemName();
		sprintf(message, "Curve Type: %s\r\n", type);
		m_surfaceinformation = m_surfaceinformation + message;

		// 定义变量来存储属性
		//GProp_GProps props;
		// 计算面的属性
		//BRepGProp::SurfaceProperties(aF, props);

		//sprintf(message, "Surface Area: %f\r\n", pEntity->GetFaceArea());
		//m_surfaceinformation = m_surfaceinformation + message;

		sprintf(message, "Transformation\r\n Translation:%f,%f,%f \r\n", pEntity->GetTransform()->GetTranslation()->x(), pEntity->GetTransform()->GetTranslation()->y(), pEntity->GetTransform()->GetTranslation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Scale:%f,%f,%f \r\n", pEntity->GetTransform()->GetScale()->x(), pEntity->GetTransform()->GetScale()->y(), pEntity->GetTransform()->GetScale()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Rotation:%f,%f,%f \r\n", pEntity->GetTransform()->GetRotation()->x(), pEntity->GetTransform()->GetRotation()->y(), pEntity->GetTransform()->GetRotation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}
	else  if (pSurface->GetType() == TYPE_BREP_OCC_VERTEX)
	{
		Shape_VERTEX_OCC* pEntity = (Shape_VERTEX_OCC*)pSurface;
		TopoDS_Vertex aF = TopoDS::Vertex(pEntity->shape);
		Shape_OCC* pOCC = (Shape_OCC*)(pEntity->pParentObject);

		Standard_Real first, last;
		gp_Pnt point = BRep_Tool::Pnt(aF);
		char message[500];

		//auto type = curve->DynamicType()->SystemName();
		sprintf(message, "Vertex %f,%f,%f\r\n", point.X(), point.Y(), point.Z());
		m_surfaceinformation = m_surfaceinformation + message;

		// 定义变量来存储属性
		//GProp_GProps props;
		// 计算面的属性
		//BRepGProp::SurfaceProperties(aF, props);

		//sprintf(message, "Surface Area: %f\r\n", pEntity->GetFaceArea());
		//m_surfaceinformation = m_surfaceinformation + message;

		sprintf(message, "Transformation\r\n Translation:%f,%f,%f \r\n", pEntity->GetTransform()->GetTranslation()->x(), pEntity->GetTransform()->GetTranslation()->y(), pEntity->GetTransform()->GetTranslation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Scale:%f,%f,%f \r\n", pEntity->GetTransform()->GetScale()->x(), pEntity->GetTransform()->GetScale()->y(), pEntity->GetTransform()->GetScale()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		sprintf(message, "Rotation:%f,%f,%f \r\n", pEntity->GetTransform()->GetRotation()->x(), pEntity->GetTransform()->GetRotation()->y(), pEntity->GetTransform()->GetRotation()->z());
		m_surfaceinformation = m_surfaceinformation + message;
		UpdateData(FALSE);
	}

}
