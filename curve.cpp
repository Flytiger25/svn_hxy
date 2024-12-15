#include "stdafx.h"
#include "point3d.h"
#include "curve.h"
#include "math.h"
#include "InterFunction.h"

// 由型值点估计端切向   type = 0 返回抛物端切向  type = 1返回圆弧端切向
Vector3D GetEndTangent(Point3D a, Point3D b, Point3D c, int type)
{
	if (type == 0)
	{
		double t;
		Vector3D  vector;
		Point3D control;
		t = Distance(a, b) / (Distance(a, b) + Distance(b, c));
		control.x = (b.x - a.x * (1 - t) * (1 - t) - c.x * t * t) / (2.0 * t * (1 - t));
		control.y = (b.y - a.y * (1 - t) * (1 - t) - c.y * t * t) / (2.0 * t * (1 - t));
		control.z = (b.z - a.z * (1 - t) * (1 - t) - c.z * t * t) / (2.0 * t * (1 - t));
		vector.x = control.x - a.x;
		vector.y = control.y - a.y;
		vector.z = control.z - a.z;
		vector.SetUnit();
		return vector;
	}
	else
	{
		Vector3D v1 = b - a, v2 = c - b;
		v1.SetUnit();
		v2.SetUnit();
		// 判向量平行: 三点是否共线
		if (fabs(v2.x / v1.x - v2.y / v1.y) < TOLER)
			return v1;
		// 三点是否共线
		Point3D center;
		Vector3D vector, vn = v2 ^ v1;
		CS_Line  lin1, lin2;
		lin1.pa = MidPoint(a, b);
		lin1.vn = v1 ^ vn;
		lin2.pa = MidPoint(b, c);
		lin2.vn = v2 ^ vn;
		LineLineInters(lin1, lin2, center);
		vector = vn ^ (center - a);
		vector.SetUnit();
		return vector;
	}
}

// 三次B样条曲线插值, 用于Sweeping: 节点向量由用户指定, 端切向由系统自动估计
int BscurvInterpolateSweep(int number, Point3D* point, double* knot, CS_BSplineCurve& rcurv)
{
	Vector3D vect, vs, ve;
	if (number > 2)
	{
		// 估计端切向
		vs = GetEndTangent(point[0], point[1], point[2]) *
			(0.36 * Distance(point[1], point[0]));
		ve = -GetEndTangent(point[number - 1], point[number - 2], point[number - 3]) *
			(0.36 * Distance(point[number - 1], point[number - 2]));
		//        vs = GetEndTangent(point[0],point[1],point[2], 1)*
		//             ((knot[4] - knot[3])*Distance(point[1],point[0]));
		//        ve = - GetEndTangent(point[number-1],point[number-2],point[number-3], 1)*
		//               ((knot[number+2] - knot[number+1])*Distance(point[number-1],point[number-2]));
		if (Distance(point[0], point[number - 1]) < TOLER)
		{
			vect = point[1] - point[number - 2];
			vect.SetUnit();
			vs = vect * (Distance(point[0], point[1]) +
				Distance(point[1], point[2])) / 5.40;
			ve = vect * (Distance(point[number - 1], point[number - 2]) +
				Distance(point[number - 2], point[number - 3])) / 5.40;
		}
	}
	return BscurvInterpolat(number, point, vs, ve, knot, rcurv);
}

Point3D C_Curve3D::GetStartPoint(void) { return Point3D(0, 0, 0); }
Point3D C_Curve3D::GetEndPoint(void) { return Point3D(0, 0, 0); }
Point3D C_Curve3D::GetMiddlePoint(void) { return Point3D(0, 0, 0); }
Point3D C_Curve3D::GetParaPoint(double t) { return Point3D(0, 0, 0); }
double C_Curve3D::GetPointPara(Point3D p) { return 0; }
double C_Curve3D::GetPointDistance(Point3D p) { return 0; }
BOOL C_Curve3D::PointOn(Point3D p, double toler) { return TRUE; }
C_Curve3D* C_Curve3D::Copy(void) { return this; }
C_Curve3D* C_Curve3D::Reverse(void) { return this; }

