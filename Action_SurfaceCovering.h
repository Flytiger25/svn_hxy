#pragma once
#include <afxdlgs.h>
#include "afxcmn.h"
#include "afxwin.h"
//#include "ap.h"
#include "resource.h"
#include <afxdlgs.h> 
#include <afx.h>
#include "Geom_BSplineCurve.hxx"
#include "gp_Pnt.hxx"
class Shape_OCC;
class Action_SurfaceCovering :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_SurfaceCovering)

public:
	Action_SurfaceCovering();
	virtual ~Action_SurfaceCovering();

	//
	void SamplingSectionCurveByArcLength(double len, Shape_OCC* pOCC, std::vector<int>& curveSectionArray, gp_Pnt& point);

	//
	void SamplingSectionCurveByArcLength(Shape_OCC* pOCC, std::vector<int>& curveSectionArray, std::vector<gp_Pnt>& curveSectionArray1_Sampling, int SamplingNum);

	//
	void Coons(const Handle(Geom_BSplineCurve)& curve1, const Handle(Geom_BSplineCurve)& curve2, const Handle(Geom_BSplineCurve)& curve3, const  Handle(Geom_BSplineCurve)& curve4);

	double ComputeMaximalDeiviation(std::vector<gp_Pnt>& curveSectionArray1_Sampling, std::vector<gp_Pnt>& curveSectionArray2_Sampling, int index);

	// Dialog Data
	enum { IDD = IDD_DIALOG_SURFACECOVERING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadTwoSurfaces();
	afx_msg void OnBnClickedFixGap();
public:
	afx_msg void OnBnClickedButtonGeneratesmallermesh();
	afx_msg void OnBnClickedButtonDetectplane();
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSurfacecovering();
	afx_msg void OnBnClickedButton1LoadBoundaryExample1();
	afx_msg void OnBnClickedButton1CoonsGeneration();
	afx_msg void OnBnClickedButton1LoadG1coonsExample1();
	afx_msg void OnBnClickedButton1LoadG1coonsExample3();
	afx_msg void OnBnClickedButton1LoadG1coonsExample2();
	afx_msg void OnBnClickedButton1G1coonsGeneration();
	afx_msg void OnBnClickedButton1Pointpaircalculation();
	afx_msg void OnBnClickedButtonBsplinecurveoffset();
};

