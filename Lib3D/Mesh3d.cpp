//********************************************
// Mesh3d.cpp
//********************************************
// class CMesh3d
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 15/01/98
//********************************************

#include "stdafx.h"
#include <math.h>
#include "Base3d.h"
#include "../SDIViewSwitch.h"
#include "Mesh3d.h"
#include "ColorRamp.h"
#include "../FormView3.h"
#include "../MyTreeView.h"
#include <fstream>
#include <atlimage.h>
#include <queue> 
#include "../Curve.h"

//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////
//********************************************
// Constructor
//********************************************
CMesh3d::CMesh3d()
{
	m_ListDone = 0;
	m_Modified = 1;
	m_SetFlagOnVertices_ArrayIndex = 0;
	m_NormalBinding = NORMAL_PER_VERTEX;
	m_ColorBinding = COLOR_PER_VERTEX;
	m_Name = _T("Mesh");
	// Texture
	m_IndexTexture = -1;
	m_pTextureCoordinate = NULL;
	m_pTextureCoordinateIndex = NULL;
	m_Show = 1;
	m_showFeatureVertexIndex = FALSE;
	m_showFeatureVertex = FALSE;
	m_SetVertexNormal = FALSE;
	m_SetFaceNormal = FALSE;
	m_showSelectedVertexIndex = FALSE;
	m_showFace = TRUE;
	m_showVertex = FALSE;
	m_showVertexIndex = FALSE;
	m_showFaceIndex = FALSE;
	m_showSelectedVertex = FALSE;
	m_showSelectedFace = 0;
	m_showSharpEdgeIndex = FALSE;
	m_showLandmarkIndex = FALSE;
	m_showEdge = FALSE;
	m_showLandmarkEdge = FALSE;
	m_showSelectedLandmarkEdge = FALSE;
	m_showLandmarkVertex = FALSE;
	m_showSharpEdge = FALSE;
	m_showSelectedSharpEdge = FALSE;
	m_showMergeConstraints = FALSE;
	m_showJianZhi = FALSE;
	m_showPointsOnSurface = 0;
	m_showStateName = FALSE;
	m_showPuzzleGraph = FALSE;
	hasvt = FALSE;
	m_showSelectedSharpVertex = FALSE;
	m_showLandmarkVertexIndex = FALSE;

	m_showAnchorPoints = FALSE;

	m_ArrayFace.RemoveAll();

	type = TYPE_MESH3D;

	name = "Mesh";

	m_SetVertexFlagAsItsIndex = FALSE;

	m_showFaceArea = FALSE;

	m_showTargetFaceArea = FALSE;

	m_showDifferenceFaceArea = FALSE;

	m_showFaceXYIndex = FALSE;

	m_showCellXYIndex = FALSE;

	m_showDistrictIndex = FALSE;

	m_SharpeEdgeColor.Set(0.8 * 256, 0, 0.8 * 256);

	m_EdgeColor.Set(0, 0, 0);
	pPolygon = NULL;

	m_showRectilinearGrid = 0;

	m_showSharpEdgeBaseNum = 0;
}

//********************************************
// Destructor
//********************************************
CMesh3d::~CMesh3d()
{
	Free();
}
BOOL CMesh3d::IsEdgeOnBoundary(int vstartindex, int vendindex)
{
	CVertex3d* pStartVertex = GetVertex(vstartindex);
	CVertex3d* pEndVertex = GetVertex(vendindex);
	if (!pStartVertex->IsOnBoundary())
		return FALSE;

	if (!pEndVertex->IsOnBoundary())
		return FALSE;

	int size = pStartVertex->m_ArrayFaceNeighbor.GetSize();
	int adjacentfacenum = 0;
	std::vector<CFace3d*> m_AdjacentFaces;
	for (int i = 0; i < size; i++)
	{
		if ((pStartVertex->m_ArrayFaceNeighbor[i]->v1() == pEndVertex || pStartVertex->m_ArrayFaceNeighbor[i]->v2() == pEndVertex || pStartVertex->m_ArrayFaceNeighbor[i]->v3() == pEndVertex))
		{
			m_AdjacentFaces.push_back(pStartVertex->m_ArrayFaceNeighbor[i]);
		}
	}

	size = pEndVertex->m_ArrayFaceNeighbor.GetSize();
	for (int i = 0; i < size; i++)
	{
		if ((pEndVertex->m_ArrayFaceNeighbor[i]->v1() == pStartVertex || pEndVertex->m_ArrayFaceNeighbor[i]->v2() == pStartVertex || pEndVertex->m_ArrayFaceNeighbor[i]->v3() == pStartVertex))
		{
			for (int j = 0; j < m_AdjacentFaces.size(); j++)
			{
				if (pEndVertex->m_ArrayFaceNeighbor[i] != m_AdjacentFaces[j])
					m_AdjacentFaces.push_back(pEndVertex->m_ArrayFaceNeighbor[i]);
			}
		}
	}

	if (m_AdjacentFaces.size() == 1)
		return TRUE;
	else
		return FALSE;
}
void CMesh3d::SaveTMFile(char* filename)
{
	std::ofstream file(filename);

	file << "#Mesh \n";
	file << NbVertex() << "  " << NbFace() << "\n";

	//Vertices
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Flag = i;
		file << GetVertex(i)->m_Coord[0] << " " << GetVertex(i)->m_Coord[1] << " " << GetVertex(i)->m_Coord[2] << " \n ";
	}

	//Faces
	for (int i = 0; i < NbFace(); i++)
	{
		file << GetFace(i)->v1()->m_Flag << " " << GetFace(i)->v2()->m_Flag << " " << GetFace(i)->v3()->m_Flag << " \n ";
	}

	file.close();
}
void CMesh3d::SaveBoundingBoxOBJFile(char* filename)
{
	std::ofstream file(filename);
	Point3D lower, upper;
	ComputeBoundingBox(lower, upper);
	file << "v " << lower[0] << " " << lower[1] << " " << lower[2] << "\n";
	file << "v " << lower[0] << " " << upper[1] << " " << lower[2] << "\n";
	file << "v " << upper[0] << " " << upper[1] << " " << upper[2] << "\n";
	file << "v " << upper[0] << " " << lower[1] << " " << upper[2] << "\n";
	file << "f 1 2 3 \n";
	file << "f 1 2 4 \n";
}
void CMesh3d::SaveOBJFile(char* filename)
{
	std::ofstream file(filename);

	//file << NbVertex() << "  " << NbFace() << "\n";

	//Vertices
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Flag = i;
		file << "v " << GetVertex(i)->m_Coord[0] << " " << GetVertex(i)->m_Coord[1] << " " << GetVertex(i)->m_Coord[2] << " \n ";
	}

	//Faces
	for (int i = 0; i < NbFace(); i++)
	{
		file << "f " << GetFace(i)->v1()->m_Flag + 1 << " " << GetFace(i)->v2()->m_Flag + 1 << " " << GetFace(i)->v3()->m_Flag + 1 << " \n ";
	}

	file.close();
}
void CMesh3d::CopyVertexRegion(CMesh3d* pStartMesh)
{
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->region = pStartMesh->GetVertex(i)->region;
		GetVertex(i)->m_OriginalIndex = pStartMesh->GetVertex(i)->m_OriginalIndex;
		GetVertex(i)->m_Normal = pStartMesh->GetVertex(i)->m_Normal;
		//file << "v " << GetVertex(i)->m_Coord[0] << " " << GetVertex(i)->m_Coord[1] << " " << GetVertex(i)->m_Coord[2] << " \n ";
	}
}
void CMesh3d::SaveMFile(char* file_name = NULL)
{
	CString s1;
	if (file_name == NULL)
	{
		s1 = filename.Left(filename.ReverseFind('.'));
		s1 = s1 + ".m";
	}
	else
		s1 = file_name;

	std::ofstream file(s1);
	std::ifstream* texturefile = NULL;
	COLORREF color;
	double r, g, b;

	int vertexoffset = 0;
	int faceoffset = 0;

	CImage m_TextureImage;
	if (hasvt)
	{
		CString s = filename.Left(filename.ReverseFind('.'));
		s = s + ".bmp";
		texturefile = new std::ifstream(s);

		m_TextureImage.Load(LPCTSTR(s));
	}
	hasuv = 0;
	hasvt = 1;
	hasregion = 1;
	for (int i = 0; i < NbVertex(); i++)
	{

		CVertex3d* pVertex = GetVertex(i);
		pVertex->m_Flag = i;

		file << "Vertex " << pVertex->m_Flag + 1 + vertexoffset << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2];
		//file << "Vertex " << pVertex->m_OriginalIndex << " " << pVertex->m_Coord[0] << " " << pVertex->m_Coord[1] << " " << pVertex->m_Coord[2];
		/*if (hasregion)
		{
			file << " {";
			file << "normal=(" << pVertex->GetNormal()->x() << " " << pVertex->GetNormal()->y() << " " << pVertex->GetNormal()->z() <<") ";
			file << "region=("<< pVertex->region<<")";
			file << "}\n";
		}*/

		/*if(hasuv)
		{
			file << " {";
			file << "uv=(" << pVertex->u << " " << pVertex->v << ") ";
			file << "}\n";
		}else
		{
			//file << "\n";
		}*/
		file << " {";
		if (hasvt)
		{
			//color = m_TextureImage.GetPixel((m_TextureImage.GetWidth() - 1) * pVertex->u + 0.5,(m_TextureImage.GetHeight() - 1) * (1 - pVertex->v) + 0.5);

			//b = ((color & 0xff0000) >> 16);
			//g = ((color & 0x00ff00) >> 8);
			//r = (color & 0x0000ff);

			//file << "rgb=("<<r/255.0<<" "<<g/255.0<<" "<<b/255.0<<") ";

			file << "rgb=(" << pVertex->GetColor()->r() / 255.0 << " " << pVertex->GetColor()->g() / 255.0 << " " << pVertex->GetColor()->b() / 255.0 << ") ";
			file << "uv=(" << pVertex->u << " " << pVertex->v << ") ";
		}
		if (m_SetVertexNormal)
		{
			file << "normal=(" << pVertex->GetNormal()->x() << " " << pVertex->GetNormal()->y() << " " << pVertex->GetNormal()->z() << ") ";
		}
		file << "}\n";
	}
	for (int i = 0; i < NbFace(); i++)
	{
		CFace3d* pFace3d = GetFace(i);
		file << "Face " << i + 1 + faceoffset << " " << pFace3d->v1()->m_Flag + 1 + vertexoffset << " " << pFace3d->v2()->m_Flag + 1 + vertexoffset << " " << pFace3d->v3()->m_Flag + 1 + vertexoffset << "\n";
	}
	for (int i = 0; i < m_ArraySharpEdges.size(); i++)
	{
		std::pair<int, int> pair = m_ArraySharpEdges[i];
		file << "Edge " << GetVertex(pair.first)->m_Flag + 1 + vertexoffset << " " << GetVertex(pair.second)->m_Flag + 1 + vertexoffset << " {sharp}" << "\n";
	}
	file.close();
}
//********************************************
// Free
//********************************************
void CMesh3d::Free()
{
	//TRACE("Cleanup mesh %x\n",this);
	m_ArrayVertex.Free();
	m_ArrayFace.Free();
	if (m_pTextureCoordinate != NULL)
		delete[] m_pTextureCoordinate;
	if (m_pTextureCoordinateIndex != NULL)
		delete[] m_pTextureCoordinateIndex;
}
double CMesh3d::TriangleArea(double u0, double v0, double u1, double v1, double u2, double v2)
{
	double a = sqrt((u1 - u0) * (u1 - u0) + (v1 - v0) * (v1 - v0));
	double b = sqrt((u2 - u1) * (u2 - u1) + (v2 - v1) * (v2 - v1));
	double c = sqrt((u0 - u2) * (u0 - u2) + (v0 - v2) * (v0 - v2));
	double p = (a + b + c) / 2;
	double area = sqrt(p * (p - a) * (p - b) * (p - c));
	return area;
}
int CMesh3d::JudeWhetherPointLocatedInTriangle(double u, double v, double u0, double v0, double u1, double v1, double u2, double v2)
{
	CPoint ptVertex[3];
	ptVertex[0].x = u0 * 1000000;
	ptVertex[0].y = v0 * 1000000;
	ptVertex[1].x = u1 * 1000000;
	ptVertex[1].y = v1 * 1000000;
	ptVertex[2].x = u2 * 1000000;
	ptVertex[2].y = v2 * 1000000;

	CRgn   rgnA;
	rgnA.CreatePolygonRgn(ptVertex, 3, ALTERNATE);
	BOOL isintriangle = rgnA.PtInRegion(u * 1000000, v * 1000000);
	return isintriangle;
}
int CMesh3d::ComputerPosition(double u, double v, Vector3D& resultvec)
{
	unsigned int NbVertex = (unsigned int)m_ArrayVertex.GetSize();
	unsigned int NbFace = (unsigned int)m_ArrayFace.GetSize();
	double u_, v_;
	int minindex = -1;
	double mindis = 1e10;

	//find the nearest 2D vertex
	for (int i = 0; i < NbVertex; i++)
	{
		m_ArrayVertex[i]->GetParameter(u_, v_);
		double dis = (u - u_) * (u - u_) + (v - v_) * (v - v_);
		if (dis < mindis)
		{
			mindis = dis;
			minindex = i;
		}
	}
	//find the adjacent faces
	//and judge which triangle it fall into.
	CVertex3d* pVertex = m_ArrayVertex[minindex];
	/*if(mindis < 1e-5)
	{
		resultvec.Set(pVertex->x(),pVertex->y(),pVertex->z());
		return TRUE;
	}*/
	for (int i = 0; i < pVertex->GetArrayFaceNeighbor()->GetSize(); i++)
	{
		CFace3d* pFace3d = pVertex->GetArrayFaceNeighbor()->GetAt(i);
		double u0, v0, u1, v1, u2, v2;
		CVector3d v1_, v2_, v3_;

		v1_.Set(pFace3d->v(0)->Get(0), pFace3d->v(0)->Get(1), pFace3d->v(0)->Get(2));
		v2_.Set(pFace3d->v(1)->Get(0), pFace3d->v(1)->Get(1), pFace3d->v(1)->Get(2));
		v3_.Set(pFace3d->v(2)->Get(0), pFace3d->v(2)->Get(1), pFace3d->v(2)->Get(2));

		pFace3d->v(0)->GetParameter(u0, v0);
		pFace3d->v(1)->GetParameter(u1, v1);
		pFace3d->v(2)->GetParameter(u2, v2);

		int isintriangle = JudeWhetherPointLocatedInTriangle(u, v, u0, v0, u1, v1, u2, v2);
		if (isintriangle)
		{
			double totalarea = TriangleArea(u0, v0, u1, v1, u2, v2);
			double area1 = TriangleArea(u, v, u0, v0, u1, v1);
			double area2 = TriangleArea(u, v, u1, v1, u2, v2);
			double area3 = TriangleArea(u, v, u2, v2, u0, v0);

			resultvec[0] = (v1_.x() * area1 + v2_.x() * area2 + v3_.x() * area3) / totalarea;
			resultvec[1] = (v1_.y() * area1 + v2_.y() * area2 + v3_.y() * area3) / totalarea;
			resultvec[2] = (v1_.z() * area1 + v2_.z() * area2 + v3_.z() * area3) / totalarea;
			return 1;
		}
	}
	return 0;
}

//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

double GetEdgeLength(CVertex3d* pStartVertex, CVertex3d* pEndVertex)
{
	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double z1 = pStartVertex->z();

	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	double z2 = pEndVertex->z();

	double length = sqrt(SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2));
	return length;
}
double CMesh3d::GetWeightedEdgeLength(CVertex3d* pStartVertex, CVertex3d* pEndVertex)
{
	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double z1 = pStartVertex->z();

	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	double z2 = pEndVertex->z();

	double length = sqrt(SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2));
	return length;
}
double GetWeightEdgeLength(CVertex3d* pStartVertex, CVertex3d* pEndVertex)
{
	double x1 = pStartVertex->x();
	double y1 = pStartVertex->y();
	double z1 = pStartVertex->z();

	double x2 = pEndVertex->x();
	double y2 = pEndVertex->y();
	double z2 = pEndVertex->z();

	double angle1, angle2;
	angle1 = pEndVertex->m_GaussCurvature;
	//m_ArrayVertexNeighbor
	angle2 = pStartVertex->m_GaussCurvature;

	double TOL_max = -2.95;
	double TOL_min = -3;
	int PUNISH = 1000;

	double length = sqrt(SQR(x1 - x2) + SQR(y1 - y2) + SQR(z1 - z2));
	if (angle1 > TOL_min && angle1 < TOL_max && angle2 > TOL_min && angle2 < TOL_max)
	{

		//pEndVertex->m_Flag = 1;
		return length;
	}
	else
	{
		return length * PUNISH;
	}
}

int CMesh3d::ComputeWeightedShortestPath(int vstartindex, int vendindex, std::vector<int>& path)
{
	//Dijkstra
//std::vector<int> openset;
	std::queue<int> openset;
	openset.push(vstartindex);
	GetVertex(vstartindex)->minimallength = 0;

	//use Flag for store parent
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Parent = -1;
		GetVertex(i)->minimallength = 1e10;
	}
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_num = i;
	}

	while (!openset.empty())
	{
		int index = openset.front();
		openset.pop();
		CVertex3d* pVertex = GetVertex(index);
		for (int i = 0; i < pVertex->NbVertexNeighbor(); i++)
		{
			if (pVertex->GetVertexNeighbor(i)->m_num != vstartindex)
			{
				if (pVertex->GetVertexNeighbor(i)->m_Parent == -1)
				{
					pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_num;
					openset.push(pVertex->GetVertexNeighbor(i)->m_num);
					pVertex->GetVertexNeighbor(i)->minimallength = pVertex->minimallength + GetWeightEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));
				}
				else
				{
					double newminimallength = pVertex->minimallength + GetWeightEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));;
					if (newminimallength < pVertex->GetVertexNeighbor(i)->minimallength)
					{
						pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_num;
						pVertex->GetVertexNeighbor(i)->minimallength = newminimallength;
					}
				}
			}

			if (pVertex->GetVertexNeighbor(i)->m_num == vendindex)
			{
				path.push_back(vendindex);

				while (GetVertex(vendindex)->m_Parent != -1)
				{
					path.push_back(GetVertex(vendindex)->m_Parent);
					vendindex = GetVertex(vendindex)->m_Parent;
				}
				//path.push_back(vstartindex);
				return path.size();
			}
		}
	}
}
int exist(std::vector<int> m_TargetVertex, int x)
{
	for (int i = 0; i < m_TargetVertex.size() - 1; i++)
	{
		if (m_TargetVertex[i] == x)
			return i;
	}
	return -1;
}
int CMesh3d::ComputeNewWeightedShortestPath(int vstartindex, int& vendindex, std::vector<int>& path, std::vector<int>& m_TargetVertex)
{
	//Dijkstra
	//std::vector<int> openset;
	std::queue<int> openset;
	openset.push(vstartindex);
	GetVertex(vstartindex)->minimallength = 0;

	//use Flag for store parent
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Parent = -1;
		GetVertex(i)->minimallength = 1e10;
	}
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_num = i;
	}

	while (!openset.empty())
	{
		int index = openset.front();
		openset.pop();
		CVertex3d* pVertex = GetVertex(index);
		for (int i = 0; i < pVertex->NbVertexNeighbor(); i++)
		{
			if (pVertex->GetVertexNeighbor(i)->m_num != vstartindex)
			{
				if (pVertex->GetVertexNeighbor(i)->m_Parent == -1)
				{
					pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_num;
					openset.push(pVertex->GetVertexNeighbor(i)->m_num);
					pVertex->GetVertexNeighbor(i)->minimallength = pVertex->minimallength + GetWeightEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));
				}
				else
				{
					double newminimallength = pVertex->minimallength + GetWeightEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));;
					if (newminimallength < pVertex->GetVertexNeighbor(i)->minimallength)
					{
						pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_num;
						pVertex->GetVertexNeighbor(i)->minimallength = newminimallength;
					}
				}
			}

			if (pVertex->GetVertexNeighbor(i)->m_Flag == 1 && exist(m_TargetVertex, pVertex->GetVertexNeighbor(i)->m_num) > -1)
			{

				//m_TargetVertex.erase(m_TargetVertex.begin() + exist(m_TargetVertex, pVertex->GetVertexNeighbor(i)->m_num));
				path.push_back(pVertex->GetVertexNeighbor(i)->m_num);
				int temp = pVertex->GetVertexNeighbor(i)->m_num;
				vendindex = temp;
				while (GetVertex(temp)->m_Parent != -1)
				{
					path.push_back(GetVertex(temp)->m_Parent);
					temp = GetVertex(temp)->m_Parent;
				}
				//path.push_back(vstartindex);
				return path.size();
			}
		}
	}
}

