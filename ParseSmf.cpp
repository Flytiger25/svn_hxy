// ParseSmf.cpp: implementation of the CParseSmf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lib3d\Base3d.h"
#include <fstream>
#include "ParseSmf.h"
#include <vector>
#include "math.h"
#include "nurbs3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParseOBJ::CParseOBJ()
{

}
CParseOBJ::~CParseOBJ()
{
	Free();
}

void CParseOBJ::Free()
{
	return;
}
int CParseOBJ::Run(char* filename, CSceneGraph3d* pSceneGraph)
{
	char s[100];
	long m_VertexNumber, m_FaceNumber; //顶点数目和面的数目
	float m_x, m_y, m_z;  //顶点的三个坐标值
	long m_index1, m_index2, m_index3;  //面的顶点序号
	std::ifstream file(filename);

	float u, v;
	std::vector<CVertex3d*> m_ArrayVertex;
	std::vector<CVertex3d*> m_ArrayVertexParameter;
	std::vector<CVertex3d*> m_ArrayVertexNormal;
	std::vector<CFace3d*>   m_ArrayFace;

	CVertex3d* pVertex;
	CFace3d* pFace;
	CColor color(40, 240, 120);
	//CColor color(255,0,0);

	int index1 = 0;
	int type = -1;
	BOOL hasvt = FALSE;
	BOOL hasvn = FALSE;

	while (!file.eof())
	{
		file >> s;
		if (strcmp(s, "v") == 0)
		{
			//vertex coordinates

			if (type == 1)
				index1++;
			else
				index1 = 0;

			file >> m_x >> m_y >> m_z;
			pVertex = new CVertex3d(m_x, m_y, m_z);
			m_ArrayVertex.push_back(pVertex);
			pVertex->SetColor(color);
			type = 1;
		}
		else
			if (strcmp(s, "vt") == 0)
			{
				//vertex parameter domain
				if (type == 2)
					index1++;
				else
					index1 = 0;

				file >> u >> v;
				//m_ArrayVertex[index1]->SetParameter(u,v);

				type = 2;
				hasvt = TRUE;
			}
			else
				if (strcmp(s, "vn") == 0)
				{
					//vertex normal
					if (type == 3)
						index1++;
					else
						index1 = 0;

					file >> m_x >> m_y >> m_z;

					//m_ArrayVertex[index1]->SetNormal(m_x,m_y,m_z);

					type = 3;
					hasvn = TRUE;
				}
				else
					if (strcmp(s, "f") == 0)
					{
						//face
						if (type == 4)
							index1++;
						else
							index1 = 0;
						int m_index1parameter, m_index1normal;
						int m_index2parameter, m_index2normal;
						int m_index3parameter, m_index3normal;
						char c;

						file >> m_index1;
						if (hasvt)
						{
							file >> c;
							file >> m_index1parameter;
						}
						if (hasvn)
						{
							file >> c;
							file >> m_index1normal;
						}


						file >> m_index2;
						if (hasvt)
						{
							file >> c;
							file >> m_index2parameter;
						}
						if (hasvn)
						{
							file >> c;
							file >> m_index2normal;
						}


						file >> m_index3;
						if (hasvt)
						{
							file >> c;
							file >> m_index3parameter;
						}
						if (hasvn)
						{
							file >> c;
							file >> m_index3normal;
						}

						pFace = new CFace3d(m_ArrayVertex[m_index1 - 1], m_ArrayVertex[m_index2 - 1], m_ArrayVertex[m_index3 - 1]);
						pFace->SetColor(color);
						m_ArrayFace.push_back(pFace);
						type = 4;
					}
	}

	//set the vertex array
	CMesh3d* pMesh = new CMesh3d;
	// Set Size (faster)
	pMesh->m_ArrayVertex.SetSize(m_ArrayVertex.size());
	pMesh->m_ArrayFace.SetSize(m_ArrayFace.size());

	pMesh->m_SetVertexNormal = hasvn;

	//Set the vertex pointer.
	for (int i = 0; i < m_ArrayVertex.size(); i++)
	{
		pMesh->m_ArrayVertex.SetAt(i, m_ArrayVertex[i]);
	}

	//Set the Face pointer
	for (int i = 0; i < m_ArrayFace.size(); i++)
	{
		pMesh->m_ArrayFace.SetAt(i, m_ArrayFace[i]);
	}

	if (m_ArrayVertex.size() == 0)
		return 1;

	//bounding box
	CVector3d m_lower, m_upper;
	m_lower.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	m_upper.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	for (int i = 0; i < m_ArrayVertex.size(); i++)
	{
		m_x = m_ArrayVertex[i]->x();
		m_y = m_ArrayVertex[i]->y();
		m_z = m_ArrayVertex[i]->z();

		if (m_x < m_lower.x())
			m_lower.x(m_x);
		if (m_y < m_lower.y())
			m_lower.y(m_y);
		if (m_z < m_lower.z())
			m_lower.z(m_z);

		if (m_x > m_upper.x())
			m_upper.x(m_x);
		if (m_y > m_upper.y())
			m_upper.y(m_y);
		if (m_z > m_upper.z())
			m_upper.z(m_z);
	}
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	float length = m_upper.x() - m_lower.x();
	if (length < m_upper.y() - m_lower.y())
		length = m_upper.y() - m_lower.y();
	if (length < m_upper.z() - m_lower.z())
		length = m_upper.z() - m_lower.z();

	transform.SetTranslation(&m_translation);
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);
	pMesh->SetTransform(transform);
	pMesh->SetFlagOnFaces(0);
	pMesh->SetColor(color);
	pMesh->SetColorBinding(COLOR_PER_VERTEX);
	pSceneGraph->Add(pMesh);
	file.close();
	return 1;
}

