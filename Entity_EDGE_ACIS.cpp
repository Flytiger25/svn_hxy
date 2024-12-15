#include "stdafx.h"
#include "Entity_EDGE_ACIS.h"
#include <lib3d/Mesh3d.h>
#include <lib3d/Array3d.h>
#include <vector>
#include <GL/gl.h>
#include <lib3d/Base3d.h>
#include <kernapi.hxx>
#include "edge.hxx"
#include "vertex.hxx"
#include "straight.hxx"
#include "point.hxx"
#include "boolapi.hxx"
#include "face.hxx"
#include "surface.hxx"
#include <spline.hxx>
#include <bs3surf.hxx>
#include <srf_def.hxx>
#include "acistol.hxx"
#include "box.hxx"
#include "bs3c_def.hxx"
#include "bs3curve.hxx"
#include "bs3surf.hxx"
#include "condef.hxx"
#include "elldef.hxx"
#include "geom_utl.hxx"
#include "law_util.hxx"
#include "param.hxx"
#include "pladef.hxx"
#include "sp3crtn.hxx"
#include "sphdef.hxx"
#include "spldef.hxx"
#include "sps3crtn.hxx"
#include "sps3srtn.hxx"
#include "srf_def.hxx"
#include "strdef.hxx"
#include "tordef.hxx"
#include "vector_utils.hxx"
#include <vector>
#include <sps3srtn.hxx>
#include <cstrapi.hxx>
#include "SDIViewSwitch.h"
#include "intdef.hxx"
#include "sps3crtn.hxx"
#include "EntityList_ACIS.h"

Entity_EDGE_ACIS::Entity_EDGE_ACIS()
{
	type = TYPE_BREP_ACIS_EDGE;
	color.Set(128, 128, 128);
	m_showControlPointsNetwork = FALSE;
}
Entity_EDGE_ACIS::~Entity_EDGE_ACIS()
{

}

void Entity_EDGE_ACIS::glRenderEdges()
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

	::glBegin(GL_LINES);
	int edgepointsize = edgepoints.size();
	for (int i = 0; i < edgepointsize - 1; i++)
	{
		SPAposition p1 = edgepoints[i];
		SPAposition p2 = edgepoints[i + 1];

		::glVertex3f(p1.x(), p1.y(), p1.z());
		::glVertex3f(p2.x(), p2.y(), p2.z());
	}
	::glEnd();

}
void Entity_EDGE_ACIS::glRenderControlPoints()
{
	if (!pAcisEntity)
		return;
	const curve* curve = &(pAcisEntity->geometry()->equation());

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


		for (int i = 0; i < num_ctrlpts - 1; i++)
		{
			SPAposition p1 = ctrlpts[i];
			SPAposition p2 = ctrlpts[i + 1];

			::glVertex3f(p1.x(), p1.y(), p1.z());
			::glVertex3f(p2.x(), p2.y(), p2.z());
		}
		::glEnd();

		::glColor3f(0.0, 0.0, 0.0);
		::glPointSize(10.0);
		::glBegin(GL_POINTS);

		for (int i = 0; i < num_ctrlpts; i++)
		{
			SPAposition p1 = ctrlpts[i];
			::glVertex3f(p1.x(), p1.y(), p1.z());
		}
		::glEnd();
	}
}
int Entity_EDGE_ACIS::glDraw()
{
	if (pParentObject && pParentObject->m_Show == 0)
		return 0;

	if (!m_Show)
		return 0;

	if (pParentObject)
	{
		if (((EntityList_ACIS*)pParentObject)->m_showEdge == 0)
			return 0;
	}
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

int Entity_EDGE_ACIS::PickTest(PickStruct& pickstruct)
{
	return 1;
}