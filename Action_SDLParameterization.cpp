#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "FormView3.h"
#include <algorithm>
#include "lib3d/Base3d.h"
#include "Nurbs3D.h"
#include "MyTreeView.h"
#include "Disline2D.h"
#include "NurbsCurve.h"
#include "MyTreeview.h"
#include "Shape_Polygon.h"
#include "ReferenceLine.h"
#include <iostream>

using namespace std;
#include "Action_SDLParameterization.h"

IMPLEMENT_DYNAMIC(Action_SDLParameterization, CPropertyPage)

Action_SDLParameterization::Action_SDLParameterization()
	: CPropertyPage(Action_SDLParameterization::IDD)
{
	referenceratio = 0;
}

Action_SDLParameterization::~Action_SDLParameterization()
{
}

void Action_SDLParameterization::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERREFERENCERATIO, m_RefereceRatioSlider);
}

BEGIN_MESSAGE_MAP(Action_SDLParameterization, CPropertyPage)
	ON_WM_HSCROLL()
	//增加按钮，将按钮与功能关联起来
	ON_BN_CLICKED(IDC_BUTTON_BezierExample1, &Action_SDLParameterization::LoadBezierExample1)
	ON_BN_CLICKED(ID_OPENMESH_CALCULATE_SDL_COEFFICIENTS, &Action_SDLParameterization::CalculateSDLCoefficient)
	ON_BN_CLICKED(ID_OPENMESH_CALCULATE_OPTIMAL_SDL, &Action_SDLParameterization::CalculateOptimalSDL)
	ON_BN_CLICKED(ID_OPENMESH_CALCULATE_eOPTIMAL_SDL, &Action_SDLParameterization::CalculateEpsilonOptimalSDL)
END_MESSAGE_MAP()

extern CFormView3* p_FormView3;

BOOL Action_SDLParameterization::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	m_RefereceRatioSlider.SetRange(0, 1000);
	m_RefereceRatioSlider.SetPos(0);

	((CEdit*)GetDlgItem(IDC_EDIT_SDL_MINRATIO))->SetWindowTextA("1.0");
	((CEdit*)GetDlgItem(IDC_EDIT_SDL_MAXRATIO))->SetWindowTextA("1.1");

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

extern CMyTreeView* pTreeView;

void Action_SDLParameterization::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	if (pSlider == &m_RefereceRatioSlider)
	{
		double scale = m_RefereceRatioSlider.GetPos();
		referenceratio = scale / 1000.0;
		CalculateSDLCoefficient();
	}
	else
		p_FormView3->UpdateSliderPosition();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

//void Action_SDLParameterization::InitializeConsole()
//{
//	AllocConsole();
//	FILE* fp;
//	freopen_s(&fp, "CONOUT$", "w", stdout);
//	freopen_s(&fp, "CONOUT$", "w", stderr);
//}


//**************给定一条二次贝塞尔曲线，把它分为n段，计算每一段的系数mnc**************实现如下：

// 定义二次贝塞尔曲线函数
Eigen::Vector2d bezierPoint(double t, const Eigen::Vector2d& P0, const Eigen::Vector2d& P1, const Eigen::Vector2d& P2, double w0, double w1, double w2) {
	double u = 1 - t;
	double denom = u * u * w0 + 2 * u * t * w1 + t * t * w2;
	return (u * u * w0 * P0 + 2 * u * t * w1 * P1 + t * t * w2 * P2) / denom;
}

// 定义二次贝塞尔曲线函数的一阶导
Eigen::Vector2d dbezierPoint(double t, const Eigen::Vector2d& P0, const Eigen::Vector2d& P1, const Eigen::Vector2d& P2, double w0, double w1, double w2) {
	double u = 1 - t;
	double ddenom = w0 * u * u + t * t * w2 - 2 * t * w1 * u;
	return -(2 * w1 * P1 * u - w0 * P0 * 2 * u + 2 * t * w1 * P1 - 2 * t * w2 * P2) / ddenom - ((t * t * w2 * P2 + w0 * P0 * u * u - 2 * t * w1 * P1 * u) * (w0 * 2 * u - 2 * t * w1 + 2 * t * w2 - 2 * w1 * u)) / (ddenom * ddenom);
}

