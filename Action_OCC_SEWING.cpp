#include "stdafx.h"
#include "Action_OCC_SEWING.h"

IMPLEMENT_DYNAMIC(Action_occ_sewing, CPropertyPage)
Action_occ_sewing::Action_occ_sewing()
{

}

Action_occ_sewing::~Action_occ_sewing()
{

}

void Action_occ_sewing::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_Omino_SplitAngleSlider, m_SplitAngleSlider);
	//DDX_Control(pDX, IDC_SplitDirectionCHECK1, m_SplitDirection);
	//DDX_Control(pDX, IDC_Omino_MergeAngleSlider, m_OminoMergeAngle);
	//DDX_Control(pDX, IDC_Omino_SloppingEdgeSubdivision, m_EdgeSubdivision);
	//DDX_Control(pDX, IDC_Omino_SmallLandmarkLengthSlider, m_MergeSmallLandmarkLength);
	//DDX_Control(pDX, IDC_Omino_MergeSmallAngleSlider, m_MergeSmallLandmarkAngle);
}

BEGIN_MESSAGE_MAP(Action_occ_sewing, CPropertyPage)
	//ON_BN_CLICKED(IDC_GENERATELANDMARKS, &Action_PolyOmino::OnBnClickedGeneratelandmarks)
END_MESSAGE_MAP()

void Action_occ_sewing::OnBnClickedFixGap()
{
	AfxMessageBox("asd");
}

