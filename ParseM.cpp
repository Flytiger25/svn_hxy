#include "stdafx.h"
#include "lib3d\Base3d.h"
#include <fstream>
#include "ParseSmf.h"
#include <vector>
#include "math.h"
#include "nurbs3D.h"
#include "ParseM.h"
#include <iostream>   // std::cout
#include <sstream>
#include <string>
#include "SDIViewSwitchDoc.h"

CParseM::CParseM(void)
{
}

CParseM::~CParseM(void)
{
}
extern CSDIViewSwitchDoc* pDoc;;
extern char Post_Message[1000];
//double stod (const string&  str, size_t* idx = 0);
//double stod (const wstring& str, size_t* idx = 0);
int CParseM::Run(char *filename,CSceneGraph3d *pSceneGraph)
{
	char s[100];
	long m_VertexNumber, m_FaceNumber; //vertex and face number
	float m_x, m_y, m_z;  //coordinates of vertex
	long m_index1, m_index2, m_index3;  //index of vertices of face
	int vindex,findex;
	std::ifstream file(filename);

	float u,v;
	std::vector<CVertex3d*> m_ArrayVertex;
	std::vector<Point2D > m_ArrayVertexParameter;
	std::vector<Point3D > m_ArrayVertexNormal;
	std::vector<CFace3d*>   m_ArrayFace;
	std::vector<std::pair<int,int>> m_ArraySharpEdges;

	CVertex3d* pVertex;
	CFace3d*   pFace;
	CColor color(40,240,120);

	BOOL hasvt = FALSE;
	BOOL hasvn = FALSE;
	BOOL hasvn_m = FALSE;
	BOOL hasuv = FALSE;
	BOOL hasmu = FALSE;
	BOOL hasregion = FALSE;
	BOOL firstimeface = TRUE;
	std::vector<int> m_ArrayIndex;
	char inputmessage[500];
	while(!file.eof()) 
	{
		file.getline(inputmessage,500);
		std::stringstream inputmessagestreatm(inputmessage);

		inputmessagestreatm >> s;
		if(strcmp(s,"Vertex") == 0)
		{
			//vertex coordinates
			inputmessagestreatm >> vindex >> m_x >> m_y >> m_z;

			pVertex = new CVertex3d(m_x,m_y,m_z);
			pVertex->SetColor(color);
			pVertex->m_OriginalIndex = vindex;
			while(!inputmessagestreatm.eof())
			{
				inputmessagestreatm >> s;
				if(strstr(s,"rgb"))
				{
					pDoc->m_Light = 0;

					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					//int pos2 = s1.find(")") - 1;
					if(pos1 >=0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						double r,g,b;
						ss >> r;
						inputmessagestreatm >> g >>s;
						s1 = s;
						int pos2 = s1.find(")");
						s2 = s1.substr(0,pos2);
						std::stringstream ssb(s2);
						ssb >> b;

						CColor color1;
						color1.r(r * 255);
						color1.g(g * 255);
						color1.b(b * 255);
						pVertex->SetColor(color1);
					}
				}
				else if(strstr(s,"normal"))
				{
					//read normal
					hasvn_m = TRUE;
					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					if(pos1 >= 0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						double x,y,z;
						ss >> x;
						inputmessagestreatm >> y >>s;
						s1 = s;
						int pos2 = s1.find(")");
						s2 = s1.substr(0,pos2);
						std::stringstream ssb(s2);
						ssb >> z;

						pVertex->SetNormal(x,y,z);
					}
				}else if(strstr(s,"uv"))
				{
					hasuv = TRUE;
					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					if(pos1 >= 0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						double u,v;
						ss >> u;
						inputmessagestreatm >> v >>s;
						
						pVertex->u = u;
						pVertex->v = v;
					}			
				}else if(strstr(s,"mu"))
				{
					hasmu = TRUE;
					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					if(pos1 >= 0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						double u,v;
						ss >> u;
						inputmessagestreatm >> v >>s;

						pVertex->mux = u;
						pVertex->muy = v;
					}			
				}
				else if (strstr(s, "region"))
				{
					hasregion = TRUE;
					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					if (pos1 >= 0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						int region;
						ss >> region;
						//inputmessagestreatm >> v >> s;

						pVertex->region = region;
					}
				}
			}

			pVertex->m_Flag = vindex;
			m_ArrayVertex.push_back(pVertex);
			
		}else
		if(strcmp(s,"vt") == 0)
		{
			//vertex parameter domain
			file >> u >> v;
			m_ArrayVertexParameter.push_back(Point2D(u, v));
			//m_ArrayVertex[index1]->SetParameter(u,v);

			hasvt = TRUE;
		}else
		if(strcmp(s,"vn") == 0)
		{
			file >> m_x >> m_y >> m_z;

			m_ArrayVertexNormal.push_back(Point3D(m_x, m_y, m_z));
	//		m_ArrayVertex[index1]->SetNormal(m_x,m_y,m_z);

			hasvn = TRUE;
		}else
		if(strcmp(s,"Face") == 0)
		{

			//first time initialze the index array
			if(firstimeface)
			{
				int maxindex = -1;
				for(int ii = 0; ii < m_ArrayVertex.size(); ii++)
				{
					if(m_ArrayVertex[ii]->m_Flag > maxindex)
						maxindex = m_ArrayVertex[ii]->m_Flag;
				}
				m_ArrayIndex.resize(maxindex + 1,-1);
				for(int ii = 0; ii < m_ArrayVertex.size(); ii++)
				{
					m_ArrayIndex[m_ArrayVertex[ii]->m_Flag] = ii;
				}
				firstimeface = FALSE;
			}
			//face
			int m_index1parameter,m_index1normal;
			int m_index2parameter,m_index2normal;
			int m_index3parameter,m_index3normal;
			char c;

			inputmessagestreatm >> findex;
			inputmessagestreatm >> m_index1;
			if(hasvt)
			{
				file >> c;
				file >> m_index1parameter;
			}
			if(hasvn)
			{
				file >> c;
				file >> m_index1normal;
			}


			inputmessagestreatm >> m_index2;
			if(hasvt)
			{
				file >> c;
				file >> m_index2parameter;
			}
			if(hasvn)
			{
				file >> c;
				file >> m_index2normal;
			}


			inputmessagestreatm >> m_index3;
			if(hasvt)
			{
				file >> c;
				file >> m_index3parameter;
			}
			if(hasvn)
			{
				file >> c;
				file >> m_index3normal;
			}
			int Index1,Index2,Index3;
			Index1 = m_ArrayIndex[m_index1];
			Index2 = m_ArrayIndex[m_index2];
			Index3 = m_ArrayIndex[m_index3];
			
			pFace = new CFace3d(m_ArrayVertex[Index1],m_ArrayVertex[Index2],m_ArrayVertex[Index3]);

			if(hasvn)
			{
				m_ArrayVertex[m_index1 - 1]->SetNormal(m_ArrayVertexNormal[m_index1normal - 1][0], m_ArrayVertexNormal[m_index1normal - 1][1], m_ArrayVertexNormal[m_index1normal - 1][2]);
				m_ArrayVertex[m_index2 - 1]->SetNormal(m_ArrayVertexNormal[m_index2normal - 1][0], m_ArrayVertexNormal[m_index2normal - 1][1], m_ArrayVertexNormal[m_index2normal - 1][2]);
				m_ArrayVertex[m_index3 - 1]->SetNormal(m_ArrayVertexNormal[m_index3normal - 1][0], m_ArrayVertexNormal[m_index3normal - 1][1], m_ArrayVertexNormal[m_index3normal - 1][2]);

			}
		
			if(hasvt)
			{
				m_ArrayVertex[m_index1 - 1]->SetParameter(m_ArrayVertexParameter[m_index1parameter - 1][0], m_ArrayVertexParameter[m_index1parameter - 1][1]);
				m_ArrayVertex[m_index2 - 1]->SetParameter(m_ArrayVertexParameter[m_index2parameter - 1][0], m_ArrayVertexParameter[m_index2parameter - 1][1]);
				m_ArrayVertex[m_index3 - 1]->SetParameter(m_ArrayVertexParameter[m_index3parameter - 1][0], m_ArrayVertexParameter[m_index3parameter - 1][1]);
			}

			char c1,c2,c3,c4,c5,c6;
			inputmessagestreatm >> c1 >> c2 >> c3 >> c4 >> c5 >> c6;
			double rr,gg,bb;
			inputmessagestreatm >> rr >> gg >> bb;
			CColor cc;
			cc.Set(rr * 256,gg * 256,bb * 256);

			pFace->SetColor(cc);
			m_ArrayFace.push_back(pFace);
		}else
		if(strcmp(s,"Edge") == 0)
		{
			if(firstimeface)
			{
				int maxindex = -1;
				for(int ii = 0; ii < m_ArrayVertex.size(); ii++)
				{
					if(m_ArrayVertex[ii]->m_Flag > maxindex)
						maxindex = m_ArrayVertex[ii]->m_Flag;
				}
				m_ArrayIndex.resize(maxindex + 1,-1);
				for(int ii = 0; ii < m_ArrayVertex.size(); ii++)
				{
					m_ArrayIndex[m_ArrayVertex[ii]->m_Flag] = ii;
				}
				firstimeface = FALSE;
			}

			int vindex1,vindex2;
			inputmessagestreatm >> vindex1 >> vindex2 >> s;
			if(vindex1 != vindex2)
			{
				std::pair<int,int> data;
				int Index1,Index2;
				Index1 = m_ArrayIndex[vindex1];
				Index2 = m_ArrayIndex[vindex2];

				data.first = Index1;
				data.second = Index2;
				m_ArraySharpEdges.push_back(data);
			}

		}else
		if(strcmp(s,"Corner") == 0)
		{
			int vindex1,vindex2;
			inputmessagestreatm >> vindex1 >> vindex2;
			int Index1,Index2;
			if(vindex1 < m_ArrayIndex.size())
				Index1 = m_ArrayIndex[vindex1];
			else
				Index1 = -1;

			if(vindex2 < m_ArrayIndex.size())
				Index2 = m_ArrayIndex[vindex2];
			else
				Index2 = -1;

			double x,y,z;
			while(!inputmessagestreatm.eof())
			{
				inputmessagestreatm >> s;
				if(strstr(s,"normal"))
				{
					//read normal
					hasvn_m = TRUE;
					std::string s1 = s;
					int pos1 = s1.find("(") + 1;
					if(pos1 >= 0)
					{
						std::string s2 = s1.substr(pos1);
						std::stringstream ss(s2);
						ss >> x;
						inputmessagestreatm >> y >>s;
						s1 = s;
						int pos2 = s1.find(")");
						s2 = s1.substr(0,pos2);
						std::stringstream ssb(s2);
						ssb >> z;
					}
				}
			}

			if(Index1 > -1)
			{
				CVector3d* pVector1 = m_ArrayVertex[Index1]->GetNormal();
				Vector3D v1(pVector1->x(),pVector1->y(),pVector1->z());

				if(Length(v1) < 0.01)
				{
					m_ArrayVertex[Index1]->SetNormal(x,y,z);
				}
			}
			if(Index2 > -1)
			{
				CVector3d* pVector2 = m_ArrayVertex[Index2]->GetNormal();
				Vector3D v2(pVector2->x(),pVector2->y(),pVector2->z());
				if(Length(v2) < 0.01)
				{
					m_ArrayVertex[Index2]->SetNormal(x,y,z);
				}
			}
			
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
	CMesh3d *pMesh = new CMesh3d;
	if(hasvn_m)
	{
		pMesh->m_SetVertexNormal = TRUE;
		pMesh->m_SetFaceNormal = TRUE;
	}
	if(hasuv)
	{
		pMesh->hasvt = TRUE;
	}
	if(hasuv)
	{
		pMesh->hasuv = TRUE;
	}

	pMesh->	m_ColorBinding = COLOR_PER_VERTEX;
	pMesh->filename = filename;
	// Set Size (faster)
	pMesh->m_ArrayVertex.SetSize(m_ArrayVertex.size());
	pMesh->m_ArrayFace.SetSize(m_ArrayFace.size());
	pMesh->m_ArraySharpEdges = m_ArraySharpEdges;

	//Set the vertex pointer.
	for(int i = 0; i < m_ArrayVertex.size(); i++)
	{
		pMesh->m_ArrayVertex.SetAt(i,m_ArrayVertex[i]);
	}

	//Set the Face pointer
	for(int i = 0; i < m_ArrayFace.size(); i++)
	{
		pMesh->m_ArrayFace.SetAt(i,m_ArrayFace[i]);
	}

	if(m_ArrayVertex.size() == 0)
		return 1;


	//bounding box
	CVector3d m_lower,m_upper;
	m_lower.Set(m_ArrayVertex[0]->x(),m_ArrayVertex[0]->y(),m_ArrayVertex[0]->z());
	m_upper.Set(m_ArrayVertex[0]->x(),m_ArrayVertex[0]->y(),m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	for(int i=0; i<m_ArrayVertex.size(); i++)
	{
		m_x = m_ArrayVertex[i]->x();
		m_y = m_ArrayVertex[i]->y();
		m_z = m_ArrayVertex[i]->z();

		if(m_x < m_lower.x())
			m_lower.x(m_x) ;
		if(m_y < m_lower.y())
			m_lower.y(m_y) ;
		if(m_z < m_lower.z())
			m_lower.z(m_z);

		if(m_x > m_upper.x())
			m_upper.x(m_x) ;
		if(m_y > m_upper.y())
			m_upper.y(m_y) ;
		if(m_z > m_upper.z())
			m_upper.z(m_z) ;	
	}
	CVector3d m_translation;
	CVector3d m_Scale;
	m_translation=m_lower+m_upper;
	m_translation/=-2;
	float length=m_upper.x()-m_lower.x();
	float length1 = m_upper.y() - m_lower.y();
	if(length<m_upper.y()-m_lower.y())
		length=m_upper.y()-m_lower.y();
	if(length<m_upper.z()-m_lower.z())
		length=m_upper.z()-m_lower.z();

	transform.SetTranslation(&m_translation);
	length = length / 2;
	m_Scale.Set(1/length,1/length,1/length);
	transform.SetScale(&m_Scale);

	if(pSceneGraph->NbObject() >= 1)
	{
		if(pSceneGraph->GetAt(0)->GetType() == TYPE_NURBSSURFACE)
		{
			CNurbsSuface* pSurface = (CNurbsSuface*)pSceneGraph->GetAt(0);
			pMesh->SetTransform(*pSurface->GetTransform());
		}
		else
			if(pSceneGraph->GetAt(0)->GetType() == TYPE_MESH3D)
			{
				CMesh3d* pSurface = (CMesh3d*)pSceneGraph->GetAt(0);
				pMesh->SetTransform(*pSurface->GetTransform());
			}
	}else
		pMesh->SetTransform(transform);

	pMesh->SetFlagOnFaces(0);
	pMesh->m_showSharpEdge = 1;
	//pMesh->SetColor(color);
	
	//pMesh->SetColorBinding(COLOR_PER_FACE);
	//TransformMeshVertices(pMesh);
	pSceneGraph->Add(pMesh);
	file.close();

	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	//pMesh->GenerateLandMarks();
	//pMesh->SaveOBJFile("D:\\mesh2.obj");
	//pMesh->SaveTMFile("D:\\mesh.tm");
	//align with the human body
	//AlignWithHumanBody(pSceneGraph);

	return 1;
}