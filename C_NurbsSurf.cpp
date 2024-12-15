// _NurbsSurf.cpp : implementation file
//

#include "stdafx.h"
#include "sdiviewswitch.h"
#include "InterFunction.h"
#include "Nurbs3D.h"
#include "curve.h"
#include "math.h"
#include "C_NurbsSurf.h"
#include <sstream> 
#include <iostream>
#include <fstream>
#include "SDIViewSwitch.h"
#include "FormView3.h"
#include "lib3d\Transform.h"
#include "Disline2D.h"
#include "disline.h"
#include "NurbsCurve.h"
#include "lib3d\Base3d.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
C_Surf3D::C_Surf3D(void)
{
	sArray = NULL;
	tArray = NULL;
	snum = 0;
	tnum = 0;
}

C_Surf3D::~C_Surf3D()
{
}



int C_Surf3D::GetClosestPoint(class Point3D const&, class Point2D*, class Point3D&, class Point2D&)
{
	return 0;
}

int C_Surf3D::GetPointPara(Point3D p, Point2D& uv, Point2D*& alluv)
{
	return 0;
}

Point3D C_Surf3D::GetParaPoint(double u, double v)
{
	return Point3D(0, 0, 0);
}

Point3D C_Surf3D::GetParaPoint(Point2D uv)
{
	return GetParaPoint(uv.x, uv.y);
}

Vector3D C_Surf3D::GetParaNormal(double u, double v)
{
	return Vector3D(0, 0, 0);
}

Vector3D C_Surf3D::GetParaNormal(Point2D uv)
{
	return GetParaNormal(uv.x, uv.y);
}

BOOL C_Surf3D::PointOn(Point3D p, double toler)
{
	return TRUE;
}

C_Surf3D* C_Surf3D::Copy(void)
{
	return this;
}

C_Surf3D* C_Surf3D::Offset(BOOL dirtag, double depth)
{
	return this;
}

void C_Surf3D::Expand(double du, double dv)
{
	return;
}

BOOL C_Surf3D::PointInPositiveSpace(Point3D point)
{
	return TRUE;
}



// 构造函数和析构函数
C_BSplineSurf::C_BSplineSurf(void)
{
}


C_BSplineSurf::C_BSplineSurf(int number_u, int number_v, int order_u, int order_v) :
	C_ParaSurf(number_u, number_v)
{
	m_order_u = order_u;
	m_order_v = order_v;
}


C_BSplineSurf::C_BSplineSurf(int number_u, int number_v, int order_u,
	int order_v, Point3D* control) :
	C_ParaSurf(number_u, number_v, control)
{
	m_order_u = order_u;
	m_order_v = order_v;
}


C_BSplineSurf::~C_BSplineSurf(void)
{
}



// 用于数据存取和显示的成员函数
int C_BSplineSurf::GetOrderU(void)
{
	return m_order_u;
}


int C_BSplineSurf::GetOrderV(void)
{
	return m_order_v;
}


int C_BSplineSurf::GetDegreeU(void)
{
	return m_order_u - 1;
}


int C_BSplineSurf::GetDegreeV(void)
{
	return m_order_v - 1;
}


void C_BSplineSurf::SetOrder(int order_u, int order_v)
{
	m_order_u = order_u;
	m_order_v = order_v;
}


int C_BSplineSurf::PrintInformation(void)
{
	return 0;
}


int C_ParaSurf::IndexMap(int i, int j)  // 从(i,j)得到表中序号
{
	return j * m_number_u + i;
}


// 构造函数和析构函数
C_ParaSurf::C_ParaSurf(void)
{
	m_control = NULL;
	m_number_u = 0;
	m_number_v = 0;
}



C_ParaSurf::C_ParaSurf(int number_u, int number_v)
{
	m_number_u = number_u;
	m_number_v = number_v;
	m_control = new Point3D[number_u * number_v];
}



C_ParaSurf::C_ParaSurf(int number_u, int number_v, Point3D* control)
{
	m_number_u = number_u;
	m_number_v = number_v;
	m_control = new Point3D[number_u * number_v];
	for (int i = 0; i < number_u * number_v; i++)
		m_control[i] = control[i];
}



C_ParaSurf::~C_ParaSurf(void) // 释放m_control的内存
{
	delete m_control;
}


// 用于数据存取和显示的成员函数
int C_ParaSurf::GetCtrlNumberU(void)
{
	return m_number_u;
}


int C_ParaSurf::GetCtrlNumberV(void)
{
	return m_number_v;
}


Point3D C_ParaSurf::GetControlPoint(int i, int j)
{
	return m_control[IndexMap(i, j)];
}


void C_ParaSurf::SetCtrlNumber(int number_u, int number_v)
{
	m_number_u = number_u;
	m_number_v = number_v;
	delete m_control;
	m_control = new Point3D[number_u * number_v];
}

CS_BSplineSurf::CS_BSplineSurf()
{
	knotu = NULL;
	knotv = NULL;
}
CS_NurbsSurf::CS_NurbsSurf()
{
	knotu = NULL;
	knotv = NULL;
	ctrl = NULL;
	weight = NULL;


	curve_left = NULL;
	curve_top = NULL;
}
void C_ParaSurf::SetControlPoint(int i, int j, Point3D point)
{
	m_control[IndexMap(i, j)] = point;
}












// 曲面控制顶点与权因子下标索引
int SPL_IndexMap(int nu, int nv, int i, int j)
{
	if (j >= nv)
	{
		j = nv - 1;
	}
	if (i >= nu)
	{
		i = nu - 1;
	}
	return j * nu + i;
}
int IndexMap(int nu, int nv, int i, int j)
{
	return SPL_IndexMap(nu, nv, i, j);
	/*
	if (j >= nv)
	{
	j = nv - 1;
	}
	if (i >= nu)
	{
	i = nu - 1;
	}
	return j*nu + i;
	*/
}
// 构造函数和析构函数
C_NonuniformBSurf::C_NonuniformBSurf(void)
{
	m_knot_u = NULL;
	m_knot_v = NULL;
}

C_NonuniformBSurf::C_NonuniformBSurf(int number_u, int number_v, int order_u, int order_v) :
	C_BSplineSurf(number_u, number_v, order_u, order_v)
{
	m_knot_u = new double[number_u + order_u];
	m_knot_v = new double[number_v + order_v];
}

C_NonuniformBSurf::C_NonuniformBSurf(int number_u, int number_v, int order_u, int order_v,
	double* knot_u, double* knot_v, Point3D* control) :
	C_BSplineSurf(number_u, number_v, order_u, order_v, control)
{
	int i;
	m_knot_u = new double[number_u + order_u];
	m_knot_v = new double[number_v + order_v];
	for (i = 0; i < number_u + order_u; i++)
		m_knot_u[i] = knot_u[i];
	for (i = 0; i < number_v + order_v; i++)
		m_knot_v[i] = knot_v[i];
}

C_NonuniformBSurf::~C_NonuniformBSurf(void)
{
	delete m_knot_u;
	delete m_knot_v;
}

// 用于数据存取和显示的成员函数
void C_NonuniformBSurf::SetNumberOrder(int number_u, int number_v, int order_u, int order_v)
{
	delete m_knot_u;
	delete m_knot_v;
	SetCtrlNumber(number_u, number_v);
	SetOrder(order_u, order_v);
	m_knot_u = new double[number_u + order_u];
	m_knot_v = new double[number_v + order_v];
}

double C_NonuniformBSurf::GetUKnot(int index)
{
	return m_knot_u[index];
}

double C_NonuniformBSurf::GetVKnot(int index)
{
	return m_knot_v[index];
}

void C_NonuniformBSurf::SetUKnot(int index, double knot)
{
	m_knot_u[index] = knot;
}

void C_NonuniformBSurf::SetVKnot(int index, double knot)
{
	m_knot_v[index] = knot;
}

int C_NonuniformBSurf::PrintInformation(void)
{
	return 0;
}

int C_NonuniformBSurf::GetUIntervalIndex(double u)
// 判别u在m_knot_u的那个节点区间
{
	/*
		int i;
		for( i = m_order_u - 1; i <= m_number_u; i++ )
			if( (u >= m_knot_u[i]-TOLER) && (u <= m_knot_u[i+1]+TOLER) && (m_knot_u[i+1] - m_knot_u[i] > TOLER) ) //sysTolZerovalue))
				return i;
		return -1;	// 非法值
	*/
	int a, b, c;
	a = m_order_u - 1;
	b = m_number_u;
	u -= TOLER;
	if (u <= m_knot_u[a])
		return(a); // Yong: 981119
	if (u >= m_knot_u[b])
		return(b); // Yong: 981119
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (u <= m_knot_u[c] - TOLER)
			b = c;
		else
			a = c;
	}
	//	if( (u >= m_knot_u[a]-(TOLER*2)) && (u <= m_knot_u[b]) ) // Removed by Jun-Hai Yong 2002-11-20
	return a;

	//    return -1;	// 非法值   // Removed by Jun-Hai Yong 2002-11-20
}

int C_NonuniformBSurf::GetVIntervalIndex(double v)
// 判别v在m_knot_v的那个节点区间
{
	/*
		int i;
		for( i = m_order_v - 1; i <= m_number_v; i++ )
			if( (v >= m_knot_v[i]-TOLER) && (v <= m_knot_v[i+1]+TOLER) && (m_knot_v[i+1] - m_knot_v[i] > TOLER) ) //sysTolZerovalue))
				return i;
		return -1;	// 非法值
	*/
	int a, b, c;
	a = m_order_v - 1;
	b = m_number_v;
	v -= TOLER;
	if (v <= m_knot_v[a])
		return(a); // Yong: 981119
	if (v >= m_knot_v[b])
		return(b); // Yong: 981119
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (v <= m_knot_v[c] - TOLER)
			b = c;
		else
			a = c;
	}
	//	if( (v >= m_knot_v[a]-(TOLER*2)) && (v <= m_knot_v[b]) )// Removed by Jun-Hai Yong 2002-11-20
	return a;

	//    return -1;	// 非法值 // Removed by Jun-Hai Yong 2002-11-20
}

int C_NonuniformBSurf::GetUKnotMulti(double value)
{
	int i, j, multiple = 0;
	for (i = 0; i < m_number_u + m_order_u; i++)
		if (fabs(m_knot_u[i] - value) < TOLER)
			break;
	if (i == m_number_u + m_order_u)
		return 0;
	multiple = 1;
	for (j = i + 1; j < m_number_u + m_order_u; j++)
		if (fabs(m_knot_u[j] - value) < TOLER)
			multiple++;
		else
			break;
	return multiple;
}

int C_NonuniformBSurf::GetVKnotMulti(double value)
{
	int i, j, multiple;
	for (i = 0; i < m_number_v + m_order_v; i++)
		if (fabs(m_knot_v[i] - value) < TOLER)
			break;
	if (i == m_number_v + m_order_v)
		return 0;
	multiple = 1;
	for (j = i + 1; j < m_number_v + m_order_v; j++)
		if (fabs(m_knot_v[j] - value) < TOLER)
			multiple++;
		else
			break;
	return multiple;
}

Point3D C_NonuniformBSurf::ComputPosition(double u, double v)
{
	int			i, k, r, ju, jv, mu, mv;
	double		fz1, fz2, fm;
	//---------------------------------------------------------------------
	//Yupiqiang 2004/04/07
	//Vector3D	C[4], D[4];
	Vector3D* C, * D;
	C = new Vector3D[m_order_v];
	D = new Vector3D[m_order_u];
	//----------------------------------------------------------------------

	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	for (k = mu; k <= ju; k++)
	{
		for (i = mv; i <= jv; i++)
		{
			C[i - mv] = GetControlPoint(k, i) - ZeroP3D;
		}
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = mv; i <= jv - r; i++)
			{
				fz1 = m_knot_v[i + m_order_v] - v;
				fz2 = v - m_knot_v[i + r];
				fm = m_knot_v[i + m_order_v] - m_knot_v[i + r];
				C[i - mv] = (C[i - mv] * fz1 + C[i - mv + 1] * fz2) / fm;
			}
		}
		D[k - mu] = C[0];
	}

	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = mu; i <= ju - r; i++)
		{
			fz1 = m_knot_u[i + m_order_u] - u;
			fz2 = u - m_knot_u[i + r];
			fm = m_knot_u[i + m_order_u] - m_knot_u[i + r];
			D[i - mu] = (D[i - mu] * fz1 + D[i - mu + 1] * fz2) / fm;
		}
	}

	//----------------------------------------------------------
	//Yu Piqiang 2004-04-08
	//return ZeroP3D + D[0];
	Vector3D vec = D[0];
	delete[]C;
	delete[]D;
	return vec;
	//----------------------------------------------------------
}

