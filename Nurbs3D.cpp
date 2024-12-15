#include "stdafx.h"
#include "disline.h"
#include "InterFunction.h"
#include "math.h"
#include "Nurbs3D.h"
#include "C_NurbsSurf.h"
#include "SDIViewSwitchDoc.h"
#include "lib3d/Base3d.h"
#include <windows.h>
#include "MyTreeView.h"
#include <iostream>
#include <fstream>
#include "resource.h"
#include <atlimage.h>

#define MAX_CHAR        128
CNurbsSuface::CNurbsSuface()
{
	textureCntrlPnt[0][0][0] = 0.0f;
	textureCntrlPnt[0][0][1] = 0.0f;
	textureCntrlPnt[0][1][0] = 0.0f;
	textureCntrlPnt[0][1][1] = 1.0f;
	textureCntrlPnt[1][0][0] = 1.0f;
	textureCntrlPnt[1][0][1] = 0.0f;
	textureCntrlPnt[1][1][0] = 1.0f;
	textureCntrlPnt[1][1][1] = 1.0f;
	m_Show = 1;
	type = TYPE_NURBSSURFACE;
	m1 = 0;
	n1 = 0;
	c1 = 0;

	/*m_ReparameterizationType = none;

	reparameterization_coefficients.clear();

	for(int i = 0; i < 2; i++)
		reparameterization_coefficients.push_back(0.5);
	//m_Mobiustransformation_Alpha = 50;
	//m_Mobiustransformation_Beta = 50;

	//m_Bilinear_Alpha1 = m_Bilinear_Alpha2 = m_Bilinear_Beta1 = m_Bilinear_Beta2 = 50;
	for(int i = 0; i < 4; i++)
		reparameterization_coefficients.push_back(0.5);

	//m_Quadratic_Alpha1 = m_Quadratic_Alpha2 = m_Quadratic_Alpha3 = 50;
	//m_Quadratic_Beta1 = m_Quadratic_Beta2 = m_Quadratic_Beta3 = 50;
	for(int i = 0; i < 6; i++)
		reparameterization_coefficients.push_back(0.5);

	//general bilinear transformation
	//for(int i = 0; i < 16; i++)
	reparameterization_coefficients.push_back(0);
	reparameterization_coefficients.push_back(0);
	reparameterization_coefficients.push_back(1);
	reparameterization_coefficients.push_back(1);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0);
	reparameterization_coefficients.push_back(1);
	reparameterization_coefficients.push_back(0);
	reparameterization_coefficients.push_back(1);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);
	reparameterization_coefficients.push_back(0.5);*/

	m_showColorBoundary = FALSE;
	m_showSubdivisionCurves = FALSE;
	m_showIsoParameterCurves = FALSE;
	m_showOptimalIsoParameterCurvesMobiusOpitmal = FALSE;
	m_showOptimalIsoParameterCurvesNonOpitimal = FALSE;
	m_showOptimalIsoParameterCurvesMobiusLinearOpitmal = FALSE;
	m_showOptimalIsoParameterCurvesMobiusQuadraticOptimal = FALSE;
	m_showOptimalIsoParameterCurves = FALSE;
	m_showSurface = TRUE;
	m_showControlPointsNetwork = FALSE;
	m_showSubSurfaceCoordinate = FALSE;
	m_showBilinearCoefficients = FALSE;
	m_showAreaPreservingIsoParameterCurves = FALSE;
	m_showAreaPreservingIsoParameterCurves_Modified = FALSE;
	pNurbsSurf = NULL;
	m_showColorOrthogonality = FALSE;
	m_showIsoParameterCurvesGeneralBilinear = FALSE;

	m_ListDone = FALSE;
	m_Modified = FALSE;

	ptransformation = NULL;

	//color
	s_r = s_g = s_b = 0;
	c_r = c_g = c_b = 0;

	//orthogonalityweight = 0.5;
	name = "NurbsSur";
}

void CNurbsSuface::Read(std::ifstream& file)
{
	int ctrlunum, ctrlvnum;
	file >> ctrlunum >> ctrlvnum;
	file >> m_udegree >> m_vdegree;

	m_uknotcount = m_udegree + ctrlunum + 1;
	m_vknotcount = m_vdegree + ctrlvnum + 1;

	m_uknot = new GLfloat[m_uknotcount];
	m_vknot = new GLfloat[m_vknotcount];

	for (int i = 0; i < m_uknotcount; i++)
	{
		file >> m_uknot[i];
	}
	for (int i = 0; i < m_vknotcount; i++)
	{
		file >> m_vknot[i];
	}

	for (int i = 0; i < GetCtrlNumberU(); i++)
		for (int j = 0; j < GetCtrlNumberV(); j++)
		{
			Vector3D p1;
			double w;
			file >> p1.x >> p1.y >> p1.z;
			file >> w;
			SetControlPoint(i, j, p1);
			SetWeight(i, j, w);
		}
}

void CNurbsSuface::Save(std::ofstream& file)
{
	file << GetCtrlNumberU() << " " << GetCtrlNumberV() << "\n";
	file << m_udegree << " " << m_vdegree << "\n";

	for (int i = 0; i < m_uknotcount; i++)
	{
		file << m_uknot[i] << " ";
	}
	file << "\n";
	for (int i = 0; i < m_vknotcount; i++)
	{
		file << m_vknot[i] << " ";
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
double CNurbsSuface::ComputeConformalEnergy()
{
	if (!ptransformation || ptransformation->m_ReparameterizationType != generalbilinear)
		return 0;

	//Xs = Xu * us + Xv * vs
	//Xt = Xu * ut + Xv * vt


}

CNurbsSuface::~CNurbsSuface()
{

}
int m_ShowTexture;
int m_ShowCoordinateAxis;
CString m_FileTextureName;
extern int RBnum;
extern struct {
	C_RationalBezierCurve3D* pCurve;
	double startu, startv;
	double endu, endv;
} m_Array[100];
extern C_Disline2D m_dis1, m_dis2;
extern C_NurbsCurve3D* pCurve;
void Exchange(Vector3D& v1, Vector3D& v2)
{
	Vector3D ceshi = v1;
	v1 = v2;
	v2 = ceshi;
}
int CNurbsSuface::GetProjectCurve(double u1, double v1, double u2, double v2, C_NurbsCurve3D*& pCurve)
{
	return 1;
}

CNurbsSuface* CNurbsSuface::copy()
{
	CNurbsSuface* pSurface1 = new CNurbsSuface();
	pSurface1->m_udegree = m_udegree;
	pSurface1->m_vdegree = m_vdegree;

	pSurface1->m_uknot = new GLfloat[m_uknotcount];
	pSurface1->m_vknot = new GLfloat[m_vknotcount];
	pSurface1->m_uknotcount = m_uknotcount;
	pSurface1->m_vknotcount = m_vknotcount;

	int i, j;
	for (i = 0; i < pSurface1->m_uknotcount; i++)
	{
		pSurface1->m_uknot[i] = m_uknot[i];
	}
	for (i = 0; i < pSurface1->m_vknotcount; i++)
	{
		pSurface1->m_vknot[i] = m_vknot[i];
	}
	for (i = 0; i < pSurface1->GetCtrlNumberU(); i++)
		for (j = 0; j < pSurface1->GetCtrlNumberV(); j++)
		{
			Point3D point = GetControlPoint(i, j);
			double weight = GetWeight(i, j);

			pSurface1->m_ControlPoint[i][j][0] = point[0] * weight;
			pSurface1->m_ControlPoint[i][j][1] = point[1] * weight;
			pSurface1->m_ControlPoint[i][j][2] = point[2] * weight;
			pSurface1->m_ControlPoint[i][j][3] = weight;

		}
	pSurface1->SetTransform(*GetTransform());
	if (ptransformation)
		pSurface1->ptransformation = ptransformation->copy();
	else
		pSurface1->ptransformation = NULL;
	return pSurface1;
}
extern CSDIViewSwitchDoc* pDoc;

C_NurbsSurf* CNurbsSuface::Converte_C_NurbsSurface()
{
	if (pNurbsSurf)
		return pNurbsSurf;

	//generate a C_NurbsSurf
	double* weight = new double[GetCtrlNumberU() * GetCtrlNumberV()];
	double* uknot = new double[m_uknotcount];
	double* vknot = new double[m_vknotcount];

	int i, j;
	for (i = 0; i < m_uknotcount; i++)
		uknot[i] = m_uknot[i];

	for (i = 0; i < m_vknotcount; i++)
		vknot[i] = m_vknot[i];

	int pnum = 0;
	Point3D* pointarray = new Point3D[GetCtrlNumberU() * GetCtrlNumberV()];
	for (j = 0; j < GetCtrlNumberV(); j++)
	{
		for (i = 0; i < GetCtrlNumberU(); i++)
		{
			pointarray[pnum][0] = m_ControlPoint[i][j][0] / m_ControlPoint[i][j][3];
			pointarray[pnum][1] = m_ControlPoint[i][j][1] / m_ControlPoint[i][j][3];
			pointarray[pnum][2] = m_ControlPoint[i][j][2] / m_ControlPoint[i][j][3];
			weight[pnum] = m_ControlPoint[i][j][3];
			pnum++;
		}
	}
	C_NurbsSurf* pSurf = new C_NurbsSurf(GetCtrlNumberU(), GetCtrlNumberV(),
		m_udegree + 1, m_vdegree + 1, uknot, vknot, pointarray, weight);

	pNurbsSurf = pSurf;
	return pSurf;
}
extern CString m_FileTextureName;
extern int m_ShowTexture;
void CNurbsSuface::DrawCoordinateAxis()
{
	glLineWidth(3.0);
	glDisable(GL_LIGHTING);
	::glColor3f(1, 0, 0);
	::glBegin(GL_LINES);
	::glVertex3f(0, 0, 0);
	::glVertex3f(10, 0, 0);
	::glEnd();

	::glColor3f(0, 1, 0);
	::glBegin(GL_LINES);
	::glVertex3f(0, 0, 0);
	::glVertex3f(0, 10, 0);
	::glEnd();

	::glColor3f(0, 0, 1);
	::glBegin(GL_LINES);
	::glVertex3f(0, 0, 0);
	::glVertex3f(0, 0, 10);
	::glEnd();
	glEnable(GL_LIGHTING);
}

void CNurbsSuface::DrawOriginalControlPointsNetwork()
{
	::glColor3f(1, 0, 0);
	glPointSize(9.0);
	::glBegin(GL_POINTS);
	int ii = 0;
	for (int i = 0; i < GetCtrlNumberU(); i++)
		for (int j = 0; j < GetCtrlNumberV(); j++)
		{
			glLoadName(ii);
			ii++;
			Vector3D point = GetControlPoint(i, j);
			glVertex3f(point.x, point.y, point.z);
		}
	::glEnd();

	//draw the control network
	::glBegin(GL_LINES);
	::glColor3f(0, 1, 0);
	for (int i = 0; i < GetCtrlNumberU(); i++)
		for (int j = 0; j < GetCtrlNumberV() - 1; j++)
		{
			Vector3D point1 = GetControlPoint(i, j);
			Vector3D point2 = GetControlPoint(i, j + 1);
			glVertex3f(point1.x, point1.y, point1.z);
			glVertex3f(point2.x, point2.y, point2.z);
		}
	::glEnd();

	::glBegin(GL_LINES);

	for (int j = 0; j < GetCtrlNumberV(); j++)
		for (int i = 0; i < GetCtrlNumberU() - 1; i++)
		{
			Vector3D point1 = GetControlPoint(i, j);
			Vector3D point2 = GetControlPoint(i + 1, j);
			glVertex3f(point1.x, point1.y, point1.z);
			glVertex3f(point2.x, point2.y, point2.z);
		}
	::glEnd();
}
void CNurbsSuface::GetReparameterizedCoordinates(double& s, double& t)
{
	// 0 don't use the reparameterization function
	// 1 general bilinear transformation
	if (ptransformation == NULL)
		return;
	double u, v;
	ptransformation->Evaluation(s, t, u, v);
	s = u;
	t = v;

	//return;
	//just for debug
	double minu, maxu, minv, maxv;
	minu = m_uknot[0];
	maxu = m_uknot[m_uknotcount - 1];
	minv = m_vknot[0];
	maxv = m_vknot[m_vknotcount - 1];
	if (s < minu)
		s = minu;
	if (t < minv)
		t = minv;
	if (s > maxu)
		s = maxu;
	if (t > maxv)
		t = maxv;
}
void CNurbsSuface::DrawControlPointsNetwork()
{
	glDisable(GL_DEPTH_TEST);
	::glDisable(GL_LIGHTING);
	//glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	//glEnable (GL_LINE_STIPPLE);
	glColor3f(0, 1, 0);
	glLineWidth(2.0f);

	//
	//if(ptransformation == NULL)
	{
		//draw the original control points
		DrawOriginalControlPointsNetwork();
	}
	::glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CNurbsSuface::ShowColorBoundary()
{
	int num = 500;//曲线的精细度
	::glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(4.0);
	glColor3f(1.0, 0.0, 0.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < num; i++)
	{
		Point3D p1 = ComputPosition(i / (num + 0.0), 0);
		Point3D p2 = ComputPosition((i + 1) / (num + 0.0), 0);

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p2.z);
	}
	for (int i = 0; i < num; i++)
	{
		Point3D p1 = ComputPosition(i / (num + 0.0), 1);
		Point3D p2 = ComputPosition((i + 1) / (num + 0.0), 1);

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p2.z);
	}
	::glEnd();

	glColor3f(0.0, 1.0, 0.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < num; i++)
	{
		Point3D p1 = ComputPosition(0, i / (num + 0.0));
		Point3D p2 = ComputPosition(0, (i + 1) / (num + 0.0));

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p2.z);
	}
	for (int i = 0; i < num; i++)
	{
		Point3D p1 = ComputPosition(1, i / (num + 0.0));
		Point3D p2 = ComputPosition(1, (i + 1) / (num + 0.0));

		::glVertex3f(p1.x, p1.y, p1.z);
		::glVertex3f(p2.x, p2.y, p2.z);
	}

	::glEnd();
	::glEnable(GL_LIGHTING);//光照颜色
}
void CNurbsSuface::DrawIsoParameterCurves_GeneralBilinear()
{

}

