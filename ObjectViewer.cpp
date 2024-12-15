// ObjectViewer.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "ObjectViewer.h"
#include "FormView3.h"
#include <algorithm>
#include "lib3d/Base3d.h"
#include "Nurbs3D.h"
#include "MyTreeView.h"
#include "Disline2D.h"
#include "NurbsCurve.h"
#include "MyTreeview.h"
#include "Shape_Polygon.h"
#include "ReferenceLine.h"
#include <iostream>

using namespace std;
//using namespace Eigen;

// ObjectViewer dialog

IMPLEMENT_DYNAMIC(ObjectViewer, CPropertyPage)

ObjectViewer::ObjectViewer()
	: CPropertyPage(ObjectViewer::IDD)
{
	referenceratio = 0;
}

ObjectViewer::~ObjectViewer()
{
}

void ObjectViewer::DoDataExchange(CDataExchange* pDX)
{
	DDX_Slider(pDX, IDC_SLIDER1, m_slider1);
	DDX_Slider(pDX, IDC_SLIDER2, m_slider2);
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERMOVESPEED, m_MoveSpeedSlider);
	DDX_Control(pDX, IDC_SLIDERFOV, m_FovySlider);
}


BEGIN_MESSAGE_MAP(ObjectViewer, CPropertyPage)
	ON_BN_CLICKED(ID_IsoParametric_Line, &ObjectViewer::OnBnClickedIsoparametricLine)
	ON_BN_CLICKED(ID_CLEAR_OBJECT_SELECTION, &ObjectViewer::OnBnClickedClearSelection)
	ON_BN_CLICKED(ID_CLEAR_OBJECTS, &ObjectViewer::OnBnClickedClearObjects)
	ON_NOTIFY(NM_RCLICK, IDC_Objects, &ObjectViewer::OnNMRClickObjects)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_CLICK, IDC_Objects, &ObjectViewer::OnNMClickObjects)
	ON_BN_CLICKED(IDC_Texture, &ObjectViewer::OnBnClickedTexture)
	ON_BN_CLICKED(IDC_Texture, OnBnClickedTexture)
	ON_BN_CLICKED(IDC_ShowTexture, OnBnClickedCheck1)
	ON_BN_CLICKED(ID_ZOOMALL, &ObjectViewer::OnBnClickedZoomall)
	ON_BN_CLICKED(ID_UPDATE_TREECONTROL, &ObjectViewer::OnBnClickedUpdateTreecontrol)

	//增加按钮，将按钮与功能关联起来
	//ON_BN_CLICKED(ID_OPENMESH_CALCULATE_SDL_COEFFICIENTS, &ObjectViewer::CalculateSDLCoefficient)
END_MESSAGE_MAP()

void ObjectViewer::OnBnClickedClearObjects()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_SceneGraph.Free();
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}
void ObjectViewer::OnBnClickedClearSelection()
{
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	for (int i = 0; i < pDoc->m_ArraySelectObjects.size(); i++)
	{
		pDoc->m_SceneGraph.GetAt(pDoc->m_ArraySelectObjects[i])->m_IsSelected = FALSE;
	}
	pDoc->m_ArraySelectObjects.clear();

	pDoc->UpdateAllViews(NULL);
}
// ObjectViewer message handlers
extern CFormView3* p_FormView3;
void ObjectViewer::OnBnClickedIsoparametricLine()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_IsoParameterCurve = !pDoc->m_IsoParameterCurve;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

