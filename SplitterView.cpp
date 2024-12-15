// SplitterView.cpp : implementation file

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "MyTreeView.h"  // LeftPane View
#include "SplitterView.h" 
#include "FormView3.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSplitterView, CView)

CSplitterView::CSplitterView()
{
	m_pWndSplitter = NULL;
	m_bShouldSetXColumn = true;
	m_bInitialized = false;
	type=3;
}
/////////////////////////////////////////////////////////////////////////////
CSplitterView::~CSplitterView()
{
	TRACE("destructing CSplitterView\n");	
	m_pWndSplitter = NULL;
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSplitterView, CView)
	//{{AFX_MSG_MAP(CSplitterView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
void CSplitterView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}
/////////////////////////////////////////////////////////////////////////////
// CSplitterView diagnostics
#ifdef _DEBUG
void CSplitterView::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
//////////////////////////////////////////////////////////////////////
int CSplitterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Create the main splitter view. There are two panes (2 columns, 1 row).
	//The left pane is a tree view, the right pane is itself a splitter view.
	m_wndSplitter.CreateStatic(this, 1, 2);
	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;
	lpCreateStruct->style |= WS_OVERLAPPED;

	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CFormView3 ),  CSize(800,1100), pContext);
    m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CMyTreeView), CSize(800,100), pContext);

	m_pWndSplitter = &m_wndSplitter;
	((CMainFrame*)GetParentFrame())->m_pNewView = this;  // could also use AfxGetMainWnd()
	
	return 0; // OnCreate must return 0 to continue the creation of the CWnd object
}
//////////////////////////////////////////////////////////////////////
void CSplitterView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_wndSplitter.MoveWindow(0, 0, cx, cy);

	//We just want to set the X column upon creation of the view. This way the user can  
	//move the splitter bar to how they like it and still resize the frame window 
	//without it snapping back:
	if(m_bShouldSetXColumn) 
		m_wndSplitter.SetColumnInfo(0, cx/4, 0);

	m_wndSplitter.RecalcLayout();  
}
//////////////////////////////////////////////////////////////////////
void CSplitterView::OnInitialUpdate() 
{
	//Because of the structure of this app, this function can be called more than once. 
	//The following flag insures the code after is only run once:
	if(m_bInitialized)
		return;
	m_bInitialized = true;

	for(int i = 0; i<2; i++) //2 vertical panes (the right pane happens to have sub panes)
	{
		CView* pView = (CView*)m_wndSplitter.GetPane(0, i);
		pView->OnInitialUpdate();
	}

	TRACE("in CSplitterView::OnInitialUpdate()\n"); 
}
//////////////////////////////////////////////////////////////////////
