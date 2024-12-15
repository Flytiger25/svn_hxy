#pragma once


// Polyomino_Edge_Subdivision dialog

class Polyomino_Edge_Subdivision : public CDialog
{
	DECLARE_DYNAMIC(Polyomino_Edge_Subdivision)

public:
	int m_EdgeSubdivisionNum;

	Polyomino_Edge_Subdivision(CWnd* pParent = NULL);   // standard constructor
	virtual ~Polyomino_Edge_Subdivision();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDGE_SUBDIVISION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
