// QuadraticTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "QuadraticTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "Quadratictransformation.h"
#include "Mytreeview.h"
// QuadraticTab dialog

IMPLEMENT_DYNAMIC(QuadraticTab, CPropertyPage)

QuadraticTab::QuadraticTab()
	: CPropertyPage(QuadraticTab::IDD)
{
	/*m_Quadratic_Alpha1 = 50;
	m_Quadratic_Alpha2 = 50;
	m_Quadratic_Alpha3 = 50;

	m_Quadratic_Beta1 = 50;
	m_Quadratic_Beta2 = 50;
	m_Quadratic_Beta3 = 50;*/
	//face
	//m_Quadratic_Alpha1 = 29;
	//m_Quadratic_Alpha2 = 25;
	//m_Quadratic_Alpha3 = 46;
	//m_Quadratic_Beta1 = 74;
	//m_Quadratic_Beta2 = 0;
	//m_Quadratic_Beta3 = 36;
	//bone
	//m_Quadratic_Alpha1 = 26;
	//m_Quadratic_Alpha2 = 38;
	//m_Quadratic_Alpha3 = 26;
	//m_Quadratic_Beta1 = 90;
	//m_Quadratic_Beta2 = 18;
	//m_Quadratic_Beta3 = 54;
	//cartop
	//m_Quadratic_Alpha1 = 58;
	//m_Quadratic_Alpha2 = 49;
	//m_Quadratic_Alpha3 = 30;
	//m_Quadratic_Beta1 = 71;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;
	//venus sophia
	//m_Quadratic_Alpha1 = 62;
	//m_Quadratic_Alpha2 = 53;
	//m_Quadratic_Alpha3 = 28;
	//m_Quadratic_Beta1 = 54;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;

	//sophia
	//m_Quadratic_Alpha1 = 74;
	//m_Quadratic_Alpha2 = 53;
	//m_Quadratic_Alpha3 = 16;
	//m_Quadratic_Beta1 = 54;
	//m_Quadratic_Beta2 = 21;
	//m_Quadratic_Beta3 = 54;

	//terrain
	m_Quadratic_Alpha1 = 91;
	m_Quadratic_Alpha2 = 58;
	m_Quadratic_Alpha3 = 5;
	m_Quadratic_Beta1 = 49;
	m_Quadratic_Beta2 = 8;
	m_Quadratic_Beta3 = 49;
}

QuadraticTab::~QuadraticTab()
{
}

void QuadraticTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_Quadratic_Alpha1, m_Quadratic_Alpha1);
	DDX_Slider(pDX, IDC_Quadratic_Alpha2, m_Quadratic_Alpha2);
	DDX_Slider(pDX, IDC_Quadratic_Alpha3, m_Quadratic_Alpha3);
	DDX_Slider(pDX, IDC_Quadratic_Beta1, m_Quadratic_Beta1);
	DDX_Slider(pDX, IDC_Quadratic_Beta2, m_Quadratic_Beta2);
	DDX_Slider(pDX, IDC_Quadratic_Beta3, m_Quadratic_Beta3);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QuadraticTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void QuadraticTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);

	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_SceneGraph.NbObject() == 0)
		return;

	CTreeCtrl* pTreeControl = (CTreeCtrl*)p_FormView3->objTab.GetDlgItem(IDC_Objects);

	vector<HTREEITEM>::iterator treeitem;
	treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),pTreeControl->GetSelectedItem());

	int index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();
	if(index < 0 || index >= pDoc->m_SceneGraph.NbObject())
		index = 0;

	if(pDoc->m_SceneGraph.GetAt(index)->GetType() != TYPE_NURBSSURFACE)
		return;

	CNurbsSuface* pSurf = (CNurbsSuface*)pDoc->m_SceneGraph.GetAt(index);
	pSurf->m_Modified = TRUE;

	QuadraticTransformation* quadratictrans;
	if(!pSurf->ptransformation)
	{
		quadratictrans = new QuadraticTransformation();
		pSurf->ptransformation = quadratictrans;

	}else 
	{
		if(pSurf->ptransformation->m_ReparameterizationType != quadratic)
		{
			delete pSurf->ptransformation;
			quadratictrans = new QuadraticTransformation();
			pSurf->ptransformation = quadratictrans;
		}else
		{
			quadratictrans = (QuadraticTransformation*)pSurf->ptransformation;
		}
	}

	quadratictrans->m_ReparameterizationType = quadratic;
	quadratictrans->umin_Reparameterization = pSurf->m_uknot[0];
	quadratictrans->umax_Reparameterization = pSurf->m_uknot[pSurf->m_uknotcount - 1];
	quadratictrans->vmin_Reparameterization = pSurf->m_vknot[0];
	quadratictrans->vmax_Reparameterization = pSurf->m_vknot[pSurf->m_vknotcount - 1];

	quadratictrans->umin = pSurf->m_uknot[0];
	quadratictrans->umax = pSurf->m_uknot[pSurf->m_uknotcount - 1];
	quadratictrans->vmin = pSurf->m_vknot[0];
	quadratictrans->vmax = pSurf->m_vknot[pSurf->m_vknotcount - 1];
	quadratictrans->m_Quadratic_Alpha1 = m_Quadratic_Alpha1 / 100.0;
	quadratictrans->m_Quadratic_Alpha2 = m_Quadratic_Alpha2 / 100.0;
	quadratictrans->m_Quadratic_Alpha3 = m_Quadratic_Alpha3 / 100.0;
	quadratictrans->m_Quadratic_Beta1 = m_Quadratic_Beta1 / 100.0;
	quadratictrans->m_Quadratic_Beta2 = m_Quadratic_Beta2 / 100.0;
	quadratictrans->m_Quadratic_Beta3 = m_Quadratic_Beta3 / 100.0;

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
// QuadraticTab message handlers
