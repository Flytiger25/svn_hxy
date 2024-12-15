#include "stdafx.h"
#include "NurbsCurve.h"
#include "Curve.h"
#include "Lib3D/Base3d.h"
#include <atlimage.h>
#include "ReferenceLine.h"
CNurbsCurve::CNurbsCurve(void)
{
	c_r = 0.0;
	c_g = 0.0;
	c_b = 0.0;
	m_showControlPoints = 0;
	m_showReferenceLines = 1;
	m_showLineSegments = 1;
	type = TYPE_NURBSCURVE3D;
	filename = "NURBS Curve";
	m_showSDLEnergy = 0;
}

CNurbsCurve::~CNurbsCurve(void)
{
}

void CNurbsCurve::glDrawReferenceLines()
{
	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	int width = m_TextureImage.GetWidth();
	int size = m_ArrayReferenceLines.size();
	double x;
	const int REFLINE_SEGMENTS = 100;
	for (int i = 0; i < size; i++)
	{
		x = (double)i * width / (size - 1);
		if (x == m_TextureImage.GetWidth())
			x--;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);
		double r = GetRValue(color) / 256.0;
		double g = GetGValue(color) / 256.0;
		double b = GetBValue(color) / 256.0;
		::glColor3f(r, g, b);
		::glBegin(GL_LINES);
		Point3D p1 = m_ArrayReferenceLines[i]->start;
		Point3D p2 = m_ArrayReferenceLines[i]->end;

		Point3D start, end;
		double t0, t1;
		//此处用虚线画referece line
		for (int j = 0; j < REFLINE_SEGMENTS; j = j + 2)
		{
			t0 = (double)j / REFLINE_SEGMENTS;
			t1 = (double)(j + 1) / REFLINE_SEGMENTS;
			start = p1 * (1 - t0) + p2 * t0;
			end = p1 * (1 - t1) + p2 * t1;

			::glVertex3f(start.x, start.y, start.z);
			::glVertex3f(end.x, end.y, end.z);
			//::glVertex3f(p1.x, p1.y, p1.z);
			//::glVertex3f(p2.x, p2.y, p2.z);
		}
		::glEnd();
	}
}

void CNurbsCurve::glDrawLineSegments()
{
	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	int width = m_TextureImage.GetWidth();
	double x;

	if (!pCurve)
		return;

	int size = m_ArraySegmentParameters.size();
	for (int i = 0; i < size - 1; i++)
	{
		x = (double)i * width / (size - 2);
		if (x == m_TextureImage.GetWidth())
			x--;
		if (x < 0)
			x = 0;
		COLORREF color = m_TextureImage.GetPixel(x, 0);
		double r = GetRValue(color) / 256.0;
		double g = GetGValue(color) / 256.0;
		double b = GetBValue(color) / 256.0;
		::glColor3f(r, g, b);

		::glBegin(GL_LINES);
		Point3D p1 = pCurve->ComputPosition(m_ArraySegmentParameters[i]);
		Point3D p2 = pCurve->ComputPosition(m_ArraySegmentParameters[i + 1]);

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p2.z);
		::glEnd();
	}
}


void CNurbsCurve::glDrawControlPoints()
{
	::glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < pCurve->GetCtrlNumber() - 1; i++)
	{
		::glBegin(GL_LINES);
		Point3D p1 = pCurve->GetControlPoint(i);
		Point3D p2 = pCurve->GetControlPoint(i + 1);

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p1.z);
		::glEnd();
	}
	for (int i = 0; i < pCurve->GetCtrlNumber(); i++)
	{
		::glBegin(GL_POINTS);
		Point3D p1 = pCurve->GetControlPoint(i);
		::glVertex3f(p1.x, p1.y, p1.z);
		::glEnd();
	}
}