void ObjectViewer::OnNMRClickObjects(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

	CPoint screenPoint, clientpoint;
	UINT uFlag;
	GetCursorPos(&screenPoint);
	clientpoint = screenPoint;
	pTreeControl->ScreenToClient(&clientpoint);
	std::vector<HTREEITEM>::iterator treeitem;
	HTREEITEM hItem = pTreeControl->HitTest(clientpoint, &uFlag);
	if ((hItem != NULL) && ((TVHT_ONITEM & uFlag) && (TVHT_ONITEMLABEL & uFlag)))
	{
		auto itor = pDoc->m_ArrayTreeItemObjectMap.find(hItem);
		//treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		if (itor == pDoc->m_ArrayTreeItemObjectMap.end())
			return;

		pDoc->m_CurrentObjectIndex = itor->second->m_ScenegraphIndex;
		if (pDoc->m_CurrentObjectIndex >= pDoc->m_SceneGraph.NbObject() || pDoc->m_CurrentObjectIndex < 0)
		{
			pDoc->m_CurrentObjectIndex = -1;

		}
		else
		{
			if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_NURBSSURFACE)
			{
				CMenu menu;
				VERIFY(menu.LoadMenu(IDR_MENU1));
				CMenu* popup = menu.GetSubMenu(0);
				ASSERT(popup != NULL);
				popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
				popup->Detach();
			}
			else
				if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_MESH3D)
				{
					CMenu menu;
					VERIFY(menu.LoadMenu(IDR_MENU3));
					CMenu* popup = menu.GetSubMenu(0);
					ASSERT(popup != NULL);
					popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
					popup->Detach();
				}
				else
					if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_NURBSCURVE3D) {
						CMenu menu;
						VERIFY(menu.LoadMenu(IDR_MENU_NURBSCURVE));
						CMenu* popup = menu.GetSubMenu(0);
						ASSERT(popup != NULL);
						popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
						popup->Detach();
					}
					else
						if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_POLYGON)
						{
							CMenu menu;
							VERIFY(menu.LoadMenu(IDR_MENU6));
							CMenu* popup = menu.GetSubMenu(0);
							ASSERT(popup != NULL);
							popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
							popup->Detach();
						}
						else
							if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_ACIS)
							{
								CMenu menu;
								VERIFY(menu.LoadMenu(IDR_MENU7));
								CMenu* popup = menu.GetSubMenu(0);
								ASSERT(popup != NULL);
								popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
								popup->Detach();
							}
							else
								if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_ACIS_FACE)
								{
									CMenu menu;
									VERIFY(menu.LoadMenu(IDR_MENU7));
									CMenu* popup = menu.GetSubMenu(0);
									ASSERT(popup != NULL);
									popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
									popup->Detach();
								}
								else
									if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_ACIS_EDGE)
									{
										CMenu menu;
										VERIFY(menu.LoadMenu(IDR_MENU7));
										CMenu* popup = menu.GetSubMenu(0);
										ASSERT(popup != NULL);
										popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
										popup->Detach();
									}
									else
										if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_ACIS_VERTEX)
										{
											CMenu menu;
											VERIFY(menu.LoadMenu(IDR_MENU7));
											CMenu* popup = menu.GetSubMenu(0);
											ASSERT(popup != NULL);
											popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
											popup->Detach();
										}
										else
											if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC)
											{
												CMenu menu;
												VERIFY(menu.LoadMenu(IDR_MENU7));
												CMenu* popup = menu.GetSubMenu(0);
												ASSERT(popup != NULL);
												popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
												popup->Detach();
											}
											else
												if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC_FACE)
												{
													CMenu menu;
													VERIFY(menu.LoadMenu(IDR_MENU7));
													CMenu* popup = menu.GetSubMenu(0);
													ASSERT(popup != NULL);
													popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
													popup->Detach();
												}
												else
													if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC_EDGE)
													{
														CMenu menu;
														VERIFY(menu.LoadMenu(IDR_MENU7));
														CMenu* popup = menu.GetSubMenu(0);
														ASSERT(popup != NULL);
														popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
														popup->Detach();
													}
													else
														if (pDoc->m_SceneGraph.GetAt(pDoc->m_CurrentObjectIndex)->GetType() == TYPE_BREP_OCC_VERTEX)
														{
															CMenu menu;
															VERIFY(menu.LoadMenu(IDR_MENU7));
															CMenu* popup = menu.GetSubMenu(0);
															ASSERT(popup != NULL);
															popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPoint.x, screenPoint.y, AfxGetMainWnd());
															popup->Detach();
														}
		}

		//popup->GetSubMenu(1)->SetCheck

	}
}

BOOL ObjectViewer::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	m_MoveSpeedSlider.SetRange(0, 100);
	m_MoveSpeedSlider.SetPos(50);
	m_FovySlider.SetRange(0, 1000);
	m_FovySlider.SetPos(250);
	//m_RefereceRatioSlider.SetRange(0, 1000);
	//m_RefereceRatioSlider.SetPos(0);
	m_slider1 = pDoc->m_Disc1;
	m_slider2 = pDoc->m_Disc2;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
#define ZNEAR	0.1
#define	ZFAR	2000.0
extern CMyTreeView* pTreeView;
void ObjectViewer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	if (pSlider == &m_MoveSpeedSlider)
	{
		double scale = m_MoveSpeedSlider.GetPos() - 50;
		scale = (scale)*scale;
		if (m_MoveSpeedSlider.GetPos() > 50)
		{
			pTreeView->m_SpeedRotation = 1.0f * scale / (100.0);
			pTreeView->m_SpeedTranslation = 1.0f * scale / (100.0);
		}
		else
			if (m_MoveSpeedSlider.GetPos() < 50)
			{
				pTreeView->m_SpeedRotation = 1.0f / (100.0 * scale);
				pTreeView->m_SpeedTranslation = 1.0f / (100.0 * scale);
			}
			else
			{
				pTreeView->m_SpeedRotation = 1.0f / (100.0);
				pTreeView->m_SpeedTranslation = 1.0f / (100.0);
			}

	}
	else if (pSlider == &m_FovySlider)
	{
		double scale = m_FovySlider.GetPos();
		pTreeView->FOVY = scale / 10;

		pTreeView->ChangeFov();
		//pTreeView->OnSize(0,rect.Width(),rect.Height());
		//gluPerspective(scale,aspect,ZNEAR,ZFAR);
		pTreeView->Invalidate(TRUE);
	}
	//else if (pSlider == &m_RefereceRatioSlider)
	//{
	//	double scale = m_RefereceRatioSlider.GetPos();
	//	referenceratio = scale / 1000.0;
	//	CalculateSDLCoefficient();
	//}
	else
		p_FormView3->UpdateSliderPosition();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
