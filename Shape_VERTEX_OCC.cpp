#include "stdafx.h"
#include "Shape_VERTEX_OCC.h"
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
#include "TopoDS_Vertex.hxx"
#include <TShape_OCC.h>

Shape_VERTEX_OCC::Shape_VERTEX_OCC()
{
	type = TYPE_BREP_OCC_VERTEX;
	color.Set(128, 128, 128);
}
Shape_VERTEX_OCC::~Shape_VERTEX_OCC()
{

}
void Shape_VERTEX_OCC::glRenderVertex()
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
	glColor3f(0, 0, 0);

	if (m_IsSelected)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);

	::glPointSize(10.0);

	::glBegin(GL_POINTS);
	TopoDS_Vertex theVertex = TopoDS::Vertex(shape);
	gp_Pnt point = BRep_Tool::Pnt(theVertex);
	::glVertex3f(point.X(), point.Y(), point.Z());
	::glEnd();
}

int Shape_VERTEX_OCC::glDraw()
{
	if (pParentObject && pParentObject->m_Show == 0)
		return 0;

	if (pParentObject)
	{
		if (!((Shape_OCC*)pParentObject)->m_showVertex)
			return 0;
	}

	if (!m_Show)
		return 0;

	::glPushMatrix();


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
	glRenderVertex();

	::glPopMatrix();

	//glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	return 1;
}

int Shape_VERTEX_OCC::PickTest(PickStruct& pickstruct)
{
	return 1;
}