int CMesh3d::ComputeShortestPath(int vstartindex, int vendindex, std::vector<int>& path)
{
	//Dijkstra
	//std::vector<int> openset;
	std::queue<int> openset;
	openset.push(vstartindex);
	GetVertex(vstartindex)->minimallength = 0;

	//use flag for store parent
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Parent = -1;
		GetVertex(i)->minimallength = 1e10;
	}
	for (int i = 0; i < NbVertex(); i++)
	{
		GetVertex(i)->m_Flag = i;
	}

	while (!openset.empty())
	{
		int index = openset.front();
		openset.pop();
		CVertex3d* pVertex = GetVertex(index);
		for (int i = 0; i < pVertex->NbVertexNeighbor(); i++)
		{
			if (pVertex->GetVertexNeighbor(i)->m_Flag != vstartindex)
			{
				if (pVertex->GetVertexNeighbor(i)->m_Parent == -1)
				{
					pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_Flag;
					openset.push(pVertex->GetVertexNeighbor(i)->m_Flag);
					pVertex->GetVertexNeighbor(i)->minimallength = pVertex->minimallength + GetEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));
				}
				else
				{
					double newminimallength = pVertex->minimallength + GetEdgeLength(pVertex, pVertex->GetVertexNeighbor(i));;
					if (newminimallength < pVertex->GetVertexNeighbor(i)->minimallength)
					{
						pVertex->GetVertexNeighbor(i)->m_Parent = pVertex->m_Flag;
						pVertex->GetVertexNeighbor(i)->minimallength = newminimallength;
					}
				}
			}

			if (pVertex->GetVertexNeighbor(i)->m_Flag == vendindex)
			{
				path.push_back(vendindex);

				while (GetVertex(vendindex)->m_Parent != -1)
				{
					path.push_back(GetVertex(vendindex)->m_Parent);
					vendindex = GetVertex(vendindex)->m_Parent;
				}
				//path.push_back(vstartindex);
				return path.size();
			}
		}
	}
}
extern CString m_FileTextureName;
extern int m_ShowTexture;
//********************************************
// BuildList
//********************************************
int CMesh3d::glBuildList()
{
	//TRACE(" Start building list ...\n");

	// Check for valid mesh
	if (m_ArrayVertex.GetSize() == 0 &&
		m_ArrayFace.GetSize() == 0)
	{
		TRACE("CMesh3d::BuildList : empty mesh\n");
		return 0;
	}

	if (!m_Modified && m_ListDone)
		return 0;

	// Erase last list
	::glDeleteLists(m_ListOpenGL, 1);

	// Search for a new list
	m_ListOpenGL = ::glGenLists(1);
	if (m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return 0;
	}
	::glEnable(GL_LIGHTING);
	// Start list
	unsigned int NbVertex = (unsigned int)m_ArrayVertex.GetSize();
	unsigned int NbFace = (unsigned int)m_ArrayFace.GetSize();

	if (!NbVertex)
		return 0;
	if (!NbFace)
		return 0;

	CFace3d* pFace;
	CVertex3d* pVertex;
	CVector3d* pVector;
	CColor* pColorPrevious;

	::glNewList(m_ListOpenGL, GL_COMPILE);

	// Material

	//test
	/*::glPushMatrix();
	glTranslatef(test.x(),test.y(),test.z());
	GLUquadricObj * pObj;
	pObj = gluNewQuadric();
	gluQuadricDrawStyle(pObj,GLU_LINE);
	gluQuadricNormals(pObj,GLU_SMOOTH);
	gluSphere(pObj,5,16,16);
	::glPopMatrix();*/

	// Init color
	double r, g, b;
	/*pFace = m_ArrayFace[0];
	pColorPrevious = pFace->GetColor();
	r = pFace->GetColor()->r();
	g = pFace->GetColor()->g();
	b = pFace->GetColor()->b();
	::glColor3ub(r,g,b);

	//test
	::glBegin(GL_LINES);
	::glVertex3f(4.743061718,2.931568056,0);
	::glVertex3f(4.743061718,2.931568056,100);
	::glEnd();
	::glBegin(GL_LINES);
	::glVertex3f(4.993950350,2.931568056,0);
	::glVertex3f(4.993950350,2.931568056,100);
	::glEnd();*/


	// Triangles
	::glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < NbFace; i++)
	{
		glLoadName(i);
		{
			pFace = m_ArrayFace[i];
			//if(pFace->GetFlag())
			//	continue;

			//if(pFace->v1()->GetFlag())
			//	continue;

			//if(pFace->v2()->GetFlag())
			//	continue;

			//if(pFace->v3()->GetFlag())
			//	continue;

			ASSERT(pFace != NULL);

			// Normal (per face)
			if (m_NormalBinding == NORMAL_PER_FACE)
			{
				pVector = pFace->GetNormal();
				::glNormal3f(pVector->x(), pVector->y(), pVector->z());
			}

			// Color (per face)
			if (m_ColorBinding == COLOR_PER_FACE &&
				pColorPrevious != pFace->GetColor())
			{
				r = pFace->GetColor()->r();
				g = pFace->GetColor()->g();
				b = pFace->GetColor()->b();
				::glColor3ub(r, g, b);
				pColorPrevious = pFace->GetColor();
			}

			//if (pFace->GetFlag() == 1)
			//{
			//	//plane
			//	::glColor3ub(0, 255, 0);
			//}
			//else
			//	continue;
			for (int j = 2; j >= 0; j--)
			{
				// Normal
				if (m_NormalBinding == NORMAL_PER_VERTEX)
				{
					pVector = pFace->v(j)->GetNormal();
					::glNormal3f(pVector->x(), pVector->y(), pVector->z());
				}

				// Color (per vertex)
				if (m_ColorBinding == COLOR_PER_VERTEX) //&& 
					//pColorPrevious != pFace->v(j)->GetColor())
				{
					r = pFace->v(j)->GetColor()->r();
					g = pFace->v(j)->GetColor()->g();
					b = pFace->v(j)->GetColor()->b();
					//r = 255;
					//g = 0;
					//b = 0;
					::glColor3ub(r, g, b);
					//::glColor3ub(255, 0, 0);
					//pColorPrevious = pFace->v(j)->GetColor();
				}

				//if (pFace->GetFlag() == 1)
				//	::glColor3ub(0, 255, 0);

				// Texture coordinate (if needed)
				if (m_ShowTexture)
				{
					glTexCoord2f(pFace->v(j)->u, pFace->v(j)->v);
				}

				// Vertex
				::glVertex3f(pFace->v(j)->x(), pFace->v(j)->y(), pFace->v(j)->z());
			}
		}
	}

	::glEnd();

	::glEndList();

	// List is done now
	m_ListDone = 1;
	m_Modified = 0;

	return 1;
}
void CMesh3d::DrawScence()
{

	CFace3d* pFace;
	CVector3d* pVector;
	CColor* pColorPrevious;
	::glPushMatrix();

	// Position / translation / scaling
	glTranslatef(m_Transform.GetTranslation()->x(),
		m_Transform.GetTranslation()->y(),
		m_Transform.GetTranslation()->z());

	glScalef(m_Transform.GetScale()->x(),
		m_Transform.GetScale()->y(),
		m_Transform.GetScale()->z());

	glRotatef(m_Transform.GetValueRotation(),
		m_Transform.GetRotation()->x(),
		m_Transform.GetRotation()->y(),
		m_Transform.GetRotation()->z());

	// Init color
	if (m_ArrayFace.GetSize() > 1)
	{
		pFace = m_ArrayFace[0];
		pColorPrevious = pFace->GetColor();
		::glColor3ub(pFace->GetColor()->r(), pFace->GetColor()->g(), pFace->GetColor()->b());
	}

	// Triangles
	::glBegin(GL_TRIANGLES);
	unsigned int NbFace = (unsigned int)m_ArrayFace.GetSize();
	for (int i = 0; i < NbFace; i++)
	{
		glLoadName(i);
		{
			pFace = m_ArrayFace[i];
			if (pFace->GetFlag())
				continue;
			ASSERT(pFace != NULL);

			// Normal (per face)
			if (m_NormalBinding == NORMAL_PER_FACE)
			{
				pVector = pFace->GetNormal();
				::glNormal3f(pVector->x(), pVector->y(), pVector->z());
			}

			// Color (per face)
			if (m_ColorBinding == COLOR_PER_FACE &&
				pColorPrevious != pFace->GetColor())
			{
				::glColor3ub(pFace->GetColor()->r(), pFace->GetColor()->g(), pFace->GetColor()->b());
				pColorPrevious = pFace->GetColor();
			}

			for (int j = 0; j < 3; j++)
			{
				// Normal
				if (m_NormalBinding == NORMAL_PER_VERTEX)
				{
					pVector = pFace->v(j)->GetNormal();
					::glNormal3f(pVector->x(), pVector->y(), pVector->z());
				}

				// Color (per vertex)
				if (m_ColorBinding == COLOR_PER_VERTEX &&
					pColorPrevious != pFace->v(j)->GetColor())
				{
					::glColor3ub(pFace->v(j)->GetColor()->r(), pFace->v(j)->GetColor()->g(), pFace->v(j)->GetColor()->b());
					pColorPrevious = pFace->v(j)->GetColor();
				}

				// Texture coordinate (if needed)
				if (m_IndexTexture != -1)
				{
					glTexCoord2f(m_pTextureCoordinate[2 * m_pTextureCoordinateIndex[3 * i + j]],
						m_pTextureCoordinate[2 * m_pTextureCoordinateIndex[3 * i + j] + 1]);
				}

				// Vertex
				::glVertex3f(pFace->v(j)->x(), pFace->v(j)->y(), pFace->v(j)->z());
			}
		}
	}

	::glEnd();
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for (int i = 0; i < NbEdge; i++)
	{
		std::pair<int, int> data;
		data = m_ArraySharpEdges[i];
		::glVertex3f(m_ArrayVertex[data.first]->x(), m_ArrayVertex[data.first]->y(), m_ArrayVertex[data.first]->z());
		::glVertex3f(m_ArrayVertex[data.second]->x(), m_ArrayVertex[data.second]->y(), m_ArrayVertex[data.second]->z());
	}
	::glEnd();
	::glEnable(GL_LIGHTING);
	::glPopMatrix();
}
extern CMyTreeView* pTreeView;
//********************************************
// Draw
//********************************************
void CMesh3d::DrawCoordinateAxis()
{
	Point3D lower, upper;
	ComputeBoundingBox(lower, upper);

	glLineWidth(3.0);
	glDisable(GL_LIGHTING);
	::glColor3f(1, 0, 0);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(upper.x, lower.y, lower.z);
	::glEnd();

	::glColor3f(0, 1, 0);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(lower.x, upper.y, lower.z);
	::glEnd();

	::glColor3f(0, 0, 1);
	::glBegin(GL_LINES);
	::glVertex3f(lower.x, lower.y, lower.z);
	::glVertex3f(lower.x, lower.y, upper.z);
	::glEnd();
	glEnable(GL_LIGHTING);
}

