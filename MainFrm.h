// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__CB97F4C5_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
#define AFX_MAINFRM_H__CB97F4C5_1752_11D2_B134_00C04FB9CA2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	CProgressCtrl   m_Progress;
	UINT m_nCurrentView;
	void OnChooseView(UINT nCmdID);
	void OnUpdateViewMenuUI(CCmdUI* pCmdUI);
	CView* m_pNewView;
	static BOOL CALLBACK MyWndEnumProc(HWND hWnd, LPARAM lParam);
	void AlterFrameForFormView3(bool bShowingRightPaneHorizSplitterView = true);
	bool m_bShowingFormView3InRightPaneHorizSplitter;

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMainFrame)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnAddFormView();
	afx_msg void OnRemoveFormView();
	afx_msg void OnUpdateAddFormView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveFormView(CCmdUI* pCmdUI);
	afx_msg LRESULT notifyMessage(WPARAM, LPARAM);
	afx_msg LRESULT setArea(WPARAM, LPARAM);
	afx_msg LRESULT notifyTreeControl(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CB97F4C5_1752_11D2_B134_00C04FB9CA2B__INCLUDED_)
