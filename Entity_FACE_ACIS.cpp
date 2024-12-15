#include "stdafx.h"
#include "Entity_FACE_ACIS.h"
#include "EntityList_ACIS.h"
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
Entity_FACE_ACIS::Entity_FACE_ACIS()
{
	type = TYPE_BREP_ACIS_FACE;
	m_showControlPointsNetwork = 0;
	m_showIsoParameterCurves = 0;
	m_showSurface = 1;
	color.Set(128, 128, 128);
}
Entity_FACE_ACIS::~Entity_FACE_ACIS()
{

}
//********************************************
// glDraw
//********************************************
void Entity_FACE_ACIS::glRenderTriangulation()
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
void Entity_FACE_ACIS::BuildTriangulationList()
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

	::glColor3ub(color.r(), color.g(), color.b());
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
void Entity_FACE_ACIS::glShowControlPointNetworks()
{
	glLineWidth(2.0f);
	glColor3f(1, 1, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//if ()
	SURFACE* pSurface = pAcisEntity->geometry();
	int identity = pSurface->identity();
	if (identity == PLANE_ID)
	{

	}
	else if (identity == SPLINE_ID)
	{
		SPLINE* surface = (SPLINE*)(pSurface);
		int dim = 0, form_u = 0, form_v = 0, pole_u = 0, pole_v = 0, num_u = 0, num_v = 0, degree_u = 0, num_uknots = 0, degree_v = 0, num_vknots = 0, rational_u = 0, rational_v = 0;
		::SPAposition* ctrlpts = nullptr;
		double* weights = nullptr, * uknots = nullptr, * vknots = nullptr;
		spline* splinesurface = (spline*)&(pAcisEntity->geometry()->equation());
		bs3_surface_to_array(splinesurface->sur(), dim, rational_u, rational_v, form_u, form_v, pole_u, pole_v, num_u, num_v, ctrlpts, weights, degree_u, num_uknots, uknots, degree_v, num_vknots, vknots, 0);
		//pAcisEntity->geometry()->equation().
		//bs3_surface_to_array()
		//	pAcisEntity->geometry()->equation()
		::glBegin(GL_LINES);
		for (int i = 0; i < num_u; i++)
			for (int j = 0; j < num_v; j++)
			{
				SPAposition  pt1 = ctrlpts[i * num_v + j];
				SPAposition pt2;
				SPAposition pt3;
				if (j < num_v - 1)
				{
					pt2 = ctrlpts[i * num_v + j + 1];

					//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
					::glVertex3f(pt1.x(), pt1.y(), pt1.z());

					//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
					::glVertex3f(pt2.x(), pt2.y(), pt2.z());
				}
				if (i < num_u - 1)
				{
					pt3 = ctrlpts[(i + 1) * num_v + j];
					//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
					::glVertex3f(pt1.x(), pt1.y(), pt1.z());

					//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
					::glVertex3f(pt3.x(), pt3.y(), pt3.z());
				}

			}
		::glEnd();
		//splinesurface
		//SPAinterval aui = pAcisEntity->geometry()->equation().param_range_u();
		SPAinterval avi = pAcisEntity->geometry()->equation().param_range_v();

		//std::vector sample_params{ SPApar_pos(spline_param.u, range_v.start_pt()), SPApar_pos(spline_param.u, range_v.end_pt()), SPApar_pos(range_u.start_pt(), spline_param.v), SPApar_pos(range_v.end_pt(), spline_param.v) };

		//bool on_boundary = false;
		//for (auto param : sample_params) {
		//	SPAposition pos;
		//	equ->eval(param, pos);
		/*const int U_NUM = 1E1, V_NUM = 1E1;

		SPApar_box par = surface->param_range();
		//double ua = surface->sur()->get_sur()->knot_start_u();


		SPAinterval aui = surface->subset_u_interval();
		SPAinterval avi = surface->subset_v_interval();*/
		/*int iso_NUM = 1E1;
		int dis = 100;
		double aua = aui.start_pt(), aub = aui.end_pt(), aud = (aub - aua) / iso_NUM;
		double ava = avi.start_pt(), avb = avi.end_pt(), avd = (avb - ava) / dis;
		::glBegin(GL_LINES);
		for (int i = 0; i <= iso_NUM; ++i)
		{
			for (int j = 0; j < dis; ++j)
			{
				SPApar_pos auv1(aua + i * aud, ava + j * avd);
				SPApar_pos auv2(aua + i * aud, ava + (j + 1) * avd);
				// 求参数处的点
				SPAposition pos1, pos2;
				pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
				pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
				::glVertex3f(pos1.x(), pos1.y(), pos1.z());

				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
				::glVertex3f(pos2.x(), pos2.y(), pos2.z());

			}
		}

		aud = (aub - aua) / dis;
		avd = (avb - ava) / iso_NUM;
		for (int i = 0; i <= iso_NUM; ++i)
		{
			for (int j = 0; j < dis; ++j)
			{
				SPApar_pos auv1(aua + j * aud, ava + i * avd);
				SPApar_pos auv2(aua + (j + 1) * aud, ava + i * avd);
				// 求参数处的点
				SPAposition pos1, pos2;
				pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
				pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
				::glVertex3f(pos1.x(), pos1.y(), pos1.z());

				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
				::glVertex3f(pos2.x(), pos2.y(), pos2.z());

			}
		}
		::glEnd();*/
	}
}
void Entity_FACE_ACIS::glShowIsoParameterCurves()
{
	glLineWidth(2.0f);
	glColor3f(0, 1, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	//if ()
	SURFACE* pSurface = pAcisEntity->geometry();
	int identity = pSurface->identity();
	if (identity == PLANE_ID)
	{

	}
	else if (identity == SPLINE_ID)
	{
		SPLINE* surface = (SPLINE*)(pSurface);
		SPAinterval aui = pAcisEntity->geometry()->equation().param_range_u();
		SPAinterval avi = pAcisEntity->geometry()->equation().param_range_v();

		//std::vector sample_params{ SPApar_pos(spline_param.u, range_v.start_pt()), SPApar_pos(spline_param.u, range_v.end_pt()), SPApar_pos(range_u.start_pt(), spline_param.v), SPApar_pos(range_v.end_pt(), spline_param.v) };

		//bool on_boundary = false;
		//for (auto param : sample_params) {
		//	SPAposition pos;
		//	equ->eval(param, pos);
		/*const int U_NUM = 1E1, V_NUM = 1E1;

		SPApar_box par = surface->param_range();
		//double ua = surface->sur()->get_sur()->knot_start_u();


		SPAinterval aui = surface->subset_u_interval();
		SPAinterval avi = surface->subset_v_interval();*/
		int iso_NUM = 1E1;
		int dis = 100;
		double aua = aui.start_pt(), aub = aui.end_pt(), aud = (aub - aua) / iso_NUM;
		double ava = avi.start_pt(), avb = avi.end_pt(), avd = (avb - ava) / dis;
		::glBegin(GL_LINES);
		for (int i = 0; i <= iso_NUM; ++i)
		{
			for (int j = 0; j < dis; ++j)
			{
				SPApar_pos auv1(aua + i * aud, ava + j * avd);
				SPApar_pos auv2(aua + i * aud, ava + (j + 1) * avd);
				// 求参数处的点
				SPAposition pos1, pos2;
				pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
				pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
				::glVertex3f(pos1.x(), pos1.y(), pos1.z());

				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
				::glVertex3f(pos2.x(), pos2.y(), pos2.z());

			}
		}

		aud = (aub - aua) / dis;
		avd = (avb - ava) / iso_NUM;
		for (int i = 0; i <= iso_NUM; ++i)
		{
			for (int j = 0; j < dis; ++j)
			{
				SPApar_pos auv1(aua + j * aud, ava + i * avd);
				SPApar_pos auv2(aua + (j + 1) * aud, ava + i * avd);
				// 求参数处的点
				SPAposition pos1, pos2;
				pAcisEntity->geometry()->equation().evaluate(auv1, pos1);
				pAcisEntity->geometry()->equation().evaluate(auv2, pos2);

				//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
				::glVertex3f(pos1.x(), pos1.y(), pos1.z());

				//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
				::glVertex3f(pos2.x(), pos2.y(), pos2.z());

			}
		}
		::glEnd();
	}
}
void Entity_FACE_ACIS::glRenderEdges()
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
	if (m_IsSelected)
		glDisable(GL_DEPTH_TEST);
	else
		glEnable(GL_DEPTH_TEST);
	int nTriangles = triangles.size() / 3;
	int n1, n2; //vertex indices
	::glBegin(GL_LINES);
	int edgesize = edgepoints.size();
	int edgepointsize;
	for (int i = 0; i < edgesize; i++)
	{
		edgepointsize = edgepoints[i].size();
		for (int j = 0; j < edgepointsize - 1; j++)
		{
			SPAposition p1 = edgepoints[i][j];
			SPAposition p2 = edgepoints[i][j + 1];

			//::glNormal3f(normalCoords[n1 * 3], normalCoords[n1 * 3 + 1], normalCoords[n1 * 3 + 2]);
			::glVertex3f(p1.x(), p1.y(), p1.z());

			//::glNormal3f(normalCoords[n2 * 3], normalCoords[n2 * 3 + 1], normalCoords[n2 * 3 + 2]);
			::glVertex3f(p2.x(), p2.y(), p2.z());
		}

	}
	::glEnd();

}
void Entity_FACE_ACIS::ComputeBoundingBox(Vector3D& lower, Vector3D& upper)
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

void create_point(const SPAposition& pos, APOINT*& pnt) {

	pnt = ACIS_NEW APOINT(pos);
}

/**
 * @brief 根据SPAposition创建一个VERTEX
 */
VERTEX* create_vertex(const SPAposition& pos) {
	APOINT* apt = nullptr;
	create_point(pos, apt);
	return ACIS_NEW VERTEX(apt);
}

/**
 * @brief 从两个端点创建直线edge
 */
EDGE* create_line_from_vertex(VERTEX* v1, VERTEX* v2) {
	return ACIS_NEW EDGE(v1, v2, ACIS_NEW STRAIGHT(v1->geometry()->coords(), normalise(v2->geometry()->coords() - v1->geometry()->coords())), 0);
}

void api_curve_line(const SPAposition& pt1, const SPAposition& pt2, EDGE*& line) {
	if (same_point(pt1, pt2)) {
		auto* ver = create_vertex(pt1);
		line = ACIS_NEW EDGE(ver, ver, nullptr, 0);
	}
	else {
		auto* v1 = create_vertex(pt1);
		auto* v2 = create_vertex(pt2);
		line = create_line_from_vertex(v1, v2);
	}
}
int Entity_FACE_ACIS::PickTest(PickStruct& pickstruct)
{
	//
	SPAposition p1(pickstruct.point.x, pickstruct.point.y, pickstruct.point.z);
	SPAposition p2(pickstruct.point.x + pickstruct.dir.x, pickstruct.point.y + +pickstruct.dir.y, pickstruct.point.z + +pickstruct.dir.z);
	//SPAposition pos1(0.0, 0.0, 0.0);
	//SPAunit_vector gme_uvec(0.0, 0.0, 1.0);
	//GME::plane gme_pla(gme_pos1, gme_uvec);
	EDGE* st1 = nullptr;
	//GME::FACE* gme_st2 = nullptr;

	/* 调用API */
	// __GME_API_BEGIN
	api_curve_line(p1, p2, st1);
	//GME::check_outcome(GME::api_face_plane(gme_pos1, 5, 5, &gme_uvec, gme_st2));

	ENTITY_LIST* entity_list = ACIS_NEW ENTITY_LIST();
	api_edfa_int(st1, pAcisEntity, entity_list);
	if (entity_list->count() > 0)
	{
		for (auto ent : *entity_list)
		{
			const char* tn = ent->type_name();

			if (strcmp(tn, "vertex") == 0)
			{
				VERTEX* ptrVertex = (VERTEX*)ent;
				SPAposition pos = ptrVertex->geometry()->coords();

				Point3D pos1(pos.x(), pos.y(), pos.z());
				double depth = (pos1 - pickstruct.point) * pickstruct.dir / (pickstruct.dir * pickstruct.dir);
				if (depth < pickstruct.mindepth)
				{
					pickstruct.mindepth = depth;
					pickstruct.m_SelectIndex = m_ScenegraphIndex;
				}
			}
		}
		return 1;
	}
	else
		return 0;
}
int Entity_FACE_ACIS::glDraw()
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


		//glRenderEdges();
	}
	if (m_showIsoParameterCurves)
		glShowIsoParameterCurves();

	if (m_showControlPointsNetwork)
		glShowControlPointNetworks();

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