extern int m_ShowCoordinateAxis;
void CMesh3d::showMergeConstraints()
{
	::glEnable(GL_LINE_SMOOTH);
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);
	::glPointSize(5.0);

	if (m_LandMarkArray.size() > 0)
	{
		for (int i = 0; i < m_ArrayMergeConstraints.size(); i++)
		{
			std::pair<int, int> pair;
			pair = m_ArrayMergeConstraints[i];

			::glLineWidth(6.0f);

			int style = m_LandMarkArray[pair.first]->style;
			if (style)
				::glColor3f(0.0, 1.0, 0.0);
			else
				::glColor3f(1.0, 0.0, 0.0);


			::glBegin(GL_LINES);
			for (int j = 0; j < m_LandMarkArray[pair.first]->landmark_points.size() - 1; j++)
			{
				int index1 = m_LandMarkArray[pair.first]->landmark_points[j];
				int index2 = m_LandMarkArray[pair.first]->landmark_points[j + 1];
				::glVertex3f(m_ArrayVertex[index1]->x(), m_ArrayVertex[index1]->y(), m_ArrayVertex[index1]->z());
				::glVertex3f(m_ArrayVertex[index2]->x(), m_ArrayVertex[index2]->y(), m_ArrayVertex[index2]->z());
			}
			::glEnd();

			style = m_LandMarkArray[pair.second]->style;
			if (style)

				::glColor3f(0.0, 1.0, 0.0);
			else
				::glColor3f(1.0, 0.0, 0.0);


			::glBegin(GL_LINES);
			for (int j = 0; j < m_LandMarkArray[pair.second]->landmark_points.size() - 1; j++)
			{
				int index1 = m_LandMarkArray[pair.second]->landmark_points[j];
				int index2 = m_LandMarkArray[pair.second]->landmark_points[j + 1];
				::glVertex3f(m_ArrayVertex[index1]->x(), m_ArrayVertex[index1]->y(), m_ArrayVertex[index1]->z());
				::glVertex3f(m_ArrayVertex[index2]->x(), m_ArrayVertex[index2]->y(), m_ArrayVertex[index2]->z());
			}
			::glEnd();
		}

	}
	else if (m_ArraySharpEdges.size() > 0)
	{
		for (int i = 0; i < m_ArrayMergeConstraints.size(); i++)
		{
			std::pair<int, int> pair, pair1;
			pair = m_ArrayMergeConstraints[i];

			::glLineWidth(6.0f);

			if (pair.first < m_ArraySharpEdges.size())
			{
				pair1 = m_ArraySharpEdges[pair.first];
				int startindex1 = pair1.first;
				int endindex1 = pair1.second;

				CVertex3d* pStartVertex1 = m_ArrayVertex[startindex1];
				CVertex3d* pEndVertex1 = m_ArrayVertex[endindex1];

				if (pStartVertex1->x() == pEndVertex1->x())
					::glColor3f(0.0, 1.0, 0.0);
				else
					::glColor3f(1.0, 0.0, 0.0);

				::glBegin(GL_LINES);
				::glVertex3f(pStartVertex1->x(), pStartVertex1->y(), pStartVertex1->z());
				::glVertex3f(pEndVertex1->x(), pEndVertex1->y(), pEndVertex1->z());
				::glEnd();
			}

			if (pair.second < m_ArraySharpEdges.size())
			{
				pair1 = m_ArraySharpEdges[pair.second];
				int startindex2 = pair1.first;
				int endindex2 = pair1.second;

				CVertex3d* pStartVertex2 = m_ArrayVertex[startindex2];
				CVertex3d* pEndVertex2 = m_ArrayVertex[endindex2];

				if (pStartVertex2->x() == pEndVertex2->x())
					::glColor3f(0.0, 1.0, 0.0);
				else
					::glColor3f(1.0, 0.0, 0.0);

				::glBegin(GL_LINES);
				::glVertex3f(pStartVertex2->x(), pStartVertex2->y(), pStartVertex2->z());
				::glVertex3f(pEndVertex2->x(), pEndVertex2->y(), pEndVertex2->z());
				::glEnd();
			}
		}
	}

	::glDepthFunc(GL_LESS);
}
void CMesh3d::GetColorTable(std::vector<std::vector<double>>& m_ArrayColorTable)
{
	std::vector<double> color;
	//Black
	//color.push_back(0);
	//color.push_back(0);
	//color.push_back(0);
	//m_ArrayColorTable.push_back(color);
	//color.clear();

	//White
	//color.push_back(1.0);
	//color.push_back(1.0);
	//color.push_back(1.0);
	//m_ArrayColorTable.push_back(color);
	//color.clear();
	//Red
	color.push_back(1.0);
	color.push_back(0.0);
	color.push_back(0.0);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Lime
	color.push_back(0.0);
	color.push_back(1.0);
	color.push_back(0.0);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Yellow
	color.push_back(1.0);
	color.push_back(1.0);
	color.push_back(0.0);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Cyan
	color.push_back(0.0);
	color.push_back(1.0);
	color.push_back(1.0);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Magenta
	color.push_back(1.0);
	color.push_back(0.0);
	color.push_back(1.0);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Silver
	//color.push_back(192.0 / 256);
	//color.push_back(192.0 / 256);
	//color.push_back(192.0 / 256);
	//m_ArrayColorTable.push_back(color);
	//color.clear();
	//Gray
	color.push_back(128.0 / 256);
	color.push_back(128.0 / 256);
	color.push_back(128.0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Maroon
	color.push_back(128.0 / 256);
	color.push_back(0 / 256);
	color.push_back(0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Olive
	color.push_back(128.0 / 256);
	color.push_back(128.0 / 256);
	color.push_back(0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Green
	color.push_back(0 / 256);
	color.push_back(128.0 / 256);
	color.push_back(0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Purple
	color.push_back(128.0 / 256);
	color.push_back(0.0 / 256);
	color.push_back(128.0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Teal
	color.push_back(0 / 256);
	color.push_back(128.0 / 256);
	color.push_back(128.0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
	//Navy
	color.push_back(0 / 256);
	color.push_back(0.0 / 256);
	color.push_back(128.0 / 256);
	m_ArrayColorTable.push_back(color);
	color.clear();
}

void CMesh3d::DrawCircle(double x, double y, double z, double radius)
{
	int i;
	int n = 100;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; ++i)
		::glVertex3f(x + radius * cos(2 * PI / n * i), y + radius * sin(2 * PI / n * i), z + 0);
	glEnd();
}

int CMesh3d::IsStateAdjacent(int i, int j)
{
	for (int ii = 0; ii < m_ArrayStates[i].size(); ii++)
		for (int jj = 0; jj < m_ArrayStates[j].size(); jj++)
		{
			if (IsFaceAdjacent(m_ArrayStates[i][ii], m_ArrayStates[j][jj]))
			{
				return 1;
			}
		}
	return 0;
}

int CMesh3d::IsFaceAdjacent(int i, int j)
{
	for (int ii = 0; ii < m_ArrayFacePointsArray[i].size(); ii++)
		for (int jj = 0; jj < m_ArrayFacePointsArray[j].size(); jj++)
		{
			int index1 = m_ArrayFacePointsArray[i][ii];
			int index2 = m_ArrayFacePointsArray[i][(ii + 1) % m_ArrayFacePointsArray[i].size()];
			int index3 = m_ArrayFacePointsArray[j][jj];
			int index4 = m_ArrayFacePointsArray[j][(jj + 1) % m_ArrayFacePointsArray[j].size()];
			if ((index1 == index3 || index1 == index4) && (index2 == index3 || index2 == index4))
				return 1;
		}
	return 0;
}
int CMesh3d::OutputUniformPovRay()
{
	std::ofstream outfile("D:\\uniform.inc");

	CImage m_TextureImage;
	m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");

	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for (int i = 0; i < NbEdge; i++)
	{
		std::pair<int, int> data;
		data = m_ArraySharpEdges[i];
		outfile << "cylinder {<" << m_ArrayVertex[data.first]->x() << " , " << m_ArrayVertex[data.first]->y() << " , " << -baseinterval << " >,<" << m_ArrayVertex[data.second]->x() << " , " << m_ArrayVertex[data.second]->y() << " , " << -baseinterval << " > ,r_edge texture { edge_tex } }" << std::endl;
	}

	double r = 1;
	double g = 0.5;
	double b = 1;
	//outfile<<"#declare k = 0.6;"<<std::endl;

	//outfile << "#declare r = 3*k;"<<std::endl;
	//outfile << "#declare g = 0.5*k;"<<std::endl;
	//outfile << "#declare b = 1*k;"<<std::endl;
	CImage m_HotTextureImage;
	m_HotTextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\summer1.PNG");
	double maxcolor = -1;
	double mincolor = 1e10;
	for (int i = 0; i < m_ArrayStates.size(); i++)
	{
		if (m_ArrayStatesColor[i][0] > maxcolor)
			maxcolor = m_ArrayStatesColor[i][0];
		if (m_ArrayStatesColor[i][0] < mincolor)
			mincolor = m_ArrayStatesColor[i][0];
		int x = m_ArrayStatesColor[i][0] * m_HotTextureImage.GetWidth();
		if (x == m_HotTextureImage.GetWidth())
			x--;
		if (x < 0)
			x = 0;
		COLORREF color = m_HotTextureImage.GetPixel(x, 0);
		double r = GetRValue(color) / 256.0;
		double g = GetGValue(color) / 256.0;
		double b = GetBValue(color) / 256.0;
		outfile << "#declare state_tex" << i << " = texture { pigment { rgb < " << r << "," << g << "," << b << "> } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};" << std::endl;
		//colorarray.push_back(GetRValue(color) / 256.0);
		//colorarray.push_back(GetGValue(color) / 256.0);
		//colorarray.push_back(GetBValue(color) / 256.0);
		//outfile << "#declare state_tex"<<i<<" = texture { pigment { rgb <"<<m_ArrayStatesColor[i][0]<<","<< m_ArrayStatesColor[i][1] <<"," << m_ArrayStatesColor[i][2] << "> } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};"<<std::endl;
		//outfile << "#declare state_tex"<<i<<" = texture { pigment { rgb < r*255*"<<m_ArrayStatesColor[i][0]<<",g*128*"<<m_ArrayStatesColor[i][0]<<",102>/255 } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};"<<std::endl;
		//glColor3f(m_ArrayStatesColor[i][0],m_ArrayStatesColor[i][1],m_ArrayStatesColor[i][2]);
	}

	for (int ii = 0; ii < m_ArrayStates.size(); ii++)
	{
		//::glColor3f(m_ArrayStatesColor[ii][0] * TOL,m_ArrayStatesColor[ii][1]  * TOL,m_ArrayStatesColor[ii][2]  * TOL);

		for (int i = 0; i < m_ArrayStates[ii].size(); i++)
		{
			std::vector<int> pointset = m_ArrayFaceAreaArray[m_ArrayStates[ii][i]];
			int size = pointset.size() / 4;
			double area = 0;
			for (int j = 0; j < size; j++)
			{
				int index1 = pointset[4 * j];
				int index2 = pointset[4 * j + 1];
				int index3 = pointset[4 * j + 2];
				int index4 = pointset[4 * j + 3];
				double TOL = 1e-8;
				for (double x = xinterval[index1]; x < xinterval[index2] - TOL; x += baseinterval)
					for (double y = yinterval[index3]; y < yinterval[index4] - TOL; y += baseinterval)
					{
						outfile << "object{Round_Box(<" << x << " , " << y << " , " << -0 << " >,<" << x + baseinterval << " , " << y + baseinterval << " , " << -baseinterval << " > ,roundradius1,0) texture { state_tex" << ii << "} }" << std::endl;
					}

				//for(double x = xinterval[index1]; x < xinterval[index2] - TOL; x += baseinterval)
				//{
				//	outfile << "cylinder {<" << x << " , " << yinterval[index3] << " , " << -baseinterval << " >,<" << x << " , " << yinterval[index4] << " , " << -baseinterval << " > ,r_edge texture { edge_tex } }" << std::endl;
					//::glVertex3f(x,yinterval[index3],0);
					//::glVertex3f(x,yinterval[index4],0);
				//}
				//for(double y = yinterval[index3]; y < yinterval[index4] - TOL; y += baseinterval)
				//{
				//	outfile << "cylinder {<" << xinterval[index1] << " , " << y << " , " << -baseinterval << " >,<" << xinterval[index2] << " , " << y << " , " << -baseinterval << " > ,r_edge texture { edge_tex } }" << std::endl;
					//::glVertex3f(xinterval[index1],y,0);
					//::glVertex3f(xinterval[index2],y,0);
				//}

			//area += (xinterval[index2] - xinterval[index1]) * (yinterval[index4] - yinterval[index3]);
			}

		}
	}


	//CImage m_TextureImage;
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\colorbar.PNG");
	//m_TextureImage.Load("D:\\papers\\code\\data\\Geo\\colorbar\\inverse_hot_colorbar.PNG");
	int colornum = 3;

	for (int i = 0; i < colornum; i++)
	{
		double value2 = ((double)i) / (colornum - 1);
		int x = value2 * m_TextureImage.GetWidth();
		if (x == m_TextureImage.GetWidth())
			x--;
		COLORREF color = m_TextureImage.GetPixel(x, 0);

		int a = GetRValue(color);
		int b = GetGValue(color);
		int c = GetBValue(color);
		outfile << "#declare cell_tex" << i << " = texture { pigment { rgb <" << a << "," << b << "," << c << ">/ 256.0 } finish { ambient thres_ambient diffuse thres_diffuse phong thres_phong  }};" << std::endl;
	}

	//top boxes
	double TOL = baseinterval / 4;
	//double TOL = 0;
	for (int i = 0; i < m_FillCells.size(); i++)
	{
		int cellnum = m_FillCells[i].size() / 4;
		for (int j = 0; j < cellnum; j++)
		{
			//glColor3f(m_FillCellsColor[i][j * 3],m_FillCellsColor[i][j * 3 + 1],m_FillCellsColor[i][j * 3 + 2]);
			if (m_FillCellStyle[i] == 0)
			{
				double x1 = m_FillCells[i][j * 4];
				double y1 = m_FillCells[i][j * 4 + 1];
				double x2 = m_FillCells[i][j * 4 + 2];
				double y2 = m_FillCells[i][j * 4 + 3];
				x1 = x1 + TOL;
				y1 = y1 + TOL;
				x2 = x2 - TOL;
				y2 = y2 - TOL;

				outfile << "object{Round_Box(<" << x1 << " , " << y1 << " , " << -baseinterval << " >,<" << x2 << " , " << y2 << " , " << -baseinterval << "-" << baseinterval << " * height" << " > ,roundradius2,0) texture { cell_tex" << m_FillCellColorIndex[i][j] << "} }" << std::endl;
			}
			else
			{
				double x1 = m_FillCells[i][j * 4];
				double y1 = m_FillCells[i][j * 4 + 1];
				double x2 = m_FillCells[i][j * 4 + 2];
				double y2 = m_FillCells[i][j * 4 + 3];
				x1 = x1 + TOL;
				y1 = y1 + TOL;
				x2 = x2 - TOL;
				y2 = y2 - TOL;

				outfile << "cylinder {<" << (x1 + x2) / 2 << " , " << (y1 + y2) / 2 << " , " << -baseinterval << " >,<" << (x1 + x2) / 2 << " , " << (y1 + y2) / 2 << " , " << -baseinterval << "-" << baseinterval << " * height" << " > ," << (x2 - x1) / 2 << " texture { cell_tex" << m_FillCellColorIndex[i][j] << "} }" << std::endl;
				//outfile << "object{Round_Box(<" << x1 << " , " << y1 << " , " << -baseinterval  << " >,<" << x2 << " , " << y2 << " , " << -baseinterval << "-" << baseinterval <<" * height" << " > ,roundradius2,0) texture { cell_tex" << m_FillCellColorIndex[i][j] <<"} }" << std::endl;
			}
		}
	}
	outfile.close();
	return 1;
}

int CMesh3d::showNURBScurves()
{
	//-------------------test
	::glDepthFunc(GL_ALWAYS);
	::glLineWidth(1.0);
	::glDisable(GL_LIGHTING);
	int ii = 1;
	if (ii)
	{
		/*int number = 8;
		int order = 4;
		Point3D* control = new Point3D[8];
		control[0].x = 2.000000;
		control[0].y = 2.000000;
		control[0].z = -12.408337;
		control[1].x = 2.067571;
		control[1].y = 2.500753;
		control[1].z = -12.815513;
		control[2].x = 1.133096;
		control[2].y = 2.753430;
		control[2].z = -10.626299;
		control[3].x = -0.011564;
		control[3].y = 2.604635;
		control[3].z = -11.515527;
		control[4].x = -0.011564;
		control[4].y = 1.121241;
		control[4].z = -11.213843;
		control[5].x = 1.133096;
		control[5].y = 0.618366;
		control[5].z = -10.183995;
		control[6].x = 2.067571;
		control[6].y = 1.599582;
		control[6].z = -12.626745;
		control[7].x = 2.000000;
		control[7].y = 2.000000;
		control[7].z = -12.408337;
		double *knot = new double[12];
		knot[0] = knot[1] = knot[2] = knot[3] = 0;
		knot[4] = 0.2;
		knot[5] = 0.4;
		knot[6] = 0.6;
		knot[7] = 0.8;
		knot[8] = knot[9] = knot[10] = knot[11] = 1;
		double *weight = new double[8];
		weight[0] = weight[1] = weight[2] = weight[3] = 1;
		weight[4] = weight[5] = weight[6] = weight[7] = 1;*/
		std::ifstream file("D:\\curve.txt");
		int number, order;
		file >> number;
		Point3D* control = new Point3D[number];
		for (int j = 0; j < number; j++)
		{
			double x, y, z;
			file >> control[j].x;
			file >> control[j].y;
			file >> control[j].z;
		}
		order = 4;
		int knotnum;
		file >> knotnum;
		double* knot = new double[knotnum];
		for (int j = 0; j < knotnum; j++)
		{
			file >> knot[j];
		}
		double* weight = new double[number];
		for (int j = 0; j < number; j++)
		{
			weight[j] = 1;
		}
		C_NurbsCurve3D* pCurve = new C_NurbsCurve3D(number, order, control, knot, weight);
		::glColor3ub(255, 0, 0);
		Point3D center(0, 0, -22);
		::glBegin(GL_LINES);
		double maxdis = -100;
		for (int j = 0; j < 100; j++)
		{
			Point3D p1 = pCurve->ComputPosition(j * 0.01);
			double dis = Distance(center, p1) - 10;
			if (dis > maxdis)
			{
				maxdis = dis;
			}
			Point3D p2 = pCurve->ComputPosition((j + 1) * 0.01);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
		}

		::glEnd();

	}
	//-------------------test
	return 1;
}
int CMesh3d::ShowEdges()
{
	//::glDepthFunc(GL_ALWAYS);
	::glDepthFunc(GL_LESS);
	::glColor3f(m_EdgeColor.r() / 256.0, m_EdgeColor.g() / 256.0, m_EdgeColor.b() / 256.0);
	::glLineWidth(1.0);
	::glDisable(GL_LIGHTING);
	//::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//::glEnable(GL_POLYGON_OFFSET_LINE);
	//::glPolygonOffset(pDoc->m_PolygonOffset,-0.01f);
	::glBegin(GL_LINES);
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace3d = GetFace(i);
		::glVertex3f(pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v1()->z());
		::glVertex3f(pFace3d->v2()->x(), pFace3d->v2()->y(), pFace3d->v2()->z());

		::glVertex3f(pFace3d->v2()->x(), pFace3d->v2()->y(), pFace3d->v2()->z());
		::glVertex3f(pFace3d->v3()->x(), pFace3d->v3()->y(), pFace3d->v3()->z());

		::glVertex3f(pFace3d->v3()->x(), pFace3d->v3()->y(), pFace3d->v3()->z());
		::glVertex3f(pFace3d->v1()->x(), pFace3d->v1()->y(), pFace3d->v1()->z());
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSharpEdges_Seperate()
{
	::glDepthFunc(GL_ALWAYS);
	//::glDepthFunc(GL_LESS);
	//::glColor3f(m_SharpeEdgeColor.r() / 256.0,m_SharpeEdgeColor.g() / 256.0,m_SharpeEdgeColor.b() / 256.0);
	::glColor3f(1.0, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10.0);
	::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges_Seperate.size();
	std::vector<double> coordinate, coordinate1;
	for (int i = 0; i < NbEdge; i++)
	{
		coordinate = m_ArraySharpEdges_Seperate[i];
		coordinate1 = m_ArraySharpEdges_Seperate[(i + 1) % m_ArraySharpEdges_Seperate.size()];

		::glVertex3f(coordinate[0], coordinate[1], coordinate[2]);
		::glVertex3f(coordinate1[0], coordinate1[1], coordinate1[2]);
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSharpEdges()
{
	::glDepthFunc(GL_ALWAYS);
	//::glDepthFunc(GL_LESS);
	//::glColor3f(m_SharpeEdgeColor.r() / 256.0,m_SharpeEdgeColor.g() / 256.0,m_SharpeEdgeColor.b() / 256.0);
	::glColor3f(1.0, 0, 0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(10.0);
	::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for (int i = 0; i < NbEdge; i++)
	{
		std::pair<int, int> data;
		data = m_ArraySharpEdges[i];
		::glVertex3f(m_ArrayVertex[data.first]->x(), m_ArrayVertex[data.first]->y(), m_ArrayVertex[data.first]->z());
		::glVertex3f(m_ArrayVertex[data.second]->x(), m_ArrayVertex[data.second]->y(), m_ArrayVertex[data.second]->z());
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSelectedFaces()
{
	::glColor3f(1.0, 0.0, 0.0);
	::glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace = m_ArrayFace[i];

		if (pFace->m_selectedflag)
		{
			::glVertex3f(pFace->v1()->x(), pFace->v1()->y(), pFace->v1()->z());
			::glVertex3f(pFace->v2()->x(), pFace->v2()->y(), pFace->v2()->z());
			::glVertex3f(pFace->v3()->x(), pFace->v3()->y(), pFace->v3()->z());
		}
	}
	::glEnd();
	return 1;
}
int CMesh3d::ShowSelectedEdges()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(6.0);
	::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for (int i = 0; i < m_ArraySelectedSharpEdgeIndex.size(); i++)
	{
		std::pair<int, int> data;
		data = m_ArraySharpEdges[m_ArraySelectedSharpEdgeIndex[i]];
		::glVertex3f(m_ArrayVertex[data.first]->x(), m_ArrayVertex[data.first]->y(), m_ArrayVertex[data.first]->z());
		::glVertex3f(m_ArrayVertex[data.second]->x(), m_ArrayVertex[data.second]->y(), m_ArrayVertex[data.second]->z());
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSharpEdgeIndex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2.0);
	//::glBegin(GL_LINES);
	unsigned int NbEdge = (unsigned int)m_ArraySharpEdges.size();
	for (int i = 0; i < NbEdge; i++)
	{
		std::pair<int, int> data;
		data = m_ArraySharpEdges[i];
		double x = (m_ArrayVertex[data.first]->x() + m_ArrayVertex[data.second]->x()) / 2;
		double y = (m_ArrayVertex[data.first]->y() + m_ArrayVertex[data.second]->y()) / 2;
		double z = (m_ArrayVertex[data.first]->z() + m_ArrayVertex[data.second]->z()) / 2;
		char message[100];
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(x, y, z, message);
		//::glVertex3f(m_ArrayVertex[data.first]->x(),m_ArrayVertex[data.first]->y(),m_ArrayVertex[data.first]->z());
		//::glVertex3f(m_ArrayVertex[data.second]->x(),m_ArrayVertex[data.second]->y(),m_ArrayVertex[data.second]->z());

	}
	//::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowLandmarkVertexIndex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2.0);
	//::glBegin(GL_LINES);

	unsigned int NbLandmark = (unsigned int)m_LandMarkArray.size();
	for (int i = 0; i < NbLandmark; i++)
	{
		GeoMapLandMark* pLandmark = m_LandMarkArray[i];
		int startindex, endindex;

		startindex = pLandmark->landmark_points[0];
		endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

		double x1 = m_ArrayVertex[startindex]->x();
		double y1 = m_ArrayVertex[startindex]->y();
		double z1 = m_ArrayVertex[startindex]->z();

		double x2 = m_ArrayVertex[endindex]->x();
		double y2 = m_ArrayVertex[endindex]->y();
		double z2 = m_ArrayVertex[endindex]->z();

		char message[100];
		sprintf(message, "%d", startindex);
		p_FormView3->drawOpenglString(x1, y1, z1, message);

		sprintf(message, "%d", endindex);
		p_FormView3->drawOpenglString(x2, y2, z2, message);
		//::glVertex3f(m_ArrayVertex[data.first]->x(),m_ArrayVertex[data.first]->y(),m_ArrayVertex[data.first]->z());
		//::glVertex3f(m_ArrayVertex[data.second]->x(),m_ArrayVertex[data.second]->y(),m_ArrayVertex[data.second]->z());

	}
	//::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}

int CMesh3d::ShowFeaturePointSequenceIndex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2.0);
	//::glBegin(GL_LINES);
	int index;
	for (int i = 0; i < m_ArraySharpEndPoints.size(); i++)
	{
		index = m_ArraySharpEndPoints[i];
		double x1 = m_ArrayVertex[index]->x();
		double y1 = m_ArrayVertex[index]->y();
		double z1 = m_ArrayVertex[index]->z();

		char message[100];
		sprintf(message, "%d", i + 1);
		p_FormView3->drawOpenglString(x1, y1, z1, message);
	}
	::glDepthFunc(GL_LESS);
	return 1;
}

int CMesh3d::ShowLandmarkIndex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glDisable(GL_LIGHTING);
	::glLineWidth(2.0);
	//::glBegin(GL_LINES);

	unsigned int NbLandmark = (unsigned int)m_LandMarkArray.size();
	for (int i = 0; i < NbLandmark; i++)
	{
		GeoMapLandMark* pLandmark = m_LandMarkArray[i];
		int startindex, endindex;

		startindex = pLandmark->landmark_points[0];
		endindex = pLandmark->landmark_points[pLandmark->landmark_points.size() - 1];

		double x = (m_ArrayVertex[startindex]->x() + m_ArrayVertex[endindex]->x()) / 2;
		double y = (m_ArrayVertex[startindex]->y() + m_ArrayVertex[endindex]->y()) / 2;
		double z = (m_ArrayVertex[startindex]->z() + m_ArrayVertex[endindex]->z()) / 2;

		char message[100];
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(x, y, z, message);
		//::glVertex3f(m_ArrayVertex[data.first]->x(),m_ArrayVertex[data.first]->y(),m_ArrayVertex[data.first]->z());
		//::glVertex3f(m_ArrayVertex[data.second]->x(),m_ArrayVertex[data.second]->y(),m_ArrayVertex[data.second]->z());

	}
	//::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowVertex()
{
	::glDepthFunc(GL_LESS);
	::glDisable(GL_LIGHTING);
	::glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	::glColor3f(0.0, 0.0, 0.0);
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glBegin(GL_POINTS);
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		::glVertex3f(m_ArrayVertex[i]->x(), m_ArrayVertex[i]->y(), m_ArrayVertex[i]->z());
	}
	::glEnd();
	return 1;
}
int CMesh3d::ShowFaceIndex()
{
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace = m_ArrayFace.GetAt(i);

		double x, y, z;
		x = (pFace->v1()->x() + pFace->v2()->x() + pFace->v3()->x()) / 3;
		y = (pFace->v1()->y() + pFace->v2()->y() + pFace->v3()->y()) / 3;
		z = (pFace->v1()->z() + pFace->v2()->z() + pFace->v3()->z()) / 3;
		char message[100];
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(x, y, z, message);
	}
	return 1;
}
int CMesh3d::ShowFeatureVertexIndex()
{
	for (int i = 0; i < m_ArrayFeatureVertexIndex.size(); i++)
	{
		int index = m_ArrayFeatureVertexIndex[i];
		{
			double x, y, z;
			x = m_ArrayVertex[index]->x();
			y = m_ArrayVertex[index]->y();
			z = m_ArrayVertex[index]->z();
			char message[100];
			sprintf(message, "%d", i);
			p_FormView3->drawOpenglString(x, y, z, message);
		}
	}
	return 1;
}
void CMesh3d::DectectFace_Plane(CFace3d* pCurrentFrontFace, std::vector<CFace3d*>& currentgroup, std::vector<CFace3d*>& currentgroupfront, double TOL)
{
	if (pCurrentFrontFace->GetFlag() == 1)
		return;

	//设置标记 1为已经检测
	pCurrentFrontFace->SetFlag(1);

	//如果共面了
	currentgroup.push_back(pCurrentFrontFace);

	//获取相邻的三个三角形
	CFace3d* pAdjFace1 = pCurrentFrontFace->f1();
	CFace3d* pAdjFace2 = pCurrentFrontFace->f2();
	CFace3d* pAdjFace3 = pCurrentFrontFace->f3();

	//判断和其中某一个三角形同向，则加入数组
	if (pAdjFace1 && pAdjFace1->GetFlag() == 0)
	{
		int isCoNormalFlag1 = IsNormalCoDirection(pCurrentFrontFace, pAdjFace1, TOL);
		if (isCoNormalFlag1)
		{
			//currentgroup.push_back(pAdjFace1);
			currentgroupfront.push_back(pAdjFace1);
		}
	}

	if (pAdjFace2 && pAdjFace2->GetFlag() == 0)
	{
		int isCoNormalFlag2 = IsNormalCoDirection(pCurrentFrontFace, pAdjFace2, TOL);
		if (isCoNormalFlag2)
		{
			//currentgroup.push_back(pAdjFace2);
			currentgroupfront.push_back(pAdjFace2);
		}
	}

	if (pAdjFace3 && pAdjFace3->GetFlag() == 0)
	{
		int isCoNormalFlag3 = IsNormalCoDirection(pCurrentFrontFace, pAdjFace3, TOL);
		if (isCoNormalFlag3)
		{
			//currentgroup.push_back(pAdjFace3);
			currentgroupfront.push_back(pAdjFace3);
		}
	}


}
int CMesh3d::ShowSelectedVertexIndex()
{
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		if (m_ArrayVertex[i]->m_Flag)
		{
			double x, y, z;
			x = m_ArrayVertex[i]->x();
			y = m_ArrayVertex[i]->y();
			z = m_ArrayVertex[i]->z();
			char message[100];
			sprintf(message, "%d", m_ArrayVertex[i]->m_Flag);
			p_FormView3->drawOpenglString(x, y, z, message);
		}
	}
	return 1;
}
int CMesh3d::ShowEdgeIndex()
{
	return 1;
}
int CMesh3d::ShowVertexIndex()
{
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		double x, y, z;
		x = m_ArrayVertex[i]->x();
		y = m_ArrayVertex[i]->y();
		z = m_ArrayVertex[i]->z();
		char message[100];
		sprintf(message, "%d", i);
		p_FormView3->drawOpenglString(x, y, z, message);
	}
	return 1;
}
int CMesh3d::ShowSelectedLandmarkEdge()
{
	::glEnable(GL_LINE_SMOOTH);
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);
	::glPointSize(5.0);

	//std::vector<int>::iterator literator;
	//for(int i = 0; i < m_LandMarkArray.size(); i++)
	for (int i = 0; i < m_SelectedLandmarkIndexArray.size(); i++)
	{
		int landmarkindex = m_SelectedLandmarkIndexArray[i];
		if (landmarkindex >= m_LandMarkArray.size())
			continue;
		if (!m_LandMarkArray[landmarkindex]->isVisible)
			continue;

		//Make the seleted Landmarks thicker
		//literator = std::find(m_SelectedLandmarkIndexArray.begin(),m_SelectedLandmarkIndexArray.end(),i);
		//if(literator == m_SelectedLandmarkIndexArray.end())
		//	continue;

		::glLineWidth(6.0f);

		int style = m_LandMarkArray[landmarkindex]->style;
		if (style)
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(0.9,1.0,0.9);
			//else
			::glColor3f(0.0, 1.0, 0.0);
		else
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(1.0,0.9,0.9);
			//else
			::glColor3f(1.0, 0.0, 0.0);


		::glBegin(GL_LINES);
		for (int j = 0; j < m_LandMarkArray[landmarkindex]->landmark_points.size() - 1; j++)
		{
			int index1 = m_LandMarkArray[landmarkindex]->landmark_points[j];
			int index2 = m_LandMarkArray[landmarkindex]->landmark_points[j + 1];
			::glVertex3f(m_ArrayVertex[index1]->x(), m_ArrayVertex[index1]->y(), m_ArrayVertex[index1]->z());
			::glVertex3f(m_ArrayVertex[index2]->x(), m_ArrayVertex[index2]->y(), m_ArrayVertex[index2]->z());
		}
		::glEnd();
	}

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowLandmarkEdge()
{
	::glEnable(GL_LINE_SMOOTH);
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);
	::glPointSize(5.0);

	std::vector<int>::iterator literator;
	for (int i = 0; i < m_LandMarkArray.size(); i++)
	{
		if (!m_LandMarkArray[i]->isVisible)
			continue;

		//Make the seleted Landmarks thicker
		if (m_LandMarkArray[i]->isAuxillary)
		{
			::glLineWidth(4.0f);
		}
		else
		{
			::glLineWidth(2.0f);
		}
		int style = m_LandMarkArray[i]->style;
		if (style)
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(0.9,1.0,0.9);
			//else
			::glColor3f(0.0, 1.0, 0.0);
		else
			//if(m_LandMarkArray[i]->isAuxillary)
			//	::glColor3f(1.0,0.9,0.9);
			//else
			::glColor3f(1.0, 0.0, 0.0);

		//::glLineWidth(2.0f);

		::glBegin(GL_LINES);
		for (int j = 0; j < m_LandMarkArray[i]->landmark_points.size() - 1; j++)
		{
			int index1 = m_LandMarkArray[i]->landmark_points[j];
			int index2 = m_LandMarkArray[i]->landmark_points[j + 1];
			::glVertex3f(m_ArrayVertex[index1]->x(), m_ArrayVertex[index1]->y(), m_ArrayVertex[index1]->z());
			::glVertex3f(m_ArrayVertex[index2]->x(), m_ArrayVertex[index2]->y(), m_ArrayVertex[index2]->z());
		}
		::glEnd();
	}

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowFace()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (pDoc->m_Light)
		::glEnable(GL_LIGHTING);
	else
		::glDisable(GL_LIGHTING);

	::glCallList(m_ListOpenGL);
	return 1;
}
int CMesh3d::ShowLandmarkVertex()
{
	::glDisable(GL_LIGHTING);
	::glDepthFunc(GL_ALWAYS);
	::glPointSize(5.0);
	::glBegin(GL_POINTS);
	::glColor3f(0.0, 0.0, 1.0);
	for (int i = 0; i < m_LandMarkArray.size(); i++)
	{
		if (!m_LandMarkArray[i]->isVisible)
			continue;

		int index1 = m_LandMarkArray[i]->landmark_points[0];
		int index2 = m_LandMarkArray[i]->landmark_points[m_LandMarkArray[i]->landmark_points.size() - 1];
		::glVertex3f(m_ArrayVertex[index1]->x(), m_ArrayVertex[index1]->y(), m_ArrayVertex[index1]->z());
		::glVertex3f(m_ArrayVertex[index2]->x(), m_ArrayVertex[index2]->y(), m_ArrayVertex[index2]->z());
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowFeatureVertex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glColor3f(0.0, 1.0, 0.0);
	::glBegin(GL_POINTS);
	for (int i = 0; i < m_ArrayFeatureVertexIndex.size(); i++)
	{
		int index = m_ArrayFeatureVertexIndex[i];
		double x, y, z;
		x = m_ArrayVertex[index]->x();
		y = m_ArrayVertex[index]->y();
		z = m_ArrayVertex[index]->z();
		::glVertex3f(x, y, z);
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSelectedSharpVertex()
{
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(1.0, 0.0, 0.0);
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glColor3f(0.0, 1.0, 0.0);
	::glBegin(GL_POINTS);
	for (int i = 0; i < m_ArraySharpVertexIndex.size(); i++)
	{
		int index = m_ArraySharpVertexIndex[i];
		double x, y, z;
		x = m_ArrayVertex[index]->x();
		y = m_ArrayVertex[index]->y();
		z = m_ArrayVertex[index]->z();
		::glVertex3f(x, y, z);
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowSelectedVertex()
{
	::glDepthFunc(GL_LESS);
	::glColor3f(1.0, 0.0, 0.0);
	::glPointSize(5.0);
	//::glCallList(m_ListOpenGL);
	::glColor3f(1.0, 0.0, 0.0);
	::glBegin(GL_POINTS);
	for (int i = 0; i < NbVertex(); i++)
	{
		CVertex3d* pVertex = GetVertex(i);
		if (pVertex->m_Flag)
		{
			::glVertex3f(pVertex->x(), pVertex->y(), pVertex->z());
		}
	}
	::glEnd();

	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::ShowAnchorPoints()
{
	//fixed vertices

	::glDepthFunc(GL_ALWAYS);
	::glColor3f(0.0, 0.0, 0.0);
	::glPointSize(10.0);
	//::glCallList(m_ListOpenGL);
	::glBegin(GL_POINTS);
	for (int i = 0; i < m_ArrayAnchorPoints.size(); i++)
	{
		double x = GetVertex(m_ArrayAnchorPoints[i])->x();
		double y = GetVertex(m_ArrayAnchorPoints[i])->y();
		double z = GetVertex(m_ArrayAnchorPoints[i])->z();
		::glVertex3f(x, y, z);
	}
	/*::glColor3f(1.0, 0.0, 0.0);
	std::ifstream file("D:\\papers\\2017\\gmod2017\\Intrinsic Parameterization and Registration of graph constrained surfaces\\data\\failpoints.txt");
	while (!file.eof())
	{
		int vindex;
		CString s1;
		file >> vindex;
		CVertex3d* pVertex = GetVertex(vindex);
		double x1 = pVertex->x();
		double y1 = pVertex->y();
		double z1 = pVertex->z();
		::glVertex3f(x1, y1, z1);
	}
	file.close();*/
	/*int faceid = 8678;
	int vindex = 1;
	CFace3d* pFace = GetFace(faceid);
	double x1 = pFace->v(vindex)->x();
	double y1 = pFace->v(vindex)->y();
	double z1 = pFace->v(vindex)->z();
	::glVertex3f(x1, y1, z1);*/
	::glEnd();
	::glDepthFunc(GL_LESS);

	return 1;
}
int CMesh3d::CheckPointInTriangle3D(Point3D p, CFace3d* pFace)
{
	double TOL = 10;
	Point3D p1(pFace->v1()->x(), pFace->v1()->y(), pFace->v1()->z());
	Point3D p2(pFace->v2()->x(), pFace->v2()->y(), pFace->v2()->z());
	Point3D p3(pFace->v3()->x(), pFace->v3()->y(), pFace->v3()->z());

	Vector3D n1 = (p3 - p2) ^ (p - p2);
	Vector3D n2 = (p1 - p3) ^ (p - p3);
	Vector3D n3 = (p2 - p1) ^ (p - p1);

	n1.SetUnit();
	n2.SetUnit();
	n3.SetUnit();

	int flag1 = IsNormalSameDirection(n1, n2, TOL);
	int flag2 = IsNormalSameDirection(n1, n3, TOL);
	int flag3 = IsNormalSameDirection(n2, n3, TOL);
	if (flag1 && flag2 && flag3)
		return 1;
	else
		return 0;
}

int CMesh3d::ShowInputPointsOnMesh()
{
	::glDepthFunc(GL_LESS);
	::glColor3f(0.0, 0.0, 0.0);
	::glPointSize(10.0);

	for (int i = 0; i < m_ArrayPointCoordinates.size(); i = i + 3)
	{
		//::glCallList(m_ListOpenGL);
		::glBegin(GL_POINTS);
		::glVertex3f(m_ArrayPointCoordinates[i], m_ArrayPointCoordinates[i + 1], m_ArrayPointCoordinates[i + 2]);
		::glEnd();
		char message[100];
		sprintf(message, "%d", i / 3);
		p_FormView3->drawOpenglString(m_ArrayPointCoordinates[i], m_ArrayPointCoordinates[i + 1], m_ArrayPointCoordinates[i + 2], message);
	}

	::glDepthFunc(GL_LESS);
	return 1;
}

int CMesh3d::ShowFixedVertices()
{
	//fixed vertices
	for (int i = 0; i < m_FixVerticesArray.size(); i++)
	{
		::glDepthFunc(GL_ALWAYS);
		::glColor3f(0.0, 0.0, 0.0);
		::glPointSize(10.0);
		//::glCallList(m_ListOpenGL);
		::glBegin(GL_POINTS);
		for (int i = 0; i < m_FixVerticesArray.size(); i++)
		{
			int index = m_FixVerticesArray[i];
			::glVertex3f(m_ArrayVertex[index]->x(), m_ArrayVertex[index]->y(), m_ArrayVertex[index]->z());
		}
		::glEnd();
		::glDepthFunc(GL_LESS);
	}
	return 1;
}
int CMesh3d::ShowFixedVertices_Inner()
{
	//fixed vertices
	::glDepthFunc(GL_ALWAYS);
	::glColor3f(0.0, 0.0, 0.0);
	::glPointSize(10.0);
	//::glCallList(m_ListOpenGL);
	::glBegin(GL_POINTS);
	for (int i = 0; i < m_FixVerticesArray_Inner.size(); i++)
	{
		int index = m_FixVerticesArray_Inner[i];
		::glVertex3f(m_ArrayVertex[index]->x(), m_ArrayVertex[index]->y(), m_ArrayVertex[index]->z());
	}
	::glEnd();
	::glDepthFunc(GL_LESS);
	return 1;
}
int CMesh3d::PlotCell(int x, int y)
{
	::glBegin(GL_TRIANGLES);
	double tol = baseinterval * 0.1;
	double x1 = xinterval[0] + baseinterval * x + tol;
	double y1 = yinterval[0] + baseinterval * y + tol;
	double x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	double y2 = yinterval[0] + baseinterval * (y + 1) - tol;

	::glVertex3f(x1, y1, 0);
	::glVertex3f(x2, y1, 0);
	::glVertex3f(x2, y2, 0);
	::glVertex3f(x1, y1, 0);
	::glVertex3f(x2, y2, 0);
	::glVertex3f(x1, y2, 0);
	::glEnd();
	return 1;
}
int CMesh3d::glDraw()
{
	if (!m_Show)
		return 0;


	::glPushMatrix();
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);

	//glLightfv( GL_LIGHT0, GL_POSITION, Position );
	::glEnable(GL_LIGHTING);
	GLubyte* pTextureBits;
	GLint width, height;
	CDib* m_pDib;
	if (m_ShowTexture)
	{
		m_pDib = new CDib(m_FileTextureName);
		pTextureBits = (GLubyte*)m_pDib->GetDibBitsPtr();
		width = m_pDib->GetDibWidth();
		height = m_pDib->GetDibHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTextureBits);
		//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}

	::glPushMatrix();

	// Position / translation / scaling
	if (!m_SetCameraMode)
	{
		glScalef(m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());

		glTranslatef(m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());

		glRotatef(m_Transform.GetValueRotation(),
			m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z());
	}

	//store the model view & projection view.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, m_viewPort);

	/*::glDepthFunc(GL_ALWAYS);
	::glColor3f(1, 0, 0);
	::glPointSize(5.0);
	::glBegin(GL_LINES);
	::glVertex3f(-1, -1, 0);
	::glVertex3f(1, 1, 0);
	::glEnd();
	::glDepthFunc(GL_LESS);*/

	//TRACE("---------------------------------------------------------------\n");
	//TRACE("%f %f %f %f\n",modelview[0],modelview[4],modelview[8],modelview[12]);
	//TRACE("%f %f %f %f\n",modelview[1],modelview[5],modelview[9],modelview[13]);
	//TRACE("%f %f %f %f\n",modelview[2],modelview[6],modelview[10],modelview[14]);
	//TRACE("%f %f %f %f\n",modelview[3],modelview[7],modelview[11],modelview[15]);
	//TRACE("---------------------------------------------------------------\n");
	//if (m_ShowCoordinateAxis)
	DrawCoordinateAxis();

#if defined(GPU_ACCELERATE)
	pTreeView->modelViewProj1 = cgGetNamedParameter(pTreeView->finalvertProgram1, "ModelViewProj");
	pTreeView->modelViewIT1 = cgGetNamedParameter(pTreeView->finalvertProgram1, "ModelViewIT");

	cgGLSetStateMatrixParameter(pTreeView->modelViewProj1, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(pTreeView->modelViewIT1, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
	GLfloat mat_diff[] = { 0.91, 0.42, 0.34, 1.0f };
	cgGLSetParameter3fv(pTreeView->frag_mat_diff1, mat_diff);
	cgGLSetParameter3fv(pTreeView->frag_mat_ambient1, mat_diff);
	cgGLSetParameter3fv(pTreeView->frag_mat_spec1, mat_diff);

	cgGLEnableProfile(pTreeView->vertProfile1);
	cgGLEnableProfile(pTreeView->fragmentProfile1);

	cgGLBindProgram(pTreeView->finalvertProgram1);
	cgGLBindProgram(pTreeView->finalfragProgram1);
#endif

	// Build list at first
	if (!m_ListDone || m_Modified)
		glBuildList();

	// Search for a new list
	if (::glIsList(m_ListOpenGL) == GL_TRUE)
	{

		::glPointSize(5.0);
		double maxlen;
		//::glCallList(m_ListOpenGL);
		/*pPolyOmino->maxPointDeviations = 0.035;
		maxlen = pPolyOmino->maxPointDeviations * 2;
		::glBegin(GL_POINTS);
		double x = 0.51;
		double y = 0.54;
		::glVertex3f(x, y,0);
		::glEnd();
		::glBegin(GL_LINES);
		::glVertex3f(x - maxlen / 2, y - maxlen / 2,0);
		::glVertex3f(x + maxlen / 2, y - maxlen / 2,0);
		::glVertex3f(x - maxlen / 2, y + maxlen / 2,0);
		::glVertex3f(x + maxlen / 2, y + maxlen / 2,0);
		::glVertex3f(x - maxlen / 2, y - maxlen / 2,0);
		::glVertex3f(x - maxlen / 2, y + maxlen / 2,0);
		::glVertex3f(x + maxlen / 2, y - maxlen / 2,0);
		::glVertex3f(x + maxlen / 2, y + maxlen / 2,0);
		::glEnd();

		pPolyOmino->maxPointDeviations = 0.011 * 2;
		maxlen = pPolyOmino->maxPointDeviations;
		::glBegin(GL_POINTS);
		//x = 0.47;
		//y = 0.52;
		::glVertex3f(0.510521, 0.542768, 0);
		::glEnd();
		::glBegin(GL_LINES);
		::glVertex3f(x - maxlen / 2, y - maxlen / 2, 0);
		::glVertex3f(x + maxlen / 2, y - maxlen / 2, 0);
		::glVertex3f(x - maxlen / 2, y + maxlen / 2, 0);
		::glVertex3f(x + maxlen / 2, y + maxlen / 2, 0);
		::glVertex3f(x - maxlen / 2, y - maxlen / 2, 0);
		::glVertex3f(x - maxlen / 2, y + maxlen / 2, 0);
		::glVertex3f(x + maxlen / 2, y - maxlen / 2, 0);
		::glVertex3f(x + maxlen / 2, y + maxlen / 2, 0);
		::glEnd();*/

		if (m_showFace)
			ShowFace();

		if (m_showEdge)
			ShowEdges();

		if (m_showSharpEdge)
			ShowSharpEdges();

		//if (m_showSharpEdge)
		//{
		//	showNURBScurves();
		//}
		ShowSharpEdges_Seperate();

		if (m_showSelectedFace)
			ShowSelectedFaces();

		if (m_showSelectedSharpEdge)
			ShowSelectedEdges();

		if (m_showSharpEdgeIndex)
			ShowSharpEdgeIndex();

		if (m_showLandmarkIndex)
			ShowLandmarkIndex();

		ShowFeaturePointSequenceIndex();

		if (m_showLandmarkVertexIndex)
			ShowLandmarkVertexIndex();

		if (m_showVertex)
			ShowVertex();

		if (m_showVertexIndex)
			ShowVertexIndex();

		if (m_showEdgeIndex)
			ShowEdgeIndex();

		if (m_showSelectedVertexIndex)
			ShowSelectedVertexIndex();

		if (m_showFaceIndex)
			ShowFaceIndex();

		if (m_showMergeConstraints)
			showMergeConstraints();

		if (m_showSelectedLandmarkEdge)
			ShowSelectedLandmarkEdge();

		if (m_showLandmarkEdge)
			ShowLandmarkEdge();

		if (m_showLandmarkVertex)
			ShowLandmarkVertex();

		if (m_showSelectedVertex)
			ShowSelectedVertex();

		if (m_showSelectedSharpVertex)
			ShowSelectedSharpVertex();

		if (m_showFeatureVertex)
			ShowFeatureVertex();

		if (m_showFeatureVertexIndex)
			ShowFeatureVertexIndex();

		if (m_showAnchorPoints)
			ShowAnchorPoints();

		if (m_showPointsOnSurface)
			ShowInputPointsOnMesh();

		ShowFixedVertices();

		ShowFixedVertices_Inner();

		//return 1;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n", m_ListOpenGL);
		//return 0;
	}

#if defined(GPU_ACCELERATE)
	cgGLDisableProfile(pTreeView->vertProfile1);
	cgGLDisableProfile(pTreeView->fragmentProfile1);
#endif

	::glPopMatrix();
	::glPopMatrix();
}
int CMesh3d::ShowVertexMUMou()
{
	return 1;
}
int CMesh3d::ShowVertexMUAngle()
{
	return 1;
}
int CMesh3d::ShowTriangleMU()
{
	return 1;
}

//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Copy
//********************************************
void CMesh3d::Copy(CMesh3d* pMesh)
{
	// Vertices
	int NbVertex = pMesh->NbVertex();
	m_ArrayVertex.SetSize(NbVertex);
	for (int i = 0; i < NbVertex; i++)
		m_ArrayVertex.SetAt(i, new CVertex3d(pMesh->GetVertex(i)));

	for (int i = 0; i < pMesh->NbVertex(); i++)
	{
		pMesh->GetVertex(i)->m_num = i;
	}

	// Faces
	int NbFace = pMesh->NbFace();
	m_ArrayFace.SetSize(NbFace);
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFace = pMesh->GetFace(i);
		//m_ArrayFace.SetAt(i,new CFace3d(
		//	m_ArrayVertex[pMesh->IndexFrom(pFace->v1())],
		//	m_ArrayVertex[pMesh->IndexFrom(pFace->v2())],
		//	m_ArrayVertex[pMesh->IndexFrom(pFace->v3())]));
		m_ArrayFace.SetAt(i, new CFace3d(
			m_ArrayVertex[pFace->v1()->m_num],
			m_ArrayVertex[pFace->v2()->m_num],
			m_ArrayVertex[pFace->v3()->m_num]));
	}

	m_ArraySharpEdges = pMesh->m_ArraySharpEdges;
	m_ArraySharpEdges_Isboundary = pMesh->m_ArraySharpEdges_Isboundary;
	m_ArraySharpVertexIndex = pMesh->m_ArraySharpVertexIndex;
	m_LandMarkEndPoints = pMesh->m_LandMarkEndPoints;
	m_FixVerticesArray = pMesh->m_FixVerticesArray;
	m_LandMarkArray = pMesh->m_LandMarkArray;

	// Transform
	m_Transform.Copy(pMesh->GetTransform());
}


//********************************************
// GetType
//********************************************
int CMesh3d::GetType()
{
	return TYPE_MESH3D;
}

//********************************************
// IsValid
//********************************************
int CMesh3d::IsValid()
{
	int NbFace = m_ArrayFace.GetSize();
	for (int i = 0; i < NbFace; i++)
		if (!m_ArrayFace[i]->IsValid())
			return 0;
	return 1;
}

CVertex3d* CMesh3d::AddVertex_NODUPLICATE(double x, double y, double z)
{
	CVertex3d* pNewVertex = new CVertex3d(x, y, z);
	int size = m_ArrayVertex.GetSize();
	for (int i = 0; i < size; i++)
	{
		CVertex3d* pV = m_ArrayVertex[i];
		double dis = Distance(pV, pNewVertex);
		if (dis < 1e-8)
		{
			delete pNewVertex;
			return pV;
		}
	}

	m_ArrayVertex.Add(pNewVertex);
	return pNewVertex;
}
//********************************************
// DeleteVertex
//********************************************
int CMesh3d::DeleteVertex(CVertex3d* pVertex)
{
	int size = m_ArrayVertex.GetSize();
	for (int i = 0; i < size; i++)
	{
		CVertex3d* pV = m_ArrayVertex[i];
		if (pV == pVertex)
		{
			m_ArrayVertex.RemoveAt(i);
			delete pVertex;
			return 1;
		}
	}
	return 0;
}

//********************************************
// DeleteVertex
//********************************************
int CMesh3d::DeleteVertex(int index)
{
	if (index < m_ArrayVertex.GetSize())
	{
		CVertex3d* pVertex = (CVertex3d*)m_ArrayVertex[index];
		m_ArrayVertex.RemoveAt(index);
		delete pVertex;
		return 1;
	}
	return 0;
}

void CMesh3d::SetVertexFlagAsZero()
{
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex3d = GetVertex(i);
		pVertex3d->m_Flag = 0;
	}

}

void CMesh3d::SetVertexFlagAsItsIndex()
{
	if (m_SetVertexFlagAsItsIndex)
		return;

	m_SetVertexFlagAsItsIndex = TRUE;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex3d = GetVertex(i);
		pVertex3d->m_Flag = i;
	}

}
void CMesh3d::SetFaceFlagAsItsIndex()
{
	//if (m_SetVertexFlagAsItsIndex)
	//	return;

	//m_SetVertexFlagAsItsIndex = TRUE;
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace3d = GetFace(i);
		pFace3d->m_Flag = i;
	}

}

int CMesh3d::DeleteFace(CFace3d* pFace)
{
	int size = m_ArrayFace.GetSize();
	for (int i = 0; i < size; i++)
	{
		CFace3d* pF = m_ArrayFace[i];
		if (pF == pFace)
		{
			m_ArrayFace.RemoveAt(i);
			delete pFace;
			return 1;
		}
	}
	return 0;
}

int CMesh3d::DeleteFace(int index)
{
	if (index < m_ArrayFace.GetSize())
	{
		CFace3d* pFace = (CFace3d*)m_ArrayFace[index];
		m_ArrayFace.RemoveAt(index);
		delete pFace;
		return 1;
	}
	return 0;
}

//********************************************
// DeleteFace
//********************************************
int CMesh3d::DeleteFaceNbNeighbors(int NbNeighbor)
{
	TRACE("Delete faces which has %d neighbors...", NbNeighbor);
	int deleted = 0;
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		CFace3d* pFace = m_ArrayFace[i];
		if (pFace->NbFaceNeighbor() == NbNeighbor)
		{
			m_ArrayFace.RemoveAt(i);
			delete pFace;
			i--;
			deleted++;
		}
	}
	TRACE("%d face(s) deleted\n", deleted);
	return deleted;
}


//////////////////////////////////////////////
// RANGE
//////////////////////////////////////////////

//********************************************
// Range
//********************************************
void CMesh3d::Range(int coord,
	float* min,
	float* max)
{
	ASSERT(coord >= 0 && coord <= 2);
	int NbVertex = m_ArrayVertex.GetSize();
	float Min = m_ArrayVertex[0]->Get(coord);
	float Max = Min;
	for (int i = 1; i < NbVertex; i++)
	{
		float value = m_ArrayVertex[i]->Get(coord);
		if (value < Min)
			Min = value;
		if (value > Max)
			Max = value;
	}
	*min = Min;
	*max = Max;
}

//********************************************
// Range (apply)
//********************************************
void CMesh3d::Range(int coord,
	float min,
	float max)
{
	TRACE("Range mesh...");
	float Min, Max;
	Range(coord, &Min, &Max);
	TRACE("old : (%g,%g) -> (%g %g)", Min, Max, min, max);
	Offset(coord, -Min);
	Scale(coord, (max - min) / (Max - Min));
	Offset(coord, min);
	TRACE("ok\n");
}

//********************************************
// Scale
//********************************************
void CMesh3d::Scale(int coord,
	float scale)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for (int i = 0; i < NbVertex; i++)
		m_ArrayVertex[i]->Set(coord, m_ArrayVertex[i]->Get(coord) * scale);
	m_Modified = 1;
}

//********************************************
// Offset
//********************************************
void CMesh3d::Offset(int coord,
	float offset)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for (int i = 0; i < NbVertex; i++)
		m_ArrayVertex[i]->Set(coord, m_ArrayVertex[i]->Get(coord) + offset);
	m_Modified = 1;
}


//////////////////////////////////////////////
// PROCESSING
//////////////////////////////////////////////

//********************************************
// BuildAdjacency
// From VertexArray and FaceArray, build
// neighboring vertices and faces, using
// edge and vertex sharing
//********************************************
int CMesh3d::BuildAdjacency()
{
	CMesh3d* pMesh = this;

	// Check for valid sizes
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Build adjacency (%d faces, %d vertices)", NbFace, NbVertex);
	if (NbVertex < 3 || NbFace <= 0)
	{
		TRACE("  invalid array sizes\n");
		return 0;
	}

	// At first, clear all neighbors for each face
	//TRACE("  clear face neighbors\n");
	TRACE(".");
	for (int i = 0; i < NbFace; i++)
		for (int k = 0; k < 6; k++)
			m_ArrayFace[i]->f(k, NULL);

	// At first, clear all neighbors for each vertex
	//TRACE("  clear vertex neighbors\n");
	TRACE(".");
	for (int i = 0; i < NbVertex; i++)
	{
		m_ArrayVertex[i]->RemoveAllFaceNeighbor();
		m_ArrayVertex[i]->RemoveAllVertexNeighbor();
	}

	//*********************************************
	// For each face, set face neighboring vertices 
	//*********************************************
	//TRACE("  set face neighboring vertices\n");
	TRACE(".");
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFaceCurrent = m_ArrayFace[i];
		for (int j = 0; j < 3; j++)
			pFaceCurrent->v(j)->AddNeighbor(pFaceCurrent);
	}

	//*********************************************
	// For each vertex, set vertex neighboring,
  // just look on neighboring faces 
	//*********************************************
	//TRACE("  set vertices neighboring vertices\n");
	TRACE(".");
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertexCurrent = m_ArrayVertex[i];
		int NbFaceNeighbor = pVertexCurrent->NbFaceNeighbor();
		for (int j = 0; j < NbFaceNeighbor; j++)
		{
			CFace3d* pFace = pVertexCurrent->GetFaceNeighbor(j);
			for (int k = 0; k < 3; k++)
				pVertexCurrent->AddNeighbor(pFace->v(k));
		}
	}

	//*********************************************
	// For each face, set face neighboring,
  // just look on faces neighboring vertices
	//*********************************************
	//TRACE("  set faces neighboring faces\n");
	TRACE(".");
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFaceCurrent = m_ArrayFace[i];
		// For each edge
		for (int j = 0; j < 3; j++)
		{
			CVertex3d* pVertex = pFaceCurrent->v(j);
			CVertex3d* pNextVertex = pFaceCurrent->v((j + 1) % 3);
			int NbFaceNeighbor = pVertex->NbFaceNeighbor();
			for (int k = 0; k < NbFaceNeighbor; k++)
			{
				// This face contain pVertex
				CFace3d* pFace = pVertex->GetFaceNeighbor(k);
				if (pFace != pFaceCurrent)
					if (pFaceCurrent->f(j) == NULL)
						if (pFace->HasVertex(pVertex))
							if (pFace->HasVertex(pNextVertex))
								pFaceCurrent->f(j, pFace);
			}
		}
	}

	/*
	// Check
	for(i=0;i<NbFace;i++)
	{
		ASSERT(m_ArrayFace[i]->IsValid());
	}
	*/


	TRACE("ok\n");

	return TRUE;
}

