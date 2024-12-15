// ColorFormView.cpp : implementation file
//

#include "stdafx.h"
#include "ColorFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorFormView
// One can derive from CColorFormView to provide some color to formviews
// The default background color is passed in to the contsructor

IMPLEMENT_DYNAMIC(CColorFormView, CFormView)

CColorFormView::CColorFormView(UINT nIDTemplate, COLORREF clr)
	: CFormView(nIDTemplate) 
{
	m_clrFVBkgnd = clr;

	//{{AFX_DATA_INIT(CColorFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
/////////////////////////////////////////////////////////////////////////////
CColorFormView::CColorFormView(LPCTSTR lpszTemplateName, COLORREF clr) 
	: CFormView(lpszTemplateName)
{
	m_clrFVBkgnd = clr;
}
/////////////////////////////////////////////////////////////////////////////
CColorFormView::~CColorFormView()
{
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CColorFormView, CFormView)
	//{{AFX_MSG_MAP(CColorFormView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CColorFormView diagnostics
#ifdef _DEBUG
void CColorFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CColorFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
int CColorFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//initialize other colors not passed into constructor:
	m_clrEdit    = RGB(249,166,174); //peach
	m_clrListBox = RGB(198, 40,133); //maroon purple
	m_clrText    = RGB(0  ,  0,  0); //black

	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//initialize brushes:
	if(m_BrushFVBkgnd.CreateSolidBrush(m_clrFVBkgnd) && 
	   m_BrushEdit   .CreateSolidBrush(m_clrEdit   ) &&
	   m_BrushListBox.CreateSolidBrush(m_clrListBox) )
		return  0; 	
	else
		return -1;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CColorFormView::OnEraseBkgnd(CDC* pDC) 
{
	CFormView::OnEraseBkgnd(pDC);

	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_BrushFVBkgnd);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
HBRUSH CColorFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch(nCtlColor)
	{
		case CTLCOLOR_EDIT:      //Edit control
			pDC->SetBkColor(m_clrEdit);
        	pDC->SetTextColor(m_clrText);
            return m_BrushEdit; 

		case CTLCOLOR_LISTBOX:   //List-box control
			return m_BrushListBox;

		case CTLCOLOR_SCROLLBAR: //Scroll-bar control
			return m_BrushListBox;

        //other cases you can play with: 
		//case CTLCOLOR_DLG:    //Dialog box
		//case CTLCOLOR_MSGBOX: //Message box
		//case CTLCOLOR_STATIC: //Static control 
		//case CTLCOLOR_BTN:    //Button control (I don't think this case works on Win9x, or NT4+)
	}

	pDC->SetBkColor(m_clrFVBkgnd);
   	pDC->SetTextColor(m_clrText);
	return m_BrushFVBkgnd;
}
