#pragma once
#include <afxdlgs.h>
class CNurbsCurve;
#include <Eigen/Dense>
class C_NurbsCurve3D;
class Action_SDLParameterization :
	public CPropertyPage
{
	DECLARE_DYNAMIC(Action_SDLParameterization)

public:
	Action_SDLParameterization();
	virtual ~Action_SDLParameterization();


	//
	void ComputeReferenceLine(CNurbsCurve* pNurbsCurve, int index, Eigen::Vector3d mnc);

	void ValidateParameterization(CNurbsCurve* pNurbsCurve);

	//
	double DotProductUnitofInput(Vector3D v1, Vector3D v2);

	//
	double SearchForEndPoint(C_NurbsCurve3D* pCurve, Point3D startpoint, Point3D middlepoint, Vector3D middletangent, double startpara, double middlepara);

	//
	void AddReferenceLine(CNurbsCurve* pNurbsCurve, Point3D startpoint, Point3D endpoint);

	double referenceratio;

	// Dialog Data
	enum { IDD = IDD_DIALOG_SDL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMRClickObjects(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickObjects(NMHDR* pNMHDR, LRESULT* pResult);

	CSliderCtrl m_RefereceRatioSlider;
	//增加按钮
	afx_msg void LoadBezierExample1();
	afx_msg void CalculateSDLCoefficient();
	afx_msg void CalculateOptimalSDL();
	afx_msg void CalculateEpsilonOptimalSDL();
};

