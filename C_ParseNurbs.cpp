#include "stdafx.h"
#include "lib3d\Base3d.h"
#include <fstream>
#include "Nurbs3D.h"
#include "C_ParseNurbs.h"
CParseNurbs::CParseNurbs()
{

}
CParseNurbs::~CParseNurbs()
{
	return;
}
int CParseNurbs::Run(char* filename, CSceneGraph3d* pSceneGraph)
{
	char s[100];
	long m_VertexNumber, m_FaceNumber; //顶点数目和面的数目
	float m_x, m_y, m_z;  //顶点的三个坐标值
	long m_index1, m_index2, m_index3;  //面的顶点序号
	int	 m_degreex, m_degreey;
	int  m_controlx, m_controly;

	std::ifstream file(filename);
	//跳过一行
//	file.getline(s, 100);
	//读入顶点数和面数
	file >> m_controlx >> m_controly;
	CNurbsSuface* pSurf = new CNurbsSuface();
	pSurf->filename = filename;
	file >> pSurf->m_udegree >> pSurf->m_vdegree;

	pSurf->m_uknot = new GLfloat[m_controlx + pSurf->m_udegree + 1];
	pSurf->m_vknot = new GLfloat[m_controly + pSurf->m_vdegree + 1];
	pSurf->m_uknotcount = m_controlx + pSurf->m_udegree + 1;
	pSurf->m_vknotcount = m_controly + pSurf->m_vdegree + 1;
	for (int i = 0; i < m_controlx + pSurf->m_udegree + 1; i++)
	{
		file >> pSurf->m_uknot[i];
	}
	for (int i = 0; i < m_controly + pSurf->m_vdegree + 1; i++)
	{
		file >> pSurf->m_vknot[i];
	}
	double temp;
	for (int i = 0; i < m_controlx; i++)
		for (int j = 0; j < m_controly; j++)
		{
			file >> pSurf->m_ControlPoint[i][j][0] >> pSurf->m_ControlPoint[i][j][1];
			file >> pSurf->m_ControlPoint[i][j][2] >> pSurf->m_ControlPoint[i][j][3];
			pSurf->m_ControlPoint[i][j][0] *= pSurf->m_ControlPoint[i][j][3];
			pSurf->m_ControlPoint[i][j][1] *= pSurf->m_ControlPoint[i][j][3];
			pSurf->m_ControlPoint[i][j][2] *= pSurf->m_ControlPoint[i][j][3];
		}


	//Get Bounding box
	Vector3D m_lower, m_upper;
	pSurf->GetBoundingBox(m_lower, m_upper);

	Vector3D m_center;
	m_center = m_lower + m_upper;
	m_center /= -2;

	float length = m_upper.x - m_lower.x;
	if (length < m_upper.y - m_lower.y)
		length = m_upper.y - m_lower.y;
	if (length < m_upper.z - m_lower.z)
		length = m_upper.z - m_lower.z;

	length = length / 2;

	CTransform transform;

	//translation
	CVector3d translation(m_center[0], m_center[1], m_center[2]);
	transform.SetTranslation(&translation);

	//scale
	CVector3d scale(1 / length, 1 / length, 1 / length);
	transform.SetScale(&scale);

	if (pSceneGraph->NbObject() >= 1)
	{
		if (pSceneGraph->GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pSurf->SetTransform(*pSurface->GetTransform());
		}
		else
			if (pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pSurf->SetTransform(*pSurface->GetTransform());
			}
	}
	else
		pSurf->SetTransform(transform);
	pSceneGraph->Add(pSurf);
	return 1;
}

