// MainFrm.cpp : implementation of the CMainFrame class

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "SplitterView.h"  //double paned vertical splitter (the right pane also has sub-panes)
#include "MyTreeView.h"
#include "MainFrm.h"
#include "urlmon.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_FORMVIEW1, ID_VIEW_SPLITTER, OnChooseView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_FORMVIEW1, ID_VIEW_SPLITTER, OnUpdateViewMenuUI)
	ON_COMMAND(ID_ADD_FORM_VIEW, OnAddFormView)
	ON_COMMAND(ID_REMOVE_FORM_VIEW, OnRemoveFormView)
	ON_UPDATE_COMMAND_UI(ID_ADD_FORM_VIEW, OnUpdateAddFormView)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_FORM_VIEW, OnUpdateRemoveFormView)
	ON_MESSAGE(WM_MESSAGE, notifyMessage)
	ON_MESSAGE(WM_SETAREA, setArea)
	ON_MESSAGE(WM_UPDATE_TREECONTROL, notifyTreeControl)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame()
{
	m_pNewView = NULL;
	m_nCurrentView = ID_VIEW_SPLITTER;

	m_bShowingFormView3InRightPaneHorizSplitter = false;
}
/////////////////////////////////////////////////////////////////////////////
CMainFrame::~CMainFrame()
{
	TRACE("destructing CMainFrame\n");
}
/////////////////////////////////////////////////////////////////////////////
//the following function is passed into EnumChildWindows() used in CMainFrame::OnChooseView()
BOOL CALLBACK CMainFrame::MyWndEnumProc(HWND hWnd, LPARAM ppWndLPARAM)
{
	CWnd* pWndChild = CWnd::FromHandlePermanent(hWnd);
	CWnd** ppWndTemp = (CWnd**)ppWndLPARAM;

	if (pWndChild && pWndChild->IsKindOf(RUNTIME_CLASS(CView)))
	{
		//any view that is found will work:
		*ppWndTemp = pWndChild; //pass ptr to found CView back to calling function
		return FALSE; //stop enumeration 
	}
	else
	{
		*ppWndTemp = NULL;
		return TRUE;  //continue enumeration
	}
}

LRESULT CMainFrame::setArea(WPARAM wParam, LPARAM lParam)
{
	//pPolyOmino->OnBnClickedPolyominoSetArea();

	return 1;
}
LRESULT CMainFrame::notifyMessage(WPARAM wParam, LPARAM lParam)
{
	// change state bar to display finished
	// and the view
	TRACE((char*)lParam);
	m_wndStatusBar.SetPaneText(0, _T((char*)lParam));
	//m_Progress.DestroyWindow();

	return 1;
}

