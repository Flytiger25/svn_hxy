#pragma once

class CNurbsSuface;
class C_NurbsCurve3D;
class C_NurbsSurf;
// CShowSurfaceParameterDomain dialog

class CShowSurfaceParameterDomain : public CDialog
{
	DECLARE_DYNAMIC(CShowSurfaceParameterDomain)

public:
	CNurbsSuface* pSurface;
	C_NurbsSurf* pSurf;
	C_NurbsCurve3D* pInterOptiCurve;
	CShowSurfaceParameterDomain(CWnd* pParent = NULL);   // standard constructor
	void DrawHyperbolas();
	virtual ~CShowSurfaceParameterDomain();
	
	double umin,umax,vmin,vmax;
	double a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3,a4,b4,c4,d4;
	double uarray[7];
	double varray[7];

	int width;
	int height;
	int offset;
	int size;

	int selectindex;

// Dialog Data
	enum { IDD = IDD_DIALOG_SHOWSURFACEDOMAIN };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