// 求解线性方程组
bool solveLinearSystem(const Eigen::Matrix3d& A, const Eigen::Vector3d& b, Eigen::Vector3d& x) {
	x = A.colPivHouseholderQr().solve(b);

	return true;
}

// 函数来计算在p0和p1的线段上距离p0为m/n处的点
Point2D getPointOnLineSegment(const Point2D& p0, const Point2D& p1, double m, double n) {
	Point2D result;
	double t = abs(m) / (abs(m) + abs(n));
	result.x = p0.x + t * (p1.x - p0.x);
	result.y = p0.y + t * (p1.y - p0.y);
	return result;
}

// 函数来画垂直于线段的虚线（这里只是示例，实际绘图需要图形库支持）
/*void drawPerpendicularLine(CNurbsCurve* pNurbsCurve, const Point2D& midPoint, const Point2D& p0, const Point2D& p1, double length) {
	// 计算垂直方向向量
	double dx = p1.x - p0.x;
	double dy = p1.y - p0.y;
	double perpendicular_dx = -dy;
	double perpendicular_dy = dx;

	// 规范化垂直方向向量的长度
	double norm = std::sqrt(perpendicular_dx * perpendicular_dx + perpendicular_dy * perpendicular_dy);
	perpendicular_dx /= norm;
	perpendicular_dy /= norm;

	// 根据指定的长度计算垂直线段的两个端点
	Point2D end1 = { midPoint.x + perpendicular_dx * length / 2, midPoint.y + perpendicular_dy * length / 2 };
	Point2D end2 = { midPoint.x - perpendicular_dx * length / 2, midPoint.y - perpendicular_dy * length / 2 };

	Point3D point1(end1.x, end1.y, 0);
	Point3D point2(end2.x, end2.y, 0);

	ReferenceLine* pLine = new ReferenceLine();
	pLine->start = point1;
	pLine->end = point2;
	pNurbsCurve->m_ArrayReferenceLines.push_back(pLine);
	// 输出结果（实际应用中可以替换为绘图函数）
	std::cout << "Perpendicular line endpoints: (" << end1.x << ", " << end1.y << ") and (" << end2.x << ", " << end2.y << ")\n";
}*/
void Action_SDLParameterization::LoadBezierExample1()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnOpenDocument("D:\\papers\\code\\data\\rationalbezier.cur");
	pDoc->UpdateAllViews(NULL);
}
double Action_SDLParameterization::DotProductUnitofInput(Vector3D v1, Vector3D v2)
{
	//return the dot product of the unit vector of the two input vectors
	v1.SetUnit();
	v2.SetUnit();
	return v1 * v2;
}
double Action_SDLParameterization::SearchForEndPoint(C_NurbsCurve3D* pCurve, Point3D startpoint, Point3D middlepoint, Vector3D middletangent, double startpara, double middlepara)
{
	//
	double endpara = middlepara * 2 - startpara;
	Point3D endpoint = pCurve->ComputPosition(endpara);

	//超过曲线的参数范围，直接返回
	if (endpara > 1.0)
		return endpara;


	double cosangle1 = DotProductUnitofInput(middlepoint - startpoint, middletangent);
	double cosangle2 = DotProductUnitofInput(endpoint - middlepoint, middletangent);

	double angle1 = acos(cosangle1);
	double angle2 = acos(cosangle2);

	int iternum = 0;
	while (fabs(cosangle1 - cosangle2) > 1e-8)
	{
		endpara = middlepara + (endpara - middlepara) * angle1 / angle2;
		if (endpara > 1)
		{
			endpara = 1;
			endpoint = pCurve->ComputPosition(endpara);

			cosangle1 = DotProductUnitofInput(middlepoint - startpoint, middletangent);
			cosangle2 = DotProductUnitofInput(endpoint - middlepoint, middletangent);

			angle1 = acos(cosangle1);
			angle2 = acos(cosangle2);
			if (angle2 < angle1)
				return 1.0;
		}
		else
		{
			endpoint = pCurve->ComputPosition(endpara);

			cosangle1 = DotProductUnitofInput(middlepoint - startpoint, middletangent);
			cosangle2 = DotProductUnitofInput(endpoint - middlepoint, middletangent);

			angle1 = acos(cosangle1);
			angle2 = acos(cosangle2);
		}

		iternum++;
	}
	return endpara;
}
extern char Post_Message[1000];
void Action_SDLParameterization::CalculateEpsilonOptimalSDL()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	if (pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() != TYPE_NURBSCURVE3D)
		return;

	CNurbsCurve* pNurbsCurve = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	pNurbsCurve->m_ArrayReferenceLines.clear();
	pNurbsCurve->m_ArraySegmentParameters.clear();


	//1. First split the curve into segments
	pNurbsCurve->m_ArraySegmentParameters.push_back(0);
	pNurbsCurve->m_ArraySegmentParameters.push_back(0.2);
	pNurbsCurve->m_ArraySegmentParameters.push_back(0.5);
	pNurbsCurve->m_ArraySegmentParameters.push_back(0.6);
	pNurbsCurve->m_ArraySegmentParameters.push_back(1);

	int nSegments = pNurbsCurve->m_ArraySegmentParameters.size() - 1;

	std::vector<Vector3D> arrayFirstPartialDerivatives;
	std::vector<Point3D> arrayPoints;
	//2. compute the partial derivatives of curve at the split points
	for (int i = 0; i < pNurbsCurve->m_ArraySegmentParameters.size(); i++)
	{
		Vector3D parDrivative = pNurbsCurve->pCurve->ComputDerivative1(pNurbsCurve->m_ArraySegmentParameters[i]);
		parDrivative.SetUnit();
		arrayFirstPartialDerivatives.push_back(parDrivative);

		Point3D p = pNurbsCurve->pCurve->ComputPosition(pNurbsCurve->m_ArraySegmentParameters[i]);
		arrayPoints.push_back(p);
	}

	//3. Compute the directional vector of the segments
	std::vector<Vector3D> arraySegmentDir;
	for (int i = 0; i < pNurbsCurve->m_ArraySegmentParameters.size() - 1; i++)
	{
		Point3D v1 = pNurbsCurve->pCurve->ComputPosition(pNurbsCurve->m_ArraySegmentParameters[i]);
		Point3D v2 = pNurbsCurve->pCurve->ComputPosition(pNurbsCurve->m_ArraySegmentParameters[i + 1]);

		Vector3D dir = v2 - v1;
		dir.SetUnit();
		arraySegmentDir.push_back(dir);
	}

	//4. Compute the angle between first partial derivative and the segments
	std::vector<double> angleDiff;
	for (int i = 0; i < pNurbsCurve->m_ArraySegmentParameters.size() - 2; i++)
	{
		Vector3D v1 = arrayFirstPartialDerivatives[i + 1];
		Vector3D v2 = arraySegmentDir[i];
		Vector3D v3 = arraySegmentDir[i + 1];
		double epsilon = acos(v1 * v2);
		double theta = acos(v1 * v3);

		angleDiff.push_back(theta - epsilon);
	}

	//
	std::vector<double> angleReferenceLinetoPerdicularLine;
	int m = (nSegments) * 2 - 1;

	double** A = new double* [m];
	for (int i = 0; i < m; i++)
		A[i] = new double[m];

	double* b = new double[m];

	//初始化
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			A[i][j] = 0;


	//
	for (int i = 0; i < nSegments; i++)
	{
		A[i][i] = 2;
		if (i < nSegments - 1)
			A[i][nSegments + i] = 1;
		if (i > 0)
			A[i][nSegments + i - 1] = -1;

		//for (int j = 0; j < m; j++)
		//	TRACE(" %f ", A[i][j]);
		//TRACE("\n");
		b[i] = 0;
	}

	//A[0][0] = 2;
	//A[0][4] = 1;
	//b[0] = 0;

	//A[1][1] = 2;
	//A[1][4] = -1;
	//A[1][5] = 1;
	//b[1] = 0;

	//A[2][2] = 2;
	//A[2][5] = -1;
	//A[2][6] = 1;
	//b[2] = 0;

	//A[3][3] = 2;
	//A[3][6] = -1;
	//b[3] = 0;

	//

	for (int i = 0; i < nSegments - 1; i++)
	{
		A[nSegments + i][i] = 1;
		A[nSegments + i][i + 1] = -1;
		b[nSegments + i] = angleDiff[i];
	}

	//A[4][0] = 1;
	//A[4][1] = -1;
	//b[4] = angleDiff[0];

	//A[5][1] = 1;
	//A[5][2] = -1;
	//b[5] = angleDiff[1];

	//A[6][2] = 1;
	//A[6][3] = -1;
	//b[6] = angleDiff[2];

	Eigen::MatrixXd A1 = Eigen::MatrixXd::Zero(m, m);

	Eigen::VectorXd b1 = Eigen::VectorXd::Zero(m);
	Eigen::VectorXd X;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
			A1(i, j) = A[i][j];

		b1[i] = b[i];
	}

	X = A1.colPivHouseholderQr().solve(b1);

	std::vector<double> angleArray;
	for (int i = 0; i < nSegments; i++)
		angleArray.push_back(X(i) * 180 / PI);

	for (int i = 0; i < nSegments; i++)
	{
		ReferenceLine* pLine = new ReferenceLine();
		pLine->middlepoint = (arrayPoints[i] + arrayPoints[i + 1]) / 2;
		Vector3D dir = arrayPoints[i + 1] - arrayPoints[i];
		double len = Length(dir);
		dir.SetUnit();
		Vector3D dirPerdicular(dir.y, -dir.x, 0);
		Vector3D Xdir(1, 0, 0);
		Vector3D Ydir(0, 1, 0);
		double angle = acos(dirPerdicular * Xdir);
		double sinanglebetweendirpendicularandydir = dirPerdicular * Ydir;
		if (sinanglebetweendirpendicularandydir < 0)
			angle = PI * 2 - angle;
		double acturalangle = angle + X(i);
		//double acturalangle = angle;
		double cosangle = cos(acturalangle);
		double sinangle = sin(acturalangle);
		pLine->start.x = pLine->middlepoint.x + cosangle * len;
		pLine->start.y = pLine->middlepoint.y + sinangle * len;

		pLine->end.x = pLine->middlepoint.x - cosangle * len;
		pLine->end.y = pLine->middlepoint.y - sinangle * len;

		pLine->m = sinangle;
		pLine->n = -cosangle;

		pNurbsCurve->m_ArrayReferenceLines.push_back(pLine);
	}


	//colorbar 显示

	//获取整个曲线的长度
	const int curveDiscreteNumber = 100;
	double S = 0;
	double I = 0;
	for (int i = 0; i < curveDiscreteNumber - 1; i++)
	{
		double t1 = (double)i / curveDiscreteNumber;
		double t2 = (double)(i + 1) / curveDiscreteNumber;
		Point3D p1 = pNurbsCurve->pCurve->ComputPosition(t1);
		Point3D p2 = pNurbsCurve->pCurve->ComputPosition(t2);
		double dis = Distance(p1, p2);
		S += dis;
	}

	std::vector<double> theSegmentInterval(nSegments);
	//获取整个曲线参数区间的长度
	for (int i = 0; i < nSegments; i++)
	{
		Vector3D dir = arrayPoints[i + 1] - arrayPoints[i];

		Vector3D refvec;//偏移后reference line的切向
		refvec[0] = pNurbsCurve->m_ArrayReferenceLines[i]->m;
		refvec[1] = pNurbsCurve->m_ArrayReferenceLines[i]->n;
		refvec[2] = 0;

		//double myInterval = fabs(refvec * dir);
		double myInterval = fabs(refvec.x * dir.x + refvec.y * dir.y);
		theSegmentInterval[i] = myInterval;  // 将I值保存到数组中

		I += myInterval;
	}

	double OptimalRatio = S / I;

	CString minRatioString;
	CString maxRatioString;

	((CEdit*)GetDlgItem(IDC_EDIT_SDL_MINRATIO))->GetWindowTextA(minRatioString);
	((CEdit*)GetDlgItem(IDC_EDIT_SDL_MAXRATIO))->GetWindowTextA(maxRatioString);

	double minRatio = atof(minRatioString.GetBuffer());
	double maxRatio = atof(maxRatioString.GetBuffer());

	//
	pNurbsCurve->arrayCurveTessellationColorBar_SDL.resize(pNurbsCurve->CURVE_DISCRETE_NUM);

	CImage m_ColorbarImage;
	m_ColorbarImage.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP6));

	//3. 计算曲线上每个点单位切向的的放缩倍数alphaalpha
	std::vector<double> scaleRatio(pNurbsCurve->CURVE_DISCRETE_NUM);

	for (int i = 0; i < pNurbsCurve->CURVE_DISCRETE_NUM; i++)
	{
		double t = (double)i / pNurbsCurve->CURVE_DISCRETE_NUM;

		//find t located segment
		int segmentindex = 0;
		for (int j = 0; j < pNurbsCurve->m_ArraySegmentParameters.size() - 1; j++)
		{
			if (t <= pNurbsCurve->m_ArraySegmentParameters[j + 1])
			{
				segmentindex = j;

				//本段的m n
				Vector3D dir = arrayPoints[segmentindex + 1] - arrayPoints[segmentindex];
				Vector3D refvec;//偏移后reference line的切向
				refvec[0] = pNurbsCurve->m_ArrayReferenceLines[segmentindex]->m;
				refvec[1] = pNurbsCurve->m_ArrayReferenceLines[segmentindex]->n;
				refvec[2] = 0;

				// 计算切向量
				Vector3D tangentvec = pNurbsCurve->pCurve->ComputDerivative1(t);
				tangentvec.SetUnit();

				// 计算放缩系数alphaalpha
				scaleRatio[i] = fabs(1.0 / (refvec[0] * tangentvec[0] + refvec[1] * tangentvec[1]));

				if (scaleRatio[i] > maxRatio)
					maxRatio = scaleRatio[i];

				if (scaleRatio[i] < minRatio)
					minRatio = scaleRatio[i];

				int x = (scaleRatio[i] - minRatio) * m_ColorbarImage.GetWidth() / (maxRatio - minRatio) + 0.5;

				if (x >= m_ColorbarImage.GetWidth())
					x = m_ColorbarImage.GetWidth() - 1;

				if (x <= 0)
					x = 0;

				// 获取对应颜色
				COLORREF color = m_ColorbarImage.GetPixel(x, 0);

				// 将点和对应的颜色存储起来
				pNurbsCurve->arrayCurveTessellationColorBar_SDL[i] = color;
				break;
			}
		}
	}
	sprintf(Post_Message, "MinRatio: %f, MaxRatio: %f", minRatio, maxRatio);
	::PostMessage(g_hwndMain, WM_MESSAGE, 0, (LPARAM)Post_Message);
	pDoc->UpdateAllViews(NULL);
}
void Action_SDLParameterization::CalculateOptimalSDL()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	if (pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() != TYPE_NURBSCURVE3D)
		return;

	CNurbsCurve* pNurbsCurve = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	pNurbsCurve->m_ArrayReferenceLines.clear();
	pNurbsCurve->m_ArraySegmentParameters.clear();


	double startpara, middlepara, endpara;
	startpara = 0;
	middlepara = 0.2;
	pNurbsCurve->m_ArraySegmentParameters.push_back(startpara);

	while (1)
	{
		//the first segment
		pNurbsCurve->m_ArraySegmentParameters.push_back(middlepara);

		Point3D startpoint = pNurbsCurve->pCurve->ComputPosition(startpara);
		Point3D middlepoint = pNurbsCurve->pCurve->ComputPosition(middlepara);
		Vector3D middletangent = pNurbsCurve->pCurve->ComputDerivative1(middlepara);

		//
		endpara = SearchForEndPoint(pNurbsCurve->pCurve, startpoint, middlepoint, middletangent, startpara, middlepara);

		//add the reference line
		AddReferenceLine(pNurbsCurve, startpoint, middlepoint);

		startpara = middlepara;
		middlepara = endpara;

		if (endpara > 1)
		{
			//compute the end point
			Point3D endpoint = pNurbsCurve->pCurve->GetEndPoint();
			Vector3D endtangent = pNurbsCurve->pCurve->ComputDerivative1(1);

			middletangent.SetUnit();
			Vector3D v1 = middlepoint - startpoint;
			v1.SetUnit();

			double cosangle = v1 * middletangent;

			Vector3D v2 = middletangent * cosangle * 2 - v1;
			v2.SetValue(v2.y, -v2.x, 0);

			double t = (middlepoint - endpoint) * v2 / (endtangent * v2);
			endpoint = endpoint + endtangent * t;

			AddReferenceLine(pNurbsCurve, middlepoint, endpoint);
			// (endpoint + endtangent * t - middlepoint)*v2 = 0
			//add the reference line
			//
			//AddReferenceLine(pNurbsCurve, middlepoint, middlepoint);
			break;
		}

	}

	pDoc->UpdateAllViews(NULL);
}

