#pragma once
#include "afxcmn.h"
#include <Eigen/Dense>

// ObjectViewer dialog
class CNurbsCurve;
class ObjectViewer : public CPropertyPage
{
	DECLARE_DYNAMIC(ObjectViewer)

public:
	ObjectViewer();
	virtual ~ObjectViewer();

	//
	void InitializeConsole();

	//
	void ComputeReferenceLine(CNurbsCurve* pNurbsCurve, int index, Eigen::Vector3d mnc);

	void ValidateParameterization(CNurbsCurve* pNurbsCurve);

	int		m_slider1;
	int		m_slider2;
	double referenceratio;

	// Dialog Data
	enum { IDD = IDD_objViewerTab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIsoparametricLine();
	afx_msg void OnNMRClickObjects(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickObjects(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedTexture();
	afx_msg void OnBnClickedCheck1();
	CSliderCtrl m_MoveSpeedSlider;
	CSliderCtrl m_FovySlider;
	//CSliderCtrl m_RefereceRatioSlider;
	afx_msg void OnBnClickedZoomall();
	afx_msg void OnBnClickedUpdateTreecontrol();
	afx_msg void OnBnClickedExtractFreeformtransformation();
	afx_msg void OnBnClickedOpenmeshClearVertexSelection();
	afx_msg void OnBnClickedClearSelection();
	afx_msg void OnBnClickedClearObjects();
	//增加按钮
	//afx_msg void CalculateSDLCoefficient();
};
