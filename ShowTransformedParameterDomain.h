#pragma once


// C_ShowTransformedParameterDomain dialog
class CNurbsSuface;
class C_ShowTransformedParameterDomain : public CDialog
{
	DECLARE_DYNAMIC(C_ShowTransformedParameterDomain)

public:
	C_ShowTransformedParameterDomain(CWnd* pParent = NULL);   // standard constructor
	virtual ~C_ShowTransformedParameterDomain();

	//Draw the general bilinear
	void DrawGeneralBilinear();

	//Draw the mesh.
	void DrawMeshes();

	CNurbsSuface* pSurface;
// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
