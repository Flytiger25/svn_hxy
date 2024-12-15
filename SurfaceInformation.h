#pragma once


// SurfaceInformation dialog
class CNurbsSuface;
class SurfaceInformation : public CDialog
{
	DECLARE_DYNAMIC(SurfaceInformation)

public:
	CObject3d* pSurface;
	SurfaceInformation(CWnd* pParent = NULL);   // standard constructor
	virtual ~SurfaceInformation();

	// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CString m_surfaceinformation;
};