void CNurbsSuface::DrawIsoParameterCurves()
{
	glDisable(GL_DEPTH_TEST);
	::glDisable(GL_LIGHTING);
	//glEnable (GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	//glEnable (GL_LINE_STIPPLE);

	C_NurbsSurf* pSurface = Converte_C_NurbsSurface();
	double umin, umax, vmin, vmax;

	if (ptransformation == NULL)
	{
		umin = m_uknot[0];
		umax = m_uknot[m_uknotcount - 1];
		vmin = m_vknot[0];
		vmax = m_vknot[m_vknotcount - 1];
	}
	else
	{
		umin = ptransformation->umin_Reparameterization;
		umax = ptransformation->umax_Reparameterization;
		vmin = ptransformation->vmin_Reparameterization;
		vmax = ptransformation->vmax_Reparameterization;
	}

	double offset = 0.0001;
	glLineWidth(2.0f);
	//pDoc->m_Disc1 = 28;
	//pDoc->m_Disc2 = 30;
	for (int j = 0; j <= pDoc->m_Disc1; j++)
	{
		for (int i = 0; i < pDoc->m_Disc2; i++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)i) / pDoc->m_Disc2;
			double t1 = ((GLfloat)j) / pDoc->m_Disc1;
			double s2 = ((GLfloat)i + 1) / pDoc->m_Disc2;
			double t2 = ((GLfloat)j) / pDoc->m_Disc1;
			double u1, v1, u2, v2;
			s1 = umin + (umax - umin) * s1;
			t1 = vmin + (vmax - vmin) * t1;
			s2 = umin + (umax - umin) * s2;
			t2 = vmin + (vmax - vmin) * t2;

			u1 = s1;
			v1 = t1;
			u2 = s2;
			v2 = t2;

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);

			//Vector3D pnormal1 = GetParaNormal(u1,v1,TRUE);
			//Vector3D pnormal2 = GetParaNormal(u2,v2,TRUE);

			//p1 = p1 + pnormal1 * offset;
			//p2 = p2 + pnormal2 * offset;

			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
		for (int i = 0; i < pDoc->m_Disc2; i++)
		{
			::glBegin(GL_LINES);
			double s1 = ((GLfloat)j) / pDoc->m_Disc1;
			double t1 = ((GLfloat)i) / pDoc->m_Disc2;
			double s2 = ((GLfloat)j) / pDoc->m_Disc1;
			double t2 = ((GLfloat)(i + 1)) / pDoc->m_Disc2;

			double u1, v1, u2, v2;

			s1 = umin + (umax - umin) * s1;
			t1 = vmin + (vmax - vmin) * t1;
			s2 = umin + (umax - umin) * s2;
			t2 = vmin + (vmax - vmin) * t2;

			u1 = s1;
			v1 = t1;
			u2 = s2;
			v2 = t2;

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);

			//Vector3D pnormal1 = GetParaNormal(u1,v1,TRUE);
			//Vector3D pnormal2 = GetParaNormal(u2,v2,TRUE);

			//p1 = p1 + pnormal1 * offset;
			//p2 = p2 + pnormal2 * offset;

			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}

	}
	//glLineWidth(3.0f);
	//glColor3f(0, 1, 0);
	//DrawOriginalControlPointsNetwork();
	::glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}
void CNurbsSuface::DrawLines(double alpha, double beta)
{
	::glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable(GL_LINE_STIPPLE);
	glColor3f(0, 0, 0);//颜色
	glLineWidth(1.0f);
	//-----------------------------------------------------------------
	int arcStep = 10;
	//-------------------------------------------------------------------------------------------------------------
	//最后画出两个方向上的等参线
	for (int j = 0; j <= arcStep; j++)
	{
		for (int i = 0; i < arcStep; i++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)i) / arcStep;
			double t1 = ((GLfloat)j) / arcStep;
			double s2 = ((GLfloat)i + 1) / arcStep;
			double t2 = ((GLfloat)j) / arcStep;

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;

			double a = alpha;
			double b = beta;
			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
		for (int i = 0; i < arcStep; i++)
		{
			::glBegin(GL_LINES);
			double s1 = ((GLfloat)j) / arcStep;
			double t1 = ((GLfloat)i) / arcStep;
			double s2 = ((GLfloat)j) / arcStep;
			double t2 = ((GLfloat)(i + 1)) / arcStep;

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;


			double a = alpha;
			double b = beta;

			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}

	}

	::glEnable(GL_LIGHTING);

}
void CNurbsSuface::DrawLines(double alpha1, double beta1, double alpha2, double beta2)
{
	::glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable(GL_LINE_STIPPLE);
	glColor3f(0, 0, 0);//颜色
	glLineWidth(1.0f);
	//-----------------------------------------------------------------
	int arcStep = 10;
	//-------------------------------------------------------------------------------------------------------------
	//最后画出两个方向上的等参线
	for (int j = 0; j <= arcStep; j++)
	{
		for (int i = 0; i < arcStep; i++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)i) / (arcStep + 0.0);
			double t1 = ((GLfloat)j) / (arcStep + 0.0);
			double s2 = ((GLfloat)i + 1) / (arcStep + 0.0);
			double t2 = ((GLfloat)j) / (arcStep + 0.0);

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;

			//mobius transformation
			double a1 = alpha1;
			double b1 = beta1;
			double a2 = alpha2;
			double b2 = beta2;

			double a = a1 * t1 + a2 * (1 - t1);
			double b = b1 * s1 + b2 * (1 - s1);

			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			a = a1 * t2 + a2 * (1 - t2);
			b = b1 * s2 + b2 * (1 - s2);

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

			if (v1 > 1.0)
			{
				v1 = 1.0;
			}
			if (u1 > 1.0)
			{
				u1 = 1.0;
			}
			if (v2 > 1.0)
			{
				v2 = 1.0;
			}
			if (u2 > 1.0)
			{
				u2 = 1.0;
			}
			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
		for (int i = 0; i < pDoc->m_Disc2; i++)
		{
			::glBegin(GL_LINES);
			double s1 = ((GLfloat)j) / (arcStep + 0.0);
			double t1 = ((GLfloat)i) / (arcStep + 0.0);
			double s2 = ((GLfloat)j) / (arcStep + 0.0);
			double t2 = ((GLfloat)(i + 1)) / (arcStep + 0.0);

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;


			double a1 = alpha1;
			double b1 = beta1;
			double a2 = alpha2;
			double b2 = beta2;

			double a = a1 * t1 + a2 * (1 - t1);
			double b = b1 * s1 + b2 * (1 - s1);

			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			a = a1 * t2 + a2 * (1 - t2);
			b = b1 * s2 + b2 * (1 - s2);

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
			if (v1 > 1.0)
			{
				v1 = 1.0;
			}
			if (u1 > 1.0)
			{
				u1 = 1.0;
			}
			if (v2 > 1.0)
			{
				v2 = 1.0;
			}
			if (u2 > 1.0)
			{
				u2 = 1.0;
			}
			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}

	}

	::glEnable(GL_LIGHTING);
}
void CNurbsSuface::DrawLines(double alpha1, double beta1, double alpha2, double beta2, double alpha3, double beta3)
{
	::glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);     /* Enable Antialiased lines */
	glEnable(GL_LINE_STIPPLE);
	glColor3f(0, 0, 0);//颜色
	glLineWidth(1.0f);
	//-----------------------------------------------------------------
	int arcStep = 10;

	/*double minimalAlpha1 = 0.20000;
	double minimalAlpha2 =0.30000;
	double minimalAlpha3 =0.20000 ;
	double minimalBeta1 =0.20000;
	double minimalBeta2 =0.30000;
	double minimalBeta3 =0.20000;
	*/
	//最后画出两个方向上的等参线
	for (int j = 0; j <= arcStep; j++)
	{
		for (int i = 0; i < arcStep; i++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)i) / (arcStep + 0.0);
			double t1 = ((GLfloat)j) / (arcStep + 0.0);
			double s2 = ((GLfloat)i + 1) / (arcStep + 0.0);
			double t2 = ((GLfloat)j) / (arcStep + 0.0);

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;

			//mobius transformation
			double a1 = alpha1;
			double b1 = beta1;
			double a2 = alpha2;
			double b2 = beta2;
			double a3 = alpha3;
			double b3 = beta3;


			double a = a1 * (1 - t1) * (1 - t1) + a2 * 2 * t1 * (1 - t1) + a3 * t1 * t1;
			double b = b1 * (1 - s1) * (1 - s1) + b2 * 2 * s1 * (1 - s1) + b3 * s1 * s1;

			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			a = a1 * (1 - t2) * (1 - t2) + a2 * 2 * t2 * (1 - t2) + a3 * t2 * t2;
			b = b1 * (1 - s2) * (1 - s2) + b2 * 2 * s2 * (1 - s2) + b3 * s2 * s2;

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

			if (v1 > 1.0)
			{
				v1 = 1.0;
			}
			if (u1 > 1.0)
			{
				u1 = 1.0;
			}
			if (v2 > 1.0)
			{
				v2 = 1.0;
			}
			if (u2 > 1.0)
			{
				u2 = 1.0;
			}
			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
		for (int i = 0; i < arcStep; i++)
		{
			::glBegin(GL_LINES);
			double s1 = ((GLfloat)j) / (arcStep + 0.0);
			double t1 = ((GLfloat)i) / (arcStep + 0.0);
			double s2 = ((GLfloat)j) / (arcStep + 0.0);
			double t2 = ((GLfloat)(i + 1)) / (arcStep + 0.0);

			double u1 = s1;
			double v1 = t1;
			double u2 = s2;
			double v2 = t2;


			double a1 = alpha1;
			double b1 = beta1;
			double a2 = alpha2;
			double b2 = beta2;
			double a3 = alpha3;
			double b3 = beta3;

			double a = a1 * (1 - t1) * (1 - t1) + a2 * 2 * t1 * (1 - t1) + a3 * t1 * t1;
			double b = b1 * (1 - s1) * (1 - s1) + b2 * 2 * s1 * (1 - s1) + b3 * s1 * s1;

			u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
			v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

			a = a1 * (1 - t2) * (1 - t2) + a2 * 2 * t2 * (1 - t2) + a3 * t2 * t2;
			b = b1 * (1 - s2) * (1 - s2) + b2 * 2 * s2 * (1 - s2) + b3 * s2 * s2;

			u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
			v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
			if (v1 > 1.0)
			{
				v1 = 1.0;
			}
			if (u1 > 1.0)
			{
				u1 = 1.0;
			}
			if (v2 > 1.0)
			{
				v2 = 1.0;
			}
			if (u2 > 1.0)
			{
				u2 = 1.0;
			}
			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}

	}
	::glEnable(GL_LIGHTING);
}