LRESULT CMainFrame::notifyTreeControl(WPARAM wParam, LPARAM lParam)
{
	// change state bar to display finished
	// and the view
	CSDIViewSwitchDoc* pDoc = (CSDIViewSwitchDoc*)GetActiveDocument();
	pDoc->UpdateAllViews(NULL);
	pDoc->UpdateTreeControl();
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//This function displays a view that is chosen from the toolbar or menu. There 
//are five types to choose from: FormView1, FormView2, EditView, ListView, and SplitterView
void CMainFrame::OnChooseView(UINT nCmdID)
{
	if (nCmdID == m_nCurrentView)
		return; //view is already selected, so don't need to destroy it and then create it again

	CSDIViewSwitchDoc* pDoc = ((CSDIViewSwitchApp*)AfxGetApp())->m_pDoc;
	if (!pDoc)  //most likely a problem reading in file
	{
		AfxMessageBox("A problem occurred. Try to open a new file. If that"
			"doesn't work, please close the application and run it again");
		return;
	}

	CWnd* pWnd;
	CWnd* pWndToDelete;

	//The use of EnumChildWindows below is to find a child window that is derived from CView.
	//GetActiveView() will return NULL if no view has focus, so we can't use it here.
	//GetActiveDocument() also can't be used because it calls GetActiveView().
	::EnumChildWindows(m_hWnd, MyWndEnumProc, (LPARAM) & (pWnd));

	if (pWnd == NULL)
	{
		AfxMessageBox("Problem: CMainFrame::OnChooseView can not find any views!");
		return;
	}

	//Now that we've found a view, search up the parent tree until we come to the CMainFrame 
	//window. Then set pWndToDelete to the CWnd that is the immediate child of CMainFrame. 
	//This is done to find the most senior view (which may or may not have children views). 
	//CMainFrame also has children, such as the toolbar, that we don't want to delete.
	while (lstrcmp(pWnd->GetRuntimeClass()->m_lpszClassName, "CMainFrame"))
	{
		pWndToDelete = pWnd;
		pWnd = pWnd->GetParent();
	}

	CRuntimeClass* pNewViewRTClass;

	pNewViewRTClass = RUNTIME_CLASS(CSplitterView);

	//Create the new view:
	CCreateContext context;
	context.m_pNewViewClass = pNewViewRTClass;
	context.m_pCurrentDoc = pDoc;

	m_pNewView = STATIC_DOWNCAST(CView, CreateView(&context));
	if (m_pNewView != NULL)
	{
		m_nCurrentView = nCmdID;

		//Make sure that the document won't be destroyed when the view is destroyed.
		//m_bAutoDelete is a public, but non-documented member of CDocument.
		pDoc->m_bAutoDelete = FALSE;

		//destroy the old view:
		pWndToDelete->DestroyWindow();

		//OnInitialUpdate() is not called as a result of calling CreateView() above. 
		//It is not always called by the framework, so it is called here:
		m_pNewView->OnInitialUpdate();

		//Show and activate view:
		m_pNewView->ShowWindow(SW_SHOW);
		SetActiveView(m_pNewView);
		RecalcLayout();
	}
	else
	{
		AfxMessageBox("problem creating a view in CMainFrame::OnChooseView()");
	}
}
HWND g_hwndMain;
char Post_Message[1000];
/////////////////////////////////////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//set the application's caption bar:
	//SetWindowText("CAD Software Package 1.0 by XJTU Geometric Center");
	SetWindowText("CAD");

	m_wndStatusBar.SetPaneInfo(0, ID_STATE_TEXT, SBPS_NOBORDERS, 2000);

	//set the global Hwnd
	g_hwndMain = GetSafeHwnd();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE; //gets rid of "Untitled" in the frame window caption
	BOOL bReturn = CFrameWnd::PreCreateWindow(cs);

	//CFrameWnd::PreCreateWindow(cs) sets some extended styles that need to be removed 
	//in this app. One of them is WS_EX_OVERLAPPEDWINDOW. If this style isn't removed,
	//there will be a window border problem.
	cs.dwExStyle &= ~WS_EX_OVERLAPPEDWINDOW;

	return bReturn;
}
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateViewMenuUI(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrentView);
}
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnAddFormView()
{
	//the following if statement should always be true, but it's here just as a safeguard:
	return;
}
////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveFormView()
{
	//the following if statement should always be true, but it's here just as a safeguard:
	return;
}
////////////////////////////////////////////////////////////////////////////////////
//enables or disables the "+" button, or the corresponding "Add Form View 3" menu item:
void CMainFrame::OnUpdateAddFormView(CCmdUI* pCmdUI)
{
	//The only time this button should be visible is when m_nCurrentView == ID_VIEW_SPLITTER, 
	//so checking this condition is just a safeguard.
	pCmdUI->Enable(m_nCurrentView == ID_VIEW_SPLITTER &&
		!m_bShowingFormView3InRightPaneHorizSplitter);
}
////////////////////////////////////////////////////////////////////////////////////
//enables or disables the "-" button, or the corresponding "Remove Form View 3" menu item:
void CMainFrame::OnUpdateRemoveFormView(CCmdUI* pCmdUI)
{
	//The only time this button should be visible is when m_nCurrentView == ID_VIEW_SPLITTER, 
	//so checking this condition is just a safeguard.
	pCmdUI->Enable(m_nCurrentView == ID_VIEW_SPLITTER &&
		m_bShowingFormView3InRightPaneHorizSplitter);
}
/////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::AlterFrameForFormView3(bool bShowingRightPaneHorizSplitterView)
{
	//We are not adding and removing all view specific menu items and toolbar buttons. The only view 
	//specific items that are being added and removed are the buttons and menu items that
	//show and remove FormView3.For example, the menu items and toolbar buttons that pertain to the 
	//editview (undo, cut, copy and paste) are left there for all views. This is the usual approach.
	//They are just grayed out when their respective view is not being displayed, or does not 
	//have the focus.

	if (m_hWnd == NULL)
		return;

	CToolBarCtrl& toolbarCtrl = m_wndToolBar.GetToolBarCtrl();
	//In destroying the frame window, the toolbar is destroyed before the view, so must check:
	if (toolbarCtrl.m_hWnd == NULL)
		return;

	CMenu* pMainMenu = GetMenu();
	if (pMainMenu == NULL || pMainMenu->m_hMenu == NULL)
		return;
	CMenu* pViewPopupMenu = pMainMenu->GetSubMenu(2);

	if (bShowingRightPaneHorizSplitterView)
	{
		//show menu items:
		pViewPopupMenu->InsertMenu(4, MF_BYPOSITION, MF_SEPARATOR);
		pViewPopupMenu->InsertMenu(6, MF_BYPOSITION, ID_REMOVE_FORM_VIEW, "   &Remove Form View 3");
		pViewPopupMenu->InsertMenu(6, MF_BYPOSITION, ID_ADD_FORM_VIEW, "   &Add       Form View 3");

		//show toolbar buttons:
		toolbarCtrl.HideButton(ID_ADD_FORM_VIEW, FALSE);
		toolbarCtrl.HideButton(ID_REMOVE_FORM_VIEW, FALSE);
	}
	else
	{
		//remove menu items:
		pViewPopupMenu->DeleteMenu(4, MF_BYPOSITION);
		pViewPopupMenu->DeleteMenu(ID_ADD_FORM_VIEW, MF_BYCOMMAND);
		pViewPopupMenu->DeleteMenu(ID_REMOVE_FORM_VIEW, MF_BYCOMMAND);

		//remove toolbar buttons:
		toolbarCtrl.HideButton(ID_ADD_FORM_VIEW, TRUE);
		toolbarCtrl.HideButton(ID_REMOVE_FORM_VIEW, TRUE);
	}

	if (m_wndToolBar.IsFloating())  //toolbar is floating in it's own miniframe window:
	{
		CMiniFrameWnd* pMiniFrameWnd = (CMiniFrameWnd*)m_wndToolBar.GetParentFrame();
		pMiniFrameWnd->RecalcLayout();
		pMiniFrameWnd->ShowWindow(SW_SHOWNA);
		pMiniFrameWnd->UpdateWindow();
	}
	else //toolbar is docked to the frame
	{
		RecalcLayout();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClose()
{
	//We must make sure that this member variable of the document object (m_bAutoDelete)
	//is set to be deleted when the frame is destroyed. If we don't, there will be a 
	//memory leak when closing down the app.
	CSDIViewSwitchApp* pApp = (CSDIViewSwitchApp*)AfxGetApp();
	if (pApp && pApp->m_pDoc)
		pApp->m_pDoc->m_bAutoDelete = TRUE;

	CFrameWnd::OnClose();
}
/////////////////////////////////////////////////////////////////////////////////////////
