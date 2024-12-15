#include "stdafx.h"
#include "EntityList_ACIS.h"
#include "lib3d/Mesh3d.h"
#include "lib3d/Array3d.h"
#include <vector>
#include <GL/gl.h>
#include "lib3d/Base3d.h"
#include <intrapi.hxx>
#include "edge.hxx"
#include <mt_stitch_apis.hxx>
#include "FormView3.h"
#include "face.hxx"
#include "surface.hxx"
#include "surdef.hxx"
#include "box.hxx"
#include "vertex.hxx"
#include "point.hxx"
#include "edge.hxx"
#include "curve.hxx"
#include "curdef.hxx"

EntityList_ACIS::EntityList_ACIS()
{
	type = TYPE_BREP_ACIS;
	color.Set(128, 128, 128);
	pAcisEntities = NULL;
	cntnr = NULL;
	m_showSurface = 1;
	m_showFaceIndex = 0;
	m_showEdgeIndex = 0;
	m_showVertexIndex = 0;
	m_showSelectedEdge = 0;
	m_showGapEdge = 0;
	m_showEdge = 1;
	m_showVertex = 1;
}
EntityList_ACIS::~EntityList_ACIS()

{
	if (pAcisEntities)
	{
		ACIS_DELETE(pAcisEntities);
		pAcisEntities = NULL;
	}

}
void EntityList_ACIS::glShowSelectedEdges()
{
	if (!cntnr)
		return;

	int size = arraySeletedEdgePairIndexAarray.size();
	double dis;
	EDGE* edge0, * edge1;
	double maxdis = -1;
	int maxindex;

	if (m_IsSelected)
		glLineWidth(5.0f);
	else
		glLineWidth(2.0f);
	::glDisable(GL_LIGHTING);
	if (m_IsSelected)
		glColor3f(0, 0, 1);
	else
		glColor3f(0, 1, 0);
	if (m_IsSelected)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);

	int edgepairindex;
	for (int i = 0; i < size; i++)
	{
		edgepairindex = arraySeletedEdgePairIndexAarray[i];
		cntnr->get_edge_pair(edgepairindex, edge0, edge1);
		dis = cntnr->get_hausdorff_distance(edgepairindex);

		//if (dis < 1e-8)
		//	continue;

		SPAposition pos = edge0->mid_pos();
		char message[100];
		sprintf(message, "%f", dis);
		p_FormView3->drawOpenglString(pos.x(), pos.y(), pos.z(), message);

		//plot the points on the two edges
		const int theTessellationPoints = 10;
		SPAparameter startparam = edge0->start_param();
		SPAparameter endparam = edge0->end_param();

		::glPointSize(10);
		glColor3f(0, 0, 1);
		::glBegin(GL_POINTS);
		double ratio;
		for (int i = 0; i <= theTessellationPoints; i++)
		{
			ratio = (double)i / theTessellationPoints;

			SPAparameter para = startparam * (1 - ratio) + endparam * ratio;
			edge0->geometry()->equation().evaluate(DOUBLE(para), pos);
			::glVertex3f(pos.x(), pos.y(), pos.z());
		}
		startparam = edge1->start_param();
		endparam = edge1->end_param();

		glColor3f(0, 1, 1);
		::glBegin(GL_POINTS);
		for (int i = 0; i <= theTessellationPoints; i++)
		{
			ratio = (double)i / theTessellationPoints;

			SPAparameter para = startparam * (1 - ratio) + endparam * ratio;
			edge1->geometry()->equation().evaluate(DOUBLE(para), pos);
			::glVertex3f(pos.x(), pos.y(), pos.z());
		}
		::glEnd();
	}


}
void EntityList_ACIS::glShowHausdorffDistance()
{
	if (!cntnr)
		return;

	int size = cntnr->size();
	double dis;
	EDGE* edge0, * edge1;
	double maxdis = -1;
	int maxindex;

	if (m_IsSelected)
		glLineWidth(5.0f);
	else
		glLineWidth(2.0f);
	::glDisable(GL_LIGHTING);
	if (m_IsSelected)
		glColor3f(0, 0, 1);
	else
		glColor3f(0, 1, 0);
	if (m_IsSelected)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);


	for (int i = 0; i < size; i++)
	{
		cntnr->get_edge_pair(i, edge0, edge1);
		dis = cntnr->get_hausdorff_distance(i);

		if (dis < 1e-8)
			continue;

		SPAposition pos = edge0->mid_pos();
		char message[100];
		sprintf(message, "%f", dis);
		p_FormView3->drawOpenglString(pos.x(), pos.y(), pos.z(), message);
	}
}

