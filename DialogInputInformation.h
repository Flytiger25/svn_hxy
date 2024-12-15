#pragma once


// DialogInputInformation dialog

class DialogInputInformation : public CDialog
{
	DECLARE_DYNAMIC(DialogInputInformation)

public:
	DialogInputInformation(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogInputInformation();

	//
	std::vector<int> m_ArraySelectedVertex;
	std::vector<int> m_ArraySelectedEdge;

// Dialog Data
	enum { IDD = IDD_INPUTINFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddvertex();
	afx_msg void OnBnClickedButtonAddedge();
};
