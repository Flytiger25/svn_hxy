#include "stdafx.h"
#include "Lib3D/Base3d.h"
#include "Shape_EDGE_OCC.h"
#include <TopoDS.hxx>
#include<TopoDS_Edge.hxx>
#include "TColgp_Array1OfPnt.hxx"
#include "BRep_TEdge.hxx"
#include <BRep_ListIteratorOfListOfCurveRepresentation.hxx>
#include <BRepTools.hxx>
#include "BRep_Curve3D.hxx"
#include <curve.hxx>
#include "BRep_Tool.hxx"
#include "Geom_BSplineCurve.hxx"
#include <TShape_OCC.h>
Shape_EDGE_OCC::Shape_EDGE_OCC()
{
	type = TYPE_BREP_OCC_EDGE;
	color.Set(128, 128, 128);
	m_showControlPointsNetwork = 0;
}
Shape_EDGE_OCC::~Shape_EDGE_OCC()
{

}
void Shape_EDGE_OCC::glRenderControlPoints()
{
	if (m_IsSelected)
		glLineWidth(5.0f);
	else
		glLineWidth(2.0f);
	::glDisable(GL_LIGHTING);
	//if (m_IsSelected)
	//	glColor3f(0, 0, 1);
	//else
	//	glColor3f(0, 1, 0);
	glColor3f(0, 0, 1);

	//if (m_IsSelected)
	glDisable(GL_DEPTH_TEST);
	//else
	//	glEnable(GL_DEPTH_TEST);

	::glBegin(GL_LINES);

	TopoDS_Edge edge = TopoDS::Edge(shape);

	TopLoc_Location loc;
	Standard_Real first, last;

	Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, loc, first, last);
	Handle(Geom_BSplineCurve) bCurve1 = Handle(Geom_BSplineCurve)::DownCast(gcurve);
	Standard_Integer nbp1 = bCurve1->NbPoles();
	TColgp_Array1OfPnt P1(1, nbp1);
	bCurve1->Poles(P1);
	for (int i = 1; i < nbp1; i++)
	{
		gp_Pnt p1 = P1[i];
		gp_Pnt p2 = P1[i + 1];

		::glVertex3f(p1.X(), p1.Y(), p1.Z());
		::glVertex3f(p2.X(), p2.Y(), p2.Z());
	}
	::glEnd();
}
void Shape_EDGE_OCC::glRenderEdges()
{
	if (m_IsSelected)
		glLineWidth(5.0f);
	else
		glLineWidth(2.0f);
	::glDisable(GL_LIGHTING);
	//if (m_IsSelected)
	//	glColor3f(0, 0, 1);
	//else
	//	glColor3f(0, 1, 0);
	glColor3f(0, 1, 0);

	if (m_IsSelected)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);

	int edgeSize = edgepoints.size();
	if (edgeSize == 0)
		return;

	::glBegin(GL_LINES);
	for (int i = 0; i < edgeSize - 1; i++)
	{
		gp_Pnt p1 = edgepoints[i];
		gp_Pnt p2 = edgepoints[i + 1];

		::glVertex3f(p1.X(), p1.Y(), p1.Z());
		::glVertex3f(p2.X(), p2.Y(), p2.Z());
	}
	::glEnd();

}

int Shape_EDGE_OCC::glDraw()
{
	if (pParentObject && pParentObject->m_Show == 0)
		return 0;

	if (pParentObject)
	{
		if (!((Shape_OCC*)pParentObject)->m_showEdge)
			return 0;
	}

	if (!m_Show)
		return 0;

	::glPushMatrix();
	//if (pParentObject->pParentObject)
	//{
	//	SetTransform(pParentObject->m_Transform);
	//}
	//else
	//{
	//	if (pDoc->m_SceneGraph.GetAt(1) != this)
	//	{
	//		SetTransform(pParentObject->m_Transform);
	//	}
	//	else
	//	{
	//		// 计算边界和中心点
	//		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	//		float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

	//		for (const auto& point : edgepoints)
	//		{
	//			if (point.x() < minX) minX = point.x();
	//			if (point.y() < minY) minY = point.y();
	//			if (point.z() < minZ) minZ = point.z();

	//			if (point.x() > maxX) maxX = point.x();
	//			if (point.y() > maxY) maxY = point.y();
	//			if (point.z() > maxZ) maxZ = point.z();
	//		}

	//		float centerX = (minX + maxX) / 2.0f;
	//		float centerY = (minY + maxY) / 2.0f;
	//		float centerZ = (minZ + maxZ) / 2.0f;

	//		float sizeX = maxX - minX;
	//		float sizeY = maxY - minY;
	//		float sizeZ = maxZ - minZ;

	//		// 设置平移
	//		float translationX = -centerX;
	//		float translationY = -centerY;
	//		float translationZ = -centerZ;

	//		// 设置缩放
	//		float scaleValue = 1.0f / std::max({ sizeX, sizeY, sizeZ });

	//		CVector3d translation(translationX, translationY, translationZ);
	//		CVector3d scale(scaleValue, scaleValue, scaleValue);
	//		m_Transform.SetScale(scale);
	//		m_Transform.SetTranslation(translation);
	//		pParentObject->SetTransform(m_Transform);
	//	}
	//}

	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);

	//------------
	// Material

	glEnable(GL_POINT_SMOOTH); // 启用反走样 
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 

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

	// draw edges
	glRenderEdges();

	if (m_showControlPointsNetwork)
		glRenderControlPoints();

	::glPopMatrix();

	//glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	return 1;
}

int Shape_EDGE_OCC::PickTest(PickStruct& pickstruct)
{
	return 1;
}