void ObjectViewer::OnNMClickObjects(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	int num = pDoc->m_SceneGraph.NbObject();
	CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

	CPoint myPoint;
	UINT uFlag;
	GetCursorPos(&myPoint);
	pTreeControl->ScreenToClient(&myPoint);
	std::vector<HTREEITEM>::iterator treeitem;
	HTREEITEM hItem = pTreeControl->HitTest(myPoint, &uFlag);
	int index;
	if (!hItem)
		hItem = pTreeControl->GetSelectedItem();
	if ((hItem != NULL) && ((TVHT_ONITEM & uFlag) && !(TVHT_ONITEMBUTTON & uFlag) && !(TVHT_ONITEMLABEL & uFlag)))
	{
		auto itor = pDoc->m_ArrayTreeItemObjectMap.find(hItem);
		//treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		index = itor->second->m_ScenegraphIndex;

		//treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
		//index = treeitem - p_FormView3->m_ArraySurfaceItems.begin();

		if (index >= pDoc->m_SceneGraph.NbObject())
			return;

		pDoc->m_CurrentObjectIndex = index;
		CObject3d* pNurbsSurface = (CObject3d*)pDoc->m_SceneGraph.GetAt(index);

		//CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

		//int flag = pTreeControl->GetCheck(hItem);

		pNurbsSurface->m_Show = !pNurbsSurface->m_Show;
		//pTreeControl->SetCheck(hItem, pNurbsSurface->m_Show);
		POSITION pos = pDoc->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
				pView->Invalidate(TRUE);
		}
	}
	else
		if (hItem)
		{
			auto itor = pDoc->m_ArrayTreeItemObjectMap.find(hItem);
			//treeitem = find(p_FormView3->m_ArraySurfaceItems.begin(),p_FormView3->m_ArraySurfaceItems.end(),hItem);
			index = itor->second->m_ScenegraphIndex;

			if (index >= pDoc->m_SceneGraph.NbObject())
				return;

			pDoc->m_CurrentObjectIndex = index;

			////更新m_show
			//CTreeCtrl* pTreeControl = (CTreeCtrl*)GetDlgItem(IDC_Objects);

			//pDoc->m_SceneGraph.GetAt(index)->m_Show = pTreeControl->GetCheck(hItem);

			//if (pDoc->m_SceneGraph.GetAt(index)->m_Show == 0)
			//{
			//	//将所有的儿子都设置成0
			//	for (int i = 0; i < pDoc->m_SceneGraph.NbObject(); i++)
			//	{
			//		if (pDoc->m_SceneGraph.GetAt(i)->pParentObject == pDoc->m_SceneGraph.GetAt(index))
			//		{
			//			pDoc->m_SceneGraph.GetAt(i)->m_Show = 0;
			//		}
			//	}
			//}
		}

	*pResult = 0;
}
extern CString m_FileTextureName;
extern int m_ShowTexture;
void ObjectViewer::OnBnClickedTexture()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CButton* pCheckBox = (CButton*)GetDlgItem(IDC_ShowTexture);
	pDoc->ShowTexture = pCheckBox->GetCheck();
	m_ShowTexture = pDoc->ShowTexture;
	CFileDialog dialog(TRUE);
	if (dialog.DoModal())
	{
		m_FileTextureName = dialog.GetPathName();
		//m_texture1.SetWindowText(m_FileTextureName);
	}
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}

extern int m_ShowCoordinateAxis;
void ObjectViewer::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	CButton* pCheckBox = (CButton*)GetDlgItem(IDC_ShowTexture);
	pDoc->ShowTexture = pCheckBox->GetCheck();
	m_ShowTexture = pDoc->ShowTexture;

	pCheckBox = (CButton*)GetDlgItem(IDC_ShowCoordinateAxis);
	pDoc->ShowCoordinateAxis = pCheckBox->GetCheck();
	m_ShowCoordinateAxis = pDoc->ShowCoordinateAxis;

	//modify the texture flag for all the objects
	for (int i = 0; i < pDoc->m_SceneGraph.NbObject(); i++)
	{
		pDoc->m_SceneGraph[i]->m_ListDone = 0;
	}

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CMyTreeView)))
			pView->Invalidate(TRUE);
	}
}
void ObjectViewer::OnBnClickedZoomall()
{
	// TODO: Add your control notification handler code here
	pTreeView->OnZoomall();
}

void ObjectViewer::OnBnClickedUpdateTreecontrol()
{
	// TODO: Add your control notification handler code here
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)p_FormView3->GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->UpdateTreeControl();
	pDoc->UpdateAllViews(NULL);
}

