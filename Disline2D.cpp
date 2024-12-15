#include "stdafx.h"
#include "Disline2D.h"

CDisline2D::CDisline2D(void)
{
	m_Show  = 1;
	c_r = c_g = c_b = 0;
}

CDisline2D::~CDisline2D(void)
{
}
int CDisline2D::glDraw()
{
	if(!m_Show)
		return 0;
	
	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 

	::glPushMatrix();

	if(!m_SetCameraMode)
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
	glGetIntegerv(GL_VIEWPORT,m_viewPort);
	
	glColor3f(c_r,c_g,c_b);

	::glDisable(GL_LIGHTING);
	glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable (GL_LINE_STIPPLE);
	glLineWidth(2.0f);

	for(int i = 0; i < pDisline2D->GetDispointNum() - 1; i++)
	{
		::glBegin(GL_LINES);
		Point2D p1 = pDisline2D->GetDispoint(i)->GetPoint();
		Point2D p2 = pDisline2D->GetDispoint(i + 1)->GetPoint();
		::glVertex3f(p1.x,p1.y,0);
		::glVertex3f(p2.x,p2.y,0);
		::glEnd();
	}
	glPointSize(5.0);
	for(int i = 0; i < pDisline2D->GetDispointNum(); i++)
	{
		::glBegin(GL_POINTS);
		Point2D p1 = pDisline2D->GetDispoint(i)->GetPoint();
		::glVertex3f(p1.x,p1.y,0);
		::glEnd();
	}
	::glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST); 
	::glPopMatrix();

	glDisable( GL_MAP2_VERTEX_4 );
	glDisable( GL_MAP2_TEXTURE_COORD_2 );
	glDisable( GL_TEXTURE_2D );

	return 1;
}