void CNurbsSuface::DrawIsoParameterCurvesNonOpitimal()
{
	DrawLines(0.5, 0.5);
}
void CNurbsSuface::DrawIsoParameterCurvesMobiusOpitmal()
{
	//-----------------------------------------------------------------
	int step = 10;//采样的数量
	int arcStep = 10;
	//计算极小化能量minimalEnergy
	double minimalAEnergy;//极小化Alpha对应的能量
	double minimalBEnergy;
	double minimalAlpha;
	double minimalBeta;
	double Energy[100] = { 0.0 };//能量
	double aEnergy[100] = { 0.0 };
	double bEnergy[100] = { 0.0 };
	double lineLength;//一条等参线的长度
	double arcLength[30];
	double averageLength;
	//-----------------------------------------------------------------
	for (int m = 1; m < step; m++)
	{
		for (int n = 1; n < step; n++)
		{
			//sample
			double a = m / (step + 10.0);
			double b = n / (step + 10.0);
			aEnergy[(m - 1) * 9 + n - 1] = a;
			bEnergy[(m - 1) * 9 + n - 1] = b;
			//计算能量
			for (int j = 0; j <= arcStep; j++)
			{
				//求长度
				for (int i = 0; i < arcStep; i++)
				{

					double s1 = ((GLfloat)i) / arcStep;
					double t1 = ((GLfloat)j) / arcStep;
					double s2 = ((GLfloat)i + 1) / arcStep;
					double t2 = ((GLfloat)j) / arcStep;

					double u1 = s1;
					double v1 = t1;
					double u2 = s2;
					double v2 = t2;

					u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
					v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

					u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
					v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

					Point3D p1 = ComputPosition(u1, v1);
					Point3D p2 = ComputPosition(u2, v2);
					//计算每一小段的弧长
					arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
					//计算整段弧长
					lineLength += arcLength[i];
				}
				//求每一小段的平均弧长
				averageLength = lineLength / arcStep;
				//求能量
				for (int i = 0; i < arcStep; i++)
				{
					double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
					Energy[(m - 1) * 9 + n - 1] += e;

				}
				lineLength = 0.0;
				//求另一方向上的能量
				for (int i = 0; i < arcStep; i++)
				{
					double s1 = ((GLfloat)j) / arcStep;
					double t1 = ((GLfloat)i) / arcStep;
					double s2 = ((GLfloat)j) / arcStep;
					double t2 = ((GLfloat)(i + 1)) / arcStep;

					double u1 = s1;
					double v1 = t1;
					double u2 = s2;
					double v2 = t2;

					u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
					v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

					u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
					v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);

					Point3D p1 = ComputPosition(u1, v1);
					Point3D p2 = ComputPosition(u2, v2);
					//计算每一小段的弧长
					arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));//
					//计算整段弧长
					lineLength += arcLength[i];
				}
				averageLength = lineLength / arcStep;
				//求能量
				for (int i = 0; i < arcStep; i++)
				{
					double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
					Energy[(m - 1) * 9 + n - 1] += e;
				}
				lineLength = 0.0;
			}
			//输出能量值
			TRACE("%d    ", (m - 1) * 9 + n - 1);
			TCHAR str[100];
			sprintf(str, "%0.5f   ", Energy[(m - 1) * 9 + n - 1]);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
			TRACE(str);
		}
	}

	//返回Energy[]中最小的Energy值
	double minx;
	minx = Energy[0];
	for (int x = 0; x < 81; x++) {
		if (minx > Energy[x] && Energy[x] != 0.0)
			minx = Energy[x];
	}
	//获得最小值所对应的Alpha，Beta值
	for (int x = 0; x < 81; x++) {

		if (minx == Energy[x])
		{
			minimalAlpha = aEnergy[x];
			minimalBeta = bEnergy[x];
			minimalAEnergy = Energy[x];
		}
	}

	TCHAR str[100];
	TRACE("\n minimalAlpha \n");
	sprintf(str, "%0.5f   ", minimalAlpha);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta \n");
	sprintf(str, "%0.5f   ", minimalBeta);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalEnergy \n");
	sprintf(str, "%0.5f   ", minx);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	//-------------------------------------------------------------------------------------------------------------
	DrawLines(minimalAlpha, minimalBeta);
}
void CNurbsSuface::DrawIsoParameterCurvesMobiusLinearOpitmal()
{
	//计算极小化能量minimalEnergy
	double minimalA1Energy;
	double minimalB1Energy;
	double minimalA2Energy;
	double minimalB2Energy;
	double minimalAlpha1;
	double minimalBeta1;
	double minimalAlpha2;
	double minimalBeta2;
	int step = 10;
	int arcStep = 10;
	double Energy[10000] = { 0.0 };
	double a1Energy[10000] = { 0.0 };
	double b1Energy[10000] = { 0.0 };
	double a2Energy[10000] = { 0.0 };
	double b2Energy[10000] = { 0.0 };
	double lineLength;
	double arcLength[30];
	double averageLength;

	for (int m1 = 1; m1 < step; m1++)
	{
		for (int n1 = 1; n1 < step; n1++)
		{
			for (int m2 = 1; m2 < step; m2++)
			{
				for (int n2 = 1; n2 < step; n2++)
				{
					//bilinear transformation.
					double a1 = m1 / (step + 0.0);
					double a2 = m2 / (step + 0.0);
					double b1 = n1 / (step + 0.0);
					double b2 = n2 / (step + 0.0);
					//sample
					a1Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] = a1;
					b1Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] = b1;
					a2Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] = a2;
					b2Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] = b2;
					//计算能量
					for (int j = 0; j <= arcStep; j++)
					{
						//求长度
						for (int i = 0; i < arcStep; i++)
						{
							double s1 = ((GLfloat)i) / (arcStep + 0.0);
							double t1 = ((GLfloat)j) / (arcStep + 0.0);
							double s2 = ((GLfloat)i + 1) / (arcStep + 0.0);
							double t2 = ((GLfloat)j) / (arcStep + 0.0);

							double u1 = s1;
							double v1 = t1;
							double u2 = s2;
							double v2 = t2;

							double a = a1 * t1 + a2 * (1 - t1);
							double b = b1 * s1 + b2 * (1 - s1);

							u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
							v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

							a = a1 * t2 + a2 * (1 - t2);
							b = b1 * s2 + b2 * (1 - s2);

							u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
							v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
							if (v1 > 1.0)
							{
								v1 = 1.0;
							}
							if (u1 > 1.0)
							{
								u1 = 1.0;
							}
							if (v2 > 1.0)
							{
								v2 = 1.0;
							}
							if (u2 > 1.0)
							{
								u2 = 1.0;
							}
							Point3D p1 = ComputPosition(u1, v1);
							Point3D p2 = ComputPosition(u2, v2);
							//计算每一小段的弧长
							arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));//
							//计算整段弧长
							lineLength += arcLength[i];
						}
						averageLength = lineLength / pDoc->m_Disc2;
						//求能量
						for (int i = 0; i < arcStep; i++)
						{
							double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
							Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] += e;

						}
						lineLength = 0.0;

						for (int i = 0; i < arcStep; i++)
						{
							double s1 = ((GLfloat)j) / (arcStep + 0.0);
							double t1 = ((GLfloat)i) / (arcStep + 0.0);
							double s2 = ((GLfloat)j) / (arcStep + 0.0);
							double t2 = ((GLfloat)(i + 1)) / (arcStep + 0.0);

							double u1 = s1;
							double v1 = t1;
							double u2 = s2;
							double v2 = t2;

							double a = a1 * t1 + a2 * (1 - t1);
							double b = b1 * s1 + b2 * (1 - s1);

							u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
							v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

							a = a1 * t2 + a2 * (1 - t2);
							b = b1 * s2 + b2 * (1 - s2);

							u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
							v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
							//---------------------------------------------------
							//保证u,v的值在0~1之间
							if (v1 > 1.0)
							{
								v1 = 1.0;
							}
							if (u1 > 1.0)
							{
								u1 = 1.0;
							}
							if (v2 > 1.0)
							{
								v2 = 1.0;
							}
							if (u2 > 1.0)
							{
								u2 = 1.0;
							}
							//-----------------------------------------------------
							Point3D p1 = ComputPosition(u1, v1);
							Point3D p2 = ComputPosition(u2, v2);
							//计算每一小段的弧长
							arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));//
							//计算整段弧长
							lineLength += arcLength[i];
						}
						averageLength = lineLength / arcStep;
						//求能量
						for (int i = 0; i < arcStep; i++)
						{

							double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
							Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1] += e;

						}
						lineLength = 0.0;
					}
					TCHAR str[100];
					TRACE("%d    ", (m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1);
					sprintf(str, "%0.5f   ", Energy[(m1 - 1) * 729 + (n1 - 1) * 81 + (m2 - 1) * 9 + n2 - 1]);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
					TRACE(str);
				}
			}
		}
	}

	//返回Energy[]中最小的Energy值，k确定了a的值
	double minx;
	minx = Energy[0];
	for (int x = 0; x < 6561; x++) {
		if (minx > Energy[x] && Energy[x] != 0.0)
			minx = Energy[x];
	}

	for (int x = 0; x < 6561; x++) {

		if (minx == Energy[x])
		{
			minimalAlpha1 = a1Energy[x];
			minimalAlpha2 = a2Energy[x];
			minimalBeta1 = b1Energy[x];
			minimalBeta2 = b2Energy[x];
		}
	}


	TCHAR str[100];
	TRACE("\n minimalAlpha1 \n");
	sprintf(str, "%0.5f   ", minimalAlpha1);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalAlpha2 \n");
	sprintf(str, "%0.5f   ", minimalBeta1);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta1 \n");
	sprintf(str, "%0.5f   ", minimalAlpha2);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta2 \n");
	sprintf(str, "%0.5f   ", minimalBeta2);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);


}
void CNurbsSuface::DrawIsoParameterCurvesMobiusQuadraticOpitmal()
{
	//-----------------------------------------------------------------
	//计算极小化能量minimalEnergy
	double minimalA1Energy;
	double minimalB1Energy;
	double minimalA2Energy;
	double minimalB2Energy;
	double minimalA3Energy;
	double minimalB3Energy;
	double minimalAlpha1;
	double minimalBeta1;
	double minimalAlpha2;
	double minimalBeta2;
	double minimalAlpha3;
	double minimalBeta3;
	int step = 10;
	int arcStep = 10;//每条线的段数
	static double Energy[1000000] = { 0.0 };
	static double a1Energy[1000000] = { 0.0 };
	static double b1Energy[1000000] = { 0.0 };
	static double a2Energy[1000000] = { 0.0 };
	static double b2Energy[1000000] = { 0.0 };
	static double a3Energy[1000000] = { 0.0 };
	static double b3Energy[1000000] = { 0.0 };
	double lineLength;
	double arcLength[100];
	double averageLength;
	//-------------------------------------------------------------------------
	for (int m1 = 1; m1 < step; m1++)
	{
		for (int n1 = 1; n1 < step; n1++)
		{
			for (int m2 = 1; m2 < step; m2++)
			{
				for (int n2 = 1; n2 < step; n2++)
				{
					for (int m3 = 1; m3 < step; m3++)
					{
						for (int n3 = 1; n3 < step; n3++)
						{
							//bilinear transformation.
							double a1 = m1 / (step + 0.0);
							double a2 = m2 / (step + 0.0);
							double a3 = m3 / (step + 0.0);
							double b1 = n1 / (step + 0.0);
							double b2 = n2 / (step + 0.0);
							double b3 = n3 / (step + 0.0);
							//sample
							a1Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = a1;
							b1Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = b1;
							a2Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = a2;
							b2Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = b2;
							a3Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = a3;
							b3Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] = b3;
							//计算能量
							for (int j = 0; j <= arcStep; j++)
							{
								//求长度
								for (int i = 0; i < arcStep; i++)
								{
									//::glBegin(GL_LINES);

									double s1 = ((GLfloat)i) / (arcStep + 0.0);
									double t1 = ((GLfloat)j) / (arcStep + 0.0);
									double s2 = ((GLfloat)i + 1) / (arcStep + 0.0);
									double t2 = ((GLfloat)j) / (arcStep + 0.0);

									double u1 = s1;
									double v1 = t1;
									double u2 = s2;
									double v2 = t2;

									double a = a1 * (1 - t1) * (1 - t1) + a2 * 2 * t1 * (1 - t1) + a3 * t1 * t1;
									double b = b1 * (1 - s1) * (1 - s1) + b2 * 2 * s1 * (1 - s1) + b3 * s1 * s1;

									u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
									v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

									a = a1 * (1 - t2) * (1 - t2) + a2 * 2 * t2 * (1 - t2) + a3 * t2 * t2;
									b = b1 * (1 - s2) * (1 - s2) + b2 * 2 * s2 * (1 - s2) + b3 * s2 * s2;

									u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
									v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
									if (v1 > 1.0)
									{
										v1 = 1.0;
									}
									if (u1 > 1.0)
									{
										u1 = 1.0;
									}
									if (v2 > 1.0)
									{
										v2 = 1.0;
									}
									if (u2 > 1.0)
									{
										u2 = 1.0;
									}
									Point3D p1 = ComputPosition(u1, v1);
									Point3D p2 = ComputPosition(u2, v2);
									//计算每一小段的弧长
									arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));//
									//计算整段弧长
									lineLength += arcLength[i];
								}
								averageLength = lineLength / (arcStep + 0.0);
								//求能量
								for (int i = 0; i < arcStep; i++)
								{
									double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
									Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] += e;

								}
								lineLength = 0.0;

								for (int i = 0; i < arcStep; i++)
								{
									double s1 = ((GLfloat)j) / (arcStep + 0.0);
									double t1 = ((GLfloat)i) / (arcStep + 0.0);
									double s2 = ((GLfloat)j) / (arcStep + 0.0);
									double t2 = ((GLfloat)(i + 1)) / (arcStep + 0.0);

									double u1 = s1;
									double v1 = t1;
									double u2 = s2;
									double v2 = t2;

									double a = a1 * (1 - t1) * (1 - t1) + a2 * 2 * t1 * (1 - t1) + a3 * t1 * t1;
									double b = b1 * (1 - s1) * (1 - s1) + b2 * 2 * s1 * (1 - s1) + b3 * s1 * s1;

									u1 = (a - 1) * s1 / (2 * a * s1 - s1 - a);
									v1 = (b - 1) * t1 / (2 * b * t1 - t1 - b);

									a = a1 * (1 - t2) * (1 - t2) + a2 * 2 * t2 * (1 - t2) + a3 * t2 * t2;
									b = b1 * (1 - s2) * (1 - s2) + b2 * 2 * s2 * (1 - s2) + b3 * s2 * s2;

									u2 = (a - 1) * s2 / (2 * a * s2 - s2 - a);
									v2 = (b - 1) * t2 / (2 * b * t2 - t2 - b);
									if (v1 > 1.0)
									{
										v1 = 1.0;
									}
									if (u1 > 1.0)
									{
										u1 = 1.0;
									}
									if (v2 > 1.0)
									{
										v2 = 1.0;
									}
									if (u2 > 1.0)
									{
										u2 = 1.0;
									}
									Point3D p1 = ComputPosition(u1, v1);
									Point3D p2 = ComputPosition(u2, v2);
									//计算每一小段的弧长
									arcLength[i] = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));//
									//计算整段弧长
									lineLength += arcLength[i];
								}
								averageLength = lineLength / (arcStep + 0.0);
								//求能量
								for (int i = 0; i < arcStep; i++)
								{

									double e = abs(arcLength[i] - averageLength) * abs(arcLength[i] - averageLength);
									Energy[(m1 - 1) * 59049 + (n1 - 1) * 6561 + (m2 - 1) * 729 + (n2 - 1) * 81 + (m3 - 1) * 9 + n3 - 1] += e;

								}
								lineLength = 0.0;
							}
							//	TCHAR str[100];
							//	TRACE("%d    ",(m1-1)*59049+(n1-1)*6561+(m2-1)*729+(n2-1)*81+(m3-1)*9+n3-1);
							//	sprintf(str,"%0.5f   ",Energy[(m1-1)*59049+(n1-1)*6561+(m2-1)*729+(n2-1)*81+(m3-1)*9+n3-1]);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
							//	TRACE(str);
						}
					}
				}
			}

		}
	}

	//返回Energy[]中最小的Energy值，k确定了a的值
	double minx;
	minx = Energy[0];
	for (int x = 0; x < 1000000; x++) {
		if (minx > Energy[x] && Energy[x] != 0.0)
			minx = Energy[x];
	}

	for (int x = 0; x < 1000000; x++) {

		if (minx == Energy[x])
		{
			minimalAlpha1 = a1Energy[x];
			minimalAlpha2 = a2Energy[x];
			minimalAlpha3 = a3Energy[x];
			minimalBeta1 = b1Energy[x];
			minimalBeta2 = b2Energy[x];
			minimalBeta3 = b3Energy[x];
		}
	}
	TCHAR str[100];
	TRACE(" minimalAlpha1 \n");
	sprintf(str, "%0.5f   ", minimalAlpha1);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalAlpha2 \n");
	sprintf(str, "%0.5f   ", minimalAlpha2);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalAlpha3 \n");
	sprintf(str, "%0.5f   ", minimalAlpha3);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta1 \n");
	sprintf(str, "%0.5f   ", minimalBeta1);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta2 \n");
	sprintf(str, "%0.5f   ", minimalBeta2);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	TRACE(" minimalBeta3 \n");
	sprintf(str, "%0.5f   ", minimalBeta3);   //%f表示格式化double类型，%0.2f保留两位小数，%0.4f保留4位小数
	TRACE(str);
	DrawLines(minimalAlpha1, minimalBeta1, minimalAlpha2, minimalBeta2, minimalAlpha3, minimalBeta3);
}
void CNurbsSuface::ShowSurfaceSubdivisionCurves()
{
	::glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glColor3f(1.0, 0.0, 0.0);
	::glBegin(GL_LINES);
	for (int i = 0; i < m_uknotcount; i++)
	{

		for (int j = 0; j < 100; j++)
		{
			Point3D p1 = ComputPosition(m_uknot[i], j * 0.01);
			Point3D p2 = ComputPosition(m_uknot[i], (j + 1) * 0.01);

			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
		}
	}
	::glEnd();

	::glBegin(GL_LINES);
	for (int i = 0; i < m_vknotcount; i++)
	{

		for (int j = 0; j < 100; j++)
		{
			Point3D p1 = ComputPosition(j * 0.01, m_vknot[i]);
			Point3D p2 = ComputPosition((j + 1) * 0.01, m_vknot[i]);

			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
		}
	}
	::glEnd();
	::glEnable(GL_LIGHTING);
	glColor3f(0, 0, 0);
}
void CNurbsSuface::ShowSubSurfaceCoordinate()
{
	if (!m_showSubSurfaceCoordinate)
		return;

	//First Get the Tree View
	CMyTreeView* pTreeView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
		{
			pTreeView = (CMyTreeView*)pView;
		}
	}
	CClientDC dc(pTreeView);

	int m_orderu = m_udegree + 1;
	int m_orderv = m_vdegree + 1;
	//For each sub surface, show its coordinates.
	int unum = GetCtrlNumberU() / m_orderu;
	int vnum = GetCtrlNumberV() / m_orderv;

	for (int i = 0; i < unum; i++)
		for (int j = 0; j < vnum; j++)
		{
			Point3D p1 = GetControlPoint(i * m_orderu, j * m_orderv);
			Point3D p2 = GetControlPoint(i * m_orderu + m_orderu - 1, j * m_orderv);
			Point3D p3 = GetControlPoint(i * m_orderu, j * m_orderv + m_orderv - 1);
			Point3D p4 = GetControlPoint(i * m_orderu + m_orderu - 1, j * m_orderv + m_orderv - 1);

			Point3D prop1, prop2, prop3, prop4;

			gluProject(p1[0], p1[1], p1[2], modelview, projection, m_viewPort, &(prop1[0]), &(prop1[1]), &(prop1[2]));
			gluProject(p2[0], p2[1], p2[2], modelview, projection, m_viewPort, &(prop2[0]), &(prop2[1]), &(prop2[2]));
			gluProject(p3[0], p3[1], p3[2], modelview, projection, m_viewPort, &(prop3[0]), &(prop3[1]), &(prop3[2]));
			gluProject(p4[0], p4[1], p4[2], modelview, projection, m_viewPort, &(prop4[0]), &(prop4[1]), &(prop4[2]));

			prop1.y = m_viewPort[3] - prop1[1];
			prop2.y = m_viewPort[3] - prop2[1];
			prop3.y = m_viewPort[3] - prop3[1];
			prop4.y = m_viewPort[3] - prop4[1];

			Point3D pp = prop1 + prop2 + prop3 + prop4;
			pp = pp / 4;
			char message[80];
			sprintf(message, "(%d %d)", i, j);
			dc.TextOut(pp.x, pp.y, message);
		}
}
void CNurbsSuface::ReadReparameterizationCoefficients(std::ifstream& file)
{
	file >> alphaarraynum1 >> alphaarraynum2 >> betaarraynum1 >> betaarraynum2;

	double val;
	for (int j = 0; j < alphaarraynum2; j++)
		for (int i = 0; i < alphaarraynum1; i++)
		{
			file >> val;
			alphaarray.push_back(val);
			//file >> alphaarray[j][i];
		}

	for (int i = 0; i < betaarraynum2; i++)
		for (int j = 0; j < betaarraynum1; j++)
		{
			file >> val;
			betaarray.push_back(val);
			//file >> betaarray[i][j];
		}

}
void CNurbsSuface::ShowBilinearCoefficients()
{
	if (!m_showBilinearCoefficients)
		return;

	//First Get the Tree View
	CMyTreeView* pTreeView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
		{
			pTreeView = (CMyTreeView*)pView;
		}
	}
	CClientDC dc(pTreeView);

	int m_orderu = m_udegree + 1;
	int m_orderv = m_vdegree + 1;
	//For each sub surface, show its coordinates.
	int unum = GetCtrlNumberU() / m_orderu;
	int vnum = GetCtrlNumberV() / m_orderv;

	for (int i = 0; i < unum; i++)
		for (int j = 0; j < vnum; j++)
		{
			Point3D p1 = GetControlPoint(i * m_orderu, j * m_orderv);
			Point3D p2 = GetControlPoint(i * m_orderu + m_orderu - 1, j * m_orderv);
			Point3D p3 = GetControlPoint(i * m_orderu, j * m_orderv + m_orderv - 1);
			Point3D p4 = GetControlPoint(i * m_orderu + m_orderu - 1, j * m_orderv + m_orderv - 1);

			Point3D prop1, prop2, prop3, prop4;

			gluProject(p1[0], p1[1], p1[2], modelview, projection, m_viewPort, &(prop1[0]), &(prop1[1]), &(prop1[2]));
			gluProject(p2[0], p2[1], p2[2], modelview, projection, m_viewPort, &(prop2[0]), &(prop2[1]), &(prop2[2]));
			gluProject(p3[0], p3[1], p3[2], modelview, projection, m_viewPort, &(prop3[0]), &(prop3[1]), &(prop3[2]));
			gluProject(p4[0], p4[1], p4[2], modelview, projection, m_viewPort, &(prop4[0]), &(prop4[1]), &(prop4[2]));

			prop1.y = m_viewPort[3] - prop1[1];
			prop2.y = m_viewPort[3] - prop2[1];
			prop3.y = m_viewPort[3] - prop3[1];
			prop4.y = m_viewPort[3] - prop4[1];

			Point3D pp = (prop1 + prop2) / 2;
			/*char message[80];
			sprintf(message,"%1.1f",alphaarray[i][j + 1]);
			dc.TextOut(pp.x,pp.y,message);

			pp = (prop3 + prop4)/2;
			sprintf(message,"%1.1f",alphaarray[i][j]);
			dc.TextOut(pp.x,pp.y,message);

			pp = (prop1 + prop3)/2;
			sprintf(message,"%1.1f",betaarray[j][i + 1]);
			dc.TextOut(pp.x,pp.y,message);

			pp = (prop2 + prop4)/2;
			sprintf(message,"%1.1f",betaarray[j][i]);
			dc.TextOut(pp.x,pp.y,message);*/
		}
}
extern CMyTreeView* pTreeView;
void CNurbsSuface::BuildTriangulationList()
{
	if (!m_Modified && m_ListDone)
		return;


	// Erase last list
	::glDeleteLists(m_ListOpenGL, 1);

	m_ListOpenGL = ::glGenLists(1);
	if (m_ListOpenGL == 0)
	{
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return;
	}
	::glNewList(m_ListOpenGL, GL_COMPILE_AND_EXECUTE);

	//CImage m_Image; 
	//m_Image.LoadFromResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP5));


	//int width = m_Image.GetWidth();
	//int height = m_Image.GetHeight();

	int unum = pDoc->m_Tessellation;
	int vnum = pDoc->m_Tessellation;

	//C_NurbsSurf* pNurbsSurf = Converte_C_NurbsSurface();

	double umin, umax, vmin, vmax;

	if (ptransformation == NULL)
	{
		umin = m_uknot[0];
		umax = m_uknot[m_uknotcount - 1];
		vmin = m_vknot[0];
		vmax = m_vknot[m_vknotcount - 1];
	}
	else
	{
		umin = ptransformation->umin_Reparameterization;
		umax = ptransformation->umax_Reparameterization;
		vmin = ptransformation->vmin_Reparameterization;
		vmax = ptransformation->vmax_Reparameterization;
	}

	//::glBegin(GL_TRIANGLES);
	::glBegin(GL_LINES);
	for (int i = 0; i < unum; i++)
		for (int j = 0; j < vnum; j++)
		{
			double u0 = umin + (umax - umin) * (i + 0.0) / unum;
			double v0 = vmin + (vmax - vmin) * (j + 0.0) / vnum;
			double u1 = umin + (umax - umin) * (i + 1.0) / unum;
			double v1 = vmin + (vmax - vmin) * (j + 1.0) / vnum;

			Point3D p00 = ComputPosition(u0, v0);
			Point3D p10 = ComputPosition(u1, v0);
			Point3D p01 = ComputPosition(u0, v1);
			Point3D p11 = ComputPosition(u1, v1);

			//u0, v0
			//Vector3D vu00 = pNurbsSurf->ComputUDerivative1(u0,v0);
			//Vector3D vv00 = pNurbsSurf->ComputVDerivative1(u0,v0);
			//vu00.SetUnit();
			//vv00.SetUnit();
			//double oe00 = fabs(acos(vu00 * vv00) * 180 / PI - 90.0)/90;
			//COLORREF coloroe00 = m_Image.GetPixel(width * oe00,0);

			//u1, v0
			//Vector3D vu10 = pNurbsSurf->ComputUDerivative1(u1,v0);
			//Vector3D vv10 = pNurbsSurf->ComputVDerivative1(u1,v0);
			//vu10.SetUnit();
			//vv10.SetUnit();
			//double oe10 = fabs(acos(vu10 * vv10) * 180 / PI - 90)/90;
			//COLORREF coloroe10 = m_Image.GetPixel(width * oe10,0);

			//u0, v1
			//Vector3D vu01 = pNurbsSurf->ComputUDerivative1(u0,v1);
			//Vector3D vv01 = pNurbsSurf->ComputVDerivative1(u0,v1);
			//vu01.SetUnit();
			//vv01.SetUnit();
			//double oe01 = fabs(acos(vu01 * vv01) * 180 / PI - 90)/90;
			//COLORREF coloroe01 = m_Image.GetPixel(width * oe01,0);

			//u1, v1
			//Vector3D vu11 = pNurbsSurf->ComputUDerivative1(u1,v1);
			//Vector3D vv11 = pNurbsSurf->ComputVDerivative1(u1,v1);
			//vu11.SetUnit();
			//vv11.SetUnit();
			//double oe11 = fabs(acos(vu11 * vv11) * 180 / PI - 90)/90;
			//COLORREF coloroe11 = m_Image.GetPixel(width * oe11,0);

			//Vector3D vn00 = vu00 ^ vv00;
			//Vector3D vn10 = vu10 ^ vv10;
			//Vector3D vn01 = vu01 ^ vv01;
			//Vector3D vn11 = vu11 ^ vv11;


			double r, g, b;

			//the first triangle p00 p10 p11
			Vector3D vn00 = (p10 - p00) ^ (p11 - p00);
			vn00.SetUnit();
			Vector3D vn10 = vn00;
			Vector3D vn01 = vn00;
			Vector3D vn11 = vn00;
			::glNormal3f(vn00.x, vn00.y, vn00.z);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe00 & 0xff0000) >> 16);
				g = ((coloroe00 & 0x00ff00) >> 8);
				r = (coloroe00 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/

			if (m_ShowTexture)
				::glTexCoord2f(u0, v0);

			::glVertex3f(p00.x, p00.y, p00.z);


			::glNormal3f(vn10.x, vn10.y, vn10.z);
			if (m_ShowTexture)
				::glTexCoord2f(u1, v0);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe10 & 0xff0000) >> 16);
				g = ((coloroe10 & 0x00ff00) >> 8);
				r = (coloroe10 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/
			::glVertex3f(p10.x, p10.y, p10.z);


			::glNormal3f(vn11.x, vn11.y, vn11.z);
			if (m_ShowTexture)
				::glTexCoord2f(u1, v1);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe11 & 0xff0000) >> 16);
				g = ((coloroe11 & 0x00ff00) >> 8);
				r = (coloroe11 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/
			::glVertex3f(p11.x, p11.y, p11.z);

			//the second triangle p00 p11 p01

			::glNormal3f(vn00.x, vn00.y, vn00.z);
			if (m_ShowTexture)
				::glTexCoord2f(u0, v0);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe00 & 0xff0000) >> 16);
				g = ((coloroe00 & 0x00ff00) >> 8);
				r = (coloroe00 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/
			::glVertex3f(p00.x, p00.y, p00.z);


			::glNormal3f(vn11.x, vn11.y, vn11.z);
			if (m_ShowTexture)
				::glTexCoord2f(u1, v1);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe11 & 0xff0000) >> 16);
				g = ((coloroe11 & 0x00ff00) >> 8);
				r = (coloroe11 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/
			::glVertex3f(p11.x, p11.y, p11.z);


			::glNormal3f(vn01.x, vn01.y, vn01.z);
			if (m_ShowTexture)
				::glTexCoord2f(u0, v1);
			/*if(m_showColorOrthogonality)
			{
				b = ((coloroe01 & 0xff0000) >> 16);
				g = ((coloroe01 & 0x00ff00) >> 8);
				r = (coloroe01 & 0x0000ff);
				::glColor3ub(r,g,b);
			}*/
			::glVertex3f(p01.x, p01.y, p01.z);
		}
	::glEnd();
	::glEndList();

	m_ListDone = 1;
	m_Modified = 0;
}
void CNurbsSuface::glRenderTriangulation()
{
	if (!m_ListDone || m_Modified)
		BuildTriangulationList();

	// Search for a new list
	if (::glIsList(m_ListOpenGL) == GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
		return;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n", m_ListOpenGL);
		return;
	}
}

