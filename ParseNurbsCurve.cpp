#include "stdafx.h"
#include "lib3d\Base3d.h"
#include <fstream>
#include "Nurbs3D.h"
#include "ParseNurbsCurve.h"
#include "NurbsCurve.h"
#include "Curve.h"
#include <fstream>

CParseNurbsCurve::CParseNurbsCurve(void)
{
}

CParseNurbsCurve::~CParseNurbsCurve(void)
{
}
int CParseNurbsCurve::Run(char* filename, CSceneGraph3d* pSceneGraph)
{
	//这个函数用来真正的读入数据，并且运用读入的数据构造一个CNurbsface()
	CNurbsCurve* pCurve = new CNurbsCurve();
	std::ifstream file(filename);	//将整个文件读入file中
	int numConPoint;
	int degree;
	int type;
	file >> type;
	file >> numConPoint;			//读入控制定点数

	file >> degree;					//读入度数
	int numOfCon;
	double* knotarray = new double[numConPoint + degree + 1];	//初始化knotVector
	for (int i = 0; i < numConPoint + degree + 1; i++)
	{
		file >> knotarray[i];
	}
	Point3D* controlarray = new Point3D[numConPoint];
	double* weightarray = new double[numConPoint];
	//double (*conVertex4f)[4]=new GLfloat [numOfConPoints+degree+1][4];
	for (int i = 0; i < numConPoint; i++)
	{
		file >> controlarray[i][0] >> controlarray[i][1] >> controlarray[i][2] >> weightarray[i];
	}

	pCurve->pCurve = new C_NurbsCurve3D(numConPoint, degree + 1, controlarray, knotarray, weightarray);


	//Get Bounding box
	Vector3D m_lower, m_upper;
	pCurve->pCurve->GetBoundingBox(m_lower, m_upper);

	CTransform transform;

	transform.SetTransformFromBoundingBox(&m_lower, &m_upper);

	if (pSceneGraph->NbObject() >= 1)
	{
		pCurve->SetTransform(*pSceneGraph->GetAt(0)->GetTransform());
	}
	else
		pCurve->SetTransform(transform);
	pSceneGraph->Add(pCurve);

	return 1;
}