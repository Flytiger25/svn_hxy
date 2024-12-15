#pragma once


// VectorField dialog

class VectorField : public CDialog
{
	DECLARE_DYNAMIC(VectorField)

public:
	VectorField(CWnd* pParent = NULL);   // standard constructor
	virtual ~VectorField();

// Dialog Data
	enum { IDD = IDD_DIALOG_VECTORFIELD };

	std::vector<std::vector<Vector2D>> m_VectorArray;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