void C_Curve3D::SplitD(double t, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_Curve3D::SplitP(Point3D& p, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_Curve3D::SplitN(int n, double t[], Point3D p[], C_Curve3D* newcurve[]) { }

Vector3D C_Curve3D::GetParaTangent(double t) { return Vector3D(0, 0, 0); }

// Calculate the box of curve in specific ucs

// 构造函数和析构函数
C_BSplineCurve3D::C_BSplineCurve3D(void)
{
}


C_BSplineCurve3D::C_BSplineCurve3D(int number, int order) :
	C_ParaCurve3D(number)
{
	m_order = order;
}



C_BSplineCurve3D::C_BSplineCurve3D(int number, int order, Point3D* control) :
	C_ParaCurve3D(number, control)
{
	m_order = order;
}


C_BSplineCurve3D::~C_BSplineCurve3D(void)
{
}


// 用于数据存取和显示的成员函数
int C_BSplineCurve3D::GetDegree(void)
{
	return m_order - 1;
}

int C_BSplineCurve3D::GetOrder(void)
{
	return m_order;
}


void C_BSplineCurve3D::SetOrder(int order)
{
	m_order = order;
}



int C_BSplineCurve3D::CalBFunc(double u, double* Nu, double* Du, double* Hu)
{
	return 0;
}


int C_BSplineCurve3D::PrintInformation(void)
{
	return 0;
}

// 用于集合运算的函数
Point3D C_BSplineCurve3D::GetStartPoint(void) { return Point3D(0, 0, 0); }
Point3D C_BSplineCurve3D::GetEndPoint(void) { return Point3D(0, 0, 0); }
Point3D C_BSplineCurve3D::GetMiddlePoint(void) { return Point3D(0, 0, 0); }
Point3D C_BSplineCurve3D::GetParaPoint(double t) { return Point3D(0, 0, 0); }
double C_BSplineCurve3D::GetPointPara(Point3D p) { return 0; }
double C_BSplineCurve3D::GetPointDistance(Point3D p) { return 0; }
BOOL C_BSplineCurve3D::PointOn(Point3D p, double toler) { return TRUE; }
C_Curve3D* C_BSplineCurve3D::Copy(void) { return this; }
C_Curve3D* C_BSplineCurve3D::Reverse(void) { return this; }

void C_BSplineCurve3D::SplitD(double t, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_BSplineCurve3D::SplitP(Point3D& p, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_BSplineCurve3D::SplitN(int n, double t[], Point3D p[], C_Curve3D* newcurve[]) { }



// 构造函数和析构函数












#define  DISNUM 124          // 曲线绘制时离散段数
#define  ITERAT_NUM 124      // 几何迭代次数

// 构造函数和析构函数
C_NonuniformBCurve3D::C_NonuniformBCurve3D(void)
{
	m_knot = NULL;
}

C_NonuniformBCurve3D::C_NonuniformBCurve3D(int number, int order) :
	C_BSplineCurve3D(number, order)
{
	m_knot = new double[number + order];
}

C_NonuniformBCurve3D::C_NonuniformBCurve3D(int number, int order, Point3D* point,
	double* knot, int flag) :
	C_BSplineCurve3D(flag == 0 || order == 2 ? number : number + 2, order)
{
	if (flag == 0)  // 一般构造函数，point 为控制顶点
	{
		int i;
		m_knot = new double[number + order];
		for (i = 0; i < m_number; i++)
			m_control[i] = point[i];
		for (i = 0; i < m_number + m_order; i++)
			m_knot[i] = knot[i];
	}
	else  // 用于反求控制顶点，但节点向量由用户输入, point 为型值点
	{
		m_knot = new double[m_number + m_order];
		int i;
		CS_BSplineCurve	 rcurve;
		if (order == 2)
		{
			for (i = 0; i < m_number; i++)
				m_control[i] = point[i];
			for (i = 0; i < m_number + m_order; i++)
				m_knot[i] = knot[i];
		}
		else
		{
			BscurvInterpolateSweep(number, point, knot, rcurve);
			for (i = 0; i < m_number; i++)
				m_control[i] = rcurve.ctrl[i];
			for (i = 0; i < m_number + m_order; i++)
				m_knot[i] = rcurve.knot[i];
			delete rcurve.ctrl;
			delete rcurve.knot;
		}
	}
}

// 插值,
C_NonuniformBCurve3D::C_NonuniformBCurve3D(int number, int order, Point3D* control, Vector3D tangent1, Vector3D tangent2,
	double* knot)
{
	m_knot = new double[m_number + m_order];
	int i;
	CS_BSplineCurve	 rcurve;
	// 三次插值
	BscurvInterpolat(number, control, tangent1, tangent2, knot, rcurve);
	for (i = 0; i < m_number; i++)
		m_control[i] = rcurve.ctrl[i];
	for (i = 0; i < m_number + m_order; i++)
		m_knot[i] = rcurve.knot[i];
	delete rcurve.ctrl;
	delete rcurve.knot;
}

// 用于反求控制顶点，节点向量由累加弦长法确定
// number 为型值点个数  point 为型值点
C_NonuniformBCurve3D::C_NonuniformBCurve3D(int number, int order, Point3D* point) :
	//                      C_BSplineCurve3D(number - 1, order)
	C_BSplineCurve3D(number + 2, order)
{
	m_knot = new double[m_number + m_order];
	int i;
	CS_BSplineCurve	 rcurve;
	BscurvInterpolatAuto(number, point, rcurve);
	for (i = 0; i < m_number; i++)
		m_control[i] = rcurve.ctrl[i];
	for (i = 0; i < m_number + m_order; i++)
		m_knot[i] = rcurve.knot[i];
	delete rcurve.ctrl;
	delete rcurve.knot;
}
void GetUniformInterpolatKnot(int number, Point3D* point, double*& knot)
// 生成均匀的节点向量
{
	int k, ctrlnumb = number + 2;
	double len, lenth, * chord;
	chord = new double[number];
	knot = new double[ctrlnumb + 4];
	for (k = 4; k < ctrlnumb; k++)
	{
		knot[k] = ((double)(k - 4 + 1)) / (number - 1);
	}
	for (k = 0; k < 4; k++)
		knot[k] = 0;
	for (k = ctrlnumb; k < ctrlnumb + 4; k++)
		knot[k] = 1;
}
void GetInterpolatKnot(int number, Point3D* point, double*& knot)
// 累加弦长确定节点向量
{
	int k, ctrlnumb = number + 2;
	double len, lenth, * chord;
	chord = new double[number];
	knot = new double[ctrlnumb + 4];
	lenth = 0;
	for (k = 1; k < number; k++)
	{
		chord[k] = Distance(point[k], point[k - 1]);
		lenth += chord[k];
	}
	len = 0;
	for (k = 4; k < ctrlnumb; k++)
	{
		len += chord[k - 4 + 1];
		knot[k] = len / lenth; //chord[k-rcurv.order+1]/lenth;
	}
	for (k = 0; k < 4; k++)
		knot[k] = 0;
	for (k = ctrlnumb; k < ctrlnumb + 4; k++)
		knot[k] = 1;
	delete chord;
}

// 三次B样条曲线插值: 节点和端切向由系统自动估计
int BscurvInterpolatAuto(int number, Point3D point[], CS_BSplineCurve& rcurv)
{
	Vector3D vect, vs, ve;
	double* knot = NULL;
	if (number > 2)
	{
		GetInterpolatKnot(number, point, knot);
		vs = point[1] - point[0];
		ve = point[number - 1] - point[number - 2];
		// 估计端切向
//        vs = GetEndTangent(point[0],point[1],point[2])*
 //           (0.36*Distance(point[1],point[0]));
//            ((knot[4] - knot[3])*Distance(point[1],point[0]));
 //       ve = - GetEndTangent(point[number-1],point[number-2],point[number-3])*
//               ((knot[number+2] - knot[number+1])*Distance(point[number-1],point[number-2]));
//               (0.36*Distance(point[number-1],point[number-2]));

/*        vs = GetEndTangent(point[0],point[1],point[2])*
			(3.0*Distance(point[1],point[0])/(knot[4] - knot[3]));
		ve = - GetEndTangent(point[number-1],point[number-2],point[number-3])*
			   (3.0*Distance(point[number-1],point[number-2])/(knot[number+2] - knot[number+1]));
		if(Distance(point[0],point[number-1]) < TOLER)
		{
			vect = point[1] - point[number-2];
			vect.SetUnit();
			vs = vect*(Distance(point[0],point[1]) +
					Distance(point[1],point[2]))/5.4;
			ve = vect*(Distance(point[number-1],point[number-2]) +
						   Distance(point[number-2],point[number-3]))/5.40;
		}*/
	}
	int flag = BscurvInterpolat(number, point, vs, ve, knot, rcurv);
	delete knot;
	return flag;
}

C_NonuniformBCurve3D::~C_NonuniformBCurve3D(void)
{
	delete m_knot;
}

// 用于数据存取和显示的成员函数
void C_NonuniformBCurve3D::SetNumberOrder(int number, int order)
{
	SetOrder(order);
	SetCtrlNumber(number);
	delete m_knot;
	m_knot = new double[number + order];
}

double C_NonuniformBCurve3D::GetKnot(int index)
{
	return m_knot[index];
}

void C_NonuniformBCurve3D::SetKnot(int index, double value)
{
	m_knot[index] = value;
}

double* C_NonuniformBCurve3D::GetKnotVector()
{
	return m_knot;
}

int C_NonuniformBCurve3D::GetKnotMulti(double value)
{
	int i, j, multiple;
	for (i = 0; i < m_number + m_order; i++)
		if (fabs(m_knot[i] - value) < TOLER)
			break;
	if (i == m_number + m_order)
		return 0;
	multiple = 1;
	for (j = i + 1; j < m_number + m_order; j++)
		if (fabs(m_knot[j] - value) < TOLER)
			multiple++;
		else
			break;
	return multiple;
}

int C_NonuniformBCurve3D::GetIntervalIndex(double  value)
// 判别value在那个节点区间
{
	/*
		int i, ret1;

		ret1 = -1;
		for( i = m_order - 1; i <= m_number; i++ )
			if( (value >= m_knot[i]-TOLER) && (value <= m_knot[i+1]+TOLER) && (m_knot[i+1] - m_knot[i] > TOLER) ) //sysTolZerovalue))
			{
				ret1 = i;
				break;
			}
	*/
	//If the value is out of the valid scope, 
	//then drag it back 
	if (value < m_knot[0])
		value = m_knot[0];
	if (value > m_knot[m_number + m_order - 1])
		value = m_knot[m_number + m_order - 1];


	int a, b, c, ret2 = -1;
	a = m_order - 1;
	b = m_number;
	value -= TOLER;
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (value <= m_knot[c] - TOLER)
			b = c;
		else
			a = c;
	}
	if ((value >= m_knot[a] - (TOLER * 2)) && (value <= m_knot[b]))
		ret2 = a;
	//	ASSERT(ret1 == ret2);

	return ret2;	// 非法值
}

int C_NonuniformBCurve3D::GetMonotonyKnot(double*& t)
{
	int  SplitNumber = 1;
	t = new double[m_number - m_order + 2];
	t[0] = 0;
	for (int i = 0; i < m_number - m_order; i++)
	{
		if (m_knot[m_order + i] > m_knot[m_order + i - 1])
		{
			t[SplitNumber] = m_knot[m_order + i];
			SplitNumber++;
		}
	}
	t[SplitNumber] = 1;
	SplitNumber++;
	return SplitNumber;
}

Point3D C_NonuniformBCurve3D::ComputPosition(double value)
{
	int			i, r, j, m;
	double		fz1, fz2, fm;
	Vector3D	C[6];

	j = GetIntervalIndex(value);
	m = j - m_order + 1;

	for (i = m; i <= j; i++)
		C[i - m] = m_control[i] - ZeroP3D;

	for (r = 1; r <= m_order - 1; r++)
	{
		for (i = m; i <= j - r; i++)
		{
			fz1 = m_knot[i + m_order] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + m_order] - m_knot[i + r];
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}

	return C[0];
}


Vector3D C_NonuniformBCurve3D::ComputDerivative1(double value)
{
	int			i, r, j, m, order;
	double		fz1, fz2, fm;
	Vector3D	C[4];

	order = m_order - 1;
	j = GetIntervalIndex(value);
	m = j - m_order + 1;

	for (i = j; i > m; i--)
		C[i - m] = (m_control[i] - m_control[i - 1]) * order / (m_knot[i + order] - m_knot[i]);

	for (r = 1; r <= order - 1; r++)
	{
		for (i = m + 1; i <= j - r; i++)
		{
			fz1 = m_knot[i + order] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + order] - m_knot[i + r];
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}
	return C[1];
}

Vector3D C_NonuniformBCurve3D::ComputDerivative2(double value)
{
	int number, order, i, j, k, l;
	number = m_number;
	order = m_order;
	k = order - 1;

	double* alf;
	Vector3D* C, pd2;

	C = new Vector3D[number];
	alf = new double[number];
	i = GetIntervalIndex(value);
	// 计算 P''(t), W''(t)
	for (j = i - k; j <= i; j++)
		C[j] = m_control[j] - ZeroP3D;
	for (j = i; j >= i - k + 1; j--)
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	for (j = i; j >= i - k + 2; j--)
		C[j] = (C[j] - C[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
	k = k - 2;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
	}
	pd2 = C[i];
	delete C;
	delete alf;
	return pd2;
}

double C_NonuniformBCurve3D::ComputCurvature(double value)
{
	Vector3D d1, d2;
	d1 = ComputDerivative1(value);
	d2 = ComputDerivative2(value);
	double len = Length(d1);
	double curvature = Length(d1 ^ d2) / len / len / len;
	return curvature;
}

/* 曲线光顺
void C_NonuniformBCurve3D::Fairness(void)
{
	C_NonuniformBCurve3D *precurv = (C_NonuniformBCurve3D*)Copy();
	C_NonuniformBCurve3D *curv = (C_NonuniformBCurve3D*)Copy();
	int i, k, number;
	double maxdist, temp, *t, *s, *c, *r;
	Point3D *point;
	number = curv->GetMonotonyKnot(t);
	if(number > 3)
	{
		c = new double[number*2];
		s = new double[number*2];
		r = new double[number*2];
		point = new Point3D[number*2];
		int apn;  //active point number
		for(i = 0; i < 99; i++)
		{
			for(k = 0; k < number; k++)
				c[k] = curv->ComputCurvature(t[k]);
			for(k = 1; k < number - 1; k++)
				r[k] = (c[k+1]-c[k])/(t[k+1]-t[k]) - (c[k]-c[k-1])/(t[k]-t[k-1]);
			r[0] = 0;
			r[number-1] = 0;
			s[0] = t[0];
			s[1] = t[1];
			apn = 2;
			for(k = 1; k < number - 2; k++)
			{
				if(r[k-1]*r[k] >= 0 && r[k]*r[k+1] >= 0 && apn != 2)
				{
					s[apn] = t[k];
					apn++;
				}
				else if(r[k]*r[k+1] < 0)
				{
					 temp = - r[k]/r[k+1];
					 s[apn] = (t[k+1] + temp*t[k])/(1 + temp);
					 apn++;
				}
				else if(r[k]*r[k+2] < 0 && t[k+1] - t[k] > 0.5*(t[k+2] - t[k-1]))
				{
					s[apn] = (t[k] + t[k+1])*0.5;
					apn++;
				}
				else
					temp = 1;
			}
			s[apn] = t[number-2];    apn++;
			s[apn] = t[number-1];    apn++;
			for(k = 0; k < apn; k++)
				point[k] = curv->ComputPosition(s[k]);
			delete precurv;
			precurv = (C_NonuniformBCurve3D*)curv->Copy();
			// 求新插值曲线
			CS_BSplineCurve	 rcurve;
			BscurvInterpolatAuto(apn, point, rcurve);
			curv->SetNumberOrder(rcurve.n, rcurve.order);
			for(k = 0; k < rcurve.n; k++)
				curv->SetControlPoint(k, rcurve.ctrl[k]);
			for(k = 0; k < rcurve.n + rcurve.order; k++)
				curv->SetKnot(k, rcurve.knot[k]);
			delete rcurve.ctrl;
			delete rcurve.knot;
			// 判是否满足终止条件
			maxdist = 0;  // 最大距离
			for(k = 0; k < number; k++)
			{
				temp = Distance(curv->ComputPosition(t[k]), precurv->ComputPosition(t[k]));
				if(temp > maxdist)
					maxdist = temp;
			}
			if(maxdist < 0.001)
				break;
		}
		delete c;
		delete s;
		delete r;
		SetNumberOrder(curv->m_number, curv->m_order);
		for(k = 0; k < m_number; k++)
			m_control[k] = curv->GetControlPoint(k);
		for(k = 0; k < m_number + m_order; k++)
			m_knot[k] = curv->GetKnot(k);
	}
	delete curv;
	delete precurv;
}
*/

void C_NonuniformBCurve3D::Draw(CDC* pDC)
{
	int      j;
	double   t1, t2, t;
	Point3D  point, newpoint;

	t1 = m_knot[m_order - 1];
	t2 = m_knot[m_number];
	point = ComputPosition(t1);
	for (j = 1; j <= DISNUM; j++)
	{
		t = t1 + j * (t2 - t1) / DISNUM;
		newpoint = ComputPosition(t);
		pDC->MoveTo(point.x, point.y);
		pDC->LineTo(newpoint.x, newpoint.y);

		point = newpoint;
	}
}

void C_NonuniformBCurve3D::InsertKnot(double value)
{
	int i, j;
	double* newknot;
	Point3D* newpoint;
	newknot = new double[m_number + m_order + 1];
	newpoint = new Point3D[m_number + 1];
	j = GetIntervalIndex(value);
	for (i = 0; i <= m_number + m_order; i++)
	{
		if (i <= j)
			newknot[i] = m_knot[i];
		else if (i == j + 1)
			newknot[i] = value;
		else
			newknot[i] = m_knot[i - 1];
	}
	for (i = 0; i <= m_number; i++)
	{
		if (i <= j - m_order + 1)
			newpoint[i] = m_control[i];
		else if (i >= j + 1)
			newpoint[i] = m_control[i - 1];
		else
			newpoint[i] = ZeroP3D + (m_control[i - 1] - ZeroP3D) * (m_knot[i + m_order - 1] - value) / (m_knot[i + m_order - 1] - m_knot[i]) +
			(m_control[i] - ZeroP3D) * (value - m_knot[i]) / (m_knot[i + m_order - 1] - m_knot[i]);
	}
	SetNumberOrder(m_number + 1, m_order);
	for (i = 0; i < m_number + m_order; i++)
		m_knot[i] = newknot[i];
	for (i = 0; i < m_number; i++)
		m_control[i] = newpoint[i];
	delete newknot;
	delete newpoint;
}

void C_NonuniformBCurve3D::InsertMultiKnot(int multiplicity, double value)
{
	for (int i = 0; i < multiplicity; i++)
		InsertKnot(value);
}

int C_NonuniformBCurve3D::PrintInformation(void)
{
	return 0;
}

// 用于集合运算的函数
Point3D C_NonuniformBCurve3D::GetStartPoint(void)
{
	double t = m_knot[m_order - 1];
	return  ComputPosition(t);
}


Point3D C_NonuniformBCurve3D::GetEndPoint(void)
{
	double t = m_knot[m_number];
	return  ComputPosition(t);
}


Point3D C_NonuniformBCurve3D::GetMiddlePoint(void)
{
	double t1, t2;
	t1 = m_knot[m_order - 1];
	t2 = m_knot[m_number];
	t1 = (t1 + t2) / 2.0;
	return  ComputPosition(t1);
}

Point3D C_NonuniformBCurve3D::GetParaPoint(double t)
{
	double t1, t2;
	t1 = m_knot[m_order - 1];
	t2 = m_knot[m_number];
	return ComputPosition(t1 + (t2 - t1) * t);
}

double C_NonuniformBCurve3D::GetPointPara(Point3D p)
{
	/* int k;
	 double value;
	 CS_BSplineCurve  curv;
	 curv.n = m_number;
	 curv.order = m_order;
	 curv.knot = new double[curv.n+curv.order];
	 curv.ctrl = new Point3D[curv.n];
	 for( k = 0; k < curv.n + curv.order; k++ )
		 curv.knot[k] = m_knot[k];
	 for( k = 0; k < curv.n; k++)
		 curv.ctrl[k] = m_control[k];
	 // 调用mathlib中的函数
	 value = BscurvGetPointPara(curv, p);
	 delete curv.knot;
	 delete curv.ctrl;
	 return value;*/
	return 0;
}

double C_NonuniformBCurve3D::GetPointDistance(Point3D p)
{
	return GetPointDistance(p, 0);
}

double C_NonuniformBCurve3D::GetPointDistance(Point3D p, double* para)
{
	/*  int k;
	  double value;
	  CS_BSplineCurve  curv;
	  curv.n = m_number;
	  curv.order = m_order;
	  curv.knot = new double[curv.n+curv.order];
	  curv.ctrl = new Point3D[curv.n];
	  for( k = 0; k < curv.n + curv.order; k++ )
		  curv.knot[k] = m_knot[k];
	  for( k = 0; k < curv.n; k++)
		  curv.ctrl[k] = m_control[k];
	  // 调用mathlib中的函数
	  double parameter;
	  value = BscurvGetPointDistance(curv, p, parameter);
	  if(para != NULL)
		  *para = parameter;
	  delete curv.knot;
	  delete curv.ctrl;
	  return value;*/
	return 0;
}

BOOL C_NonuniformBCurve3D::PointOn(Point3D p, double toler)
{
	Point3D temp;
	return PointOn(p, temp, toler);
}

BOOL C_NonuniformBCurve3D::PointOn(Point3D p, Point3D& q, double toler)
{
	double dist, para;
	dist = GetPointDistance(p, &para);
	if (dist < toler)
	{
		q = ComputPosition(para);
		return TRUE;
	}
	else
		return FALSE;
}

C_Curve3D* C_NonuniformBCurve3D::Copy(void)
{
	return new C_NonuniformBCurve3D(m_number, m_order, m_control, m_knot);
}

void C_NonuniformBCurve3D::Expand(int flag, double length)
{
	/*  int k;
	  double value;
	  CS_BSplineCurve  curv;
	  curv.n = m_number;
	  curv.order = m_order;
	  curv.knot = new double[curv.n+curv.order];
	  curv.ctrl = new Point3D[curv.n];
	  for( k = 0; k < curv.n + curv.order; k++ )
		  curv.knot[k] = m_knot[k];
	  for( k = 0; k < curv.n; k++)
		  curv.ctrl[k] = m_control[k];
	  // 调用mathlib中的函数
	  BscurvExtend(curv, flag, length);
	  SetNumberOrder(curv.n,curv.order);
	  for( k = 0; k < m_number + m_order; k++ )
		  SetKnot(k, curv.knot[k]);
	  for( k = 0; k < m_number; k++)
		  SetControlPoint(k, curv.ctrl[k]);
	  delete curv.knot;
	  delete curv.ctrl;*/

}

C_Curve3D* C_NonuniformBCurve3D::Reverse(void)
{
	int i, j;
	double  knot, sum;
	Point3D pp;
	i = 0;
	j = m_number - 1;
	while (i < j)
	{
		pp = m_control[i];
		m_control[i] = m_control[j];
		m_control[j] = pp;
		i++;
		j--;
	}
	sum = m_knot[0] + m_knot[m_order + m_number - 1];
	i = 0;
	j = m_order + m_number - 1;
	while (i < j)
	{
		knot = sum - m_knot[i];
		m_knot[i] = sum - m_knot[j];
		m_knot[j] = knot;
		i++;
		j--;
	}
	if (i == j)
		m_knot[i] = sum - m_knot[i];
	return this;
}

void C_NonuniformBCurve3D::SplitD(double t, C_Curve3D** pC1, C_Curve3D** pC2)
{
	int i, j, r, number1, number2;
	C_NonuniformBCurve3D* curve = (C_NonuniformBCurve3D*)Copy();
	t = m_knot[m_order - 1] + (m_knot[m_number] - m_knot[m_order - 1]) * t;
	//j = curve->GetIntervalIndex(t);
	for (j = m_number + m_order - 1; t < m_knot[j] + TOLER; j--);
	r = curve->GetKnotMulti(t);
	curve->InsertMultiKnot(m_order - r, t);
	number1 = j + 1;
	number2 = m_number + m_order - r - j - 1;
	C_NonuniformBCurve3D* c1, * c2;
	c1 = new C_NonuniformBCurve3D(number1, m_order);
	c2 = new C_NonuniformBCurve3D(number2, m_order);
	for (i = 0; i <= j; i++)
		c1->SetKnot(i, m_knot[i]);
	for (i = j + 1; i <= number1 + m_order - 1; i++)
		c1->SetKnot(i, t);
	for (i = 0; i < number1; i++)
		c1->SetControlPoint(i, curve->m_control[i]);
	for (i = number2 + m_order - 1; i >= m_order; i--)
		c2->SetKnot(i, m_knot[i + m_number - number2]);
	for (i = m_order - 1; i >= 0; i--)
		c2->SetKnot(i, t);
	for (i = number2 - 1; i >= 0; i--)
		c2->SetControlPoint(i, curve->m_control[i + curve->m_number - number2]);

	// 对节点向量重新整理，规划为0、1之间
	double length = c1->m_knot[c1->m_number] - c1->m_knot[c1->m_order - 1];
	for (i = c1->m_order; i < c1->m_number; i++)
		c1->SetKnot(i, (c1->m_knot[i] - c1->m_knot[c1->m_order - 1]) / length);
	for (i = 0; i < c1->m_order; i++)
		c1->SetKnot(i, 0);
	for (i = c1->m_number; i < c1->m_number + c1->m_order; i++)
		c1->SetKnot(i, 1);
	length = c2->m_knot[c2->m_number] - c2->m_knot[c2->m_order - 1];
	for (i = c2->m_order; i < c2->m_number; i++)
		c2->SetKnot(i, (c2->m_knot[i] - c2->m_knot[c2->m_order - 1]) / length);
	for (i = 0; i < c2->m_order; i++)
		c2->SetKnot(i, 0);
	for (i = c2->m_number; i < c2->m_number + c2->m_order; i++)
		c2->SetKnot(i, 1);

	*pC1 = c1;
	*pC2 = c2;
	delete curve;
}

void C_NonuniformBCurve3D::SplitP(Point3D& p, C_Curve3D** pC1, C_Curve3D** pC2)
{
	double value = GetPointPara(p);
	SplitD(value, pC1, pC2);
}

void C_NonuniformBCurve3D::SplitN(int n, double t[], Point3D p[], C_Curve3D* newcurve[])
{
	/*  CS_BSplineCurve bcurve, *newbcurve;
	  newbcurve = new CS_BSplineCurve [n+1];
	  ModelCurveToIntLib_BSplineCurve(this, &bcurve);
	  BscurvMultiSplit(bcurve, n, t, newbcurve);
	  for (int i=0; i<=n; i++)
		  newcurve[i] = new C_NonuniformBCurve3D(newbcurve[i].n, newbcurve[i].order, newbcurve[i].ctrl, newbcurve[i].knot);
	  FreeBSplineCurve(bcurve);
	  for (i=0; i<=n; i++)
		  FreeBSplineCurve(newbcurve[i]);
	  delete [] newbcurve;*/
}

Vector3D C_NonuniformBCurve3D::GetParaTangent(double t)
{
	t = m_knot[m_order - 1] + t * (m_knot[m_number] - m_knot[m_order - 1]);
	Vector3D vector;
	vector = ComputDerivative1(t);
	vector.SetUnit();
	return vector;
}



int C_NonuniformBCurve3D::ConvertToBezier(C_BezierCurve3D* bezier[])
{
	int i, j, number, order, bez_num;
	number = m_number;
	order = m_order;
	bez_num = number - order + 1;  // 曲线段数

	if (bez_num == 1)
	{
		bezier[0] = new C_BezierCurve3D(4);
		for (i = 0; i <= 3; i++)
			bezier[0]->SetControlPoint(i, m_control[i]);
		return 1;
	}
	else
	{
		int  SplitNumber = 0;
		Point3D* poin = new Point3D[bez_num - 1];
		double* t = new double[bez_num - 1];
		for (i = 0; i <= bez_num - 2; i++)
		{
			if (m_knot[order + i] > m_knot[order + i - 1])
			{
				t[SplitNumber] = m_knot[order + i];
				SplitNumber++;
			}
		}
		C_Curve3D** curve = new C_Curve3D * [SplitNumber + 1];
		SplitN(SplitNumber, t, poin, curve);
		// 填 bezier[i] 结构
		Point3D pp;
		for (i = 0; i < SplitNumber + 1; i++)
		{
			bezier[i] = new C_BezierCurve3D(4);
			for (j = 0; j <= 3; j++)
			{
				pp = ((C_NonuniformBCurve3D*)curve[i])->GetControlPoint(j);
				bezier[i]->SetControlPoint(j, pp);
			}
		}
		delete[] poin;
		delete[] t;
		for (i = 0; i < SplitNumber + 1; i++)
			delete curve[i];
		delete[] curve;
		return SplitNumber + 1;
	}
}

// 曲线离散为点及相应参数
void C_NonuniformBCurve3D::SubdivisionWithPara(double Epsilon, Point3D*& vp, double*& t, int& number)
{
	/* int k;
	 double value;
	 CS_BSplineCurve  curv;
	 curv.n = m_number;
	 curv.order = m_order;
	 curv.knot = new double[curv.n+curv.order];
	 curv.ctrl = new Point3D[curv.n];
	 for( k = 0; k < curv.n + curv.order; k++ )
		 curv.knot[k] = m_knot[k];
	 for( k = 0; k < curv.n; k++)
		 curv.ctrl[k] = m_control[k];
	 // 调用mathlib中的函数
	 BscurvSubdivisionWithPara(curv, Epsilon, vp, t, number);
	 delete curv.knot;
	 delete curv.ctrl;*/
}


BOOL C_NonuniformBCurve3D::IfSelfInter(void)
{
	/*  int k, flag;
	  CS_BSplineCurve  curv;
	  curv.n = m_number;
	  curv.order = m_order;
	  curv.knot = new double[curv.n+curv.order];
	  curv.ctrl = new Point3D[curv.n];
	  for( k = 0; k < curv.n + curv.order; k++ )
		  curv.knot[k] = m_knot[k];
	  for( k = 0; k < curv.n; k++)
		  curv.ctrl[k] = m_control[k];
	  // 调用mathlib中的函数
	  flag = IFBscurvSelfInter(curv);
	  delete curv.knot;
	  delete curv.ctrl;
	  return flag;*/
	return TRUE;
}



void C_NonuniformBCurve3D::Serialize(CArchive& ar)
{
	C_BSplineCurve3D::Serialize(ar);

	if (ar.IsStoring())
	{
		for (int i = 0; i < m_number + m_order; i++)
			ar << m_knot[i];
	}
	else
	{
		m_knot = new double[m_number + m_order];
		for (int i = 0; i < m_number + m_order; i++)
			ar >> m_knot[i];
	}
}

void C_NonuniformBCurve3D::ComputDerivative2All(double value, Point3D& position, Vector3D& p1p, Vector3D& p2p)
{
	int number, order, i, j, k, l;
	number = m_number;
	order = m_order;
	k = order - 1;

	double* alf;
	Vector3D* C, pu;
	C = new Vector3D[number];
	alf = new double[number];
	i = GetIntervalIndex(value);
	// 计算 P(t)
	for (j = i - k; j <= i; j++)
		C[j] = m_control[j] - ZeroP3D;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
	}
	pu = C[i];
	position = ZeroP3D + pu;

	// 计算 P'(t) 
	for (j = i - k; j <= i; j++)
		C[j] = m_control[j] - ZeroP3D;
	for (j = i; j >= i - k + 1; j--)
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	k = k - 1;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
	}
	p1p = C[i];

	k = k + 1;
	// 计算 P''(t)
	for (j = i - k; j <= i; j++)
		C[j] = m_control[j] - ZeroP3D;
	for (j = i; j >= i - k + 1; j--)
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	for (j = i; j >= i - k + 2; j--)
		C[j] = (C[j] - C[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
	k = k - 2;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
	}
	p2p = C[i];
	delete C;
	delete alf;
	return;
}
#define DISNUM 64
#define  ITERAT_NUM 124      // 几何迭代次数

C_NurbsCurve3D::C_NurbsCurve3D(void)
{
	m_weight = NULL;
}

void C_NurbsCurve3D::GetBoundingBox(Vector3D& lower, Vector3D& upper)
{
	lower.x = 1e10;
	lower.y = 1e10;
	lower.z = 1e10;
	upper.x = -1e10;
	upper.y = -1e10;
	upper.z = -1e10;

	for (int i = 0; i < GetCtrlNumber(); i++)
	{
		Vector3D p = GetControlPoint(i) - ZeroP3D;

		if (p[0] < lower[0])
			lower[0] = p[0];
		if (p[1] < lower[1])
			lower[1] = p[1];
		if (p[2] < lower[2])
			lower[2] = p[2];

		if (p[0] > upper[0])
			upper[0] = p[0];
		if (p[1] > upper[1])
			upper[1] = p[1];
		if (p[2] > upper[2])
			upper[2] = p[2];
	}
}

C_NurbsCurve3D::C_NurbsCurve3D(int number, int order) :
	C_NonuniformBCurve3D(number, order)
{
	m_weight = new double[number];
}

C_NurbsCurve3D::C_NurbsCurve3D(int number, int order, Point3D* control,
	double* knot, double* weight) :
	C_NonuniformBCurve3D(number, order, control, knot)
{
	m_weight = new double[number];
	for (int i = 0; i < number; i++)
		m_weight[i] = weight[i];
}


C_NurbsCurve3D::C_NurbsCurve3D(int number, int order, Point3D* control, Vector3D tangent0, Vector3D tangent1,
	double* knot, double* weight)
{
	int i;
	CS_NurbsCurve rcurve;
	// 三次插值
	NurbscInterpolat(number, control, knot, weight, tangent0, tangent1, rcurve);
	m_number = rcurve.n;
	m_order = rcurve.order;
	m_control = new Point3D[rcurve.n];
	m_weight = new double[rcurve.n];
	m_knot = new double[rcurve.n + rcurve.order];
	for (i = 0; i < m_number; i++)
	{
		m_control[i] = rcurve.ctrl[i];
		m_weight[i] = rcurve.weight[i];
	}
	for (i = 0; i < m_number + m_order; i++)
		m_knot[i] = rcurve.knot[i];
	delete rcurve.ctrl;
	delete rcurve.knot;
	delete rcurve.weight;
}

// 用于反求控制顶点，节点向量由累加弦长法确定
// number 为型值点个数  point 为型值点
C_NurbsCurve3D::C_NurbsCurve3D(int number, int order, Point3D* point) :
	C_NonuniformBCurve3D(number, order, point)
{
	m_weight = new double[m_number];
	for (int i = 0; i < m_number; i++)
		m_weight[i] = 1;
}

C_NurbsCurve3D::C_NurbsCurve3D(int number, int order, Point3D* point, double* knot) :
	C_NonuniformBCurve3D(number, order, point, knot, 1)
{
	m_weight = new double[m_number];
	for (int i = 0; i < m_number; i++)
		m_weight[i] = 1;
}

C_NurbsCurve3D::~C_NurbsCurve3D(void)
{
	delete m_weight;
}

double C_NurbsCurve3D::ComputeBasis(int i, double t)
{
	return ComputeBasis(i, GetOrder() - 1, t);
}

double C_NurbsCurve3D::ComputeBasis(int i, int p, double u)
{
	double ui = GetKnot(i);
	double uia1 = GetKnot(i + 1);
	if (p == 0)
	{
		if (u >= ui && u <= uia1)
			return 1;
		else
			return 0;
	}
	else
	{
		double denorm1 = (GetKnot(i + p) - ui);
		double denorm2 = (GetKnot(i + p + 1) - uia1);

		double term1, term2;
		if (fabs(denorm1) < 1e-8)
		{
			term1 = 0;
		}
		else
		{
			term1 = (u - ui) * ComputeBasis(i, p - 1, u) / denorm1;
		}
		if (fabs(denorm2) < 1e-8)
		{
			term2 = 0;
		}
		else
		{
			term2 = (GetKnot(i + p + 1) - u) * ComputeBasis(i + 1, p - 1, u) / denorm2;
		}

		double u1 = term1 + term2;
		return u1;
	}
}
// 用于数据存取和显示的成员函数
double C_NurbsCurve3D::GetWeight(int index)
{
	return m_weight[index];
}

void C_NurbsCurve3D::SetWeight(int index, double value)
{
	m_weight[index] = value;
}

int C_NurbsCurve3D::PrintInformation(void)
{
	return 0;
}

void C_NurbsCurve3D::SetNumberOrder(int number, int order)
{
	C_NonuniformBCurve3D::SetNumberOrder(number, order);
	delete m_weight;
	m_weight = new double[number];
}

Point3D C_NurbsCurve3D::ComputPosition(double value)
{
	int			i, r, j, m;
	double		W[80], fz1, fz2, fm;
	Vector3D	C[80];

	j = GetIntervalIndex(value);
	m = j - m_order + 1;

	for (i = m; i <= j; i++)
	{
		W[i - m] = m_weight[i];
		C[i - m] = (m_control[i] - ZeroP3D) * m_weight[i];
	}

	for (r = 1; r <= m_order - 1; r++)
	{
		for (i = m; i <= j - r; i++)
		{
			fz1 = m_knot[i + m_order] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + m_order] - m_knot[i + r];
			W[i - m] = (W[i - m] * fz1 + W[i - m + 1] * fz2) / fm;
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}

	return ZeroP3D + C[0] / W[0];
}


Vector3D C_NurbsCurve3D::ComputDerivative1(double value)
// 导数 = (P'*W - P*W')/(W*W) = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
{
	int			i, r, j, m, order;
	double		fz1, fz2, fm;
	double		W[10], w_value, tw_value;
	Vector3D	C[10], p_vector, tp_vector, tvector;

	order = m_order;
	j = GetIntervalIndex(value);
	m = j - m_order + 1;

	// 计算 P(t),W(t)
	for (i = m; i <= j; i++)
	{
		W[i - m] = m_weight[i];
		C[i - m] = (m_control[i] - ZeroP3D) * m_weight[i];
	}

	for (r = 1; r <= order - 1; r++)
	{
		for (i = m; i <= j - r; i++)
		{
			fz1 = m_knot[i + order] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + order] - m_knot[i + r];
			W[i - m] = (W[i - m] * fz1 + W[i - m + 1] * fz2) / fm;
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}
	p_vector = C[0];
	w_value = W[0];

	// 计算 P'(t),W'(t)
	order--;
	for (i = m; i <= j; i++)
	{
		W[i - m] = m_weight[i];
		C[i - m] = (m_control[i] - ZeroP3D) * m_weight[i];
	}
	for (i = j; i > m; i--)
	{
		fm = order / (m_knot[i + order] - m_knot[i]);
		W[i - m] = (W[i - m] - W[i - m - 1]) * fm;
		C[i - m] = (C[i - m] - C[i - m - 1]) * fm;
	}

	for (r = 1; r <= order - 1; r++)
	{
		for (i = m + 1; i <= j - r; i++)
		{
			fz1 = m_knot[i + order] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + order] - m_knot[i + r];
			W[i - m] = (W[i - m] * fz1 + W[i - m + 1] * fz2) / fm;
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}
	tp_vector = C[1];
	tw_value = W[1];

	tvector = (tp_vector * w_value - p_vector * tw_value) / (w_value * w_value);

	return tvector;
}

Vector3D C_NurbsCurve3D::ComputDerivative2(double value)
{
	Point3D  p;
	Vector3D d1, d2;
	ComputAll(value, p, d1, d2);
	return d2;
}

void C_NurbsCurve3D::ComputAll(double value, Point3D& position, Vector3D& Derivative1, Vector3D& Derivative2)
{
	int number, order, i, j, k, l;
	number = m_number;
	order = m_order;
	k = order - 1;

	double* alf, * W, wu, w1p, w2p, w2;
	Vector3D* C, pu, p1p, p2p, v;
	// 导数 = (P'*W - P*W')/(W*W) = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
	C = new Vector3D[number];
	W = new double[number];
	alf = new double[number];
	i = GetIntervalIndex(value);
	// 计算 P(t) , W(t)
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	pu = C[i];
	wu = W[i];

	// 计算 P'(t), W'(t)
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (j = i; j >= i - k + 1; j--)
	{
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	}
	k = k - 1;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	p1p = C[i];
	w1p = W[i];

	k = k + 1;
	// 计算 P''(t), W''(t)
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (j = i; j >= i - k + 1; j--)
	{
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	}
	for (j = i; j >= i - k + 2; j--)
	{
		C[j] = (C[j] - C[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
	}
	k = k - 2;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	p2p = C[i];
	w2p = W[i];
	delete C;
	delete W;
	delete alf;
	Vector3D cv2;
	w2 = wu * wu;
	position = ZeroP3D + pu / wu;
	Derivative1 = (p1p * wu - pu * w1p) / w2;
	Derivative2 = (p2p * wu - pu * w2p) * w2 - (p1p * wu - pu * w1p) * 2 * wu * w1p;
	Derivative2 /= w2 * w2;
}

double C_NurbsCurve3D::ComputCurvature(double value)
{
	Point3D  p;
	Vector3D d1, d2;
	ComputAll(value, p, d1, d2);
	double len = Length(d1);
	double curvature = Length(d1 ^ d2) / len / len / len;
	return curvature;
}


void C_NurbsCurve3D::Draw(CDC* pDC)
{
	int      j;
	double   t1, t2, t;
	Point3D  point, newpoint;

	t1 = m_knot[m_order - 1];
	t2 = m_knot[m_number];
	point = ComputPosition(t1);
	for (j = 1; j <= DISNUM; j++)
	{
		t = t1 + j * (t2 - t1) / DISNUM;
		newpoint = ComputPosition(t);
		//pCanvas->DrawLine(pDC, point, newpoint);
		point = newpoint;
	}
}


void C_NurbsCurve3D::InsertKnot(double value)
{
	int i, j;
	double* newknot, * newweight;
	Vector3D* newvect, * P;

	newknot = new double[m_number + m_order + 1];
	newvect = new Vector3D[m_number + 1];
	P = new Vector3D[m_number];
	newweight = new double[m_number + 1];

	for (i = 0; i < m_number; i++)
		P[i] = (m_control[i] - ZeroP3D) * m_weight[i];
	j = GetIntervalIndex(value);    // Value 在第j个节点区间
	// 计算新的节点向量
	for (i = 0; i <= m_number + m_order; i++)
	{
		if (i <= j)
			newknot[i] = m_knot[i];
		else if (i == j + 1)
			newknot[i] = value;
		else
			newknot[i] = m_knot[i - 1];
	}
	// 分子插节点
	for (i = 0; i <= m_number; i++)
	{
		if (i <= j - m_order + 1)
			newvect[i] = P[i];
		else if (i >= j + 1)
			newvect[i] = P[i - 1];
		else
			newvect[i] = P[i - 1] * (m_knot[i + m_order - 1] - value) / (m_knot[i + m_order - 1] - m_knot[i]) +
			P[i] * (value - m_knot[i]) / (m_knot[i + m_order - 1] - m_knot[i]);
	}
	delete P;
	// 分母插节点
	for (i = 0; i <= m_number; i++)
	{
		if (i <= j - m_order + 1)
			newweight[i] = m_weight[i];
		else if (i >= j + 1)
			newweight[i] = m_weight[i - 1];
		else
			newweight[i] = m_weight[i - 1] * (m_knot[i + m_order - 1] - value) / (m_knot[i + m_order - 1] - m_knot[i]) +
			m_weight[i] * (value - m_knot[i]) / (m_knot[i + m_order - 1] - m_knot[i]);
	}
	//构造新的对象
	SetNumberOrder(m_number + 1, m_order);
	for (i = 0; i < m_number + m_order; i++)
		m_knot[i] = newknot[i];
	delete newknot;
	for (i = 0; i < m_number; i++)
	{
		m_control[i] = ZeroP3D + newvect[i] / newweight[i];
		m_weight[i] = newweight[i];
	}
	delete newvect;
	delete newweight;
}

void C_NurbsCurve3D::InsertMultiKnot(int multiplicity, double value)
{
	for (int i = 0; i < multiplicity; i++)
		InsertKnot(value);
}

int C_NurbsCurve3D::InsertKnotSeq(int count, double* knots)
{
	return 0;
}


// 用于集合运算的函数

/*
Point3D C_NurbsCurve3D::GetStartPoint(void) { return Point3D(0,0,0); }
Point3D C_NurbsCurve3D::GetEndPoint(void)  { return Point3D(0,0,0); }
Point3D C_NurbsCurve3D::GetMiddlePoint(void)  { return Point3D(0,0,0); }
Point3D C_NurbsCurve3D::GetParaPoint(double t) { return Point3D(0,0,0); }
BOOL C_NurbsCurve3D::PointOn(Point3D p) { return TRUE; }
*/

double C_NurbsCurve3D::GetPointPara(Point3D p)
{
	int k;
	double value;
	CS_NurbsCurve  curv;
	curv.n = m_number;
	curv.order = m_order;
	curv.knot = new double[curv.n + curv.order];
	curv.ctrl = new Point3D[curv.n];
	curv.weight = new double[curv.n];
	for (k = 0; k < curv.n + curv.order; k++)
		curv.knot[k] = m_knot[k];
	for (k = 0; k < curv.n; k++)
	{
		curv.ctrl[k] = m_control[k];
		curv.weight[k] = m_weight[k];
	}
	// 调用mathlib中的函数
	value = NurbscGetPointPara(curv, p);
	delete curv.knot;
	delete curv.ctrl;
	delete curv.weight;
	return value;
	return 0;
}

// 求点到曲线最短距离
double C_NurbsCurve3D::GetPointDistance(Point3D p)
{
	return GetPointDistance(p, 0);
}
// 求参数曲线曲面的min_max包围盒
CS_Box2d SPL_GetCurvSurfBox2d(int& number, Point2D points[])
{
	int i;
	CS_Box2d  box;
	box.m_min.x = box.m_min.y = 1e+10;
	box.m_max.x = box.m_max.y = -1e+10;
	for (i = 0; i < number; i++)
	{
		if (points[i].x < box.m_min.x)
			box.m_min.x = points[i].x;
		if (points[i].y < box.m_min.y)
			box.m_min.y = points[i].y;
		if (points[i].x > box.m_max.x)
			box.m_max.x = points[i].x;
		if (points[i].y > box.m_max.y)
			box.m_max.y = points[i].y;
	}
	return box;
}
// Bezier曲线高度估计
double SPL_RbezcHeightEstimate(CS_RBezierCurve curve)
{
	int    i;
	double dist, maxdist = 0.0;
	Line3D  line(curve.ctrl[0], curve.ctrl[curve.n - 1]);

	for (i = 1; i < curve.n - 1; i++)
	{
		dist = line.distanceTo(curve.ctrl[i]);
		if (dist > maxdist)
			maxdist = dist;
	}
	return(maxdist);
}
Line3D::Line3D(Point3D start, Point3D end)
{
	m_start = start;
	m_end = end;
}
double Line3D::distanceTo(Point3D pt)
{
	Vector3D v1 = Vector3D(pt.x - m_start.x, pt.y - m_start.y, pt.z - m_start.z);
	Vector3D v2 = Vector3D(m_end.x - m_start.x, m_end.y - m_start.y, m_end.z - m_start.z);
	v2.SetUnit();
	double l = v1 * v2;
	Vector3D v3 = v2 * l;
	return Length(v1 - v3);

}
void SPL_RbezcSplit(CS_RBezierCurve2d curve, double value, CS_RBezierCurve2d& pc1, CS_RBezierCurve2d& pc2)
{
	//为pc1,pc2申请空间
	int number = curve.n;
	pc1.n = number;
	pc1.ctrl = new Point2D[number];
	pc1.weight = new double[number];
	pc2.n = number;
	pc2.ctrl = new Point2D[number];
	pc2.weight = new double[number];

	int     i, j, k;
	double* pp = new double[number];
	// 对控制顶点的三个分量及权因子用de casteljau算法的中间点得到新控制顶点
	pc1.ctrl[0] = curve.ctrl[0];
	pc2.ctrl[number - 1] = curve.ctrl[number - 1];
	pc1.weight[0] = curve.weight[0];
	pc2.weight[number - 1] = curve.weight[number - 1];
	for (k = 1; k <= 4; k++)
	{
		for (j = 0; j < number; j++)
		{
			if (k == 1)
				pp[j] = curve.ctrl[j].x * curve.weight[j];
			else if (k == 2)
				pp[j] = curve.ctrl[j].y * curve.weight[j];
			else if (k == 3)
				pp[j] = curve.weight[j];
			else;
		}
		for (i = 1; i < number; i++)
		{
			for (j = 0; j < number - i; j++)
				pp[j] = (1 - value) * pp[j] + value * pp[j + 1];
			if (k == 1)
			{
				pc1.ctrl[i].x = pp[0];
				pc2.ctrl[number - 1 - i].x = pp[number - 1 - i];
			}
			else if (k == 2)
			{
				pc1.ctrl[i].y = pp[0];
				pc2.ctrl[number - 1 - i].y = pp[number - 1 - i];
			}
			else if (k == 4)
			{
				pc1.weight[i] = pp[0];
				pc2.weight[number - 1 - i] = pp[number - 1 - i];
			}
			else;
		}
	}
	delete pp;
	// 控制顶点与权因子分离   
	for (j = 1; j < number; j++)
	{
		pc1.ctrl[j].x = pc1.ctrl[j].x / pc1.weight[j];
		pc1.ctrl[j].y = pc1.ctrl[j].y / pc1.weight[j];
	}
	for (j = 0; j < number - 1; j++)
	{
		pc2.ctrl[j].x = pc2.ctrl[j].x / pc2.weight[j];
		pc2.ctrl[j].y = pc2.ctrl[j].y / pc2.weight[j];

	}
}
// GRBezier曲线在参数为Value处分割为两段
void SPL_GRBezcSplit(CS_GRBezierCurve2d curve, double value, CS_GRBezierCurve2d& pc1, CS_GRBezierCurve2d& pc2)
{
	SPL_RbezcSplit(curve.rbezier, value, pc1.rbezier, pc2.rbezier);
	pc1.start = curve.start;
	pc1.end = curve.start * (1 - value) + curve.end * value;
	pc2.start = curve.start * (1 - value) + curve.end * value;
	pc2.end = curve.end;
}
Line2D::Line2D(void)
{
}
Line2D::Line2D(Point2D start, Point2D end)
{
	m_start = start;
	m_end = end;
}
double Line2D::distanceTo(Point2D pt)
{
	Vector2D v1 = Vector2D(pt.x - m_start.x, pt.y - m_start.y);
	Vector2D v2 = Vector2D(m_end.x - m_start.x, m_end.y - m_start.y);
	v2.SetUnit();
	double l = v1 * v2;
	Vector2D v3 = v2 * l;
	return Length(v1 - v3);

}
// Bezier曲线高度估计
double SPL_RbezcHeightEstimate(CS_RBezierCurve2d curve)
{
	int    i;
	double dist, maxdist = 0.0;
	Line2D  line(curve.ctrl[0], curve.ctrl[curve.n - 1]);

	for (i = 1; i < curve.n - 1; i++)
	{
		dist = line.distanceTo(curve.ctrl[i]);
		if (dist > maxdist)
			maxdist = dist;
	}
	return(maxdist);
}

// GRBezier曲线在参数为Value处分割为两段
void SPL_GRBezcSplit(CS_GRBezierCurve curve, double value, CS_GRBezierCurve& pc1, CS_GRBezierCurve& pc2)
{
	SPL_RbezcSplit(curve.rbezier, value, pc1.rbezier, pc2.rbezier);
	pc1.start = curve.start;
	pc1.end = curve.start * (1 - value) + curve.end * value;
	pc2.start = curve.start * (1 - value) + curve.end * value;
	pc2.end = curve.end;
}
// GRBezier递归分割
void SPL_GRBezcRecursive(CS_GRBezierCurve curve, double length, Point3D*& vp, double*& t, int& number)
{
	int i;
	if (SPL_RbezcHeightEstimate(curve.rbezier) < length)
	{
		vp = new Point3D[curve.rbezier.n];
		t = new double[curve.rbezier.n];
		for (i = 0; i < curve.rbezier.n - 1; i++)
		{
			vp[i] = curve.rbezier.ctrl[i];
			t[i] = curve.start + (curve.end - curve.start) * i / (curve.rbezier.n - 1);
		}
		number = curve.rbezier.n - 1;
	}
	else
	{
		int  numb1, numb2;
		Point3D* vp1, * vp2;
		double* t1, * t2;
		CS_GRBezierCurve  pc1, pc2;
		SPL_GRBezcSplit(curve, 0.5, pc1, pc2);
		SPL_GRBezcRecursive(pc1, length, vp1, t1, numb1);
		SPL_GRBezcRecursive(pc2, length, vp2, t2, numb2);
		vp = new Point3D[numb1 + numb2 + 1];
		t = new double[numb1 + numb2 + 1];
		for (i = 0; i < numb1; i++)
		{
			vp[i] = vp1[i];
			t[i] = t1[i];
		}
		for (i = 0; i < numb2; i++)
		{
			vp[i + numb1] = vp2[i];
			t[i + numb1] = t2[i];
		}
		number = numb1 + numb2;
		delete vp1;
		delete vp2;
		delete t1;
		delete t2;
		delete pc1.rbezier.ctrl;
		delete pc1.rbezier.weight;
		delete pc2.rbezier.ctrl;
		delete pc2.rbezier.weight;
	}
}
// GRBezier递归分割
void SPL_GRBezcRecursive(CS_GRBezierCurve2d curve, double length, Point2D*& vp, double*& t, int& number)
{
	int i;
	if (SPL_RbezcHeightEstimate(curve.rbezier) < length)
	{
		vp = new Point2D[curve.rbezier.n];
		t = new double[curve.rbezier.n];
		for (i = 0; i < curve.rbezier.n - 1; i++)
		{
			vp[i] = curve.rbezier.ctrl[i];
			t[i] = curve.start + (curve.end - curve.start) * i / (curve.rbezier.n - 1);
		}
		number = curve.rbezier.n - 1;
	}
	else
	{
		int  numb1, numb2;
		Point2D* vp1, * vp2;
		double* t1, * t2;
		CS_GRBezierCurve2d  pc1, pc2;
		SPL_GRBezcSplit(curve, 0.5, pc1, pc2);
		SPL_GRBezcRecursive(pc1, length, vp1, t1, numb1);
		SPL_GRBezcRecursive(pc2, length, vp2, t2, numb2);
		vp = new Point2D[numb1 + numb2 + 1];
		t = new double[numb1 + numb2 + 1];
		for (i = 0; i < numb1; i++)
		{
			vp[i] = vp1[i];
			t[i] = t1[i];
		}
		for (i = 0; i < numb2; i++)
		{
			vp[i + numb1] = vp2[i];
			t[i + numb1] = t2[i];
		}
		number = numb1 + numb2;
		delete vp1;
		delete vp2;
		delete t1;
		delete t2;
		delete pc1.rbezier.ctrl;
		delete pc1.rbezier.weight;
		delete pc2.rbezier.ctrl;
		delete pc2.rbezier.weight;
	}
}
// GRBezier曲线离散为点及相应参数, 
void SPL_GRbezcSubdivision(CS_GRBezierCurve2d curve, double Epsilon, Point2D*& vp, double*& t, int& number)
{
	double   length;
	CS_Box2d   box;
	box = SPL_GetCurvSurfBox2d(curve.rbezier.n, curve.rbezier.ctrl);
	length = Epsilon * Distance(box.m_min, box.m_max);
	//by Chen xiaodiao 2005-09-20
	if (length < 1e-10)
	{
		number = curve.rbezier.n;
		vp = new Point2D[number];
		t = new double[curve.rbezier.n];
		for (int i = 0; i < number; i++) {
			vp[i] = curve.rbezier.ctrl[i];
			t[i] = curve.start + (curve.end - curve.start) * i / (curve.rbezier.n - 1);
		}
		return;
	}
	// 递归分割
	SPL_GRBezcRecursive(curve, length, vp, t, number);
	vp[number] = curve.rbezier.ctrl[curve.rbezier.n - 1];
	t[number] = curve.end;
	number++;
}

// GRBezier曲线离散为点及相应参数, 
void SPL_GRbezcSubdivision(CS_GRBezierCurve curve, double Epsilon, Point3D*& vp, double*& t, int& number)
{
	double   length;
	CS_Box   box;
	box = SPL_GetCurvSurfBox3d(curve.rbezier.n, curve.rbezier.ctrl);
	length = Epsilon * Distance(box.m_min, box.m_max);

	//by Chen xiaodiao 2005-09-20
	if (length < 1e-10)
	{
		number = curve.rbezier.n;
		vp = new Point3D[number];
		t = new double[curve.rbezier.n];
		for (int i = 0; i < number; i++) {
			vp[i] = curve.rbezier.ctrl[i];
			t[i] = curve.start + (curve.end - curve.start) * i / (curve.rbezier.n - 1);
		}
		return;
	}

	// 递归分割
	SPL_GRBezcRecursive(curve, length, vp, t, number);
	vp[number] = curve.rbezier.ctrl[curve.rbezier.n - 1];
	t[number] = curve.end;
	number++;
}
// GRBezier曲线离散为点及相应参数, 
void GRbezcSubdivision(CS_GRBezierCurve curve, double Epsilon, Point3D*& vp, double*& t, int& number)
{
	SPL_GRbezcSubdivision(curve, Epsilon, vp, t, number);
	/*
	double   length;
	CS_Box   box;
	box = GetCurvSurfBox(curve.rbezier.n, curve.rbezier.ctrl);
	length = Epsilon*Distance(box.m_min, box.m_max);
	// 递归分割
	GRBezcRecursive(curve, length, vp, t, number);
	vp[number] = curve.rbezier.ctrl[curve.rbezier.n-1];
	t[number] = curve.end;
	number++;
	*/
}
// Nurbsc曲线离散, 得到离散点及相应参数
void NurbscSubdivisionWithPara(const CS_NurbsCurve& curve, double Epsilon, Point3D*& vp, double*& t, int& number)
{
	// 先将曲线curve转化为Bezier曲线, 
	int numb, order, bcnumber;
	numb = curve.n;
	order = curve.order;
	bcnumber = numb - order + 1; // 曲线最大段数
	//曲线curve转化为Bezier曲线
	double* splitpara;
	CS_RBezierCurve* rcurv = new CS_RBezierCurve[bcnumber];
	bcnumber = NurbscToRBezier(curve, rcurv, splitpara);
	// 调用GRBezier曲线离散
	int k, j;
	Point3D** bezvp;
	bezvp = new Point3D * [bcnumber];
	double** bezt;
	bezt = new double* [bcnumber];
	int* beznumber;
	beznumber = new int[bcnumber];
	CS_GRBezierCurve gcurv;
	gcurv.rbezier.n = order;
	//removed by LiuXiaoming 2007-6-12,avoid unnecessary memory alloca and free
	//gcurv.rbezier.ctrl = new Point3D[order];
	//gcurv.rbezier.weight = new double[order];
	for (k = 0; k < bcnumber; k++)
	{
		gcurv.start = 0;
		gcurv.end = 1;
		//_______________________________
		//revised by LiuXiaoming 2007-6-12
		//for(j = 0; j < order; j++)
		//{
		//    gcurv.rbezier.ctrl[j] = rcurv[k].ctrl[j];
		//    gcurv.rbezier.weight[j] = rcurv[k].weight[j];
		//}
		gcurv.rbezier.ctrl = rcurv[k].ctrl;
		gcurv.rbezier.weight = rcurv[k].weight;
		//_______________________________


		// --------------------------------------------------------------
		// liuyushen 2002/11/28 防止曲线退回成点造成死机
		// --------------------------------------------------------------
		for (j = 1; j < order; j++)
		{
			if (gcurv.rbezier.ctrl[j] != rcurv[k].ctrl[0])
				break;
		}
		if (j < order)
			GRbezcSubdivision(gcurv, Epsilon, bezvp[k], bezt[k], beznumber[k]);
		else
		{
			bezvp[k] = NULL;
			bezt[k] = NULL;
			beznumber[k] = NULL;
		}
		// --------------------------------------------------------------
//        GRbezcSubdivision(gcurv, Epsilon, bezvp[k], bezt[k], beznumber[k]); 
	}
	// 填写vp t;
	number = 0;
	for (k = 0; k < bcnumber; k++)
		number += beznumber[k];
	vp = new Point3D[number];
	t = new double[number];
	int index = 0;
	for (k = 0; k < bcnumber; k++)
	{
		if (bcnumber == 1)
		{
			for (j = 0; j < beznumber[0] - 1; j++)
			{
				vp[index] = bezvp[0][j];
				t[index] = bezt[0][j];
				index++;
			}
		}
		else
			for (j = 0; j < beznumber[k] - 1; j++)
			{
				vp[index] = bezvp[k][j];
				if (k == 0)
					t[index] = splitpara[0] * bezt[k][j];
				else if (k == bcnumber - 1)
					t[index] = splitpara[k - 1] + (1 - splitpara[k - 1]) * bezt[k][j];
				else
					t[index] = splitpara[k - 1] + (splitpara[k] - splitpara[k - 1]) * bezt[k][j];
				index++;
			}
	}
	/////////////// ??????????????? //////////////////////////
	// Modified by Jun-Hai Yong: 2002-12-18
	// Modification reason: Program down!!!
	if (bezvp != NULL && bezvp[bcnumber - 1] != NULL)
	{
		k = beznumber[bcnumber - 1] - 1;
		if (k < 0)
			k = 0;
		vp[index] = bezvp[bcnumber - 1][k];
		t[index] = 1.0;
		index++;
	}
	// Modified by Jun-Hai Yong: 2002-12-18
	/////////////// ??????????????? //////////////////////////
	// Before modification.
	// vp[index] = bezvp[bcnumber-1][beznumber[bcnumber-1] -1];
	for (k = 0; k < bcnumber; k++)
	{
		delete[] rcurv[k].ctrl;
		delete[] rcurv[k].weight;
		delete[] bezvp[k];
		delete[] bezt[k];
	}
	delete[] rcurv;
	delete[] bezvp;
	delete[] bezt;
	//removed by LiuXiaoming 2007-6-12
   // delete [] gcurv.rbezier.ctrl;
  // delete [] gcurv.rbezier.weight;
	delete[] beznumber;
	delete[] splitpara;
	number = index;
}

// 取得NURBS曲面的BSpl曲面表示（不重新复制数据结构）用于临时计算
void PutNurbsCToBspl(const CS_NurbsCurve& ncurv, CS_BSplineCurve& bcurv)
{
	bcurv.ctrl = ncurv.ctrl;
	bcurv.knot = ncurv.knot;
	bcurv.n = ncurv.n;
	bcurv.order = ncurv.order;
}



Vector3D BscurvDerivative1(CS_BSplineCurve curve, double value)
{
	int			i, r, j, m, order;
	double		fz1, fz2, fm;
	//-------------------------------------------------
	//Yupiqiang 2003/07/21,改为动态数组，防止越界
	Vector3D* C;

	C = new Vector3D[curve.order];
	//-------------------------------------------------
	order = curve.order - 1;
	j = BscurvIntervalIndex(curve, value);
	m = j - curve.order + 1;

	for (i = j; i > m; i--)
		C[i - m] = (curve.ctrl[i] - curve.ctrl[i - 1]) * order / (curve.knot[i + order] - curve.knot[i]);

	for (r = 1; r <= order - 1; r++)
	{
		for (i = m + 1; i <= j - r; i++)
		{
			fz1 = curve.knot[i + order] - value;
			fz2 = value - curve.knot[i + r];
			fm = curve.knot[i + order] - curve.knot[i + r];
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}
	//-----------------------------------------------------------------
	//return C[1];
	Vector3D vec = C[1];
	delete[]C;
	return vec;
	//-----------------------------------------------------------------
/*
	int i,j,r,order,number;
	Vector3D  *C, tvector;

	order  = curve.order -1;
	number = curve.n;
	C = new Vector3D[number];
	j = BscurvIntervalIndex(curve, value);

	for( i = 1; i <= number - 1; i++ )
		C[i] = (curve.ctrl[i] - curve.ctrl[i-1])*order/(curve.knot[i+order] - curve.knot[i]);

	for( r = 1; r <= order -1; r++)
	{
		for( i = j - order + 1; i <= j - r; i++ )
			C[i] = C[i]*(curve.knot[i+order] - value)/(curve.knot[i+order] - curve.knot[i+r]) +
				   C[i+1]*(value - curve.knot[i+r])/(curve.knot[i+order] - curve.knot[i+r]);
	}
	tvector = C[j-order+1];
	delete 	C;
	return  tvector;
*/
}
// 计算BSpline曲线上参数为Value处的值和导数
void BscurvComputAll(CS_BSplineCurve curve, double value, Point3D& iterp, Vector3D& axis)
{
	iterp = BscurvPosition(curve, value);
	axis = BscurvDerivative1(curve, value);
}
// 计算Nurbs曲线上参数为Value处的值、一阶导数值
void NurbscComputAll(CS_NurbsCurve& curve, double value, Point3D& position, Vector3D& cv1)
{

	if (CS_NurbsCIsBspl(curve))
	{
		CS_BSplineCurve bcurv;
		PutNurbsCToBspl(curve, bcurv);
		BscurvComputAll(bcurv, value, position, cv1);
		return;
	}
	int number, order, i, j, k, l, m;
	number = curve.n;
	order = curve.order;
	k = order - 1;

	double* alf, * W, * W1, wu, w1p, w2;
	Vector3D* C, * D, pu, p1p, v;
	// 导数 = (P'*W - P*W')/(W*W) = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
	//C = new Vector3D[number];
	//W = new double[number];
	//alf = new double[number];
	//revised by LiuXiaoming, 2007-6-7,减少空间使用,使用栈内存提高速度
	//C = new Vector3D[order];
	C = new Vector3D[sizeof(Vector3D) * order * 2];
	D = C + order;

	W = new double[sizeof(double) * order * 3];
	W1 = W + order;
	alf = W1 + order;
	//W = new double[order];
	//alf = new double[order];
	i = NurbscIntervalIndex(curve, value);
	m = i - order + 1;
	// 计算 P(t) , W(t)
	for (j = i - k; j <= i; j++)
	{
		C[j - m] = (curve.ctrl[j] - ZeroP3D) * curve.weight[j];
		W[j - m] = curve.weight[j];
	}
	//保存中间结果,避免重复计算,LiuXiaoming 2007-6-7
	memcpy(D, C, sizeof(Vector3D) * order);
	memcpy(W1, W, sizeof(double) * order);

	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j - m] = (value - curve.knot[j]) / (curve.knot[j + k + 1 - l] - curve.knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j - m] = C[j - 1 - m] * (1 - alf[j - m]) + C[j - m] * alf[j - m];
			W[j - m] = W[j - 1 - m] * (1 - alf[j - m]) + W[j - m] * alf[j - m];
		}
	}
	pu = C[i - m];
	wu = W[i - m];

	// 计算 P'(t), W'(t) 
/*    for( j = i-k; j <= i; j++ )
	{
		C[j-m] = (curve.ctrl[j] - ZeroP3D)*curve.weight[j];
		W[j-m] = curve.weight[j];
	}*/
	for (j = i; j >= i - k + 1; j--)
	{
		D[j - m] = (D[j - m] - D[j - 1 - m]) / (curve.knot[j + k] - curve.knot[j]);
		W1[j - m] = (W1[j - m] - W1[j - 1 - m]) / (curve.knot[j + k] - curve.knot[j]);
	}
	k = k - 1;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j - m] = (value - curve.knot[j]) / (curve.knot[j + k + 1 - l] - curve.knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			D[j - m] = D[j - 1 - m] * (1 - alf[j - m]) + D[j - m] * alf[j - m];
			W1[j - m] = W1[j - 1 - m] * (1 - alf[j - m]) + W1[j - m] * alf[j - m];
		}
	}
	/*    p1p = D[i-m];
		w1p = W1[i-m]; */
	p1p = D[i - m] * (k + 1);
	w1p = W1[i - m] * (k + 1);
	//减少乘法次数

	delete[]C;
	delete[]W;
	//delete []alf;
	w2 = wu * wu;
	position = ZeroP3D + pu / wu;
	cv1 = (p1p * wu - pu * w1p) / w2;
}


// 求点到Nurbsc曲线最短距离,并返回参数
double NurbscGetPointDistance(CS_NurbsCurve curve, Point3D p, double& para)
{
	// 带参数离散
	int numb;
	double* t;
	Point3D* poin;
	NurbscSubdivisionWithPara(curve, 0.05, poin, t, numb);
	// 求初始参数
	int i, index;
	// Modified by  Yong (Mark begins) 981215
	// Old begins	
	//    double mindist = 19680910;
	// Old ends
	// New begins
	if (numb > 0)
	{
		double mindist = Distance(p, poin[0]);
		index = 0;
		// New ends.
		// Modified by  Yong (Mark ends) 981215
		for (i = 0; i < numb; i++)
			if (Distance(p, poin[i]) < mindist)
			{
				mindist = Distance(p, poin[i]);
				index = i;
			}
		para = t[index];
	}
	else
	{
		para = 0;
	}

	// 迭代修正
	int      iter_num;
	Point3D  projp, iterp;
	Vector3D axis;
	double   dt;
	// 几何迭代
	iter_num = 0;

	while (iter_num < ITERAT_NUM)
	{
		NurbscComputAll(curve, para, iterp, axis);
		projp = PointProjectToLine(p, iterp, axis);
		dt = ((projp - iterp) * axis) / (axis * axis);
		para += dt;
		if (para > curve.knot[curve.n])           para = curve.knot[curve.n];
		if (para < curve.knot[curve.order - 1])     para = curve.knot[curve.order - 1];
		if (fabs(dt) < TOLER)
			break;
		iter_num++;
	}
	if (iter_num == ITERAT_NUM)
		para = t[index];
	delete[] t;
	delete[] poin;
	return Distance(p, NurbscPosition(curve, para));
}
// 求点到曲线最短距离, 并返回最近点参数
double C_NurbsCurve3D::GetPointDistance(Point3D p, double* para)
{
	int k;
	double value;
	CS_NurbsCurve  curv;
	curv.n = m_number;
	curv.order = m_order;
	curv.knot = new double[curv.n + curv.order];
	curv.ctrl = new Point3D[curv.n];
	curv.weight = new double[curv.n];
	for (k = 0; k < curv.n + curv.order; k++)
		curv.knot[k] = m_knot[k];
	for (k = 0; k < curv.n; k++)
	{
		curv.ctrl[k] = m_control[k];
		curv.weight[k] = m_weight[k];
	}
	// 调用mathlib中的函数
	double parameter;
	value = NurbscGetPointDistance(curv, p, parameter);
	if (para != NULL)
		*para = parameter;
	//FreeNurbsCurve(curv);
	return value;
}

BOOL C_NurbsCurve3D::PointOn(Point3D p, double toler)
{
	Point3D temp;
	return PointOn(p, temp, toler);
}

BOOL C_NurbsCurve3D::PointOn(Point3D p, Point3D& q, double toler)
{
	double dist, para;
	dist = GetPointDistance(p, &para);
	if (dist < toler)
	{
		q = ComputPosition(para);
		return TRUE;
	}
	else
		return FALSE;
}

C_Curve3D* C_NurbsCurve3D::Copy(void)
{
	return new C_NurbsCurve3D(m_number, m_order, m_control, m_knot, m_weight);
}

void C_NurbsCurve3D::Expand(int flag, double length)
{
	/*  int k;
	  double value;
	  CS_NurbsCurve  curv;
	  curv.n = m_number;
	  curv.order = m_order;
	  curv.knot = new double[curv.n+curv.order];
	  curv.ctrl = new Point3D[curv.n];
	  curv.weight = new double[curv.n];
	  for( k = 0; k < curv.n + curv.order; k++ )
		  curv.knot[k] = m_knot[k];
	  for( k = 0; k < curv.n; k++)
	  {
		  curv.ctrl[k] = m_control[k];
		  curv.weight[k] = m_weight[k];
	  }
	  // 调用mathlib中的函数
	  NurbscExtend(curv, flag, length);
	  SetNumberOrder(curv.n,curv.order);
	  for( k = 0; k < m_number + m_order; k++ )
		  SetKnot(k, curv.knot[k]);
	  for( k = 0; k < m_number; k++)
	  {
		  SetControlPoint(k, curv.ctrl[k]);
		  SetWeight(k, curv.weight[k]);
	  }
	  delete curv.knot;
	  delete curv.weight;
	  delete curv.ctrl;*/

}

C_Curve3D* C_NurbsCurve3D::Reverse(void)
{
	int i, j;
	double  weight, knot, sum;
	Point3D pp;
	i = 0;
	j = m_number - 1;
	while (i < j)
	{
		pp = m_control[i];
		m_control[i] = m_control[j];
		m_control[j] = pp;
		weight = m_weight[i];
		m_weight[i] = m_weight[j];
		m_weight[j] = weight;
		i++;
		j--;
	}
	sum = m_knot[0] + m_knot[m_order + m_number - 1];
	i = 0;
	j = m_order + m_number - 1;
	while (i < j)
	{
		knot = sum - m_knot[i];
		m_knot[i] = sum - m_knot[j];
		m_knot[j] = knot;
		i++;
		j--;
	}
	if (i == j)
		m_knot[i] = sum - m_knot[i];
	return this;
}

void C_NurbsCurve3D::SplitD(double t, C_Curve3D** pC1, C_Curve3D** pC2)
{
	int i, j, r, number1, number2;
	C_NurbsCurve3D* curve = (C_NurbsCurve3D*)Copy();
	t = m_knot[m_order - 1] + (m_knot[m_number] - m_knot[m_order - 1]) * t;
	//j = curve->GetIntervalIndex(t);
	for (j = m_number + m_order - 1; t < m_knot[j] + TOLER; j--);
	r = curve->GetKnotMulti(t);
	curve->InsertMultiKnot(m_order - r, t);
	number1 = j + 1;
	number2 = m_number + m_order - r - j - 1;
	C_NurbsCurve3D* c1, * c2;
	c1 = new C_NurbsCurve3D(number1, m_order);
	c2 = new C_NurbsCurve3D(number2, m_order);
	for (i = 0; i <= j; i++)
		c1->SetKnot(i, m_knot[i]);
	for (i = j + 1; i <= number1 + m_order - 1; i++)
		c1->SetKnot(i, t);
	for (i = 0; i < number1; i++)
	{
		c1->SetControlPoint(i, curve->m_control[i]);
		c1->SetWeight(i, curve->m_weight[i]);
	}
	for (i = number2 + m_order - 1; i >= m_order; i--)
		c2->SetKnot(i, m_knot[i + m_number - number2]);
	for (i = m_order - 1; i >= 0; i--)
		c2->SetKnot(i, t);
	for (i = number2 - 1; i >= 0; i--)
	{
		c2->SetControlPoint(i, curve->m_control[i + curve->m_number - number2]);
		c2->SetWeight(i, curve->m_weight[i + curve->m_number - number2]);
	}

	// 对节点向量重新整理，规划为0、1之间
	double length = c1->m_knot[c1->m_number] - c1->m_knot[c1->m_order - 1];
	for (i = c1->m_order; i < c1->m_number; i++)
		c1->SetKnot(i, (c1->m_knot[i] - c1->m_knot[c1->m_order - 1]) / length);
	for (i = 0; i < c1->m_order; i++)
		c1->SetKnot(i, 0);
	for (i = c1->m_number; i < c1->m_number + c1->m_order; i++)
		c1->SetKnot(i, 1);
	length = c2->m_knot[c2->m_number] - c2->m_knot[c2->m_order - 1];
	for (i = c2->m_order; i < c2->m_number; i++)
		c2->SetKnot(i, (c2->m_knot[i] - c2->m_knot[c2->m_order - 1]) / length);
	for (i = 0; i < c2->m_order; i++)
		c2->SetKnot(i, 0);
	for (i = c2->m_number; i < c2->m_number + c2->m_order; i++)
		c2->SetKnot(i, 1);

	*pC1 = c1;
	*pC2 = c2;
	delete curve;
}

void C_NurbsCurve3D::SplitP(Point3D& p, C_Curve3D** pC1, C_Curve3D** pC2)
{
	double value = GetPointPara(p);
	SplitD(value, pC1, pC2);
}
void C_NurbsCurve3D::glDraw()
{
	int i;
	double start, end;
	Point3D p1, p2;
	::glBegin(GL_LINE_STRIP);
	glLineWidth(3.0);
	int m_turn = 0;

	for (i = 0; i <= 200; i++)
	{
		start = i * 0.005;
		//end = (i + 1)* 0.005;

		p1 = ComputPosition(start);
		//p2 = ComputPosition(end);

		::glVertex3f(p1.x, p1.y, p1.z);
		//::glVertex3f(p2.x,p2.y,p2.z);
	}
	::glEnd();

}
void C_NurbsCurve3D::SplitN(int n, double t[], Point3D p[], C_Curve3D* newcurve[])
{
	C_Curve3D* tm1, * tm2;
	SplitD(t[0], &newcurve[0], &tm1);
	for (int i = 1; i < n; i++)
	{
		((C_NurbsCurve3D*)tm1)->SplitD((t[i] - t[i - 1]) / (1 - t[i - 1]), &newcurve[i], &tm2);
		delete tm1;
		tm1 = tm2;
	}
	newcurve[n] = tm1;
}

Vector3D C_NurbsCurve3D::GetParaTangent(double t)  // 0<t<1
{
	t = m_knot[m_order - 1] + t * (m_knot[m_number] - m_knot[m_order - 1]);
	Vector3D vector;
	vector = ComputDerivative1(t);
	vector.SetUnit();
	return vector;
}




void C_NurbsCurve3D::Serialize(CArchive& ar)
{
	C_NonuniformBCurve3D::Serialize(ar);

	if (ar.IsStoring())
	{
		for (int i = 0; i < m_number; i++)
			ar << m_weight[i];
	}
	else
	{
		m_weight = new double[m_number];
		for (int i = 0; i < m_number; i++)
			ar >> m_weight[i];
	}
}

void C_NurbsCurve3D::ComputLocal(double value1, Point3D& position, Vector3D& tangent, Vector3D& normal,
	Vector3D& binormal, double& curvature)
{
	double value;
	if (value1 > 1)
		value = 1;
	else if (value1 < 0)
		value = 0;
	else
		value = value1;

	int number, order, i, j, k, l;
	number = m_number;
	order = m_order;
	k = order - 1;

	double* alf, * W, wu, w1p, w2p, w2;
	Vector3D* C, pu, p1p, p2p, v;
	// 导数 = (P'*W - P*W')/(W*W) = (tp_vector*w_value - p_vector*tw_value)/(w_value*w_value)
	C = new Vector3D[number];
	W = new double[number];
	alf = new double[number];
	i = GetIntervalIndex(value);
	// 计算 P(t) , W(t)
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	pu = C[i];
	wu = W[i];

	// 计算 P'(t), W'(t) 
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (j = i; j >= i - k + 1; j--)
	{
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	}
	k = k - 1;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	p1p = C[i];
	w1p = W[i];

	k = k + 1;
	// 计算 P''(t), W''(t) 
	for (j = i - k; j <= i; j++)
	{
		C[j] = (m_control[j] - ZeroP3D) * m_weight[j];
		W[j] = m_weight[j];
	}
	for (j = i; j >= i - k + 1; j--)
	{
		C[j] = (C[j] - C[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * k / (m_knot[j + k] - m_knot[j]);
	}
	for (j = i; j >= i - k + 2; j--)
	{
		C[j] = (C[j] - C[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
		W[j] = (W[j] - W[j - 1]) * (k - 1) / (m_knot[j + k - 1] - m_knot[j]);
	}
	k = k - 2;
	for (l = 1; l <= k; l++)
	{
		for (j = i - k + l; j <= i; j++)
			alf[j] = (value - m_knot[j]) / (m_knot[j + k + 1 - l] - m_knot[j]);
		for (j = i; j >= i - k + l; j--)
		{
			C[j] = C[j - 1] * (1 - alf[j]) + C[j] * alf[j];
			W[j] = W[j - 1] * (1 - alf[j]) + W[j] * alf[j];
		}
	}
	p2p = C[i];
	w2p = W[i];
	delete C;
	delete W;
	delete alf;
	Vector3D cv1, cv2;
	w2 = wu * wu;
	position = ZeroP3D + pu / wu;
	cv1 = (p1p * wu - pu * w1p) / w2;
	cv2 = (p2p * wu - pu * w2p) * w2 - (p1p * wu - pu * w1p) * 2 * wu * w1p;
	cv2 /= w2 * w2;
	double len1 = Length(cv1);
	curvature = Length(cv1 ^ cv2) / len1 / len1 / len1;
	binormal = cv1 ^ cv2;
	normal = binormal ^ cv1;
	tangent = cv1;
	if (Length(normal) > TOLER)
		normal.SetUnit();
	if (Length(binormal) > TOLER)
		binormal.SetUnit();
	if (fabs(value - value1) > TOLER)
	{
		position += cv1 * (value1 - value);
		curvature = 0;
	}

}
C_RationalBezierCurve3D::C_RationalBezierCurve3D(void)
{
}

C_RationalBezierCurve3D::C_RationalBezierCurve3D(int number) :
	C_BezierCurve3D(number)
{
	m_weight = new double[number];
}


C_RationalBezierCurve3D::C_RationalBezierCurve3D(int number, Point3D* control, double* weight) :
	C_BezierCurve3D(number, control)
{
	m_weight = new double[number];
	for (int i = 0; i < number; i++)
		m_weight[i] = weight[i];
}


C_RationalBezierCurve3D::~C_RationalBezierCurve3D(void)
{
}


void C_RationalBezierCurve3D::SetWeight(int index, double value)
{
	m_weight[index] = value;
}

double C_RationalBezierCurve3D::GetWeight(int index)
{
	return m_weight[index];
}


C_Curve3D* C_RationalBezierCurve3D::Copy(void)
{
	return new C_RationalBezierCurve3D(m_number, m_control, m_weight);
}


int C_RationalBezierCurve3D::PrintInformation(void)
{
	return 0;
}



void C_RationalBezierCurve3D::Serialize(class CArchive& ar)
{
	C_BezierCurve3D::Serialize(ar);

	if (ar.IsStoring())
	{
		for (int i = 0; i < m_number; i++)
			ar << m_weight[i];
	}
	else
	{
		m_weight = new double[m_number];
		for (int i = 0; i < m_number; i++)
			ar >> m_weight[i];
	}
}
C_BezierCurve3D::C_BezierCurve3D(void)
{
}



C_BezierCurve3D::C_BezierCurve3D(int number) :
	C_ParaCurve3D(number)
{

}


C_BezierCurve3D::C_BezierCurve3D(int number, Point3D* control) :
	C_ParaCurve3D(number, control)
{

}



C_Curve3D* C_BezierCurve3D::Copy(void)
{
	return new C_BezierCurve3D(m_number, m_control);
}



// 计算曲线上一点的值
Point3D C_BezierCurve3D::ComputPosition(double value)
{
	int     i, j, k;
	double* pp = new double[m_number];
	Point3D poin;
	for (k = 1; k < m_number; k++)
	{
		for (j = 0; j < m_number; j++)
		{
			if (k == 1)
				pp[j] = m_control[j].x;
			else if (k == 2)
				pp[j] = m_control[j].y;
			else if (k == 3)
				pp[j] = m_control[j].z;
			else;
		}
		for (i = 1; i < m_number; i++)
			for (j = 0; j < m_number - i; j++)
				pp[j] = (1 - value) * pp[j] + value * pp[j + 1];
		if (k == 1)
			poin.x = pp[0];
		else if (k == 2)
			poin.y = pp[0];
		else if (k == 3)
			poin.z = pp[0];
		else;
	}
	delete pp;
	return poin;
}
Point3D C_RationalBezierCurve3D::ComputPosition(double value)
{
	int			i, r, j, m;
	//--------------------------------------------------
	//Yu Piqiang 2004-04-06
	//double		W[4], fz1, fz2, fm;
	//Vector3D	C[4];
	double		fz1, fz2, fm;
	double* W = new double[m_number];
	Vector3D* C = new Vector3D[m_number];
	//---------------------------------------------------
	double m_knot[50];
	for (i = 0; i < m_number; i++)
	{
		m_knot[i] = 0;
		m_knot[i + m_number] = 1;
	}
	j = m_number - 1;
	m = j - m_number + 1;

	for (i = m; i <= j; i++)
	{
		W[i - m] = m_weight[i];
		C[i - m] = (m_control[i] - ZeroP3D) * m_weight[i];
	}

	for (r = 1; r <= m_number - 1; r++)
	{
		for (i = m; i <= j - r; i++)
		{
			fz1 = m_knot[i + m_number] - value;
			fz2 = value - m_knot[i + r];
			fm = m_knot[i + m_number] - m_knot[i + r];
			if (fm == 0)
				fm = 1;
			W[i - m] = (W[i - m] * fz1 + W[i - m + 1] * fz2) / fm;
			C[i - m] = (C[i - m] * fz1 + C[i - m + 1] * fz2) / fm;
		}
	}

	//-------------------------------------
	//return ZeroP3D + C[0]/W[0];
	Vector3D vect = C[0] / W[0];
	delete[]C;
	delete[]W;
	return vect;
	//--------------------------------------
}
struct __declspec(dllexport) CS_BezierCurve {
	int n;
	Point3D* ctrl;
	CS_BezierCurve() { ; }
};
void SPL_BezcSplit(CS_BezierCurve curve, double value, CS_BezierCurve& pc1, CS_BezierCurve& pc2)
{
	//为pc1,pc2申请空间
	int number = curve.n;
	pc1.n = number;
	pc1.ctrl = new Point3D[number];
	pc2.n = number;
	pc2.ctrl = new Point3D[number];

	int     i, j, k;
	double* pp, ppp[100];
	if (number > 100)
		pp = new double[number];
	else
		pp = ppp;
	// 对控制顶点的三个分量用de casteljau算法的中间点得到新控制顶点
	pc1.ctrl[0] = curve.ctrl[0];
	pc2.ctrl[number - 1] = curve.ctrl[number - 1];
	for (k = 1; k <= 3; k++)
	{
		for (j = 0; j < number; j++)
		{
			if (k == 1)
				pp[j] = curve.ctrl[j].x;
			else if (k == 2)
				pp[j] = curve.ctrl[j].y;
			else if (k == 3)
				pp[j] = curve.ctrl[j].z;
			else;
		}
		for (i = 1; i < number; i++)
		{
			for (j = 0; j < number - i; j++)
				pp[j] = (1 - value) * pp[j] + value * pp[j + 1];
			if (k == 1)
			{
				pc1.ctrl[i].x = pp[0];
				pc2.ctrl[number - 1 - i].x = pp[number - 1 - i];
			}
			else if (k == 2)
			{
				pc1.ctrl[i].y = pp[0];
				pc2.ctrl[number - 1 - i].y = pp[number - 1 - i];
			}
			else if (k == 3)
			{
				pc1.ctrl[i].z = pp[0];
				pc2.ctrl[number - 1 - i].z = pp[number - 1 - i];
			}
			else;
		}
	}
	if (number > 100)
		delete pp;
}
void BezcSplit(CS_BezierCurve curve, double value, CS_BezierCurve& pc1, CS_BezierCurve& pc2)
{
	SPL_BezcSplit(curve, value, pc1, pc2);
	/*
	//为pc1,pc2申请空间
	int number = curve.n;
	pc1.n = number;
	pc1.ctrl = new Point3D[number];
	pc2.n = number;
	pc2.ctrl = new Point3D[number];

	int     i,j,k;
	double  *pp, ppp[100];
	if (number > 100)
		pp = new double[number];
	else
		pp = ppp;
	// 对控制顶点的三个分量用de casteljau算法的中间点得到新控制顶点
	pc1.ctrl[0] = curve.ctrl[0];
	pc2.ctrl[number-1] = curve.ctrl[number-1];
	for( k = 1 ; k <= 3; k++ )
	{
		for( j = 0 ; j < number ; j++ )
		{
			if( k == 1 )
			pp[j] = curve.ctrl[j].x;
			else if (k == 2)
			pp[j] = curve.ctrl[j].y;
			else if (k == 3)
			pp[j] = curve.ctrl[j].z;
			else;
		}
		for( i = 1 ; i < number; i++ )
		{
			for( j = 0 ; j < number -i; j++ )
				pp[j] = (1-value)*pp[j] + value*pp[j+1];
			if( k == 1 )
			{
				pc1.ctrl[i].x = pp[0];
				pc2.ctrl[number-1-i].x = pp[number-1-i];
			}
			else if (k == 2)
			{
				pc1.ctrl[i].y = pp[0];
				pc2.ctrl[number-1-i].y = pp[number-1-i];
			}
			else if (k == 3)
			{
				pc1.ctrl[i].z = pp[0];
				pc2.ctrl[number-1-i].z = pp[number-1-i];
			}
			else;
		}
	}
	if (number > 100)
		delete pp;
	*/

}
// 将曲线分割为两条曲线
void C_BezierCurve3D::SplitD(double value, C_BezierCurve3D& pc1, C_BezierCurve3D& pc2)
{
	int i;
	CS_BezierCurve curv, bez1, bez2;
	curv.n = m_number;
	curv.ctrl = new Point3D[curv.n];
	for (i = 0; i < m_number; i++)
		curv.ctrl[i] = m_control[i];
	BezcSplit(curv, value, bez1, bez2);
	pc1.SetCtrlNumber(bez1.n);
	pc2.SetCtrlNumber(bez2.n);
	for (i = 0; i < pc1.m_number; i++)
		pc1.SetControlPoint(i, bez1.ctrl[i]);
	for (i = 0; i < pc2.m_number; i++)
		pc2.SetControlPoint(i, bez2.ctrl[i]);
	delete bez1.ctrl;
	delete bez2.ctrl;
}


// 将曲线分割为两条曲线
void C_BezierCurve3D::SplitP(Point3D point, C_BezierCurve3D& pc1, C_BezierCurve3D& pc2)
{
	Point3D control[4];
	double knot[8];
	double weight[4];
	int i;
	for (i = 0; i < 4; i++)
	{
		control[i] = GetControlPoint(i);
		knot[i] = 0;
		knot[i + 4] = 1;
		weight[i] = 1;
	}

	C_NurbsCurve3D* pNurbs = new C_NurbsCurve3D(4, 4, control, knot, weight);
	double value = pNurbs->GetPointPara(point);

	CS_BezierCurve curv, bez1, bez2;
	curv.n = m_number;
	curv.ctrl = new Point3D[curv.n];
	for (i = 0; i < m_number; i++)
		curv.ctrl[i] = m_control[i];
	BezcSplit(curv, value, bez1, bez2);
	pc1.SetCtrlNumber(bez1.n);
	pc2.SetCtrlNumber(bez2.n);
	for (i = 0; i < pc1.m_number; i++)
		pc1.SetControlPoint(i, bez1.ctrl[i]);
	for (i = 0; i < pc2.m_number; i++)
		pc2.SetControlPoint(i, bez2.ctrl[i]);
	delete bez1.ctrl;
	delete bez2.ctrl;
}
// 将曲线离散为直线段，Epsilon为高度估计的误差，vp点列，number为点个数
void C_BezierCurve3D::Subdivision(double Epsilon, Point3D*& vp, int& number)
{
	/* int i;
	 CS_BezierCurve curv;
	 curv.n = m_number;
	 curv.ctrl = new Point3D[curv.n];
	 for(i = 0; i < m_number; i++)
		 curv.ctrl[i] = m_control[i];
	 BezcSubdivision(curv, Epsilon, vp, number);
	 delete curv.ctrl;*/
}

// Bezier曲线离散为点及相应参数 
void C_BezierCurve3D::SubdivisionWithPara(double Epsilon, Point3D*& vp, double*& t, int& number)
{
	/*  int i;
	  CS_GBezierCurve curv;
	  curv.bezier.n = m_number;
	  curv.start = 0;
	  curv.end = 1;
	  curv.bezier.ctrl = new Point3D[m_number];
	  for(i = 0; i < m_number; i++)
		  curv.bezier.ctrl[i] = m_control[i];
	  GBezcSubdivision(curv, Epsilon, vp, t, number);
	  delete curv.bezier.ctrl;*/
}


int C_BezierCurve3D::PrintInformation(void)
{
	return 0;
}



void C_BezierCurve3D::Serialize(class CArchive& ar)
{
	C_ParaCurve3D::Serialize(ar);

	if (ar.IsStoring())
	{
	}
	else
	{
	}
}
// 1. 直线:  非法 return 1                  
int IllegalLine(CS_Line& line)
{
	if (Length(line.vn) < TOLER)
		return 1;
	else
		return 0;
}
//直线与直线求交  (魏开敏：12)
int LineLineInters(CS_Line ln1, CS_Line ln2, Point3D& pi)
{
	if (IllegalLine(ln1) || IllegalLine(ln2))
		return -1;

	ln1.vn.SetUnit();
	ln2.vn.SetUnit();
	Vector3D pp = ln2.pa - ln1.pa;
	if (fabs(pp * (ln1.vn ^ ln2.vn)) > CS_EPS)
		return 0;//异面直线
	else
	{
		if (Length(ln1.vn ^ ln2.vn) <= TOLER)//平行
			if (Length(ln1.vn ^ pp) > CS_EPS)
				return 0;//平行不重合
			else
				return 2;//重合                 
		else//相交不重合
		{
			double t, Dxy, Dyz, Dzx;
			Dxy = ln1.vn.x * ln2.vn.y - ln2.vn.x * ln1.vn.y;
			Dyz = ln1.vn.y * ln2.vn.z - ln2.vn.y * ln1.vn.z;
			Dzx = ln1.vn.z * ln2.vn.x - ln2.vn.z * ln1.vn.x;
			if (fabs(Dxy) > fabs(Dyz))
			{
				if (fabs(Dxy) > fabs(Dzx))    // |Dxy| is max
					t = (pp.x * ln2.vn.y - pp.y * ln2.vn.x) / Dxy;
				else                            // |Dzx| is max
					t = (pp.z * ln2.vn.x - pp.x * ln2.vn.z) / Dzx;
			}
			else
			{
				if (fabs(Dyz) > fabs(Dzx))    // |Dyz| is max
					t = (pp.y * ln2.vn.z - pp.z * ln2.vn.y) / Dyz;
				else                            // |Dzx| is max
					t = (pp.z * ln2.vn.x - pp.x * ln2.vn.z) / Dzx;
			}
			pi = ln1.pa + ln1.vn * t;
			return 1;
		}
	}
}
// 三次B样条曲线插值: 节点和端切向由用户指定
int BscurvInterpolat(int number, Point3D* point, Vector3D vs,
	Vector3D ve, double* knot, CS_BSplineCurve& rcurv)
{
	int i, j, k;
	if (number == 1)
	{
		rcurv.n = 4;
		rcurv.order = 4;
		rcurv.knot = new double[8];
		rcurv.ctrl = new Point3D[4];
		for (k = 0; k < 4; k++)
			rcurv.ctrl[k] = point[0];
		for (k = 0; k < 4; k++)
			rcurv.knot[k] = 0;
		for (k = 4; k < 8; k++)
			rcurv.knot[k] = 1;
		return 1;
	}
	else if (number == 2)
	{
		rcurv.n = 4;
		rcurv.order = 4;
		rcurv.knot = new double[8];
		rcurv.ctrl = new Point3D[4];
		for (k = 0; k < 4; k++)
			rcurv.knot[k] = 0;
		for (k = 4; k < 8; k++)
			rcurv.knot[k] = 1;
		Vector3D vect = point[1] - point[0];
		rcurv.ctrl[0] = point[0];
		rcurv.ctrl[1] = point[0] + vect / 3.0;
		rcurv.ctrl[2] = point[0] + vect * 2.0 / 3.0;
		rcurv.ctrl[3] = point[1];
		return 1;
	}
	else
	{
		rcurv.n = number + 2;
		rcurv.order = 4;
		rcurv.knot = new double[rcurv.n + rcurv.order];
		rcurv.ctrl = new Point3D[rcurv.n];
		// 节点向量赋值
		for (k = 0; k < rcurv.order + rcurv.n; k++)
			rcurv.knot[k] = knot[k];

		// 反求B样条曲线的控制顶点     
		double** cm, * row;
		cm = new PTRDOUBLE[rcurv.n];
		for (i = 0; i < rcurv.n; i++)
			cm[i] = new double[rcurv.n];
		// 求系数矩阵, 与边界条件相关的首尾两行另算        
		row = new double[rcurv.n];
		for (i = 1; i < rcurv.n - 1; i++)
		{
			BscurvBaseValu(rcurv, rcurv.knot[i + 2], row);
			for (j = 0; j < rcurv.n; j++)
			{
				cm[i][j] = row[j];
			}
		}
		delete row;
		// 求系数矩阵中与边界条件相关的首尾两行        
		for (j = 0; j < rcurv.n; j++)
		{
			cm[0][j] = 0;
			cm[rcurv.n - 1][j] = 0;
		}
		cm[0][0] = -1;  cm[0][1] = 1;
		cm[rcurv.n - 1][rcurv.n - 1] = -1;  cm[rcurv.n - 1][rcurv.n - 2] = 1;

		// 调用EquatSystemPoint(...)，反求控制顶点
		Point3D* cy;
		cy = new Point3D[rcurv.n];
		for (k = 0; k < number; k++)
			cy[k + 1] = point[k];
		cy[0] = ZeroP3D + vs;
		cy[number + 1] = ZeroP3D - ve;
		//        cy[0] = ZeroP3D + vs*(knot[4] - knot[3])/3.0;
		//		cy[number+1] = ZeroP3D - ve*(knot[number+2] - knot[number+1])/3.0;
		int flag = EquatSystemPoint(rcurv.n, cm, cy, rcurv.ctrl);
		for (i = 0; i < rcurv.n; i++)
			delete cm[i];
		delete cm;
		delete cy;
		if (flag == 0)    return 0;
		else             return 1;
	}
}
//高斯消元法解方程
int EquatSystemNumber(int np, double** aa, double* bb, double* x)
{
	int i, j, k;
	double** a;
	int* z;
	double* xx;
	z = new int[np];
	xx = new double[np];

	a = new double* [np];
	for (i = 0; i < np; i++)
		a[i] = new double[np + 1];

	for (i = 0; i < np; i++)
		z[i] = i;

	for (i = 0; i < np; i++)
		for (j = 0; j < np; j++)
			a[i][j] = aa[i][j];
	for (i = 0; i < np; i++)
		a[i][np] = bb[i];

	for (k = 0; k < np; k++)
	{
		guassSelect(a, z, k, np);

		for (i = k + 1; i < np; i++)
		{
			double l = a[i][k] / a[k][k];
			for (j = k + 1; j <= np; j++)
				a[i][j] -= l * a[k][j];
		}
	}

	for (i = 0; i < np; i++)
		xx[i] = 0;

	if (fabs(a[np - 1][np - 1]) < TOLER)
		return 0;

	for (i = np - 1; i >= 0; i--)
	{
		for (j = i + 1; j < np; j++)
			a[i][np] -= a[i][j] * xx[j];

		xx[i] = a[i][np] / a[i][i];
	}
	for (i = 0; i < np; i++)
	{
		x[z[i]] = xx[i];
	}

	for (i = 0; i < np; i++)
		delete[] a[i];
	delete[]a;
	delete[]xx;
	delete[]z;
	return 1;
}
// 三次Nurbs曲线插值
int NurbscInterpolat(int number, Point3D* point, double* knot, double* weight,
	Vector3D vs, Vector3D ve, CS_NurbsCurve& rcurv)
{
	int i, j, k;
	if (number == 1)
	{
		rcurv.n = 4;
		rcurv.order = 4;
		rcurv.knot = new double[8];
		rcurv.weight = new double[4];
		rcurv.ctrl = new Point3D[4];
		for (k = 0; k < 4; k++)
			rcurv.ctrl[k] = point[0];
		for (k = 0; k < 4; k++)
			rcurv.weight[k] = weight[0];
		for (k = 0; k < 4; k++)
			rcurv.knot[k] = 0;
		for (k = 4; k < 8; k++)
			rcurv.knot[k] = 1;
		return 1;
	}
	else if (number == 2)
	{
		rcurv.n = 4;
		rcurv.order = 4;
		rcurv.knot = new double[8];
		rcurv.weight = new double[4];
		rcurv.ctrl = new Point3D[4];
		for (k = 0; k < 4; k++)
			rcurv.knot[k] = 0;
		for (k = 4; k < 8; k++)
			rcurv.knot[k] = 1;
		rcurv.ctrl[0] = point[0];
		rcurv.ctrl[1] = point[0] + vs / 3.0;
		rcurv.ctrl[2] = point[1] - ve / 3.0;
		rcurv.ctrl[3] = point[1];
		rcurv.weight[0] = weight[0];
		rcurv.weight[1] = weight[0] * 0.66666666666 + weight[1] * 0.333333333333;
		rcurv.weight[2] = weight[0] * .333333333333 + weight[1] * 0.666666666666;
		rcurv.weight[3] = weight[1];
		return 1;
	}
	else
	{
		int flag;
		rcurv.n = number + 2;
		rcurv.order = 4;
		rcurv.knot = new double[rcurv.n + rcurv.order];
		rcurv.weight = new double[rcurv.n];
		rcurv.ctrl = new Point3D[rcurv.n];
		// 节点向量赋值
		for (k = 0; k < rcurv.order + rcurv.n; k++)
			rcurv.knot[k] = knot[k];

		// 反求Nurbs曲线的控制顶点及权因子     
		double** cm1, ** cm2, * row;
		cm1 = new PTRDOUBLE[rcurv.n];
		cm2 = new PTRDOUBLE[rcurv.n];
		for (i = 0; i < rcurv.n; i++)
		{
			cm1[i] = new double[rcurv.n];
			cm2[i] = new double[rcurv.n];
		}
		// 求系数矩阵, 与边界条件相关的首尾两行另算        
		row = new double[rcurv.n];
		CS_BSplineCurve rcurve;
		rcurve.n = rcurv.n;
		rcurve.order = rcurv.order;
		rcurve.knot = new double[rcurve.n + rcurve.order];
		rcurve.ctrl = new Point3D[rcurve.n];
		for (k = 0; k < rcurve.n + rcurve.order; k++)
			rcurve.knot[k] = rcurv.knot[k];
		for (i = 1; i < rcurv.n - 1; i++)
		{
			BscurvBaseValu(rcurve, rcurve.knot[i + 2], row);
			for (j = 0; j < rcurv.n; j++)
			{
				cm1[i][j] = row[j];
				cm2[i][j] = row[j];
			}
		}
		delete rcurve.knot;
		delete rcurve.ctrl;
		delete row;
		// 求系数矩阵中与边界条件相关的首尾两行        
		for (j = 0; j < rcurv.n; j++)
		{
			cm1[0][j] = 0;
			cm1[rcurv.n - 1][j] = 0;
			cm2[0][j] = 0;
			cm2[rcurv.n - 1][j] = 0;
		}
		cm1[0][0] = -1;  cm1[0][1] = 1;
		cm1[rcurv.n - 1][rcurv.n - 1] = -1;  cm1[rcurv.n - 1][rcurv.n - 2] = 1;
		cm2[0][0] = -1;  cm2[0][1] = 1;
		cm2[rcurv.n - 1][rcurv.n - 1] = -1;  cm2[rcurv.n - 1][rcurv.n - 2] = 1;

		// 调用EquatSystemPoint(...)，反求W_i*P_i
		Point3D* cy;
		cy = new Point3D[rcurv.n];
		for (k = 0; k < number; k++)
			cy[k + 1] = ZeroP3D + (point[k] - ZeroP3D) * weight[k];
		cy[0] = ZeroP3D + vs; //*(Distance(point[0],point[1]) + Distance(point[1],point[2]))/5.0;
		cy[number + 1] = ZeroP3D - ve; // *(Distance(point[number-1],point[number-2]) + Distance(point[number-2],point[number-3]))/5.0;
		flag = EquatSystemPoint(rcurv.n, cm1, cy, rcurv.ctrl);

		// 调用EquatSystemNumber(...)，反求W_i
		double* ny;
		ny = new double[rcurv.n];
		for (k = 0; k < number; k++)
			ny[k + 1] = weight[k];
		ny[0] = weight[1] - weight[0];
		ny[number + 1] = weight[number - 1] - weight[number - 2];
		flag = EquatSystemNumber(rcurv.n, cm2, ny, rcurv.weight);

		// 有理化，(W_i*P_i)/W_i得到P_i
		for (i = 0; i < rcurv.n; i++)
			rcurv.ctrl[i] = ZeroP3D + (rcurv.ctrl[i] - ZeroP3D) / rcurv.weight[i];

		for (i = 0; i < rcurv.n; i++)
		{
			delete cm1[i];
			delete cm2[i];
		}
		delete cm1;
		delete cm2;
		delete cy;
		delete ny;

		if (flag == 0)
			return 0;
		else
			return 1;
	}
}
// 求B样条基函数u处的值, 
void BscurvBaseValu(CS_BSplineCurve curve, double u, double* coef)
{
	int number, order;
	number = curve.n;
	order = curve.order;
	int i, j, k;
	double** b;
	b = new PTRDOUBLE[number + 1];
	for (i = 0; i < number + 1; i++)
		b[i] = new double[number + 1];
	for (i = 0; i < number; i++)
		for (j = 0; j < number; j++)
			b[i][j] = 0;

	j = BscurvIntervalIndex(curve, u);
	b[1][j] = 1;
	double  ca, cb;
	for (k = 2; k <= order; k++)
		for (i = j - k + 1; i <= j; i++)
		{
			if (fabs(curve.knot[i + k - 1] - curve.knot[i]) > 1e-5)
				ca = (u - curve.knot[i]) / (curve.knot[i + k - 1] - curve.knot[i]);
			else
				ca = 0;
			if (fabs(curve.knot[i + k] - curve.knot[i + 1]) > 1e-5)
				cb = (curve.knot[i + k] - u) / (curve.knot[i + k] - curve.knot[i + 1]);
			else
				cb = 0;
			b[k][i] = b[k - 1][i] * ca + b[k - 1][i + 1] * cb;
		}
	for (i = 0; i < number; i++)
		coef[i] = b[curve.order][i];
	for (i = 0; i < number + 1; i++)
		delete[] b[i];
	delete[] b;
	/*  //只对四阶以下有效
		int number, order;
		number = curve.n;
		order = curve.order;
		int i,j,k;
		double **b;
		b = new PTRDOUBLE[number];
		for(i = 0;i < number; i++)
			b[i] = new double[number];
		for(i = 0; i < number; i++)
		for(j = 0; j < number; j++)
			b[i][j] = 0;

		j = BscurvIntervalIndex(curve, u);
		b[1][j] = 1;
		double  ca,cb;
		for(k = 2; k < 5; k++)
		for(i = j-k+1; i <= j; i++)
		{
			if(fabs(curve.knot[i+k-1]-curve.knot[i]) > 1e-5)
				ca = (u-curve.knot[i])/(curve.knot[i+k-1]-curve.knot[i]);
			else
				ca = 0;
			if(fabs(curve.knot[i+k]-curve.knot[i+1]) > 1e-5)
				cb = (curve.knot[i+k] - u)/(curve.knot[i+k]-curve.knot[i+1]);
			else
				cb = 0;
			b[k][i] = b[k-1][i]*ca + b[k-1][i+1]*cb;
		}
		for(i = 0; i < number; i++)
			coef[i] = b[4][i];
		for(i = 0;i < number; i++)
			delete [] b[i];
		delete [] b;
	*/
}

//高斯消元法中选主元素的程序
void guassSelect(double**& a, int*& z, int k, int np)
{
	int xi = 0, yj = 0;
	double tmp = 0;
	int i;
	for (i = k; i < np; i++)
	{
		for (int j = k; j < np; j++)
		{
			if (fabs(a[i][j]) > fabs(tmp))
			{
				tmp = a[i][j];
				xi = i;
				yj = j;
			}
		}
	}
	if (xi != k)
	{
		for (int i = k; i < np + 1; i++)
		{
			tmp = a[k][i];
			a[k][i] = a[xi][i];
			a[xi][i] = tmp;
		}
	}
	if (yj != k)
	{
		for (int i = 0; i < np; i++)
		{
			tmp = a[i][k];
			a[i][k] = a[i][yj];
			a[i][yj] = tmp;
		}
		i = z[k];
		z[k] = z[yj];
		z[yj] = i;
	}
}
//高斯消元法解方程
int EquatSystemPoint(int np, double** aa, Point3D* bb, Point3D* x)
{
	int i, j, k;
	double** a;
	int* z;
	double* xx;
	z = new int[np];
	xx = new double[np];

	a = new double* [np];
	for (i = 0; i < np; i++)
		a[i] = new double[np + 1];

	//计算x值
	for (i = 0; i < np; i++)
		z[i] = i;

	for (i = 0; i < np; i++)
		for (j = 0; j < np; j++)
			a[i][j] = aa[i][j];
	for (i = 0; i < np; i++)
		a[i][np] = bb[i].x;

	for (k = 0; k < np; k++)
	{
		guassSelect(a, z, k, np);

		for (i = k + 1; i < np; i++)
		{
			double l = a[i][k] / a[k][k];
			for (j = k + 1; j <= np; j++)
				a[i][j] -= l * a[k][j];
		}
	}

	for (i = 0; i < np; i++)
		xx[i] = 0;

	if (fabs(a[np - 1][np - 1]) < TOLER)
		return 0;
	for (i = np - 1; i >= 0; i--)
	{
		for (j = i + 1; j < np; j++)
			a[i][np] -= a[i][j] * xx[j];

		xx[i] = a[i][np] / a[i][i];
	}
	for (i = 0; i < np; i++)
	{
		x[z[i]].x = xx[i];
	}

	//计算y值
	for (i = 0; i < np; i++)
		z[i] = i;

	for (i = 0; i < np; i++)
		for (j = 0; j < np; j++)
			a[i][j] = aa[i][j];
	for (i = 0; i < np; i++)
		a[i][np] = bb[i].y;

	for (k = 0; k < np; k++)
	{
		guassSelect(a, z, k, np);

		for (i = k + 1; i < np; i++)
		{
			double l = a[i][k] / a[k][k];
			for (j = k + 1; j <= np; j++)
				a[i][j] -= l * a[k][j];
		}
	}

	for (i = 0; i < np; i++)
		xx[i] = 0;

	if (fabs(a[np - 1][np - 1]) < TOLER)
		return 0;
	for (i = np - 1; i >= 0; i--)
	{
		for (j = i + 1; j < np; j++)
			a[i][np] -= a[i][j] * xx[j];

		xx[i] = a[i][np] / a[i][i];
	}
	for (i = 0; i < np; i++)
	{
		x[z[i]].y = xx[i];
	}

	//计算z值
	for (i = 0; i < np; i++)
		z[i] = i;

	for (i = 0; i < np; i++)
		for (j = 0; j < np; j++)
			a[i][j] = aa[i][j];
	for (i = 0; i < np; i++)
		a[i][np] = bb[i].z;

	for (k = 0; k < np; k++)
	{
		guassSelect(a, z, k, np);

		for (i = k + 1; i < np; i++)
		{
			double l = a[i][k] / a[k][k];
			for (j = k + 1; j <= np; j++)
				a[i][j] -= l * a[k][j];
		}
	}

	for (i = 0; i < np; i++)
		xx[i] = 0;

	if (fabs(a[np - 1][np - 1]) < TOLER)
		return 0;
	for (i = np - 1; i >= 0; i--)
	{
		for (j = i + 1; j < np; j++)
			a[i][np] -= a[i][j] * xx[j];

		xx[i] = a[i][np] / a[i][i];
	}
	for (i = 0; i < np; i++)
	{
		x[z[i]].z = xx[i];
	}

	for (i = 0; i < np; i++)
		delete[] a[i];
	delete[]a;
	delete[]xx;
	delete[]z;
	return 1;
}
C_ParaCurve3D::C_ParaCurve3D(void)
{
	m_control = NULL;
}


C_ParaCurve3D::C_ParaCurve3D(int number)
{
	m_number = number;
	m_control = new Point3D[number];
}


C_ParaCurve3D::C_ParaCurve3D(int number, Point3D* control)
{
	m_number = number;
	m_control = new Point3D[number];
	for (int i = 0; i < number; i++)
		m_control[i] = control[i];
}



C_ParaCurve3D::~C_ParaCurve3D(void) // 释放m_control的内存
{
	delete m_control;
}



// 用于数据存取和显示的成员函数
int C_ParaCurve3D::GetCtrlNumber(void)
{
	return m_number;
}



Point3D C_ParaCurve3D::GetControlPoint(int index)
{
	return  m_control[index];
}


void C_ParaCurve3D::SetCtrlNumber(int number)
{
	m_number = number;
	delete m_control;
	m_control = new Point3D[number];
}



void C_ParaCurve3D::SetControlPoint(int index, Point3D point)
{
	m_control[index] = point;
}




// 用于集合运算的函数
Point3D C_ParaCurve3D::GetStartPoint(void) { return Point3D(0, 0, 0); }
Point3D C_ParaCurve3D::GetEndPoint(void) { return Point3D(0, 0, 0); }
Point3D C_ParaCurve3D::GetMiddlePoint(void) { return Point3D(0, 0, 0); }
Point3D C_ParaCurve3D::GetParaPoint(double t) { return Point3D(0, 0, 0); }
double C_ParaCurve3D::GetPointPara(Point3D p) { return 0; }
double C_ParaCurve3D::GetPointDistance(Point3D p) { return 0; }
BOOL C_ParaCurve3D::PointOn(Point3D p, double toler) { return TRUE; }
C_Curve3D* C_ParaCurve3D::Copy(void) { return this; }
C_Curve3D* C_ParaCurve3D::Reverse(void) { return this; }

void C_ParaCurve3D::SplitD(double t, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_ParaCurve3D::SplitP(Point3D& p, C_Curve3D** pC1, C_Curve3D** pC2) { }
void C_ParaCurve3D::SplitN(int n, double t[], Point3D p[], C_Curve3D* newcurve[]) { }

Vector3D C_ParaCurve3D::GetParaTangent(double t)
{
	return Vector3D(0, 0, 0);
}



void C_ParaCurve3D::Serialize(CArchive& ar)
{
	/*C_Curve3D::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_number;
		for (int i = 0; i < m_number; i++)
			ar << m_control[i];
	}
	else
	{
		ar >> m_number;
		m_control = new Point3D[m_number];
		for (int i = 0; i < m_number; i++)
			ar >> m_control[i];
	}*/
}