void CNurbsSuface::ShowAreaPreservingIsoParameterCurves_modified()
{
	for (int i = 0; i < uarraymodified.size(); i++)
	{
		for (int j = 0; j < pDoc->m_Disc2; j++)
		{
			::glBegin(GL_LINES);

			double t1 = ((GLfloat)j) / pDoc->m_Disc2;
			double t2 = ((GLfloat)j + 1) / pDoc->m_Disc2;

			double u1 = uarraymodified[i];
			double v1 = t1;
			double u2 = uarraymodified[i];
			double v2 = t2;


			//Point3D pp1 = pUInterOptiModifyCurve->ComputPosition(u1);
			//Point3D pp2 = pVInterOptiModifyCurve->ComputPosition(v1);

			//u1 = pp1[0];
			//v1 = pp2[0];

			//Point3D pp3 = pUInterOptiModifyCurve->ComputPosition(u2);
			//Point3D pp4 = pVInterOptiModifyCurve->ComputPosition(v2);

			//u2 = pp3[0];
			//v2 = pp4[0];

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
	}

	for (int i = 0; i < varraymodified.size(); i++)
	{
		for (int j = 0; j < pDoc->m_Disc2; j++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)j) / pDoc->m_Disc2;
			double s2 = ((GLfloat)j + 1) / pDoc->m_Disc2;

			double u1 = s1;
			double v1 = varraymodified[i];
			double u2 = s2;
			double v2 = varraymodified[i];


			//Point3D pp1 = pUInterOptiModifyCurve->ComputPosition(u1);
			//Point3D pp2 = pVInterOptiModifyCurve->ComputPosition(v1);

			//u1 = pp1[0];
			//v1 = pp2[0];

			//Point3D pp3 = pUInterOptiModifyCurve->ComputPosition(u2);
			//Point3D pp4 = pVInterOptiModifyCurve->ComputPosition(v2);

			//u2 = pp3[0];
			//v2 = pp4[0];

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
	}
}

