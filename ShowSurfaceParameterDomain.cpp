// ShowSurfaceParameterDomain.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ShowSurfaceParameterDomain.h"
#include "Lib3D/Mesh3d.h"
#include "curve.h"
#include "Nurbs3D.h"
#include "math.h"

// CShowSurfaceParameterDomain dialog

IMPLEMENT_DYNAMIC(CShowSurfaceParameterDomain, CDialog)

CShowSurfaceParameterDomain::CShowSurfaceParameterDomain(CWnd* pParent /*=NULL*/)
	: CDialog(CShowSurfaceParameterDomain::IDD, pParent)
{

	uarray[0] = 0.612;
	varray[0] = 0.333;
	uarray[1] = 0.928;
	varray[1] = 0.265;
	uarray[2] = 0.426;
	varray[2] = 0.785;
	uarray[3] = 1.165;
	varray[3] = 0.863;
	uarray[4] = 0.811;
	varray[4] = 0.839;
	uarray[5] = 0.746;
	varray[5] = 0.304;
	uarray[6] = 0.820;
	varray[6] = 0.563;

	umin = 23;
	vmin = 30;
	width = 395;
	height = 422;

	offset = 627;
	size = 4;
	selectindex = -1;
}

CShowSurfaceParameterDomain::~CShowSurfaceParameterDomain()
{
}

