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
#include "../Action_PolyOmino.h"
#include <glut.h>
#include "Mesh3d_OpenMesh.h"
#include "../GeoMapLandMark.h"
//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////
extern Action_PolyOmino* pPolyOmino;

void CMesh3d::ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6, double x7, double y7, double x8, double y8,double x9,double y9,double x10,double y10, std::ofstream &file, std::ofstream &edgefile,int type)
{
	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x2 << " ," << y2 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x5 << " ," << y5 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x6 << " ," << y6 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x7 << " ," << y7 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x8 << " ," << y8 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x9 << " ," << y9 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x9 << " ," << y9 << " , -0 >,<" << x10 << " ," << y10 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x10 << " ," << y10 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -h >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -h >,<" << x7 << " ," << y7 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x8 << " ," << y8 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x9 << " ," << y9 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x9 << " ," << y9 << " , -0 >,<" << x10 << " ," << y10 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x10 << " ," << y10 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x7 << " ," << y7 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x8 << " ," << y8 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x9 << " ," << y9 << " , -0 >,<" << x9 << " ," << y9 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x10 << " ," << y10 << " , -0 >,<" << x10 << " ," << y10 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	file << "prism{ linear_sweep linear_spline 0, h, 11, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">, <" << x7 << "," << y7 << ">,<" << x8 << "," << y8 << ">,<" << x9 << "," << y9 << ">,<" << x10 << "," << y10 << ">,<"  << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << type << "} } \n";
}

void CMesh3d::ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5, double y5, double x6, double y6,double x7,double y7,double x8,double y8, std::ofstream &file, std::ofstream &edgefile, int type)
{
	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x2 << " ," << y2 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x5 << " ," << y5 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x6 << " ," << y6 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x7 << " ," << y7 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x8 << " ," << y8 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -h >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -h >,<" << x7 << " ," << y7 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x8 << " ," << y8 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x7 << " ," << y7 << " , -0 >,<" << x7 << " ," << y7 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x8 << " ," << y8 << " , -0 >,<" << x8 << " ," << y8 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	file << "prism{ linear_sweep linear_spline 0, h, 9, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">, <" << x7 << "," << y7 << ">,<" << x8 << "," << y8 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << type << "} } \n";
}

void CMesh3d::ExportPovray(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double x5,double y5, double x6,double y6,std::ofstream &file, std::ofstream &edgefile, int type)
{
	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x2 << " ," << y2 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x5 << " ," << y5 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x6 << " ," << y6 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -h >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -h >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	file << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << type << "} } \n";
}

void CMesh3d::ExportPovray(double x1,double y1,double x2,double y2,double x3,double y3 ,double x4,double y4,std::ofstream &file, std::ofstream &edgefile, int type)
{
		edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x2 << " ," << y2 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

		edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

		edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
		edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

		file << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << type << "} } \n";
}
int CMesh3d::plotPolyominoType3(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10;

	//if (type != 7)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4,x5,y5,x6,y6, file, edgefile, type);
	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 5)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 6)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 7)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 8)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		x7 = x1;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,x7,y7,x8,y8,x9,y9,x10,y10, file, edgefile, type);
	}
	else if (type == 9)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 10)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 11)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * (x + 2) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 2) - tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 12)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * (x + 1) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 2) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 13)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * (x ) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 14)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 15)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x ) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = xinterval[0] + baseinterval * (x - 1)+tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x - 2) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 16)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = xinterval[0] + baseinterval * (x + 1) + tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 17)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 18)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = xinterval[0] + baseinterval * (x) + tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 19)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 20)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * (x + 2) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;
		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	return 1;
}
int CMesh3d::plotPolyominoType2(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,x7,y7,x8,y8,x9,y9,x10,y10;

	//if (type != 7)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 4) - tol;
		x4 = x1;
		y4 = y3;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, file, edgefile,type);
	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4,x5,y5,x6,y6, file, edgefile, type);
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x3;
		y6 = y5;
		x7 = x3;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = x1;
		y8 = y7;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,x7,y7,x8,y8, file, edgefile, type);
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		x7 = x1;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8,x9,y9,x10,y10, file, edgefile, type);
	}
	else if (type == 5)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x3;
		y6 = y5;
		x7 = x3;
		y7 = yinterval[0] + baseinterval * (y + 1) - tol;
		x8 = x1;
		y8 = y7;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, file, edgefile, type);
	}
	else if (type == 6)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = xinterval[0] + baseinterval * (x + 2) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 7)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * x - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x - 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) + tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 8)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 4) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = x1;
		y4 = y3;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, file, edgefile, type);
	}
	else if (type == 9)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 10)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = xinterval[0] + baseinterval * (x + 1) + tol;
		y6 = y5;
		x7 = x6;
		y7 = y4;
		x8 = x1;
		y8 = y7;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, file, edgefile, type);
	}
	else if (type == 11)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x3;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 12)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * y - tol;
		x6 = xinterval[0] + baseinterval * (x + 2) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 13)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * y + tol;
		x6 = xinterval[0] + baseinterval * (x + 3) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 2) - tol;
		x8 = xinterval[0] + baseinterval * (x + 2) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 14)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * y + tol;
		x6 = xinterval[0] + baseinterval * (x + 3) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 1) - tol;
		x8 = x1;
		y8 = y7;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, file, edgefile, type);
	}
	else if (type == 15)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = xinterval[0] + baseinterval * (x + 2) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * (y + 3) - tol;
		x8 = xinterval[0] + baseinterval * (x + 1) + tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 2) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	else if (type == 16)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 17)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file, edgefile, type);
	}
	else if (type == 18)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y - 1) + tol;
		x6 = xinterval[0] + baseinterval * (x + 3) - tol;
		y6 = y5;
		x7 = x6;
		y7 = yinterval[0] + baseinterval * y - tol;
		x8 = xinterval[0] + baseinterval * (x + 2) - tol;
		y8 = y7;
		x9 = x8;
		y9 = yinterval[0] + baseinterval * (y + 1) - tol;
		x10 = x1;
		y10 = y9;

		ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file, edgefile, type);
	}
	return 1;
}
int CMesh3d::plotPolyominoType1(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;

	//if (type != 5)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 4) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 4) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x2 << " ," << y2 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	file << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << type << "} } \n";

	return 1;
}
int CMesh3d::plotPolyominoType1(int type, int x, int y)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;

	//if (type != 5)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 4) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 4) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = x1;
		y4 = y3;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
	}
	return 1;
}
int CMesh3d::plotPolyominoType_world(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile)
{
	switch (type)
	{
	case 0:
		plotPolyominoType1(1, x, y, file, edgefile);
		break;
	case 1:
		plotPolyominoType1(2, x, y, file, edgefile);
		break;
	case 2:
		plotPolyominoType1(3, x, y, file, edgefile);
		break;
	case 3:
		plotPolyominoType1(4, x, y, file, edgefile);
		break;
	case 4:
		plotPolyominoType(5, x, y, file, edgefile);
		break;
	case 5:
		plotPolyominoType(6, x, y, file, edgefile);
		break;
	case 6:
		plotPolyominoType(9, x, y, file, edgefile);
		break;
	case 7:
		plotPolyominoType(12, x, y, file, edgefile);
		break;
	case 8:
		plotPolyominoType(14, x, y, file, edgefile);
		break;
	}
	return 1;
}
int CMesh3d::plotPolyominoType(int type, int x, int y, std::ofstream &file, std::ofstream &edgefile)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5,x6,y6;

	//if (type != 5)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = yinterval[0] + baseinterval * (y + 1) + tol;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x1, y1, 0);
		::glEnd();

		//face
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x1 << "," << y1 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/

		//edge

	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = yinterval[0] + baseinterval * (y + 1) - tol;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 5)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x4 << "," << y4 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 6)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 7)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 8)
	{
		//return 1;
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 9)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 10)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 11)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 12)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x - 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) + tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x3, y3, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x4 << "," << y4 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 13)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 14)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<"<< x2 <<" ,"<< y2 <<" , -0 > ,r_edge texture{ edge_tex"<< type <<" } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x3 << " ," << y3 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x4 << " ," << y4 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x5 << " ," << y5 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x6 << " ," << y6 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x1 << " ," << y1 << " , -0 > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -h >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -h >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -h >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -h >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -h >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -h >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	edgefile << "cylinder{ <" << x1 << " ," << y1 << " , -0 >,<" << x1 << " ," << y1 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x2 << " ," << y2 << " , -0 >,<" << x2 << " ," << y2 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x3 << " ," << y3 << " , -0 >,<" << x3 << " ," << y3 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x4 << " ," << y4 << " , -0 >,<" << x4 << " ," << y4 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x5 << " ," << y5 << " , -0 >,<" << x5 << " ," << y5 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";
	edgefile << "cylinder{ <" << x6 << " ," << y6 << " , -0 >,<" << x6 << " ," << y6 << " , -h > ,r_edge texture{ edge_tex" << type << " } }\n";

	file << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 <<", "<< y1 << ">, <" << x2 <<", "<<y2<<">, <"<<x3<<"," << y3<< ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<"<< x6 <<","<<y6<<">,<"<<x1<<","<<y1<<"> rotate <-90, 0, 0> texture{ T"<<type<<"} } \n";

	return 1;
}