void CNurbsSuface::GetBoundingBox(Vector3D& lower, Vector3D& upper)
{
	int resolution = 100;
	int i, j;

	double umin, umax, vmin, vmax;
	if (ptransformation == NULL)
	{
		umin = m_uknot[0];
		umax = m_uknot[m_uknotcount - 1];
		vmin = m_vknot[0];
		vmax = m_vknot[m_vknotcount - 1];
	}
	else
	{
		umin = ptransformation->umin_Reparameterization;
		umax = ptransformation->umax_Reparameterization;
		vmin = ptransformation->vmin_Reparameterization;
		vmax = ptransformation->vmax_Reparameterization;
	}


	//Vector3D p = ComputPosition(0,0) - ZeroP3D;
	lower.x = 1e10;
	lower.y = 1e10;
	lower.z = 1e10;
	upper.x = -1e10;
	upper.y = -1e10;
	upper.z = -1e10;

	for (i = 0; i <= resolution; i++)
		for (j = 0; j <= resolution; j++)
		{
			double u = (i + 0.0) / resolution;
			double v = (j + 0.0) / resolution;

			u = umin + (umax - umin) * u;
			v = vmin + (vmax - vmin) * v;
			/*if(ptransformation)
			{
				double u1,v1;
				ptransformation->Evaluation(u,v,u1,v1);
				u = u1;
				v = v1;
			}*/

			Vector3D p = ComputPosition(u, v) - ZeroP3D;

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
void CNurbsSuface::ShowAreaPreservingIsoParameterCurves()
{
	for (int i = 0; i < uarray.size(); i++)
	{
		for (int j = 0; j < pDoc->m_Disc2; j++)
		{
			::glBegin(GL_LINES);

			double t1 = ((GLfloat)j) / pDoc->m_Disc2;
			double t2 = ((GLfloat)j + 1) / pDoc->m_Disc2;

			double u1 = uarray[i];
			double v1 = t1;
			double u2 = uarray[i];
			double v2 = t2;

			//Point3D pp1 = pUInterOptiCurve->ComputPosition(u1);
			//Point3D pp2 = pVInterOptiCurve->ComputPosition(v1);

			//u1 = pp1[0];
			//v1 = pp2[0];

			//Point3D pp3 = pUInterOptiCurve->ComputPosition(u2);
			//Point3D pp4 = pVInterOptiCurve->ComputPosition(v2);

			//u2 = pp3[0];
			//v2 = pp4[0];

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
	}

	for (int i = 0; i < varray.size(); i++)
	{
		for (int j = 0; j < pDoc->m_Disc2; j++)
		{
			::glBegin(GL_LINES);

			double s1 = ((GLfloat)j) / pDoc->m_Disc2;
			double s2 = ((GLfloat)j + 1) / pDoc->m_Disc2;

			double u1 = s1;
			double v1 = varray[i];
			double u2 = s2;
			double v2 = varray[i];

			//Point3D pp1 = pUInterOptiCurve->ComputPosition(u1);
			//Point3D pp2 = pVInterOptiCurve->ComputPosition(v1);

			//u1 = pp1[0];
			//v1 = pp2[0];

			//Point3D pp3 = pUInterOptiCurve->ComputPosition(u2);
			//Point3D pp4 = pVInterOptiCurve->ComputPosition(v2);

			//u2 = pp3[0];
			//v2 = pp4[0];

			Point3D p1 = ComputPosition(u1, v1);
			Point3D p2 = ComputPosition(u2, v2);
			::glVertex3f(p1.x, p1.y, p1.z);
			::glVertex3f(p2.x, p2.y, p2.z);
			::glEnd();
		}
	}
}
int CNurbsSuface::glDraw()
{
	if (!m_Show)
		return 0;

	::glPushMatrix();
	glTranslated(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling, m_yScaling, m_zScaling);

	glFrontFace(GL_CW);
	//------------
	// Material

	//
	glEnable(GL_POINT_SMOOTH); // 启用反走样
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND); // 启动融合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 产生融合因子
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);// 权衡图像质量与绘制速度 


	//construct texture
	GLubyte* pTextureBits;
	GLint width, height;
	if (m_ShowTexture)
	{
		m_pDib = new CDib(m_FileTextureName);
		pTextureBits = (GLubyte*)m_pDib->GetDibBitsPtr();
		width = m_pDib->GetDibWidth();
		height = m_pDib->GetDibHeight();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTextureBits);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	//---------------
	//This is a for Bezier surface;
	glMap2f(GL_MAP2_VERTEX_4,
		0.0, 1.0, 4, m_udegree + 1, 0.0, 1.0, 800, m_vdegree + 1,
		&m_ControlPoint[0][0][0]);

	//
	GLboolean re1 = glIsEnabled(GL_MAP2_VERTEX_4);
	if (re1 == GL_INVALID_ENUM || re1 == GL_INVALID_VALUE)
	{
		int i = 0;
	}

	//----------- Show Texture----------------
	if (m_ShowTexture)
	{
		glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, (const float*)textureCntrlPnt);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_MAP2_TEXTURE_COORD_2);
	}
	//-----------End Texture--------------

	//::glDisable(GL_LIGHTING);
	glEnable(GL_MAP2_VERTEX_4);
	//glColor3f(0.0,0.0,1.0);

	glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
	::glPushMatrix();

	//Show the Coordinate Axis
	if (m_ShowCoordinateAxis)
	{
		DrawCoordinateAxis();
	}

	if (!m_SetCameraMode)
	{
		// Position / translation / scaling
		glScalef(m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());

		glRotatef(m_Transform.GetValueRotation(),
			m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z());

		glTranslatef(m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());
	}

	//store the model view & projection view.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, m_viewPort);

	//Draw the iso parameter curves.

	if (m_showSurface)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (pDoc->m_Light)
			::glEnable(GL_LIGHTING);
		else
			::glDisable(GL_LIGHTING);

		glColor3f(s_r, s_g, s_b);

		if (pDoc->m_UsingOpenglNurbsRender && !m_showColorOrthogonality)
		{
			::glEnable(GL_LIGHTING);
			GLUnurbsObj* theNurb;
			theNurb = gluNewNurbsRenderer(); // 创建一个NURBS曲面对象
			// 修改NURBS曲面对象的属性
			gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
			gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
			gluBeginSurface(theNurb); // 开始曲面绘制
			gluNurbsSurface(theNurb, m_uknotcount, m_uknot, m_vknotcount, m_vknot, 800, 4, &m_ControlPoint[0][0][0], m_udegree + 1, m_vdegree + 1, GL_MAP2_VERTEX_4); // 定义曲面的数学模型，确定其形状
			gluEndSurface(theNurb);

			glLineWidth(1.0f);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(3.0f, 6.0f);

			glEnd();
		}
		else
		{
			//::glDisable(GL_LIGHTING);
			::glEnable(GL_LIGHTING);
			glRenderTriangulation();
			//::glEnable(GL_LIGHTING);
		}

		glLineWidth(2.0f);
	}
	if (m_showIsoParameterCurves)
	{
		glColor3f(c_r, c_g, c_b);

		DrawIsoParameterCurves();
	}
	if (m_showIsoParameterCurvesGeneralBilinear)
	{
		DrawIsoParameterCurves_GeneralBilinear();
	}
	if (m_showOptimalIsoParameterCurvesNonOpitimal)
	{
		DrawIsoParameterCurvesNonOpitimal();//mobius
	}
	if (m_showOptimalIsoParameterCurvesMobiusOpitmal)
	{
		DrawIsoParameterCurvesMobiusOpitmal();//mobius
	}
	if (m_showOptimalIsoParameterCurvesMobiusLinearOpitmal)
	{
		DrawIsoParameterCurvesMobiusLinearOpitmal();//mobius+bilinear
	}
	if (m_showOptimalIsoParameterCurvesMobiusQuadraticOptimal)
	{
		DrawIsoParameterCurvesMobiusQuadraticOpitmal();//mobius+quadratic
	}
	//Draw the control points.
	if (m_showControlPointsNetwork)
	{
		DrawControlPointsNetwork();
	}
	if (m_showColorBoundary)
	{
		ShowColorBoundary();
	}
	//draw the subdivision curves.
	if (m_showSubdivisionCurves)
	{
		ShowSurfaceSubdivisionCurves();
	}

	//Draw the Area Preserving Iso-Parameter Curves.
	//Stored in the uarray & varray
	if (m_showAreaPreservingIsoParameterCurves)
	{
		ShowAreaPreservingIsoParameterCurves();
	}
	if (m_showAreaPreservingIsoParameterCurves_Modified)
	{
		ShowAreaPreservingIsoParameterCurves_modified();
	}
	::glPopMatrix();

	::glPopMatrix();
	glFrontFace(GL_CCW);

	glDisable(GL_MAP2_VERTEX_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	return 1;
}

