#pragma once
#include <afxdlgs.h>
#include <body.hxx>
#include <position.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Math_matrix.hxx>
/*
#include <mobius/bspl_UnifyKnots.h>
#include <mobius/cascade.h>
#include <mobius/geom_InterpolateCurve.h>
#include <mobius/geom_InterpolateMultiCurve.h>
#include <mobius/geom_SkinSurface.h>
#include <mobius/geom_UnifyBCurves.h>
#include <mobius/core_Ptr.h>
#include <mobius/core_XYZ.h>
#include <mobius/geom_BSplineSurface.h>

using namespace mobius;
*/
class Action_GORDEN :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_GORDEN)

public:
	Action_GORDEN();
	virtual ~Action_GORDEN();

	// Dialog Data
	enum { IDD = IDD_DIALOG_GORDEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMThemeChangedScrollbar2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	EDGE* create_spline_edge(const std::vector<SPAposition>& points);
	BODY* create_body_from_edges(EDGE* edges[], int num_edges);
	BODY* create_gordon_surface(BODY* v_wires[], int num_v_wires, BODY* u_wires[], int num_u_wires);
	void GeomLib_ChangeUBounds(Handle(Geom_BSplineSurface)& aSurface,
		const Standard_Real newU1,
		const Standard_Real newU2);
	void GeomLib_ChangeVBounds(Handle(Geom_BSplineSurface)& aSurface,
		const Standard_Real newV1,
		const Standard_Real newV2);
	void GeomLib_ChangeCurveBounds(Handle(Geom_BSplineCurve)& aCurve,
		const Standard_Real newU1,
		const Standard_Real newU2);
	Standard_Integer SetSameDistribution(Handle(Geom_BSplineCurve)& C1,
		Handle(Geom_BSplineCurve)& C2);

	void BuildMyGordonSurf(std::vector<Handle(Geom_BSplineCurve)> uCurves,
		std::vector<Handle(Geom_BSplineCurve)> vCurves,
		std::vector<Standard_Real>& uIsoparamParams,
		std::vector<Standard_Real>& vIsoparamParams,
		TopoDS_Face& face);

	afx_msg void OnBnClickedButtonGorden();
	afx_msg void OnBnClickedButtonCreategordon();
	void export_step_OCC(TopoDS_Shape shape, std::string filePath);
	void export_brep_OCC(TopoDS_Shape shape, std::string filePath);
	void export_step_ACIS(ENTITY* ent, std::string filePath);
	afx_msg void OnBnClickedButtonMygordon();
	afx_msg void OnBnClickedButtonGuidecoons();
	afx_msg void OnBnClickedButtonAnalysis();
	afx_msg void OnBnClickedButtonIterateguidecoons();
};