void Action_SDLParameterization::AddReferenceLine(CNurbsCurve* pNurbsCurve, Point3D startpoint, Point3D endpoint)
{
	ReferenceLine* pLine = new ReferenceLine();

	Point3D middlepoint = (startpoint + endpoint) / 2;

	Vector3D dir = endpoint - startpoint;

	dir.SetValue(dir.y, -dir.x, 0);

	pLine->start = middlepoint + dir;
	pLine->end = middlepoint - dir;

	dir.SetUnit();

	pLine->m = -dir.y;
	pLine->n = dir.x;
	pLine->c = -(-dir.y * middlepoint.x + -dir.x * middlepoint.y);
	pNurbsCurve->m_ArrayReferenceLines.push_back(pLine);
}
void Action_SDLParameterization::CalculateSDLCoefficient()
{
	//InitializeConsole();
	// 定义贝塞尔曲线的控制点和权重
	//Vector2d P0(-2, 5), P1(1, 2), P2(3, 7);
	//double w0 = 1, w1 = 0.5, w2 = 1;
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_SceneGraph.NbObject() == 0)
		return;
	if (pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1)->GetType() != TYPE_NURBSCURVE3D)
		return;

	CNurbsCurve* pNurbsCurve = (CNurbsCurve*)pDoc->m_SceneGraph.GetAt(pDoc->m_SceneGraph.NbObject() - 1);

	pNurbsCurve->m_ArrayReferenceLines.clear();
	pNurbsCurve->m_ArraySegmentParameters.clear();

	Eigen::Vector2d P0, P1, P2;
	double w0, w1, w2;

	P0(0) = pNurbsCurve->pCurve->GetControlPoint(0).x;
	P0(1) = pNurbsCurve->pCurve->GetControlPoint(0).y;
	w0 = pNurbsCurve->pCurve->GetWeight(0);

	P1(0) = pNurbsCurve->pCurve->GetControlPoint(1).x;
	P1(1) = pNurbsCurve->pCurve->GetControlPoint(1).y;
	w1 = pNurbsCurve->pCurve->GetWeight(1);

	P2(0) = pNurbsCurve->pCurve->GetControlPoint(2).x;
	P2(1) = pNurbsCurve->pCurve->GetControlPoint(2).y;
	w2 = pNurbsCurve->pCurve->GetWeight(2);

	// 分段数
	int n = 3;

	// 均分参数t
	vector<double> t_values(n + 1);
	for (int i = 0; i <= n; ++i) {
		t_values[i] = static_cast<double>(i) / n;

		//add to nurbs curve
		pNurbsCurve->m_ArraySegmentParameters.push_back(t_values[i]);
	}


	// 存储每段曲线的起点、终点和切向
	vector<Eigen::Vector2d> points(n + 1);
	vector<Eigen::Vector2d> dpoints(n + 1);
	vector<Point2D> POINTSS(n + 1);
	//for (int i = 0; i <= n; ++i) {
	//	points[i] = bezierPoint(t_values[i], P0, P1, P2, w0, w1, w2); //点坐标
	//	dpoints[i] = dbezierPoint(t_values[i], P0, P1, P2, w0, w1, w2); //点切向
	//}

	for (int i = 0; i <= n; ++i) {
		//points[i] = bezierPoint(t_values[i], P0, P1, P2, w0, w1, w2); //点坐标
		Point3D point = pNurbsCurve->pCurve->ComputPosition(t_values[i]);
		points[i][0] = point[0];
		points[i][1] = point[1];
		//dpoints[i] = dbezierPoint(t_values[i], P0, P1, P2, w0, w1, w2); //点切向
		Vector3D tangentvec = pNurbsCurve->pCurve->ComputDerivative1(t_values[i]);
		dpoints[i][0] = tangentvec[0];
		dpoints[i][1] = tangentvec[1];
	}

	for (int i = 0; i <= n; i++) {
		POINTSS[i].x = points[i].x();
		POINTSS[i].y = points[i].y();
	}

	// 计算每段曲线的系数
	Eigen::Vector3d mnc, next_mnc;
	//double d = -0.3;
	double d = 0;
	double ratio = referenceratio;
	for (int i = 0; i < n; ++i) {
		Eigen::Matrix3d A;
		Eigen::Vector3d b;

		// 第一段特殊处理
		if (i == 0) {
			A << points[0].x() * (1 - ratio) + points[1].x() * ratio, points[0].y()* (1 - ratio) + points[1].y() * ratio, 1,
				dpoints[0].x(), dpoints[0].y(), 0,
				dpoints[1].x(), dpoints[1].y(), 0;
			b << 0, 1, 1;
		}
		else {
			A << points[i].x(), points[i].y(), 1,
				dpoints[i].x(), dpoints[i].y(), 0,
				dpoints[i + 1].x(), dpoints[i + 1].y(), 0;
			b << d, 1, 1;
		}

		if (!solveLinearSystem(A, b, next_mnc)) {
			cout << "No solution exists for segment " << i << endl;
			//return -1;
			return;
		}
		mnc = next_mnc;
		double test;
		if (i == 0) {
			test = (points[0].x() * (1 - ratio) + points[1].x() * ratio) * mnc[0] + (points[0].y() * (1 - ratio) + points[1].y() * ratio) * mnc[1] + mnc[2];
			test = dpoints[0].x() * mnc[0] + dpoints[0].y() * mnc[1] - 1;
			test = dpoints[1].x() * mnc[0] + dpoints[1].y() * mnc[1] - 1;
		}
		else {
			test = (points[i].x()) * mnc[0] + (points[i].y()) * mnc[1] + mnc[2] - d;
			test = dpoints[i].x() * mnc[0] + dpoints[i].y() * mnc[1] - 1;
			test = dpoints[i + 1].x() * mnc[0] + dpoints[i + 1].y() * mnc[1] - 1;
		}



		d = mnc[0] * points[i + 1].x() + mnc[1] * points[i + 1].y() + mnc[2];

		//cout << "Segment " << i << ": m" << i + 1 << " = " << mnc[0] << ", n" << i + 1 << " = " << mnc[1] << ", c" << i + 1 << " = " << mnc[2] << ", d" << i + 1 << " = " << d << endl;

		//Point2D midPoint = getPointOnLineSegment(POINTSS[i], POINTSS[i + 1], mnc(0), mnc(1));
		//std::cout << "Point on line segment: (" << midPoint.x << ", " << midPoint.y << ")\n";

		//int length = 10;
		//drawPerpendicularLine(pNurbsCurve, midPoint, POINTSS[i], POINTSS[i + 1], length);
		ComputeReferenceLine(pNurbsCurve, i, mnc);
	}

	ValidateParameterization(pNurbsCurve);

	//return 0;
	pDoc->UpdateAllViews(NULL);
	return;

}