//********************************************
// Rebuild
//********************************************
void CMesh3d::Rebuild()
{
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	SetModified(1);
}

/////////////////////////////////////////////
// DEBUG
//////////////////////////////////////////////

//********************************************
// Trace
//********************************************
void CMesh3d::Trace()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();

	TRACE("\n");
	TRACE(" Mesh \n");
	TRACE("Vertex : %d\n", NbVertex);
	TRACE("Face   : %d\n", NbFace);

	for (int i = 0; i < NbVertex; i++)
		((CVertex3d*)m_ArrayVertex[i])->Trace();

	for (int j = 0; j < NbFace; j++)
		((CFace3d*)m_ArrayFace[j])->Trace();
}



//********************************************
// IndexFrom
// Return -1 if failed
//********************************************
int CMesh3d::IndexFrom(CFace3d* pFace)
{
	ASSERT(pFace != NULL);
	int NbFace = m_ArrayFace.GetSize();
	for (int i = 0; i < NbFace; i++)
		if (m_ArrayFace[i] == pFace)
			return i;
	return -1;
}

//********************************************
// IndexFrom
// Return -1 if failed
//********************************************
int CMesh3d::IndexFrom(CVertex3d* pVertex)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for (int i = 0; i < NbVertex; i++)
		if (m_ArrayVertex[i] == pVertex)
			return i;
	return -1;
}