int CMesh3d::plotPolyominoType(int type, int x, int y)
{
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;

	//if (type != 5)
	//	return 1;
	if (type == 1)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = yinterval[0] + baseinterval * (y + 1) + tol;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;

		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x1, y1, 0);
		::glEnd();

		//face
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x1 << "," << y1 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/

		//edge

	}
	else if (type == 2)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = yinterval[0] + baseinterval * (y + 1) - tol;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 3)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x + 2) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 4)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 3) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 5)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x4 << "," << y4 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 6)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();

		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 7)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 8)
	{
		//return 1;
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 9)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) + tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y - 1) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x2, y2, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x2 << "," << y2 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 10)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) + tol;
		x4 = xinterval[0] + baseinterval * (x + 2) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 3) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();
		/*file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x3 << "," << y3 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";*/
	}
	else if (type == 11)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 3) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 1) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) - tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 2) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);
		::glVertex3f(x5, y5, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x6 << "," << y6 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 12)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 1) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x - 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) + tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x4, y4, 0);

		::glVertex3f(x4, y4, 0);
		::glVertex3f(x6, y6, 0);
		::glVertex3f(x3, y3, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x4 << "," << y4 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 13)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 3) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();
		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}
	else if (type == 14)
	{
		x1 = xinterval[0] + baseinterval * x + tol;
		y1 = yinterval[0] + baseinterval * y + tol;
		x2 = xinterval[0] + baseinterval * (x + 2) - tol;
		y2 = y1;
		x3 = x2;
		y3 = yinterval[0] + baseinterval * (y + 2) - tol;
		x4 = xinterval[0] + baseinterval * (x + 1) + tol;
		y4 = y3;
		x5 = x4;
		y5 = yinterval[0] + baseinterval * (y + 1) - tol;
		x6 = x1;
		y6 = y5;
		::glBegin(GL_TRIANGLES);
		::glVertex3f(x1, y1, 0);
		::glVertex3f(x5, y5, 0);
		::glVertex3f(x6, y6, 0);

		::glVertex3f(x1, y1, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x5, y5, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x2, y2, 0);
		::glVertex3f(x3, y3, 0);

		::glVertex3f(x5, y5, 0);
		::glVertex3f(x3, y3, 0);
		::glVertex3f(x4, y4, 0);
		::glEnd();

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x6 << "," << y6 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x1 << "," << y1 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x5 << "," << y5 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x2 << "," << y2 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";

		//file << "smooth_triangle { < " << x5 << "," << y5 << " , " << 0 << " >,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x3 << "," << y3 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << ">,<";
		//file << x4 << "," << y4 << "," << 0 << ">,<" << 0 << "," << 0 << "," << -1 << "> texture{T" << type << "}}" << "\n";
	}

	return 1;
}
int CMesh3d::PlotPolyominoes_world()
{
	std::ifstream file("D:\\puzzles.txt");
	std::vector<std::vector<std::pair<int, int>>> m_ArrayPolyominos;
	std::vector<std::pair<int, int>> polyominos;
	std::pair<int, int> pair;
	int index, index1, index2;
	while (!file.eof())
	{
		file >> index;
		polyominos.clear();
		while (1)
		{
			file >> index1 >> index2;
			if (index1 < 0)
				break;
			pair.first = index1;
			pair.second = index2;
			polyominos.push_back(pair);
		}
		m_ArrayPolyominos.push_back(polyominos);
	}
	file.close();
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	for (int i = 0; i < m_ArrayPolyominos.size(); i++)
	{
		for (int j = 0; j < m_ArrayPolyominos[i].size(); j++)
		{
			pair = m_ArrayPolyominos[i][j];

			::glColor3f(1.0, 0, 0);
			plotPolyominoType_world(i, pair.first, pair.second, file1, file2);
			//PlotCell(pair.first, pair.second);
			//PlotCell(pair.first, pair.second + 1);
			//PlotCell(pair.first + 1, pair.second + 1);
			//PlotCell(pair.first + 2, pair.second + 1);

		}
	}
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
	int x = 2;
	int y = 10;

	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 3) - tol;
	x4 = x1;
	y4 = y3;

	file1 << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 1 << "} } \n";

	//file1 << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 8 << "} } \n";

	x = 5;
	y = 10;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 4) - tol;
	x4 = x1;
	y4 = y3;

	file1 << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 2 << "} } \n";

	x = 8;
	y = 10;

	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 2) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 2) - tol;
	x6 = x1;
	y6 = y5;

	file1 << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 5 << "} } \n";

	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}
