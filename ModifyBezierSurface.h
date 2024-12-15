#if !defined(AFX_MODIFYBEZIERSURFACE_H__B8711915_1956_48B8_99AA_A3652CEC742A__INCLUDED_)
#define AFX_MODIFYBEZIERSURFACE_H__B8711915_1956_48B8_99AA_A3652CEC742A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyBezierSurface.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ModifyBezierSurface dialog

class ModifyBezierSurface : public CDialog
{
// Construction
public:
	ModifyBezierSurface(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ModifyBezierSurface)
	enum { IDD = IDD_DIALOG3 };
	double	m_a1;
	double	m_a2;
	double	m_b1;
	double	m_b2;
	int		m_i;
	int		m_j;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModifyBezierSurface)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModifyBezierSurface)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYBEZIERSURFACE_H__B8711915_1956_48B8_99AA_A3652CEC742A__INCLUDED_)