//********************************************
// Move
//********************************************
void CMesh3d::Move(float dx, float dy, float dz)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for (int i = 0; i < NbVertex; i++)
		m_ArrayVertex[i]->Move(dx, dy, dz);
	m_Modified = 1;
}


//********************************************
// FindVertexInFaces
//********************************************
int CMesh3d::FindVertex(CVertex3d* pVertex)
{
	int find = 0;

	int NbFace = m_ArrayFace.GetSize();
	for (int j = 0; j < NbFace; j++)
		if (m_ArrayFace[j]->HasVertex(pVertex))
		{
			find = 1;
			TRACE("Find vertex [%x] in face %d\n", pVertex, j);
		}

	int NbVertex = m_ArrayVertex.GetSize();
	for (int j = 0; j < NbVertex; j++)
		if (m_ArrayVertex[j] == pVertex)
		{
			find = 1;
			TRACE("Find vertex [%x] at position %d\n", pVertex, j);
		}

	for (int j = 0; j < NbVertex; j++)
	{
		CVertex3d* pV = m_ArrayVertex[j];
		if (pV->HasNeighbor(pVertex))
		{
			find = 1;
			TRACE("Find vertex [%x] in neighbors of vertex %d\n", pVertex, j);
		}
	}

	return find;
}
CFace3d* CMesh3d::FindFace(CVertex3d* pVertex1, CVertex3d* pVertex2, CVertex3d*& pVertex3)
{
	//Given two vertices, find the face with the two vertices
	for (int i = 0; i < NbFace(); i++)
	{
		CFace3d* pFace = GetFace(i);
		for (int j = 0; j < 3; j++)
		{
			if (pFace->v(j) == pVertex1 && pFace->v((j + 1) % 3) == pVertex2)
			{
				pVertex3 = pFace->v((j + 2) % 3);
				return pFace;
			}
		}
	}

	return NULL;
}
//********************************************
// FindFace
//********************************************
int CMesh3d::FindFace(CFace3d* pFace)
{
	int find = 0;

	int NbFace = m_ArrayFace.GetSize();
	for (int j = 0; j < NbFace; j++)
	{
		CFace3d* pF = m_ArrayFace[j];
		if (pF == pFace)
		{
			find = 1;
			TRACE("Find face [%x] in mesh (index : %d)\n", pFace, j);
		}

		for (int i = 0; i < 3; i++)
			if (pF->f(i) == pFace)
			{
				find = 1;
				TRACE("Find face [%x] in neighbors %d of face %d\n", pFace, i, j);
			}
	}

	return find;
}


double CMesh3d::TriangleArea(Vector3D v1, Vector3D v2, Vector3D v3)
{
	return Length((v1 - v2) ^ (v1 - v3)) / 2;
}
//////////////////////////////////////////////
// NORMALS
//////////////////////////////////////////////

double CMesh3d::CalculateMaximalMeanCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	double maximalmeanvature = -1e10;
	int maxindex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		double meancurvature = Length(pVertex->m_LaplaceOperatorVec) / 2;
		if (meancurvature > maximalmeanvature)
		{
			maximalmeanvature = meancurvature;
			maxindex = i;
		}
	}
	return maximalmeanvature;
}

double CMesh3d::CalculateMinimalMeanCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	double minimalmeanvature = 1e10;
	int minindex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		double meancurvature = Length(pVertex->m_LaplaceOperatorVec) / 2;
		if (meancurvature < minimalmeanvature)
		{
			minindex = i;
			minimalmeanvature = meancurvature;
		}
	}
	return minimalmeanvature;
}
double CMesh3d::CalculateMaximalGaussCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	double maximalcurvature = -1e10;
	int maxindex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		double meancurvature = (pVertex->m_GaussCurvature);
		if (meancurvature > maximalcurvature)
		{
			maximalcurvature = meancurvature;
			maxindex = i;
		}
	}
	return maximalcurvature;
}

double CMesh3d::CalculateMinimalGaussCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	double minimalcurvature = 1e10;
	int minindex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		double curvature = (pVertex->m_GaussCurvature);
		if (curvature < minimalcurvature)
		{
			minimalcurvature = curvature;
			minindex = i;
		}
	}
	return minimalcurvature;
}
int CMesh3d::CalculateGaussCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Calculate normal per vertex (%d faces, %d vertices)...", NbFace, NbVertex);
	Vector3D LaplaceOperatorVec;
	CVertex3d* pPreOppositeVertex, * pOppositeVertex, * pNextOppositeVertex;
	double angle = PI * 2;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		angle = PI * 2;
		//m_ArrayVertexNeighbor
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			pOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pNextOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];

			double l1 = Distance(pVertex, pOppositeVertex);
			double l2 = Distance(pVertex, pNextOppositeVertex);
			double l3 = Distance(pOppositeVertex, pNextOppositeVertex);

			double alpha = acos((SQR(l1) + SQR(l2) - SQR(l3)) / (2 * l1 * l2));

			angle = angle - alpha;
		}
		pVertex->m_GaussCurvature = angle;
	}
	return 1;
}
int CMesh3d::SmoothGaussCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	double angle;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		angle = pVertex->m_GaussCurvature;
		//m_ArrayVertexNeighbor
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			angle += pVertex->m_ArrayVertexNeighbor[j]->m_GaussCurvature;
		}
		angle = angle / (pVertex->m_ArrayVertexNeighbor.GetSize() + 1);
		pVertex->tempval = angle;
	}
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		pVertex->m_GaussCurvature = pVertex->tempval;
	}
	return 1;
}
int CMesh3d::SharpGaussCurvature()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	double angle;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		angle = pVertex->m_GaussCurvature * (pVertex->m_ArrayVertexNeighbor.GetSize());
		//m_ArrayVertexNeighbor
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			angle -= pVertex->m_ArrayVertexNeighbor[j]->m_GaussCurvature;
		}
		//angle = angle / (pVertex->m_ArrayVertexNeighbor.GetSize() + 1);
		pVertex->tempval = angle;
	}
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		pVertex->m_GaussCurvature = pVertex->tempval;
	}
	return 1;
}
int CMesh3d::CalculateLaplaceBeltramiOperator()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Calculate normal per vertex (%d faces, %d vertices)...", NbFace, NbVertex);
	Vector3D LaplaceOperatorVec;
	CVertex3d* pPreOppositeVertex, * pOppositeVertex, * pNextOppositeVertex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		pVertex->m_Flag = i;
	}
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];

		LaplaceOperatorVec.SetValue(0, 0, 0);
		//m_ArrayVertexNeighbor
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			pPreOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j - 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pNextOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];

			double l1 = Distance(pVertex, pOppositeVertex);
			double l2 = Distance(pVertex, pPreOppositeVertex);
			double l3 = Distance(pVertex, pNextOppositeVertex);
			double l4 = Distance(pOppositeVertex, pPreOppositeVertex);
			double l5 = Distance(pOppositeVertex, pNextOppositeVertex);

			if (fabs(l2) < 1e-5 || fabs(l4) < 1e-5 || fabs(l3) < 1e-5 || fabs(l5) < 1e-5)
				continue;

			double alpha = acos((SQR(l2) + SQR(l4) - SQR(l1)) / (2 * l2 * l4));
			double beta = acos((SQR(l3) + SQR(l5) - SQR(l1)) / (2 * l3 * l5));

			double weight = 0;
			if (fabs(beta) > 1e-2 && fabs(alpha) > 1e-2)
				weight = (cos(alpha) / sin(alpha) + cos(beta) / sin(beta));

			LaplaceOperatorVec[0] += weight * (pOppositeVertex->x() - pVertex->x());
			LaplaceOperatorVec[1] += weight * (pOppositeVertex->y() - pVertex->y());
			LaplaceOperatorVec[2] += weight * (pOppositeVertex->z() - pVertex->z());
		}

		//Compute the Ai
		Vector3D v1, v2, v3, v1v2_middlepoint, v1v3_middlepoint;
		Vector3D barycentricpoint;
		double Ai = 0;
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			pOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pNextOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];

			v1.SetValue(pVertex->x(), pVertex->y(), pVertex->z());
			v2.SetValue(pOppositeVertex->x(), pOppositeVertex->y(), pOppositeVertex->z());
			v3.SetValue(pNextOppositeVertex->x(), pNextOppositeVertex->y(), pNextOppositeVertex->z());

			v1v2_middlepoint = (v1 + v2) / 2;
			v1v3_middlepoint = (v1 + v3) / 2;
			barycentricpoint = (v1 + v2 + v3) / 3;

			Ai += TriangleArea(v1, v1v2_middlepoint, barycentricpoint);
			Ai += TriangleArea(v1, v1v3_middlepoint, barycentricpoint);
		}
		if (pVertex->m_ArrayVertexNeighbor.GetSize() >= 1 && fabs(Ai) > 0.01)
			LaplaceOperatorVec = LaplaceOperatorVec / (Ai * 2);
		pVertex->m_LaplaceOperatorVec = LaplaceOperatorVec;
	}

	return 1;
}
int CMesh3d::CalculateLaplaceBeltramiOperator_NoArea()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Calculate normal per vertex (%d faces, %d vertices)...", NbFace, NbVertex);
	Vector3D LaplaceOperatorVec;
	CVertex3d* pPreOppositeVertex, * pOppositeVertex, * pNextOppositeVertex;
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];

		LaplaceOperatorVec.SetValue(0, 0, 0);
		//m_ArrayVertexNeighbor
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			pPreOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j - 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pNextOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];

			double l1 = Distance(pVertex, pOppositeVertex);
			double l2 = Distance(pVertex, pPreOppositeVertex);
			double l3 = Distance(pVertex, pNextOppositeVertex);
			double l4 = Distance(pOppositeVertex, pPreOppositeVertex);
			double l5 = Distance(pOppositeVertex, pNextOppositeVertex);

			double alpha = acos((SQR(l2) + SQR(l4) - SQR(l1)) / (2 * l2 * l4));
			double beta = acos((SQR(l3) + SQR(l5) - SQR(l1)) / (2 * l3 * l5));

			double weight = 0;
			if (fabs(beta) > 1e-2 && fabs(alpha) > 1e-2)
				weight = (cos(alpha) / sin(alpha) + cos(beta) / sin(beta));

			LaplaceOperatorVec[0] += weight * (pOppositeVertex->x() - pVertex->x());
			LaplaceOperatorVec[1] += weight * (pOppositeVertex->y() - pVertex->y());
			LaplaceOperatorVec[2] += weight * (pOppositeVertex->z() - pVertex->z());
		}

		//Compute the Ai
		/*Vector3D v1, v2, v3, v1v2_middlepoint, v1v3_middlepoint;
		Vector3D barycentricpoint;
		double Ai = 0;
		for (int j = 0; j < pVertex->m_ArrayVertexNeighbor.GetSize(); j++)
		{
			pOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];
			pNextOppositeVertex = pVertex->m_ArrayVertexNeighbor[(j + 1 + pVertex->m_ArrayVertexNeighbor.GetSize()) % pVertex->m_ArrayVertexNeighbor.GetSize()];

			v1.SetValue(pVertex->x(), pVertex->y(), pVertex->z());
			v2.SetValue(pOppositeVertex->x(), pOppositeVertex->y(), pOppositeVertex->z());
			v3.SetValue(pNextOppositeVertex->x(), pNextOppositeVertex->y(), pNextOppositeVertex->z());

			v1v2_middlepoint = (v1 + v2) / 2;
			v1v3_middlepoint = (v1 + v3) / 2;
			barycentricpoint = (v1 + v2 + v3) / 3;

			Ai += TriangleArea(v1, v1v2_middlepoint, barycentricpoint);
			Ai += TriangleArea(v1, v1v3_middlepoint, barycentricpoint);
		}

		LaplaceOperatorVec = LaplaceOperatorVec / (Ai * 2);*/
		pVertex->m_LaplaceOperatorVec = LaplaceOperatorVec;
	}

	return 1;
}
//********************************************
// CalculateNormalPerVertex
//********************************************
int CMesh3d::CalculateNormalPerVertex(void)
{
	if (m_SetVertexNormal)
		return 1;

	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Calculate normal per vertex (%d faces, %d vertices)...", NbFace, NbVertex);
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		int NbNormal = 0;
		CVector3d vector;
		int NbFaceNeighbor = pVertex->NbFaceNeighbor();
		for (int j = 0; j < NbFaceNeighbor; j++)
		{
			CFace3d* pFace = pVertex->GetFaceNeighbor(j);
			NbNormal += 1;
			vector += pFace->GetNormal();
		}
		if (NbNormal >= 1)
		{
			vector.NormalizeL2();
			pVertex->SetNormal(vector);
		}
	}
	m_Modified = 1;
	m_SetVertexNormal = TRUE;
	TRACE("ok\n");

	return 1;
}

//********************************************
// CalculateNormalPerFace
//********************************************
int CMesh3d::CalculateNormalPerFace(void)
{
	if (m_SetFaceNormal)
		return 1;

	int size = m_ArrayFace.GetSize();
	TRACE("Calculate normal per face (%d faces)...", size);
	for (int i = 0; i < size; i++)
	{
		CFace3d* pFace = m_ArrayFace[i];
		if (pFace->IsValid())
			pFace->CalculateNormal();
	}
	m_Modified = 1;
	m_SetFaceNormal = 1;
	TRACE("ok\n");
	return 1;
}




//********************************************
// ColorSharpEdges
//********************************************
int CMesh3d::ColorSharpEdge(float threshold,
	CColor& color)
{
	int NbFace = m_ArrayFace.GetSize();

	TRACE(" Start ColorSharpEdges\n");
	TRACE("   Faces : %d\n", NbFace);
	for (int i = 0; i < NbFace; i++)
		m_ArrayFace[i]->ColorSharpEdge(threshold, color);
	TRACE(" End ColorSharpEdges\n");

	return 1;
}


//********************************************
// SetNormalBinding
//********************************************
void CMesh3d::SetNormalBinding(int type)
{
	m_NormalBinding = type;
	m_Modified = 1;
}

//********************************************
// GetNormalBinding
//********************************************
int CMesh3d::GetNormalBinding(void)
{
	return m_NormalBinding;
}

//********************************************
// SetColorBinding
//********************************************
void CMesh3d::SetColorBinding(int type)
{
	m_ColorBinding = type;
	m_Modified = 1;
}

//********************************************
// SetColorBinding
//********************************************
void CMesh3d::SetColor(CColor& color)
{
	int size = m_ArrayFace.GetSize();
	for (int i = 0; i < size; i++)
		m_ArrayFace[i]->SetColor(color);

	size = m_ArrayVertex.GetSize();
	for (int i = 0; i < size; i++)
		m_ArrayVertex[i]->SetColor(color);

	m_Modified = 1;
}

//********************************************
// GetColorBinding
//********************************************
int CMesh3d::GetColorBinding(void)
{
	return m_ColorBinding;
}

//********************************************
// SetFlagOnFaces
//********************************************
void CMesh3d::SetFlagOnFaces(int flag)
{
	int size = m_ArrayFace.GetSize();
	for (int i = 0; i < size; i++)
		m_ArrayFace[i]->SetFlag(flag);
}

//********************************************
// GetMaxFlagOnFaces
//********************************************
int CMesh3d::GetMaxFlagOnFaces()
{
	int size = m_ArrayFace.GetSize();
	int max = 0;
	for (int i = 0; i < size; i++)
	{
		int tmp = m_ArrayFace[i]->GetFlag();
		max = (tmp > max) ? tmp : max;
	}
	return max;
}

//********************************************
// SetFlagOnVertices
//********************************************
void CMesh3d::SetFlagOnVertices(int flag)
{
	int size = m_ArrayVertex.GetSize();
	for (int i = 0; i < size; i++)
		m_ArrayVertex[i]->SetFlag(flag);
}

void CMesh3d::SetFlagOnVertices_ArrayIndex()
{
	if (m_SetFlagOnVertices_ArrayIndex)
		return;
	int size = m_ArrayVertex.GetSize();
	for (int i = 0; i < size; i++)
	{
		m_ArrayVertex[i]->SetFlag(i);
	}
	m_SetFlagOnVertices_ArrayIndex = 1;
}

//********************************************
// GetFirstVertexWithFlag
//********************************************
float CMesh3d::GetMeanLengthEdge()
{
	int size = m_ArrayVertex.GetSize();
	double sum = 0;
	for (int i = 0; i < size; i++)
		sum += m_ArrayVertex[i]->GetMeanLengthEdgeAround();
	if (size)
		return (float)sum / size;
	else
		return 0.0f;
}





//////////////////////////////////////////////
// SUBDIVISION
//////////////////////////////////////////////

//********************************************
// Alpha
// From Piecewise smooth reconstruction (Hoppe)
//********************************************
float CMesh3d::Alpha(int n)
{
	float tmp = 3.0f + 2.0f * (float)cos(2.0f * 3.14159265359f / (float)n);
	float a = 5.0f / 8.0f - (tmp * tmp) / 64.0f;
	return (n * (1 - a) / a);
}


