// MobiusTab.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MobiusTab.h"
#include "FormView3.h"
#include <vector>
#include <algorithm>
#include "lib3D\base3d.h"
#include "Nurbs3D.h"
#include "mobiustransformation.h"
#include "Mytreeview.h"
// MobiusTab dialog

IMPLEMENT_DYNAMIC(MobiusTab, CPropertyPage)

MobiusTab::MobiusTab()
	: CPropertyPage(MobiusTab::IDD)
{
	m_Mobiustransformation_Alpha = 50;
	m_Mobiustransformation_Beta = 50;
}

MobiusTab::~MobiusTab()
{
}

void MobiusTab::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_Mobius_Alpha, m_Mobiustransformation_Alpha);
	DDX_Slider(pDX, IDC_Mobius_Beta, m_Mobiustransformation_Beta);
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MobiusTab, CPropertyPage)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void MobiusTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//p_FormView3->UpdateSliderPosition();
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
	MobiusTransformation* mobiustrans;
	if(!pSurf->ptransformation)
	{
		mobiustrans = new MobiusTransformation();
		pSurf->ptransformation = mobiustrans;
	}else 
	{
		if(pSurf->ptransformation->m_ReparameterizationType != mobius)
		{
			delete pSurf->ptransformation;
			mobiustrans = new MobiusTransformation();
			pSurf->ptransformation = mobiustrans;
		}else
		{
			mobiustrans = (MobiusTransformation*)pSurf->ptransformation;
		}
	}

	mobiustrans->m_ReparameterizationType = mobius;
	mobiustrans->umin_Reparameterization = 0;
	mobiustrans->umax_Reparameterization = 1;
	mobiustrans->vmin_Reparameterization = 0;
	mobiustrans->vmax_Reparameterization = 1;
	mobiustrans->m_Mobiustransformation_Alpha = m_Mobiustransformation_Alpha / 100.0;
	mobiustrans->m_Mobiustransformation_Beta = m_Mobiustransformation_Beta / 100.0;

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

// MobiusTab message handlers