Vector3D C_NonuniformBSurf::ComputUDerivative1(double u, double v)
{
	int			i, k, r, ju, jv, mu, mv;
	double		fz1, fz2, fm;
	//---------------------------------------------------------------------
	//Yupiqiang 2004/04/08
	//Vector3D	C[4], D[4];
	Vector3D* C, * D;
	C = new Vector3D[m_order_v];
	D = new Vector3D[m_order_u];
	//----------------------------------------------------------------------

	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	//求P,W
	for (k = mu; k <= ju; k++)
	{
		for (i = mv; i <= jv; i++)
			C[i - mv] = GetControlPoint(k, i) - ZeroP3D;
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = mv; i <= jv - r; i++)
			{
				fz1 = m_knot_v[i + m_order_v] - v;
				fz2 = v - m_knot_v[i + r];
				fm = m_knot_v[i + m_order_v] - m_knot_v[i + r];
				C[i - mv] = (C[i - mv] * fz1 + C[i - mv + 1] * fz2) / fm;
			}
		}
		D[k - mu] = C[0];
	}

	//求P',W'
	int order = m_order_u - 1;
	for (i = ju; i > mu; i--)
	{
		fm = order / (m_knot_u[i + order] - m_knot_u[i]);
		D[i - mu] = (D[i - mu] - D[i - mu - 1]) * fm;
	}
	for (r = 1; r <= order - 1; r++)
	{
		for (i = mu + 1; i <= ju - r; i++)
		{
			fz1 = m_knot_u[i + order] - u;
			fz2 = u - m_knot_u[i + r];
			fm = m_knot_u[i + order] - m_knot_u[i + r];
			D[i - mu] = (D[i - mu] * fz1 + D[i - mu + 1] * fz2) / fm;
		}
	}

	//--------------------------------------------------------
	//return D[1];
	Vector3D vec = D[1];
	delete[]C;
	delete[]D;
	return vec;
	//----------------------------------------------------------
}


Vector3D C_NonuniformBSurf::ComputVDerivative1(double u, double v)
{
	int			i, k, r, ju, jv, mu, mv;
	double		fz1, fz2, fm;
	//---------------------------------------------------------------------
	//Yupiqiang 2003/04/08
	//Vector3D	C[4], D[4];
	Vector3D* C, * D;
	C = new Vector3D[m_order_u];
	D = new Vector3D[m_order_v];
	//----------------------------------------------------------------------


	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	//求P,W
	for (k = mv; k <= jv; k++)
	{
		for (i = mu; i <= ju; i++)
			C[i - mu] = GetControlPoint(i, k) - ZeroP3D;
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = mu; i <= ju - r; i++)
			{
				fz1 = m_knot_u[i + m_order_u] - u;
				fz2 = u - m_knot_u[i + r];
				fm = m_knot_u[i + m_order_u] - m_knot_u[i + r];
				C[i - mu] = (C[i - mu] * fz1 + C[i - mu + 1] * fz2) / fm;
			}
		}
		D[k - mv] = C[0];
	}

	//求P',W'
	int order = m_order_v - 1;
	for (i = jv; i > mv; i--)
	{
		fm = order / (m_knot_v[i + order] - m_knot_v[i]);
		D[i - mv] = (D[i - mv] - D[i - mv - 1]) * fm;
	}
	for (r = 1; r <= order - 1; r++)
	{
		for (i = mv + 1; i <= jv - r; i++)
		{
			fz1 = m_knot_v[i + order] - v;
			fz2 = v - m_knot_v[i + r];
			fm = m_knot_v[i + order] - m_knot_v[i + r];
			D[i - mv] = (D[i - mv] * fz1 + D[i - mv + 1] * fz2) / fm;
		}
	}

	//--------------------------------------------------------
	//return D[1];
	Vector3D vec = D[1];
	delete[]C;
	delete[]D;
	return vec;
	//----------------------------------------------------------
}

void C_NonuniformBSurf::ComputAll(double u, double v, Point3D& position, Vector3D& xvector, Vector3D& yvector)
{
	int i, j, k, r;
	Vector3D* C, * D, * E;
	C = new Vector3D[m_number_v];
	D = new Vector3D[m_number_u];
	E = new Vector3D[m_number_u];

	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			C[i] = GetControlPoint(k, i) - ZeroP3D;
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				C[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		D[k] = C[j - m_order_v + 1];
	}
	delete C;

	j = GetUIntervalIndex(u);
	for (i = 0; i <= m_number_u - 1; i++)
		E[i] = D[i];
	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = j - m_order_u + 1; i <= j - r; i++)
			E[i] = E[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
			E[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
	}
	position = ZeroP3D + E[j - m_order_u + 1];


	int order, number;
	order = m_order_u - 1;
	number = m_number_u;

	for (i = 1; i <= number - 1; i++)
		E[i] = (D[i] - D[i - 1]) * order / (m_knot_u[i + order] - m_knot_u[i]);
	delete D;
	for (r = 1; r <= order - 1; r++)
	{
		for (i = j - order + 1; i <= j - r; i++)
			E[i] = E[i] * (m_knot_u[i + order] - u) / (m_knot_u[i + order] - m_knot_u[i + r]) +
			E[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + order] - m_knot_u[i + r]);
	}
	xvector = E[j - order + 1];
	//    xvector.SetUnit();
	delete 	E;
	yvector = ComputVDerivative1(u, v);
}

Vector3D C_NonuniformBSurf::ComputUDerivative2(double u, double v)
{
	int i, k;
	C_NonuniformBCurve3D vcurve(m_number_v, m_order_v), ucurve(m_number_u, m_order_u);
	for (i = 0; i < m_number_v + m_order_v; i++)
		vcurve.SetKnot(i, m_knot_v[i]);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			vcurve.SetControlPoint(i, GetControlPoint(k, i));
		ucurve.SetControlPoint(k, vcurve.ComputPosition(v));
	}
	for (i = 0; i < m_number_u + m_order_u; i++)
		ucurve.SetKnot(i, m_knot_u[i]);
	return ucurve.ComputDerivative2(u);
}


Vector3D C_NonuniformBSurf::ComputVDerivative2(double u, double v)
{
	int i, k;
	C_NonuniformBCurve3D vcurve(m_number_v, m_order_v), ucurve(m_number_u, m_order_u);
	for (i = 0; i < m_number_v + m_order_v; i++)
		vcurve.SetKnot(i, m_knot_v[i]);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			vcurve.SetControlPoint(i, GetControlPoint(k, i));
		ucurve.SetControlPoint(k, vcurve.ComputDerivative2(v));
	}
	for (i = 0; i < m_number_u + m_order_u; i++)
		ucurve.SetKnot(i, m_knot_u[i]);
	return ucurve.ComputPosition(u) - ZeroP3D;
}

Vector3D C_NonuniformBSurf::ComputUVDerivative2(double u, double v)
{
	int i, k;
	C_NonuniformBCurve3D vcurve(m_number_v, m_order_v), ucurve(m_number_u, m_order_u);
	for (i = 0; i < m_number_v + m_order_v; i++)
		vcurve.SetKnot(i, m_knot_v[i]);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			vcurve.SetControlPoint(i, GetControlPoint(k, i));
		ucurve.SetControlPoint(k, ZeroP3D + vcurve.ComputDerivative1(v));
	}
	for (i = 0; i < m_number_u + m_order_u; i++)
		ucurve.SetKnot(i, m_knot_u[i]);
	return ucurve.ComputDerivative1(u);
}

C_Curve3D* C_NonuniformBSurf::GetUCurve(double v)
{
	int i, j, k, r;
	Vector3D* C;
	C_NonuniformBCurve3D* curv;
	curv = new C_NonuniformBCurve3D(m_number_u, m_order_u);

	C = new Vector3D[m_number_v];

	for (i = 0; i <= m_number_u + m_order_u - 1; i++)
		curv->SetKnot(i, m_knot_u[i]);

	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			C[i] = GetControlPoint(k, i) - ZeroP3D;
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				C[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		curv->SetControlPoint(k, ZeroP3D + C[j - m_order_v + 1]);
	}
	delete C;
	return curv;
}

C_Curve3D* C_NonuniformBSurf::GetVCurve(double u)
{
	int i, j, k, r;
	Vector3D* C;
	C_NonuniformBCurve3D* curv;
	curv = new C_NonuniformBCurve3D(m_number_v, m_order_v);

	C = new Vector3D[m_number_u];

	for (i = 0; i <= m_number_v + m_order_v - 1; i++)
		curv->SetKnot(i, m_knot_v[i]);

	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_v; k++)
	{
		for (i = 0; i < m_number_u; i++)
			C[i] = GetControlPoint(i, k) - ZeroP3D;
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = j - m_order_u + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
				C[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
		}
		curv->SetControlPoint(k, ZeroP3D + C[j - m_order_u + 1]);
	}
	delete C;
	return curv;
}
CS_BSplineSurf BssurfCopy(CS_BSplineSurf surf)
{
	int i, j, orderu, numberu, orderv, numberv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	numberu = surf.nu;
	numberv = surf.nv;

	CS_BSplineSurf  Rsurf;
	Rsurf.knotu = new double[numberu + orderu];
	Rsurf.knotv = new double[numberv + orderv];
	Rsurf.ctrl = new Point3D[numberu * numberv];
	Rsurf.nu = numberu;
	Rsurf.nv = numberv;
	Rsurf.orderu = orderu;
	Rsurf.orderv = orderv;

	for (i = 0; i <= numberu + orderu - 1; i++)
		Rsurf.knotu[i] = surf.knotu[i];
	for (i = 0; i <= numberv + orderv - 1; i++)
		Rsurf.knotv[i] = surf.knotv[i];
	for (j = 0; j < numberv * numberu; j++)
		Rsurf.ctrl[j] = surf.ctrl[j];
	return Rsurf;
}
void BscurvInsertMultiKnot(CS_BSplineCurve& curve, int multiplicity, double value)
{
	int order, number;
	number = curve.n;
	order = curve.order;

	int      i, j, k;
	double* newknot, pnewknot[100];
	Vector3D* newvect, * vect, pnewvect[100], pvect[100];
	int keynumber = number + multiplicity;

	// 申请空间
	if (keynumber + order > 100)
		newknot = new double[keynumber + order];
	else
		newknot = pnewknot;
	if (keynumber > 100)
	{
		vect = new Vector3D[keynumber];
		newvect = new Vector3D[keynumber];
	}
	else
	{
		vect = pvect;
		newvect = pnewvect;
	}
	// 赋初值
	for (i = 0; i < number; i++)
		vect[i] = curve.ctrl[i] - ZeroP3D;
	j = BscurvIntervalIndex(curve, value);    // Value 在第j个节点区间
	// 计算新的节点向量
	for (i = 0; i < number + order + multiplicity; i++)
	{
		if (i <= j)
			newknot[i] = curve.knot[i];
		else if (i <= j + multiplicity)
			newknot[i] = value;
		else
			newknot[i] = curve.knot[i - multiplicity];
	}
	// 插节点  
	double alpha;
	for (k = 1; k <= multiplicity; k++)
	{
		for (i = 0; i < number + k; i++)
		{
			if (i <= j - order + k)
				newvect[i] = vect[i];
			else if (i >= j + 1)
				newvect[i] = vect[i - 1];
			else
			{
				alpha = (value - curve.knot[i]) / (curve.knot[i + order - k] - curve.knot[i]);
				newvect[i] = vect[i - 1] * (1 - alpha) + vect[i] * alpha;
			}
		}
		for (i = 0; i < number + k; i++)
			vect[i] = newvect[i];
	}
	//构造新的对象
	number += multiplicity;
	curve.n = number;
	curve.order = order;
	delete  curve.knot;
	curve.knot = new double[number + order];
	delete  curve.ctrl;
	curve.ctrl = new Point3D[number];
	for (i = 0; i < number + order; i++)
		curve.knot[i] = newknot[i];
	for (i = 0; i < number; i++)
		curve.ctrl[i] = ZeroP3D + newvect[i];

	if (keynumber + order > 100)
		delete newknot;
	if (keynumber > 100)
	{
		delete vect;
		delete newvect;
	}
}