void CNurbsCurve::glDrawCurve()
{
	int sdlSize = arrayCurveTessellationColorBar_SDL.size();
	for (int i = 0; i < CURVE_DISCRETE_NUM - 1; i++)
	{
		::glBegin(GL_LINES);
		Point3D p1 = pCurve->ComputPosition((double)i / CURVE_DISCRETE_NUM);
		Point3D p2 = pCurve->ComputPosition((double)(i + 1) / CURVE_DISCRETE_NUM);

		if (m_showSDLEnergy && sdlSize > 0)
		{
			COLORREF color = arrayCurveTessellationColorBar_SDL[i];
			double r = GetRValue(color) / 256.0;
			double g = GetGValue(color) / 256.0;
			double b = GetBValue(color) / 256.0;
			::glColor3f(r, g, b);
		}
		::glVertex3f(p1.x, p1.y, p1.z);

		if (m_showSDLEnergy && sdlSize > 0)
		{
			COLORREF color = arrayCurveTessellationColorBar_SDL[i + 1];
			double r = GetRValue(color) / 256.0;
			double g = GetGValue(color) / 256.0;
			double b = GetBValue(color) / 256.0;
			::glColor3f(r, g, b);
		}

		::glVertex3f(p2.x, p2.y, p1.z);
		::glEnd();
	}

}
int CNurbsCurve::glDraw()
{
	/*	if (!m_Show)
			return 0;

		::glPushMatrix();
		glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
		glRotatef(m_xRotation, 1.0, 0.0, 0.0);
		glRotatef(m_yRotation, 0.0, 1.0, 0.0);
		glRotatef(m_zRotation, 0.0, 0.0, 1.0);
		glScalef(m_xScaling, m_yScaling, m_zScaling);

		//
		glEnable(GL_POINT_SMOOTH); // 启用反走样
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND); // 启动融合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
		glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度

		::glPushMatrix();

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

		glColor3f(c_r, c_g, c_b);

		::glDisable(GL_LIGHTING);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(2.0f);

		int DISCRETE_NUM = 100;
		::glBegin(GL_LINES);
		for (int i = 0; i < DISCRETE_NUM; i++)
		{

			Point3D p1 = pCurve->ComputPosition((double)i / DISCRETE_NUM);
			Point3D p2 = pCurve->ComputPosition((double)(i + 1) / DISCRETE_NUM);

			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p1.z);
		}
		::glEnd();
		if (m_showControlPoints)
			glDrawControlPoints();

		::glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		::glPopMatrix();

		glDisable(GL_MAP2_VERTEX_4);
		glDisable(GL_MAP2_TEXTURE_COORD_2);
		glDisable(GL_TEXTURE_2D);
		::glPopMatrix(); */
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
	//if (m_ShowTexture)
	//{
	//	m_pDib = new CDib(m_FileTextureName);
	//	pTextureBits = (GLubyte*)m_pDib->GetDibBitsPtr();
	//	width = m_pDib->GetDibWidth();
	//	height = m_pDib->GetDibHeight();
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTextureBits);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//}
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
	//if (m_ShowTexture)
	{
		//glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, (const float*)textureCntrlPnt);
	//	glEnable(GL_TEXTURE_2D);
		//glEnable(GL_MAP2_TEXTURE_COORD_2);
	}
	//-----------End Texture--------------

	//::glDisable(GL_LIGHTING);
	glEnable(GL_MAP2_VERTEX_4);
	//glColor3f(0.0,0.0,1.0);

	glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
	::glPushMatrix();

	//Show the Coordinate Axis
	//if (m_ShowCoordinateAxis)
	//{
	//	DrawCoordinateAxis();
	//}

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
		//	::glEnable(GL_LIGHTING);
		//else
		::glDisable(GL_LIGHTING);

		glColor3f(c_r, c_g, c_b);
		//glColor3f(1.0, 0.0, 0.0);
		{
			//::glDisable(GL_LIGHTING);
			//glDrawControlPoints();
			glDrawCurve();

			if (m_showControlPoints)
				glDrawControlPoints();

			if (m_showReferenceLines)
				glDrawReferenceLines();

			if (m_showLineSegments)
				glDrawLineSegments();
			//::glEnable(GL_LIGHTING);
		}

		glLineWidth(2.0f);
	}


	::glPopMatrix();

	::glPopMatrix();
	glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);
	return 1;
}