int CMesh3d::PlotPolyominoes1()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\puzzle\\puzzle1.txt");
	std::vector<std::vector<std::pair<int, int>>> m_ArrayPolyominos;
	std::vector<std::pair<int, int>> polyominos;
	std::pair<int, int> pair;
	int index, index1, index2;
	while (!file.eof())
	{
		file >> index;
		polyominos.clear();
		while (1)
		{
			file >> index1 >> index2;
			if (index1 < 0)
				break;
			pair.first = index1;
			pair.second = index2;
			polyominos.push_back(pair);
		}
		m_ArrayPolyominos.push_back(polyominos);
	}
	file.close();
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	for (int i = 0; i < m_ArrayPolyominos.size(); i++)
	{
		for (int j = 0; j < m_ArrayPolyominos[i].size(); j++)
		{
			pair = m_ArrayPolyominos[i][j];

			::glColor3f(1.0, 0, 0);
			plotPolyominoType(i + 1, pair.first, pair.second,file1,file2);
			//PlotCell(pair.first, pair.second);
			//PlotCell(pair.first, pair.second + 1);
			//PlotCell(pair.first + 1, pair.second + 1);
			//PlotCell(pair.first + 2, pair.second + 1);

		}
	}
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
	int x = 2;
	int y = 2;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 2) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = x1;
	y6 = y5;
	file1 << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 8 << "} } \n";

	x = 5;
	y = 2;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 2) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 2) - tol;
	x6 = x1;
	y6 = y5;
	file1 << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 5 << "} } \n";

	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}
