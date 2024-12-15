#pragma once


// RenderMode dialog

class RenderMode : public CDialog
{
	DECLARE_DYNAMIC(RenderMode)

public:
	RenderMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~RenderMode();

	void OnInitialUpdate();
// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_UsingOpenglNurbsRender;
	int m_Tessellation;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