void Action_SDLParameterization::ValidateParameterization(CNurbsCurve* pNurbsCurve)
{

	// 

	for (int i = 0; i < pNurbsCurve->m_ArrayReferenceLines.size() - 1; i++)
	{
		ReferenceLine* pLine1 = pNurbsCurve->m_ArrayReferenceLines[i];
		ReferenceLine* pLine2 = pNurbsCurve->m_ArrayReferenceLines[i + 1];

		double t = pNurbsCurve->m_ArraySegmentParameters[i + 1];
		Point3D Position = pNurbsCurve->pCurve->ComputPosition(t);
		Vector3D tangentdir = pNurbsCurve->pCurve->ComputDerivative1(t);

		//C0 连续
		double t0 = Position.x * pLine1->m + Position.y * pLine1->n + pLine1->c;
		double t1 = Position.x * pLine2->m + Position.y * pLine2->n + pLine2->c;
		double toler0 = fabs(t0 - t1);
		ASSERT(toler0 < 1e-8);

		//C1连续
		double toler10 = fabs(tangentdir.x * pLine1->m + tangentdir.y * pLine1->n - 1);
		double toler11 = fabs(tangentdir.x * pLine2->m + tangentdir.y * pLine2->n - 1);
		ASSERT(toler10 < 1e-8);
		ASSERT(toler11 < 1e-8);
	}


}