void BssurfInsertMultiUKnot(CS_BSplineSurf& surf, int multiplicity, double value)
{
	int numberu, numberv, orderu, orderv;
	CS_BSplineSurf  Csurf = BssurfCopy(surf);

	delete[] surf.ctrl;
	delete[] surf.knotu;  // not delete knotv
	surf.nu = surf.nu + multiplicity;
	numberu = surf.nu;
	numberv = surf.nv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	surf.knotu = new double[numberu + orderu];
	surf.ctrl = new Point3D[numberu * numberv];

	int i, j;
	CS_BSplineCurve curve;
	curve.knot = new double[Csurf.nu + Csurf.orderu];
	curve.ctrl = new Point3D[Csurf.nu];
	for (j = 0; j < Csurf.nv; j++)
	{
		curve.n = Csurf.nu;
		curve.order = Csurf.orderu;
		for (i = 0; i <= curve.n + curve.order - 1; i++)
			curve.knot[i] = Csurf.knotu[i];
		for (i = 0; i < curve.n; i++)
			curve.ctrl[i] = Csurf.ctrl[IndexMap(Csurf.nu, Csurf.nv, i, j)];
		BscurvInsertMultiKnot(curve, multiplicity, value);

		if (j == 0)
			for (i = 0; i <= curve.n + curve.order - 1; i++)
				surf.knotu[i] = curve.knot[i];
		for (i = 0; i < curve.n; i++)
			surf.ctrl[IndexMap(surf.nu, surf.nv, i, j)] = curve.ctrl[i];
	}
	delete[] curve.ctrl;
	delete[] curve.knot;
	delete[] Csurf.ctrl;
	delete[] Csurf.knotu;
	delete[] Csurf.knotv;
}
void C_NonuniformBSurf::InsertMultiUKnot(int multiplicity, double value)
{
	int k;
	CS_BSplineSurf  surf;
	surf.nu = m_number_u;
	surf.nv = m_number_v;
	surf.orderu = m_order_u;
	surf.orderv = m_order_v;
	surf.knotu = new double[surf.nu + surf.orderu];
	surf.knotv = new double[surf.nv + surf.orderv];
	surf.ctrl = new Point3D[surf.nu * surf.nv];
	for (k = 0; k <= surf.nu + surf.orderu - 1; k++)
		surf.knotu[k] = m_knot_u[k];
	for (k = 0; k <= surf.nv + surf.orderv - 1; k++)
		surf.knotv[k] = m_knot_v[k];
	for (k = 0; k < surf.nu * surf.nv; k++)
		surf.ctrl[k] = m_control[k];
	// 调用mathlib中的函数
	BssurfInsertMultiUKnot(surf, multiplicity, value);
	SetNumberOrder(surf.nu, surf.nv, surf.orderu, surf.orderv);
	for (k = 0; k <= m_number_u + m_order_u - 1; k++)
		m_knot_u[k] = surf.knotu[k];
	for (k = 0; k <= m_number_v + m_order_v - 1; k++)
		m_knot_v[k] = surf.knotv[k];
	for (k = 0; k < m_number_u * m_number_v; k++)
		m_control[k] = surf.ctrl[k];
	delete surf.knotu;
	delete surf.knotv;
	delete surf.ctrl;
}
void BssurfInsertMultiVKnot(CS_BSplineSurf& surf, int multiplicity, double value)
{
	int numberu, numberv, orderu, orderv;
	CS_BSplineSurf  Csurf = BssurfCopy(surf);

	delete[] surf.ctrl;
	delete[] surf.knotv;  // not delete knotu
	surf.nv = surf.nv + multiplicity;
	numberu = surf.nu;
	numberv = surf.nv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	surf.knotv = new double[numberv + orderv];
	surf.ctrl = new Point3D[numberu * numberv];

	int i, j;
	CS_BSplineCurve curve;
	curve.knot = new double[Csurf.nv + Csurf.orderv];
	curve.ctrl = new Point3D[Csurf.nv];
	for (i = 0; i < Csurf.nu; i++)
	{
		curve.n = Csurf.nv;
		curve.order = Csurf.orderv;
		for (j = 0; j <= curve.n + curve.order - 1; j++)
			curve.knot[j] = Csurf.knotv[j];
		for (j = 0; j < curve.n; j++)
			curve.ctrl[j] = Csurf.ctrl[IndexMap(Csurf.nu, Csurf.nv, i, j)];
		BscurvInsertMultiKnot(curve, multiplicity, value);

		if (i == 0)
			for (j = 0; j <= curve.n + curve.order - 1; j++)
				surf.knotv[j] = curve.knot[j];
		for (j = 0; j < curve.n; j++)
			surf.ctrl[IndexMap(surf.nu, surf.nv, i, j)] = curve.ctrl[j];
	}
	delete[] curve.ctrl;
	delete[] curve.knot;
	delete[] Csurf.ctrl;
	delete[] Csurf.knotu;
	delete[] Csurf.knotv;
}
void C_NonuniformBSurf::InsertMultiVKnot(int multiplicity, double value)
{
	int k;
	CS_BSplineSurf  surf;
	surf.nu = m_number_u;
	surf.nv = m_number_v;
	surf.orderu = m_order_u;
	surf.orderv = m_order_v;
	surf.knotu = new double[surf.nu + surf.orderu];
	surf.knotv = new double[surf.nv + surf.orderv];
	surf.ctrl = new Point3D[surf.nu * surf.nv];
	for (k = 0; k <= surf.nu + surf.orderu - 1; k++)
		surf.knotu[k] = m_knot_u[k];
	for (k = 0; k <= surf.nv + surf.orderv - 1; k++)
		surf.knotv[k] = m_knot_v[k];
	for (k = 0; k < surf.nu * surf.nv; k++)
		surf.ctrl[k] = m_control[k];
	// 调用mathlib中的函数
	BssurfInsertMultiVKnot(surf, multiplicity, value);
	SetNumberOrder(surf.nu, surf.nv, surf.orderu, surf.orderv);
	for (k = 0; k <= m_number_u + m_order_u - 1; k++)
		m_knot_u[k] = surf.knotu[k];
	for (k = 0; k <= m_number_v + m_order_v - 1; k++)
		m_knot_v[k] = surf.knotv[k];
	for (k = 0; k < m_number_u * m_number_v; k++)
		m_control[k] = surf.ctrl[k];
	delete surf.knotu;
	delete surf.knotv;
	delete surf.ctrl;
}



Point3D C_NonuniformBSurf::GetParaPoint(double u, double v)
// 求曲面参数域中一点对应的空间点坐标值
{
	u = m_knot_u[m_order_u - 1] + (m_knot_u[m_number_u] - m_knot_u[m_order_u - 1]) * u;
	v = m_knot_v[m_order_v - 1] + (m_knot_v[m_number_v] - m_knot_v[m_order_v - 1]) * v;
	return ComputPosition(u, v);
}

Vector3D C_NonuniformBSurf::GetParaNormal(double u, double v)
// 求曲面参数域中一点对应的空间点的法向
{
	Vector3D vec1, vec2, vect;
	vec1 = ComputUDerivative1(u, v);
	vec2 = ComputVDerivative1(u, v);
	if (Length(vec1) < TOLER)
	{
		if (v < 0.5)
			vec1 = ComputUDerivative1(u, v + 0.01);
		else
			vec1 = ComputUDerivative1(u, v - 0.01);
	}
	if (Length(vec2) < TOLER)
	{
		if (u < 0.5)
			vec2 = ComputVDerivative1(u + 0.01, v);
		else
			vec2 = ComputVDerivative1(u - 0.01, v);
	}
	vect = vec1 ^ vec2;
	// vect.SetUnit();
	return vect;
}

Vector3D C_NonuniformBSurf::GetParaNormal(double u, double v, BOOL IsUnit)
// 求曲面参数域中一点对应的空间点的法向
{
	Vector3D vect = GetParaNormal(u, v);
	if (IsUnit == TRUE)
		vect.SetUnit();
	return vect;
}



C_Surf3D* C_NonuniformBSurf::Copy(void)
{
	return new C_NonuniformBSurf(m_number_u, m_number_v, m_order_u, m_order_v, m_knot_u, m_knot_v, m_control);
}









void C_NonuniformBSurf::ComputDerivative2All(double u, double v, Point3D& position, Vector3D& Uvector, Vector3D& Vvector, Vector3D& U2vector, Vector3D& V2vector, Vector3D& UVvector)
{
	int i, k;
	Point3D p;
	Vector3D p1p, p2p;
	C_NonuniformBCurve3D vcurve(m_number_v, m_order_v);
	C_NonuniformBCurve3D ucurve(m_number_u, m_order_u), u1curve(m_number_u, m_order_u), u2curve(m_number_u, m_order_u);
	for (i = 0; i < m_number_v + m_order_v; i++)
		vcurve.SetKnot(i, m_knot_v[i]);

#ifdef  _DEBUG
	TRACE("\nDebug for NonuniformSurf\n");
#endif

	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			vcurve.SetControlPoint(i, GetControlPoint(k, i));
		vcurve.ComputDerivative2All(v, p, p1p, p2p);
		ucurve.SetControlPoint(k, p);
		u1curve.SetControlPoint(k, p1p);
#ifdef  _DEBUG
		TRACE("%d:  %lf,%lf,%lf\n", k, p1p.x, p1p.y, p1p.z);
#endif
		u2curve.SetControlPoint(k, p2p);

	}
	for (i = 0; i < m_number_u + m_order_u; i++)
	{
		ucurve.SetKnot(i, m_knot_u[i]);
		u1curve.SetKnot(i, m_knot_u[i]);
		u2curve.SetKnot(i, m_knot_u[i]);
	}
	ucurve.ComputDerivative2All(u, position, Uvector, U2vector);
	Vvector = u1curve.ComputPosition(u) - ZeroP3D;
	V2vector = u2curve.ComputPosition(u) - ZeroP3D;
	UVvector = u1curve.ComputDerivative1(u);
}


/////////////////////////////////////////////////////////////////////////////
// C_NurbsSurf
C_NurbsSurf::C_NurbsSurf(void)
{
	m_weight = NULL;
}

C_NurbsSurf::C_NurbsSurf(int number_u, int number_v, int order_u, int order_v) :
	C_NonuniformBSurf(number_u, number_v, order_u, order_v)
{
	m_weight = new double[number_u * number_v];
}

C_NurbsSurf::C_NurbsSurf(int number_u, int number_v, int order_u, int order_v,
	double* knot_u, double* knot_v, Point3D* control, double* weight) :
	C_NonuniformBSurf(number_u, number_v, order_u, order_v,
		knot_u, knot_v, control)
{
	m_weight = new double[number_u * number_v];
	for (int i = 0; i < number_u * number_v; i++)
		m_weight[i] = weight[i];
}

C_NurbsSurf::~C_NurbsSurf(void)
{
	delete m_weight;
}

// 用于数据存取和显示的成员函数
void  C_NurbsSurf::SetNumberOrder(int number_u, int number_v, int order_u, int order_v)
{
	C_NonuniformBSurf::SetNumberOrder(number_u, number_v, order_u, order_v);
	delete m_weight;
	m_weight = new double[number_u * number_v];
}

double C_NurbsSurf::GetWeight(int i, int j)
{
	return m_weight[C_ParaSurf::IndexMap(i, j)];
}

void C_NurbsSurf::SetWeight(int i, int j, double weight)
{
	m_weight[C_ParaSurf::IndexMap(i, j)] = weight;
}

int C_NurbsSurf::PrintInformation(void)
{
	return 0;
}

void C_NurbsSurf::ComputeControlPointBoundingBox(Point3D& minpoint, Point3D& maxpoint)
{
	minpoint.x = minpoint.y = minpoint.z = 1e10;
	maxpoint.x = maxpoint.y = maxpoint.z = -1e10;
	Point3D point;
	for (int i = 0; i < GetCtrlNumberU(); i++)
		for (int j = 0; j < GetCtrlNumberV(); j++)
		{
			point = GetControlPoint(i, j);
			if (point.x < minpoint.x)
				minpoint.x = point.x;
			if (point.y < minpoint.y)
				minpoint.y = point.y;
			if (point.z < minpoint.z)
				minpoint.z = point.z;

			if (point.x > maxpoint.x)
				maxpoint.x = point.x;
			if (point.y > maxpoint.y)
				maxpoint.y = point.y;
			if (point.z > maxpoint.z)
				maxpoint.z = point.z;
		}
}