void CShowSurfaceParameterDomain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CShowSurfaceParameterDomain::DrawHyperbolas()
{
	CClientDC dc(this);

		//c1[i] * s*t + c2[i]*s + c3[i]*t + c4[i] = 0;
	
}
void CShowSurfaceParameterDomain::OnPaint() 
{
	//
	CClientDC dc(this);

	dc.SetROP2(R2_XORPEN);

	//draw the paprameter domain.
	dc.MoveTo(umin,vmin);
	dc.LineTo(umin + width,vmin);
	dc.LineTo(umin + width,vmin + height);
	dc.LineTo(umin, vmin + height);
	dc.LineTo(umin,vmin);

	dc.MoveTo(umin + offset,vmin);
	dc.LineTo(umin + width  + offset,vmin);
	dc.LineTo(umin + width  + offset,vmin + height);
	dc.LineTo(umin  + offset, vmin + height);
	dc.LineTo(umin  + offset,vmin);

	//draw the seven points
	for(int i = 0; i < 7 ; i++)
		dc.Ellipse(umin + uarray[i] * width - size,vmin + (1 - varray[i]) * height - size, umin + uarray[i] * width + size,vmin + (1 - varray[i]) * height + size);

	double u1 = uarray[0];
	double v1 = varray[0];

	double u2 = uarray[1];
	double v2 = varray[1];

	double u3 = uarray[2];
	double v3 = varray[2];

	double u4 = uarray[3];
	double v4 = varray[3];

	double u5 = uarray[4];
	double v5 = varray[4];

	double u6 = uarray[5];
	double v6 = varray[5];

	double u7 = uarray[6];
	double v7 = varray[6];

	double a1 = u1*(2*u5*u2*(u3-u4)+u7*u4*(u5-2*u3+2*u2-u6)+u7*u3*(u5+u6-2*u2)+2*u3*u6*(u4-u5))+u2*(u7*u4*(u3-u6)+u3*u6*(u7-u4)+u4*u5*(u6-u7)+u7*u3*(u4-u5)+u4*u6*(u5-u3));
	double b1 = -(u1*(u3*u5*(u2-u6)+u5*u2*(u3-u4)+u4*u5*(u6-u2)+2*u7*u2*(u4-u3) +u7*u6*(u3-u4))+u2*(u5*u6*(u4-u3)+u7*u6*(u3-u4)));
	double c1 = -(u1*(u3*u5*(u2-u6)+u3*u6*(u4-u5)+u7*u3*(u5-u2)+u7*u6*(u3-u4)+u4*u7*(u2-u3)+u5*u4*(u6-u2))+u2*(u3*u5*(u6-u7)+u3*u4*(u7-u6)));
	double d1 = u1*(u2*(u3-u4)*(u5-u7)+u6*(u4-u3)*(u5-u7));
	double a2 = u1*(2*u7*(u5-u3)+u5*(u3-u4)+u6*(u3+u4-2*u5))+u2*(2*u5*(u6-u7)-u6*(u4+u3)+u5*(u3-u4)+2*u7*u4)+2*u6*(u7-u5)*(u3-u4);
	double b2 = -(u3-u4)*(u5-u7)*(u1+u2-2*u6);
	double c2 = -(u1*(u4-u5)*(u6-u7)+u2*(u5*(-u7+u3-u4+u6)+u7*(u4-u3)+u4*(u7-u6))+u6*(u7-u5)*(u3-u4));
	double d2 = (u2-u6)*(u3-u4)*(u5-u7);

	double a3 = v1*(2*v5*v2*(v3-v4)+v7*v4*(v5-2*v3+2*v2-v6)+v7*v3*(v5+v6-2*v2)+2*v3*v6*(v4-v5))+v2*(v7*v4*(v3-v6)+v3*v6*(v7-v4)+v4*v5*(v6-v7)+v7*v3*(v4-v5)+v4*v6*(v5-v3));
	double b3 = -(v1*(v3*v5*(v2-v6)+v5*v2*(v3-v4)+v4*v5*(v6-v2)+2*v7*v2*(v4-v3) +v7*v6*(v3-v4))+v2*(v5*v6*(v4-v3)+v7*v6*(v3-v4)));
	double c3 = -(v1*(v3*v5*(v2-v6)+v3*v6*(v4-v5)+v7*v3*(v5-v2)+v7*v6*(v3-v4)+v4*v7*(v2-v3)+v5*v4*(v6-v2))+v2*(v3*v5*(v6-v7)+v3*v4*(v7-v6)));
	double d3 = v1*(v2*(v3-v4)*(v5-v7)+v6*(v4-v3)*(v5-v7));
	double a4 = v1*(2*v7*(v5-v3)+v5*(v3-v4)+v6*(v3+v4-2*v5))+v2*(2*v5*(v6-v7)-v6*(v4+v3)+v5*(v3-v4)+2*v7*v4)+2*v6*(v7-v5)*(v3-v4);
	double b4 = -(v3-v4)*(v5-v7)*(v1+v2-2*v6);
	double c4 = -(v1*(v4-v5)*(v6-v7)+v2*(v5*(-v7+v3-v4+v6)+v7*(v4-v3)+v4*(v7-v6))+v6*(v7-v5)*(v3-v4));
	double d4 = (v2-v6)*(v3-v4)*(v5-v7);

	double s,t;
	s = 0;
	t = 0;
	double u11 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v11 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u1,v1,u11,v11);
	s = 1;
	t = 0;
	double u21 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v21 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u2,v2,u21,v21);
	s = 0;
	t = 1;
	double u31 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v31 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u3,v3,u31,v31);
	s = 1;
	t = 1;
	double u41 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v41 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u4,v4,u41,v41);
	s = 0.5;
	t = 1;
	double u51 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v51 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u5,v5,u51,v51);
	s = 0.5;
	t = 0;
	double u61 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v61 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u6,v6,u61,v61);
	s = 0.5;
	t = 0.5;
	double u71 = (a1 * s * t + b1 * s + c1 * t + d1)/(a2 * s * t + b2 * s + c2 * t + d2);
	double v71 = (a3 * s * t + b3 * s + c3 * t + d3)/(a4 * s * t + b4 * s + c4 * t + d4);
	TRACE("(%f,%f)  -  (%f,%f)\n",u7,v7,u71,v71);

	//first the degenerate curve 
	//a2 * s * t + b2 * s + c2 * t + d2
	//a4 * s * t + b4 * s + c4 * t + d4
	for(int i = 0; i <= 10; i++)
	{
		double s = i * 0.1;
		double t = -(b2 * s + d2)/(a2 * s +c2);
		if(i == 0)
			dc.MoveTo(umin + width * s + offset,vmin + height * (1 - t));
		else
			dc.LineTo(umin + width * s + offset,vmin + height * (1 - t));
		dc.Ellipse(umin + width * s + offset - 2,vmin + height * (1 - t) - 2,umin + width * s + offset + 2,vmin + height * (1 - t) + 2);
	}
	for(int i = 0; i <= 100; i++)
	{
		double s = i * 0.01;
		double t = -(b4 * s + d4)/(a4 * s +c4);
		if(i == 0)
			dc.MoveTo(umin + width * s + offset,vmin + height * (1 - t));
		else
			dc.LineTo(umin + width * s + offset,vmin + height * (1 - t));
		dc.Ellipse(umin + width * s + offset - 2,vmin + height * (1 - t) - 2,umin + width * s + offset + 2,vmin + height * (1 - t) + 2);
	}


	//draw the mapped parabola curves of the original domain.
	for(int i = 0; i <= 10; i++)
	{
		double s = i * 0.1;
		for(int j = 0; j <= 100; j++)
		{
			double t = j * 0.01;
			if(fabs(a2 * s * t + b2 * s + c2 * t + d2) < 1e-5 || fabs(a4 * s * t + b4 * s + c4 * t + d4) < 1e-5)
			{
				int ii = 0; 
			}
			double u = (a1 * s * t + b1 * s + c1 * t + d1) / (a2 * s * t + b2 * s + c2 * t + d2);
			double v = (a3 * s * t + b3 * s + c3 * t + d3) / (a4 * s * t + b4 * s + c4 * t + d4);
			if(j == 0)
			{
				dc.MoveTo(umin + width * u, vmin + height * (1 - v));
			}
			dc.LineTo(umin + width * u, vmin + height * (1 - v));
		}
	}

	for(int i = 0; i <= 10; i++)
	{
		double t = i * 0.1;
		for(int j = 0; j <= 100; j++)
		{
			double s = j * 0.01;
			double u = (a1 * s * t + b1 * s + c1 * t + d1) / (a2 * s * t + b2 * s + c2 * t + d2);
			double v = (a3 * s * t + b3 * s + c3 * t + d3) / (a4 * s * t + b4 * s + c4 * t + d4);
			if(fabs(a2 * s * t + b2 * s + c2 * t + d2) < 1e-5 || fabs(a4 * s * t + b4 * s + c4 * t + d4) < 1e-5)
			{
				int ii = 0; 
			}
			if(j == 0)
			{
				dc.MoveTo(umin + width * u, vmin + height * (1 - v));
			}
			dc.LineTo(umin + width * u, vmin + height * (1 - v));
		}
	}
	/*for(int i = 0; i <= 10; i++)
	{
		double s
	}*/

	//Draw the parameter domain

		//plot another curve.
		/*dc.MoveTo(0,0);
		for(int i = 0; i <= 1000; i++)
		{
			double t = i * 0.001;
			Point3D point = pInterOptiCurve->ComputPosition(t);
			dc.LineTo(point[0] * 100,point[1] * 100);
		}*/

	//Draw the parameter domain
	
	// Do not call CDialog::OnPaint() for painting messages
}

