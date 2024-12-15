#pragma once
#include "Point3D.h"
#include <vector>
#include "ShapeFile_Polygon_Header.h"
#include "lib3d\Array3d.h"
#include "lib3d\Vertex3d.h"
#include "lib3d\Face3d.h"
#include "lib3d\Vector3d.h"
#include "lib3d\Material.h"
#include "lib3d\Transform.h"
#include "Point3D.h"	// Added by ClassView
#include "Dib.h"
#include "GeoMapLandMark.h"
#include <fstream>
class Record_Header
{
public:
	int m_recordnumber;
	int m_contentlength;
};
class Record_Content
{
public:
	int     m_ShapeTye;   //Shape Type
	double  m_BoundingBox[4]; //bounding box
	int		m_NumParts;  //number of parts
	int		m_NumPoints; //number of points
	std::vector<int> m_ArrayParts;  //the first point index of each ring
	std::vector<Point2D> m_ArrayPoints; //the points array
	std::vector<int> m_ArrayFlag; //flag
	std::vector<double> m_ArrayArea; //area of each ring
	std::vector<int> m_ArrayRingIndex; //Ring index
};

class Record
{
public:
	Record_Header m_recordheader;
	Record_Content m_recordcontent;
};

class Shape_Polygon : public CObject3d
{
public:  //data structure
	int m_showPolygonRingEdge; //show edge
	int m_showPolygonRingIndex;
	int m_showPolygonInnerRing;

	ShapeFile_Polygon_Header header;
	std::vector<Record*> m_ArrayRecords;
public:
	Shape_Polygon();
	~Shape_Polygon();

	//glDraw
	virtual int GetType();
	virtual int glBuildList();
	virtual int glDraw();

	//
	int ShowPolygonInnerRing();

	//
	int ShowPolygonRingIndex();

	//
	int ShowPolygonRings();

	//Compute Area
	double polygonArea(double* X, double* Y, int numPoints);

	//
	double polygonArea(Record* pRecord, int ringindex);
};