Vector3D CNurbsSuface::GetControlPoint(int i, int j)
{
	Vector3D p1;
	p1.x = m_ControlPoint[i][j][0] / m_ControlPoint[i][j][3];
	p1.y = m_ControlPoint[i][j][1] / m_ControlPoint[i][j][3];
	p1.z = m_ControlPoint[i][j][2] / m_ControlPoint[i][j][3];
	return p1;
}
void CNurbsSuface::SetControlPoint(int i, int j, Vector3D p)
{
	m_ControlPoint[i][j][0] = p[0] * m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][1] = p[1] * m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][2] = p[2] * m_ControlPoint[i][j][3];
}
Vector4D CNurbsSuface::GetControlPoint4D(int i, int j)
{
	Vector4D p1;
	p1.x = m_ControlPoint[i][j][0];
	p1.y = m_ControlPoint[i][j][1];
	p1.z = m_ControlPoint[i][j][2];
	p1.w = m_ControlPoint[i][j][3];
	return p1;
}
double CNurbsSuface::GetWeight(int i, int j)
{
	return m_ControlPoint[i][j][3];
}

void CNurbsSuface::SetWeight(int i, int j, double w)
{
	m_ControlPoint[i][j][0] = m_ControlPoint[i][j][0] / m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][1] = m_ControlPoint[i][j][1] / m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][2] = m_ControlPoint[i][j][2] / m_ControlPoint[i][j][3];

	m_ControlPoint[i][j][3] = w;
	m_ControlPoint[i][j][0] = m_ControlPoint[i][j][0] * m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][1] = m_ControlPoint[i][j][1] * m_ControlPoint[i][j][3];
	m_ControlPoint[i][j][2] = m_ControlPoint[i][j][2] * m_ControlPoint[i][j][3];
}

