#pragma once


// curveintersection dialog

class curveintersection : public CDialog
{
	DECLARE_DYNAMIC(curveintersection)

public:
	curveintersection(CWnd* pParent = NULL);   // standard constructor
	virtual ~curveintersection();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	int c00;
	int c01;
	int c02;
	int c10;
	int c12;
	int c20;
	int c21;
	int c22;
	afx_msg void OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
};