int CParseOBJ::Run2(char* filename, CSceneGraph3d* pSceneGraph)
{
	char s[1000];
	long m_VertexNumber, m_FaceNumber; //顶点数目和面的数目
	float m_x, m_y, m_z;  //顶点的三个坐标值
	long m_index1, m_index2, m_index3;  //面的顶点序号
	std::ifstream file(filename);

	float u, v;
	std::vector<CVertex3d*> m_ArrayVertex;
	std::vector<Point2D > m_ArrayVertexParameter;
	std::vector<Point3D > m_ArrayVertexNormal;
	std::vector<CFace3d*>   m_ArrayFace;

	CVertex3d* pVertex;
	CFace3d* pFace;
	//CColor color(40, 240, 120);
	//CColor color(255, 0, 0);
	CColor color(128, 128, 128);
	BOOL hasvt = FALSE;
	BOOL hasvn = FALSE;

	int i = 0;
	while (!file.eof())
	{
		file >> s;
		if (strlen(s) == 0)
			break;
		if (strcmp(s, "v") == 0)
		{
			//vertex coordinates

			file >> m_x >> m_y >> m_z;
			pVertex = new CVertex3d(m_x, m_y, m_z);
			m_ArrayVertex.push_back(pVertex);
			pVertex->SetColor(color);
		}
		else
			if (strcmp(s, "vt") == 0)
			{
				//vertex parameter domain
				file >> u >> v;
				m_ArrayVertexParameter.push_back(Point2D(u, v));
				//m_ArrayVertex[index1]->SetParameter(u,v);

				hasvt = TRUE;
			}
			else
				if (strcmp(s, "vn") == 0)
				{
					i++;
					file >> m_x >> m_y >> m_z;

					m_ArrayVertexNormal.push_back(Point3D(m_x, m_y, m_z));
					//		m_ArrayVertex[index1]->SetNormal(m_x,m_y,m_z);

					hasvn = TRUE;
				}
				else
					if (strcmp(s, "f") == 0)
					{
						//face
						int m_index1parameter, m_index1normal;
						int m_index2parameter, m_index2normal;
						int m_index3parameter, m_index3normal;
						char c;

						file >> m_index1;

						if (hasvt)
						{
							file >> c;
							file >> m_index1parameter;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}

						if (hasvn)
						{
							file >> c;
							file >> m_index1normal;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}


						file >> m_index2;
						if (hasvt)
						{
							file >> c;
							file >> m_index2parameter;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}
						if (hasvn)
						{
							file >> c;
							file >> m_index2normal;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}


						file >> m_index3;
						if (hasvt)
						{
							file >> c;
							file >> m_index3parameter;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}
						if (hasvn)
						{
							file >> c;
							file >> m_index3normal;
						}
						else
						{
							file >> c;
							if (c != '/')
								file.unget();
						}

						pFace = new CFace3d(m_ArrayVertex[m_index1 - 1], m_ArrayVertex[m_index2 - 1], m_ArrayVertex[m_index3 - 1]);
						if (hasvn)
						{
							m_ArrayVertex[m_index1 - 1]->SetNormal(m_ArrayVertexNormal[m_index1normal - 1][0], m_ArrayVertexNormal[m_index1normal - 1][1], m_ArrayVertexNormal[m_index1normal - 1][2]);
							m_ArrayVertex[m_index2 - 1]->SetNormal(m_ArrayVertexNormal[m_index2normal - 1][0], m_ArrayVertexNormal[m_index2normal - 1][1], m_ArrayVertexNormal[m_index2normal - 1][2]);
							m_ArrayVertex[m_index3 - 1]->SetNormal(m_ArrayVertexNormal[m_index3normal - 1][0], m_ArrayVertexNormal[m_index3normal - 1][1], m_ArrayVertexNormal[m_index3normal - 1][2]);

						}

						if (hasvt)
						{
							m_ArrayVertex[m_index1 - 1]->SetParameter(m_ArrayVertexParameter[m_index1parameter - 1][0], m_ArrayVertexParameter[m_index1parameter - 1][1]);
							m_ArrayVertex[m_index2 - 1]->SetParameter(m_ArrayVertexParameter[m_index2parameter - 1][0], m_ArrayVertexParameter[m_index2parameter - 1][1]);
							m_ArrayVertex[m_index3 - 1]->SetParameter(m_ArrayVertexParameter[m_index3parameter - 1][0], m_ArrayVertexParameter[m_index3parameter - 1][1]);
						}

						pFace->SetColor(color);
						m_ArrayFace.push_back(pFace);
					}
	}


	/*for (int i = 0; i < (int) m_ArrayVertex.size(); ++ i)
	{
		if (i >= (int) m_ArrayVertexParameter.size() || i >= (int) m_ArrayVertexNormal.size())
		{
			break;
		}
		m_ArrayVertex[i]->SetParameter(m_ArrayVertexParameter[i][0], m_ArrayVertexParameter[i][1]);
		m_ArrayVertex[i]->SetNormal(m_ArrayVertexNormal[i][0], m_ArrayVertexNormal[i][1], m_ArrayVertexNormal[i][2]);
	}*/
	m_ArrayVertexParameter.clear();
	m_ArrayVertexNormal.clear();

	//compute a new mesh
	/*for(int i = 0; i < m_ArrayVertex.size(); i++)
	{
		m_ArrayVertex[i]->m_Flag = i;
	}
	std::vector<CFace3d*>   m_ArrayFace1;
	for(int i = 0; i < m_ArrayFace.size(); i++)
	{
		CFace3d* pFace1 = m_ArrayFace[i++];
		CFace3d* pFace2 = m_ArrayFace[i++];
		CFace3d* pFace3 = m_ArrayFace[i++];
		CFace3d* pFace4 = m_ArrayFace[i];
		int index1 = pFace1->v1()->m_Flag;
		int index2 = pFace1->v2()->m_Flag;
		int index3 = pFace1->v3()->m_Flag;

		int index4 = pFace3->v2()->m_Flag;
		int index5 = pFace3->v3()->m_Flag;

		CFace3d* pFace = new CFace3d(m_ArrayVertex[index2],m_ArrayVertex[index3],m_ArrayVertex[index4]);
		m_ArrayFace1.push_back(pFace);
		pFace = new CFace3d(m_ArrayVertex[index2],m_ArrayVertex[index4],m_ArrayVertex[index5]);
		m_ArrayFace1.push_back(pFace);
	}
	m_ArrayFace.clear();
	m_ArrayFace = m_ArrayFace1;
	m_ArrayFace1.clear();*/

	//set the vertex array
	CMesh3d* pMesh = new CMesh3d;
	if (hasvn)
	{
		pMesh->m_SetFaceNormal = TRUE;
		pMesh->m_SetVertexNormal = TRUE;
	}
	if (hasvt)
	{
		pMesh->hasvt = TRUE;
	}
	pMesh->filename = filename;
	// Set Size (faster)
	pMesh->m_ArrayVertex.SetSize(m_ArrayVertex.size());
	pMesh->m_ArrayFace.SetSize(m_ArrayFace.size());

	//Set the vertex pointer.
	for (int i = 0; i < m_ArrayVertex.size(); i++)
	{
		pMesh->m_ArrayVertex.SetAt(i, m_ArrayVertex[i]);
	}

	//Set the Face pointer
	for (int i = 0; i < m_ArrayFace.size(); i++)
	{
		pMesh->m_ArrayFace.SetAt(i, m_ArrayFace[i]);
	}

	if (m_ArrayVertex.size() == 0)
		return 1;


	//bounding box
	CVector3d m_lower, m_upper;
	m_lower.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	m_upper.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	for (int i = 0; i < m_ArrayVertex.size(); i++)
	{
		m_x = m_ArrayVertex[i]->x();
		m_y = m_ArrayVertex[i]->y();
		m_z = m_ArrayVertex[i]->z();

		if (m_x < m_lower.x())
			m_lower.x(m_x);
		if (m_y < m_lower.y())
			m_lower.y(m_y);
		if (m_z < m_lower.z())
			m_lower.z(m_z);

		if (m_x > m_upper.x())
			m_upper.x(m_x);
		if (m_y > m_upper.y())
			m_upper.y(m_y);
		if (m_z > m_upper.z())
			m_upper.z(m_z);
	}
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation = m_lower + m_upper;
	m_translation /= -2;
	float length = m_upper.x() - m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if (length < m_upper.y() - m_lower.y())
		length = m_upper.y() - m_lower.y();
	if (length < m_upper.z() - m_lower.z())
		length = m_upper.z() - m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1 / length, 1 / length, 1 / length);
	transform.SetScale(&m_Scale);

	if (pSceneGraph->NbObject() >= 1)
	{
		if (pSceneGraph->GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh->SetTransform(*pSurface->GetTransform());
		}
		else
			if (pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh->SetTransform(*pSurface->GetTransform());
			}
	}
	else
		pMesh->SetTransform(transform);

	pMesh->SetFlagOnFaces(0);
	//pMesh->SetColor(color);
	//pMesh->SetColorBinding(COLOR_PER_VERTEX);
	pMesh->SetColorBinding(COLOR_PER_FACE);

	//TransformMeshVertices(pMesh);
	pSceneGraph->Add(pMesh);
	file.close();

	//pMesh->SaveOBJFile("D:\\mesh2.obj");
	//pMesh->SaveTMFile("D:\\mesh.tm");
	//align with the human body
	//AlignWithHumanBody(pSceneGraph);

	return 1;
}
int CParseOBJ::TransformMeshVertices(CMesh3d* pMesh)
{
	for (int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		double x = pMesh->m_ArrayVertex[i]->x();
		double y = pMesh->m_ArrayVertex[i]->y();
		double z = pMesh->m_ArrayVertex[i]->z();

		//translation first.
		x = x + pMesh->GetTransform()->GetTranslation()->x();
		y = y + pMesh->GetTransform()->GetTranslation()->y();
		z = z + pMesh->GetTransform()->GetTranslation()->z();

		//scale next
		x = x * pMesh->GetTransform()->GetScale()->x();
		y = y * pMesh->GetTransform()->GetScale()->y();
		z = z * pMesh->GetTransform()->GetScale()->z();

		pMesh->m_ArrayVertex[i]->x(x);
		pMesh->m_ArrayVertex[i]->y(y);
		pMesh->m_ArrayVertex[i]->z(z);
	}

	CTransform tran;
	pMesh->SetTransform(tran);
	return 1;
}
int CParseOBJ::AlignWithHumanBody(CSceneGraph3d* pSceneGraph)
{

	if (pSceneGraph->NbObject() > 1)
	{
		CMesh3d* pMesh3D0 = (CMesh3d*)pSceneGraph->GetAt(0);
		CMesh3d* pMesh3D = (CMesh3d*)pSceneGraph->GetAt(1);

		/*//Get the central point
		Vector3D v48(pMesh3D->m_ArrayVertex[48]->x(),pMesh3D->m_ArrayVertex[48]->y(),pMesh3D->m_ArrayVertex[48]->z());
		Vector3D v47(pMesh3D->m_ArrayVertex[47]->x(),pMesh3D->m_ArrayVertex[47]->y(),pMesh3D->m_ArrayVertex[47]->z());
		Vector3D v51(pMesh3D->m_ArrayVertex[51]->x(),pMesh3D->m_ArrayVertex[51]->y(),pMesh3D->m_ArrayVertex[51]->z());
		Vector3D v52(pMesh3D->m_ArrayVertex[52]->x(),pMesh3D->m_ArrayVertex[52]->y(),pMesh3D->m_ArrayVertex[52]->z());
		Vector3D v54(pMesh3D->m_ArrayVertex[54]->x(),pMesh3D->m_ArrayVertex[54]->y(),pMesh3D->m_ArrayVertex[54]->z());
		Vector3D v49(pMesh3D->m_ArrayVertex[49]->x(),pMesh3D->m_ArrayVertex[49]->y(),pMesh3D->m_ArrayVertex[49]->z());

		Vector3D centralpoint = (v48 + v47 + v51 + v52 + v54 + v49)/6;
		Vector3D v23(pMesh3D0->m_ArrayVertex[23]->x(),pMesh3D0->m_ArrayVertex[23]->y(),pMesh3D0->m_ArrayVertex[23]->z());

		Vector3D trans = v23 - centralpoint;

		//define a local coordinate frame
		Vector3D xaxis = v47 - v48;
		xaxis.SetUnit();
		Vector3D yaxis = v51 - v47;
		yaxis = yaxis - xaxis * (yaxis * xaxis);
		yaxis.SetUnit();
		Vector3D zaxis = xaxis ^ yaxis;

		for(int i = 0; i < pMesh3D->NbVertex(); i++)
		{
			double x = pMesh3D->m_ArrayVertex[i]->x();
			double y = pMesh3D->m_ArrayVertex[i]->y();
			double z = pMesh3D->m_ArrayVertex[i]->z();
			Vector3D point(x,y,z);

			Vector3D vv = point - centralpoint;
			x = vv[0];
			y = vv[1];
			z = vv[2];

			x = (vv) * xaxis;
			y = (vv) * yaxis;
			z = (vv) * zaxis;

			z = -z;
			x = -x;


			double angle = 3 * 3.1415926 / 180;
			x = cos(angle) * x - sin(angle) * z;
			z = sin(angle) * x + cos(angle) * z;
			y = -y;

			x = x *  0.95;
			y = y *  0.95;
			z = z *  0.95;

			//x = x * 0.9 * 0.2;
			//y = y * 0.9 * 0.2;
			//z = z * 0.9 * 0.2;

			x = x + v23[0];
			y = y + v23[1] ;
			z = z + v23[2] + 0.01;

			pMesh3D->m_ArrayVertex[i]->x(x);
			pMesh3D->m_ArrayVertex[i]->y(y);
			pMesh3D->m_ArrayVertex[i]->z(z);
		}*/

		CTransform tran = *pMesh3D0->GetTransform();
		//tran.SetTranslation(new CVector3d(trans[0],trans[1],trans[2]));
		pMesh3D->SetTransform(tran);
		return 1;

		//transform.
		/*for(int i = 0; i < pMesh3D->m_ArrayVertex.GetSize(); i++)
		{
			double x = pMesh3D->m_ArrayVertex[i]->x() * 0.2;
			double y = pMesh3D->m_ArrayVertex[i]->y() * 0.2;
			double z = pMesh3D->m_ArrayVertex[i]->z() * 0.2;
			pMesh3D->m_ArrayVertex[i]->x(x);
			pMesh3D->m_ArrayVertex[i]->y(y);
			pMesh3D->m_ArrayVertex[i]->z(z);
		}*/
	}
	return 1;
}