int CMesh3d::PlotPolyominoes2()
{
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
	int x = 2;
	int y = 22;
	x1 = xinterval[0] + baseinterval * (x + 1) - tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	
	x2 = xinterval[0] + baseinterval * x + tol;
	y2 = y1;

	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;

	x4 = x1;
	y4 = y3;

	file1 << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 1 << "} } \n";

	x = 5;
	y = 2;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 2) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 2) - tol;
	x6 = x1;
	y6 = y5;
	file1 << "prism{ linear_sweep linear_spline 0, h, 7, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">, <" << x5 << "," << y5 << ">,<" << x6 << "," << y6 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 5 << "} } \n";

	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}
int CMesh3d::PlotPolyominoes3()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\puzzle\\puzzle2.txt");
	std::vector<std::vector<std::pair<int, int>>> m_ArrayPolyominos;
	std::vector<std::pair<int, int>> polyominos;
	std::pair<int, int> pair;
	int index, index1, index2;
	while (!file.eof())
	{
		file >> index;
		polyominos.clear();
		while (1)
		{
			file >> index1 >> index2;
			if (index1 < 0)
				break;
			pair.first = index1;
			pair.second = index2;
			polyominos.push_back(pair);
		}
		m_ArrayPolyominos.push_back(polyominos);
	}
	file.close();
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	for (int i = 0; i < m_ArrayPolyominos.size(); i++)
	{
		for (int j = 0; j < m_ArrayPolyominos[i].size(); j++)
		{
			pair = m_ArrayPolyominos[i][j];

			::glColor3f(1.0, 0, 0);
			plotPolyominoType1(i + 1, pair.first, pair.second, file1, file2);
			//PlotCell(pair.first, pair.second);
			//PlotCell(pair.first, pair.second + 1);
			//PlotCell(pair.first + 1, pair.second + 1);
			//PlotCell(pair.first + 2, pair.second + 1);

		}
	}
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
	int x = 1;
	int y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 3) - tol;
	x4 = x1;
	y4 = y3;
	file1 << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 8 << "} } \n";

	x = 3;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 4) - tol;
	x4 = x1;
	y4 = y3;
	file1 << "prism{ linear_sweep linear_spline 0, h, 5, <" << x1 << ", " << y1 << ">, <" << x2 << ", " << y2 << ">, <" << x3 << "," << y3 << ">, <" << x4 << ", " << y4 << ">,<" << x1 << "," << y1 << "> rotate <-90, 0, 0> texture{ T" << 9 << "} } \n";
	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}
