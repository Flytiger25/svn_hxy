#pragma once
#include "afxcmn.h"
#include "Actions_General.h"
#include "Actions_GeneralBilinear.h"
#include "Actions_Bilinear.h"
#include "Action_Render.h"
#include "Action_LeastSquare.h"
#include "Actions_Tspline.h"
#include "Action_AreaPreserving.h"
#include "Action_LandMark.h"
#include "Actions_RicciFlow.h"
#include "Actions_HarmonicMapping.h"
#include "Action_PolyOmino.h"
#include "Action_PolyOmino2.h"
#include "Action_Levy_Leastsquare.h"
#include "Actions_TeichmullerMap.h"
#include "Actions_ConstrainedRegistration.h"
#include "Actions_DynamicQuasiconformal.h"
//#include "Actions_DataAnalysis.h"
#include "Actions_EdgeSwap.h"
#include "Action_Tooth.h"
#include "Actions_BoolOperation.h"
// Actions dialog

class ActionsTab : public CPropertyPage
{
	DECLARE_DYNAMIC(ActionsTab)

public:
	ActionsTab();
	virtual ~ActionsTab();

// Dialog Data
	enum { IDD = IDD_ActionsTab };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
	//CMFCTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