C_RationalBezierCurve3D* CNurbsSuface::GetUCurve(double v)
{
	int i, j, k, r;
	Vector4D* C;
	C_RationalBezierCurve3D* curv;
	m_udegree++;
	m_vdegree++;
	curv = new C_RationalBezierCurve3D(m_udegree);

	C = new Vector4D[m_vdegree];


	j = m_udegree - 1;
	for (k = 0; k < m_udegree; k++)
	{
		for (i = 0; i < m_udegree; i++)
			C[i] = GetControlPoint4D(k, i);
		for (r = 1; r <= m_vdegree - 1; r++)
		{
			for (i = j - m_vdegree + 1; i <= j - r; i++)
				C[i] = C[i] * (m_vknot[i + m_vdegree] - v) / (m_vknot[i + m_vdegree] - m_vknot[i + r]) +
				C[i + 1] * (v - m_vknot[i + r]) / (m_vknot[i + m_vdegree] - m_vknot[i + r]);
		}
		Vector3D temp;
		temp.x = C[j - m_vdegree + 1][0] / C[j - m_vdegree + 1][3];
		temp.y = C[j - m_vdegree + 1][1] / C[j - m_vdegree + 1][3];
		temp.z = C[j - m_vdegree + 1][2] / C[j - m_vdegree + 1][3];
		curv->SetControlPoint(k, ZeroP3D + temp);
		curv->SetWeight(k, C[j - m_vdegree + 1][3]);
	}
	m_udegree--;
	m_vdegree--;
	delete C;
	return curv;
}

C_RationalBezierCurve3D* CNurbsSuface::GetVCurve(double u)
{
	int i, j, k, r;
	Vector4D* C;
	C_RationalBezierCurve3D* curv;
	m_udegree++;
	m_vdegree++;
	curv = new C_RationalBezierCurve3D(m_vdegree);

	C = new Vector4D[m_udegree];


	j = m_vdegree - 1;
	for (k = 0; k < m_vdegree; k++)
	{
		for (i = 0; i < m_vdegree; i++)
			C[i] = GetControlPoint4D(i, k);
		for (r = 1; r <= m_udegree - 1; r++)
		{
			for (i = j - m_udegree + 1; i <= j - r; i++)
				C[i] = C[i] * (m_uknot[i + m_udegree] - u) / (m_uknot[i + m_udegree] - m_uknot[i + r]) +
				C[i + 1] * (u - m_uknot[i + r]) / (m_uknot[i + m_udegree] - m_uknot[i + r]);
		}
		Vector3D temp;
		temp.x = C[j - m_udegree + 1][0] / C[j - m_udegree + 1][3];
		temp.y = C[j - m_udegree + 1][1] / C[j - m_udegree + 1][3];
		temp.z = C[j - m_udegree + 1][2] / C[j - m_udegree + 1][3];
		curv->SetControlPoint(k, ZeroP3D + temp);
		curv->SetWeight(k, C[j - m_udegree + 1][3]);
		//curv->SetControlPoint(k,ZeroP3D + C[j-m_udegree+1]);
	}
	m_udegree--;
	m_vdegree--;
	delete C;
	return curv;
}

int CNurbsSuface::GetUIntervalIndex(double u)
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
	a = m_udegree - 1;
	b = m_uknotcount - 1;
	u -= TOLER;
	if (u <= m_uknot[a])
		return(a); // Yong: 981119
	if (u >= m_uknot[b])
		return(b); // Yong: 981119
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (u <= m_uknot[c] - TOLER)
			b = c;
		else
			a = c;
	}
	//	if( (u >= m_knot_u[a]-(TOLER*2)) && (u <= m_knot_u[b]) ) // Removed by Jun-Hai Yong 2002-11-20
	return a;

	//    return -1;	// 非法值   // Removed by Jun-Hai Yong 2002-11-20
}
int CNurbsSuface::GetVIntervalIndex(double v)
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
	a = m_vdegree - 1;
	b = m_vknotcount - 1;
	v -= TOLER;
	if (v <= m_vknot[a])
		return(a); // Yong: 981119
	if (v >= m_vknot[b])
		return(b); // Yong: 981119
	while (b - a > 1)
	{
		c = (a + b) / 2;
		if (v <= m_vknot[c] - TOLER)
			b = c;
		else
			a = c;
	}
	//	if( (v >= m_knot_v[a]-(TOLER*2)) && (v <= m_knot_v[b]) )// Removed by Jun-Hai Yong 2002-11-20
	return a;

	//    return -1;	// 非法值 // Removed by Jun-Hai Yong 2002-11-20
}
void CNurbsSuface::GetInterpolationUVScale(double u, double v, double& uscale, double& vscale)
{
	//
	double uscale1, uscale2, uscale3;
	double vscale1, vscale2, vscale3;
	double w1, w2, w3;
	for (int i = 0; i < uarray.size() - 1; i++)
	{
		if (u <= uarray[i + 1])
		{
			uscale2 = (uarray[i + 1] - uarray[i]) * (uarray.size());

			if (i > 0)
			{
				uscale1 = (uarray[i] - uarray[i - 1]) * (uarray.size());
			}
			else
				uscale1 = uscale2;

			if (i < uarray.size() - 2)
			{
				uscale3 = (uarray[i + 2] - uarray[i + 1]) * (uarray.size());
			}
			else
				uscale3 = uscale2;

			double ss = (u - uarray[i]) / (uarray[i + 1] - uarray[i]);
			if (ss < 0.5)
				w1 = 0.5 - ss;
			else
				w1 = 0;

			if (ss > 0.5)
				w3 = ss - 0.5;
			else
				w3 = 0;

			w2 = 0.5;

			uscale = (uscale1 * w1 + uscale2 * w2 + uscale3 * w3) / (w1 + w2 + w3);
			break;
		}
	}

	for (int i = 0; i < varray.size() - 1; i++)
	{
		if (v <= varray[i + 1])
		{
			vscale2 = (varray[i + 1] - varray[i]) * (varray.size());

			if (i > 0)
			{
				vscale1 = (varray[i] - varray[i - 1]) * (varray.size());
			}
			else
				vscale1 = vscale2;

			if (i < varray.size() - 2)
			{
				vscale3 = (varray[i + 2] - varray[i + 1]) * (varray.size());
			}
			else
				vscale3 = vscale2;

			double ss = (v - varray[i]) / (varray[i + 1] - varray[i]);
			if (ss < 0.5)
				w1 = 0.5 - ss;
			else
				w1 = 0;

			if (ss > 0.5)
				w3 = ss - 0.5;
			else
				w3 = 0;

			w2 = 0.5;

			vscale = (vscale1 * w1 + vscale2 * w2 + vscale3 * w3) / (w1 + w2 + w3);
			break;
		}
	}
}

void CNurbsSuface::GetInterpolationModifiedUVScale(double u, double v, double& uscale, double& vscale)
{
	//

}
void CNurbsSuface::GetInterpolationModifiedUV(double& u, double& v)
{
	//
	double mint, maxt;
	Point3D pp;

	//compute the function value for the current u value.
	mint = 0;
	maxt = 1;

	while (1)
	{
		double middlet = (mint + maxt) / 2;
		pp = pUInterOptiModifyCurve->ComputPosition(middlet);
		if (pp[1] > u)
			maxt = middlet;
		else
			mint = middlet;

		if (maxt - mint < 1e-5)
			break;
	}
	pp = pUInterOptiModifyCurve->ComputPosition(mint);
	u = pp[0];

	//compute the function value for the current v value
	mint = 0;
	maxt = 1;

	while (1)
	{
		double middlet = (mint + maxt) / 2;
		pp = pVInterOptiModifyCurve->ComputPosition(middlet);
		if (pp[1] > v)
			maxt = middlet;
		else
			mint = middlet;

		if (maxt - mint < 1e-5)
			break;
	}
	pp = pVInterOptiModifyCurve->ComputPosition(mint);
	v = pp[0];
}
void CNurbsSuface::GetInterpolationUV(double& u, double& v)
{
	//
	double mint, maxt;
	Point3D pp;

	//compute the function value for the current u value.
	mint = 0;
	maxt = 1;

	while (1)
	{
		double middlet = (mint + maxt) / 2;
		pp = pUInterOptiCurve->ComputPosition(middlet);
		if (pp[1] > u)
			maxt = middlet;
		else
			mint = middlet;

		if (maxt - mint < 1e-5)
			break;
	}
	pp = pUInterOptiCurve->ComputPosition(mint);
	u = pp[0];

	//compute the function value for the current v value
	mint = 0;
	maxt = 1;

	while (1)
	{
		double middlet = (mint + maxt) / 2;
		pp = pVInterOptiCurve->ComputPosition(middlet);
		if (pp[1] > v)
			maxt = middlet;
		else
			mint = middlet;

		if (maxt - mint < 1e-5)
			break;
	}
	pp = pVInterOptiCurve->ComputPosition(mint);
	v = pp[0];
}

Vector3D CNurbsSuface::GetParaNormal(double u, double v, BOOL IsUnit)
{
	GetReparameterizedCoordinates(u, v);
	C_NurbsSurf* pSurface = Converte_C_NurbsSurface();

	Vector3D normal = pSurface->GetParaNormal(u, v, IsUnit);
	return normal;
}