int CMesh3d::PlotPolyominoes4()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\puzzle\\puzzle3.txt");
	std::vector<std::vector<std::pair<int, int>>> m_ArrayPolyominos;
	std::vector<std::pair<int, int>> polyominos;
	std::pair<int, int> pair;
	int index, index1, index2;
	while (!file.eof())
	{
		file >> index;
		polyominos.clear();
		while (1)
		{
			file >> index1 >> index2;
			if (index1 < 0)
				break;
			pair.first = index1;
			pair.second = index2;
			polyominos.push_back(pair);
		}
		m_ArrayPolyominos.push_back(polyominos);
	}
	file.close();
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	for (int i = 0; i < m_ArrayPolyominos.size(); i++)
	{
		for (int j = 0; j < m_ArrayPolyominos[i].size(); j++)
		{
			pair = m_ArrayPolyominos[i][j];

			::glColor3f(1.0, 0, 0);
			plotPolyominoType2(i + 1, pair.first, pair.second, file1, file2);
			//PlotCell(pair.first, pair.second);
			//PlotCell(pair.first, pair.second + 1);
			//PlotCell(pair.first + 1, pair.second + 1);
			//PlotCell(pair.first + 2, pair.second + 1);

		}
	}
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6,x7,y7,x8,y8,x9,y9,x10,y10;
	int x = 1;
	int y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 4) - tol;
	x4 = x1;
	y4 = y3;

	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, file1, file2,1);

	x = 2;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 3) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = x1;
	y6 = y5;

	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file1, file2,2);

	x = 5;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 3) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 2) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = xinterval[0] + baseinterval * (x + 1) + tol;
	y6 = y5;
	x7 = x6;
	y7 = y4;
	x8 = x1;
	y8 = y7;

	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, file1, file2, 10);

	x = 9;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 2) + tol;
	x4 = xinterval[0] + baseinterval * (x + 2) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = x1;
	y6 = y5;
	x7 = x1;
	y7 = yinterval[0] + baseinterval * (y + 2) - tol;
	x8 = xinterval[0] + baseinterval * (x - 1) + tol;
	y8 = y7;
	x9 = x8;
	y9 = yinterval[0] + baseinterval * (y + 1) + tol;
	x10 = x1;
	y10 = y9;

	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file1, file2,4);

	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}
