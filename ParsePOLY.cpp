#include "stdafx.h"
#include "lib3d\Base3d.h"
#include "ParsePOLY.h"
#include <iostream>   // std::cout
#include <sstream>
#include <string>
#include <fstream>
#include "nurbs3D.h"
CParsePOLY::CParsePOLY(void)
{
}

CParsePOLY::~CParsePOLY(void)
{
}

void CParsePOLY::Free(void)
{

}
extern char Post_Message[1000];
int CParsePOLY::Run_Element(char *filename,CSceneGraph3d *pSceneGraph)
{
	CString NodeFileName = filename;
	NodeFileName = NodeFileName.Left(NodeFileName.ReverseFind('.'));
	NodeFileName = NodeFileName + ".node";

	char s[100];
	long m_VertexNumber, m_FaceNumber; //vertex and face number
	float m_x, m_y, m_z;  //coordinates of vertex
	long m_index1, m_index2, m_index3;  //index of vertices of face
	int vindex,findex;
	std::ifstream nodefile(NodeFileName);
	std::ifstream elefile(filename);

	int vnum,dimension,attributsnum,hasvertexboundarymarker;
	nodefile >> vnum >> dimension >> attributsnum >> hasvertexboundarymarker;

	CVertex3d* pVertex;
	int index;
	double x,y;
	//only works for 2D PSLG
	if(dimension != 2)
		return 0;

	CMesh3d* pMesh = new CMesh3d();
	double attribute;
	for(int i = 0; i < vnum; i++)
	{
		nodefile >> index >> x >> y;
		pVertex = new CVertex3d(x,y,0);
		pVertex->m_OriginalIndex = index;
		for(int j = 0; j < attributsnum; j++)
		{
			nodefile >> attribute;
			pVertex->m_Attibuts.push_back(attribute);
		}
		if(hasvertexboundarymarker)
			nodefile >> pVertex->isBoundary;

		pMesh->m_ArrayVertex.Add(pVertex);
	}
	nodefile.close();

	//find the biggest Original Index;
	int maximalindex = -1;
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		if(pVertex->m_OriginalIndex > maximalindex)
			maximalindex = pVertex->m_OriginalIndex;
	}
	std::vector<int> m_IndexArray;
	m_IndexArray.resize(maximalindex + 1,0);
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		m_IndexArray[pVertex->m_OriginalIndex] = i;
	}

	int numtriangles,numofverticespertriangle,triangleattributes;
	elefile >> numtriangles >> numofverticespertriangle >> triangleattributes;
	if(numofverticespertriangle != 3)
		return 0;
	int nindex1,nindex2,nindex3;
	CFace3d* pFace3d;
	for(int i = 0; i < numtriangles; i++)
	{
		CFace3d* pFace3d = new CFace3d();
		elefile >> index >> nindex1 >> nindex2 >> nindex3;

		pFace3d->Set(pMesh->m_ArrayVertex[m_IndexArray[nindex1]],pMesh->m_ArrayVertex[m_IndexArray[nindex2]],pMesh->m_ArrayVertex[m_IndexArray[nindex3]]);
		for(int j = 0; j < triangleattributes; j++)
		{
			double val;
			elefile >> val;
			pFace3d->m_Attibuts.push_back(val);
		}
		pMesh->m_ArrayFace.Add(pFace3d);
	}
	pMesh->m_ColorBinding = COLOR_PER_VERTEX;
	pMesh->filename = filename;

	if(pMesh->m_ArrayVertex.GetSize() == 0)
		return 0;

	//bounding box
	CVector3d m_lower,m_upper;
	m_lower.Set(pMesh->m_ArrayVertex[0]->x(),pMesh->m_ArrayVertex[0]->y(),pMesh->m_ArrayVertex[0]->z());
	m_upper.Set(pMesh->m_ArrayVertex[0]->x(),pMesh->m_ArrayVertex[0]->y(),pMesh->m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	for(int i=0; i<pMesh->m_ArrayVertex.GetSize(); i++)
	{
		m_x = pMesh->m_ArrayVertex[i]->x();
		m_y = pMesh->m_ArrayVertex[i]->y();
		m_z = pMesh->m_ArrayVertex[i]->z();

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
	//pMesh->SetColor(color);

	pMesh->SetColorBinding(COLOR_PER_VERTEX);
	//TransformMeshVertices(pMesh);
	pSceneGraph->Add(pMesh);
	elefile.close();

	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	return 1;
}

int CParsePOLY::Run_PSLG(char *filename,CSceneGraph3d *pSceneGraph)
{
	char s[100];
	long m_VertexNumber, m_FaceNumber; //vertex and face number
	float m_x, m_y, m_z;  //coordinates of vertex
	long m_index1, m_index2, m_index3;  //index of vertices of face
	int vindex,findex;
	std::ifstream file(filename);

	int vnum,dimension,attributsnum,hasvertexboundarymarker;
	file >> vnum >> dimension >> attributsnum >> hasvertexboundarymarker;

	CVertex3d* pVertex;
	int index;
	double x,y;
	//only works for 2D PSLG
	if(dimension != 2)
		return 1;

	CMesh3d* pMesh = new CMesh3d();
	double attribute;
	for(int i = 0; i < vnum; i++)
	{
		file >> index >> x >> y;
		pVertex = new CVertex3d(x,y,0);
		pVertex->m_OriginalIndex = index;
		for(int j = 0; j < attributsnum; j++)
		{
			file >> attribute;
			pVertex->m_Attibuts.push_back(attribute);
		}
		if(hasvertexboundarymarker)
			file >> pVertex->isBoundary;

		pMesh->m_ArrayVertex.Add(pVertex);
	}

	//find the biggest Original Index;
	int maximalindex = -1;
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		if(pVertex->m_OriginalIndex > maximalindex)
			maximalindex = pVertex->m_OriginalIndex;
	}
	std::vector<int> m_IndexArray;
	m_IndexArray.resize(maximalindex + 1,0);
	for(int i = 0; i < pMesh->m_ArrayVertex.GetSize(); i++)
	{
		pVertex = pMesh->GetVertex(i);
		m_IndexArray[pVertex->m_OriginalIndex] = i;
	}

	int edgenum,hasedgeboundarymarker;
	file >> edgenum >>  hasedgeboundarymarker;
	int start,end;

	std::pair<int,int> edge;
	int flag;
	for(int i = 0; i < edgenum; i++)
	{
		file >> index >> start >> end;

		edge.first = m_IndexArray[start];
		edge.second = m_IndexArray[end];
		if(hasedgeboundarymarker)
		{
			file >> flag;
			pMesh->m_ArraySharpEdges_Isboundary.push_back(flag);
		}
		pMesh->m_ArraySharpEdges.push_back(edge);
	}

	pMesh->m_ColorBinding = COLOR_PER_VERTEX;
	pMesh->filename = filename;

	if(pMesh->m_ArrayVertex.GetSize() == 0)
		return 1;


	//bounding box
	CVector3d m_lower,m_upper;
	m_lower.Set(pMesh->m_ArrayVertex[0]->x(),pMesh->m_ArrayVertex[0]->y(),pMesh->m_ArrayVertex[0]->z());
	m_upper.Set(pMesh->m_ArrayVertex[0]->x(),pMesh->m_ArrayVertex[0]->y(),pMesh->m_ArrayVertex[0]->z());
	CTransform transform;
	transform.Clear();
	for(int i=0; i<pMesh->m_ArrayVertex.GetSize(); i++)
	{
		m_x = pMesh->m_ArrayVertex[i]->x();
		m_y = pMesh->m_ArrayVertex[i]->y();
		m_z = pMesh->m_ArrayVertex[i]->z();

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
	//pMesh->SetColor(color);

	pMesh->SetColorBinding(COLOR_PER_VERTEX);
	//TransformMeshVertices(pMesh);
	pSceneGraph->Add(pMesh);
	file.close();

	sprintf(Post_Message,"Face num: %10d; Vertex num: %10d; Selected Edge num: %10d; Landmark num: %10d",pMesh->NbFace(),pMesh->NbVertex(),pMesh->m_ArraySharpEdges.size(),pMesh->m_LandMarkArray.size());
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
}
