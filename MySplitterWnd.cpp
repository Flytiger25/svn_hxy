// splitter.cpp 

#include "stdafx.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SDIViewSwitchDoc.h"
#include "SDIViewSwitch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWnd)

CMySplitterWnd::CMySplitterWnd()
{
}
/////////////////////////////////////////////////////////////////////////////
CMySplitterWnd::~CMySplitterWnd()
{
	TRACE("destructing CMySplitterWnd\n");	
}
/////////////////////////////////////////////////////////////////////////////
//This function is called to replace the view in the right hand pane of the 
//splitter window. It is called by clicking on an appropriate treeview item in the 
//left hand pane, or by the functions which add and remove FormView3 
//(CMainFrame::OnAddFormView(), and CMainFrame::OnRemoveFormView() ):
BOOL CMySplitterWnd::ReplaceView(CRuntimeClass* pRTClass)
{
	TRACE("inside CMySplitterWnd::ReplaceView\n");

	//GetParentFrame()->GetActiveDocument() won't work here, 
	//because at times, there is no active view:
	CSDIViewSwitchDoc* pDoc = ((CSDIViewSwitchApp*)AfxGetApp())->m_pDoc;
	if(!pDoc)  //most likely a problem reading in file
	{
		AfxMessageBox("There was a problem reading the file, please open another one.");
		return FALSE;
	}

	CWnd* pWndRightPane = GetPane(0,1);
	if(pWndRightPane->IsKindOf(pRTClass))
	   return FALSE; //View is already selected, so don't need to destroy it and then create it again.

    //Set flag so that document will not be deleted when view is destroyed.
	//m_bAutoDelete is a non-documented member of CDocument.
	pDoc->m_bAutoDelete = FALSE;    

    //Delete old view: 
	pWndRightPane->DestroyWindow();  

	//Create new view:
	CCreateContext context;
	context.m_pNewViewClass   = pRTClass;
	context.m_pCurrentDoc     = pDoc;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView       = NULL;
	context.m_pCurrentFrame   = NULL;
	CreateView(0, 1, pRTClass, CSize(0,0), &context);

	CView* pNewView = (CView*)GetPane(0,1);
	RecalcLayout(); 
	pNewView->OnInitialUpdate();
	pNewView->SendMessage(WM_PAINT);

	return TRUE;
}