BEGIN_MESSAGE_MAP(CShowSurfaceParameterDomain, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CShowSurfaceParameterDomain message handlers

void CShowSurfaceParameterDomain::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	selectindex = -1;
	CClientDC dc(this);
	//search whether some points are selected
	for(int i = 0; i < 7; i++)
	{
		if(SQR(umin + uarray[i] * width - point.x) + SQR(vmin + (1 - varray[i]) * height - point.y) < 8)
		{
			selectindex = i;
			dc.Ellipse(umin + uarray[i] * width - size,vmin + (1 - varray[i]) * height - size, umin + uarray[i] * width + size,vmin + (1 - varray[i]) * height + size);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CShowSurfaceParameterDomain::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	if(selectindex >= 0)
	{
		dc.Ellipse(umin + uarray[selectindex] * width - size,vmin + (1 - varray[selectindex]) * height - size, umin + uarray[selectindex] * width + size,vmin + (1 - varray[selectindex]) * height + size);
		uarray[selectindex] = ((double)(point.x - umin)) / width;
		varray[selectindex] = ((double)(point.y - vmin)) / height;
		dc.Ellipse(umin + uarray[selectindex] * width - size,vmin + (1 - varray[selectindex]) * height - size, umin + uarray[selectindex] * width + size,vmin + (1 - varray[selectindex]) * height + size);
		selectindex = -1;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CShowSurfaceParameterDomain::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	if(selectindex >= 0)
	{
		dc.Ellipse(umin + uarray[selectindex] * width - size,vmin + (1 - varray[selectindex]) * height - size, umin + uarray[selectindex] * width + size,vmin + (1 - varray[selectindex]) * height + size);
		uarray[selectindex] = ((double)(point.x - umin)) / width;
		varray[selectindex] = ((double)(point.y - vmin)) / height;
		dc.Ellipse(umin + uarray[selectindex] * width - size,vmin + (1 - varray[selectindex]) * height - size, umin + uarray[selectindex] * width + size,vmin + (1 - varray[selectindex]) * height + size);
	}
	CDialog::OnMouseMove(nFlags, point);
}