//********************************************
// SubdivisionLoop
// From Piecewise smooth reconstruction (Hoppe)
// Charles Loop (SIGGRAPH 94)
//********************************************
int CMesh3d::SubdivisionLoop(int MoveOnBundary /* = 1 */)
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();

	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start loop's subdivision (%d faces, %d vertices)\n", NbFace, NbVertex);

	TRACE("  subdivision info (%d vertices)\n", m_ArrayVertex.GetSize());

	// For each face
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFace = m_ArrayFace[i];

		// Check face has at least 1 neighbor
		if (pFace->NbFaceNeighbor() == 0)
			continue;

		// Check valid neighboring
		if (!pFace->IsValid())
			continue;

		// On each edge
		for (int IndexEdge = 0; IndexEdge < 3; IndexEdge++)
		{
			// Get IndexEdge on neighbor
			int IndexCurrent, IndexNeighbor;
			CFace3d* pFaceNeighbor = pFace->f(IndexEdge);

			// No neighbor on this edge, go to next
			if (pFaceNeighbor == NULL)
				continue;

			VERIFY(pFace->Share2Vertex(pFaceNeighbor, &IndexCurrent, &IndexNeighbor));
			ASSERT(IndexCurrent == IndexEdge);

			// If neighboring face has been treated, then get vertex
			// and go to next step
			if (pFaceNeighbor->GetFlag())
			{
				CVertex3d* pVertex = pFaceNeighbor->v(3 + (IndexNeighbor % 3));
				pFace->v(3 + (IndexEdge % 3), pVertex);
				continue;
			}

			// Vertex weighting
			// 0 & 1 : weight : 3, 2 & 3 : weight : 1
			CVertex3d* pVertex[4];

			// Weight : 3
			pVertex[0] = pFace->v(IndexEdge);
			pVertex[1] = pFace->v((IndexEdge + 1) % 3);
			// Weight : 1
			pVertex[2] = pFace->v((IndexEdge + 2) % 3);
			pVertex[3] = pFaceNeighbor->v((IndexNeighbor + 2) % 3);

			ASSERT(pVertex[0] != NULL && pVertex[1] != NULL && pVertex[2] != NULL && pVertex[3] != NULL);

			// For each composant
			float coord[3];
			for (int k = 0; k < 3; k++)
				coord[k] = (3.0f * (pVertex[0]->Get(k) + pVertex[1]->Get(k)) +
					pVertex[2]->Get(k) + pVertex[3]->Get(k)) / 8.0f;

			// Add vertex to global mesh array, and set face's vertex
			CVertex3d* pNewVertex = new CVertex3d(coord[0], coord[1], coord[2]);
			m_ArrayVertex.Add(pNewVertex);

			pFace->v(3 + IndexCurrent, pNewVertex);
		}

		// Set flag
		pFace->SetFlag(1);
	}

	//*****************************
	// Create faces
	//*****************************
	TRACE("  creating faces (%d faces)\n", NbFace);
	// For each valid face
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFace = m_ArrayFace[i];
		pFace->SetFlag(0);

		// Valid face
		int NbVertex = pFace->NbVertex();
		CFace3d* pNewFace;

		switch (NbVertex)
		{
		case 4:
			// Create one face

			// On edge 0
			if (pFace->v(3) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(2));
				m_ArrayFace.Add(pNewFace);

				// Move current face
				pFace->v(1, pFace->v(3));
			}
			else
				// On edge 1
				if (pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(0), pFace->v(4), pFace->v(2));
					m_ArrayFace.Add(pNewFace);

					// Move current face
					pFace->v(2, pFace->v(4));
				}
				else
					// On edge 2
					if (pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(5), pFace->v(1), pFace->v(2));
						m_ArrayFace.Add(pNewFace);

						// Move current face
						pFace->v(2, pFace->v(5));
					}
			break;
		case 5:
			// Create two faces

			// On edge 0 & 2
			if (pFace->v(3) != NULL &&
				pFace->v(5) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(0), pFace->v(3), pFace->v(5));
				m_ArrayFace.Add(pNewFace);
				pNewFace = new CFace3d(pFace->v(5), pFace->v(3), pFace->v(2));
				m_ArrayFace.Add(pNewFace);

				// Move current face
				pFace->v(0, pFace->v(3));
			}
			else
				// On edge 0 & 1
				if (pFace->v(3) != NULL &&
					pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					pNewFace = new CFace3d(pFace->v(0), pFace->v(3), pFace->v(4));
					m_ArrayFace.Add(pNewFace);

					// Move current face
					pFace->v(1, pFace->v(4));
				}
				else
					// On edge 1 & 2
					if (pFace->v(4) != NULL &&
						pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(1), pFace->v(4), pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						pNewFace = new CFace3d(pFace->v(4), pFace->v(2), pFace->v(5));
						m_ArrayFace.Add(pNewFace);

						// Move current face
						pFace->v(2, pFace->v(5));
					}
			break;
		case 6:
			// Create three faces

			// First (v3,v1,v4)
			pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(4));
			m_ArrayFace.Add(pNewFace);
			// Second (v3,v4,v5)
			pNewFace = new CFace3d(pFace->v(3), pFace->v(4), pFace->v(5));
			m_ArrayFace.Add(pNewFace);
			// Third (v5,v4,v2)
			pNewFace = new CFace3d(pFace->v(5), pFace->v(4), pFace->v(2));
			m_ArrayFace.Add(pNewFace);

			// Move current face
			pFace->v(1, pFace->v(3));
			pFace->v(2, pFace->v(5));

			break;
		}
		// Remove subdivision info
		for (int k = 3; k < 6; k++)
			pFace->v(k, NULL);

	}
	TRACE("  end creating faces (%d faces)\n", m_ArrayFace.GetSize());


	//*****************************
	// Move original vertices
	//*****************************

	// Copy
	TRACE("  copy\n");
	CArray3d<CVertex3d> ArrayVertex;
	ArrayVertex.SetSize(NbVertex);
	for (int i = 0; i < NbVertex; i++)
		ArrayVertex.SetAt(i, new CVertex3d);

	// For each vertex (at least 3 neighbors)
	for (int i = 0; i < NbVertex; i++)
	{

		CVertex3d* pVertex = m_ArrayVertex[i];

		// Do we move boundaries ??
		if (!MoveOnBundary)
			if (pVertex->IsOnBoundary())
			{
				ArrayVertex[i]->Set(pVertex);
				continue;
			}

		int n = pVertex->NbVertexNeighbor();
		float alpha = Alpha(n);
		float tmp = alpha + (float)n;

		// For each composant
		for (unsigned int j = 0; j < 3; j++)
		{
			float value = alpha * pVertex->Get(j);
			for (int k = 0; k < n; k++)
				value += pVertex->GetVertexNeighbor(k)->Get(j);
			value /= tmp;
			ArrayVertex[i]->Set(j, value);
		}
	}

	// Restore
	TRACE("  restore\n");
	for (int i = 0; i < NbVertex; i++)
		for (unsigned int j = 0; j < 3; j++)
			m_ArrayVertex[i]->Set(j, ArrayVertex[i]->Get(j));

	ArrayVertex.Free();

	// Rebuild adjacency and normals
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	m_Modified = 1;

	TRACE("End loop's subdivision (%d faces, %d vertices)\n",
		m_ArrayFace.GetSize(), m_ArrayVertex.GetSize());

	return 1;
}


//********************************************
// Subdivision
// Simple : 1->4
//********************************************
int CMesh3d::Subdivision(void)
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();

	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start subdivision (%d faces, %d vertices)\n", NbFace, NbVertex);

	TRACE("  subdivision info (%d vertices)\n", m_ArrayVertex.GetSize());

	// For each face
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFace = m_ArrayFace[i];

		// On each edge
		for (int IndexEdge = 0; IndexEdge < 3; IndexEdge++)
		{
			// Get IndexEdge on neighbor
			int IndexCurrent, IndexNeighbor;
			CFace3d* pFaceNeighbor = pFace->f(IndexEdge);

			// No neighbor on this edge, go to next
			if (pFaceNeighbor != NULL)
			{

				VERIFY(pFace->Share2Vertex(pFaceNeighbor, &IndexCurrent, &IndexNeighbor));
				ASSERT(IndexCurrent == IndexEdge);

				// If neighboring face has been treated, then get vertex
				// and go to next step
				if (pFaceNeighbor->GetFlag())
				{
					CVertex3d* pVertex = pFaceNeighbor->v(3 + (IndexNeighbor % 3));
					pFace->v(3 + (IndexEdge % 3), pVertex);
					continue;
				}
			}

			// Vertex weighting
			// 0 & 1 : weight : 1
			CVertex3d* pVertex[2];

			// Weight : 1
			pVertex[0] = pFace->v(IndexEdge);
			pVertex[1] = pFace->v((IndexEdge + 1) % 3);

			ASSERT(pVertex[0] != NULL && pVertex[1] != NULL);

			// For each composant
			float coord[3];
			for (int k = 0; k < 3; k++)
				coord[k] = (pVertex[0]->Get(k) + pVertex[1]->Get(k)) / 2.0f;

			// Add vertex to global mesh array, and set face's vertex
			CVertex3d* pNewVertex = new CVertex3d(coord[0], coord[1], coord[2]);
			m_ArrayVertex.Add(pNewVertex);

			pFace->v(3 + IndexEdge, pNewVertex);
		}

		// Set flag
		pFace->SetFlag(1);
	}

	//*****************************
	// Create faces
	//*****************************
	TRACE("  creating faces (%d faces)\n", NbFace);
	// For each valid face
	for (int i = 0; i < NbFace; i++)
	{
		CFace3d* pFace = m_ArrayFace[i];
		pFace->SetFlag(0);

		// Valid face
		int NbVertex = pFace->NbVertex();
		CFace3d* pNewFace;

		switch (NbVertex)
		{
		case 4:
			// Create one face

			// On edge 0
			if (pFace->v(3) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(2));
				m_ArrayFace.Add(pNewFace);

				// Move current face
				pFace->v(1, pFace->v(3));
			}
			else
				// On edge 1
				if (pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(0), pFace->v(4), pFace->v(2));
					m_ArrayFace.Add(pNewFace);

					// Move current face
					pFace->v(2, pFace->v(4));
				}
				else
					// On edge 2
					if (pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(5), pFace->v(1), pFace->v(2));
						m_ArrayFace.Add(pNewFace);

						// Move current face
						pFace->v(2, pFace->v(5));
					}
			break;
		case 5:
			// Create two faces

			// On edge 0 & 2
			if (pFace->v(3) != NULL &&
				pFace->v(5) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(0), pFace->v(3), pFace->v(5));
				m_ArrayFace.Add(pNewFace);
				pNewFace = new CFace3d(pFace->v(5), pFace->v(3), pFace->v(2));
				m_ArrayFace.Add(pNewFace);

				// Move current face
				pFace->v(0, pFace->v(3));
			}
			else
				// On edge 0 & 1
				if (pFace->v(3) != NULL &&
					pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					pNewFace = new CFace3d(pFace->v(0), pFace->v(3), pFace->v(4));
					m_ArrayFace.Add(pNewFace);

					// Move current face
					pFace->v(1, pFace->v(4));
				}
				else
					// On edge 1 & 2
					if (pFace->v(4) != NULL &&
						pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(1), pFace->v(4), pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						pNewFace = new CFace3d(pFace->v(4), pFace->v(2), pFace->v(5));
						m_ArrayFace.Add(pNewFace);

						// Move current face
						pFace->v(2, pFace->v(5));
					}
			break;
		case 6:
			// Create three faces

			// First (v3,v1,v4)
			pNewFace = new CFace3d(pFace->v(3), pFace->v(1), pFace->v(4));
			m_ArrayFace.Add(pNewFace);
			// Second (v3,v4,v5)
			pNewFace = new CFace3d(pFace->v(3), pFace->v(4), pFace->v(5));
			m_ArrayFace.Add(pNewFace);
			// Third (v5,v4,v2)
			pNewFace = new CFace3d(pFace->v(5), pFace->v(4), pFace->v(2));
			m_ArrayFace.Add(pNewFace);

			// Move current face
			pFace->v(1, pFace->v(3));
			pFace->v(2, pFace->v(5));

			break;
		}
		// Remove subdivision info
		for (int k = 3; k < 6; k++)
			pFace->v(k, NULL);

	}
	TRACE("  end creating faces (%d faces)\n", m_ArrayFace.GetSize());

	// Rebuild adjacency and normals
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	m_Modified = 1;

	TRACE("End subdivision (%d faces, %d vertices)\n",
		m_ArrayFace.GetSize(), m_ArrayVertex.GetSize());

	return 1;
}

//********************************************
// Smooth
// 30/09/98
//********************************************
int CMesh3d::Smooth(int MoveOnBundary /* = 1 */)
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();

	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start smoothing (%d faces, %d vertices)", NbFace, NbVertex);

	//*****************************
	// Move original vertices
	//*****************************

	// Copy
	TRACE(".");
	CArray3d<CVertex3d> ArrayVertex;
	ArrayVertex.SetSize(NbVertex);
	for (int i = 0; i < NbVertex; i++)
		ArrayVertex.SetAt(i, new CVertex3d);

	// For each vertex (at least 3 neighbors)
	for (int i = 0; i < NbVertex; i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];

		if (!MoveOnBundary)
			if (pVertex->IsOnBoundary())
			{
				ArrayVertex[i]->Set(pVertex);
				continue;
			}

		int n = pVertex->NbVertexNeighbor();
		float alpha = Alpha(n);
		float tmp = alpha + (float)n;
		// For each composant
		for (unsigned int j = 0; j < 3; j++)
		{
			float value = alpha * pVertex->Get(j);
			for (int k = 0; k < n; k++)
				value += pVertex->GetVertexNeighbor(k)->Get(j);
			value /= tmp;
			ArrayVertex[i]->Set(j, value);
		}
	}

	// Restore
	TRACE(".");
	for (int i = 0; i < NbVertex; i++)
		for (unsigned int j = 0; j < 3; j++)
			m_ArrayVertex[i]->Set(j, ArrayVertex[i]->Get(j));

	// Cleanup
	TRACE(".");
	ArrayVertex.Free();

	TRACE("ok\n");

	// Rebuild adjacency and normals
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	m_Modified = 1;

	return 1;
}






//********************************************
// ColorCurvature
// Each face is colored, function of mean curvature
//********************************************
void CMesh3d::ColorCurvature(CColorRamp* pRamp)
{
	TRACE("Start coloring mesh (curvature)\n");
	int NbVertex = m_ArrayVertex.GetSize();
	TRACE("  %d vertices\n", NbVertex);

	double* pMax = new double[NbVertex];

	// Store curvatures
	for (int i = 0; i < NbVertex; i++)
		pMax[i] = m_ArrayVertex[i]->GetMaxAngleAround();

	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for (int i = 0; i < NbVertex; i++)
	{
		min = (pMax[i] < min) ? pMax[i] : min;
		max = (pMax[i] > max) ? pMax[i] : max;
	}

	min = (min < 0.0f) ? 0.0f : min;
	double amplitude = max - min;//max-min;
	TRACE("  min : %g\n", min);
	TRACE("  max : %g\n", max);
	TRACE("  amplitude : %g\n", amplitude);
	for (int i = 0; i < NbVertex; i++)
	{
		int _grey = (int)((pMax[i] - min) / amplitude * 255.0f);
		unsigned char grey = _grey > 255 ? 255 : _grey;
		m_ArrayVertex[i]->SetColor(CColor(pRamp->Red(grey), pRamp->Green(grey), pRamp->Blue(grey)));
	}

	SetModified();

	TRACE("End coloring mesh (curvature)\n");
}

//********************************************
// ColorSpaceNormal
// Each vertex is colored, function of 
// normal space (sum of angles between
// adjacent faces)
//********************************************
void CMesh3d::ColorNormalSpace(CColorRamp* pRamp)
{
	TRACE("Start coloring mesh (space of normals)\n");
	int NbVertex = m_ArrayVertex.GetSize();
	TRACE("  %d vertices\n", NbVertex);

	double* pSum = new double[NbVertex];

	// Store curvatures
	for (int i = 0; i < NbVertex; i++)
		m_ArrayVertex[i]->NormalMax(&pSum[i]);

	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for (int i = 0; i < NbVertex; i++)
	{
		min = (pSum[i] < min) ? pSum[i] : min;
		max = (pSum[i] > max) ? pSum[i] : max;
	}

	min = (min < 0.0f) ? 0.0f : min;
	double amplitude = max - min;//max-min;
	TRACE("  min : %g\n", min);
	TRACE("  max : %g\n", max);
	TRACE("  amplitude : %g\n", amplitude);
	for (int i = 0; i < NbVertex; i++)
	{
		int _grey = (int)((pSum[i] - min) / amplitude * 255.0f);
		unsigned char grey = _grey > 255 ? 255 : _grey;
		//unsigned char grey = (unsigned char)((pCurvature[i]-min)/amplitude * 255.0f);
		m_ArrayVertex[i]->SetColor(CColor(pRamp->Red(grey), pRamp->Green(grey), pRamp->Blue(grey)));
	}

	SetModified();
	delete[] pSum;

	TRACE("End coloring mesh (space of normals)\n");
}


//********************************************
// ColorCompacity
// Each face is colored, function of face
// compacity
//********************************************
void CMesh3d::ColorCompacity(CColorRamp* pRamp)
{
	TRACE("Start coloring mesh (compacity)\n");
	int NbFace = m_ArrayFace.GetSize();
	TRACE("  %d face(s)\n", NbFace);

	double* pCompacity = new double[NbFace];

	// Store compacity
	for (int i = 0; i < NbFace; i++)
		pCompacity[i] = m_ArrayFace[i]->Compacity();

	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for (int i = 0; i < NbFace; i++)
	{
		min = (pCompacity[i] < min) ? pCompacity[i] : min;
		max = (pCompacity[i] > max) ? pCompacity[i] : max;
	}

	double amplitude = max - min;//max-min;
	TRACE("  min : %g\n", min);
	TRACE("  max : %g\n", max);
	TRACE("  amplitude : %g\n", amplitude);
	for (int i = 0; i < NbFace; i++)
	{
		int _grey = (int)((pCompacity[i] - min) / amplitude * 255.0f);
		unsigned char grey = _grey > 255 ? 255 : _grey;
		//unsigned char grey = (unsigned char)((pCurvature[i]-min)/amplitude * 255.0f);
		m_ArrayFace[i]->SetColor(CColor(pRamp->Red(grey), pRamp->Green(grey), pRamp->Blue(grey)));
	}

	SetModified();
	delete[] pCompacity;

	TRACE("End coloring mesh (compacity)\n");
}

//********************************************
// ColorHeight
// Each vertex is colored, function of height
//********************************************
void CMesh3d::ColorHeight(CColorRamp* pRamp)
{
	TRACE("Start coloring mesh (height)\n");
	int NbVertex = m_ArrayVertex.GetSize();
	TRACE("  %d vertices\n", NbVertex);

	double* pMax = new double[NbVertex];

	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for (int i = 0; i < NbVertex; i++)
	{
		float height = m_ArrayVertex[i]->y();
		min = (height < min) ? height : min;
		max = (height > max) ? height : max;
	}

	double amplitude = max - min;//max-min;
	TRACE("  min : %g\n", min);
	TRACE("  max : %g\n", max);
	TRACE("  amplitude : %g\n", amplitude);
	for (int i = 0; i < NbVertex; i++)
	{
		float height = m_ArrayVertex[i]->y();
		int _grey = (int)((height - min) / amplitude * 255.0f);
		unsigned char grey = _grey > 255 ? 255 : _grey;
		m_ArrayVertex[i]->SetColor(CColor(pRamp->Red(grey), pRamp->Green(grey), pRamp->Blue(grey)));
	}

	delete[] pMax;
	SetModified();

	TRACE("End coloring mesh (height)\n");
}



//////////////////////////////////////////////
// PREDEFINED
//////////////////////////////////////////////