//********************************************
// glDraw
//********************************************

void EntityList_ACIS::glRenderTriangulation()

{
	//2024-7-12 yyj
	//这是一个面的集合，不显示
	return;

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

void EntityList_ACIS::BuildTriangulationList()

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
	// Position / translation / scaling
	/*glScalef(m_Transform.GetScale()->x(),
		m_Transform.GetScale()->y(),
		m_Transform.GetScale()->z());

	glRotatef(m_Transform.GetValueRotation(),
		m_Transform.GetRotation()->x(),
		m_Transform.GetRotation()->y(),
		m_Transform.GetRotation()->z());

	glTranslatef(m_Transform.GetTranslation()->x(),
		m_Transform.GetTranslation()->y(),
		m_Transform.GetTranslation()->z());
		*/
		//std::vector<SVNMesh::FaceMesh> faceData;
		//for (auto face : faceData)
		//{
		//	face.di
		//}
	int nTriangles = triangles.size() / 3;
	int n1, n2, n3; //vertex indices
	::glBegin(GL_TRIANGLES);
	for (int i = 0; i < nTriangles; i++)
	{
		n1 = triangles[i * 3];
		n2 = triangles[i * 3 + 1];
		n3 = triangles[i * 3 + 2];
		::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
		::glVertex3f(coords[n1 * 3], coords[n1 * 3 + 1], coords[n1 * 3 + 2]);

		::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
		::glVertex3f(coords[n2 * 3], coords[n2 * 3 + 1], coords[n2 * 3 + 2]);

		::glNormal3f(normalCoords[n3 * 3], normalCoords[n3 * 3 + 1], normalCoords[n3 * 3 + 2]);
		::glVertex3f(coords[n3 * 3], coords[n3 * 3 + 1], coords[n3 * 3 + 2]);
	}
	::glEnd();
	::glEndList();

	m_ListDone = 1;
	m_Modified = 0;
}
int EntityList_ACIS::PickTest(PickStruct& pickstruct)
{
	return -1;
}
void EntityList_ACIS::ComputeBoundingBox(Vector3D& lower, Vector3D& upper)

{
	SPAposition minpt, maxpt;
	api_get_entity_box(*pAcisEntities, minpt, maxpt);
	lower.SetValue(minpt.x(), minpt.y(), minpt.z());
	upper.SetValue(maxpt.x(), maxpt.y(), maxpt.z());
}
void EntityList_ACIS::glDrawEdgeIndex()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayACISEdges.size();
	gp_Pnt p1;
	TopLoc_Location loc;
	Standard_Real first, last;
	for (int i = 0; i < size; i++)
	{
		EDGE* pEdge = arrayACISEdges[i];

		SPAposition position = pEdge->mid_pos();

		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(position.x(), position.y(), position.z(), message);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
void EntityList_ACIS::glDrawVertexIndex()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayACISVertices.size();
	SPAposition position;
	for (int i = 0; i < size; i++)
	{
		VERTEX* pVertex = arrayACISVertices[i];
		position = pVertex->geometry()->coords();
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(position.x(), position.y(), position.z(), message);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
void EntityList_ACIS::glDrawFaceIndex()
{

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	char message[100];

	int size = arrayACISFaces.size();
	for (int i = 0; i < size; i++)
	{
		FACE* pFace = arrayACISFaces[i];

		SPApar_box* uv_bound = pFace->uv_bound();
		SPApar_pos pos;
		SPAposition position;
		if (uv_bound)
		{
			pos.u = uv_bound->u_range().interpolate(0.5);
			pos.v = uv_bound->v_range().interpolate(0.5);

			position = pFace->geometry()->equation().eval_position(pos);
		}
		else
		{
			SPAbox* box = pFace->bound();

			position = box->mid();
		}

		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(position.x(), position.y(), position.z(), message);
	}
	glEnable(GL_DEPTH_TEST);
}
int EntityList_ACIS::glDraw()
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
			glRenderTriangulation();
			//::glEnable(GL_LIGHTING);
		}

		glLineWidth(2.0f);
	}
	if (m_showGapEdge)
		glShowHausdorffDistance();

	if (m_showSelectedEdge)
		glShowSelectedEdges();

	if (m_showFaceIndex)
		glDrawFaceIndex();

	if (m_showEdgeIndex)
		glDrawEdgeIndex();

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