Point3D C_NurbsSurf::ComputPosition(double u, double v)
{
	int			i, k, r, ju, jv, mu, mv;
	//------------------------------------------------------------------
	//Yupiqiang 2004/04/08
	double		/*E[4], F[4],*/* E, * F, fz1, fz2, fm;
	Vector3D* C, * D; //C[4], D[4];
	E = new double[m_order_v];
	C = new Vector3D[m_order_v];
	F = new double[m_order_u];
	D = new Vector3D[m_order_u];
	//-----------------------------------------------------------------


	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	//ju = jv = 3;
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	for (k = mu; k <= ju; k++)
	{
		for (i = mv; i <= jv; i++)
		{
			E[i - mv] = GetWeight(k, i);
			C[i - mv] = (GetControlPoint(k, i) - ZeroP3D) * E[i - mv];
		}
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = mv; i <= jv - r; i++)
			{
				fz1 = m_knot_v[i + m_order_v] - v;
				fz2 = v - m_knot_v[i + r];
				fm = m_knot_v[i + m_order_v] - m_knot_v[i + r];
				E[i - mv] = (E[i - mv] * fz1 + E[i - mv + 1] * fz2) / fm;
				C[i - mv] = (C[i - mv] * fz1 + C[i - mv + 1] * fz2) / fm;
			}
		}
		F[k - mu] = E[0];
		D[k - mu] = C[0];
	}

	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = mu; i <= ju - r; i++)
		{
			fz1 = m_knot_u[i + m_order_u] - u;
			fz2 = u - m_knot_u[i + r];
			fm = m_knot_u[i + m_order_u] - m_knot_u[i + r];
			F[i - mu] = (F[i - mu] * fz1 + F[i - mu + 1] * fz2) / fm;
			D[i - mu] = (D[i - mu] * fz1 + D[i - mu + 1] * fz2) / fm;
		}
	}

	//--------------------------------------------------------------------
	//return ZeroP3D + D[0]/F[0];
	Vector3D vec = D[0] / F[0];
	delete[]E;
	delete[]C;
	delete[]F;
	delete[]D;

	return ZeroP3D + vec;
	//---------------------------------------------------------------------
}

Vector3D C_NurbsSurf::ComputUDerivative1(double u, double v)
// 导数 = (P'*W - P*W')/(W*W)
//      = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
{
	int			i, k, r, ju, jv, mu, mv;
	//------------------------------------------------------------------
	//Yupiqiang 2003/04/08
	double		/*E[4], F[4], H[4],*/* E, * F, * H, fz1, fz2, fm;
	Vector3D* C, * D, * G; //C[4], D[4], G[4];
	int size = MAX(m_order_v, m_order_u);
	E = new double[size];
	C = new Vector3D[size];
	F = new double[size];
	D = new Vector3D[size];
	H = new double[size];
	G = new Vector3D[size];
	//---------------------------------------------------------------------

	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	//求P,W
	for (k = mu; k <= ju; k++)
	{
		for (i = mv; i <= jv; i++)
		{
			E[i - mv] = GetWeight(k, i);
			C[i - mv] = (GetControlPoint(k, i) - ZeroP3D) * E[i - mv];
		}
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = mv; i <= jv - r; i++)
			{
				fz1 = m_knot_v[i + m_order_v] - v;
				fz2 = v - m_knot_v[i + r];
				fm = m_knot_v[i + m_order_v] - m_knot_v[i + r];
				E[i - mv] = (E[i - mv] * fz1 + E[i - mv + 1] * fz2) / fm;
				C[i - mv] = (C[i - mv] * fz1 + C[i - mv + 1] * fz2) / fm;
			}
		}
		H[k - mu] = F[k - mu] = E[0];
		G[k - mu] = D[k - mu] = C[0];
	}

	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = mu; i <= ju - r; i++)
		{
			fz1 = m_knot_u[i + m_order_u] - u;
			fz2 = u - m_knot_u[i + r];
			fm = m_knot_u[i + m_order_u] - m_knot_u[i + r];
			F[i - mu] = (F[i - mu] * fz1 + F[i - mu + 1] * fz2) / fm;
			D[i - mu] = (D[i - mu] * fz1 + D[i - mu + 1] * fz2) / fm;
		}
	}

	//求P',W'
	int order = m_order_u - 1;
	for (i = ju; i > mu; i--)
	{
		fm = order / (m_knot_u[i + order] - m_knot_u[i]);
		H[i - mu] = (H[i - mu] - H[i - mu - 1]) * fm;
		G[i - mu] = (G[i - mu] - G[i - mu - 1]) * fm;
	}
	for (r = 1; r <= order - 1; r++)
	{
		for (i = mu + 1; i <= ju - r; i++)
		{
			fz1 = m_knot_u[i + order] - u;
			fz2 = u - m_knot_u[i + r];
			fm = m_knot_u[i + order] - m_knot_u[i + r];
			H[i - mu] = (H[i - mu] * fz1 + H[i - mu + 1] * fz2) / fm;
			G[i - mu] = (G[i - mu] * fz1 + G[i - mu + 1] * fz2) / fm;
		}
	}

	//--------------------------------------------------------------------------
	//return (G[1]*F[0] - D[0]*H[1])/(F[0]*F[0]);
	Vector3D vec = (G[1] * F[0] - D[0] * H[1]) / (F[0] * F[0]);
	delete[]E;
	delete[]C;
	delete[]F;
	delete[]D;
	delete[]H;
	delete[]G;
	return vec;
	//----------------------------------------------------------------------------
}
C_NurbsSurf* C_NurbsSurf::GetSubSurface(double minu, double maxu, double minv, double maxv, int flag)
{
	//Get the sub surface
	if (flag == 0)
	{
		int mul = GetUKnotMulti(minu);
		if (mul < 4)
			InsertMultiUKnot(4 - mul, minu);

		mul = GetUKnotMulti(maxu);
		if (mul < 4)
			InsertMultiUKnot(4 - mul, maxu);

		mul = GetVKnotMulti(minv);
		if (mul < 4)
			InsertMultiVKnot(4 - mul, minv);

		mul = GetVKnotMulti(maxv);
		if (mul < 4)
			InsertMultiVKnot(4 - mul, maxv);
	}

	int uminindex, vminindex, umaxindex, vmaxindex;
	uminindex = vminindex = umaxindex = vmaxindex = 0;

	TRACE("\n");
	for (int i = 0; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		TRACE("%f ", GetUKnot(i));
	}
	TRACE("\n");
	for (int i = 0; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		TRACE("%f ", GetVKnot(i));
	}

	for (int i = 0; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		double u = GetUKnot(i);
		if (u < minu)
			uminindex++;

		if (u < maxu)
			umaxindex++;
	}
	for (int i = 0; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		double v = GetVKnot(i);
		if (v < minv)
			vminindex++;

		if (v < maxv)
			vmaxindex++;
	}
	int uorder = 4;
	int vorder = 4;

	int uctrlnum = umaxindex - uminindex;
	int vctrlnum = vmaxindex - vminindex;

	double* uknot = new double[uctrlnum + uorder];
	double* vknot = new double[vctrlnum + vorder];

	for (int i = 0; i < uctrlnum + uorder; i++)
		uknot[i] = (GetUKnot(i + uminindex) - GetUKnot(uminindex)) / (GetUKnot(umaxindex) - GetUKnot(uminindex));

	//for(int i = 0; i < uctrlnum + uorder; i++)
	//	TRACE("%f ",uknot[i]);
	//TRACE("\n");

	for (int i = 0; i < vctrlnum + vorder; i++)
		vknot[i] = (GetVKnot(i + vminindex) - GetVKnot(vminindex)) / (GetVKnot(vmaxindex) - GetVKnot(vminindex));

	//for(int i = 0; i < vctrlnum + vorder; i++)
	//	TRACE("%f ",vknot[i]);
	//TRACE("\n");

	C_NurbsSurf* newsurf = new C_NurbsSurf(uctrlnum, vctrlnum, uorder, vorder);
	for (int i = 0; i < uctrlnum + uorder; i++)
		newsurf->SetUKnot(i, uknot[i]);

	for (int i = 0; i < vctrlnum + vorder; i++)
		newsurf->SetVKnot(i, vknot[i]);

	for (int i = 0; i < uctrlnum; i++)
		for (int j = 0; j < vctrlnum; j++)
		{
			newsurf->SetControlPoint(i, j, GetControlPoint(uminindex + i, vminindex + j));
			newsurf->SetWeight(i, j, GetWeight(uminindex + i, vminindex + j));
		}
	return newsurf;
}
double C_NurbsSurf::ComputeUBasis(int i, int p, double u)
{
	double ui = GetUKnot(i);
	double uia1 = GetUKnot(i + 1);
	if (p == 0)
	{
		if (u >= ui && u <= uia1)
			return 1;
		else
			return 0;
	}
	else
	{
		double denorm1 = (GetUKnot(i + p) - ui);
		double denorm2 = (GetUKnot(i + p + 1) - uia1);

		double term1, term2;
		if (fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}
		else
		{
			term1 = (u - ui) * ComputeUBasis(i, p - 1, u) / denorm1;
		}
		if (fabs(denorm2) < 1e-8)
		{
			term2 = 0;
		}
		else
		{
			term2 = (GetUKnot(i + p + 1) - u) * ComputeUBasis(i + 1, p - 1, u) / denorm2;
		}

		double u1 = term1 + term2;
		return u1;
	}
}
double C_NurbsSurf::ComputeUBasis(int i, double u)
{
	return ComputeUBasis(i, GetOrderU() - 1, u);
}
double C_NurbsSurf::ComputeUBasis_PartialU(int i, int p, double u)
{
	double ui_p = GetUKnot(i + p);
	double ui = GetUKnot(i);
	double ui_p_1 = GetUKnot(i + p + 1);
	double ui_1 = GetUKnot(i + 1);
	double term1 = 0;
	double term2 = 0;
	if (fabs(ui_p - ui) > 1e-5)
		term1 = p * ComputeUBasis(i, p - 1, u) / (ui_p - ui);
	if (fabs(ui_p_1 - ui_1) > 1e-5)
		term2 = p * ComputeUBasis(i + 1, p - 1, u) / (ui_p_1 - ui_1);
	double val = term1 - term2;
	return val;
}
double C_NurbsSurf::ComputeUBasis_PartialU(int i, double u)
{
	int p = GetOrderU() - 1;
	return ComputeUBasis_PartialU(i, p, u);
}
double C_NurbsSurf::ComputeVBasis_PartialV(int i, int p, double v)
{

	double vi_p = GetVKnot(i + p);
	double vi = GetVKnot(i);
	double vi_p_1 = GetVKnot(i + p + 1);
	double vi_1 = GetVKnot(i + 1);
	double term1 = 0;
	double term2 = 0;
	if (fabs(vi_p - vi) > 1e-5)
		term1 = p * ComputeVBasis(i, p - 1, v) / (vi_p - vi);
	if (fabs(vi_p_1 - vi_1) > 1e-5)
		term2 = p * ComputeVBasis(i + 1, p - 1, v) / (vi_p_1 - vi_1);
	double val = term1 - term2;

	return val;

}
double C_NurbsSurf::ComputeVBasis_PartialV(int i, double v)
{
	int p = GetOrderV() - 1;
	return ComputeVBasis_PartialV(i, p, v);
}

double C_NurbsSurf::ComputeUBasis_PartialU_2(int i, double u)
{
	int p = GetOrderU() - 1;
	double ui_p = GetUKnot(i + p);
	double ui = GetUKnot(i);
	double ui_p_1 = GetUKnot(i + p + 1);
	double ui_1 = GetUKnot(i + 1);
	double term1 = 0;
	double term2 = 0;
	if (fabs(ui_p - ui) > 1e-5)
		term1 = p / (ui_p - ui) * ComputeUBasis_PartialU(i, p - 1, u);
	if (fabs(ui_p_1 - ui_1) > 1e-5)
		term2 = p / (ui_p_1 - ui_1) * ComputeUBasis_PartialU(i + 1, p - 1, u);

	double val = term1 - term2;

	return val;
}

double C_NurbsSurf::ComputeVBasis_PartialV_2(int i, double v)
{
	int p = GetOrderV() - 1;
	double vi_p = GetVKnot(i + p);
	double vi = GetVKnot(i);
	double vi_p_1 = GetVKnot(i + p + 1);
	double vi_1 = GetVKnot(i + 1);
	double term1 = 0;
	double term2 = 0;
	if (fabs(vi_p - vi) > 1e-5)
		term1 = p * ComputeVBasis(i, p - 1, v) / (vi_p - vi) * ComputeVBasis_PartialV(i, p - 1, v);
	if (fabs(vi_p_1 - vi_1) > 1e-5)
		term2 = p * ComputeVBasis(i + 1, p - 1, v) / (vi_p_1 - vi_1) * ComputeVBasis_PartialV(i + 1, p - 1, v);

	double val = term1 - term2;

	return val;
}