void Action_SDLParameterization::ComputeReferenceLine(CNurbsCurve* pNurbsCurve, int index, Eigen::Vector3d mnc)
{
	//compute the intersection point of the line segment and the referece line
	Point3D p0 = pNurbsCurve->pCurve->ComputPosition(pNurbsCurve->m_ArraySegmentParameters[index]);
	Point3D p1 = pNurbsCurve->pCurve->ComputPosition(pNurbsCurve->m_ArraySegmentParameters[index + 1]);

	double d = Length(p1 - p0);

	Point2D p02d(p0.x, p0.y);
	Point2D p12d(p1.x, p1.y);

	Point2D Q(p0.x, (-mnc[0] * p0.x - mnc[2]) / mnc[1]);

	//perpendicular direction vector of the referece line
	Vector2D Qv(mnc[0], mnc[1]);
	double t = (Q - p02d) * Qv / ((p12d - p02d) * Qv);

	Vector2D refdir(-mnc[1], mnc[0]);
	refdir.SetUnit();

	Point2D C = MidPoint(p02d, p12d);
	double newt = (C - Q) * refdir;
	//t = newt;

	//Point2D intersectionpoint = p02d + (p12d - p02d) * t;
	Point2D intersectionpoint = Q + refdir * newt;
	//

	Point2D start = intersectionpoint + refdir * d;
	Point2D end = intersectionpoint - refdir * d;

	ReferenceLine* pLine = new ReferenceLine();
	pLine->start = start;
	pLine->end = end;
	pLine->m = mnc[0];
	pLine->n = mnc[1];
	pLine->c = mnc[2];
	pNurbsCurve->m_ArrayReferenceLines.push_back(pLine);
}