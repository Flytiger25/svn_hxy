#pragma once
class ShapeFile_Polygon_Header
{
public:
	int m_FileCode;
	int m_ArrayUnused[5];
	int m_FileLength;
	int m_Version;
	int m_ShapeType;
	double m_boundingbox_xmin;
	double m_boundingbox_ymin;
	double m_boundingbox_xmax;
	double m_boundingbox_ymax;
	double m_boundingbox_zmin;
	double m_boundingbox_zmax;
	double m_boundingbox_mmin;
	double m_boundingbox_mmax;
	ShapeFile_Polygon_Header();
	~ShapeFile_Polygon_Header();
};

