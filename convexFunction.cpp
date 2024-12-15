// onvexFunction.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "convexFunction.h"


// ConvexFunction dialog

IMPLEMENT_DYNAMIC(ConvexFunction, CDialog)

ConvexFunction::ConvexFunction(CWnd* pParent /*=NULL*/)
	: CDialog(ConvexFunction::IDD, pParent)
{

}

ConvexFunction::~ConvexFunction()
{
}

void ConvexFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConvexFunction, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ConvexFunction message handlers

void ConvexFunction::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255)); 

	dc.MoveTo(10,500);
	dc.LineTo(500,500);
	dc.MoveTo(10,500);
	dc.LineTo(10,10);

	for(int i = 10; i< 500 - 1; i++)
	{
		double a1 = (i - 10.0) / 490;
		double b1 = 1.0/(4 * a1);

		double a2 = (i + 1 - 10.0) / 490;
		double b2 = 1.0/(4 * a2);

		if(b1 >0  && b1 < 1 && b2 > 0 && b2 < 1)
		{
			dc.MoveTo(a1 * 490 + 10,500 - 490 * b1);
			dc.LineTo(a2 * 490 + 10,500 - 490 * b2);
		}
	}

	for(int i = 10; i< 500 - 1; i++)
	{
		double a1 = (i - 10.0) / 490;
		double b1 = 1 - 1.0/(4 * a1);

		double a2 = (i + 1 - 10.0) / 490;
		double b2 = 1 - 1.0/(4 * a2);

		if(b1 >0  && b1 < 1 && b2 > 0 && b2 < 1)
		{
			dc.MoveTo(a1 * 490 + 10,500 - 490 * b1);
			dc.LineTo(a2 * 490 + 10,500 - 490 * b2);
		}
	}

	for(int i = 10; i< 500 - 1; i++)
	{
		double a1 = (i - 10.0) / 490;
		double b1 = 1.0/(4 * (1 - a1));

		double a2 = (i + 1 - 10.0) / 490;
		double b2 = 1.0/(4 * (1 - a2));

		if(b1 >0  && b1 < 1 && b2 > 0 && b2 < 1)
		{
			dc.MoveTo(a1 * 490 + 10,500 - 490 * b1);
			dc.LineTo(a2 * 490 + 10,500 - 490 * b2);
		}
	}

	for(int i = 10; i< 500 - 1; i++)
	{
		double a1 = (i - 10.0) / 490;
		double b1 = 1 - 1.0/(4 * (1 - a1));

		double a2 = (i + 1 - 10.0) / 490;
		double b2 = 1 - 1.0/(4 * (1 - a2));

		if(b1 >0  && b1 < 1 && b2 > 0 && b2 < 1)
		{
			dc.MoveTo(a1 * 490 + 10,500 - 490 * b1);
			dc.LineTo(a2 * 490 + 10,500 - 490 * b2);
		}
	}

	/*for(int i = 1; i < 500; i++)
		for(int j = 1; j < 500; j++)
		{
			double a = (i) / 500.0;
			double b = j / 500.0;
			double tt = 2*b*b-24*a*a*a*a*b-6*a*a*a*a*a*a*b+18*a*a*a*a*a*b-18*a*a*a*a*a*b*b+6*a*a*a*a*a*a*b*b+18*a*a*a*b-18*a*a*a*b*b-6*a*a*a+12*a*a*b*b+18*b*b*b*a-18*a*a*b*b*b-6*b*b*b+8*b*b*b*b-6*b*b*b*b*b+2*a*a-6*a*b*b-6*a*a*b+24*b*b*b*b*a*a+18*b*b*b*b*b*a-18*b*b*b*b*b*a*a-24*b*b*b*b*a+2*b*b*b*b*b*b-6*b*b*b*b*b*b*a+6*b*b*b*b*b*b*a*a+8*a*a*a*a-6*a*a*a*a*a+2*a*a*a*a*a*a+24*a*a*a*a*b*b;
			if(tt < 0 )
			{
				AfxMessageBox("tt");
			}
		}
	return;*/
	/*double c2 = 1;
	for(int k2 = 1; k2 < 10000; k2++)
	{
		c2 = k2;
		for(int i = 1; i < 500; i++)
			for(int j = 1; j < 500; j++)
			{
				double a = (i) / 500.0;
				double b = j / 500.0;

				if(b < 1.0 / (4 * (1 - a)) && b > 1.0 / (4 * a) && b > 1 - 1.0 / (4 * a))
				{
					double test = -496*c2*c2*a*a*a*b*b+504*c2*c2*a*a*a*b*b*b+8*a*a*a*a*a*c2*c2*b*b*b*b*b-160*a*a*a*a*c2*b*b*b*b-16*a*a*a*a*a*c2*b*b*b*b*b+184*c2*c2*a*b*b*b-424*c2*c2*a*a*b*b*b+84*c2*c2*a*b+20*c2*c2*a*b*b*b*b*b-44*c2*c2*a*a*b*b*b*b*b+4*c2*a-32*a*a*a*a*a*c2*c2*b*b-256*a*a*a*c2*c2*b*b*b*b+48*a*a*a*a*a*c2*c2*b*b*b-432*a*a*a*b*b*b*c2+272*a*a*a*a*c2*c2*b*b+152*a*a*a*a*c2*c2*b*b*b*b+416*c2*c2*a*a*b*b-204*c2*c2*a*a*b+52*a*a*a*a*a*c2*b*b*b*b-288*a*a*a*a*c2*c2*b*b*b-176*c2*c2*a*b*b-32*a*a*a*a*a*c2*c2*b*b*b*b-32*a*a*a*a*c2*c2*b*b*b*b*b+244*c2*c2*a*a*a*b+52*a*a*a*c2*c2*b*b*b*b*b+40*a*a*a*a*c2*b*b*b*b*b-96*c2*c2*a*b*b*b*b+216*c2*c2*a*a*b*b*b*b-16*c2*c2*a+40*c2*c2*a*a-48*c2*c2*a*a*a-128*c2*c2*a*a*a*a*b+44*a*a*a*a*b*b*b*b-100*b*b*b*c2*a-16*a*a*a*a*b-12*c2*a*a*a*a+4*a*a*a*a*a*b-16*a*a*a*a*a*b*b+24*a*a*a*a*a*b*b*b+4*a*b+400*a*a*a*c2*b*b+48*a*a*a*a*a*c2*b*b+96*c2*a*a*a*a*b-172*c2*a*a*b*b*b*b-20*b*b*b*b*b*c2*a-8*a*a*a*a*b*b*b*b*b+8*a*a*a*a*a*b*b*b*b*b+24*a*a*a*b-72*a*a*a*b*b-20*a*a*a*a*a*b*b*b*b+24*c2*a*a*a+3*c2*c2-36*b*b*b*c2*c2+48*a*a*b*b+8*a*a*a*a*a*c2*c2*b+304*b*b*b*c2*a*a+12*b*b*b*c2+12*b*b*b*a-48*a*a*b*b*b-16*c2*c2*b+34*c2*c2*b*b+3*b*b*b*b+19*c2*c2*b*b*b*b+4*b*b*b*b*b*a*a*a+24*c2*c2*a*a*a*a+44*b*b*b*b*b*c2*a*a-56*b*b*b*b*b*a*a*a*c2-4*c2*c2*b*b*b*b*b+92*c2*a*b*b+124*c2*a*a*b-40*c2*a*b-284*c2*a*a*b*b-12*a*b*b-16*a*a*b+4*c2*b-10*c2*b*b-16*c2*a*a+72*a*a*a*b*b*b-52*a*a*a*b*b*b*b-10*c2*b*b*b*b+40*b*b*b*b*a*a+4*b*b*b*b*b*c2-16*b*b*b*b*a+52*a*a*a*a*b*b-60*a*a*a*a*b*b*b+236*a*a*a*c2*b*b*b*b-172*a*a*a*c2*b-240*c2*a*a*a*a*b*b+64*c2*a*b*b*b*b-72*a*a*a*a*a*b*b*b*c2+276*b*b*b*c2*a*a*a*a-12*a*a*a*a*a*c2*b;
					if(test < 0 )
					{
						char message[100];
						sprintf(message,"there are not convex at some points for c2 %d",k2);
						AfxMessageBox(message);
						goto ExitLoop;
					}
				}
			}
	}
ExitLoop:
int ii = 0;*/
}
