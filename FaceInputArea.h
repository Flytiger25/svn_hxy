#pragma once


// FaceInputArea dialog

class FaceInputArea : public CDialog
{
	DECLARE_DYNAMIC(FaceInputArea)

public:
	double area;
	FaceInputArea(CWnd* pParent = NULL);   // standard constructor
	virtual ~FaceInputArea();

// Dialog Data
	enum { IDD = IDD_SETFACEAREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