double C_NurbsSurf::ComputeVBasis(int i, int p, double v)
{
	double vi = GetVKnot(i);
	double via1 = GetVKnot(i + 1);
	if (p == 0)
	{
		if (v >= vi && v <= via1)
			return 1;
		else
			return 0;
	}
	else
	{
		double denorm1 = (GetVKnot(i + p) - vi);
		double denorm2 = (GetVKnot(i + p + 1) - via1);

		double term1, term2;
		if (fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}
		else
		{
			term1 = (v - vi) * ComputeVBasis(i, p - 1, v) / denorm1;
		}
		if (fabs(denorm2) < 1e-8)
		{
			term2 = 0;
		}
		else
			term2 = (GetVKnot(i + p + 1) - v) * ComputeVBasis(i + 1, p - 1, v) / denorm2;

		double v1 = term1 + term2;
		return v1;
	}
}
double C_NurbsSurf::ComputeVBasis(int i, double v)
{
	return ComputeVBasis(i, GetOrderV() - 1, v);
}
Vector3D C_NurbsSurf::ComputVDerivative1(double u, double v)
{
	int			i, k, r, ju, jv, mu, mv;
	//------------------------------------------------------------------
	//Yupiqiang 2003/04/08
	double		/*E[4], F[4], H[4],*/* E, * F, * H, fz1, fz2, fm;
	Vector3D* C, * D, * G; //C[4], D[4], G[4];
	int size = MAX(m_order_v, m_order_u);
	E = new double[size];
	C = new Vector3D[size];
	F = new double[size];
	D = new Vector3D[size];
	H = new double[size];
	G = new Vector3D[size];
	//--------------------------------------------------------------------

	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);
	mv = jv - m_order_v + 1;
	mu = ju - m_order_u + 1;

	//求P,W
	for (k = mv; k <= jv; k++)
	{
		for (i = mu; i <= ju; i++)
		{
			E[i - mu] = GetWeight(i, k);
			C[i - mu] = (GetControlPoint(i, k) - ZeroP3D) * E[i - mu];
		}
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = mu; i <= ju - r; i++)
			{
				fz1 = m_knot_u[i + m_order_u] - u;
				fz2 = u - m_knot_u[i + r];
				fm = m_knot_u[i + m_order_u] - m_knot_u[i + r];
				E[i - mu] = (E[i - mu] * fz1 + E[i - mu + 1] * fz2) / fm;
				C[i - mu] = (C[i - mu] * fz1 + C[i - mu + 1] * fz2) / fm;
			}
		}
		H[k - mv] = F[k - mv] = E[0];
		G[k - mv] = D[k - mv] = C[0];
	}

	for (r = 1; r <= m_order_v - 1; r++)
	{
		for (i = mv; i <= jv - r; i++)
		{
			fz1 = m_knot_v[i + m_order_v] - v;
			fz2 = v - m_knot_v[i + r];
			fm = m_knot_v[i + m_order_v] - m_knot_v[i + r];
			F[i - mv] = (F[i - mv] * fz1 + F[i - mv + 1] * fz2) / fm;
			D[i - mv] = (D[i - mv] * fz1 + D[i - mv + 1] * fz2) / fm;
		}
	}

	//求P',W'
	int order = m_order_v - 1;
	for (i = jv; i > mv; i--)
	{
		fm = order / (m_knot_v[i + order] - m_knot_v[i]);
		H[i - mv] = (H[i - mv] - H[i - mv - 1]) * fm;
		G[i - mv] = (G[i - mv] - G[i - mv - 1]) * fm;
	}
	for (r = 1; r <= order - 1; r++)
	{
		for (i = mv + 1; i <= jv - r; i++)
		{
			fz1 = m_knot_v[i + order] - v;
			fz2 = v - m_knot_v[i + r];
			fm = m_knot_v[i + order] - m_knot_v[i + r];
			H[i - mv] = (H[i - mv] * fz1 + H[i - mv + 1] * fz2) / fm;
			G[i - mv] = (G[i - mv] * fz1 + G[i - mv + 1] * fz2) / fm;
		}
	}

	//-------------------------------------------------------------------------
	//return (G[1]*F[0] - D[0]*H[1])/(F[0]*F[0]);
	Vector3D vec = (G[1] * F[0] - D[0] * H[1]) / (F[0] * F[0]);

	delete[]E;
	delete[]C;
	delete[]F;
	delete[]D;
	delete[]H;
	delete[]G;
	return vec;
	//--------------------------------------------------------------------------------------
}

void C_NurbsSurf::ComputAll(double u, double v, Point3D& position, Vector3D& xvector, Vector3D& yvector)
{
	// 位置 =  P/W
	int i, j, k, r;
	double    w_value, tw_value;
	Vector3D  p_vector, tp_vector;

	// P
	Vector3D* C, * D, * G;
	C = new Vector3D[m_number_v];
	D = new Vector3D[m_number_u];
	G = new Vector3D[m_number_u];

	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			C[i] = (GetControlPoint(k, i) - ZeroP3D) * GetWeight(k, i);
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				C[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		D[k] = C[j - m_order_v + 1];
	}

	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_u; k++)
		G[k] = D[k];
	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = j - m_order_u + 1; i <= j - r; i++)
			G[i] = G[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
			G[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
	}
	p_vector = G[j - m_order_u + 1];
	delete G;

	// 求W
	double* E, * F, * H;
	E = new double[m_number_v];
	F = new double[m_number_u];
	H = new double[m_number_u];
	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			E[i] = GetWeight(k, i);
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				E[i] = E[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				E[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		F[k] = E[j - m_order_v + 1];
	}

	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_u; k++)
		H[k] = F[k];
	for (r = 1; r <= m_order_u - 1; r++)
	{
		for (i = j - m_order_u + 1; i <= j - r; i++)
			H[i] = H[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
			H[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
	}
	w_value = H[j - m_order_u + 1];
	delete H;
	position = ZeroP3D + p_vector / w_value;

	//*******************
	//求xvector
	// 导数 = (P'*W - P*W')/(W*W)
	//      = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
	//求P'和W'

	//求P'
	int order = m_order_u - 1, number = m_number_u;
	j = GetUIntervalIndex(u);
	for (i = number - 1; i >= 1; i--)
		D[i] = (D[i] - D[i - 1]) * order / (m_knot_u[i + order] - m_knot_u[i]);
	for (r = 1; r <= order - 1; r++)
	{
		for (i = j - order + 1; i <= j - r; i++)
			D[i] = D[i] * (m_knot_u[i + order] - u) / (m_knot_u[i + order] - m_knot_u[i + r]) +
			D[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + order] - m_knot_u[i + r]);
	}
	tp_vector = D[j - order + 1];
	//求W'
	j = GetUIntervalIndex(u);
	for (i = number - 1; i >= 1; i--)
		F[i] = (F[i] - F[i - 1]) * order / (m_knot_u[i + order] - m_knot_u[i]);
	for (r = 1; r <= order - 1; r++)
	{
		for (i = j - order + 1; i <= j - r; i++)
			F[i] = F[i] * (m_knot_u[i + order] - u) / (m_knot_u[i + order] - m_knot_u[i + r]) +
			F[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + order] - m_knot_u[i + r]);
	}
	tw_value = F[j - order + 1];
	xvector = (tp_vector * w_value - p_vector * tw_value) / (w_value * w_value);
	xvector.SetUnit();

	//***********************
	 //求yvector
	 // 导数 = (P'*W - P*W')/(W*W)
	 //      = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
	 //求P'和W'

	 //求P'
	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_v; k++)
	{
		for (i = 0; i < m_number_u; i++)
			D[i] = (GetControlPoint(i, k) - ZeroP3D) * GetWeight(i, k);
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = j - m_order_u + 1; i <= j - r; i++)
				D[i] = D[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
				D[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
		}
		C[k] = D[j - m_order_u + 1];
	}
	delete D;
	order = m_order_v - 1;
	number = m_number_v;
	j = GetVIntervalIndex(v);
	for (i = number - 1; i >= 1; i--)
		C[i] = (C[i] - C[i - 1]) * order / (m_knot_v[i + order] - m_knot_v[i]);
	for (r = 1; r <= order - 1; r++)
	{
		for (i = j - order + 1; i <= j - r; i++)
			C[i] = C[i] * (m_knot_v[i + order] - v) / (m_knot_v[i + order] - m_knot_v[i + r]) +
			C[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + order] - m_knot_v[i + r]);
	}
	tp_vector = C[j - order + 1];
	delete C;
	//求W'
	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_v; k++)
	{
		for (i = 0; i < m_number_u; i++)
			F[i] = GetWeight(i, k);
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = j - m_order_u + 1; i <= j - r; i++)
				F[i] = F[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
				F[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
		}
		E[k] = F[j - m_order_u + 1];
	}
	delete F;
	j = GetVIntervalIndex(v);
	for (i = number - 1; i >= 1; i--)
		E[i] = (E[i] - E[i - 1]) * order / (m_knot_v[i + order] - m_knot_v[i]);
	for (r = 1; r <= order - 1; r++)
	{
		for (i = j - order + 1; i <= j - r; i++)
			E[i] = E[i] * (m_knot_v[i + order] - v) / (m_knot_v[i + order] - m_knot_v[i + r]) +
			E[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + order] - m_knot_v[i + r]);
	}
	tw_value = E[j - order + 1];
	delete E;
	yvector = (tp_vector * w_value - p_vector * tw_value) / (w_value * w_value);
	yvector.SetUnit();
}

Vector3D C_NurbsSurf::ComputUDerivative2(double u, double v)
{
	Vector3D vector(0, 0, 0);
	// vector = p''/w - pw''/w^2 - 2p'w'/w^2 + 2pw'^2/w^3
	int i, j;
	Point3D point(0, 0, 0);
	C_NonuniformBSurf Psurf(m_number_u, m_number_v, m_order_u, m_order_v),
		Wsurf(m_number_u, m_number_v, m_order_u, m_order_v);
	for (i = 0; i < m_number_u; i++)
		for (j = 0; j < m_number_v; j++)
		{
			Psurf.SetControlPoint(i, j, ZeroP3D + (GetControlPoint(i, j) - ZeroP3D) * GetWeight(i, j));
			point.x = GetWeight(i, j);
			Wsurf.SetControlPoint(i, j, point);
		}
	for (i = 0; i < m_number_u + m_order_u; i++)
	{
		Psurf.SetUKnot(i, m_knot_u[i]);
		Wsurf.SetUKnot(i, m_knot_u[i]);
	}
	for (i = 0; i < m_number_v + m_order_v; i++)
	{
		Psurf.SetVKnot(i, m_knot_v[i]);
		Wsurf.SetVKnot(i, m_knot_v[i]);
	}
	Vector3D p, pd1, pd2;
	double   w, wd1, wd2;
	p = Psurf.ComputPosition(u, v) - ZeroP3D;
	pd1 = Psurf.ComputUDerivative1(u, v);
	pd2 = Psurf.ComputUDerivative2(u, v);
	w = (Wsurf.ComputPosition(u, v)).x;
	wd1 = (Wsurf.ComputUDerivative1(u, v)).x;
	wd2 = (Wsurf.ComputUDerivative2(u, v)).x;
	vector = pd2 / w - (pd1 * wd1 * 2.0 + p * wd2) / (w * w) + p * wd1 * wd1 * 2.0 / (w * w * w);
	return vector;
}


Vector3D C_NurbsSurf::ComputVDerivative2(double u, double v)
{
	Vector3D vector(0, 0, 0);
	// vector = p''/w - pw''/w^2 + 2p'w'/w^2 - 2pw'^2/w^3
	int i, j;
	Point3D point(0, 0, 0);
	C_NonuniformBSurf Psurf(m_number_u, m_number_v, m_order_u, m_order_v),
		Wsurf(m_number_u, m_number_v, m_order_u, m_order_v);
	for (i = 0; i < m_number_u; i++)
		for (j = 0; j < m_number_v; j++)
		{
			Psurf.SetControlPoint(i, j, ZeroP3D + (GetControlPoint(i, j) - ZeroP3D) * GetWeight(i, j));
			point.x = GetWeight(i, j);
			Wsurf.SetControlPoint(i, j, point);
		}
	for (i = 0; i < m_number_u + m_order_u; i++)
	{
		Psurf.SetUKnot(i, m_knot_u[i]);
		Wsurf.SetUKnot(i, m_knot_u[i]);
	}
	for (i = 0; i < m_number_v + m_order_v; i++)
	{
		Psurf.SetVKnot(i, m_knot_v[i]);
		Wsurf.SetVKnot(i, m_knot_v[i]);
	}
	Vector3D p, pd1, pd2;
	double   w, wd1, wd2;
	p = Psurf.ComputPosition(u, v) - ZeroP3D;
	pd1 = Psurf.ComputVDerivative1(u, v);
	pd2 = Psurf.ComputVDerivative2(u, v);
	w = (Wsurf.ComputPosition(u, v)).x;
	wd1 = (Wsurf.ComputVDerivative1(u, v)).x;
	wd2 = (Wsurf.ComputVDerivative2(u, v)).x;
	vector = pd2 / w - (pd1 * wd1 * 2.0 + p * wd2) / (w * w) + p * wd1 * wd1 * 2.0 / (w * w * w);
	return vector;
}

