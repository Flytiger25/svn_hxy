﻿//********************************************
// Vertex3d.h
// (Part of 3d Toolbox)
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 10/12/97
// Modified : 29/04/98
//********************************************

#ifndef _VERTEX_3D_
#define _VERTEX_3D_

#include "Object3d.h"
#include "Vector3d.h"
#include "Color.h"
#include "Array3d.h"

class CMesh3d;
class CFace3d;
class CEdge3d;

class CVertex3d : public CObject3d
{

public:

	double     m_Coord[3];	 // Geometry
	double    u, v;           //parameter domain.
	double    mux, muy;       //the mu 
	int region;
	CVector3d m_Normal;    // Normal
	CColor    m_Color;     // Color
	int		  m_Flag;      // Flag
	int       m_OriginalIndex; //Original Vertex Index
	int       m_Parent;        //store the parent information
	void* m_pBag;		   // Any associated structure
	double   minimallength;    //shortest path
	std::vector<int> m_ArrayPointOnSurface;
	int		 m_interior_vertices_index;
	int      m_Landmarknumber;
	int      m_num;

	//For PSLG
	std::vector<double> m_Attibuts; //attributs
	int		  isBoundary;           //Is boundary

	// Neighbors (faces and vertices)
	CArray3d<CVertex3d> m_ArrayVertexNeighbor;
	CArray3d<CFace3d>   m_ArrayFaceNeighbor;

	Vector3D m_LaplaceOperatorVec;
	double m_GaussCurvature;
	double tempval;
public:

	// Constructors
	CVertex3d() { m_Flag = 0; Set(0.0f, 0.0f, 0.0f); u = v = 0; }
	CVertex3d(CVertex3d& vertex);
	CVertex3d(CVertex3d* pVertex);
	CVertex3d(CVertex3d* pVertex, CVector3d* pVector);
	CVertex3d(const double x, const double y, const double z);

	// Destructor (CArray3d will do the work...)
	virtual ~CVertex3d() {}

	// Data setting
	void Set(const double x, const double y, const double z);
	void Set(CVertex3d* pVertex);
	void Set(CVertex3d& vertex);

	// Moving
	void Move(CVector3d& vector, const double ratio);
	void Move(CVector3d* pVector, const double ratio = 1.0f);
	void Move(const double dx, const double dy, const double dz);

	// Per coordinate
	void Set(const unsigned int index, const double value) { m_Coord[index] = value; }
	void SetParameter(double u_, double v_) { u = u_; v = v_; }
	void x(const double x) { m_Coord[0] = x; }
	void y(const double y) { m_Coord[1] = y; }
	void z(const double z) { m_Coord[2] = z; }

	// Data access 
	double Get(const unsigned int index) { return m_Coord[index]; }
	void GetParameter(double& u_, double& v_) { u_ = u; v_ = v; }
	double x(void) { return m_Coord[0]; }
	double y(void) { return m_Coord[1]; }
	double z(void) { return m_Coord[2]; }

	// Type
	virtual int GetType();

	// Color
	CColor* GetColor(void) { return &m_Color; }
	void SetColor(CColor& color) { m_Color.Set(color); }
	void SetColor(int r, int g, int b) { m_Color.Set(r, g, b); }

	// Flag
	int GetFlag(void) { return m_Flag; }
	void SetFlag(const int flag) { m_Flag = flag; }
	int HasNeighborWithFlag(int flag);
	int HasNeighborWithFlagButDiff(int flag, CVertex3d* pVertexDiff);
	CVertex3d* GetFirstVertexNeighborWithFlag(int flag);
	CVertex3d* GetNearestVertexNeighborWithFlag(int flag);
	CVertex3d* GetFirstVertexNeighborWithFlagButDiff(int flag, CVertex3d* pVertexDiff);
	CVertex3d* GetFirstNeighborWithFlagAndSharingFaceWithFlag(int FlagVertex, int FlagFace);
	int DiffFlagsOnNeighboringFaces();
	int IsFlagSmallestOnNeighboringFaces(int flag);
	int FindVertexNeighborsWhichFlagIsGreater(int flag, CArray3d<CVertex3d>& array);
	int FindVertexNeighborsWhichFlagIsSmaller(int flag, CArray3d<CVertex3d>& array);
	int NbVertexNeighborWithFlag(int flag);

	// Bag, dirty :-(
	void SetBag(void* pBag) { m_pBag = pBag; }
	void* GetBag() { return m_pBag; }

	// Normal
	void SetNormal(const double x, const double y, const double z) { m_Normal.Set(x, y, z); }
	void SetNormal(CVector3d& vector) { m_Normal.Set(vector); }
	CVector3d* GetNormal(void) { return &m_Normal; }

	// Operators
	CVertex3d operator=(CVertex3d& vertex);
	int Diff(CVertex3d* pVertex);
	int Equal(CVertex3d* pVertex);

