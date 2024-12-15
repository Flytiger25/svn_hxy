#pragma once
#include "lib3d\object3d.h"
#include "lib3d\Transform.h"

class C_NurbsCurve3D;
class ReferenceLine;
class CNurbsCurve :public CObject3d
{
public:

	//the pointer of the curve
	C_NurbsCurve3D* pCurve;

	//Reference lines
	std::vector<ReferenceLine*> m_ArrayReferenceLines;

	//Line Segements
	std::vector<double> m_ArraySegmentParameters;

	const int CURVE_DISCRETE_NUM = 100;
	//
	std::vector<COLORREF> arrayCurveTessellationColorBar_SDL;

	//
	int m_showControlPoints;

	//
	int m_showReferenceLines;

	//
	int m_showLineSegments;

	//
	int m_showSDLEnergy;

public:
	/*提供的函数*/
	CNurbsCurve();
	~CNurbsCurve(void);

	//CTransform	 m_Transform;
	int glDraw();

	//
	void glDrawLineSegments();

	//
	void glDrawControlPoints();

	//draw reference lines
	void glDrawReferenceLines();

	void glDrawCurve();

	virtual int PickTest(PickStruct& pickstruct) { return -1; }
	//int m_Show;
	double c_r, c_g, c_b;
};