Vector3D C_NurbsSurf::ComputUVDerivative2(double u, double v)
{
	Vector3D vector(0, 0, 0);
	// vector = puv/w + puwv/w^2 - pvwu/w^2 - pwuv/w^2 - 2puwv/w^2 + 2pwuwv/w^3
	int i, j;
	Point3D point(0, 0, 0);
	C_NonuniformBSurf Psurf(m_number_u, m_number_v, m_order_u, m_order_v),
		Wsurf(m_number_u, m_number_v, m_order_u, m_order_v);
	for (i = 0; i < m_number_u; i++)
		for (j = 0; j < m_number_v; j++)
		{
			Psurf.SetControlPoint(i, j, ZeroP3D + (GetControlPoint(i, j) - ZeroP3D) * GetWeight(i, j));
			point.x = GetWeight(i, j);
			Wsurf.SetControlPoint(i, j, point);
		}
	for (i = 0; i < m_number_u + m_order_u; i++)
	{
		Psurf.SetUKnot(i, m_knot_u[i]);
		Wsurf.SetUKnot(i, m_knot_u[i]);
	}
	for (i = 0; i < m_number_v + m_order_v; i++)
	{
		Psurf.SetVKnot(i, m_knot_v[i]);
		Wsurf.SetVKnot(i, m_knot_v[i]);
	}
	Vector3D P, Pu, Pv, Puv;
	double   W, Wu, Wv, Wuv;
	P = Psurf.ComputPosition(u, v) - ZeroP3D;
	Pu = Psurf.ComputUDerivative1(u, v);
	Pv = Psurf.ComputVDerivative1(u, v);
	Puv = Psurf.ComputUVDerivative2(u, v);
	W = (Wsurf.ComputPosition(u, v)).x;
	Wu = (Wsurf.ComputUDerivative1(u, v)).x;
	Wv = (Wsurf.ComputVDerivative1(u, v)).x;
	Wuv = (Wsurf.ComputUVDerivative2(u, v)).x;
	vector = Puv / W - (Pu * Wv + Pv * Wu + P * Wuv) / (W * W) + P * Wu * Wv * 2.0 / (W * W * W);
	return vector;
}

C_Curve3D* C_NurbsSurf::GetUCurve(double v)
{
	int i, j, k, r;
	C_NurbsCurve3D* curv;
	curv = new C_NurbsCurve3D(m_number_u, m_order_u);

	for (i = 0; i <= m_number_u + m_order_u - 1; i++)
		curv->SetKnot(i, m_knot_u[i]);

	Vector3D* C, * D;
	C = new Vector3D[m_number_v];
	D = new Vector3D[m_number_u];
	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			C[i] = (GetControlPoint(k, i) - ZeroP3D) * GetWeight(k, i);
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				C[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		D[k] = C[j - m_order_v + 1];
	}
	delete C;

	double* E, * F;
	E = new double[m_number_v];
	F = new double[m_number_u];
	j = GetVIntervalIndex(v);
	for (k = 0; k < m_number_u; k++)
	{
		for (i = 0; i < m_number_v; i++)
			E[i] = GetWeight(k, i);
		for (r = 1; r <= m_order_v - 1; r++)
		{
			for (i = j - m_order_v + 1; i <= j - r; i++)
				E[i] = E[i] * (m_knot_v[i + m_order_v] - v) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]) +
				E[i + 1] * (v - m_knot_v[i + r]) / (m_knot_v[i + m_order_v] - m_knot_v[i + r]);
		}
		F[k] = E[j - m_order_v + 1];
	}
	delete E;

	for (i = 0; i < m_number_u; i++)
	{
		curv->SetWeight(i, F[i]);
		curv->SetControlPoint(i, ZeroP3D + D[i] / F[i]);
	}
	delete D;
	delete F;
	return curv;
}

C_Curve3D* C_NurbsSurf::GetVCurve(double u)
{
	int i, j, k, r;
	C_NurbsCurve3D* curv;
	curv = new C_NurbsCurve3D(m_number_v, m_order_v);

	for (i = 0; i <= m_number_v + m_order_v - 1; i++)
		curv->SetKnot(i, m_knot_v[i]);

	Vector3D* C, * D;
	C = new Vector3D[m_number_u];
	D = new Vector3D[m_number_v];
	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_v; k++)
	{
		for (i = 0; i < m_number_u; i++)
			C[i] = (GetControlPoint(i, k) - ZeroP3D) * GetWeight(i, k);
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = j - m_order_u + 1; i <= j - r; i++)
				C[i] = C[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
				C[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
		}
		D[k] = C[j - m_order_u + 1];
	}
	delete C;

	double* E, * F;
	E = new double[m_number_u];
	F = new double[m_number_v];
	j = GetUIntervalIndex(u);
	for (k = 0; k < m_number_v; k++)
	{
		for (i = 0; i < m_number_u; i++)
			E[i] = GetWeight(i, k);
		for (r = 1; r <= m_order_u - 1; r++)
		{
			for (i = j - m_order_u + 1; i <= j - r; i++)
				E[i] = E[i] * (m_knot_u[i + m_order_u] - u) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]) +
				E[i + 1] * (u - m_knot_u[i + r]) / (m_knot_u[i + m_order_u] - m_knot_u[i + r]);
		}
		F[k] = E[j - m_order_u + 1];
	}
	delete E;

	for (i = 0; i < m_number_v; i++)
	{
		curv->SetWeight(i, F[i]);
		curv->SetControlPoint(i, ZeroP3D + D[i] / F[i]);
	}
	delete D;
	delete F;
	return curv;
}

void C_NurbsSurf::Save(std::ofstream& file)
{
	//control number
	file << GetCtrlNumberU() << " " << GetCtrlNumberV() << "\n";

	//degree
	file << GetOrderU() - 1 << " " << GetOrderV() - 1 << "\n";

	//uknots
	for (int i = 0; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		file << GetUKnot(i) << " ";
	}
	file << "\n";

	//v knots
	for (int i = 0; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		file << GetVKnot(i) << " ";
	}
	file << "\n";

	for (int i = 0; i < GetCtrlNumberU(); i++)
		for (int j = 0; j < GetCtrlNumberV(); j++)
		{
			Point3D p1 = GetControlPoint(i, j);
			double w = GetWeight(i, j);

			file << p1.x << " " << p1.y << " " << p1.z << " " << w << "\n";
		}
}

void C_NurbsSurf::Read(std::ifstream& file)
{
	//control number
	int uctrlnum, vctrlnum;
	file >> uctrlnum >> vctrlnum;

	int udegree, vdegree;
	//degree
	file >> udegree >> vdegree;

	SetNumberOrder(uctrlnum, vctrlnum, udegree + 1, vdegree + 1);

	//uknots
	double uknot, vknot;
	for (int i = 0; i < uctrlnum + udegree + 1; i++)
	{
		file >> uknot;
		SetUKnot(i, uknot);
	}

	//v knots
	for (int i = 0; i < vctrlnum + vdegree + 1; i++)
	{
		file >> vknot;
		SetVKnot(i, vknot);
	}

	Point3D point;
	double weight;
	for (int i = 0; i < uctrlnum; i++)
		for (int j = 0; j < vctrlnum; j++)
		{
			file >> point.x >> point.y >> point.z >> weight;
			SetControlPoint(i, j, point);
			SetWeight(i, j, weight);
		}
}

CS_NurbsSurf NurbssCopy(CS_NurbsSurf surf)
{
	int orderu, numberu, orderv, numberv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	numberu = surf.nu;
	numberv = surf.nv;
	int knotnumu = numberu + orderu;
	int knotnumv = numberv + orderv;
	int ctrlnum = numberu * numberv;

	CS_NurbsSurf  Rsurf;
	Rsurf.knotu = new double[knotnumu];
	Rsurf.knotv = new double[knotnumv];
	Rsurf.weight = new double[ctrlnum];
	Rsurf.ctrl = new Point3D[ctrlnum];
	Rsurf.nu = numberu;
	Rsurf.nv = numberv;
	Rsurf.orderu = orderu;
	Rsurf.orderv = orderv;

	memcpy(Rsurf.knotu, surf.knotu, knotnumu * sizeof(double));
	memcpy(Rsurf.knotv, surf.knotv, knotnumv * sizeof(double));
	memcpy(Rsurf.weight, surf.weight, ctrlnum * sizeof(double));
	memcpy(Rsurf.ctrl, surf.ctrl, ctrlnum * sizeof(Point3D));

	/*		 Revised by Yang Lei 2005-10-30
	for( i = 0; i <= numberu+orderu-1; i++ )
	Rsurf.knotu[i] = surf.knotu[i];
	for( i = 0; i <= numberv+orderv-1; i++ )
	Rsurf.knotv[i] = surf.knotv[i];
	for(j = 0; j < numberv*numberu; j++)
	{
	Rsurf.ctrl[j] = surf.ctrl[j];
	Rsurf.weight[j] = surf.weight[j];
	}
	*/
	return Rsurf;
}



