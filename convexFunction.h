#pragma once


// ConvexFunction dialog

class ConvexFunction : public CDialog
{
	DECLARE_DYNAMIC(ConvexFunction)

public:
	ConvexFunction(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConvexFunction();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