//********************************************
// GenerateBox
//********************************************
int CMesh3d::GenerateBox(float dx,
	float dy,
	float dz)
{
	TRACE("Generate box...");
	CVertex3d* pVertex;

	pVertex = new CVertex3d(-dx / 2, -dy / 2, -dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(-dx / 2, +dy / 2, -dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(+dx / 2, +dy / 2, -dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(+dx / 2, -dy / 2, -dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(-dx / 2, -dy / 2, +dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(-dx / 2, +dy / 2, +dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(+dx / 2, +dy / 2, +dz / 2);
	m_ArrayVertex.Add(pVertex);

	pVertex = new CVertex3d(+dx / 2, -dy / 2, +dz / 2);
	m_ArrayVertex.Add(pVertex);

	CFace3d* pFace;
	pFace = new CFace3d(m_ArrayVertex[0],
		m_ArrayVertex[1],
		m_ArrayVertex[3]);
	pFace->SetNormal(0.0f, 0.0f, -1.0f);
	m_ArrayFace.Add(pFace);

	pFace = new CFace3d(m_ArrayVertex[3],
		m_ArrayVertex[1],
		m_ArrayVertex[2]);
	pFace->SetNormal(0.0f, 0.0f, -1.0f);
	m_ArrayFace.Add(pFace);

	pFace = new CFace3d(m_ArrayVertex[0],
		m_ArrayVertex[4],
		m_ArrayVertex[1]);
	pFace->SetNormal(-1.0f, 0.0f, 0.0f);
	m_ArrayFace.Add(pFace);

	pFace = new CFace3d(m_ArrayVertex[1],
		m_ArrayVertex[4],
		m_ArrayVertex[5]);
	pFace->SetNormal(-1.0f, 0.0f, 0.0f);
	m_ArrayFace.Add(pFace);

	pFace = new CFace3d(m_ArrayVertex[3],
		m_ArrayVertex[2],
		m_ArrayVertex[7]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(1.0f, 0.0f, 0.0f);

	pFace = new CFace3d(m_ArrayVertex[7],
		m_ArrayVertex[2],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(1.0f, 0.0f, 0.0f);

	pFace = new CFace3d(m_ArrayVertex[4],
		m_ArrayVertex[0],
		m_ArrayVertex[3]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, -1.0f, 0.0f);

	pFace = new CFace3d(m_ArrayVertex[7],
		m_ArrayVertex[4],
		m_ArrayVertex[3]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, -1.0f, 0.0f);

	pFace = new CFace3d(m_ArrayVertex[6],
		m_ArrayVertex[4],
		m_ArrayVertex[7]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, 0.0f, 1.0f);

	pFace = new CFace3d(m_ArrayVertex[6],
		m_ArrayVertex[5],
		m_ArrayVertex[4]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, 0.0f, 1.0f);

	pFace = new CFace3d(m_ArrayVertex[1],
		m_ArrayVertex[5],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, 1.0f, 0.0f);

	pFace = new CFace3d(m_ArrayVertex[2],
		m_ArrayVertex[1],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f, 1.0f, 0.0f);

	TRACE("ok\n");

	return 1;
}

//********************************************
// GenerateMap
//********************************************
int CMesh3d::GenerateMap(int line,
	int col,
	float min,
	float max)
{
	TRACE("Generate map...");
	float x, y, z;
	int i, j;

	// Set vertices
	for (i = 0; i < col; i++)
		for (j = 0; j < line; j++)
		{
			x = min + ((float)i / (float)line) * (max - min);
			z = min + ((float)j / (float)line) * (max - min);
			y = (float)(cos(x) * cos(z));
			m_ArrayVertex.Add(new CVertex3d(x, y, z));
		}

	// Set faces
	for (i = 0; i < col - 1; i++)
		for (j = 0; j < line - 1; j++)
		{
			CVertex3d* pVertex1 = m_ArrayVertex[line * i + j];
			CVertex3d* pVertex2 = m_ArrayVertex[line * i + j + 1];
			CVertex3d* pVertex3 = m_ArrayVertex[line * (i + 1) + j + 1];
			CVertex3d* pVertex4 = m_ArrayVertex[line * (i + 1) + j];

			m_ArrayFace.Add(new CFace3d(pVertex1, pVertex2, pVertex3));
			m_ArrayFace.Add(new CFace3d(pVertex1, pVertex3, pVertex4));
		}

	TRACE("ok\n");
	return 1;
}


//********************************************
// GenerateMapFromImage
//********************************************
int CMesh3d::GenerateMap(CTexture* pTexture,
	int width,
	int height,
	int FlagColor /* = 1 */)
{
	// Cleanup
	Free();

	int WidthImage = pTexture->GetWidth();
	int HeightImage = pTexture->GetHeight();
	unsigned char red, green, blue;

	// Vertices
	int k = 0;
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			int xImage = (int)((float)i / (float)width * (float)WidthImage);
			int yImage = (int)((float)j / (float)height * (float)HeightImage);
			int index = m_ArrayVertex.Add(new CVertex3d((float)i, (float)pTexture->Grey(xImage, yImage), (float)j));
			if (FlagColor)
			{
				pTexture->Color(xImage, yImage, &red, &green, &blue);
				m_ArrayVertex[index]->SetColor(CColor(red, green, blue));
			}
		}

	// Faces
	for (int j = 0; j < height - 1; j++)
		for (int i = 0; i < width - 1; i++)
		{
			int index = m_ArrayFace.Add(new CFace3d(m_ArrayVertex[j * width + i + 1],
				m_ArrayVertex[j * width + i],
				m_ArrayVertex[(j + 1) * width + i + 1]));
			m_ArrayFace.Add(new CFace3d(m_ArrayVertex[(j + 1) * width + i + 1],
				m_ArrayVertex[j * width + i],
				m_ArrayVertex[(j + 1) * width + i]));
			if (FlagColor)
			{
				m_ArrayFace[index]->SetColor(*m_ArrayVertex[j * width + i]->GetColor());
				m_ArrayFace[index + 1]->SetColor(*m_ArrayVertex[j * width + i + 1]->GetColor());
			}
		}

	// Rebuild
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();

	return 1;
}

/////////////////////////////////////////////
// INTERSECTION
/////////////////////////////////////////////

//********************************************
// NearestIntersectionWithLine
// Non-optimized
// Nearest -> distance from pV0 to pVertexResult
//********************************************
int CMesh3d::NearestIntersectionWithLine(CVertex3d* pV0,
	CVertex3d* pV1,
	CVertex3d* pVertexResult,
	CFace3d** ppFaceResult,
	int* pNbFaceVisited)
{
	return ::NearestIntersectionWithLine(&m_ArrayFace, pV0, pV1, pVertexResult, ppFaceResult, pNbFaceVisited);
}

/////////////////////////////////////////////
// I/O
/////////////////////////////////////////////

//********************************************
// WriteFile
//********************************************
int CMesh3d::WriteFile(CStdioFile& file)
{
	CString string;
	TRY
	{
		// Comment
		string.Format("# Mesh : %d vertices, %d faces\n",NbVertex(),NbFace());
		file.WriteString(string);

		// First line
		file.WriteString("DEF Mesh-ROOT Transform {\n");

		// Transform
		string.Format("  translation %g %g %g\n",m_Transform.GetTranslation()->x(),
												 m_Transform.GetTranslation()->y(),
																						 m_Transform.GetTranslation()->z());
		file.WriteString(string);
		string.Format("  rotation %g %g %g %g\n",m_Transform.GetRotation()->x(),
											 m_Transform.GetRotation()->y(),
												 m_Transform.GetRotation()->z(),
																					   m_Transform.GetValueRotation() / 360.0f * 2 * 3.14159265359f);
		file.WriteString(string);
		string.Format("  scale %g %g %g\n",m_Transform.GetScale()->x(),
											 m_Transform.GetScale()->y(),
											 m_Transform.GetScale()->z());
		file.WriteString(string);

		// Material
		file.WriteString("  children [\n");
		file.WriteString("    Shape {\n");
		file.WriteString("      appearance Appearance {\n");
		file.WriteString("        material Material {\n");
		file.WriteString("          diffuseColor 0 0 0\n"); // todo
		file.WriteString("        }\n");
		file.WriteString("      }\n");

		// Geometry
		file.WriteString("      geometry DEF Mesh-FACES IndexedFaceSet {\n");
		file.WriteString("        ccw TRUE\n");
		file.WriteString("        solid TRUE\n");

		// Vertices
		file.WriteString("        coord DEF Mesh-COORD Coordinate { point [\n");
		int NbVertex = m_ArrayVertex.GetSize();
		for (int i = 0; i < NbVertex; i++)
		{
			string.Format("          %g %g %g",m_ArrayVertex[i]->x(),
											 m_ArrayVertex[i]->y(),
																		   m_ArrayVertex[i]->z());
			file.WriteString(string);
			if (i != (NbVertex - 1))
				file.WriteString(",\n");
			else
				file.WriteString("]\n");
		}
		file.WriteString("        }\n");

		// Faces
		file.WriteString("        coordIndex [\n");
		int NbFace = m_ArrayFace.GetSize();
		for (int i = 0; i < NbFace; i++)
		{
			string.Format("          %d, %d, %d, -1",m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(0)),
													 m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(1)),
																						   m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(2)));
			file.WriteString(string);
			if (i != (NbFace - 1))
				file.WriteString(",\n");
			else
				file.WriteString("]\n");
		}

		// End
		file.WriteString("        }\n");
		file.WriteString("      }\n");
		file.WriteString("    ]\n");
		file.WriteString("  }\n\n");

	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during writing transform" << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during writing transform");
		return 0;
	}
	END_CATCH

		return 1;
}

//********************************************
// WriteFileRaw (binary raw mode)
//********************************************
int CMesh3d::WriteFileRaw(CFile& file)
{
	CString string;
	TRY
	{
		unsigned int len = m_Name.GetLength();
		file.Write(&len,sizeof(unsigned int));
		file.Write(m_Name.GetBuffer(),len);

		TRACE("writename is %s\n",m_Name);


		// CTransform m_Transform;
		// Translation (xyz)
		float x,y,z;
		x = m_Transform.GetTranslation()->x();
		y = m_Transform.GetTranslation()->y();
		z = m_Transform.GetTranslation()->z();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));
		//TRACE("m_Transform.GetTranslation x %f y%f z%f***\n",x,y,z);
		// Rotation (xyz)
		x = m_Transform.GetRotation()->x();
		y = m_Transform.GetRotation()->y();
		z = m_Transform.GetRotation()->z();
		float v = m_Transform.GetValueRotation();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));
		file.Write(&v,sizeof(float));
		// Scale (xyz)
		x = m_Transform.GetScale()->x();
		y = m_Transform.GetScale()->y();
		z = m_Transform.GetScale()->z();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));

		file.Write(&hasvt,sizeof(BOOL));
		file.Write(&m_SetVertexNormal,sizeof(BOOL));
		file.Write(&m_SetFaceNormal,sizeof(BOOL));
		file.Write(&m_SetFlagOnVertices_ArrayIndex,sizeof(BOOL));

		//GLdouble   mView[16]; //model matrix.
		for (int i = 0; i < 16; i++)
		{
			file.Write(&mView[i],sizeof(GLdouble));
		}
		//int m_Show;int m_showVertex;int m_showEdge;int m_showFace;(ok)
		file.Write(&m_Show,sizeof(int));
		file.Write(&m_showVertex,sizeof(int));
		file.Write(&m_showEdge,sizeof(int));
		file.Write(&m_showFace,sizeof(int));

		//CArray3d<CVertex3d> m_ArrayVertex;CArray3d<CFace3d>   m_ArrayFace;的大小
		unsigned int NbVertex = m_ArrayVertex.GetSize();
		file.Write(&NbVertex,sizeof(unsigned int));
		TRACE("NbVertex = m_ArrayVertex.GetSize() %d\n",NbVertex);
		unsigned int NbFace = m_ArrayFace.GetSize();
		file.Write(&NbFace,sizeof(unsigned int));
		TRACE("NbFace = m_ArrayFace.GetSize(); %d \n",NbFace);
		for (unsigned int i = 0; i < NbVertex; i++)
		{
			m_ArrayVertex[i]->m_Flag = i;
		}

		for (unsigned int i = 0; i < NbFace; i++)
		{
			m_ArrayFace[i]->SetFlag(i);
		}

		//CArray3d<CVertex3d> m_ArrayVertex;
		double tmp1,tmp2;
		//CColor    m_Color;     // Color
		unsigned char r,g,b,a;
		for (unsigned int i = 0; i < NbVertex; i++)
		{
			//float     m_Coord[3];	 // Geometry
			x = m_ArrayVertex[i]->x();
			y = m_ArrayVertex[i]->y();
			z = m_ArrayVertex[i]->z();
			file.Write(&x,sizeof(float));
			file.Write(&y,sizeof(float));
			file.Write(&z,sizeof(float));
			//double    u,v;           //parameter domain.
			tmp1 = m_ArrayVertex[i]->u;
			tmp2 = m_ArrayVertex[i]->v;
			file.Write(&tmp1,sizeof(double));
			file.Write(&tmp2,sizeof(double));
			//double    mux,muy;       //the mu 
			tmp1 = m_ArrayVertex[i]->mux;
			tmp2 = m_ArrayVertex[i]->muy;
			file.Write(&tmp1,sizeof(double));
			file.Write(&tmp2,sizeof(double));
			//CVector3d m_Normal;    // Normal
			x = m_ArrayVertex[i]->GetNormal()->x();
			y = m_ArrayVertex[i]->GetNormal()->y();
			z = m_ArrayVertex[i]->GetNormal()->z();
			file.Write(&x,sizeof(float));
			file.Write(&y,sizeof(float));
			file.Write(&z,sizeof(float));
			//CColor    m_Color;     // Color
			r = m_ArrayVertex[i]->GetColor()->r();
			g = m_ArrayVertex[i]->GetColor()->g();
			b = m_ArrayVertex[i]->GetColor()->b();
			a = m_ArrayVertex[i]->GetColor()->a();
			file.Write(&r,sizeof(unsigned char));
			file.Write(&g,sizeof(unsigned char));
			file.Write(&b,sizeof(unsigned char));
			file.Write(&a,sizeof(unsigned char));
			//int    m_Flag;      // Flag
			int flag = m_ArrayVertex[i]->m_Flag;
			file.Write(&flag,sizeof(int));

		}
		// Faces
		unsigned int v1,v2,v3,f1,f2,f3;
		//unsigned char r,g,b,a;
		for (unsigned int i = 0; i < NbFace; i++)
		{
			//CVector3d m_Normal;    // Normal
			x = m_ArrayFace[i]->GetNormal()->x();
			y = m_ArrayFace[i]->GetNormal()->y();
			z = m_ArrayFace[i]->GetNormal()->z();
			file.Write(&x,sizeof(float));
			file.Write(&y,sizeof(float));
			file.Write(&z,sizeof(float));
			//CColor    m_Color;     // Color
			r = m_ArrayFace[i]->GetColor()->r();
			g = m_ArrayFace[i]->GetColor()->g();
			b = m_ArrayFace[i]->GetColor()->b();
			a = m_ArrayFace[i]->GetColor()->a();
			file.Write(&r,sizeof(unsigned char));
			file.Write(&g,sizeof(unsigned char));
			file.Write(&b,sizeof(unsigned char));
			file.Write(&a,sizeof(unsigned char));

			//unsigned int m_IndexMaterial; // Material
			unsigned int matrial = m_ArrayFace[i]->GetMatrial();
			file.Write(&matrial,sizeof(unsigned int));
			//int    m_Flag;      // Flag
			int a = m_ArrayFace[i]->GetFlag();
			file.Write(&a,sizeof(int));

			//int m_vertexindex[3];       // for store mesh
			int index1 = m_ArrayFace[i]->GetVertexIndex(0);
			int index2 = m_ArrayFace[i]->GetVertexIndex(1);
			int index3 = m_ArrayFace[i]->GetVertexIndex(2);
			file.Write(&index1,sizeof(int));
			file.Write(&index2,sizeof(int));
			file.Write(&index3,sizeof(int));
		}
		//构建拓扑

		//CVertex
		unsigned int nbvertex,nbface;
		int flag;
		for (unsigned int i = 0; i < NbVertex; i++)
		{
			nbvertex = m_ArrayVertex[i]->m_ArrayVertexNeighbor.GetSize();
			file.Write(&nbvertex,sizeof(unsigned int));
			for (unsigned int j = 0; j < nbvertex; j++)
			{
				flag = m_ArrayVertex[i]->m_ArrayVertexNeighbor[j]->m_Flag;
				file.Write(&flag,sizeof(int));
			}
			nbface = m_ArrayVertex[i]->m_ArrayFaceNeighbor.GetSize();
			file.Write(&nbface,sizeof(unsigned int));
			for (unsigned int j = 0; j < nbface; j++)
			{
				flag = m_ArrayVertex[i]->m_ArrayFaceNeighbor[j]->GetFlag();
				file.Write(&flag,sizeof(int));
			}
		}
		//CFace
		for (unsigned int i = 0; i < NbFace; i++)
		{
			v1 = m_ArrayFace[i]->v1()->m_Flag;
			v2 = m_ArrayFace[i]->v2()->m_Flag;
			v3 = m_ArrayFace[i]->v3()->m_Flag;
			file.Write(&v1,sizeof(unsigned int));
			file.Write(&v2,sizeof(unsigned int));
			file.Write(&v3,sizeof(unsigned int));

			//TRACE("di %d ci face de vertex *******v1%d v2%d v3%d \n",i,v1,v2,v3);
			//CFace3d *m_pFace[3];          // Neighbors

			if (m_ArrayFace[i]->f1())
				f1 = m_ArrayFace[i]->f1()->GetFlag();
			else
				f1 = -1;
			if (m_ArrayFace[i]->f2())
				f2 = m_ArrayFace[i]->f2()->GetFlag();
			else
				f2 = -1;
			if (m_ArrayFace[i]->f3())
				f3 = m_ArrayFace[i]->f3()->GetFlag();
			else
				f3 = -1;
			file.Write(&f1,sizeof(int));
			file.Write(&f2,sizeof(int));
			file.Write(&f3,sizeof(int));
			//TRACE("di %d ci face de face *******f1%d f2%d f3%d \n",i,f1,f2,f3);
		}
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during writing " << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during writing");
		return 0;
	}
	END_CATCH

		return 1;
}

int  CMesh3d::ReadFile(CFile& file)
{
	CString string;
	TRY
	{
		unsigned int len;
		CString name;
		char cc[50];
		file.Read(&len,sizeof(unsigned int));
		file.Read(cc,len);
		//file.Read(&name,len);
		TRACE("len is %d\n",len);
		cc[len] = '\0';
		name = cc;
		m_Name = name;

		// CTransform m_Transform;
		// Translation (xyz)
		CVector3d m_Vector;
		float x,y,z,v;
		file.Read(&x,sizeof(float));
		file.Read(&y,sizeof(float));
		file.Read(&z,sizeof(float));
		m_Vector.Set(x,y,z);
		m_Transform.SetTranslation(m_Vector);


		// Rotation (xyz)
		file.Read(&x,sizeof(float));
		file.Read(&y,sizeof(float));
		file.Read(&z,sizeof(float));
		file.Read(&v,sizeof(float));
		//TRACE("************* %d********\n",x);
		m_Vector.Set(x,y,z);
		m_Transform.SetRotation(m_Vector);
		m_Transform.SetValueRotation(v);

		file.Read(&x,sizeof(float));
		file.Read(&y,sizeof(float));
		file.Read(&z,sizeof(float));
		//TRACE("************* %d********\n",x);
		m_Vector.Set(x,y,z);
		m_Transform.SetScale(m_Vector);


		file.Read(&hasvt,sizeof(BOOL));
		file.Read(&m_SetVertexNormal,sizeof(BOOL));
		file.Read(&m_SetFaceNormal,sizeof(BOOL));
		file.Read(&m_SetFlagOnVertices_ArrayIndex,sizeof(BOOL));

		//GLdouble   mView[16]; //model matrix.
		for (int i = 0; i < 16; i++)
		{
			file.Read(&mView[i],sizeof(GLdouble));
		}
		//int m_Show;int m_showVertex;int m_showEdge;int m_showFace;(ok)
		file.Read(&m_Show,sizeof(int));
		file.Read(&m_showVertex,sizeof(int));
		file.Read(&m_showEdge,sizeof(int));
		file.Read(&m_showFace,sizeof(int));

		unsigned int NbVertex,NbFace;
		file.Read(&NbVertex,sizeof(unsigned int));
		TRACE("read Nbverx is *****%d\n",NbVertex);
		file.Read(&NbFace,sizeof(unsigned int));
		TRACE("read Nbface is *****%d\n",NbFace);

		//CArray3d<CVertex3d> m_ArrayVertex;
		double tmp1,tmp2;
		unsigned char r,g,b,a;
		for (unsigned int i = 0; i < NbVertex; i++)
		{
			CVertex3d* vertexobj = new CVertex3d();
			m_ArrayVertex.Add(vertexobj);

			//float     m_Coord[3];	 // Geometry
			file.Read(&x,sizeof(float));
			file.Read(&y,sizeof(float));
			file.Read(&z,sizeof(float));

			vertexobj->SetMCoord(x,y,z);
			//TRACE("SetMCoord(x,y,z);");

			//double    u,v;           //parameter domain.
			file.Read(&tmp1,sizeof(double));
			file.Read(&tmp2,sizeof(double));
			//TRACE("//double    u,v **** %f%f\n",tmp1,tmp2);
			vertexobj->SetParameter(tmp1,tmp2);

			//double    mux,muy;       //the mu 		
			file.Read(&tmp1,sizeof(double));
			file.Read(&tmp2,sizeof(double));
			vertexobj->mux = tmp1;
			vertexobj->muy = tmp2;
			//CVector3d m_Normal;    // Normal
			file.Read(&x,sizeof(float));
			file.Read(&y,sizeof(float));
			file.Read(&z,sizeof(float));
			vertexobj->SetNormal(x,y,z);


			//CColor    m_Color;     // Color
			file.Read(&r,sizeof(unsigned char));
			file.Read(&g,sizeof(unsigned char));
			file.Read(&b,sizeof(unsigned char));
			file.Read(&a,sizeof(unsigned char));
			CColor  colortmp;
			colortmp.Set(r,g,b,a);
			vertexobj->SetColor(colortmp);

			//int    m_Flag;      // Flag
			int flag;
			file.Read(&flag,sizeof(int));
			vertexobj->m_Flag = flag;
		}


		// Faces
		TRACE(" start face !!!****\n");

		//unsigned char r,g,b,a;
		for (unsigned int i = 0; i < NbFace; i++)
		{
			CFace3d* faceobj = new CFace3d();
			//CVector3d m_Normal;    // Normal
			file.Read(&x,sizeof(float));
			file.Read(&y,sizeof(float));
			file.Read(&z,sizeof(float));
			faceobj->SetNormal(x,y,z);
			//TRACE("facetmp->SetNormal(x,y,z);\n");

			//CColor    m_Color;     // Color
			file.Read(&r,sizeof(unsigned char));
			file.Read(&g,sizeof(unsigned char));
			file.Read(&b,sizeof(unsigned char));
			file.Read(&a,sizeof(unsigned char));
			CColor colortmp;
			colortmp.Set(r,g,b,a);
			faceobj->SetColor(colortmp);
			//TRACE("facetmp->SetColor(colortmp);\n");


			//unsigned int m_IndexMaterial; // Material
			unsigned int matrial;
			file.Read(&matrial,sizeof(unsigned int));
			faceobj->SetMatrial(matrial);
			//TRACE("facetmp->SetMatrial(matrial);\n");

			//int    m_Flag;      // Flag
			int a;
			file.Read(&a,sizeof(int));
			faceobj->SetFlag(a);
			//TRACE("facetmp->SetFlag(a);\n");

			//int m_vertexindex[3];       // for store mesh
			int index1,index2,index3;
			file.Read(&index1,sizeof(int));
			file.Read(&index2,sizeof(int));
			file.Read(&index3,sizeof(int));
			faceobj->SetVertexIndex(index1,index2,index3);
			//TRACE("facetmp->SetVertexIndex(index1,index2,index3);\n");

			m_ArrayFace.Add(faceobj);
			//TRACE("m_ArrayFace.Add(facetmp);%d\n",i);	
		}
		//读拓扑结构
		unsigned int nbvertex,nbface;
		int flag;
		for (unsigned int i = 0; i < NbVertex; i++)
		{
			file.Read(&nbvertex,sizeof(unsigned int));
			for (unsigned int j = 0; j < nbvertex; j++)
			{
				file.Read(&flag,sizeof(int));
				m_ArrayVertex[i]->m_ArrayVertexNeighbor.Add(m_ArrayVertex[j]);
			}
			file.Read(&nbface,sizeof(unsigned int));
			for (unsigned int k = 0; k < nbface; k++)
			{
				file.Read(&flag,sizeof(int));
				m_ArrayVertex[i]->m_ArrayFaceNeighbor.Add(m_ArrayFace[k]);
			}
		}
		unsigned int v1,v2,v3;
		int f1,f2,f3;
		for (unsigned int i = 0; i < NbFace; i++)
		{
			file.Read(&v1,sizeof(unsigned int));
			m_ArrayFace[i]->v1(m_ArrayVertex[v1]);
			file.Read(&v2,sizeof(unsigned int));
			m_ArrayFace[i]->v2(m_ArrayVertex[v2]);
			file.Read(&v3,sizeof(unsigned int));
			m_ArrayFace[i]->v3(m_ArrayVertex[v3]);
			//TRACE("read  %d ci face *******v1%d v2%d v3%d \n",i,v1,v2,v3);
			//TRACE("read  %d ci face cunru *******v1%d v2%d v3%d \n",i,
			//	m_ArrayFace[i]->v1()->m_Flag,m_ArrayFace[i]->v2()->m_Flag,m_ArrayFace[i]->v3()->m_Flag);

			file.Read(&f1,sizeof(int));
			if (f1 == -1)
				m_ArrayFace[i]->f1(NULL);
			else m_ArrayFace[i]->f1(m_ArrayFace[f1]);
			file.Read(&f2,sizeof(int));
			if (f2 == -1)
				m_ArrayFace[i]->f2(NULL);
			else m_ArrayFace[i]->f2(m_ArrayFace[f2]);
			file.Read(&f3,sizeof(int));
			if (f3 == -1)
				m_ArrayFace[i]->f3(NULL);
			else m_ArrayFace[i]->f3(m_ArrayFace[f3]);

			//TRACE("read  %d ci face *******f1%d f2%d f3%d \n",i,f1,f2,f3);

			//TRACE("read  %d ci face cunru *******v1%d v2%d v3%d \n",i,

			//m_ArrayFace[f1]->GetFlag(),m_ArrayFace[f2]->GetFlag(),m_ArrayFace[f3]->GetFlag());
		}
		//(m_ArrayFace[i]->f1())->GetFlag(),(m_ArrayFace[i]->f2())->GetFlag(),(m_ArrayFace[i]->f3())->GetFlag());
		//m_ArrayFace[f1]->GetFlag(),m_ArrayFace[f2]->GetFlag(),m_ArrayFace[f3]->GetFlag()


	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during reading " << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during reading");
		return 0;
	}
	END_CATCH

		return 1;

}

//********************************************
// GetMinArea
//********************************************
double CMesh3d::GetMinArea(CFace3d** ppFace /* NULL */)
{
	double min = MAX_DOUBLE;
	int size = m_ArrayFace.GetSize();
	for (int i = 0; i < size; i++)
	{
		double area = m_ArrayFace[i]->Area();
		if (area < min)
		{
			min = area;
			if (ppFace != NULL)
				*ppFace = m_ArrayFace[i];
		}
	}
	return min;
}

//********************************************
// GetMeanArea
//********************************************
double CMesh3d::GetMeanArea()
{
	return ::GetMeanArea(&m_ArrayFace);
}




// ** EOF **