// prototype revised by Yang Lei
void FreeNurbsCurve(CS_NurbsCurve& curve)
{
	if (curve.ctrl) {
		delete[] curve.ctrl;
		curve.ctrl = NULL;
	}
	if (curve.weight) {
		delete[] curve.weight;
		curve.weight = NULL;
	}
	if (curve.knot) {
		delete[] curve.knot;
		curve.knot = NULL;
	}
}
void FreeNurbsSurf(CS_NurbsSurf& surf)
{
	delete[] surf.ctrl;
	delete[] surf.weight;
	delete[] surf.knotu;
	delete[] surf.knotv;
	/*
	//----------------------Add by Ruan Xiao Yu-----2005-9-30-----------------------------
	if(surf.curve_left)	//	Add by Yang Lei	2005-11-4
	{
	FreeNurbsCurve(*surf.curve_left);
	delete surf.curve_left;
	surf.curve_left = NULL;
	}
	if(surf.curve_top)
	{
	FreeNurbsCurve(*surf.curve_top);
	delete surf.curve_top;
	surf.curve_top = NULL;
	}
	//--------------------------------------------------------------------------------------------------
	*/
}
void NurbssInsertMultiUKnot(CS_NurbsSurf& surf, int multiplicity, double value)
{
	int numberu, numberv, orderu, orderv;
	CS_NurbsSurf  Csurf = NurbssCopy(surf);

	surf.nu = surf.nu + multiplicity;
	numberu = surf.nu;
	numberv = surf.nv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	delete[] surf.ctrl;
	delete[] surf.weight;
	delete[] surf.knotu;  // not delete knotv
	surf.knotu = new double[numberu + orderu];
	surf.weight = new double[numberu * numberv];
	surf.ctrl = new Point3D[numberu * numberv];

	int i, j;
	CS_NurbsCurve curve;
	curve.knot = new double[Csurf.nu + Csurf.orderu];
	curve.ctrl = new Point3D[Csurf.nu];
	curve.weight = new double[Csurf.nu];
	for (j = 0; j < Csurf.nv; j++)
	{
		curve.n = Csurf.nu;
		curve.order = Csurf.orderu;
		for (i = 0; i <= curve.n + curve.order - 1; i++)
			curve.knot[i] = Csurf.knotu[i];
		for (i = 0; i < curve.n; i++)
		{
			curve.weight[i] = Csurf.weight[IndexMap(Csurf.nu, Csurf.nv, i, j)];
			curve.ctrl[i] = Csurf.ctrl[IndexMap(Csurf.nu, Csurf.nv, i, j)];
		}
		NurbscInsertMultiKnot(curve, multiplicity, value);

		if (j == 0)
			for (i = 0; i <= curve.n + curve.order - 1; i++)
				surf.knotu[i] = curve.knot[i];
		for (i = 0; i < curve.n; i++)
		{
			surf.ctrl[IndexMap(surf.nu, surf.nv, i, j)] = curve.ctrl[i];
			surf.weight[IndexMap(surf.nu, surf.nv, i, j)] = curve.weight[i];
		}
	}
	FreeNurbsCurve(curve);
	FreeNurbsSurf(Csurf);
}
void NurbssInsertMultiVKnot(CS_NurbsSurf& surf, int multiplicity, double value)
{
	int numberu, numberv, orderu, orderv;
	CS_NurbsSurf  Csurf = NurbssCopy(surf);

	surf.nv = surf.nv + multiplicity;
	numberu = surf.nu;
	numberv = surf.nv;
	orderu = surf.orderu;
	orderv = surf.orderv;
	delete[] surf.ctrl;
	delete[] surf.weight;
	delete[] surf.knotv;  // not delete knotu
	surf.knotv = new double[numberv + orderv];
	surf.weight = new double[numberu * numberv];
	surf.ctrl = new Point3D[numberu * numberv];

	int i, j;
	CS_NurbsCurve curve;
	curve.knot = new double[Csurf.nv + Csurf.orderv];
	curve.ctrl = new Point3D[Csurf.nv];
	curve.weight = new double[Csurf.nv];
	for (i = 0; i < Csurf.nu; i++)
	{
		curve.n = Csurf.nv;
		curve.order = Csurf.orderv;
		for (j = 0; j <= curve.n + curve.order - 1; j++)
			curve.knot[j] = Csurf.knotv[j];
		for (j = 0; j < curve.n; j++)
		{
			curve.weight[j] = Csurf.weight[IndexMap(Csurf.nu, Csurf.nv, i, j)];
			curve.ctrl[j] = Csurf.ctrl[IndexMap(Csurf.nu, Csurf.nv, i, j)];
		}
		NurbscInsertMultiKnot(curve, multiplicity, value);

		if (i == 0)
			for (j = 0; j <= curve.n + curve.order - 1; j++)
				surf.knotv[j] = curve.knot[j];
		for (j = 0; j < curve.n; j++)
		{
			surf.ctrl[IndexMap(surf.nu, surf.nv, i, j)] = curve.ctrl[j];
			surf.weight[IndexMap(surf.nu, surf.nv, i, j)] = curve.weight[j];
		}
	}
	FreeNurbsCurve(curve);
	FreeNurbsSurf(Csurf);
}
void C_NurbsSurf::InsertMultiUKnot(int multiplicity, double value)
{
	int k;
	CS_NurbsSurf  surf;
	surf.nu = m_number_u;
	surf.nv = m_number_v;
	surf.orderu = m_order_u;
	surf.orderv = m_order_v;
	surf.knotu = new double[surf.nu + surf.orderu];
	surf.knotv = new double[surf.nv + surf.orderv];
	surf.ctrl = new Point3D[surf.nu * surf.nv];
	surf.weight = new double[surf.nu * surf.nv];

	for (k = 0; k <= surf.nu + surf.orderu - 1; k++)
		surf.knotu[k] = m_knot_u[k];
	for (k = 0; k <= surf.nv + surf.orderv - 1; k++)
		surf.knotv[k] = m_knot_v[k];
	for (k = 0; k < surf.nu * surf.nv; k++)
	{
		surf.ctrl[k] = m_control[k];
		surf.weight[k] = m_weight[k];
	}
	// 调用mathlib中的函数
	NurbssInsertMultiUKnot(surf, multiplicity, value);
	SetNumberOrder(surf.nu, surf.nv, surf.orderu, surf.orderv);
	for (k = 0; k <= m_number_u + m_order_u - 1; k++)
		m_knot_u[k] = surf.knotu[k];
	for (k = 0; k <= m_number_v + m_order_v - 1; k++)
		m_knot_v[k] = surf.knotv[k];
	for (k = 0; k < m_number_u * m_number_v; k++)
	{
		m_control[k] = surf.ctrl[k];
		m_weight[k] = surf.weight[k];
	}
	delete surf.knotu;
	delete surf.knotv;
	delete surf.ctrl;
	delete surf.weight;
}

void C_NurbsSurf::InsertMultiVKnot(int multiplicity, double value)
{
	int k;
	CS_NurbsSurf  surf;
	surf.nu = m_number_u;
	surf.nv = m_number_v;
	surf.orderu = m_order_u;
	surf.orderv = m_order_v;
	surf.knotu = new double[surf.nu + surf.orderu];
	surf.knotv = new double[surf.nv + surf.orderv];
	surf.ctrl = new Point3D[surf.nu * surf.nv];
	surf.weight = new double[surf.nu * surf.nv];
	for (k = 0; k <= surf.nu + surf.orderu - 1; k++)
		surf.knotu[k] = m_knot_u[k];
	for (k = 0; k <= surf.nv + surf.orderv - 1; k++)
		surf.knotv[k] = m_knot_v[k];
	for (k = 0; k < surf.nu * surf.nv; k++)
	{
		surf.ctrl[k] = m_control[k];
		surf.weight[k] = m_weight[k];
	}
	// 调用mathlib中的函数
	NurbssInsertMultiVKnot(surf, multiplicity, value);
	SetNumberOrder(surf.nu, surf.nv, surf.orderu, surf.orderv);
	for (k = 0; k <= m_number_u + m_order_u - 1; k++)
		m_knot_u[k] = surf.knotu[k];
	for (k = 0; k <= m_number_v + m_order_v - 1; k++)
		m_knot_v[k] = surf.knotv[k];
	for (k = 0; k < m_number_u * m_number_v; k++)
	{
		m_control[k] = surf.ctrl[k];
		m_weight[k] = surf.weight[k];
	}
	delete surf.knotu;
	delete surf.knotv;
	delete surf.ctrl;
	delete surf.weight;
}




//Point3D C_NurbsSurf::GetParaPoint(double u, double v)
// 求曲面参数域中一点对应的空间点坐标值 {  return Point3D(0,0,0) }
//Vector3D C_NurbsSurf::GetParaNormal(double u, double v)
// 求曲面参数域中一点对应的空间点的法向 {  return Vector3D(0,0,0); }

Vector3D C_NurbsSurf::GetParaNormal(double u, double v, BOOL IsUnit)
// 求曲面参数域中一点对应的空间点的法向
{
	return C_NonuniformBSurf::GetParaNormal(u, v, IsUnit);
}






C_Surf3D* C_NurbsSurf::Copy(void)
{
	return new C_NurbsSurf(m_number_u, m_number_v, m_order_u, m_order_v,
		m_knot_u, m_knot_v, m_control, m_weight);
}







//------------------------------------------------------------
//By Chen xiaodiao  2005-11-08
//显示nurbs曲面的控制网格
//-----------------------------------------------------------





void C_NurbsSurf::ComputDerivative2ALL(double u, double v, Point3D& position, Vector3D& Uvector, Vector3D& Vvector, Vector3D& U2vector, Vector3D& V2vector, Vector3D& UVvector)
{
	Vector3D vector(0, 0, 0);
	// vector = puv/w + puwv/w^2 - pvwu/w^2 - pwuv/w^2 - 2puwv/w^2 + 2pwuwv/w^3
	int i, j;
	Point3D point(0, 0, 0);
	C_NonuniformBSurf Psurf(m_number_u, m_number_v, m_order_u, m_order_v),
		Wsurf(m_number_u, m_number_v, m_order_u, m_order_v);
	for (i = 0; i < m_number_u; i++)
		for (j = 0; j < m_number_v; j++)
		{
			Psurf.SetControlPoint(i, j, ZeroP3D + (GetControlPoint(i, j) - ZeroP3D) * GetWeight(i, j));
			point.x = GetWeight(i, j);
			Wsurf.SetControlPoint(i, j, point);
		}
	for (i = 0; i < m_number_u + m_order_u; i++)
	{
		Psurf.SetUKnot(i, m_knot_u[i]);
		Wsurf.SetUKnot(i, m_knot_u[i]);
	}
	for (i = 0; i < m_number_v + m_order_v; i++)
	{
		Psurf.SetVKnot(i, m_knot_v[i]);
		Wsurf.SetVKnot(i, m_knot_v[i]);
	}
	Point3D poi, wpoi;
	Vector3D P, Pu, Pv, Puv, Pu2, Pv2;
	Vector3D wP, wPu, wPv, wPuv, wPu2, wPv2;
	double   W, Wu, Wv, Wu2, Wv2, Wuv;
	Psurf.ComputDerivative2All(u, v, poi, Pu, Pv, Pu2, Pv2, Puv);
	P = poi - ZeroP3D;
	Wsurf.ComputDerivative2All(u, v, wpoi, wPu, wPv, wPu2, wPv2, wPuv);
	W = wpoi.x;
	Wu = wPu.x;
	Wv = wPv.x;
	Wu2 = wPu2.x;
	Wv2 = wPv2.x;
	Wuv = wPuv.x;
	// 位置 =  P/W
	position = ZeroP3D + P / W;
	// 导数 = (P'*W - P*W')/(W*W)
	Uvector = (Pu * W - P * Wu) / (W * W);
	Vvector = (Pv * W - P * Wv) / (W * W);
	// vector = p''/w - pw''/w^2 - 2p'w'/w^2 + 2pw'^2/w^3
	U2vector = Pu2 / W - (Pu * Wu * 2.0 + P * Wu2) / (W * W) + P * Wu * Wu * 2.0 / (W * W * W);
	V2vector = Pv2 / W - (Pv * Wv * 2.0 + P * Wv2) / (W * W) + P * Wv * Wv * 2.0 / (W * W * W);
	// vector = puv/w + puwv/w^2 - pvwu/w^2 - pwuv/w^2 - 2puwv/w^2 + 2pwuwv/w^3
	UVvector = Puv / W - (Pu * Wv + Pv * Wu + P * Wuv) / (W * W) + P * Wu * Wv * 2.0 / (W * W * W);
	return;
}
CNurbsSuface* C_NurbsSurf::GetCNurbsSurface()
{
	C_NurbsSurf* pSurface = this;
	CNurbsSuface* pSurface1 = new CNurbsSuface();
	pSurface1->m_udegree = pSurface->GetDegreeU();
	pSurface1->m_vdegree = pSurface->GetDegreeV();

	pSurface1->m_uknot = new GLfloat[pSurface->GetCtrlNumberU() + pSurface->GetOrderU()];
	pSurface1->m_vknot = new GLfloat[pSurface->GetCtrlNumberV() + pSurface->GetOrderV()];
	pSurface1->m_uknotcount = pSurface->GetCtrlNumberU() + pSurface->GetOrderU();
	pSurface1->m_vknotcount = pSurface->GetCtrlNumberV() + pSurface->GetOrderV();

	int i, j;
	for (i = 0; i < pSurface1->m_uknotcount; i++)
	{
		pSurface1->m_uknot[i] = pSurface->GetUKnot(i);
	}
	for (i = 0; i < pSurface1->m_vknotcount; i++)
	{
		pSurface1->m_vknot[i] = pSurface->GetVKnot(i);
	}
	for (i = 0; i < pSurface1->GetCtrlNumberU(); i++)
		for (j = 0; j < pSurface1->GetCtrlNumberV(); j++)
		{
			Point3D point = pSurface->GetControlPoint(i, j);
			double weight = pSurface->GetWeight(i, j);

			pSurface1->m_ControlPoint[i][j][0] = point[0] * weight;
			pSurface1->m_ControlPoint[i][j][1] = point[1] * weight;
			pSurface1->m_ControlPoint[i][j][2] = point[2] * weight;
			pSurface1->m_ControlPoint[i][j][3] = weight;

		}
	return pSurface1;
}

//modified pow
double pow_(double a, int n)
{
	double val;
	if (n == 0)
		val = 1.0;
	else
		val = pow(a, n);
	return val;
}

