// urveintersection.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "urveintersection.h"
#include "math.h"

// curveintersection dialog

IMPLEMENT_DYNAMIC(curveintersection, CDialog)

curveintersection::curveintersection(CWnd* pParent /*=NULL*/)
	: CDialog(curveintersection::IDD, pParent)
	, c00(1)
	, c01(1)
	, c02(1)
	, c10(1)
	, c12(1)
	, c20(1)
	, c21(1)
	, c22(1)
{

}

curveintersection::~curveintersection()
{
}
void curveintersection::OnDraw(CDC* pDC)
{
	// TODO: add draw code here


}
void curveintersection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER1, c00);
	DDX_Slider(pDX, IDC_SLIDER2, c01);
	DDX_Slider(pDX, IDC_SLIDER4, c02);
	DDX_Slider(pDX, IDC_SLIDER6, c10);
	DDX_Slider(pDX, IDC_SLIDER5, c12);
	DDX_Slider(pDX, IDC_SLIDER7, c20);
	DDX_Slider(pDX, IDC_SLIDER8, c21);
	DDX_Slider(pDX, IDC_SLIDER9, c22);
}


BEGIN_MESSAGE_MAP(curveintersection, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER5, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER6, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER7, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER8, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER9, &curveintersection::OnNMThemeChangedSlider1)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER1, &curveintersection::OnTRBNThumbPosChangingSlider1)
END_MESSAGE_MAP()

void curveintersection::OnPaint()
{ // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	UpdateData(TRUE);

	CClientDC dc(this);
	dc.Rectangle(10,10,510,510);

	//dc.MoveTo(10,10);
	//dc.LineTo(510,10);
	//dc.LineTo(510,510);
	//dc.LineTo(10,510);
	//dc.LineTo(10,10);

	//plot one curve.
	for(int i = 0; i <= 100; i++)
	{
		double b = i * 0.01;
		double val = (c00*(1-b)*(1-b)+c01*(1-b)*b+c02*b*b)/(c20*(1-b)*(1-b)+c21*(1-b)*b+c22*b*b);
		double a = sqrt(val)/(1 + sqrt(val));
		if(i == 0)
			dc.MoveTo(a * 500 + 10,510 - b * 500);
		else
			dc.LineTo(a * 500 + 10,510 -  b * 500);
	}
	for(int i = 0; i <= 100; i++)
	{
		double a = i * 0.01;
		double val = (c00*(1-a)*(1-a)+c10*(1-a)*a+c20*a*a)/(c02*(1-a)*(1-a)+c12*(1-a)*a+c22*a*a);
		double b = sqrt(val)/(1 + sqrt(val));
		if(i == 0)
			dc.MoveTo(a * 500 + 10,510 - b * 500);
		else
			dc.LineTo(a * 500 + 10,510 - b * 500);
	}

	//CDialog::OnPaint();
}
// curveintersection message handlers

void curveintersection::OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	//Invalidate(TRUE);

	*pResult = 0;
}

void curveintersection::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	//Invalidate(TRUE);
	//CClientDC dc(this);
	//dc.Rectangle(10,10,510,510);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