	// Vertex neighbors
	int  NbVertexNeighbor(void);
	void AddNeighbor(CVertex3d* pVertex);
	void AddNeighborNoCheck(CVertex3d* pVertex);
	int  HasNeighbor(CVertex3d* pVertex) { return m_ArrayVertexNeighbor.Has(pVertex); }
	int  RemoveNeighbor(CVertex3d* pVertex);
	void RemoveAllVertexNeighbor(void);
	void UpdateNeighborRecursive(CVertex3d* pVertexOld, CVertex3d* pVertexNew);
	int  UpdateNeighbor(CVertex3d* pVertexOld, CVertex3d* pVertexNew);
	int FindFaceAroundContainVertex(CVertex3d* pVertex, CArray3d<CFace3d>& ArrayFace);
	CVertex3d* GetVertexNeighbor(const unsigned int index) { return m_ArrayVertexNeighbor[index]; }
	CArray3d<CVertex3d>* GetArrayVertexNeighbor() { return &m_ArrayVertexNeighbor; }

	// Face neighbors
	int  NbFaceNeighbor(void);
	void AddNeighbor(CFace3d* pFace);
	int  RemoveNeighbor(CFace3d* pFace);
	void RemoveAllFaceNeighbor(void);
	int  HasNeighbor(CFace3d* pFace) { return m_ArrayFaceNeighbor.Has(pFace); }
	CFace3d* GetFaceNeighbor(const unsigned int index) { return m_ArrayFaceNeighbor[index]; }
	CArray3d<CFace3d>* GetArrayFaceNeighbor() { return &m_ArrayFaceNeighbor; }

	// Face sharing
	int FindSharingFaces(CVertex3d* pVertex, CArray3d<CFace3d>& array);

	// Area
	double GetMeanAreaAround();
	double GetMinAreaAround();
	// Distance
	double GetMeanLengthEdgeAround();
	// Curve
	double GetMeanCurveAround();
	double GetSumCurveAround();
	double GetMaxCurveAround();
	double GetMaxAngleAround();
	double GetMaxCurveAroundAndNeighbors();

	// Precision
	void ReplaceOnGrid(float threshold);

	// Boundaries
	int IsOnBoundary();

	// Count sharp edges around vertex
	int NbSharpEdge(const double threshold = 0.5);

	// Normals's 
	int NormalSum(double* pSum);
	int NormalMax(double* pMax);

	// Debug
	void Trace();

	// OpenGL highlighting
	void glDrawHighlight(const float radius, const float RadiusNeighbor,
		unsigned char* ColorVertex, CMesh3d* pMesh = NULL,
		unsigned char* ColorNeightbor = NULL);
	virtual int glDraw();
	void glDraw(const float radius, unsigned char* ColorVertex, CMesh3d* pMesh = NULL);
	void SetMCoord(const double m1, const double m2, const double m3)
	{
		m_Coord[0] = m1;
		m_Coord[1] = m2;
		m_Coord[2] = m3;
	}

	virtual int PickTest(PickStruct& pickstruct);
};



// Constructor
inline CVertex3d::CVertex3d(CVertex3d& vertex)
{
	m_Coord[0] = vertex.x();
	m_Coord[1] = vertex.y();
	m_Coord[2] = vertex.z();
	m_Flag = 0;
	m_Color = vertex.m_Color;
}

// Constructor
inline CVertex3d::CVertex3d(CVertex3d* pVertex)
{
	m_Coord[0] = pVertex->x();
	m_Coord[1] = pVertex->y();
	m_Coord[2] = pVertex->z();
	m_Flag = 0;
	m_Color = pVertex->m_Color;
}

// Constructor
inline CVertex3d::CVertex3d(CVertex3d* pVertex, CVector3d* pVector)
{
	m_Coord[0] = pVertex->x() + pVector->x();
	m_Coord[1] = pVertex->y() + pVector->y();
	m_Coord[2] = pVertex->z() + pVector->z();
	m_Flag = 0;
}

// Constructor
inline CVertex3d::CVertex3d(const double x, const double y, const double z)
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;
	m_Flag = 0;
}

// Setting
inline void CVertex3d::Set(const double x, const double y, const double z)
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;
}

// Setting
inline void CVertex3d::Set(CVertex3d* pVertex)
{
	m_Coord[0] = pVertex->x();
	m_Coord[1] = pVertex->y();
	m_Coord[2] = pVertex->z();
}

// Setting
inline void CVertex3d::Set(CVertex3d& vertex)
{
	m_Coord[0] = vertex.x();
	m_Coord[1] = vertex.y();
	m_Coord[2] = vertex.z();
}

// Moving
inline void CVertex3d::Move(const double dx,
	const double dy,
	const double dz)
{
	m_Coord[0] += dx;
	m_Coord[1] += dy;
	m_Coord[2] += dz;
}

// Moving
inline void CVertex3d::Move(CVector3d* pVector,
	double ratio)
{
	m_Coord[0] += ratio * pVector->x();
	m_Coord[1] += ratio * pVector->y();
	m_Coord[2] += ratio * pVector->z();
}

// Moving
inline void CVertex3d::Move(CVector3d& vector,
	double ratio)
{
	m_Coord[0] += ratio * vector.x();
	m_Coord[1] += ratio * vector.y();
	m_Coord[2] += ratio * vector.z();
}


#endif // _VERTEX_3D_