C_NurbsSurf* C_NurbsSurf::GetGeneralBilinearTransformedSurface
(double e00, double e01, double e10, double e11, double f00, double f01, double f10, double f11,
	double g00, double g01, double g10, double g11, double h00, double h01, double h10, double h11,
	double umin, double umax, double vmin, double vmax)
{
	int m = GetDegreeU();
	int n = GetDegreeV();

	int udegree = m + n;
	int vdegree = m + n;

	//generate a C_NurbsSurf
	double* weight = new double[(udegree + 1) * (vdegree + 1)];
	double* uknot = new double[(udegree + 1) * 2];
	double* vknot = new double[(vdegree + 1) * 2];

	int i, j;
	for (i = 0; i < udegree + 1; i++)
	{
		uknot[i] = umin;
		uknot[i + udegree + 1] = umax;
	}

	for (i = 0; i < vdegree + 1; i++)
	{
		vknot[i] = vmin;
		vknot[i + vdegree + 1] = vmax;
	}
	Point3D* pointarray = new Point3D[(udegree + 1) * (vdegree + 1)];
	C_NurbsSurf* pNewNurbsSurface = new C_NurbsSurf(udegree + 1, vdegree + 1,
		udegree + 1, vdegree + 1, uknot, vknot, pointarray, weight);
	int k1, k2;
	double weight_ = 0;
	Vector3D point;
	Vector3D p_;
	double temp_;
	double wij;
	double R, R1;
	for (k2 = 0; k2 < vdegree + 1; k2++)
	{
		for (k1 = 0; k1 < udegree + 1; k1++)
		{
			point[0] = 0;
			point[1] = 0;
			point[2] = 0;
			weight_ = 0;

			for (int i1 = 0; i1 <= m; i1++)
				for (int j1 = 0; j1 <= n; j1++)
				{
					int A1 = min(min(m - i1, k1), k2);
					for (int p1 = 0; p1 <= A1; p1++)
					{
						int A2 = min(k1 - p1, m - i1 - p1);
						for (int p2 = 0; p2 <= A2; p2++)
						{
							int A3 = min(k2 - p1, m - i1 - p1 - p2);
							for (int p3 = 0; p3 <= A3; p3++)
							{
								int A5 = min(min(i1, k1 - p1 - p2), k2 - p1 - p3);
								for (int p5 = 0; p5 <= A5; p5++)
								{
									int A6 = min(i1 - p5, k1 - p1 - p2 - p5);
									for (int p6 = 0; p6 <= A6; p6++)
									{
										int A7 = min(i1 - p5 - p6, k2 - p1 - p3 - p5);
										for (int p7 = 0; p7 <= A7; p7++)
										{
											int A9 = min(min(n - j1, k1 - p1 - p2 - p5 - p6), k2 - p1 - p3 - p5 - p7);
											for (int p9 = 0; p9 <= A9; p9++)
											{
												int A10 = min(n - j1 - p9, k1 - p1 - p2 - p5 - p6 - p9);
												for (int p10 = 0; p10 <= A10; p10++)
												{
													int A11 = min(n - j1 - p9 - p10, k2 - p1 - p3 - p5 - p7 - p9);
													for (int p11 = 0; p11 <= A11; p11++)
													{
														int I13 = max(0, k1 + k2 - j1 - 2 * p1 - p2 - p3 - 2 * p5 - p6 - p7 - 2 * p9 - p10 - p11);
														int A13 = min(min(j1, k1 - p1 - p2 - p5 - p6 - p9 - p10), k2 - p1 - p3 - p5 - p7 - p9 - p11);
														for (int p13 = I13; p13 <= A13; p13++)
														{
															int p4 = m - i1 - p1 - p2 - p3;
															int p8 = i1 - p5 - p6 - p7;
															int p12 = n - j1 - p9 - p10 - p11;
															int p14 = k1 - p1 - p2 - p5 - p6 - p9 - p10 - p13;
															int p15 = k2 - p1 - p3 - p5 - p7 - p9 - p11 - p13;
															int p16 = j1 - p13 - k1 - k2 + 2 * p1 + p2 + p3 + 2 * p5 + p6 + p7 + 2 * p9 + p10 + p11 + 2 * p13;
															R1 = 1.0 * Factorial(m, i1) * Factorial(n, j1) * Factorial(m - i1, p1) *
																Factorial(m - i1 - p1, p2) * Factorial(m - i1 - p1 - p2, p3) * Factorial(i1, p5) *
																Factorial(i1 - p5, p6) * Factorial(i1 - p5 - p6, p7) * Factorial(n - j1, p9) * Factorial(n - j1 - p9, p10) *
																Factorial(n - j1 - p9 - p10, p11) * Factorial(j1, p13) / (Factorial(m + n, k1) * Factorial(m + n, k2) * 1.0);

															R = R1 * pow_(f11 - e11, p1) * pow_(f10 - e10, p2) * pow_(f01 - e01, p3) * pow_(f00 - e00, p4) * pow_(e11, p5) *
																pow_(e10, p6) * pow_(e01, p7) * pow_(e00, p8) * pow_(h11 - g11, p9) * pow_(h10 - g10, p10) * pow_(h01 - g01, p11) * pow_(h00 - g00, p12) *
																pow_(g11, p13) * pow_(g10, p14) * pow_(g01, p15) * pow_(g00, p16);
															wij = GetWeight(i1, j1);
															temp_ = R * wij;
															weight_ += temp_;
															p_ = GetControlPoint(i1, j1) - ZeroP3D;
															point = point + p_ * (temp_);
															if (fabs(R) > 1e-5)
																TRACE("(%d %d) R = %f wij = %f pij=(%f %f %f)\n", i1, j1, R, wij, p_.x, p_.y, p_.z);
															int ijij = 0;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			pNewNurbsSurface->SetWeight(k1, k2, weight_);
			point = point / weight_;
			pNewNurbsSurface->SetControlPoint(k1, k2, point);
		}
	}

	return pNewNurbsSurface;
}

double C_NurbsSurf::Compute_cubicclk(int l, int k, int m, int n, double a1, double a2, double a3)
{

	int e1low = max(0, l + k - m * 2);
	int e1high = min(l / 2, k);
	int e1, e2, e3;
	char message_[1000];
	char message[1000] = "";
	double rval = 0;
	for (e1 = e1low; e1 <= e1high; e1++)
	{
		int e2low = max(0, l + k * 2 - m * 2 - e1 * 2);
		int e2high = min(k - e1, l - e1 * 2);

		for (e2 = e2low; e2 <= e2high; e2++)
		{
			int e3low = max(l + k - m - e2 - e1 * 2, 0);
			int e3high = min((l - e1 * 2 - e2) / 2, m - k);
			for (e3 = e3low; e3 <= e3high; e3++)
			{
				int f1, f2, f3, f4;
				f1 = f2 = f3 = f4 = 1;
				if (e1 > 0 && e1 < k)
					f1 = Factorial(k, e1);

				if (e2 > 0 && e2 < k - e1)
					f2 = Factorial(k - e1, e2);

				if (e3 > 0 && e3 < m - k)
					f3 = Factorial(m - k, e3);

				if (l - e1 * 2 - e2 - e3 * 2 > 0 && l - e1 * 2 - e2 - e3 * 2 < m - k - e3)
					f4 = Factorial(m - k - e3, l - e1 * 2 - e2 - e3 * 2);

				int n1, n2, n3, n4;
				n1 = -1;
				n2 = m - k;
				n3 = 2;
				n4 = l - e1 * 2 - e3 * 2;

				int r1, r2;
				r1 = pow((double)n1, (double)n2);
				r2 = pow((double)n3, (double)n4);
				double f = f1 * f2 * f3 * f4 * r1 * r2;

				if (e1 > 0)
				{
					f = f * pow(a3 - 1, e1);
				}
				if (e2 > 0)
				{
					f = f * pow(a2 - 1, e2);
				}
				if (k - e1 - e2 > 0)
				{
					f = f * pow(a1 - 1, k - e1 - e2);
				}
				if (l - e1 * 2 - e2 - e3 * 2 > 0)
				{
					f = f * pow(a2, l - e1 * 2 - e2 - e3 * 2);
				}
				if (e3 > 0)
				{
					f = f * pow(a3, e3);
				}
				if (m + e2 + e1 * 2 + e3 - k - l > 0)
				{
					f = f * pow(a1, m + e2 + e1 * 2 + e3 - k - l);
				}

				rval = rval + f;
			}
		}
	}
	return rval;
}

void C_NurbsSurf::TraceKnots()
{
	int index = 0;
	while (1)
	{
		for (int ii = 0; ii < 10; ii++)
			TRACE(" %1.2f ", GetUKnot(index));

		index = GetNextUknotIndex(index);
		if (index == -1)
			break;
	}
	TRACE("\n");

	index = 0;
	while (1)
	{
		for (int ii = 0; ii < 10; ii++)
			TRACE(" %1.2f ", GetVKnot(index));

		index = GetNextVknotIndex(index);
		if (index == -1)
			break;
	}
	TRACE("\n");
}
void C_NurbsSurf::SubdividSurface()
{
	//Subdivide the surface into Bezier surfaces.
	for (int i = 0; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		double u = GetUKnot(i);
		int mul = GetUKnotMulti(u);
		if (mul < 4)
			InsertMultiUKnot(4 - mul, u);
	}

	for (int i = 0; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		double v = GetVKnot(i);
		int mul = GetVKnotMulti(v);
		if (mul < 4)
			InsertMultiVKnot(4 - mul, v);
	}
}
int  C_NurbsSurf::GetUKnotMulti(double u)
{
	//Get the multiplicity of the u knot.
	int mul = 0;
	double tol = 1e-8;
	for (int i = 0; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		if (fabs(GetUKnot(i) - u) < tol)
			mul++;
	}
	return mul;
}
int  C_NurbsSurf::GetNextUknotIndex(int index)
{
	double tol = 1e-8;
	for (int i = index + 1; i < GetCtrlNumberU() + GetOrderU(); i++)
	{
		if (fabs(GetUKnot(i) - GetUKnot(index)) > tol)
		{
			return i;
		}
	}
	return -1;
}
int  C_NurbsSurf::GetNextVknotIndex(int index)
{
	double tol = 1e-8;
	for (int i = index + 1; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		if (fabs(GetVKnot(i) - GetVKnot(index)) > tol)
		{
			return i;
		}
	}
	return -1;
}
int  C_NurbsSurf::GetVKnotMulti(double v)
{
	//Get the multiplicity of the v knot.
	int mul = 0;
	double tol = 1e-8;
	for (int i = 0; i < GetCtrlNumberV() + GetOrderV(); i++)
	{
		if (fabs(GetVKnot(i) - v) < tol)
			mul++;
	}
	return mul;
}

Point3D C_NurbsSurf::GetClosestPoint(Point3D point, Point2D& para)
{
	int subdivisionUnum = 10;
	int subdivisionVnum = 10;

	int closestUindex;
	int closestVindex;
	double minumdis = 1e10;
	double ustart, uend;
	double vstart, vend;
	Point3D p;
	ustart = 0;
	uend = 1;
	vstart = 0;
	vend = 1;
	double tol = 1e-5;
LL:
	for (int i = 0; i <= subdivisionVnum; i++)
		for (int j = 0; j <= subdivisionUnum; j++)
		{
			double v = vstart + i * (vend - vstart) / (0.0 + subdivisionVnum);
			double u = ustart + j * (uend - ustart) / (0.0 + subdivisionUnum);

			p = ComputPosition(u, v);
			double dis = Distance(p, point);

			if (dis < minumdis)
			{
				minumdis = dis;
				closestVindex = i;
				closestUindex = j;
			}
		}
	double ulength = uend - ustart;
	double vlength = vend - vstart;
	if (closestUindex == 0 || closestUindex == subdivisionUnum ||
		closestVindex == 0 || closestVindex == subdivisionVnum)
	{

		ustart = ustart - ulength / 2;
		uend = uend + ulength / 2;

		vstart = vstart - vlength / 2;
		vend = vend + vlength / 2;
		goto LL;
	}


	double v = vstart + closestVindex * (vend - vstart) / (0.0 + subdivisionVnum);
	double u = ustart + closestUindex * (uend - ustart) / (0.0 + subdivisionUnum);
	p = ComputPosition(u, v);
	Vector3D udir = ComputUDerivative1(u, v);
	Vector3D vdir = ComputVDerivative1(u, v);
	udir.SetUnit();
	vdir.SetUnit();
	Vector3D normal = udir ^ vdir;
	Vector3D dir = point - p;
	dir.SetUnit();
	if (fabs(fabs(dir * normal) - 1) < tol)
	{
		para.x = u;
		para.y = v;
		return p;
	}

	ustart = ustart + (closestUindex - 1) * (ulength) / (0.0 + subdivisionUnum);
	uend = ustart + (closestUindex + 1) * (ulength) / (0.0 + subdivisionUnum);

	vstart = vstart + (closestVindex - 1) * (vlength) / (0.0 + subdivisionVnum);
	vend = vstart + (closestVindex + 1) * (vlength) / (0.0 + subdivisionVnum);

	goto LL;
}