int CMesh3d::PlotPolyominoes5()
{
	std::ifstream file("D:\\papers\\code\\data\\Geo\\puzzle\\puzzle4.txt");
	std::vector<std::vector<std::pair<int, int>>> m_ArrayPolyominos;
	std::vector<std::pair<int, int>> polyominos;
	std::pair<int, int> pair;
	int index, index1, index2;
	while (!file.eof())
	{
		file >> index;
		polyominos.clear();
		while (1)
		{
			file >> index1 >> index2;
			if (index1 < 0)
				break;
			pair.first = index1;
			pair.second = index2;
			polyominos.push_back(pair);
		}
		m_ArrayPolyominos.push_back(polyominos);
	}
	file.close();
	std::ofstream file1("D:\\plotpuzzle.inc");
	std::ofstream file2("D:\\plotpuzzleedges.inc");
	//file1 << "mesh {" << "\n";
	for (int i = 0; i < m_ArrayPolyominos.size(); i++)
	{
		for (int j = 0; j < m_ArrayPolyominos[i].size(); j++)
		{
			pair = m_ArrayPolyominos[i][j];

			::glColor3f(1.0, 0, 0);
			plotPolyominoType3(i + 1, pair.first, pair.second, file1, file2);
			//PlotCell(pair.first, pair.second);
			//PlotCell(pair.first, pair.second + 1);
			//PlotCell(pair.first + 1, pair.second + 1);
			//PlotCell(pair.first + 2, pair.second + 1);

		}
	}
	double tol = baseinterval * 0.1;
	double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
	double x7, y7, x8, y8, x9, y9, x10, y10;
	int x = 1;
	int y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 2) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = x1;
	y6 = y5;
	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file1, file2, 9);

	x = 3;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 3) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) - tol;
	x4 = xinterval[0] + baseinterval * (x + 1) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 3) - tol;
	x6 = x1;
	y6 = y5;
	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, file1, file2, 19);

	x = 6;
	y = 0;
	x1 = xinterval[0] + baseinterval * x + tol;
	y1 = yinterval[0] + baseinterval * y + tol;
	x2 = xinterval[0] + baseinterval * (x + 1) - tol;
	y2 = y1;
	x3 = x2;
	y3 = yinterval[0] + baseinterval * (y + 1) + tol;
	x4 = xinterval[0] + baseinterval * (x + 3) - tol;
	y4 = y3;
	x5 = x4;
	y5 = yinterval[0] + baseinterval * (y + 2) - tol;
	x6 = xinterval[0] + baseinterval * (x + 2) - tol;
	y6 = y5;
	x7 = x6;
	y7 = yinterval[0] + baseinterval * (y + 3) - tol;
	x8 = xinterval[0] + baseinterval * (x + 1) + tol;
	y8 = y7;
	x9 = x8;
	y9 = yinterval[0] + baseinterval * (y + 2) - tol;
	x10 = x1;
	y10 = y9;
	ExportPovray(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, file1, file2, 11);
	//file1 << "}" << "\n";
	file1.close();
	//file2 << "}" << "\n";
	file2.close();
	return 1;
}