#pragma once


// ProjectToParameterDomain dialog
class CNurbsSuface;
class ProjectToParameterDomain : public CDialog
{
	DECLARE_DYNAMIC(ProjectToParameterDomain)

public:
	CNurbsSuface* pSurface;
	ProjectToParameterDomain(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProjectToParameterDomain();
	COLORREF m_colorarray[101][101];

// Dialog Data
	enum { IDD = IDD_ProjectBackToParameterDomain };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
