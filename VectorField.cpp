// VectorField.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "VectorField.h"
#include "math.h"

// VectorField dialog

IMPLEMENT_DYNAMIC(VectorField, CDialog)

VectorField::VectorField(CWnd* pParent /*=NULL*/)
	: CDialog(VectorField::IDD, pParent)
{

}

VectorField::~VectorField()
{
}

void VectorField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(VectorField, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// VectorField message handlers

void VectorField::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	int startx = 100;
	int starty = 100;
	double scalex = 600;
	double scaley = 600;

	double scale = 1;

	double max_length = 20;

	COLORREF color = 0x00000000;
	CPen normalpen(PS_SOLID,1,color);
	color = 0x000000FF;
	CPen redpen(PS_SOLID,1,color);
	for(int i = 0; i < m_VectorArray.size(); i++)
	{
		dc.SelectObject(&normalpen);
		Vector2D coordinate = m_VectorArray[i][0];
		Vector2D phi_s = m_VectorArray[i][1];
		Vector2D phi_t = m_VectorArray[i][2];

		int posx = startx + coordinate.x * scalex;
		int posy = starty + coordinate.y * scaley;

		Vector2D phi_s_scaled(phi_s.x, phi_s.y);
		Vector2D phi_t_scaled(phi_t.x, phi_t.y);

		if(fabs(phi_s_scaled.x) < 1e-5)
		{
			phi_s_scaled.x = phi_s_scaled.x > 0 ? 1e5 : -1e5;
		}else{
			phi_s_scaled.x = 1.0 / phi_s_scaled.x;
		}

		if(fabs(phi_s_scaled.y) < 1e-5)
		{
			phi_s_scaled.y = phi_s_scaled.y > 0 ? 1e5 : -1e5;
		}else{
			phi_s_scaled.y = 1.0 / phi_s_scaled.y;
		}

		if(fabs(phi_t_scaled.x) < 1e-5)
		{
			phi_t_scaled.x = phi_t_scaled.x > 0 ? 1e5 : -1e5;
		}else{
			phi_t_scaled.x = 1.0 / phi_t_scaled.x;
		}

		if(fabs(phi_t_scaled.y) < 1e-5)
		{
			phi_t_scaled.y = phi_t_scaled.y > 0 ? 1e5 : -1e5;
		}else
		{
			phi_t_scaled.y = 1.0 / phi_t_scaled.y;
		}


		phi_s_scaled *= scale;
		phi_t_scaled *= scale;

		double length = Length(phi_s_scaled);
		if(length > max_length)
		{
			phi_s_scaled.SetUnit();
			phi_s_scaled *= max_length;
			dc.SelectObject(&redpen);
		}
		dc.MoveTo(posx,posy);
		dc.LineTo(posx + phi_s_scaled.x,posy + phi_s_scaled.y);

		dc.SelectObject(&normalpen);
		length = Length(phi_t_scaled);
		if(length > max_length)
		{
			dc.SelectObject(&redpen);
			phi_t_scaled.SetUnit();
			phi_t_scaled *= max_length;
		}
		
		dc.MoveTo(posx,posy);
		dc.LineTo(posx + phi_t_scaled.x,posy + phi_t_scaled.y);
	}
}