Point3D CNurbsSuface::ComputPosition(double u, double v)
{
	GetReparameterizedCoordinates(u, v);
	int			i, k, r, ju, jv, mu, mv;
	//------------------------------------------------------------------
	//Yupiqiang 2004/04/08
	double		/*E[4], F[4],*/* E, * F, fz1, fz2, fm;
	Vector3D* C, * D; //C[4], D[4];
	m_vdegree++;
	m_udegree++;
	E = new double[m_vdegree];
	C = new Vector3D[m_vdegree];
	F = new double[m_udegree];
	D = new Vector3D[m_udegree];
	//-----------------------------------------------------------------


	jv = GetVIntervalIndex(v);
	ju = GetUIntervalIndex(u);

	//ju = m_udegree - 1;
	//if(jv > m_vdegree - 1)
	//	jv = m_vdegree - 1;
	//if bezier & reparameterization 
	//just for debug and show
	//ju = jv = 3;

	mv = jv - m_vdegree + 1;
	mu = ju - m_udegree + 1;

	for (k = mu; k <= ju; k++)
	{
		for (i = mv; i <= jv; i++)
		{
			E[i - mv] = GetWeight(k, i);
			C[i - mv] = (GetControlPoint(k, i)) * E[i - mv];
		}
		for (r = 1; r <= m_vdegree - 1; r++)
		{
			for (i = mv; i <= jv - r; i++)
			{
				fz1 = m_vknot[i + m_vdegree] - v;
				fz2 = v - m_vknot[i + r];
				fm = m_vknot[i + m_vdegree] - m_vknot[i + r];
				E[i - mv] = (E[i - mv] * fz1 + E[i - mv + 1] * fz2) / fm;
				C[i - mv] = (C[i - mv] * fz1 + C[i - mv + 1] * fz2) / fm;
			}
		}
		F[k - mu] = E[0];
		D[k - mu] = C[0];
	}

	for (r = 1; r <= m_udegree - 1; r++)
	{
		for (i = mu; i <= ju - r; i++)
		{
			fz1 = m_uknot[i + m_udegree] - u;
			fz2 = u - m_uknot[i + r];
			fm = m_uknot[i + m_udegree] - m_uknot[i + r];
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
	m_vdegree--;
	m_udegree--;
	return ZeroP3D + vec;
	//---------------------------------------------------------------------
}
void CNurbsSuface::OptimalReparameterization()
{
}
CNurbsSuface* CNurbsSuface::reverse()
{
	CNurbsSuface* pNurbs = new CNurbsSuface();
	pNurbs->m_uknotcount = m_uknotcount;
	pNurbs->m_udegree = m_udegree;
	pNurbs->m_vdegree = m_vdegree;
	pNurbs->m_vknotcount = m_vknotcount;
	pNurbs->m_uknot = new float[m_uknotcount];
	for (int i = 0; i < m_uknotcount; i++)
		pNurbs->m_uknot[i] = m_uknot[m_uknotcount - 1 - i];
	pNurbs->m_uknot = new float[m_vknotcount];
	for (int i = 0; i < m_vknotcount; i++)
		pNurbs->m_vknot[i] = m_vknot[m_vknotcount - 1 - i];
	for (int i = 0; i < m_uknotcount - m_udegree - 1; i++)
		for (int j = 0; j < m_vknotcount - m_vdegree - 1; j++)
		{
			pNurbs->m_ControlPoint[i][j][0] = m_ControlPoint[m_uknotcount - m_udegree - 2 - i][m_vknotcount - m_vdegree - 2 - j][0];
			pNurbs->m_ControlPoint[i][j][1] = m_ControlPoint[m_uknotcount - m_udegree - 2 - i][m_vknotcount - m_vdegree - 2 - j][1];
			pNurbs->m_ControlPoint[i][j][2] = m_ControlPoint[m_uknotcount - m_udegree - 2 - i][m_vknotcount - m_vdegree - 2 - j][2];
			pNurbs->m_ControlPoint[i][j][3] = m_ControlPoint[m_uknotcount - m_udegree - 2 - i][m_vknotcount - m_vdegree - 2 - j][3];
		}
	return pNurbs;
}
int CNurbsSuface::GetCtrlNumberV()
{
	return m_vknotcount - m_vdegree - 1;
}
int CNurbsSuface::GetCtrlNumberU()
{
	return m_uknotcount - m_udegree - 1;
}

CNurbsSuface* CNurbsSuface::VSplitSurface(double vmin, double vmax)
{
	//首先将现有的CNurbs转化成C_NurbsSurf. 然后插入节点，接着将
	//插入节点后的曲面转化为CNurbs曲面。
	double* m_uknot1 = new double[m_uknotcount];
	double* m_vknot1 = new double[m_vknotcount];

	int j, k;
	for (k = 0; k <= m_uknotcount; k++)
		m_uknot1[k] = m_uknot[k];
	for (k = 0; k <= m_vknotcount; k++)
		m_vknot1[k] = m_vknot[k];
	Point3D* ctrl1 = new Point3D[(m_uknotcount - m_udegree - 1) * (m_vknotcount - m_vdegree - 1)];
	double* weight1 = new double[(m_uknotcount - m_udegree - 1) * (m_vknotcount - m_vdegree - 1)];
	for (j = 0; j < (m_uknotcount - m_udegree - 1); j++)
		for (k = 0; k < (m_vknotcount - m_vdegree - 1); k++)
		{
			ctrl1[j + k * (m_uknotcount - m_udegree - 1)][0] = m_ControlPoint[j][k][0] / m_ControlPoint[j][k][3];
			ctrl1[j + k * (m_uknotcount - m_udegree - 1)][1] = m_ControlPoint[j][k][1] / m_ControlPoint[j][k][3];
			ctrl1[j + k * (m_uknotcount - m_udegree - 1)][2] = m_ControlPoint[j][k][2] / m_ControlPoint[j][k][3];
			weight1[j + k * (m_uknotcount - m_udegree - 1)] = m_ControlPoint[j][k][3];
		}

	C_NurbsSurf* pNurbsSurf = new C_NurbsSurf(m_uknotcount - m_udegree - 1, m_vknotcount - m_vdegree - 1, m_udegree + 1,
		m_vdegree + 1, m_uknot1, m_vknot1, ctrl1, weight1);

	pNurbsSurf->InsertMultiVKnot(m_vdegree, vmin);
	pNurbsSurf->InsertMultiVKnot(m_vdegree, vmax);

	CNurbsSuface* pNurbsSurf1 = new CNurbsSuface();
	pNurbsSurf1->m_udegree = m_udegree;
	pNurbsSurf1->m_vdegree = m_vdegree;
	pNurbsSurf1->m_uknotcount = m_uknotcount;
	pNurbsSurf1->m_vknotcount = m_vknotcount;
	pNurbsSurf1->m_uknot = new float[m_uknotcount];
	pNurbsSurf1->m_vknot = new float[m_vknotcount];
	for (j = 0; j < m_uknotcount; j++)
		pNurbsSurf1->m_uknot[j] = m_uknot[j];
	for (j = 0; j < m_vknotcount; j++)
	{
		pNurbsSurf1->m_vknot[j] = m_vknot[j];
	}
	//截取中间的控制顶点
	for (j = 0; j < m_uknotcount - m_udegree - 1; j++)
		for (k = m_vknotcount - m_vdegree - 2; k < m_vknotcount - 1; k++)
		{

			Point3D pp = pNurbsSurf->GetControlPoint(j, k);
			double  wt = pNurbsSurf->GetWeight(j, k);

			//
			pNurbsSurf1->m_ControlPoint[j][k - (m_vknotcount - m_vdegree - 2)][0] = pp.x * wt;
			pNurbsSurf1->m_ControlPoint[j][k - (m_vknotcount - m_vdegree - 2)][1] = pp.y * wt;
			pNurbsSurf1->m_ControlPoint[j][k - (m_vknotcount - m_vdegree - 2)][2] = pp.z * wt;
			pNurbsSurf1->m_ControlPoint[j][k - (m_vknotcount - m_vdegree - 2)][3] = wt;
		}
	return pNurbsSurf1;
}
int CNurbsSuface::PickTest(PickStruct& pickstruct)
{
	return -1;
}
void CNurbsSuface::GenerateSamplingMesh(CMesh3d*& pMesh3D, CMesh3d*& pMesh2D, int samplesm, int samplesn)
{
	C_NurbsSurf* pNurbsSurface = Converte_C_NurbsSurface();
	//sampling density
	//int samplesm = 100;
	//int samplesn = 100;

	pMesh3D = new CMesh3d();
	pMesh2D = new CMesh3d();

	int i, j;
	double minu = -0.0;
	double maxu = 1.0;
	double minv = -0.0;
	double maxv = 1.0;

	for (j = 0; j <= samplesn; j++)
	{
		for (i = 0; i <= samplesm; i++)
		{	//

			double s = minu + (double)i * (maxu - minu) / samplesm;
			double t = minv + (double)j * (maxv - minv) / samplesn;

			double u, v;
			if (ptransformation)
			{
				s = s * (ptransformation->umax_Reparameterization - ptransformation->umin_Reparameterization) + ptransformation->umin_Reparameterization;
				t = t * (ptransformation->vmax_Reparameterization - ptransformation->vmin_Reparameterization) + ptransformation->vmin_Reparameterization;

				ptransformation->Evaluation(s, t, u, v);
			}
			else
			{
				double umin = pNurbsSurface->GetUKnot(0);
				double umax = pNurbsSurface->GetUKnot(pNurbsSurface->GetCtrlNumberU() + pNurbsSurface->GetOrderU() - 1);

				double vmin = pNurbsSurface->GetVKnot(0);
				double vmax = pNurbsSurface->GetVKnot(pNurbsSurface->GetCtrlNumberV() + pNurbsSurface->GetOrderV() - 1);

				s = umin + (umax - umin) * s;
				t = vmin + (vmax - vmin) * t;

				u = s;
				v = t;
			}

			Vector3D vv1 = (pNurbsSurface->ComputPosition(u, v) - ZeroP3D);

			//
			CVertex3d* pVertex3D = new CVertex3d(vv1.x, vv1.y, vv1.z);
			CVertex3d* pVertex2D = new CVertex3d(u, v, 0);

			pMesh3D->AddVertex(pVertex3D);
			pMesh2D->AddVertex(pVertex2D);
		}
	}

	for (j = 0; j < samplesn; j++)
	{
		for (i = 0; i < samplesm; i++)
		{
			//
			//outfile << "f " << (samplesn + 1) * j + i + 1 <<" " << (samplesn + 1) * j + i + 2 << " "<< (samplesn + 1) * j + i + 2 + samplesn + 1 << " "<< (samplesn + 1) * j + i + 1 + samplesn + 1 << endl;
			int index1 = (samplesn + 1) * j + i;
			int index2 = (samplesn + 1) * j + i + 1;
			int index3 = (samplesn + 1) * j + i + 1 + samplesn;
			int index4 = (samplesn + 1) * j + i + 2 + samplesn;
			pMesh3D->AddFace(new CFace3d(pMesh3D->GetVertex(index1), pMesh3D->GetVertex(index2), pMesh3D->GetVertex(index3)));
			pMesh3D->AddFace(new CFace3d(pMesh3D->GetVertex(index2), pMesh3D->GetVertex(index4), pMesh3D->GetVertex(index3)));
			pMesh2D->AddFace(new CFace3d(pMesh2D->GetVertex(index1), pMesh2D->GetVertex(index2), pMesh2D->GetVertex(index3)));
			pMesh2D->AddFace(new CFace3d(pMesh2D->GetVertex(index2), pMesh2D->GetVertex(index4), pMesh2D->GetVertex(index3)));
		}
	}

	pMesh3D->CalculateNormalPerFace();
	pMesh3D->CalculateNormalPerVertex();
}