void CMesh3d::ConstructTetrahedra()
{

}
void CMesh3d::ComputeBoundingBox(Point3D& lower, Point3D& upper)
{
	CVector3d m_lower, m_upper;
	m_lower.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	m_upper.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	double m_x, m_y, m_z;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
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
	lower.setValue(m_lower.x(), m_lower.y(), m_lower.z());
	upper.setValue(m_upper.x(), m_upper.y(), m_upper.z());
}
void CMesh3d::GetFourCornerPoints(int& index00, int& indexm0, int& index0n, int& indexmn)
{
	Point3D p00, pm0, p0n, pmn;
	CVector3d m_lower, m_upper;
	m_lower.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	m_upper.Set(m_ArrayVertex[0]->x(), m_ArrayVertex[0]->y(), m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	double m_x, m_y, m_z;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
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
	p00.setValue(m_lower.x(), m_lower.y(), m_lower.z());
	pmn.setValue(m_upper.x(), m_upper.y(), m_upper.z());
	pm0.setValue(m_upper.x(), m_lower.y(), m_lower.z());
	p0n.setValue(m_lower.x(), m_upper.y(), m_lower.z());

	index00 = 0;
	indexm0 = 0;
	index0n = 0;
	indexmn = 0;

	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		Point3D pp;
		pp[0] = m_ArrayVertex[i]->x();
		pp[1] = m_ArrayVertex[i]->y();
		pp[2] = m_ArrayVertex[i]->z();

		Point3D pi00, pim0, pi0n, pimn;

		pi00[0] = m_ArrayVertex[index00]->x();
		pi00[1] = m_ArrayVertex[index00]->y();
		pi00[2] = m_ArrayVertex[index00]->z();

		pim0[0] = m_ArrayVertex[indexm0]->x();
		pim0[1] = m_ArrayVertex[indexm0]->y();
		pim0[2] = m_ArrayVertex[indexm0]->z();

		pi0n[0] = m_ArrayVertex[index0n]->x();
		pi0n[1] = m_ArrayVertex[index0n]->y();
		pi0n[2] = m_ArrayVertex[index0n]->z();

		pimn[0] = m_ArrayVertex[indexmn]->x();
		pimn[1] = m_ArrayVertex[indexmn]->y();
		pimn[2] = m_ArrayVertex[indexmn]->z();

		if (Distance(p00, pp) < Distance(p00, pi00))
		{
			index00 = i;
		}
		if (Distance(pm0, pp) < Distance(pm0, pim0))
		{
			indexm0 = i;
		}
		if (Distance(p0n, pp) < Distance(p0n, pi0n))
		{
			index0n = i;
		}
		if (Distance(pmn, pp) < Distance(pmn, pimn))
		{
			indexmn = i;
		}
	}
}

void CMesh3d::Normalize()
{
	Point3D lower, upper;
	ComputeBoundingBox(lower, upper);
	double maxlength = upper.x - lower.x;
	if (maxlength < upper.y - lower.y)
		maxlength = upper.y - lower.y;
	if (maxlength < upper.z - lower.z)
		maxlength = upper.z - lower.z;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		CVertex3d* pVertex = m_ArrayVertex[i];
		double x = pVertex->x();
		double y = pVertex->y();
		double z = pVertex->z();

		x = (x - lower[0]) / maxlength;
		y = (y - lower[1]) / maxlength;
		z = (z - lower[2]) / maxlength;

		pVertex->Set(x, y, z);
	}
}
int CMesh3d::IsNormalPerdicularToDirection(CFace3d* pFace, Vector3D axis, double TOL)
{
	if (!pFace)
		return 0;

	CVector3d n1 = pFace->GetNormal();

	Vector3D nv1(n1.x(), n1.y(), n1.z());

	nv1.SetUnit();

	double val1 = nv1 * axis;
	double angle = acos(val1) * 180 / PI;

	int flag1 = IsNormalPerdicularToDirection(pFace->v1(), axis, TOL);
	int flag2 = IsNormalPerdicularToDirection(pFace->v2(), axis, TOL);
	int flag3 = IsNormalPerdicularToDirection(pFace->v3(), axis, TOL);

	return (fabs(angle - 90) < TOL || flag1 && flag2 && flag3);
}
int CMesh3d::IsNormalPerdicularToDirection(CVertex3d* pVertex, Vector3D axis, double TOL)
{
	if (!pVertex)
		return 0;

	CVector3d n1 = pVertex->GetNormal();

	Vector3D nv1(n1.x(), n1.y(), n1.z());

	nv1.SetUnit();

	double val1 = nv1 * axis;
	double angle = acos(val1) * 180 / PI;
	return (fabs(angle - 90) < TOL);
}
int CMesh3d::IsNormalSameDirection(Vector3D v1, Vector3D v2, double TOL)
{
	double val1 = v1 * v2;
	double angle = acos(val1) * 180 / PI;
	return (angle < TOL);
}
int CMesh3d::IsNormalCoDirection(CFace3d* pFace1, CFace3d* pFace2, double TOL)
{
	if (!pFace1 || !pFace2)
		return 0;

	CVector3d n1 = pFace1->GetNormal();
	CVector3d n2 = pFace2->GetNormal();

	Vector3D nv1(n1.x(), n1.y(), n1.z());
	Vector3D nv2(n2.x(), n2.y(), n2.z());

	nv1.SetUnit();
	nv2.SetUnit();

	double val1 = nv1 * nv2;
	double angle = acos(val1) * 180 / PI;
	return (angle < TOL);
}
void CMesh3d::DetectTriangleType(double angletolerance, int  groupnumber)
{
	double TOL = angletolerance;
	double cylinderangletol = 40;
	Vector3D zaxis(0, 0, 1);
	CColor  planecolor;
	planecolor.Set(0, 255, 0);

	CColor cylindercolor;
	cylindercolor.Set(0, 0, 255);
	CColor  color1;
	color1.Set(128, 128, 128);
	BuildAdjacency();
	CalculateNormalPerVertex();

	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		m_ArrayFace[i]->SetFlag(0);
		m_ArrayFace[i]->SetColor(color1);
		m_ArrayFace[i]->m_originalIndex = i;
	}

	std::vector<std::vector<CFace3d*>> m_ArrayFaceGroup;

	std::vector<CFace3d*> currentgroup;
	std::vector<CFace3d*> currentgroupfront;

	CFace3d* pFace, * pCurrentFrontFace;
	//遍历所有三角形
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		pFace = m_ArrayFace[i];

		if (pFace->GetFlag() == 1)
			continue;

		//设置标记 1为已经检测
		pFace->SetFlag(1);
		//pFace->SetColor(color);

		currentgroup.clear();
		currentgroupfront.clear();

		//获取相邻的三个三角形
		CFace3d* pAdjFace1 = pFace->f1();
		CFace3d* pAdjFace2 = pFace->f2();
		CFace3d* pAdjFace3 = pFace->f3();

		//判断和其中某一个三角形同向，则加入数组
		int isCoNormalFlag1 = IsNormalCoDirection(pFace, pAdjFace1, TOL);
		int isCoNormalFlag2 = IsNormalCoDirection(pFace, pAdjFace2, TOL);
		int isCoNormalFlag3 = IsNormalCoDirection(pFace, pAdjFace3, TOL);


		//如果共面了
		currentgroup.push_back(pFace);

		if (isCoNormalFlag1)
		{
			//currentgroup.push_back(pAdjFace1);
			currentgroupfront.push_back(pAdjFace1);
		}


		if (isCoNormalFlag2)
		{
			//currentgroup.push_back(pAdjFace2);
			currentgroupfront.push_back(pAdjFace2);
		}


		if (isCoNormalFlag3)
		{
			//currentgroup.push_back(pAdjFace3);
			currentgroupfront.push_back(pAdjFace3);
		}

		while (currentgroupfront.size() > 0)
		{
			//从当前的front里面取出一个面
			pCurrentFrontFace = currentgroupfront[0];
			currentgroupfront.erase(currentgroupfront.begin());

			//检测当前的三角形，更新group和front
			DectectFace_Plane(pCurrentFrontFace, currentgroup, currentgroupfront, TOL);

			////if (currentgroup.size() > 10)
			//	break;
		}

		if (currentgroup.size() < groupnumber)
		{
			//for (int j = 0; j < currentgroup.size(); j++)
			//	currentgroup[j]->SetFlag(0);

			continue;
		}


		for (int j = 0; j < currentgroup.size(); j++)
			currentgroup[j]->SetColor(planecolor);


		//检测出来的曲面分类
		m_ArrayFaceGroup.push_back(currentgroup);
		//break;
	}
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		m_ArrayFace[i]->SetFlag(0);
	}
	for (int i = 0; i < m_ArrayFaceGroup.size(); i++)
	{
		for (int j = 0; j < m_ArrayFaceGroup[i].size(); j++)
			m_ArrayFaceGroup[i][j]->SetFlag(1);
	}


	currentgroup.clear();

	////遍历所有三角形检测圆柱
	//for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	//{
	//	currentgroup.clear();
	//	currentgroupfront.clear();

	//	pFace = m_ArrayFace[i];

	//	if (pFace->GetFlag() == 1)
	//		continue;

	//	//设置标记 1为已经检测
	//	pFace->SetFlag(1);
	//	/*pFace->SetColor(cylindercolor);*/

	//	currentgroup.push_back(pFace);

	//	CFace3d* pAdjFace1 = pFace->f1();
	//	CFace3d* pAdjFace2 = pFace->f2();
	//	CFace3d* pAdjFace3 = pFace->f3();

	//	if (pAdjFace1 == NULL || pAdjFace2 == NULL || pAdjFace3 == NULL)
	//	{
	//		continue;
	//	}

	//	// 邻面的法向量
	//	CVector3d* v = pFace->GetNormal();
	//	CVector3d* v1 = pAdjFace1->GetNormal();
	//	CVector3d* v2 = pAdjFace2->GetNormal();
	//	CVector3d* v3 = pAdjFace3->GetNormal();

	//	// 法向量单位化
	//	v->NormalizeL2();
	//	v1->NormalizeL2();
	//	v2->NormalizeL2();
	//	v3->NormalizeL2();

	//	CVector3d normal1_ = *v ^ *v1;
	//	CVector3d normal2_ = *v ^ *v2;
	//	CVector3d normal3_ = *v ^ *v3;

	//	normal1_.NormalizeL2();
	//	normal2_.NormalizeL2();
	//	normal3_.NormalizeL2();

	//	// 计算叉积
	//	//Vector3D normal1((v->m_y * v1->m_z - v1->m_y * v->m_z), -(v->m_x * v1->m_z - v1->m_x * v->m_z), (v->m_x * v1->m_y - v1->m_x * v->m_y));
	//	//Vector3D normal2((v->m_y * v2->m_z - v2->m_y * v->m_z), -(v->m_x * v2->m_z - v2->m_x * v->m_z), (v->m_x * v2->m_y - v2->m_x * v->m_y));
	//	//Vector3D normal3((v->m_y * v3->m_z - v3->m_y * v->m_z), -(v->m_x * v3->m_z - v3->m_x * v->m_z), (v->m_x * v3->m_y - v3->m_x * v->m_y));

	//	// 叉积单位化
	//	//normal1.SetUnit();
	//	//normal2.SetUnit();
	//	//normal3.SetUnit();

	//	Vector3D normal1(normal1_.x(), normal1_.y(), normal1_.z());
	//	Vector3D normal2(normal2_.x(), normal2_.y(), normal2_.z());
	//	Vector3D normal3(normal3_.x(), normal3_.y(), normal3_.z());

	//	Vector3D compareNormal(0, 0, 0);

	//	int flag1 = IsNormalSameDirection(normal1, normal2, cylinderangletol);
	//	int flag2 = IsNormalSameDirection(normal1, normal3, cylinderangletol);
	//	if (flag1 && flag2)
	//	{
	//		/*pFace->SetColor(cylindercolor);*/
	//		compareNormal = (normal1 + normal2 + normal3) / 3;
	//		compareNormal.SetUnit();
	//		currentgroupfront.push_back(pAdjFace1);
	//		currentgroupfront.push_back(pAdjFace2);
	//		currentgroupfront.push_back(pAdjFace3);
	//		/*pAdjFace1->SetColor(cylindercolor);
	//		pAdjFace2->SetColor(cylindercolor);
	//		pAdjFace3->SetColor(cylindercolor);*/
	//	}
	//	else
	//	{
	//		continue;
	//	}


	//	/*for (auto iter = currentgroupfront.begin(); iter != currentgroupfront.end(); iter++)*/
	//	while (currentgroupfront.size() > 0)
	//	{
	//		CFace3d* dFace = currentgroupfront[0];
	//		currentgroup.push_back(dFace);
	//		currentgroupfront.erase(currentgroupfront.begin());


	//		if (dFace->GetFlag() == 1)
	//			continue;

	//		dFace->SetFlag(1);

	//		// 获取邻面
	//		CFace3d* dAdjFace1 = dFace->f1();
	//		CFace3d* dAdjFace2 = dFace->f2();
	//		CFace3d* dAdjFace3 = dFace->f3();
	//		int flag1 = 0;
	//		int flag2 = 0;
	//		int flag3 = 0;

	//		// 获取法相
	//		v = dFace->GetNormal();

	//		// 邻面不为空再进行操作：1.法向单位化 2.和平均法相进行比较，判断是否平行
	//		if (dAdjFace1 != NULL)
	//		{
	//			v1 = dAdjFace1->GetNormal();

	//			CVector3d norm1_ = *v ^ *v1;

	//			norm1_.NormalizeL2();

	//			Vector3D normal1(norm1_.x(), norm1_.y(), norm1_.z());
	//			normal1.SetUnit();
	//			flag1 = IsNormalSameDirection(normal1, compareNormal, cylinderangletol);
	//		}

	//		if (dAdjFace2 != NULL)
	//		{
	//			v2 = dAdjFace2->GetNormal();
	//			CVector3d normal2_ = *v ^ *v2;

	//			normal2_.NormalizeL2();

	//			Vector3D normal2(normal2_.x(), normal2_.y(), normal2_.z());
	//			normal2.SetUnit();
	//			flag2 = IsNormalSameDirection(normal2, compareNormal, cylinderangletol);
	//		}

	//		if (dAdjFace3 != NULL)
	//		{
	//			v3 = dAdjFace3->GetNormal();
	//			CVector3d normal3_ = *v ^ *v3;

	//			normal3_.NormalizeL2();

	//			Vector3D normal3(normal3_.x(), normal3_.y(), normal3_.z());
	//			normal3.SetUnit();
	//			flag3 = IsNormalSameDirection(normal3, compareNormal, cylinderangletol);
	//		}

	//		if (flag1)
	//		{
	//			currentgroupfront.push_back(dAdjFace1);
	//			/*dAdjFace1->SetColor(cylindercolor);*/
	//		}

	//		if (flag2)
	//		{
	//			currentgroupfront.push_back(dAdjFace2);
	//			/*dAdjFace2->SetColor(cylindercolor);*/
	//		}

	//		if (flag3)
	//		{
	//			currentgroupfront.push_back(dAdjFace3);
	//			/*dAdjFace3->SetColor(cylindercolor);*/
	//		}
	//	}

	//	/*if (currentgroup.size() < groupnumber)
	//	{
	//		for (int j = 0; j < currentgroup.size(); j++)
	//			currentgroup[j]->SetFlag(0);
	//		continue;
	//	}*/


	//	for (int j = 0; j < currentgroup.size(); j++)
	//		currentgroup[j]->SetColor(cylindercolor);



	//	m_ArrayFaceGroup.push_back(currentgroup);
	//}
	CArray3d<CFace3d> pArrayFace;
	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		currentgroup.clear();
		currentgroupfront.clear();

		pFace = m_ArrayFace[i];

		if (pFace->GetFlag() == 1)
			continue;

		//设置标记 1为已经检测
		pFace->SetFlag(1);

		int flag = IsNormalPerdicularToDirection(pFace, zaxis, cylinderangletol);

		if (!flag)
			continue;

		//如果共面了
		if (flag)
			currentgroup.push_back(pFace);

		//adjacentFaces.clear();
		pFace->BuildArrayNeighbor(&pArrayFace, 0);

		for (int j = 0; j < pArrayFace.GetSize(); j++)
		{
			int flag1 = IsNormalPerdicularToDirection(pArrayFace[j], zaxis, cylinderangletol);

			if (flag1)
				currentgroupfront.push_back(pArrayFace[j]);
		}
		//int flag1 = IsNormalPerdicularToDirection(pFace->f1(), zaxis, cylinderangletol);
		//int flag2 = IsNormalPerdicularToDirection(pFace->f2(), zaxis, cylinderangletol);
		//int flag3 = IsNormalPerdicularToDirection(pFace->f3(), zaxis, cylinderangletol);


		//if (flag1)
		//{
			//currentgroup.push_back(pAdjFace1);
		//	currentgroupfront.push_back(pFace->f1());
		//}


		//if (flag2)
		//{
			//currentgroup.push_back(pAdjFace2);
		//	currentgroupfront.push_back(pFace->f2());
		//}


		//if (flag3)
		//{
			//currentgroup.push_back(pAdjFace3);
		//	currentgroupfront.push_back(pFace->f3());
		//}

		while (currentgroupfront.size() > 0)
		{
			//从当前的front里面取出一个面
			pCurrentFrontFace = currentgroupfront[0];
			currentgroupfront.erase(currentgroupfront.begin());

			//检测当前的三角形，更新group和front
			DectectFace_Cylinder(pCurrentFrontFace, currentgroup, currentgroupfront, zaxis, TOL);

			////if (currentgroup.size() > 10)
			//	break;
		}

		if (currentgroup.size() < 0)
		{
			//for (int j = 0; j < currentgroup.size(); j++)
			//	currentgroup[j]->SetFlag(0);

			continue;
		}


		for (int j = 0; j < currentgroup.size(); j++)
			currentgroup[j]->SetColor(cylindercolor);


		//检测出来的曲面分类
		m_ArrayFaceGroup.push_back(currentgroup);

		//if (flag || flag1 && flag2 && flag3)
		//{
		////	currentgroup.push_back(pFace);
		//}
	}

	//for (int j = 0; j < currentgroup.size(); j++)
	//	currentgroup[j]->SetColor(cylindercolor);

	//m_ArrayFaceGroup.push_back(currentgroup);
	//for (int i = 0; i < m_ArrayFaceGroup)
		//--------------


	/*
		SetNormalBinding(NORMAL_PER_FACE);

		for (int i = 0; i < m_ArrayFace.GetSize(); i++)
		{
			m_ArrayFace[i]->SetFlag(0);
		}
		for (int i = 0; i < m_ArrayFace.GetSize(); i++)
		{
			CFace3d* pFace1 = GetFace(i)->f1();
			CFace3d* pFace2 = GetFace(i)->f2();
			CFace3d* pFace3 = GetFace(i)->f3();

			CVector3d n = GetFace(i)->GetNormal();
			CVector3d n1 = pFace1->GetNormal();
			CVector3d n2 = pFace2->GetNormal();
			CVector3d n3 = pFace3->GetNormal();
			Vector3D nv(n.x(), n.y(), n.z());
			Vector3D nv1(n1.x(), n1.y(), n1.z());
			Vector3D nv2(n2.x(), n2.y(), n2.z());
			Vector3D nv3(n3.x(), n3.y(), n3.z());
			nv.SetUnit();
			nv1.SetUnit();
			nv2.SetUnit();
			nv3.SetUnit();
			double val1 = nv * nv1;
			double val2 = nv * nv2;
			double val3 = nv * nv3;
			CColor color;
			if (fabs(val1 - 1) < TOL || fabs(val2 - 1) < TOL || fabs(val3 - 1) < TOL)
			{

				//plane
				color.Set(0, 255, 0);
				m_ArrayFace[i]->SetFlag(1);
				m_ArrayFace[i]->SetColor(color);
			}
			else
			{
				color.Set(255, 0, 0);
				m_ArrayFace[i]->SetFlag(0);
				m_ArrayFace[i]->SetColor(color);
			}
		}*/
	SetModified(TRUE);
}
CMesh3d* CMesh3d::SubtructSmallerMesh(Point3D lower, Point3D upper)
{
	double m_x, m_y, m_z;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		m_ArrayVertex[i]->SetFlag(0);
	}
	int index = 0;
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		m_x = m_ArrayVertex[i]->x();
		m_y = m_ArrayVertex[i]->y();
		m_z = m_ArrayVertex[i]->z();

		if (m_x < lower.x || m_x > upper.x || m_y < lower.y || m_y > upper.y || m_z < lower.z || m_z > upper.z)
		{
			m_ArrayVertex[i]->SetFlag(1);
		}
		else
		{
			m_ArrayVertex[i]->m_OriginalIndex = index++;
		}
	}

	CMesh3d* pNewMesh = new CMesh3d();
	for (int i = 0; i < m_ArrayVertex.GetSize(); i++)
	{
		if (m_ArrayVertex[i]->GetFlag() == 0)
		{
			pNewMesh->AddVertex(new CVertex3d(m_ArrayVertex[i]));
		}
	}

	for (int i = 0; i < m_ArrayFace.GetSize(); i++)
	{
		if (GetFace(i)->v1()->GetFlag() == 0 && GetFace(i)->v2()->GetFlag() == 0 && GetFace(i)->v3()->GetFlag() == 0)
		{
			pNewMesh->AddFace(new CFace3d(pNewMesh->m_ArrayVertex[GetFace(i)->v1()->m_OriginalIndex], pNewMesh->m_ArrayVertex[GetFace(i)->v2()->m_OriginalIndex], pNewMesh->m_ArrayVertex[GetFace(i)->v3()->m_OriginalIndex]));
		}
	}
	pNewMesh->SetTransform(*GetTransform());
	return pNewMesh;
}

void CMesh3d::DectectFace_Cylinder(CFace3d* pCurrentFrontFace, std::vector<CFace3d*>& currentgroup, std::vector<CFace3d*>& currentgroupfront, Vector3D axis, double TOL)
{
	if (pCurrentFrontFace->GetFlag() == 1)
		return;

	//设置标记 1为已经检测
	pCurrentFrontFace->SetFlag(1);

	//如果共面了
	currentgroup.push_back(pCurrentFrontFace);

	//adjacentFaces.clear();
	CArray3d<CFace3d> pArrayFace;
	pCurrentFrontFace->BuildArrayNeighbor(&pArrayFace, 0);

	for (int j = 0; j < pArrayFace.GetSize(); j++)
	{
		int flag1 = IsNormalPerdicularToDirection(pArrayFace[j], axis, TOL);

		if (flag1)
			currentgroupfront.push_back(pArrayFace[j]);
	}

	////获取相邻的三个三角形
	//CFace3d* pAdjFace1 = pCurrentFrontFace->f1();
	//CFace3d* pAdjFace2 = pCurrentFrontFace->f2();
	//CFace3d* pAdjFace3 = pCurrentFrontFace->f3();

	////判断和其中某一个三角形同向，则加入数组
	//if (pAdjFace1 && pAdjFace1->GetFlag() == 0)
	//{
	//	int Flag1 = IsNormalPerdicularToDirection(pAdjFace1,axis, TOL);
	//	if (Flag1)
	//	{
	//		//currentgroup.push_back(pAdjFace1);
	//		currentgroupfront.push_back(pAdjFace1);
	//	}
	//}

	//if (pAdjFace2 && pAdjFace2->GetFlag() == 0)
	//{
	//	int Flag2 = IsNormalPerdicularToDirection(pAdjFace2, axis, TOL);
	//	if (Flag2)
	//	{
	//		//currentgroup.push_back(pAdjFace2);
	//		currentgroupfront.push_back(pAdjFace2);
	//	}
	//}

	//if (pAdjFace3 && pAdjFace3->GetFlag() == 0)
	//{
	//	int Flag3 = IsNormalPerdicularToDirection(pAdjFace3, axis, TOL);
	//	if (Flag3)
	//	{
	//		//currentgroup.push_back(pAdjFace3);
	//		currentgroupfront.push_back(pAdjFace3);
	//	}
	//}


}